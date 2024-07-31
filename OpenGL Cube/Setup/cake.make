global
{
cxx : g++
std : 17
optimize : 3
define : DEBUG
}
 compile
{
    include_dir(directory/include)
    include_dir(include) // so this fine me include "Shader.h" instead of "../include/Shader.h"
    include_dir(vendor)
    
    srcfile(src/*.cpp)
    srcfile(vendor/stb/*.cpp)
   

    srcfile(src/*.c)
   }
linker
{
    link_dir(directory/lib)
    attach(glfw3)
    attach(opengl32)
    attach(gdi32)
    attach(kernel32)
}
output{
    outfile(bin/app);
}

