# Compiler
compiler = g++

# App Directory
dirApp = App

# Libraries
libBoard = -lpthread
libSwitch = -lwiringPi

#  Board source - binary
srcBoard = */*.cpp *.cpp
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

all : board

board : $(srcBoard)
	$(compiler) $(srcBoard) -o $(binBoard) $(libBoard) $(libSwitch)

clean:
	rm $(dirApp)/*
