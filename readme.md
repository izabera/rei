jscpp (name subject to change)
===

a silly c++23 "dialect" to get variables that behave sorta like in js

- variables can be strings, numbers, booleans, or nulls
- dict is a map from var to var
- io reads and writes to files

this uses no templates and no constexpr anything, so i can use c++ to play
advent of code and not wait 397h to compile each file



example.cpp is a mostly complete example of how to use all the functionality in
the library, and it compiles in single-digit centiseconds on my machine
(if you remove the -MMD and the other flags (and also not link it))
(use mold to link things faster)



the aoc directory has some solutions for advent of code 2019.
the dialect/library is evolving with what i need to solve the problems



slightly outdated log but the timings are not orders of magnitude off:

```
$ make
clang++ -std=c++20   -c -o main.o main.cpp
real	0:00.05	user	0	sys	0.01
clang++ -std=c++20   -c -o var.o var.cpp
real	0:00.76	user	0	sys	0.06
clang++ -std=c++20   -c -o io.o io.cpp
real	0:00.34	user	0	sys	0.05
clang++ -std=c++20   -c -o dict.o dict.cpp
real	0:00.17	user	0	sys	0.02
ar rv libjs.a var.o io.o dict.o
ar: creating libjs.a
a - var.o
a - io.o
a - dict.o
real	0:00.10	user	0	sys	0.04
clang++ -std=c++20    main.o libjs.a  -o main
real	0:00.09	user	0	sys	0.05
$
$
$ touch main.cpp   # <---- normal usage of the library
$
$
$ make
clang++ -std=c++20   -c -o main.o main.cpp
real	0:00.07	user	0	sys	0.03
clang++ -std=c++20    main.o libjs.a  -o main
real	0:00.08	user	0	sys	0.05
$
```
