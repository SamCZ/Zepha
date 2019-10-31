# Idiot's Guide to building Zepha on Windows:

## ~~Your Amazon Wish List~~ Prequesites:
### Step 0:
Download [Zepha](https://github.com/Aurailus/Zeus_cpp/)
### Step 1:
Install [VCPKG](https://github.com/microsoft/vcpkg), (like to your desktop, as it's just a folder)
### Step 2:
Install [CMake](https://cmake.org/download/)
### Step 3:
Install [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/) (Clang and GCC not supported!)
### Step 4:
Install VCPKG dependancies:
In powershell, in the `vcpkg` folder, execute:
`.\vcpkg install gzip glm sol2 stb-image glew glfw assimp libnoise enet lua pthread --triplet x64-windows`

## ~~Hiring Cowboy Builders~~ The Pre-Building Process:
Load up CMake, and find the downloaded `Zeus_cpp` repo, and create another folder called `Zeus_build`. 
With CMake, click `Browse Source`, select the `Zeus_cpp` folder as mentioned earlier as this contains the information to correctly build the game. With that step done, click `Browse Build`, and select the folder `Zeus_build`. With these folders correctly chosen, click configure, which should bring up a wizard like window.

Once in the window for configuring the build, set `optional platform for generator` to `x64` under the dropdown menu. After that, select the radio button `Specify toolchain for crosscompiling`, which will allow you to select the `vcpkg` libraries and header files required by `Zeus_cpp`. With that out of the way, click `next`

Navigate to the toolchain file which is located in the `vcpkg` folder, which is found here: `vcpkg\scripts\buildsystems\vcpkg.cmake` with the toolchain file completed, click `Finish`

## ~~Reading Some Books~~ Getting Libraries Fixed:
