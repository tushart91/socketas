#include <server.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    char buff[512];
    int array[SIZE][SIZE];
    int portNumber = 21702;
    int remotePort = 25702;
    const char *remoteHost="localhost";
    const char *message = "Fuck You";
    cout << "Creating..." << endl;
    Server obj(portNumber, remoteHost, remotePort, A);

    obj.read("serverA.txt");

    string serial;
    obj.serialize(&serial);
    cout << serial << endl;

    cout << "Sending..." << endl;
    obj.send(serial.c_str());

    cout << "Receive..." << endl;
    obj.receive(buff);
    obj.deserialize(buff, array);

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
            cout << array[i][j] << "\t";
        cout << endl;
    }
    
    cout << "Closing..." << endl;
	return 0;
}
