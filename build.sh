#!/bin/bash

x86_64-w64-mingw32-windres icons.rc -O coff -o icons.res
x86_64-w64-mingw32-g++ -fno-exceptions -fno-rtti -s -O3 -static -mwindows main.cpp icons.res -lstdc++ -o mouse_move    
