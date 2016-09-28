#!/bin/bash
TEST=$(pwd)
cd ~
cd /bin
$(sudo ln -s ~/workspace/portsetter.cpp.o setport)
cd $TEST