#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
using namespace std;

class NetworkUtils 
{
	public:
        int createStaticTCPSocket(const int &, int *);
		int createStaticUDPSocket(const int &, int *);
		int createDynamicTCPSocket(const int &, const char*, int *);
		int createDynamicUDPSocket(const int &, const char*, int *, struct sockaddr_in *);
		int tcpReceive(const int &, char *);
		int tcpSend(const int &, const char *);
		int udpReceive(const int &, char *);
		int udpSend(const int &, const struct sockaddr_in &, const char *);
        void closeSocket(const int &);
};

#endif
