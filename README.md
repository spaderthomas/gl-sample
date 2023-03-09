# gl-sample
Windows only. There's a solution file in `build/windows`. Crank that bad boy up, and you should get a window with a single triangle. This code is pulled from my 2D game engine, so a few of the innards might not quite make sense. In addition, some of the abstractions over OpenGL (e.g. for shaders) probably aren't exactly what you want, but the point of this is that it's a reference for how to do the tricky setup for this stack, and then you rip out what you want once it's working.

There are also a lot of tools (arrays, a pretty file monitor, temporary linear allocator, timer, vector and matrix types). These are in `source/utils`.
