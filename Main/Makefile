# Compiler
compiler = g++

# App Directory
dirApp = App

# Libraries
libBoard = -lpthread
libSwitch = -lwiringPi

#  Board source - binary
srcBoard = Board/*.cpp Switch/*.cpp
binBoard = $(dirApp)/board

# Client source - binary
srcClient = Client/*.cpp 
binClient = $(dirApp)/Client

# Server source - binary
srcServer = Server/*.cpp 
binServer = $(dirApp)/Server

# Server source - binary
srcSwitchReader = ReadSwitch/*.cpp Switch/*.cpp Switch/*.h
binSwitchReader = $(dirApp)/readSwitch

all : board client server readSwitch

board : $(srcBoard)
	$(compiler) $(srcBoard) -o $(binBoard) $(libBoard)

client : $(srcClient)
		$(compiler) $(srcClient) $(additionalFlat) -o $(binClient) $(libBoard)

server : $(srcServer)
	$(compiler) $(srcServer) -o $(binServer) $(libBoard)

readSwitch : $(srcSwitchReader)
	$(compiler) $(srcSwitchReader) -I Switch/ -o $(binSwitchReader) $(libBoard) $(libSwitch)

clean:
	rm $(dirApp)/*
