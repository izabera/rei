CXX = clang++
CXXFLAGS = -std=c++23 -MMD -Wall -Wextra -ggdb3

reiobjects = dict.o functional.o io.o var.o



ifdef LTO
    # you probably don't care about build times
    CXXFLAGS += -flto=auto -O3 -march=native
    LDFLAGS += -flto=auto -O3 -march=native
    ifeq (,$(findstring clang,$(CXX)))
        AR = gcc-ar
    else
        AR = llvm-ar
    endif
else
    # mold supports lto but (from my understanding) in this case the plugin
    # would need to take ownership of everything
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
    $(reiobjects): CXXFLAGS += -O3
endif



# default target for now
# will probably just leave librei.a and the tests at some point
all: librei.a example testsuite aoc/all

example: example.o librei.a
	$(LINK.cpp) $^ $(LDLIBS) -o $@

testsuite: testsuite.o librei.a
	$(LINK.cpp) $^ $(LDLIBS) -o $@

ARFLAGS = rcs
librei.a: $(reiobjects)
	$(AR) $(ARFLAGS) $@ $?

aocsrc = $(wildcard aoc/*.cpp)
aocbin = $(aocsrc:.cpp=)

aoc/all: $(aocbin)

aoc/%: aoc/%.cpp librei.a
	$(LINK.cpp) $< $(LDLIBS) -o $@
$(aocbin): CXXFLAGS += -I.
$(aocbin): LDLIBS += librei.a

clean:
	rm -rf *.[ado] aoc/*.[ado] $(aocbin) example testsuite file.tmp

.PHONY: clean all aoc/all

-include *.d
