# Idiot's Guide to building Zepha on Windows:

## ~~Your Amazon Wish List~~ Prequesites:
### Step 0:
Download the GitHub .zip, or `git clone` [Zepha](https://github.com/Aurailus/Zepha/)
### Step 1:
Install [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/) (Clang and GCC not supported!)
### Step 2:
Install [CMake](https://cmake.org/download/)
### Step 3:
Install [VCPKG](https://github.com/microsoft/vcpkg), (like to your desktop, as it's just a folder)

### Step 4:
Install VCPKG dependancies:
In powershell, in the `vcpkg` folder, execute:
`.\vcpkg install gzip glm stb-image glew glfw assimp libnoise enet lua pthread --triplet x64-windows`

## ~~Hiring Cowboy Builders~~ The Pre-Building Process:
Load up CMake, and find the downloaded `Zepha` repo, and create another folder called `Zepha_build`. 
With CMake, click `Browse Source`, select the `Zepha` folder as mentioned earlier as this contains the information to correctly build the game. With that step done, click `Browse Build`, and select the folder `Zepha_build`. With these folders correctly chosen, click configure, which should bring up a wizard like window.

Once in the window for configuring the build, set `optional platform for generator` to `x64` under the dropdown menu. After that, select the radio button `Specify toolchain for crosscompiling`, which will allow you to select the `vcpkg` libraries and header files required by `Zepha`. With that out of the way, click `next`.

Navigate to the toolchain file which is located in the `vcpkg` folder, which is found here: `\where you installed\vcpkg\scripts\buildsystems\vcpkg.cmake` with the toolchain file completed, click `Finish`

## ~~Triforce of Compiling???~~ Fixing The Linker:
Here's the tricky part, as the linker for the `Zepha` project in Visual Studio Community has a few issues with how ~~the idiot who did the CMakeLists~~ CMake's `find_path()` and `find_library()` misbehave with VCPKG. Because of this, the linker will stop compiling even if Zepha's `.cpp` and `.h` files compile successfully. In otherwords, you need to right click the `Zepha` Project. Click the `Properties` option in the right click list. Under the `Linker` options, expand the options with the arrow, and go to the `Input` submenu.

These next instructions are build type specific, `Debug` builds work with the VS Community debugger, while the release build does not. Click the `Additional Dependancies` dropdown arrow, then hit `<Edit>` Delete all the included libraries, and use the following:

Keep in mind, `\vcpkg\` refers to where you installed it.

### Building For Debug (With Debug Symbols):

```
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
```

### Building For Release ~~(To Flex On People)~~:

```
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
```

## ~~Rocket Science~~ Launching Zepha:

With either build type completed, head into `Zepha_build\[Debug or Release]\` you should notice a freshly built copy of Zepha. For obvious reasons, head into the `Zepha` directory and copy both `assets` and `mods` to the same folder as the Release or Debug executable. By double clicking Zepha.exe, it'll try connecting to the localhost server, so to make the game *start* you'll need a PowerShell window. In Windows 10, this is accomplished by `shift + right clicking` inside the same folder as `Zepha.exe`. Inside the PowerShell window, type `.\Zepha --mode=server`. With the server running, start `Zepha.exe` as you would normally. The game should connect to the localhost server and begin playing the game.