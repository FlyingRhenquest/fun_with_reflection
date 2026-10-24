# Fun with C++26 Reflection -- Building a Library for C


There are some important rules to C++26 reflection. You can reflect on
damn near anything. You can generate structures with
`define_aggregate`. You can't create new functions.


Unless...


## Let's say you have a legacy C project and would like to use that [spiffy C++26 refection library...](https://github.com/munich-data-engineering/reflect-cpp)


Wat?


Chances are you only have a handful, maybe couple dozen or so structs
you really need to serialize.  So what if you wrote a [templated
function.](include/fwr/templated_function.h) We'll only do serialize
for this example to keep things simple.


Uhm...


Of course to use that in C, we'd need to make the signature look like
something C can understand. We can't write a function, but we CAN
create a structure with a pointer to a function in it! So let's just
roll up a [Serializer class](include/fwr/Serializer.h)


Now we can build a [library!](src/Demo.cpp) We'll need some [demo Cstructs](demo/c_structs.h).
Because we want to use the same
serializer type in the library and in a program that [outputs a header](demo/demo_header.cpp) that
C can understand, we'll define a separate file for our [serializer type](demo/serializer_type.h)


We're almost there! We just need to put together a [CmakeLists.txt](CMakeLists.txt) to build our library
and our header file...


And then... I have to remember how to write an [old-timey C program.](demo/c_demo.c) God, it's been
a while...


Now there *is* one minor detail in that reflect-cpp is encoding an array of characters
in our structs rather than a string. So if you want to output the json it encodes to,
you'll get the array of characters rather than a string. This can probably be addressed
with reflect-cpp if you were so inclined, but I'll leave that as an exercise for the
reader. Adding another structure to deserialize the JSON back into a struct would
be quite easy at this point, but I'll leave that as an exercise for the reader, too.


Now you know how to provide a C library generated with C++26 reflection. If you wanted to,
you could just as easily emit code to build a pybind11 library or Rust code to take
advantage of the static link library. Or Java JNI or import the DLL with C#. All from
single source of truth structure definitions.



## Building


If you don't have a reflection-capable compiler to build this with, you can grab
my [gcc_playground](https://github.com/FlyingRhenquest/gcc_playground) project
and build the latest gcc with the build_gcc.sh script provided there. I've tested
it in a couple of WSL ubuntu LTS distributions and bleeding edge Debian. It
installs to /usr/local/gcc by default so it shouldn't interfere with other
compilers on your system. I also provide a toolchain file you can use with
CMake, so it's quite easy;


```
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=/usr/local/gcc/toolchain.cmake
./c_demo
```
