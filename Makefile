CXX = clang++
CXXFLAGS = -std=c++23 -Wall -Wextra -ggdb3 -fsanitize=address,undefined

export TIME = \nreal\t%E\nuser\t%U\nsys\t%S
SHELL = time sh

main: var.o io.o dict.o

clean:
	rm -f *.o main file
.PHONY: clean
