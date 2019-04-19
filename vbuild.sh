#!/bin/bash

vagrant ssh -c "cd /vagrant; CC=clang CXX=clang++ make -C . -j 6 -k $*"
