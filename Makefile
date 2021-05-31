#!/bin/sh

# CONSTANTES
CC = g++ -pthread

Sources = museum.c

# make entries
%.o: %.cpp %.h
	$(CC) -c $<

# Cette entree permet de generer le programme binaire "server" a partir des Sources
server: *.c
	$(CC) $(Sources) -o server

all:
	make server
