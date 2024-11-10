CXX = clang++
CXXFLAGS = -std=c++23 -MMD -Wall -Wextra -ggdb3

ifdef DEBUG
CXXFLAGS += -fsanitize=address,undefined
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
	rm -rf *.o *.a *.d main file.tmp

.PHONY: clean

-include *.d
