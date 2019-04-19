#!/bin/bash

cd /home/vagrant/
if [ ! -d src/jma ]; then
    mkdir -p src/jma
fi
if [ ! -d bin ]; then
    mkdir bin
fi
cd src/jma
if [ ! -d dotfiles ]; then
    git clone https://github.com/JeffAbrahamson/dotfiles.git
else
    (cd dotfiles && git pull --ff-only)
fi
cd dotfiles
./install.sh
cd
