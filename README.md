# Rinvid

Rinvid is a small framework for 2D games and multimedia applications development. Although it's primary use is for me to gain some experience and learn a bit about OpenGl and perhaps if it turns out well, in the future it could be used as a starting point for all my game projects. It is written in C++ and uses OpenGl and SFML. It's developed on Ubuntu 20.04 so Linux is the only supported OS for now.  

## How to build it

These are instructions for Ubuntu 20.04, it would probably work on other Debian based distros:  
First, you will need to install Bazel and SFML. [Bazel](https://bazel.build/) is a build system used by Rinvid. [SFML](https://www.sfml-dev.org/) is a lib that provides a simple interface to the various components of your PC and is used by Rinvid.  

    sudo apt install npm  
    sudo npm install -g @bazel/bazelisk  
    sudo apt install libsfml-dev  

After that, clone the repo and inside any repo directory run:  
    
    bazel run //examples/testing_grounds:test

This will build and run a Rinvid test example. If it succeeds, you are all set.  

## External libraries used by Rinvid

[SFML](https://www.sfml-dev.org/)  
[stb_image](https://github.com/nothings/stb)  
[glm](https://github.com/g-truc/glm)  
