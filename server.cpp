#include <iostream>
#include <network_utils.h>
#include <server.h>
#include <fstream>
#include <string>
#include <sstream>
#include <errno.h>
using namespace std;

Server::Server(int port_number, const char *remote_host, int remote_port,
                    Name type)
{
    createStaticUDPSocket(port_number, &d_udpSocketHandle);
    createDynamicTCPSocket(remote_port, remote_host, &d_tcpSocketHandle);

    d_type = type;

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            d_adj[i][j] = 0;
}
Server::~Server()
{
    closeSocket(d_udpSocketHandle);
    closeSocket(d_tcpSocketHandle);
}
int Server::send(const char *buff)
{
    return tcpSend(d_tcpSocketHandle, buff);
}
int Server::receive(char *buff)
{
    return udpReceive(d_udpSocketHandle, buff);
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

    string word;
    int index = 0, cost = 0;
    while (file >> word)
    {
        index = word[word.size() - 1] - 65;
        file >> word;
        cost = atoi(word.c_str());
        d_adj[d_type][index] = cost;
        d_adj[index][d_type] = cost;
    }
    file.close();
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
}
int Server::serialize(string *serial)
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
            oss << d_adj[i][j];
            *serial += oss.str();
            oss.str(string());
        }
    }
    return 0;
}
int Server::deserialize(string input, int array[SIZE][SIZE])
{
    istringstream ss(input);
    string token;
    int i = 0, j = 0;
    while(getline(ss, token, ';'))
    {
        istringstream sss(token);
        string subtoken;
        j = 0;
        while(getline(sss, subtoken, ','))
        {
            array[i][j] = atoi(subtoken.c_str());
            ++j;
        }
        ++i;
    }
    return 0;
}
