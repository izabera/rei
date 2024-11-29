CXX = clang++
CXXFLAGS = -std=c++23 -MMD -Wall -Wextra -ggdb3

reiobjects = build/dict.o build/functional.o build/io.o build/var.o



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


VPATH = src

# default target for now
# will probably just leave librei.a and the tests at some point
all: build/librei.a build/example build/life build/testsuite build/aoc/all

ifdef INSTRUMENT
    ifneq (,$(findstring clang,$(CXX)))
        # clang produces some weird linker error with libstdc++
        CXXFLAGS += -stdlib=libc++
    endif
    $(reiobjects): CXXFLAGS += -finstrument-functions
    all: LDFLAGS += -rdynamic
    all: extra/libinstrument.so
    extra/instrument.o: CXXFLAGS += -fpic -O3
    extra/libinstrument.so: LDFLAGS += -shared
    extra/libinstrument.so: extra/instrument.o
	    $(LINK.cpp) $< $(LDLIBS) -o $@
    extra/%.o: %.cpp
	    $(COMPILE.cpp) $(OUTPUT_OPTION) $<
endif

build build/aoc:
	mkdir -p $@

build/%: CXXFLAGS += -Iinclude
build/%: LDLIBS += -Lbuild -lrei
build/%.o: %.cpp | build
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

build/example: example.cpp build/librei.a | build
	$(LINK.cpp) $< $(LDLIBS) -o $@

build/life: life.cpp build/librei.a | build
	$(LINK.cpp) $< $(LDLIBS) -o $@

build/testsuite: testsuite.cpp build/librei.a | build
	$(LINK.cpp) $< $(LDLIBS) -o $@

ARFLAGS = rcs
build/librei.a: $(reiobjects) | build
	$(AR) $(ARFLAGS) $@ $?

aocsrc = $(wildcard aoc/*.cpp)
aocbin = $(addprefix build/,$(aocsrc:.cpp=))

build/aoc/all: $(aocbin)
build/aoc/%: aoc/%.cpp build/librei.a | build/aoc
	$(LINK.cpp) $< $(LDLIBS) -o $@

clean:
	rm -rf file.tmp build extra/*o

.PHONY: clean all build/aoc/all

dotd = $(wildcard build/*.d) $(wildcard build/aoc/*.d)
-include $(dotd)
