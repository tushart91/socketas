#ifndef CLIENT_H 
#define CLIENT_H

#include <iostream>
#include <network_utils.h>

using namespace std;

class Client : public NetworkUtils
{
	private:
	   	int d_tcpSocketHandle;
	   	int d_udpSocketHandle;
	   	struct sockaddr_in d_remoteSocketInfo;

	public:
		Client(int, const char *, int);
		~Client();
		int send(const char *);
		int receive(char *);
};

#endif
