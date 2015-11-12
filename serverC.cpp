#include <server.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    int array[SIZE][SIZE];
    int portNumber = 23702;
    int remotePort = 25702;
    const char *remoteHost="localhost";
    Server obj(portNumber, remoteHost, remotePort, C);
    cout << endl;
    
    obj.read("serverC.txt");
    cout << endl;
    obj.send();
    cout << endl;
    obj.receive(array);
    
    return 0;
}
