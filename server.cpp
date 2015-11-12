#include <iostream>
#include <server.h>
#include <fstream>
using namespace std;

Server::Server(int port_number, const char *remote_host, int remote_port,
                    Name server_type)
{
    createStaticUDPSocket(port_number, &d_udpSocketHandle);
    createDynamicTCPSocket(remote_port, remote_host, &d_tcpSocketHandle);

    d_type = server_type;

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            d_adj[i][j] = 0;
    cout << "The Server " << ::type[d_type] << " is up and running." << endl;
}
Server::~Server()
{
    closeSocket(d_udpSocketHandle);
    closeSocket(d_tcpSocketHandle);
}
int Server::send()
{
    return tcpSend(d_type, d_tcpSocketHandle, d_adj);
}
int Server::receive(int adj[SIZE][SIZE])
{
    return udpReceive(d_type, d_udpSocketHandle, adj);
}
int Server::read(const char *file_name)
{
    ifstream file;
    file.open (file_name);
    if (!file.is_open())
    {
        cout << "Cannot find " << file_name;
        exit(EXIT_FAILURE);
    }
    cout << "The Server " << ::type[d_type] << " has the following neighbor information:" << endl;
    cout << "Neighbor\tCost" << endl;
    string word;
    int index = 0, cost = 0;
    while (file >> word)
    {
        index = word[word.size() - 1] - 65;
        cout << word << "\t\t";
        file >> word;
        cost = atoi(word.c_str());
        cout << cost << "\n";
        d_adj[d_type][index] = cost;
        d_adj[index][d_type] = cost;
    }
    file.close();
    return 0;
}
int Server::display()
{
    cout << endl;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
            cout << d_adj[i][j] << "\t";
        cout << endl;
    }
    cout << endl;
    return 0;
}
