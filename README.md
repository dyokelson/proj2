# Dewi Yokelson - Quack Compiler

## C++ (bison, re-flex) version of project 2, parser, extended to contain the compiler which generates C code as output.

This uses my lexer from project 1

The code is very messy as I was simultaneously learning C and C++.

### To Run:

in the terminal in the proj2 directory run "make image"

"docker image ls" will list images, proj1 should be the one built by above command

then run "docker run -it proj2"

to test a file run "bin/parser samples/<file_name>.qk"


