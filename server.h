#ifndef SERVER_H 
#define SERVER_H

#include <string>
#include <network_utils.h>

using namespace std;

#define SIZE 4

enum Name 
{
    A = 0, B = 1, C = 2, D = 3
};

class Server : public NetworkUtils
{
    private:
        int d_tcpSocketHandle;
        int d_udpSocketHandle;
        int d_adj[4][4];
        Name d_type;

    public:

        Server(int, const char *, int, Name);
        ~Server();
        int send(const char *);
        int receive(char *);
        int read(const char *);
        int serialize(string *);
        int deserialize(string, int [SIZE][SIZE]);
        int display();
};

#endif
