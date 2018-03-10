# Compiler
compiler = g++

# Libraries
libBoard = -lpthread
libSwitch = -lwiringPi

#  Board source - binary
srcBoard = Board/*.cpp Switch/*.cpp
binBoard = board

# Client source - binary
srcClient = Client/*.cpp Client/TCPLibrary/*.cpp
binClient = Client

# Server source - binary
srcServer = Server/*.cpp 
binServer = Server

# Server source - binary
srcSwitchReader = ReadSwitch/*.cpp
binSwitchReader = readSwitch


all : board client server readSwitch

board : $(srcBoard)
	$(compiler) $(srcBoard) -o $(binBoard) $(libBoard)

client : $(srcClient)
		$(compiler) $(srcClient) -o $(binClient) $(libBoard)

server : $(srcServer)
	$(compiler) $(srcServer) -o $(binServer) $(libBoard)

readSwitch : $(srcSwitchReader)
	$(compiler) $(srcSwitchReader) -o $(binSwitchReader) $(libBoard) $(libSwitch)

clean:
	rm $(binBoard)
