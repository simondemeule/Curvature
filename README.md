# Curvature

Curvature is an experimental 3D raytracing engine written from scratch with one goal in mind: allowing light to be curved efficiently. 

![](https://github.com/simondemeule/Curvature/blob/master/doc/anim.gif)

This program implements standard techniques for raytracing acceleration such as BVH acceleration and multithreading support, but goes further with new, purpose-designed optimizations to accelerate curved lightpath rendering. At their core, these new techniques are inspired by ray-marching, and make use of distance functions to define safe, spherical zones where some calculations can be skipped in the iterative ray-curving process. In practice, these improve render time by 1.5x-4x, depending on the complexity of the scene. The engine represents the curvature of space as a set of bounded fields (which are free to intersect with each other or any normal object in the scene), which, given any point in space and a ray direction, return an incremental change in ray propagation. This way, a wide variety of light-bending fields can be defined. The engine is also intended to eventually support portals, and feature a more efficient path integration algorithm, improving the current, naive Euler method to the Runge-Kutta method.

I have been working this as a fun side project since early 2019, mostly as a proof of concept for this improved algorithm I came up with. I am completely aware this code is far from perfect: I definetely cut corners on memory management in some parts of this application. I wish to eventually fix this, but in the mean time, I hope you will look past this and be just as excited as I am to be rendering weird, curved-spacetime images.

# Requirements

* **standard library** is used for data structures and threading (**C++11 version** used for development)
* **CImg** is required for writing images to files (**version 292** used for development, but only extremely basic functions are used; older versions will likely work)
* **GLM** is required for all vector math (**version 0.9.9.8** used for development, but again, standard features used only; older versions will likely work)

# Compatibility Note

Curvature was developped on macOS, so there might be issues on Windows or Linux (especially the scene / mesh parsing code). Please let me know if you encounter / fix issues related to this.

# Getting Started

At the moment, getting scenes onto Curvature is a bit messy. The *RenderData* class contains all the information needed by the *RenderScheduler* class to produce an image. You can either use one of its test constructors with a hardcoded test scene, or load a scene from a simple text file. The documentation for this file format doesn't exist yet, and it unfortunately doesn't support specifying fields at the moment. It is just simpler to hard-code a test scene by rewriting the test scene constructors, until this issue gets properly fixed. Hopefully more documentation wil be produced looking forwards.
