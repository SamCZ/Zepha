# Linux Building

# Guide for Debian 10 (and or apt based systems)

## Install Dependancies:

`sudo apt install freeglut3-dev liblua5.3-dev libnoise-dev libglm-dev libglew-dev libglfw3-dev libenet-dev libassimp-dev zlib1g-dev`

## Configure CMake:
`cmake .`

## Build the sucka:
`time make -j<number of cpu threads>`

## Executing The Client:
`./Zepha`

## Executing The Server:
`./Zepha --mode=server --subgame=zeus`

Have fun.