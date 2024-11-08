CXX = clang++
CXXFLAGS = -std=c++23 -Wall -Wextra -ggdb3 -fsanitize=address,undefined
main: var.o io.o

clean:
	rm -f *.o main file
.PHONY: clean
