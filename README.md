GamePhysics
===========

Experimenting with physics using the [ezEngine][ezEngine] for the lecture Game Physics by Prof. Dr. Roland Schmitz and Andreas Stiegler at Hochschule der Medien Stuttgart.

Build From Source
=================

Requirements
------------

* Python 3.4 or later
* Visual Studio 2013 or later
* [ezEngine][ezEngine] Milestone 5, i.e. commit 3fe9867d6da8d46af99c5a8c864ee4b8f19148e7
  * Needs CMake version 2.x (2.8 recommended)
  * Either `Debug` or `RelWithDebInfo` builds
* OpenGL 2.0 or later
* GLEW 1.11.0 or later

Instructions
------------

1. Create an environment variable `EZ_ENGINE_DIR` which points to the top-level directory of the ezEngine repo/installation, e.g. `C:/data/projects/3rdparty/ezEngine/`.
1. Create an environment variable `OPENGL_GLEW_DIR` which points to the top-level directory of the GLEW installation, e.g. `C:\data\projects\3rdparty\OpenGL\glew-1.11.0`
  * Make sure the directories `bin`, `lib`, and `include` are located directly under that
1. Open `Build/VisualStudio/GamePhysics.sln` using Visual Studio 2013 and build the any of projects within the `Apps` Solution Folders using either the `Debug` or `Release` configuration in 32 or 64 bit.

The Future
==========

If I find the time, I will try to make this project run on Linux using GCC or Clang. Using CMake, of course...

Third Party Software
--------------------

* [ezEngine][ezEngine]
* OpenGL and GLEW 1.11.0

[ezEngine]: https://github.com/ezEngine/ezEngine
[Manuzor]: https://github.com/Manuzor

Screenshots
===========

Particles Sample
----------------

The first sample I built. You simply click to spawn a particle, and click again to give it some linear velocity, according to the mouse position and the particle position.

![Particles](http://manuzor.github.io/GamePhysics/images/v0.1.0-exam_Particles.png)


Force Fields Sample
-------------------

The idea is to spawn a particle in the lower left spawn area, marked in red, and trying to hit the yellow circle on the top-right (the sun, if you want). The challenge here is that those gray circle are applying a linear acceleration to you particle, as long as you touch them.

![Force Fields](http://manuzor.github.io/GamePhysics/images/v0.1.0-exam_ForceFieldsSample.png)

Rotation Sample
---------------

This sample demonstrates the effect of different inertia tensors for the same shape. They use the same inertia tensor (solid cuboids) but with different mass and spatial distribution values, according to their size and the column row they're located in. Of course, the screenshot doesn't say much about how fast each object is rotating because, well, it is just a screenshot. :smiley:

![Rotating Bodies](http://manuzor.github.io/GamePhysics/images/v0.1.0-exam_RotationSample.png)


Sphere-Sphere Collision
-----------------------

Some spheres colliding with each other!

![Spheres](http://manuzor.github.io/GamePhysics/images/v0.1.0-exam_Experiments.png)
