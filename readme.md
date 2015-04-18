# Toolbox
A collection of random (possibly) useful C++ classes and functions. These are things that I have used in multiple projects, so I figured I would put them all in one place.

## Using the files
This is all currently in C++. I use C++11 features here and there. One class uses some x86 assembly and may only compile using GCC.

## The files
### Averager and Running Averager
Keep track of the mean of a bunch of values. One version takes the all-time mean, the other takes a running average of a configurable number of items.

### CycleTimer
Measure elapsed time with supposedly cycle-level precision. Only works on x86-64.

### LazyArray
A fixed-size array that doesn't initialize (call default constructors on) its contents. Elements can be constructed or destructed individually.

### Vector2
A 2D vector class with lots of useful operations and overloads.
