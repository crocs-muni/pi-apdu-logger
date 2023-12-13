#!/bin/bash
if ! test -d build ; then
 mkdir build
fi
g++ -o ./build/apduLogger ./src/app/Sampler.cpp ./src/app/Filter.cpp ./src/app/FileManager.cpp ./src/app/Decoder.cpp ./src/app/APDULogger.cpp -l wiringPi
