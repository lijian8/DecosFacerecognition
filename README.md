A C++ implementation of the OpenFace face recognition library
=======================================================

This package includes the neural network and part of the code from the open source
project [OpenFace](http://cmusatyalab.github.io/openface/) which has been implemented
in C++ for use with this robot project.

The code is supposed to run on a NVIDIA Tegra Jetson TK1 board. The login data
for this board are:
```
Username: ubuntu
Password: ubuntu
```

In order to compile this library some dependencies are required. Most importantly
the OpenFace library depends on [dlib](http://dlib.net/), [OpenCV](http://opencv.org/)
and [Torch](http://torch.ch).

To install the torch library, start a new terminal and run the following commands.
```
$ git clone https://github.com/torch/distro.git ~/torch --recursive
$ cd ~/torch; bash install-deps;
$ ./install.sh
```

OpenCV has to be version 3.1 and needs to be compiled from source with cuda support.
Check this [link](http://elinux.org/Jetson/Installing_OpenCV#Installing_OpenCV_.28including_the_GPU_module.29_on_Jetson_TK1) for a complete description of this step:
```
$ TODO
```

At some point it would be nice to have dlib as a cmake submodule but for now
you have to download it and put it into the include path manually.

```
$ cd ~/Downloads
$ wget http://dlib.net/files/dlib-18.18.tar.bz2
$ tar jxf dlib-18.18.tar.bz2
$ cp -R dlib-18.18/dlib /usr/local/include/
$ rm dlib-18.18.tar.bz2
$ rm -r dlib-18.18
```

Next we also need some lua packages for to run the neural network in torch.


== Possible errors ==
=== Not found: ... errors ===
If you get a lot of not found: <torch_function_name> errors, then you are compiling
against the wrong lua library. So in the CMakeLists.txt file you should find
the lua library in target_link_libraries and make sure that it links against luajit.


=== File.lua:370: table index is nil ===
This error usually comes from trying to load a 64-bit binary neural network on a
32-bit machine. The solution can then be to export the neural network to ascii format
on a 64-bit machine. 
