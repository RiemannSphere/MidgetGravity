#!/bin/bash
gcc -o MidgetGravity -Wall -lm MidgetGravity.c
./MidgetGravity
gnuplot plot.gpi
gwenview MidgetGravity.png
