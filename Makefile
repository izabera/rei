CXX = clang++
CXXFLAGS = -std=c++23 -MMD -Wall -Wextra -ggdb3

reiobjects = dict.o functional.o io.o var.o



ifdef LTO
    CXXFLAGS += -flto=auto
    LDFLAGS += -flto=auto
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



aocsrc = $(wildcard aoc/*.cpp)
aocbin = $(aocsrc:.cpp=)

# default target for now.  will probably move librei.a first at some point
aoc/all: $(aocbin)

all: example aoc/all

example: example.o librei.a
	$(LINK.cpp) $^ $(LDLIBS) -o $@

ARFLAGS = rcs
librei.a: $(reiobjects)
	$(AR) $(ARFLAGS) $@ $?

aoc/%: aoc/%.cpp librei.a
	$(LINK.cpp) $< $(LDLIBS) -o $@
$(aocbin): CXXFLAGS += -I.
$(aocbin): LDLIBS += librei.a

clean:
	rm -rf *.[ado] aoc/*.[ado] $(aocbin) example file.tmp

.PHONY: clean all aoc/all

-include *.d
