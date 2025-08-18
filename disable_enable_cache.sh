#! /bin/bash

as $1.S -o $1.o
ld $1.o -o $1
