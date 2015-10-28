#ifndef CLIENT_H 
#define CLIENT_H

#include <iostream>
#include <string>
#include <network_utils.h>

using namespace std;

#define SIZE 4
enum Name 
{
    A = 0, B = 1, C = 2, D = 3
};

class Client : public NetworkUtils
{
	private:
	   	int d_tcpSocketHandle;
	   	int d_udpSocketHandle[SIZE];
	   	struct sockaddr_in d_remoteSocketInfo[SIZE];
	   	int d_adj[SIZE][SIZE];

	public:
		Client(int, const char *, int[SIZE]);
		~Client();
		int send(const char *, Name);
		int receive(char *);
		int serialize(string *);
		int deserialize(string , int [SIZE][SIZE]);
		int combine(int [SIZE][SIZE]);
		int display();
		int minKey(int [], bool []);
		void computeMST();
};

#endif
