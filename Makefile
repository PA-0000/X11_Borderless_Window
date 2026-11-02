#----------------------------------------------------------------------------------#
# This program is a simple example of creating a borderless and resizable window
# in x11. Copyright (c) 2026-2027 Pouya Asadi
#
# This software is in the public domain and is provided AS IS, with NO WARRANTY.
# ---------------------------------------------------------------------------------#
# The borderless window can be resized, moved, and also supports all the XWindow
# features a regular window has:
#  - snapping to desktop halves/quadrants
#  - shake to minimize all other windows
#  - animations when minimizing, maximizing, restoring, snapping
#  - native soft shadow around the window
#----------------------------------------------------------------------------------#

PWD      = $(shell pwd)

CC       = gcc
WARNNING = -Wall -Wformat

CFLAGS   = $(WARNNING) -std=c99 -O2
LFLAGS   = -lm -lGL -lX11

clean:
	rm -rf $(PWD)/BorderlessWindow $(PWD)/BorderlessWindow.o
build:
	$(CC) $(CFLAGS) -o $(PWD)/BorderlessWindow.o -c $(PWD)/BorderlessWindow.c -I $(PWD)
	$(CC) $(CFLAGS) -o $(PWD)/BorderlessWindow $(PWD)/BorderlessWindow.o $(LFLAGS)
	$(PWD)/BorderlessWindow
