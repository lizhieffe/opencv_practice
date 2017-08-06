#!/bin/bash

cmake . || exit
make || exit
./opencv_practice
