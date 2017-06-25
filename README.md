r128
====
128-bit (64.64) signed fixed-point arithmetic.

Purpose
-------
R128 provides a data structure and routines for manipulating 128-bit (64.64)
fixed-point quantities. Including:

* Basic arithmetic (add, subtract, multiply, divide)
* Bitwise operations (and, or, xor, not, shift)
* Comparison (min, max, floor, ceiling)
* Conversion (to and from floating point and ASCII/UTF-8 string)

Why fixed point?
----------------
Sometimes you need more, or more consistent, precision over a given range than
is possible with double-precision floating-point, but don't need fully
arbitrary-precision arithmetic. In these cases, fixed-point gives you adequate
precision without sacrificing a fixed memory footprint and bounded run-time.

128 bits has sufficient range and precision to cover the diameter of the
observable universe accurate to within the width of one hydrogen atom, or to
track the lifetime of the universe accurate to the time it takes a photon to
travel the width of that same hydrogen atom. This is probably sufficient for
most applications to obviate the need for an arbitrary-precision library.

Usage
-----
Place r128.h somewhere in your project and include it wherever it is needed.
There is no separate .c file for this library. To get the code, in *one* file
in your project, put:

#define R128_IMPLEMENTATION

before you include r128.h. You don't need to clone the repository unless you
want to run the tests.

Compiler/Library Support
------------------------
This library requires a C99 compliant compiler, however it could be made to
compile with a pre-C99 compiler that supports 64-bit integers. The only two
changes needed are providing suitable typedefs for int64_t and uint64_t and
replacing the ull integer suffix with the appropriate one for your compiler.

The only C runtime library functionality used by this library is <assert.h>.
This can be avoided by defining an R128_ASSERT macro in your implementation
file. Since this library uses 64-bit arithmetic, this may implicitly add a
runtime library dependency on 32-bit platforms.

C++ constructors and operator overloads are provided for C++ files that include
r128.h. All C++-isms are guarded by conditional compilation blocks, and all C++
functions are marked static inline, so r128.h can be included in both C and C++
source files. The source file that defines R128_IMPLEMENTATION can be either C
or C++.

Performance
-----------
Fixed-point uses integer machine instructions, which on most modern processors
are no faster, and often slower, than their floating-point equivalents.
Therefore, if performance is a concern, it may be better to use fixed-point for
storage of values, and to do computation on the differences between values as
floating point, if the precision loss of conversion is acceptable.

Attempts have been made to provide optimized code paths for 32-bit x86, but
performance on any 32-bit system--especially of multiplication and division--
will be much worse than on a 64-bit system.

License and Thanks
------------------
R128 is licensed under an MIT-style license. See LICENSE for details.

Special thanks go to Sean Barrett for developing the single-file library idea.
