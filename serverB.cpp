#include <server.h>
#include <iostream>

using namespace std;

int main()
{
    char buff[512];
    int portNumber = 21702;
    int remotePort = 25702;
    const char *remoteHost="localhost";
    const char *message = "Fuck You Two";
    cout << "Creating..." << endl;
    Server obj(portNumber, remoteHost, remotePort);
    cout << "Sending..." << endl;
    obj.send(message);
    // cout << "Receive..." << endl;
    // obj.receive(buff);
    // cout << buff << endl;
    cout << "Closing..." << endl;
    return 0;
}
