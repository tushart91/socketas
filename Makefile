CC     = g++
CFLAGS = -Wall -I.
LIBS   = -lsocket -lnsl -lresolv
CLIENT = client
SERVER_A = serverA
SERVER_B = serverB
SERVER_C = serverC
SERVER_D = serverD

all: client A B C D

client: client.o network_utils.o
		$(CC) $(CFLAGS) -o $(CLIENT) client.o network_utils.o $(LIBS)
client.o: client.cpp client.h network_utils.h
		$(CC) $(CFLAGS) -c client.cpp
A:		serverA.o server.o network_utils.o
		$(CC) $(CFLAGS) -o $(SERVER_A) serverA.o server.o network_utils.o $(LIBS)
B:		serverB.o server.o network_utils.o
		$(CC) $(CFLAGS) -o $(SERVER_B) serverB.o server.o network_utils.o $(LIBS)
C:		serverC.o server.o network_utils.o
		$(CC) $(CFLAGS) -o $(SERVER_C) serverC.o server.o network_utils.o $(LIBS)
D:		serverD.o server.o network_utils.o
		$(CC) $(CFLAGS) -o $(SERVER_D) serverD.o server.o network_utils.o $(LIBS)
serverA.o: serverA.cpp server.h
		$(CC) $(CFLAGS) -c serverA.cpp
serverB.o: serverB.cpp server.h
		$(CC) $(CFLAGS) -c serverB.cpp
serverC.o: serverC.cpp server.h
		$(CC) $(CFLAGS) -c serverC.cpp
serverD.o: serverD.cpp server.h
		$(CC) $(CFLAGS) -c serverD.cpp
server.o: server.cpp server.h network_utils.h
		$(CC) $(CFLAGS) -c server.cpp
network_utils.o: network_utils.cpp network_utils.h
		$(CC) $(CFLAGS) -c network_utils.cpp
clean: 
		rm $(CLIENT) $(SERVER_A) $(SERVER_B) $(SERVER_C) $(SERVER_D) *.o
