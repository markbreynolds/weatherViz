# weatherViz
OpenGL Visualization showing how different elements in an environment are effected by the weather.

# System Requirements:

Hardware:
-Graphics card with OpenGL 3 support.

Software:
-glm
-glfw3
-glew
-up-to-date graphics card drivers.

If any of these are problematic, please let me know. I can look into converting code OGL 2 if needed and I can include dependencies if need be.

# Installation:

Run the following commands:
-cmake .
-make

This should generate an executable for your system. However, assets are not packed into this executable, so it will only run while the "assets" folder is in the same folder as the executable.

Unfortunately, there is no system install procedure.

# Controls:

The camera currently always looks toward the object at (0,0,0).

Left Arrow: Rotate Mesh Left
Right Arrow: Rotate Mesh Right

W: Move Camera North
A: Move Camera West
S: Move Camera South
D: Move Camera East

E: Move Camera Down
Q: Move Camera Up
