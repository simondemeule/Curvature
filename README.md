# Curvature

This is an experimental 3D raytracing engine written from scratch with one goal in mind: allowing light to be curved efficiently. 

![](https://github.com/simondemeule/Curvature/blob/master/doc/anim.gif)

This program implements standard techniques for raytracing acceleration such as BVH acceleration and multithreading support, but goes further with new, purpose-designed optimizations to accelerate curved lightpath rendering. At their core, these new techniques are inspired by ray-marching, and make use of distance functions to define safe, spherical zones where some calculations can be skipped in the iterative ray-curving process. In practice, these improve render time by 1.5x-4x, depending on the complexity of the scene. The engine represents the curvature of space as a set of bounded fields (which are free to intersect with each other or any normal object in the scene), which, given any point in space and a ray direction, return an incremental change in ray propagation. This way, a wide variety of light-bending fields can be defined. The engine is also intended to eventually support portals, and feature a more efficient path integration algorithm, improving the current, naive Euler method to the Runge-Kutta method.

I have been working this as a fun side project since early 2019, mostly as a proof of concept for this improved algorithm I came up with. I am completely aware this code is far from perfect: I definetely cut corners on memory management in some parts of this application. I wish to eventually fix this, but in the mean time, I hope you will look past this and be just as excited as I am to be rendering weird, curved-spacetime images.
