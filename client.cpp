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

// A utility function to find the vertex with minimum key value, from
// the set of vertices not yet included in MST
int Client::minKey(int key[], bool mstSet[])
{
   // Initialize min value
   int min = INT_MAX, min_index;
 
   for (int v = 0; v < SIZE; v++)
     if (mstSet[v] == false && key[v] < min)
         min = key[v], min_index = v;
 
   return min_index;
}
 
// Function to construct and print MST for a graph represented using adjacency
// matrix representation
void Client::computeMST()
{

    int parent[SIZE]; // Array to store constructed MST
    int key[SIZE];   // Key values used to pick minimum weight edge in cut
    bool mstSet[SIZE];  // To represent set of vertices not yet included in MST

    // Initialize all keys as INFINITE
    for (int i = 0; i < SIZE; i++)
        key[i] = INT_MAX, mstSet[i] = false;

    // Always include first 1st vertex in MST.
    key[0] = 0;     // Make key 0 so that this vertex is picked as first vertex
    parent[0] = -1; // First node is always root of MST 

    // The MST will have V vertices
    for (int count = 0; count < SIZE-1; count++)
    {
        // Pick thd minimum key vertex from the set of vertices
        // not yet included in MST
        int u = minKey(key, mstSet);

        // Add the picked vertex to the MST Set
        mstSet[u] = true;

        // Update key value and parent index of the adjacent vertices of
        // the picked vertex. Consider only those vertices which are not yet
        // included in MST
        for (int v = 0; v < SIZE; v++)

            // d_adj[u][v] is non zero only for adjacent vertices of m
            // mstSet[v] is false for vertices not yet included in MST
            // Update the key only if d_adj[u][v] is smaller than key[v]
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
    obj.computeMST();

    return 0;
}
