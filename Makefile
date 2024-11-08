CXX = clang++
CXXFLAGS = -std=c++23 -Wall -Wextra -ggdb3 -fsanitize=address,undefined
main: var.o

clean:
	rm -f var.o main
.PHONY: clean
