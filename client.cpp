#include <iostream>
#include <network_utils.h>
#include <client.h>
using namespace std;

Client::Client(int port_number, const char *remote_host, int remote_port)
{
	createStaticTCPSocket(port_number, &d_tcpSocketHandle);
    createDynamicUDPSocket(remote_port, remote_host, &d_udpSocketHandle, &d_remoteSocketInfo);
}
Client::~Client()
{
    closeSocket(d_tcpSocketHandle);
	closeSocket(d_udpSocketHandle);
}
int Client::send(const char *buff)
{
    return udpSend(d_udpSocketHandle, d_remoteSocketInfo, buff);
}
int Client::receive(char *buff)
{
	return tcpReceive(d_tcpSocketHandle, buff);
}

int main()
{
    char buff[512];
    int portNumber = 25702;
    int remotePort = 21702;
    const char *remoteHost="localhost";
    const char *message = "Fuck You";
    cout << "Creating..." << endl;
    Client obj(portNumber, remoteHost, remotePort);
    cout << "Receive...A" << endl;
    obj.receive(buff);
    cout << buff << endl;
    cout << "Receive...B" << endl;
    obj.receive(buff);
    cout << buff << endl;
    // cout << "Receive...C" << endl;
    // obj.receive(buff);
    // cout << buff << endl;
    // cout << "Receive...D" << endl;
    // obj.receive(buff);
    // cout << buff << endl;
    // cout << "Send..." << endl;
    // obj.send(message);
    cout << "Closing..." << endl;
	return 0;
}
