#!/bin/sh

set -xe

clang -Wall -Wextra `pkg-config --cflags raylib` -o main main.c `pkg-config --libs raylib`
