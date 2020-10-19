# Linux Building Manual

## Guide for Debian 10 (and or apt[-get] based systems)

### Install Dependancies:

`sudo apt install freeglut3-dev liblua5.3-dev libnoise-dev libglm-dev libglew-dev libglfw3-dev libenet-dev libassimp-dev zlib1g-dev`

### Configure CMake:
`cmake .`

### Build it:
`time make -j<number of cpu threads>` 

##  General Instructions

### Executing The Client:
`./Zepha`

### Executing The Server With Specified Subgame:
`./Zepha --mode=server --subgame=subgame_name`

Have fun.

## Copy and Paste Template For UNIX-like systems:

```
## Guide for Your Distro/Package Manager Here:

### Install Dependancies:
`sudo ??? ?? ?? ?? ??`

### Configure CMake:
`cmake .`

### Build It:
`time make -j<number of cpu threads>`
```
