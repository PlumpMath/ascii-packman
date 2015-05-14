ascii-packman
=============
Another study project of mine, done in May 2011.

## A study project to learn...

- curses lib
- inter process communication
- multithreading

## finished:

- server program
- client program
- single-play program

## unfinihsed:

- semaphore in chatting (didn't think it's needed)
- backspacing support in chatting
- Korean support in chatting

## make

    $ g++ ./packman_client.c -o ./packman_client -lncurses
    $ g++ ./packman_server.c -o ./packman_server -lncurses -lpthread

## run

First. Run packman_server first in a terminal.

    $ ./packman_server

Second. Run 3 packman_client in each other terminal sessions.

    $ ./packman_client

