# Building Zepha

## Linux

### Using apt\[-get\]

1. Install Dependencies

    `sudo apt install freeglut3-dev liblua5.3-dev libnoise-dev libglm-dev libglew-dev libglfw3-dev libenet-dev libassimp-dev zlib1g-dev`

2. Configure CMake

    `cmake .`

3. Build using make

    `time make -j<number of cpu threads>`
    
4. Run the program

    See [docs/Executing.md](https://github.com/Aurailus/Zepha/blob/master/docs/Executing.md) for instructions for how to run the built program.

### Template for Contributing

```markdown
### Using <package manager>

1. Install Dependencies

    `sudo ???`

2. Configure CMake

    `cmake .`

3. Build using make

    `time make -j<number of cpu threads>`
    
4. Launch Zepha

    See [docs/Executing.md](https://github.com/Aurailus/Zepha/blob/master/docs/Executing.md) for instructions for how to run the built program.
```

## Windows

1. Install [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/).

2. Install [CMake](https://cmake.org/download/).

3. Download [VCPKG](https://github.com/microsoft/vcpkg) to your desktop.

4. Install VCPKG dependencies.
    
    Execute the following in the VCPKG folder in powershell:
     
    `.\vcpkg install gzip glm stb-image glew glfw assimp libnoise enet lua pthread --triplet x64-windows`

5. Pre building:

    Load CMake, find the downloaded Zepha repository, and create a new folder called Zepha_build. In CMake, click Browse Source, select the Zepha, as this contains the information to correctly build the game. Next, click Browse Build and select the folder Zepha_build. Finally, click configure, which should bring up a configuration wizard.
    
    Once in the wizard, set the Optional Platform for Generator to x64 in the dropdown menu. After that, select Specify Toolchain for Crosscompiling, which will allow you to select the VCPKG libraries and header files required by Zepha. Then click next.
    
    Navigate to the toolchain file which is located in the VCPKG folder, which is found in `\vcpkg\scripts\buildsystems\vcpkg.cmake`. Click Finish.
    
6. Fixing the linker:

    The linker for Zepha in Visual Studio Community has a few issues with CMake's find_path() and find_library() functions. Because of this, the linker will stop compiling even if the source files compile successfully. In other words, you need to right click the Zepha Project, then click the Properties option in the context menu, and under the Linker Options, expand the options, and go to the Input submenu.
    
    These next instructions are build type specific, Debug builds work with the VS Community debugger, while the release build does not. Click the Additional Dependencies dropdown arrow, then click <Edit>. Delete all the included libraries, and input the following:
    
    Keep in mind, \vcpkg\ refers to the directory that you installed it in.
    
    - For Debug:
    
            src\Debug\Zepha_Core.lib
            \vcpkg\installed\x64-windows\debug\lib\enet.lib
            \vcpkg\installed\x64-windows\debug\lib\glfw3dll.lib
            \vcpkg\installed\x64-windows\debug\lib\glew32d.lib
            \vcpkg\installed\x64-windows\debug\lib\assimp-vc142-mtd.lib
            \vcpkg\installed\x64-windows\debug\lib\lua.lib
            \vcpkg\installed\x64-windows\debug\lib\pthreadVC3d.lib
            \vcpkg\installed\x64-windows\debug\lib\z.lib
            \vcpkg\installed\x64-windows\debug\bin\noise.lib
            opengl32.lib
            kernel32.lib
            user32.lib
            gdi32.lib
            winspool.lib
            shell32.lib
            ole32.lib
            oleaut32.lib
            uuid.lib
            comdlg32.lib
            advapi32.lib
            ws2_32.lib
            winmm.lib
    
    - For Release:
    
            src\Release\Zepha_Core.lib
            \vcpkg\installed\x64-windows\lib\enet.lib
            \vcpkg\installed\x64-windows\lib\glfw3dll.lib
            \vcpkg\installed\x64-windows\lib\glew32.lib
            \vcpkg\installed\x64-windows\lib\assimp-vc142-mt.lib
            \vcpkg\installed\x64-windows\lib\lua.lib
            \vcpkg\installed\x64-windows\lib\pthreadVC3.lib
            \vcpkg\installed\x64-windows\lib\z.lib
            \vcpkg\installed\x64-windows\bin\noise.lib
            opengl32.lib
            kernel32.lib
            user32.lib
            gdi32.lib
            winspool.lib
            shell32.lib
            ole32.lib
            oleaut32.lib
            uuid.lib
            comdlg32.lib
            advapi32.lib
            ws2_32.lib
            winmm.lib
            
7. Launch Zepha

With either build type completed, navigate to `Zepha_build\[Debug or Release]\` and you will find a freshly built copy of Zepha.

See [docs/Executing.md](https://github.com/Aurailus/Zepha/blob/master/docs/Executing.md) for instructions for how to run the built program.