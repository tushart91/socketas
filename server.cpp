#include <iostream>
#include <network_utils.h>
#include <server.h>
using namespace std;

Server::Server(int port_number, const char *remote_host, int remote_port)
{
	createStaticUDPSocket(port_number, &d_udpSocketHandle);
	createDynamicTCPSocket(remote_port, remote_host, &d_tcpSocketHandle);
}
Server::~Server()
{
	closeSocket(d_udpSocketHandle);
	closeSocket(d_tcpSocketHandle);
}
int Server::send(const char *buff)
{
	return tcpSend(d_tcpSocketHandle, buff);
}
int Server::receive(char *buff)
{
	return udpReceive(d_udpSocketHandle, buff);
}
