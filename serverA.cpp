#include <server.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    int array[SIZE][SIZE];
    int portNumber = 21702;
    int remotePort = 25702;
    const char *remoteHost="localhost";
    Server obj(portNumber, remoteHost, remotePort, A);
    cout << endl;
    
    obj.read("serverA.txt");
    cout << endl;
    obj.send();
    cout << endl;
    obj.receive(array);
    
	return 0;
}
