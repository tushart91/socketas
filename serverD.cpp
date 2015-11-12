#include <server.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    int array[SIZE][SIZE];
    int portNumber = 24702;
    int remotePort = 25702;
    const char *remoteHost="localhost";
    Server obj(portNumber, remoteHost, remotePort, D);
    cout << endl;
    
    obj.read("serverD.txt");
    cout << endl;
    obj.send();
    cout << endl;
    obj.receive(array);
    
    return 0;
}
