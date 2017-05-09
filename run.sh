#!/bin/bash

g++ lab1.cpp InitShader.cpp drawable.cpp point.cpp line.cpp square.cpp circle.cpp triangle.cpp -o ex -lGL -lGLU -lglut -lGLEW
./ex
