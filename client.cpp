#include <client.h>
#include <iostream>
using namespace std;

Client::Client(int port_number, const char *remote_host, int remote_port[SIZE])
{
    createStaticTCPSocket(port_number, &d_tcpSocketHandle);

    for (int i = 0; i < SIZE; i++)
        createDynamicUDPSocket(remote_port[i], remote_host, &(d_udpSocketHandle[i]), &(d_remoteSocketInfo[i]));

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            d_adj[i][j] = 0;
}
Client::~Client()
{
    closeSocket(d_tcpSocketHandle);
    for (int i = 0; i < SIZE; i++)
        closeSocket(d_udpSocketHandle[i]);
}
int Client::send(const Name &server)
{
    return udpSend(server, d_udpSocketHandle[server], d_remoteSocketInfo[server], d_adj);
}
int Client::receive(const Name &server, int adj[SIZE][SIZE])
{
    return tcpReceive(server, d_tcpSocketHandle, adj);
}
int Client::display()
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

int Client::combine(int array[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (array[i][j] > 0)
                d_adj[i][j] = array[i][j];
    return 0;
}

int Client::min_key(int key[], bool mstSet[])
{
   int min = INT_MAX, min_index;
 
   for (int v = 0; v < SIZE; v++)
     if (mstSet[v] == false && key[v] < min)
         min = key[v], min_index = v;
 
   return min_index;
}
/*
 * This code was reused from 
 * http://www.geeksforgeeks.org/greedy-algorithms-set-5-prims-minimum-spanning-tree-mst-2/
 */
void Client::compute_mst()
{

    int parent[SIZE];
    int key[SIZE];
    bool mstSet[SIZE];

    for (int i = 0; i < SIZE; i++)
        key[i] = INT_MAX, mstSet[i] = false;

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < SIZE-1; count++)
    {
        int u = min_key(key, mstSet);

        mstSet[u] = true;

        for (int v = 0; v < SIZE; v++)

            if (d_adj[u][v] && mstSet[v] == false && d_adj[u][v] <  key[v])
                parent[v]  = u, key[v] = d_adj[u][v];
        }

    int output[SIZE][SIZE];

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
            output[i][j] = 0;
        if (i > 0)
            output[parent[i]][i] = output[i][parent[i]] = d_adj[i][parent[i]];
    }
    int sum = 0;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            d_adj[i][j] = output[i][j];
            if (j > i && d_adj[i][j] != 0)
                sum += d_adj[i][j];
        }
    }
    cout << "The Client has calculated a tree. The tree cost is " << sum;
    cout << "\nEdge\tCost\n";
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (j > i && d_adj[i][j] != 0)
                cout << ::type[i] << ::type[j] << "\t" << d_adj[i][j] << endl;
        }
    }
}

int main()
{
    int arrayA[SIZE][SIZE], arrayB[SIZE][SIZE], arrayC[SIZE][SIZE], arrayD[SIZE][SIZE];
    int portNumber = 25702;
    int remotePort[] = {21702, 22702, 23702, 24702};
    const char *remoteHost="127.0.0.1";
    Client obj(portNumber, remoteHost, remotePort);
    cout << endl;
    obj.receive(A, arrayA);
    obj.combine(arrayA);
    cout << endl;
    obj.receive(B, arrayB);
    obj.combine(arrayB);
    cout << endl;
    obj.receive(C, arrayC);
    obj.combine(arrayC);
    cout << endl;
    obj.receive(D, arrayD);
    obj.combine(arrayD);
    cout << endl;
    obj.send(A);
    cout << endl;
    obj.send(B);
    cout << endl;
    obj.send(C);
    cout << endl;
    obj.send(D);
    cout << endl;
    obj.compute_mst();

    return 0;
}
