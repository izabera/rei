CXX = clang++
CXXFLAGS = -std=c++20

ifdef DEBUG
CXXFLAGS += -Wall -Wextra -ggdb3 -fsanitize=address,undefined
endif

ifndef NOTIME
ifneq (, $(shell command -v time))
export TIME = real\t%E\tuser\t%t\tsys\t%S
SHELL = time sh
endif
endif

main: main.o libjs.a
	$(LINK.cpp) $^ $(LDLIBS) -o $@

libjs.a: var.o io.o dict.o
	$(AR) $(ARFLAGS) $@ $?

clean:
	rm -rf *.o *.a main file .cache compile_commands.json

.PHONY: clean
