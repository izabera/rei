CXX = clang++
CXXFLAGS = -std=c++23 -Wall -Wextra -ggdb3 -fsanitize=address,undefined
#LDFLAGS = -fuse-ld=mold

export TIME = \nreal\t%E\nuser\t%U\nsys\t%S
SHELL = time sh

main: main.o libjs.a
	$(LINK.cpp) $^ $(LDLIBS) -o $@

libjs.a: var.o io.o dict.o
	$(AR) $(ARFLAGS) $@ $?

clean:
	rm -rf *.o *.a main file .cache compile_commands.json

.PHONY: clean
