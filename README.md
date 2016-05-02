# weatherViz
OpenGL Visualization showing how different elements in an environment are effected by the weather.

# System Requirements:

Hardware:  
+ Graphics card with OpenGL 3 support.

Software:  
+ cmake  
+ glm  
+ glfw3  
+ glew  
+ up-to-date graphics card drivers.

If any of these are problematic, please let me know. I can look into converting code OGL 2 if needed and I can include dependencies if need be.

# Installation:

Run the following commands in the top directory:  
1. `cmake .`  
2. `make`

This should generate an executable for your system. However, assets are not packed into this executable, so it will only run while the "assets" folder is in the same folder as the executable.

There is currently no system install procedure.

# Controls:

Left Arrow: Rotate Mesh Left  
Right Arrow: Rotate Mesh Right

W: Move Camera Forward  
A: Move Camera Left  
S: Move Camera Back  
D: Move Camera Right  

E: Move Camera Down  
Q: Move Camera Up

O: Increase Windspeed  
L: Decrease Windspeed

You can click and drag with the mouse to look around. This is a WIP at the moment and is currently behaving strangely.

# Milestones:

~~Milestone 1:~~  
+ ~~Models (mesh/textures showing properly in OpenGL):~~  
 + ~~Windmill~~  
 + ~~Ground~~  
 + ~~Sky box/sphere~~  
+ ~~Manual weather control.~~  
+ ~~Windmill turns faster with wind speed.~~  

Milestone 2:  
+ Grass (Either particle system or billboard)  
+ Trees  
+ Soft body dynamics (Wind) for grass and trees  
+ Rain (Probably particle system, if not, fullscreen texture)  

Milestone 3:  
+ Fog (Find some way to make OGL fog look decent)  
+ Snow  
+ Hail  
+ Shadows/Proper Lighting  
+ Automatic weather control (online)  

Milestone 4 (Extras):  
+ 360-degree look around  
+ Automatic weather control (from Raspberry Pi based weather station)  
+ Change lighting for time of day.  
+ Clouds/Fog move slowly  
+ Special Effects/Compositing  
+ Lens Flare  
+ Focus  
+ New visualization environments  
 + Mountain  
 + Forest  
 + City  

# Documentation:

Documentation can be generated if you have Doxygen installed. To generate the documentation run the following commands:

1. `cmake .`  
2. `make docs`

This will generate the documentation in the "docs/html" directory.
