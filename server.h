#ifndef SERVER_H 
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <network_utils.h>

using namespace std;

class Server : public NetworkUtils
{
	private:
	   	int d_tcpSocketHandle;
	   	int d_udpSocketHandle;

	public:
		Server(int, const char *, int);
		~Server();
		int send(const char *);
		int receive(char *buff);
};

#endif
