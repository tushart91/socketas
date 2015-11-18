## Contributor
**Tushar Tiwari**

**ID: 9879276702**

## About
The main idea behind this project is to understand the working of sockets and how computer programs communicate data over the internet. In this project I have written a simple client and server application that communicate with each other. The servers communicate their neighbors to the client and the client updates all the servers with the topology of the entire network. The client also calculates the minimum spanning tree of the network topology.


## File Breakdown
1. **networkutils.h/networkutils.cpp** - This file contains all the functions that are required to create static and dynamic TCP, UDP sockets as well as functions required to send and recieve data over these sockets. It also contains functions to serialize and deserialize data.
2. **client.h/client.cpp** - This file contains the Client class which is a template for creating **1 static TCP** socket and **4 dynamic UDP** sockets. It also maps the send functionality to the UDP sockets and receive to the TCP socket. It also contains functionality to merge **adjacency matrices** and calculate the **minimum spanning tree**. Apart from that it finally contains the **main() for instantiating a client**.
3. **server.h/server.cpp** - This file contains the Server class which is a template for creating **1 static UDP** socket and **1 dynamic TCP** socket. It maps the send functionality to the TCP port and receive to the UDP port. It **does not contain code to instantiate a server**.
4. **serverA/B/C/D.cpp** - These 4 files contain the **main() for instantiating** the server. They also specify the order of actions.

## How To Compile
### All Files
    make all
### Only Client
    make client
### Only Server A
    make A
### Only Server B
    make B
### Only Server C
    make C
### Only Server D
    make D

## How To Run
*Note: You need to open 5 instances of the terminal. 1 terminal will run the client and each of the other 4 will correspond to a server. Run each of the commands in different terminal windows in the given order.*

###Client
    make runclient
### Server A
    make runa
### Server B
    make runb
### Server C
    make runc
### Server D
    make rund

## Message formats
This program uses adjacency matrix to store the neighbor information. However to exchange messages on the network, we need to serialize the matrix. So a matrix like:

    ---------
    |0|1|2|3|
    ---------
    |1|0|4|5|
    ---------
    |2|4|0|6|
    ---------
    |3|5|6|0|
    ---------

This matrix is then serialized as follows:

    "0,1,2,3;1,0,4,5;2,4,0,6;3,5,6,0"

## Idiosyncrasies
* The project will not play nice if the executables are run out of order. This is because when the server sends messages to the client the client assumes the first message is from server A and second from server B and so on.
* Also this project assumes there are only 4 servers in the topology.
* Also this project assumes all input txt files are available. There is no validation.

## References
* For computing the minimum spanning tree from adjacency matrix 
<http://www.geeksforgeeks.org/greedy-algorithms-set-5-prims-minimum-spanning-tree-mst-2/>
* Used this as base code for creating TCP sockets but modified over it based on requirements
<http://www.yolinux.com/TUTORIALS/Sockets.html>