#include <network_utils.h>
#include <client.h>
#include <iostream>
#include <string>
#include <sstream>
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
int Client::send(const char *buff, Name type)
{
    return udpSend(d_udpSocketHandle[type], d_remoteSocketInfo[type], buff);
}
int Client::receive(char *buff)
{
    return tcpReceive(d_tcpSocketHandle, buff);
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
}
int Client::serialize(string *serial)
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
int Client::deserialize(string input, int array[SIZE][SIZE])
{
    istringstream ss(input);
    string token;
    int i = 0, j = 0, value;
    while(getline(ss, token, ';')) {
        istringstream sss(token);
        string subtoken;
        j = 0;
        while(getline(sss, subtoken, ',')) {
            array[i][j] = atoi(subtoken.c_str());
            ++j;
        }
        ++i;
    }
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

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            d_adj[i][j] = output[i][j];
}

int main()
{
    char buffA[512], buffB[512], buffC[512], buffD[512];
    int arrayA[SIZE][SIZE], arrayB[SIZE][SIZE], arrayC[SIZE][SIZE], arrayD[SIZE][SIZE];
    int portNumber = 25702;
    int remotePort[] = {21702, 22702, 23702, 24702};
    const char *remoteHost="localhost";
    string message;
    cout << "Creating..." << endl;
    Client obj(portNumber, remoteHost, remotePort);
    
    cout << "Receive...A" << endl;
    obj.receive(buffA);
    cout << buffA << endl;

    cout << "Deserialize...A" << endl;
    obj.deserialize(buffA, arrayA);

    cout << "Combine...A" << endl;
    obj.combine(arrayA);
    
    cout << "Receive...B" << endl;
    obj.receive(buffB);
    cout << buffB << endl;

    cout << "Deserialize...B" << endl;
    obj.deserialize(buffB, arrayB);

    cout << "Combine...B" << endl;
    obj.combine(arrayB);
    
    cout << "Receive...C" << endl;
    obj.receive(buffC);
    cout << buffC << endl;

    cout << "Deserialize...C" << endl;
    obj.deserialize(buffC, arrayC);

    cout << "Combine...C" << endl;
    obj.combine(arrayC);
    
    cout << "Receive...D" << endl;
    obj.receive(buffD);
    cout << buffD << endl;

    cout << "Deserialize...D" << endl;
    obj.deserialize(buffD, arrayD);

    cout << "Combine...D" << endl;
    obj.combine(arrayD);
    
    obj.display();

    cout << "MST..." << endl;
    obj.computeMST();
    obj.display();

    cout << "Serialize..." << endl;
    obj.serialize(&message);

    cout << "Send...A" << endl;
    obj.send(message.c_str(), A);
    cout << "Send...B" << endl;
    obj.send(message.c_str(), B);
    cout << "Send...C" << endl;
    obj.send(message.c_str(), C);
    cout << "Send...D" << endl;
    obj.send(message.c_str(), D);

    cout << "Closing..." << endl;
    return 0;
}
