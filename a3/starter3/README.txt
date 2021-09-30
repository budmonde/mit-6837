Build Instructions (Athena with Makefile)
==================
$ mkdir build
$ cd build
$ cmake ..
$ make

The sample solution binaries are in sample_solution/

Submission
==========
We only provide official support for developing under the Athena cluster.
Your submission has to build and run over there to receive credit.

Please submit your entire source directory (excluding the build
directory) and compiled binary in inst/.

- Sorry my athena compiler is kinda screwed because of some compiler configurations that were changed when I was taking another class (6.172). I tried briefly to figure it out but it never worked out.
- No collaborators
- No sources apart from cplusplus.com
- No problems should be present. The configurations of the different systems are set in main.cpp and can easily be changed to emulate the behaviour in the sample_solutions
- I added the following extra credit features:
    - Wind Toggle for the cloth only. The toggle key is 'w'. Creates a wind that varies slightly over time in the Vector3f::FORWARD diretion
    - Implemented smooth shading for the cloth
    - Implemented a 3D structure called GelatinSystem
    - Started to implement to implement a mouse based UI before promptly giving up when I was trying to figure out how to get world coordinates from window coordinates in OpenGL :(
- No comments
