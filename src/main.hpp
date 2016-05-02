#pragma once

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FOV 45.0f 

#define moveAmt 1
#define rotAmt 0.1

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

void showTimePerFrame();
