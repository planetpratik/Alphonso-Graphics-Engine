# Alphonso Graphics Engine
### Alphonso - Graphics Rendering Engine For Windows Built With Vulkan SDK.

This is a Personal Programming Project developed as a part of Programming-III curriculum ( FIEA, University of Central Florida - Summer 2019 )

### Prerequisites :-
This Project Requires following Tools and SDK's to be installed

* [CMake 3.14 (or Above)](https://cmake.org/download/) — Cross-Platform Build Environment
* [Vcpkg](https://github.com/microsoft/vcpkg) — Package Manager for Windows
* [Vulkan SDK for Windows (v1.1.106 or above)](https://vulkan.lunarg.com/sdk/home) — Latest Vulkan SDK
* [Visual Studio 2019](https://visualstudio.microsoft.com) - MSVC toolchain

Once ```Vcpkg``` is installed, You will need to install following dependencies through it ```(for both x86 & x64)```.
Checkout Vcpkg documentation about how to bootstrap and add new ports.

* GLFW (v3.3 or Above) - For Window Creation & Controller Input
* glm - OpenGL Maths Library
* OpenGL - 3D Rendering API

Once All dependencies are set-up, Check your ```Path``` Environment Variable whether it's pointing to Vulkan libraries or not. Vulkan path should be automatically added after installation but if not found, update Path Variable.

### How to Run :-
Now You're ready to run this project. Clone this repository using following command
```sh
git clone https://github.com/planetpratik/Alphonso-Graphics-Engine.git
```
Once complete you can run Project by directly opening folder in Visual Studio (During First Run CMake Cache will be generated along with build directory). Then Select your desired configuration and Click on Run.
