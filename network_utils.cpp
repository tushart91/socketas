#include <iostream>	
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <network_utils.h>
#define MAXHOSTNAME 256

using namespace std;

int NetworkUtils::createStaticTCPSocket(const int &portNumber, int *socketHandle)
{
	struct sockaddr_in socketInfo;
	struct hostent *hPtr;
	char sysHost[MAXHOSTNAME+1];  // Hostname of this computer we are running on

	// Clear structure memory
	memset(&socketInfo, 0, sizeof(sockaddr_in));

	// Get system information
	gethostname(sysHost, MAXHOSTNAME);  // Get the name of this computer we are running on
	if((hPtr = gethostbyname(sysHost)) == NULL)
	{
		cerr << "System hostname misconfigured." << endl;
		exit(EXIT_FAILURE);
	}

	// Create socket
	if((*socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		closeSocket(*socketHandle);
		cerr << "Cannot create socket";
		exit(EXIT_FAILURE);
	}

	// Load system information into socket data structures
	socketInfo.sin_addr.s_addr = htonl(INADDR_ANY); // Use any address available to the system
	socketInfo.sin_family = AF_INET;
	socketInfo.sin_port = htons(portNumber);      // Set port number

	// Bind the socket to a local socket address
	if(bind(*socketHandle, (struct sockaddr *) &socketInfo, sizeof(socketInfo)) < 0)
	{
		closeSocket(*socketHandle);
		cerr << "Cannot bind socket";
		exit(EXIT_FAILURE);
	}
	listen(*socketHandle, 1);
	return 1;
}

int NetworkUtils::createStaticUDPSocket(const int &portNumber, int *socketHandle)
{
	struct sockaddr_in socketInfo;
	struct hostent *hPtr;
	char sysHost[MAXHOSTNAME+1];  // Hostname of this computer we are running on

	// Clear structure memory
	memset(&socketInfo, 0, sizeof(sockaddr_in));

	// Get system information
	gethostname(sysHost, MAXHOSTNAME);  // Get the name of this computer we are running on
	if((hPtr = gethostbyname(sysHost)) == NULL)
	{
		cerr << "System hostname misconfigured." << endl;
		exit(EXIT_FAILURE);
	}

	// Create socket
	if((*socketHandle = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		closeSocket(*socketHandle);
		cerr << "Cannot create socket";
		exit(EXIT_FAILURE);
	}

	// Load system information into socket data structures
	socketInfo.sin_addr.s_addr = htonl(INADDR_ANY); // Use any address available to the system
	socketInfo.sin_family = AF_INET;
	socketInfo.sin_port = htons(portNumber);      // Set port number

	// Bind the socket to a local socket address
	if(bind(*socketHandle, (struct sockaddr *) &socketInfo, sizeof(socketInfo)) < 0)
	{
		closeSocket(*socketHandle);
		cerr << "Cannot bind socket";
		exit(EXIT_FAILURE);
	}
	return 1;
}
int NetworkUtils::createDynamicTCPSocket(const int &remotePort, const char* remoteHost, int *socketHandle)
{
	struct sockaddr_in remoteSocketInfo;
	struct hostent *hPtr;

	memset(&remoteSocketInfo, 0, sizeof(sockaddr_in)); // Clear structure memory

	// Get system information
	if((hPtr = gethostbyname(remoteHost)) == NULL)
	{
		cerr << "System DNS name resolution not configured properly." << endl;
		cerr << "Error number: " << ECONNREFUSED << endl;
		exit(EXIT_FAILURE);
	}

	// create socket
	if((*socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cerr << "Cannot create socket";
		closeSocket(*socketHandle);
		exit(EXIT_FAILURE);
	}

	// Load system information into socket data structures
	memcpy((char *)&remoteSocketInfo.sin_addr, hPtr->h_addr, hPtr->h_length);
	remoteSocketInfo.sin_family = AF_INET;
	remoteSocketInfo.sin_port = htons((u_short)remotePort);      // Set port number

	if(connect(*socketHandle, (struct sockaddr *)&remoteSocketInfo, sizeof(sockaddr_in)) < 0)
	{
		cerr << "Cannot connect socket";
		closeSocket(*socketHandle);
		exit(EXIT_FAILURE);
	}
	return 1;
}
int NetworkUtils::createDynamicUDPSocket(const int &remotePort, const char* remoteHost, int *socketHandle,
                                        struct sockaddr_in *remoteSocketInfo)
{
	struct hostent *hPtr;

	memset(remoteSocketInfo, 0, sizeof(sockaddr_in)); // Clear structure memory

	// Get system information
	if((hPtr = gethostbyname(remoteHost)) == NULL)
	{
		cerr << "System DNS name resolution not configured properly." << endl;
		cerr << "Error number: " << ECONNREFUSED << endl;
		exit(EXIT_FAILURE);
	}

	// create socket
	if((*socketHandle = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		cerr << "Cannot create socket";
		closeSocket(*socketHandle);
		exit(EXIT_FAILURE);
	}

	// Load system information into socket data structures
	memcpy((char *)&(remoteSocketInfo->sin_addr), hPtr->h_addr, hPtr->h_length);
	remoteSocketInfo->sin_family = AF_INET;
	remoteSocketInfo->sin_port = htons((u_short)remotePort);      // Set port number

	return 1;
}
void NetworkUtils::closeSocket(const int &socketHandle)
{
	close(socketHandle);
}
int NetworkUtils::tcpSend(const int &socketHandle, const char *buff)
{
	int len = strlen(buff);
	send(socketHandle, &len, sizeof(len), 0);
	send(socketHandle, buff, len, 0);
}
int NetworkUtils::tcpReceive(const int &socketHandle, char *buff)
{

	int socketConnection;
	if( (socketConnection = accept(socketHandle, NULL, NULL)) < 0)
	{
		closeSocket(socketHandle);
		cerr << "Accept Failure";
		return 0;
	}

	//int rc = 0;  // Actual number of bytes read

	// rc is the number of characters returned.
	// Note this is not typical. Typically one would only specify the number 
	// of bytes to read a fixed header which would include the number of bytes
	// to read. See "Tips and Best Practices" below.
	int len = 0;
	recv(socketConnection, &len, sizeof(len), 0);
	recv(socketConnection, buff, len, 0);
	buff[len] = (char) NULL;        // Null terminate string
	return 1;
}
int NetworkUtils::udpSend(const int &socketHandle, const struct sockaddr_in &remoteSocketInfo, const char *buff)
{
	int len = strlen(buff);
    socklen_t addr_size = sizeof (remoteSocketInfo);
	sendto(socketHandle, &len, sizeof(len), 0, (struct sockaddr *) &remoteSocketInfo, addr_size);
	sendto(socketHandle, buff, len, 0, (struct sockaddr *) &remoteSocketInfo, addr_size);
	return 1;
}
int NetworkUtils::udpReceive(const int &socketHandle, char *buff)
{
	struct sockaddr_in recvSocketHandle;
	int len = 0;
	socklen_t recvSocketLen = (sizeof recvSocketHandle);

	recvfrom(socketHandle, &len, sizeof(len), 0, NULL, NULL);
	recvfrom(socketHandle, buff, len, 0, NULL, NULL);
	buff[len] = (char) NULL;
	return 1;
}
