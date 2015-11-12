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

#define SIZE 4
enum Name 
{
    A = 0, B = 1, C = 2, D = 3
};
static const char *type = "ABCD";

class NetworkUtils 
{
	public:
        int createStaticTCPSocket(const int &, int *);
		int createStaticUDPSocket(const int &, int *);
		int createDynamicTCPSocket(const int &, const char*, int *);
		int createDynamicUDPSocket(const int &, const char*, int *, struct sockaddr_in *);
		int tcpSend(const Name &, const int &, const int[SIZE][SIZE]);
		int tcpReceive(const Name &, const int &, int[SIZE][SIZE]);
		int udpSend(const Name &, const int &, const struct sockaddr_in &, const int[SIZE][SIZE]);
		int udpReceive(const Name &, const int &, int[SIZE][SIZE]);
		int serialize(const int[SIZE][SIZE], string *);
		int deserialize(string, int[SIZE][SIZE]);
        void closeSocket(const int &);
};

#endif
