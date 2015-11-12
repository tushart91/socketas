#include <server.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    int array[SIZE][SIZE];
    int portNumber = 22702;
    int remotePort = 25702;
    const char *remoteHost="localhost";
    Server obj(portNumber, remoteHost, remotePort, B);
    cout << endl;
    
    obj.read("serverB.txt");
    cout << endl;
    obj.send();
    cout << endl;
    obj.receive(array);
    
    return 0;
}
