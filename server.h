#ifndef SERVER_H 
#define SERVER_H

#include <network_utils.h>

using namespace std;

class Server : public NetworkUtils
{
    private:
        int d_tcpSocketHandle;
        int d_udpSocketHandle;
        int d_adj[SIZE][SIZE];
        Name d_type;

    public:

        Server(int, const char *, int, Name);
        ~Server();
        int send();
        int receive(int[SIZE][SIZE]);
        int read(const char *);
        int display();
};

#endif
