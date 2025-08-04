#pragma once
#include "C:\glfw-3.4.bin.WIN64\include\GLFW\glfw3.h"
#include <iostream>

#define anchorHorizontal 1
#define anchorVertical 2
#define stretchHorizontal 4
#define stretchVertical 8
#define maintainLocalHorizontal 16
#define maintainLocalVertical 32

extern int Vwidth;
extern int Vheight;
extern GLFWwindow* debugWindow;
extern unsigned char bitmapFont[];
extern unsigned char paneTexture[];
extern inline double screenToPixelCoordinateX(int X);
extern inline double screenToPixelCoordinateY(int Y);
extern inline double screenToLocalPixelCoordinateX(int X);
extern inline double screenToLocalPixelCoordinateY(int Y);
extern void drawPane(int width, int height, int X, int Y, unsigned char flags, bool isClicked, unsigned char outlineThickness, void (*functionOnClick)(), unsigned short ID);
extern void drawText(float X, float Y, unsigned short lineHeight, unsigned short tabWidth, float size, unsigned short charWrap, const char* string, unsigned short ID, unsigned short characterSpacing);
extern void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
extern void placeholder();
extern unsigned short PSEdropDownID;