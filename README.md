# WORK IN PROGRESS! Code isn't ready.

## PLANS
In the future I will extract some functions etc to another .cpp files, and refactor code.

### Programming-Class-Final-Project
It's a classic 2D Space Invaders - style game I created with C++ and Allegro5 for my Introduction to Programming classes final project. 
I decided to use one C++ file, because game isn't really complicated.

#### COMPILATION
The best way to run this project is to set up new console application project in Visual Studio 2019, include GraAllegroPierwszyRelease.cpp
as a source file in solution, and to install Allegro5 through Nuget packages manager (it's propably the fastest way to set up Allegro).

Also, you need to include some add-ons in project properties in Allegro5 tab:
* Font Add-on
* True Type Add-on
* Image Add-on
* Primitives Add-on

You can compile it in Release mode for x86 Windows. 
Assets folder must be in Release folder of the solution, or errors will occur.
