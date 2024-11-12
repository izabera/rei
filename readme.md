rei
===

rei is a c++23 dialect for rapid prototyping with fast rebuilds



a large number of modern c++ libraries are distributed as header-only.  this is
generally considered a good thing: it makes it easy to add them to your
company's project, and everything can be checked, optimised and inlined by
the compiler.  however, actually working with them becomes extremely painful

consider writing a basic parser: these days one might reach for a map, a
handful of regexes, and fmt or std::format.  the mere thought of including them
will immediately blow your compile times out of proportion.  now rebuilding
your 50 line parser takes 2 minutes because your compiler has to instantiate
308241 recursive variadic templates and resolve 74031 static asserts.  it makes
modern c++ completely unsuitable for rapid iteration

rei is the opposite: it's precompiled ahead-of-time as much as possible.  the
headers rei does provide are designed to be extremely lightweight, so you can
use this to play advent of code and not wait 397h to compile each file.  they
expose (almost) no templates and no constexpr



rei provides:

- a generic variable type that can hold strings, numbers, booleans, or nulls
- dictionaries that map variables to variables
- an io library to read and write files



rei code remains valid c++ code, and it requires a c++ compiler to build.
interoperation with existing c++ code is trivial



example.cpp is a mostly complete example of how to use all the functionality in
the library, and it compiles in single-digit centiseconds on my machine



the aoc directory has some solutions for advent of code 2019.
the dialect/library is evolving with what i need to solve the problems



the runtime is a bit slow for now but things are getting better



run make to build librei.a and the advent of code solutions
