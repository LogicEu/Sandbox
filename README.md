@eulogic - SANDBOX 2021
Sandbox framework for developing modular C99 libraries and projects. 
It contains math, graphics/rendering, image processing, 
data structures, utilities and more.

The spirit of this sandbox is to root horizontal expantion of knowledge
and a personal code base built upon modules. 

It easily compiles and runs in MacOS and Linux.

Libraries:
    -ZBug: Simple log system to report memory allocations, leaks and
             seg faults.
    -Fract: Math, vectors, matrixes, random functions, perlin noise
    -Utopia: Utilities and generic data structures (list, dynamic
            array, stacks, queue)
    -Imagine: API for importing, exporting and transforming various 
            image formats (png, jpeg, gif, ppm).
    -Core: Thin level of abstraction over GLFW for handling windows 
            and OpenGL contexts.
    -Mass: Rudimentary library for reading and writing Wavefront .obj
            models.
    -Taxi: Simple graphic abstractions over Imagine and Core libs (textures,
            framebuffers, fonts...)
    -Aural: Example abstraction built on top of FMOD Audio API.
    -Modular: Implementation of a run-time data oriented ECS (Entity 
            Component System)
    -Nano: Attempt to simplify usage of the ENet cetworking C library
    -Spawn: Example of some kind of serialization of data.
    -Photon: Simple 2D collision librarie.
    -Dwarf: Examples on drawing a number of different objects using OpenGL.

All the dependencies can be easily installed by a packet manager, o be
manually built and put in the /lib subdirectory to compile.

Dependencies:
    -libpng
    -libjpeg
    -glfw
    -FreeType
    -FMOD
    -enet
