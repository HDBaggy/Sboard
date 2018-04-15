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

all : board

board : $(srcBoard)
	$(compiler) $(srcBoard) -o $(binBoard) $(libBoard) $(libSwitch)

clean:
	rm $(dirApp)/*
