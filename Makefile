CXX = clang++
CXXFLAGS = -std=c++23 -MMD -Wall -Wextra -ggdb3

jsobjects = dict.o functional.o io.o var.o

ifdef LTO
    CXXFLAGS += -flto=auto
    LDFLAGS += -flto=auto
    ifeq (,$(findstring clang,$(CXX)))
        AR = gcc-ar
    else
        AR = llvm-ar
    endif
else
    ifneq (,$(shell command -v mold))
        LDFLAGS = -fuse-ld=mold
    endif
endif

# totally unnecessary
silent = $(findstring s,$(word 1,$(MAKEFLAGS)))
ifeq (,$(silent)$(NOTIME))
    ifneq (,$(shell command -v time))
        export TIME = real\t%E\tuser\t%t\tsys\t%S
        SHELL = time sh
    endif
endif

ifdef DEBUG
    CXXFLAGS += -fsanitize=address,undefined
else
    $(jsobjects): CXXFLAGS += -O3
endif

aocsrc = $(wildcard aoc/*.cpp)
aocbin = $(aocsrc:.cpp=)
aoc/all: $(aocbin)

all: example aoc/all

example: example.o libjs.a
	$(LINK.cpp) $^ $(LDLIBS) -o $@

ARFLAGS = rcs
libjs.a: $(jsobjects)
	$(AR) $(ARFLAGS) $@ $?

aoc/%: aoc/%.cpp libjs.a
	$(LINK.cpp) $< $(LDLIBS) -o $@
$(aocbin): CXXFLAGS += -I.
$(aocbin): LDLIBS += libjs.a

clean:
	rm -rf *.[ado] aoc/*.[ado] $(aocbin) example file.tmp

.PHONY: clean all aoc/all

-include *.d
