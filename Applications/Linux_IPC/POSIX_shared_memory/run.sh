#!/bin/bash


gcc writer.c -o writer.o -lrt
gcc reader.c -o reader.o -lrt

./writer.o &
./reader.o