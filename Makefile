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

aocsrc = $(wildcard aoc/*.cpp)
aocbin = $(aocsrc:.cpp=)
aoc/all: $(aocbin)

all: example aoc/all

example: example.o libjs.a
	$(LINK.cpp) $^ $(LDLIBS) -o $@

libjs.a: dict.o functional.o io.o var.o
	$(AR) $(ARFLAGS) $@ $?

aoc/%: aoc/%.cpp libjs.a
	$(LINK.cpp) $< $(LDLIBS) -o $@
$(aocbin): CXXFLAGS += -I.
$(aocbin): LDLIBS += libjs.a

clean:
	rm -rf *.[ado] aoc/*.[ado] $(aocbin) example file.tmp

.PHONY: clean all aoc/all

-include *.d
