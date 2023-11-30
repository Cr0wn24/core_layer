# Core layer

This is my core layer written in C used as a foundation in almost all of my projects. It is all written from scratch with no libraries except for:

- stb_image.h (for reading .png, .jpg, ...)
- freetype (for creating font bitmaps)

It includes:

- Base layer
  - Memory arenas
  - Strings
  - Math
  - Implementation of doubly & singly linked lists, queues and stacks
- OS Layer (Windows only)
  -  Absraction layer over Win32
  -  Create a window & poll events
  -  Load in OpenGL internals
  -  Other miscellaneous features that you expect from the OS
-  Renderer layer
   - 2D abstraction layer built on Direct3D11 and OpenGL (OpenGL not working at the current state)
   - Push rectangles, text or bitmaps onto batches
   - Subpixel text rendering
   - Atlas packing
- UI Layer
  - Immediate-mode style GUI
  - Easy to use API
  - Sliders, buttons, radio buttons, text input & more

## Building from source
This demonstrates how to build and run the provided example in examples folder.

This shows how to build the provided example. Only builds on windows at the moment and requires the MSVC compiler.

1. Run `build_core_layer_demo.bat` inside `examples` folder inside the `x64 Native Tools Command Prompt for VS 2022`
2. A folder `build` has been created in the root folder, run the .exe created.

## Showcase

This is a showcase of some basic features of my core layer.

![image](https://github.com/Cr0wn24/core_layer/assets/61281897/41eb770c-97ee-40a1-8d33-3b240463a14d)

