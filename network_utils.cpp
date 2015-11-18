#include <iostream> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <sstream>
#include <network_utils.h>
#define MAXHOSTNAME 256

using namespace std;

int NetworkUtils::createStaticTCPSocket(const int &portNumber, int *socketHandle)
{
    /*
     * Most of this code was reused from http://www.yolinux.com/TUTORIALS/Sockets.html
     * Modifications were made were and when necessary.
     */
     
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
    socketInfo.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Use any address available to the system
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_port = htons(portNumber);      // Set port number

    // Bind the socket to a local socket address
    if(bind(*socketHandle, (struct sockaddr *) &socketInfo, sizeof(socketInfo)) < 0)
    {
        closeSocket(*socketHandle);
        cerr << "Cannot bind socket";
        exit(EXIT_FAILURE);
    }
    else
    {
        listen(*socketHandle, 1);
        cout << "The Client has TCP port number "
             << ntohs(socketInfo.sin_port) << " and IP address "
             << ((socketInfo.sin_addr.s_addr&0xFF000000) >> 24) << "."
             << ((socketInfo.sin_addr.s_addr&0xFF0000) >> 16) << "."
             << ((socketInfo.sin_addr.s_addr&0xFF00) >> 8) << "."
             << (socketInfo.sin_addr.s_addr&0xFF) << endl;
    }
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
    socketInfo.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Use any address available to the system
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
int NetworkUtils::tcpSend(const Name &server, const int &socketHandle, const int adj[SIZE][SIZE])
{
    string message;
    serialize(adj, &message);
    const char *buff = message.c_str();
    int len = strlen(buff);
    send(socketHandle, &len, sizeof(len), 0);
    send(socketHandle, buff, len, 0);
    delete buff;
    struct sockaddr_in pin, sin;
    socklen_t peerlen = sizeof(pin);
    socklen_t socklen = sizeof(sin);
    if (getpeername(socketHandle, (struct sockaddr *)&pin, &peerlen) == -1 || 
            getsockname(socketHandle, (struct sockaddr *)&sin, &socklen) == -1)
        cerr << "getpeername or getsockname error" << endl;
    else
    {
        cout << "The Server " << ::type[server] 
             << " finishes sending its neighbor information to the Client with TCP port number "
             << ntohs(pin.sin_port) << " and IP address "
             << ((pin.sin_addr.s_addr&0xFF000000) >> 24) << "."
             << ((pin.sin_addr.s_addr&0xFF0000) >> 16) << "."
             << ((pin.sin_addr.s_addr&0xFF00) >> 8) << "."
             << (pin.sin_addr.s_addr&0xFF) << endl;

        cout << "For this connection with the Client, the Server " << ::type[server] 
             << " has TCP port number "
             << ntohs(sin.sin_port) << " and IP address "
             << ((sin.sin_addr.s_addr&0xFF000000) >> 24) << "."
             << ((sin.sin_addr.s_addr&0xFF0000) >> 16) << "."
             << ((sin.sin_addr.s_addr&0xFF00) >> 8) << "."
             << (sin.sin_addr.s_addr&0xFF) << endl;
    }

    return 0;
}
int NetworkUtils::tcpReceive(const Name &server, const int &socketHandle, int adj[SIZE][SIZE])
{
    int socketConnection;
    if( (socketConnection = accept(socketHandle, NULL, NULL)) < 0)
    {
        closeSocket(socketHandle);
        cerr << "Accept Failure";
        return 0;
    }
    struct sockaddr_in pin;
    socklen_t peerlen = sizeof(pin);
    int len = 0; char buff[512];
    recvfrom(socketConnection, &len, sizeof(len), 0, NULL, NULL);
    recvfrom(socketConnection, buff, len, 0, (struct sockaddr *)&pin, &peerlen);
    buff[len] = (char) NULL;
    struct sockaddr_in sin;
    socklen_t socklen = sizeof(sin);
    if (getsockname(socketConnection, (struct sockaddr *)&sin, &socklen) == -1)
        cerr << "getsockname error" << endl;
    else
    {
        cout << "The Client receives neighbor information from the Server " << ::type[server]
         << " with TCP port number "
         << ntohs(pin.sin_port) << " and IP address "
         << ((pin.sin_addr.s_addr&0xFF000000) >> 24) << "."
         << ((pin.sin_addr.s_addr&0xFF0000) >> 16) << "."
         << ((pin.sin_addr.s_addr&0xFF00) >> 8) << "."
         << (pin.sin_addr.s_addr&0xFF) << endl;

        deserialize(buff, adj);
        cout << "The Server " << ::type[server] << " has the following neighbor information\n"
             << "Neighbor\tCost\n";
        int i = server;
        for (int j = 0; j < SIZE; j++)
            if (j != i && adj[i][j] != 0)
                cout << "server" << ::type[j] << "\t\t" << adj[i][j] << endl;

        cout << "For this connection with Server " << ::type[server]
             << ", the Client has TCP port number "
             << ntohs(sin.sin_port) << " and IP address "
             << ((sin.sin_addr.s_addr&0xFF000000) >> 24) << "."
             << ((sin.sin_addr.s_addr&0xFF0000) >> 16) << "."
             << ((sin.sin_addr.s_addr&0xFF00) >> 8) << "."
             << (sin.sin_addr.s_addr&0xFF) << endl;
    }
    return 1;
}

int NetworkUtils::udpSend(const Name &server, const int &socketHandle, 
                            const struct sockaddr_in &pin, const int adj[4][4])
{
    string message;
    serialize(adj, &message);
    const char *buff = message.c_str();
    int len = strlen(buff);
    socklen_t peerlen = sizeof (pin);
    sendto(socketHandle, &len, sizeof(len), 0, (struct sockaddr *) &pin, peerlen);
    sendto(socketHandle, buff, len, 0, (struct sockaddr *) &pin, peerlen);
    delete buff;
    //Documentation
    struct sockaddr_in sin;
    socklen_t socklen = sizeof(sin);
    connect(socketHandle, (struct sockaddr *) &pin, peerlen);
    if (getsockname(socketHandle, (struct sockaddr *)&sin, &socklen) == -1)
        cerr << "getsockname error" << endl;
    else
    {
        cout << "The Client has sent the network topology to the Server " 
             << ::type[server] << " with UDP port number "
             << ntohs(pin.sin_port) << " and IP address "
             << ((pin.sin_addr.s_addr&0xFF000000) >> 24) << "."
             << ((pin.sin_addr.s_addr&0xFF0000) >> 16) << "."
             << ((pin.sin_addr.s_addr&0xFF00) >> 8) << "."
             << (pin.sin_addr.s_addr&0xFF) << " as follows" << endl;
             
        cout << "Edge\tCost\n";
        for (int i = 0; i < SIZE; i++)
            for (int j = i + 1; j < SIZE; j++)
                if (adj[i][j] != 0)
                    cout << ::type[i] << ::type[j] << "\t" << adj[i][j] << endl;

        cout << "For this connection with the Server " << ::type[server]
             << " the Client has UDP port number "
             << ntohs(sin.sin_port) << " and IP address "
             << ((sin.sin_addr.s_addr&0xFF000000) >> 24) << "."
             << ((sin.sin_addr.s_addr&0xFF0000) >> 16) << "."
             << ((sin.sin_addr.s_addr&0xFF00) >> 8) << "."
             << (sin.sin_addr.s_addr&0xFF) << endl;
    }
    return 1;
}
int NetworkUtils::udpReceive(const Name &server, const int &socketHandle, int adj[SIZE][SIZE])
{
    int len = 0; char buff[512];
    struct sockaddr_in pin;
    socklen_t peerlen = sizeof(pin);
    recvfrom(socketHandle, &len, sizeof(len), 0, NULL, NULL);
    recvfrom(socketHandle, buff, len, 0, (struct sockaddr *)&pin, &peerlen);
    buff[len] = (char) NULL;

    struct sockaddr_in sin;
    socklen_t socklen = sizeof(sin);
    if (getsockname(socketHandle, (struct sockaddr *)&sin, &socklen) == -1)
        cerr << "getsockname error" << endl;
    else
    {
        cout << "The Server " << ::type[server] << " has received the network "
             << "topology from the Client with UDP port number "
             << ntohs(pin.sin_port) << " and IP address "
             << ((pin.sin_addr.s_addr&0xFF000000) >> 24) << "."
             << ((pin.sin_addr.s_addr&0xFF0000) >> 16) << "."
             << ((pin.sin_addr.s_addr&0xFF00) >> 8) << "."
             << (pin.sin_addr.s_addr&0xFF) << " as follows\n";

        deserialize(buff, adj);
        cout << "Edge\tCost\n";
        for (int i = 0; i < SIZE; i++)
            for (int j = i + 1; j < SIZE; j++)
                if (adj[i][j] != 0)
                    cout << ::type[i] << ::type[j] << "\t" << adj[i][j] << endl;

        cout << "For this connection with the Client, "
             << "the Server " << ::type[server] <<" has UDP port number "
             << ntohs(sin.sin_port) << " and IP address "
             << ((sin.sin_addr.s_addr&0xFF000000) >> 24) << "."
             << ((sin.sin_addr.s_addr&0xFF0000) >> 16) << "."
             << ((sin.sin_addr.s_addr&0xFF00) >> 8) << "."
             << (sin.sin_addr.s_addr&0xFF) << endl;
    }
    return 1;
}
int NetworkUtils::serialize(const int arr[SIZE][SIZE], string *serial)
{
    *serial = "";
    std::ostringstream oss;
    for (int i = 0; i < SIZE; i++)
    {
        if (i != 0)
            *serial += ";";
        for (int j = 0; j < SIZE; j++)
        {
            if (j != 0)
                *serial += ",";
            oss << arr[i][j];
            *serial += oss.str();
            oss.str(string());
        }
    }
    return 0;
}
int NetworkUtils::deserialize(string input, int array[SIZE][SIZE])
{
    istringstream ss(input);
    string token;
    int i = 0, j = 0;
    while(getline(ss, token, ';')) {
        istringstream sss(token);
        string subtoken;
        j = 0;
        while(getline(sss, subtoken, ',')) {
            array[i][j] = atoi(subtoken.c_str());
            ++j;
        }
        ++i;
    }
    return 0;
}
