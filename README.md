# SARAS
The above game is developed by Group SARAS from D4B consisting of Suyog, Aryamaan and Rutuj. Saras is an interactive maze game written using SDL library in C programming language. You can find the actual code in library.c file and the command script to compile the code in CmakeLists.txt file.

## Compilation:-
## Download and install msys2 64bit
* Download msys2 64bit from here: https://www.msys2.org/ and install it, then open MYSYS2 UCRT64 application.
  
## Update msys2
* Update msys2 64bit after install by running `pacman -Syu` if pacman needs to be updated you might have to close and reopen the terminal and run `pacman -Syu` again

## Install mingw64 gcc compiler
Now enter the following command:-
```sh
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
```

## Install build tools
```sh
pacman -S git mingw-w64-x86_64-toolchain mingw64/mingw-w64-x86_64-SDL2 mingw64/mingw-w64-x86_64-SDL2_mixer mingw64/mingw-w64-x86_64-SDL2_image mingw64/mingw-w64-x86_64-SDL2_ttf mingw64/mingw-w64-x86_64-SDL2_net mingw64/mingw-w64-x86_64-cmake make
``` 
