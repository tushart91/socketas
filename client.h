#ifndef CLIENT_H 
#define CLIENT_H

#include <network_utils.h>

using namespace std;

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
		int send(const Name &);
		int receive(const Name &, int[SIZE][SIZE]);
		int combine(int[SIZE][SIZE]);
		int display();
		int minKey(int[], bool[]);
		void computeMST();
};

#endif
