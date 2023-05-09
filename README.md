# C-Compiler
//Takes C++ syntax as input and compiles in C++

//Example test case
(base) fenske212@MacBook-Pro CSE340PJ3 % make
g++ demo.o inputbuf.o lexer.o compiler.o

(base) fenske212@MacBook-Pro CSE340PJ3 % ./a.out < testcases/test_control_for1.txt
0 1 2 3 4 5 6 7 8 9 %

//or shell script command

(base) fenske212@MacBook-Pro CSE340PJ3 % ./testrun.sh