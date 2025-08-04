// NeoGC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <bitset>
#include <chrono>
#include <thread>
//#include "..\"
#include "C:\glfw-3.4.bin.WIN64\include\GLFW\glfw3.h"
//#include <gl\GL.h>
#include "tinyfiledialogs.h"
#include "portaudio.h"
#include <Windows.h>
#include "UI.h"
#define IMGUI_DEFINE_MATH_OPERATORS
//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl2.h"
//#include "C:\imgui-1.91.9b\backends\imgui_impl_glfw.cpp"
//#include "C:\imgui-1.91.9b\backends\imgui_impl_opengl3.cpp"
#include "memAccess.h"

#include "memory.h"
#include "opcodes.h"
#include "VPU.h"
#include "APU.h"
#include "testData.h"

PaStream* stream;
GLFWwindow* debugWindow;
GLFWwindow* window;
GLuint viewWindowtextureID;
GLuint fontTextureID;
GLuint paneTextureID;

#define pi 3.14159265359

unsigned short testCounter;
unsigned short testCounter2;
unsigned int i;
boolean IsLoaded;
boolean IsFileLoaded;
unsigned short instructionArrayIndex;
unsigned short cycleCount;
boolean memAccessSuccessful;
unsigned int testValue;
unsigned int testValue2;
unsigned int bufferIndex;
unsigned int VPUUsage;
unsigned short testFlags;
signed short inputScale = 256;
unsigned short controlFlags;
unsigned short frameBufferOffset;
bool programTerminate;
unsigned short PCCycleDelay;
unsigned char cycleDelayCount;
unsigned long outputBufferOffset = (547 << 2);
unsigned char CPUStep;
bool isDelayFetched;
unsigned char currentCCEntry;
unsigned char CCCount;
unsigned int CPUUsage;
unsigned char DMACycleDelay;
unsigned char DMACycleCounter;
bool frameInt;
bool isDMADelayFetched;
bool testFlag;
unsigned int createWindow = 0;
bool mouseDown = false;
int Vwidth, Vheight;
float foreverCounter;
unsigned short PSEdropDownID;
unsigned char stringListStart;
unsigned char DISPMode;
unsigned char DISPPalette;
unsigned char currentTab;
unsigned short vram_display_address;
unsigned short vram_display_address_copy;
unsigned int current_address;
unsigned int current_address_copy;
signed int stepCounter = -1;
signed long PCHalt = -1;
signed int currentCycle;
signed long previousPC = -1;
unsigned int OPringBuffer[2048];
unsigned short OPringBufferPointer;
unsigned short decodeOpcode;
signed long breakAddress = -1;
unsigned char tileRowWidth = 64;
bool isAPUPCM;
bool tileBankSelect;
GLFWgamepadstate state;

unsigned int instructionAddressBuffer[4096];
char NumCharbuffer[4096];
char NumString[32];
char* buffer_ptr = NumCharbuffer;
unsigned char buffer_src_ptr;

const char* list1Strings[] =
{
    "Bitmap",
    "Tiles",
    "Tilemap",
};

const char* areaStrings[] =
{
    "WRAM",
    "IO",
    "CRAM",
    "VRAM",
    "OAM",
    "ROM",
    "IntRAM",
    "N/A",
};

const char* numStrings[] =
{
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
};

char testString[] =
{
    "this is a message. THIS IS A MESSAGE. 123456789"
};

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Check if the button was the LEFT mouse button
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        // Check if the action was PRESS (not release)
        if (action == GLFW_PRESS)
        {
            // --- THIS IS WHERE YOUR CODE GOES ---
            // This block executes ONLY when the left mouse button
            // is initially pressed down.

            // Example: Get cursor position at the moment of the click
            //double xpos, ypos;
            //glfwGetCursorPos(window, &xpos, &ypos);
            //std::cout << "Clicked at: (" << xpos << ", " << ypos << ")" << std::endl;

            // You could set a flag here, call another function, etc.
            // bool left_mouse_just_clicked = true; // (You'd need to reset this flag later)
            mouseDown = true;
        }
    }
    // You can add checks for other buttons (GLFW_MOUSE_BUTTON_RIGHT, etc.) here
}

float vertices[] = {
    // Positions          // Texture Coords
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom Left
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom Right
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top Right

    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom Left
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top Right
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // Top Left
};

uint16_t tileOffsetLT[64] = {
0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0100, 0x0101, 0x0102, 0x0103, 0x0104, 0x0105, 0x0106, 0x0107,
0x0200, 0x0201, 0x0202, 0x0203, 0x0204, 0x0205, 0x0206, 0x0207, 0x0300, 0x0301, 0x0302, 0x0303, 0x0304, 0x0305, 0x0306, 0x0307,
0x0400, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407, 0x0500, 0x0501, 0x0502, 0x0503, 0x0504, 0x0505, 0x0506, 0x0507,
0x0600, 0x0601, 0x0602, 0x0603, 0x0604, 0x0605, 0x0606, 0x0607, 0x0700, 0x0701, 0x0702, 0x0703, 0x0704, 0x0705, 0x0706, 0x0707
};

uint16_t tileOffsetLT2[64] = {
0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0200, 0x0201, 0x0202, 0x0203, 0x0204, 0x0205, 0x0206, 0x0207,
0x0400, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407, 0x0600, 0x0601, 0x0602, 0x0603, 0x0604, 0x0605, 0x0606, 0x0607,
0x0800, 0x0801, 0x0802, 0x0803, 0x0804, 0x0805, 0x0806, 0x0807, 0x0A00, 0x0A01, 0x0A02, 0x0A03, 0x0A04, 0x0A05, 0x0A06, 0x0A07,
0x0C00, 0x0C01, 0x0C02, 0x0C03, 0x0C04, 0x0C05, 0x0C06, 0x0C07, 0x0E00, 0x0E01, 0x0E02, 0x0E03, 0x0E04, 0x0E05, 0x0E06, 0x0E07
};


int16_t triangleLut[256] = {
    32,    34,    36,    38,    40,    41,    43,
    45,    47,    49,    51,    53,    55,    56,
    58,    60,    62,    64,    66,    68,    70,
    71,    73,    75,    77,    79,    81,    83,
    85,    86,    88,    90,    92,    94,    96,
    98,   100,   101,   103,   105,   107,   109,
   111,   113,   115,   116,   118,   120,   122,
   124,   126,   128,   130,   131,   133,   135,
   137,   139,   141,   143,   145,   146,   148,
   150,   152,   154,   156,   158,   159,   161,
   163,   165,   167,   169,   171,   173,   174,
   176,   178,   180,   182,   184,   186,   188,
   189,   191,   193,   195,   197,   199,   201,
   203,   204,   206,   208,   210,   212,   214,
   216,   218,   219,   221,   223,   225,   227,
   229,   231,   233,   234,   236,   238,   240,
   242,   244,   246,   248,   249,   251,   253,
   255,   257,   259,   261,   263,   264,   266,
   268,   270,   272,   270,   268,   266,   264,
   263,   261,   259,   257,   255,   253,   251,
   249,   248,   246,   244,   242,   240,   238,
   236,   234,   233,   231,   229,   227,   225,
   223,   221,   219,   218,   216,   214,   212,
   210,   208,   206,   204,   203,   201,   199,
   197,   195,   193,   191,   189,   188,   186,
   184,   182,   180,   178,   176,   174,   173,
   171,   169,   167,   165,   163,   161,   159,
   158,   156,   154,   152,   150,   148,   146,
   145,   143,   141,   139,   137,   135,   133,
   131,   130,   128,   126,   124,   122,   120,
   118,   116,   115,   113,   111,   109,   107,
   105,   103,   101,   100,    98,    96,    94,
    92,    90,    88,    86,    85,    83,    81,
    79,    77,    75,    73,    71,    70,    68,
    66,    64,    62,    60,    58,    56,    55,
    53,    51,    49,    47,    45,    43,    41,
    40,    38,    36,    34 };



GLFWwindow* viewerWindow;

#define MAX_FILE_SIZE (16777216 << 1)
std::vector<unsigned short> fileData;
std::streamsize fileSize;


void symbolRegArg1()
{
    int chars_written = snprintf(buffer_ptr, 16, "%01X: ", (decodeOpcode >> 5) & 7);
    buffer_ptr += chars_written; // Move the pointer forward
}

void symbolRegArg2()
{
    int chars_written = snprintf(buffer_ptr, 16, "%01X: ", (decodeOpcode >> 8) & 7);
    buffer_ptr += chars_written; // Move the pointer forward
}

void FixedImmediate()
{
    int chars_written;
    if (readFromMemory(current_address, temp2) == true) {
        chars_written = snprintf(buffer_ptr, 6, "%04X ", temp2);
    }
    else {
        chars_written = snprintf(buffer_ptr, 6, "???  ");
    }
    buffer_ptr += chars_written; // Move the pointer forward
    current_address++;
}

void QuickImmediate()
{
    int chars_written = snprintf(buffer_ptr, 6, "%02X ", ((decodeOpcode >> 8) & 63) + ((decodeOpcode >> 15) << 6));
    buffer_ptr += chars_written; // Move the pointer forward
}

void jumpOrCall()
{
    int chars_written = 0;
    if (decodeOpcode & (1 << 9))
    {
        chars_written = snprintf(buffer_ptr, 6, "CALL ");
    }
    else
    {
        chars_written = snprintf(buffer_ptr, 6, "JMP ");
    }
    buffer_ptr += chars_written; // Move the pointer forward
}

void NonFixedImmediate()
{
    int chars_written = 0;
    if (decodeOpcode & 32768)
    {
        if (readFromMemory(current_address, temp2) == true && readFromMemory(current_address + 1, temp3) == true)
            chars_written = snprintf(buffer_ptr, 6, "%08X ", temp2 + (temp3 << 16));
        else
            chars_written = snprintf(buffer_ptr, 6, "???");
        current_address += 2;
    }
    else
    {
        if (readFromMemory(current_address, temp2) == true && readFromMemory(current_address + 1, temp3) == true)
        {
            chars_written = snprintf(buffer_ptr, 6, "%04X ", temp2);
        }
        else
            chars_written = snprintf(buffer_ptr, 6, "???");
        current_address++;
    }
    buffer_ptr += chars_written; // Move the pointer forward
}

const char* operationStrings[] =
{
    "ADD",
    "SUB",
    "AND",
    "OR",
    "XOR",
    "SHL",
    "SHR",
    "MUL",
};

void operation()
{
    int chars_written = snprintf(buffer_ptr, 6, operationStrings[(decodeOpcode >> 11) & 7]);
    buffer_ptr += chars_written; // Move the pointer forward
}

const char* comparisonStrings[] =
{
    "LTR",
    "EQR",
    "GRR",
    "undefined",
    "!LTR",
    "!EQR",
    "!GRR",
    "undefined",
};

void comparison()
{
    int chars_written = snprintf(buffer_ptr, 10, comparisonStrings[(decodeOpcode >> 11) & 7]);
    buffer_ptr += chars_written; // Move the pointer forward
}

void IndirectDirectOrImmediate()
{
    int chars_written = 0;
    if (decodeOpcode & (1 << 8))
    {
        chars_written = snprintf(buffer_ptr, 6, "R%01X", ((decodeOpcode >> 5) & 7));
        buffer_ptr += chars_written;
    }
    else
    {
        if (decodeOpcode & 32768)
        {
            if (readFromMemory(current_address, temp2) == true && readFromMemory(current_address + 1, temp3) == true)
                chars_written = snprintf(buffer_ptr, 6, "#%08X ", temp2 + (temp3 << 16));
            else
                chars_written = snprintf(buffer_ptr, 6, "???");
            current_address += 2;
            buffer_ptr += chars_written;
        }
        else
        {
            if (readFromMemory(current_address, temp2) == true && readFromMemory(current_address + 1, temp3) == true)
            {
                chars_written = snprintf(buffer_ptr, 6, "#%04X ", temp2);
            }
            else
                chars_written = snprintf(buffer_ptr, 6, "???");
            current_address++;
            buffer_ptr += chars_written;
        }
    }
}

void (*InstructionSubFunctions[])() =
{
    symbolRegArg1,
    symbolRegArg2,
    FixedImmediate,
    QuickImmediate,
    jumpOrCall,
    NonFixedImmediate,
    operation,
    comparison,
    IndirectDirectOrImmediate
};

//#define symbolRegArg1 0x7F
//#define symbolRegArg2 0x80
//#define FixedImmediate 0x81
//#define QuickImmediate 0x82
//#define jumpOrCall 0x83
//#define NonFixedImmediate 0x84
//#define operation 0x85
//#define comparison 0x86
//#define IndirectOrImmediate 0x87

std::string instructionSubname[] = {
    "NOP",
    "SYNC",

    "\x85 #\x81, R\x7F, R\x80",
    "\x85 #\x81, R\x7F, %R\x80",
    "\x85 R\x7F, R\x80, R\x7F",
    "LD #\x81, R\x80",
    "STR #\x81, %R\x7F",
    "LD QI #\x82, R\x7F",
    "LD R\x7F, R\x80",
    "STR R\x7F, %R\x80",
    "LD %R\x7F, R\x80",
    "STR %R\x7F, %R\x80",
    "STRI R\x7F, %R\x80",
    "LDI %R\x7F, R\x80",
    "STRI %R\x7F, %R\x80",
    "\x83 \x87",
    "CMP R\x7F, R\x80",
    "CMP %R\x7F, %R\x80",
    "JMP \x86 #\x81, \x84",
    "CALL \x86 #\x81, \x86\x84",
    "JMP \x86, #\x84",
    "CALL \x86, #\x84",
    "RET",
    "NOT R\x7F, R\x80",
    "INC R\x7F",
    "DEC R\x7F",
    "INC %R\x7F",
    "DEC %R\x7F",
    "LD INC %R\x7F, R\x80",
    "LD DEC %R\x7F, R\x80",
    "STR INC %R\x7F, %R\x80",
    "STR DEC %R\x7F, %R\x80",
};

void outputInstruction()
{
    buffer_ptr = NumCharbuffer;
    if (breakAddress == current_address)
    {
        snprintf(buffer_ptr, 3, "*");
    }
    else
    {
        snprintf(buffer_ptr, 3, " ");
    }
    buffer_ptr ++;
    if (current_address == storedPC)
    {
        snprintf(buffer_ptr, 3, "> ");
    }
    else
    {
        snprintf(buffer_ptr, 3, "  ");
    }
    buffer_ptr += 2;
    if (readFromMemory(current_address, temp2) == true)
    {
        decodeOpcode = temp2;
        if ((decodeOpcode & (1 << 14)) == 0)
        {
            current_address++;
            buffer_src_ptr = 0;
            while ((unsigned char)instructionSubname[(decodeOpcode & 31)][buffer_src_ptr] > 0)
            {
                if ((unsigned char)instructionSubname[(decodeOpcode & 31)][buffer_src_ptr] < 0x7F)
                {
                    buffer_ptr[0] = instructionSubname[(decodeOpcode & 31)][buffer_src_ptr];
                    buffer_ptr++;
                }
                else
                {
                    InstructionSubFunctions[(unsigned char)instructionSubname[(decodeOpcode & 31)][buffer_src_ptr] - 0x7F]();
                }
                buffer_src_ptr++;
            }
        }
        else
        {
            if (decodeOpcode & (1 << 15))
            {
                snprintf(buffer_ptr, 10, "STR ADD ");
                buffer_ptr += 8;
            }
            else
            {
                snprintf(buffer_ptr, 9, "LD ADD ");
                buffer_ptr += 7;
            }
            if (decodeOpcode & (1 << 3))
            {
                buffer_ptr[0] = 'L';
                buffer_ptr++;
            }
            if ((decodeOpcode & (1 << 15)) == 0)
            {
                snprintf(buffer_ptr, 3, "R");
                buffer_ptr++;
                snprintf(buffer_ptr, 8, "%01X", (decodeOpcode & 7));
                buffer_ptr++;
                buffer_ptr[0] = ' ';
                buffer_ptr++;
            }
            snprintf(buffer_ptr, 3, "%%[");
            buffer_ptr += 2;
            switch (((decodeOpcode >> 7) & 3))
            {
            case 0:
                buffer_ptr[0] = 'R';
                buffer_ptr++;
                break;
            case 1:
                snprintf(buffer_ptr, 3, "%%R");
                buffer_ptr += 2;
                break;
            case 2:
                snprintf(buffer_ptr, 3, "LR");
                buffer_ptr += 2;
                break;
            case 3:
                snprintf(buffer_ptr, 3, "IR");
                buffer_ptr += 2;
                break;
            }
            snprintf(buffer_ptr, 8, "%01X", ((decodeOpcode >> 4) & 7));
            buffer_ptr ++;
            snprintf(buffer_ptr, 3, ", ");
            buffer_ptr += 2;
            switch (((decodeOpcode >> 12) & 3))
            {
            case 0:
                buffer_ptr[0] = 'R';
                buffer_ptr++;
                break;
            case 1:
                snprintf(buffer_ptr, 3, "%%R");
                buffer_ptr += 2;
                break;
            case 2:
                snprintf(buffer_ptr, 3, "LR");
                buffer_ptr += 2;
                break;
            case 3:
                snprintf(buffer_ptr, 3, "IR");
                buffer_ptr += 2;
                break;
            }
            snprintf(buffer_ptr, 8, "%01X", ((decodeOpcode >> 9) & 7));
            buffer_ptr++;
            snprintf(buffer_ptr, 3, "]");
            buffer_ptr++;
            if ((decodeOpcode & (1 << 15)))
            {
                snprintf(buffer_ptr, 5, ", R");
                buffer_ptr += 3;
                snprintf(buffer_ptr, 8, "%01X", (decodeOpcode & 7));
                buffer_ptr++;
            }
            current_address++;
        }
    }
    else
    {
        snprintf(buffer_ptr, 6, "???");
        buffer_ptr += 3;
        current_address++;
    }
    snprintf(buffer_ptr, 6, "\x0");
}

void outputInstructionWithoutBuffer()
{
    buffer_ptr = NumCharbuffer;
    if (readFromMemory(current_address, temp2) == true)
    {
        decodeOpcode = temp2;
        current_address++;
        buffer_src_ptr = 0;
        while ((unsigned char)instructionSubname[(decodeOpcode & 31)][buffer_src_ptr] > 0)
        {
            if ((unsigned char)instructionSubname[(decodeOpcode & 31)][buffer_src_ptr] < 0x7F)
            {

            }
            else
            {
                InstructionSubFunctions[(unsigned char)instructionSubname[(decodeOpcode & 31)][buffer_src_ptr] - 0x7F]();
            }
            buffer_src_ptr++;
        }
    }
    else
    {
        current_address++;
    }
}

void virtualSystemInit()
{
    for (i = 0; i < sizeof(testGBAGFX) >> 1; i ++)
    {
        //VRAM[i] = testGBAGFX[i];
        //VRAM[i] = ((i & 255) << 1) + ((((i & 255) << 1) + 1) << 8);
    }
    for (i = 0; i < sizeof(testGBAPAL) >> 1; i++)
    {
        //palettes[i] = testGBAPAL[i];
        //palettes[i] = (i >> 3) + ((i >> 3) << 5) + ((i >> 3) << 10);
    }
    //OAM[3] = (sizeof(testGBAGFX) >> 1);
    PC = (5 << 24);
    IsLoaded = true;
    CPUFreeze = false;
    APUFreeze = false;
    previousPC = PC;
    stackPointer = 0;
    writeRegs = regBank1;
}

inline void executeDMA()
{
    if (isDMADelayFetched == false)
    {
        DMACycleDelay = getCycleDelay(IO[1] + (IO[2] << 16), false);
        DMACycleDelay += getCycleDelay(IO[3] + (IO[4] << 16), false);
        isDMADelayFetched = true;
    }
    if (DMACycleDelay == 0)
    {
        temp1 = IO[1] + (IO[2] << 16);
        readFromMemory(temp1,temp2);
        if (IO[2] & (1 << 11))
        {
            temp1++;
            IO[1] = temp1;
            IO[2] &= 63488;
            IO[2] |= ((temp1 >> 16) & 2047);
        }
        temp1 = IO[3] + (IO[4] << 16);
        writeToMemory(temp1,temp2);
        temp1++;
        IO[3] = temp1;
        IO[4] = (temp1 >> 16);
        IO[0x05]--;
        isDMADelayFetched = false;
    }
    else
    {
        DMACycleDelay--;
    }
}

inline void executeInstructions()
{
    if (instructionReturn == true)
    {
        if (IO[0x6B] & 32768 && frameInt == true)
        {
            stack[stackPointer & 7] = PC;
            stackPointer++;
            PC = IO[0x6A] + (IO[0x6B] << 16);
            storedPC = PC;
            storedRegBank = writeRegs;
            writeRegs = regBank3;
            frameInt = false;
            return;
        }
        if (isDelayFetched == false)
        {
            if (((PC >> 24) & 7) < 6 && (PC & memSize[(PC >> 24) & 7]) < memSizeNAND[((PC >> 24) & 7)] && isBusy[(PC >> 24) & 7] == 0)
            {
                getOpcode = components[(PC >> 24) & 7][PC & memSize[(PC >> 24) & 7]];
                //std::cout << "opcode:" << instructionSubname[(getOpcode & 31)] << std::endl;
                //std::cout << "address:" << PC << std::endl;
            }
            else
            {
                //std::cout << "(opcode undefined)" << std::endl;
            }
            cycleDelayCount = getCycleDelay(PC, false);
            isDelayFetched = true;
        }

        //std::cout << "register values:" << writeRegs[0] << ", " << writeRegs[1] << ", " << writeRegs[2] << ", " << writeRegs[3] << ", " << writeRegs[4] << ", " << writeRegs[5] << ", " << writeRegs[6] << ", " << writeRegs[7] << ", " << std::endl;
        //PC++;
        //std::cout << "full instruction:" << getOpcode << " & " << components[((PC + 1) >> 24) & 7][(PC + 1) & memSize[((PC + 1) >> 24) & 7]] << std::endl;
        //PC--;
        if (cycleDelayCount == 0)
        {
            instructionReturn = false;
            isDelayFetched = false;
            storedPC = PC;
            PC++;
        }
        if (cycleDelayCount > 0)
        cycleDelayCount--;
    }
        if (instructionReturn == false)
        {
            if (isDelayFetched == false)
            {
                if (getOpcode & (1 << 14))
                {
                    temp1 = 0;
                    cycleDelayCount += 3;
                    if (getOpcode & (1 << 3))
                    {
                        cycleDelayCount += getCycleDelay(((getOpcode) & 7) + ((((getOpcode) + 1) & 7) << 16), false);
                        readFromMemory(writeRegs[((getOpcode) & 7)] + (writeRegs[(((getOpcode) + 1) & 7)] << 16), memBuffer[0]);
                    }
                    if (((getOpcode >> 7) & 3) == 1)
                    {
                        cycleDelayCount += getCycleDelay(((getOpcode >> 4) & 7) + ((((getOpcode >> 4)+1) & 7) << 16), false);
                        readFromMemory((writeRegs[(getOpcode >> 4) & 7]) + (writeRegs[(((getOpcode >> 4) + 1) & 7)] << 16), memBuffer[1]);
                    }
                    if (((getOpcode >> 12) & 3) == 1)
                    {
                        cycleDelayCount += getCycleDelay(((getOpcode >> 9) & 7) + ((((getOpcode >> 9) + 1) & 7) << 16), false);
                        readFromMemory((writeRegs[(getOpcode >> 9) & 7]) + ((writeRegs[((getOpcode >> 9) + 1) & 7]) << 16), memBuffer[2]);
                    }
                    switch (((getOpcode >> 7) & 3))
                    {
                    case 0:
                        temp1 = writeRegs[((getOpcode >> 4) & 7)];
                        break;
                    case 1:
                        temp1 = memBuffer[1];
                        break;
                    case 2:
                        temp1 = writeRegs[((getOpcode >> 4) & 7)] + (writeRegs[(((getOpcode >> 4) + 1) & 7)] << 16);
                        break;
                    case 3:
                        temp1 = writeRegs[((getOpcode >> 4) & 7)];
                        writeRegs[((getOpcode >> 4) & 7)]++;
                        break;
                    }
                    switch (((getOpcode >> 12) & 3))
                    {
                    case 0:
                        temp1 += writeRegs[((getOpcode >> 9) & 7)];
                        break;
                    case 1:
                        temp1 += memBuffer[2];
                        break;
                    case 2:
                        temp1 += writeRegs[((getOpcode >> 9) & 7)] + (writeRegs[(((getOpcode >> 9) + 1) & 7)] << 16);
                        break;
                    case 3:
                        temp1 += writeRegs[((getOpcode >> 9) & 7)];
                        writeRegs[((getOpcode >> 9) & 7)]++;
                        break;
                    }
                    cycleDelayCount += getCycleDelay(temp1, false);
                    LDADDSTRAddress = temp1;
                }
                else
                {
                    currentCCEntry = (getOpcode & 31) << 2;
                    cycleDelayCount = instructionCycleCounts[currentCCEntry];
                    currentCCEntry++;
                    for (CCCount = 1; CCCount < 4; CCCount++, currentCCEntry++)
                    {
                        switch (instructionCycleCounts[currentCCEntry])
                        {
                        case fromMemory:
                            cycleDelayCount += getCycleDelay(PC, false);
                            readFromMemory(PC, memBuffer[CCCount - 1]);
                            if (((PC >> 24) & 7) == 5)
                                instructionFlagBuffer[(PC & 16777215)] = 2;
                            PC++;
                            break;
                        case sameRegOr32bitPenalty:
                            if (((getOpcode >> 5) & 7) == ((getOpcode >> 8) & 7) || (getOpcode & 32768))
                                cycleDelayCount++;
                            break;
                        case toRegMemoryArg1:
                            cycleDelayCount += getCycleDelay(writeRegs[((getOpcode >> 5) & 7)] + (writeRegs[(((getOpcode >> 5) + 1) & 7)] << 16), false);
                            break;
                        case toRegMemoryArg2:
                            cycleDelayCount += getCycleDelay(writeRegs[((getOpcode >> 8) & 7)] + (writeRegs[(((getOpcode >> 8) + 1) & 7)] << 16), false);
                            break;
                        case sameRegPenalty:
                            if (((getOpcode >> 5) & 7) == ((getOpcode >> 8) & 7))
                                cycleDelayCount++;
                            break;
                        case sizeableImmediate:
                            cycleDelayCount += getCycleDelay(PC, false);
                            readFromMemory(PC, memBuffer[CCCount - 1]);
                            if (((PC >> 24) & 7) == 5)
                                instructionFlagBuffer[(PC & 16777215)] = 2;
                            PC++;
                            if ((getOpcode & 32768))
                            {
                                cycleDelayCount += getCycleDelay(PC, false);
                                readFromMemory(PC, memBuffer[CCCount]);
                                if (((PC >> 24) & 7) == 5)
                                    instructionFlagBuffer[(PC & 16777215)] = 2;
                                PC++;
                            }
                            break;
                        case IndirectDirectOrRelative:
                            if (getOpcode & (1 << 8))
                            {
                                cycleDelayCount += getCycleDelay(writeRegs[((getOpcode >> 5) & 7)] + (writeRegs[(((getOpcode >> 5) + 1) & 7)] << 16), false);
                                readFromMemory(writeRegs[((getOpcode >> 5) & 7)] + (writeRegs[(((getOpcode >> 5) + 1) & 7)] << 16), memBuffer[CCCount - 1]);
                                cycleDelayCount += getCycleDelay(writeRegs[((getOpcode >> 5) & 7)] + (writeRegs[(((getOpcode >> 5) + 1) & 7)] << 16) + 1, false);
                                readFromMemory(writeRegs[((getOpcode >> 5) & 7)] + (writeRegs[(((getOpcode >> 5) + 1) & 7)] << 16) + 1, memBuffer[CCCount]);
                            }
                            else
                            {
                                cycleDelayCount += getCycleDelay(PC, false);
                                readFromMemory(PC, memBuffer[CCCount - 1]);
                                PC++;
                                if ((getOpcode & 32768))
                                {
                                    cycleDelayCount += getCycleDelay(PC, false);
                                    readFromMemory(PC, memBuffer[CCCount]);
                                    if (((PC >> 24) & 7) == 5)
                                        instructionFlagBuffer[(PC & 16777215)] = 2;
                                    PC++;
                                }
                            }
                            break;
                        case IndirectOrDirect:
                            if (getOpcode & (1 << 8))
                            {
                                cycleDelayCount += getCycleDelay(writeRegs[((getOpcode >> 5) & 7)] + (writeRegs[(((getOpcode >> 5) + 1) & 7)] << 16), false);
                                readFromMemory(writeRegs[((getOpcode >> 5) & 7)] + (writeRegs[(((getOpcode >> 5) + 1) & 7)] << 16), memBuffer[CCCount - 1]);
                                cycleDelayCount += getCycleDelay(writeRegs[((getOpcode >> 5) & 7)] + (writeRegs[(((getOpcode >> 5) + 1) & 7)] << 16) + 1, false);
                                readFromMemory(writeRegs[((getOpcode >> 5) & 7)] + (writeRegs[(((getOpcode >> 5) + 1) & 7)] << 16) + 1, memBuffer[CCCount]);
                            }
                            else
                            {
                                cycleDelayCount += getCycleDelay(PC, false);
                                readFromMemory(PC, memBuffer[CCCount - 1]);
                                if (((PC >> 24) & 7) == 5)
                                    instructionFlagBuffer[(PC & 16777215)] = 2;
                                PC++;
                                cycleDelayCount += getCycleDelay(PC, false);
                                readFromMemory(PC, memBuffer[CCCount]);
                                if (((PC >> 24) & 7) == 5)
                                    instructionFlagBuffer[(PC & 16777215)] = 2;
                                PC++;
                            }
                            break;
                        case multiplicationPenalty:
                            if (((getOpcode >> 11) & 7) == 7)
                                cycleDelayCount += 10;
                            break;
                        }
                    }
                }
                isDelayFetched = true;
            }
            if (cycleDelayCount == 0)
            {
                if (getOpcode & (1 << 14))
                    handleDMode();
                else
                opcodefunctions[(getOpcode & 31)]();
                instructionReturn = true;
                isDelayFetched = false;
            }
            if (cycleDelayCount > 0)
            cycleDelayCount--;
        }
    //std::cout << "address: " << (PC & 0xFFFFFF) << " component number: " << ((PC >> 24) & 7) << std::endl;
}

void createDebugViewer()
{
    viewerWindow = glfwCreateWindow(512, 512, "1690 debugger", NULL, NULL);
}

void loadFileIntoArray(const char* filePath, std::vector<unsigned short>& data) {
    // Open the file in binary mode
    std::ifstream file(filePath, std::ios::binary);

    if (file) {
        // Determine the file size
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);


        if (fileSize > MAX_FILE_SIZE) {
            std::wcerr << L"Error: File size exceeds the maximum allowed size (16 megabytes)." << std::endl;
            return;
        }

        // Resize the vector to accommodate the file data
        data.resize(static_cast<size_t>(fileSize));

        // Read the file data into the vector
        file.read(reinterpret_cast<char*>(data.data()), fileSize);

        // Close the file
        file.close();
    }
    else {
        // Handle the case where the file couldn't be opened
        std::cerr << "Error opening file: " << filePath << std::endl;
    }
    memSizeNAND[5] = (fileSize >> 1);
    for (i = 0; i < (fileSize >> 1); i ++)
    {
        ROM[i] = fileData[i];
    }
}

void openFileDialogAndLoadFile() {
    Pa_StopStream(stream);
    const char* filterPatterns[] = { "*.bin" }; // Adjust file extensions as needed

    // Show the file dialog to select a file
    const char* filePath = tinyfd_openFileDialog(
        "Select File",
        "",
        1, // Number of filter patterns
        filterPatterns,
        "Game Files (.bin)",
        0
    );

    // Check if the user selected a file
    if (filePath != NULL) {
        // Load the selected file into memory
        loadFileIntoArray(filePath, fileData);
        IsLoaded = false;
        IsFileLoaded = true;
    }
    fileData.resize(0);
    Pa_StartStream(stream);
    ringBufferOffset = outputBufferOffset;
}

int fillBuffer(const void* inputBuffer,     // For audio input (recording), ignore for output
    void* outputBuffer,          // Buffer YOU need to fill with audio data
    unsigned long framesPerBufferArg, // How many frames PortAudio wants YOU to provide
    const PaStreamCallbackTimeInfo* timeInfo, // Timing info
    PaStreamCallbackFlags statusFlags,  // Flags for underruns/overruns
    void* userData) {            // A custom pointer you can pass to your callback

    //if (APUFinished == false)
    //{
    //    std::cerr << "simultaneous buffer access" << std::endl;
    //    return paContinue; // or paComplete if this is the last buffer, paAbort to stop
    //}
    //std::cout << "amount of frames: " << framesPerBufferArg << std::endl;
    //ringBufferOffset -= (framesPerBufferArg << 1);
    //std::cout << "buffer read offset: " << (outputBufferOffset & 2047) << std::endl;
    //std::cout << "buffer write offset: " << (ringBufferOffset & 2047) << std::endl;
    signed int* out = (signed int*)outputBuffer;    // PA wants int32_t samples
    unsigned int bufferPointer1 = (((currentBuffer + 1) & 1) << 1);
    unsigned int bufferPointer2 = bufferPointer1 + 1;
    unsigned short inputBufferOffset = 0;
    unsigned short samplesRead = 0;

    if (IsLoaded == true)
    {
        for (samplesRead = 0; inputBufferOffset < framesPerBufferArg; samplesRead += 2, inputBufferOffset++, outputBufferOffset += 2)
        {
            out[samplesRead] = ringBuffer[(outputBufferOffset & 8191)];
            out[samplesRead + 1] = ringBuffer[((outputBufferOffset + 1) & 8191)];
        }
    }
    else
    {
        for (samplesRead = 0; inputBufferOffset < framesPerBufferArg; samplesRead += 2, inputBufferOffset++)
        {
            out[samplesRead] = 0;
            out[samplesRead + 1] = 0;
        }
    }

    return paContinue; // or paComplete if this is the last buffer, paAbort to stop
}

void audioInit()
{
    Pa_Initialize();
    // Open a PortAudio stream for playback
    Pa_OpenDefaultStream(&stream, 0, 2, SAMPLE_FORMAT, SAMPLE_RATE,
        547, fillBuffer, NULL);
    // Start the audio stream
    Pa_StartStream(stream);
}

void scaleTest(unsigned short angle, signed short scale, signed short width, signed short height)
{
    signed short XScale = int((cos(angle * 0.000191747598486) * scale) + (cos(angle * 0.000191747598486) * ((width * scale) >> 8)));
    signed short XShear = int((-sin(angle * 0.000191747598486) * scale) + (-sin(angle * 0.000191747598486) * ((width * scale) >> 8)));
    signed short YShear = int((sin(angle * 0.000191747598486) * scale) + (sin(angle * 0.000191747598486) * ((height * scale) >> 8)));
    signed short YScale = int((cos(angle * 0.000191747598486) * scale) + (cos(angle * 0.000191747598486) * ((height * scale) >> 8)));
    OAM[4] = XScale;
    OAM[5] = YScale;
    OAM[6] = XShear;
    OAM[7] = YShear;
}

// Callback function to handle key input
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_O) {
        // Key pressed
        openFileDialogAndLoadFile();
    }
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_1)
        {
            if (createWindow == 0)
                createWindow = 1;
        }
    }
    if (action == GLFW_REPEAT)
    {

    }
    // If you want to detect key repeats, you can check action == GLFW_REPEAT
}

void DWkeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_REPEAT) {
        if (key == GLFW_KEY_UP)
        {
            current_address -= 8;
            vram_display_address -= (tileRowWidth << 1);
        }
        if (key == GLFW_KEY_DOWN)
        {
            current_address += 8;
            vram_display_address += (tileRowWidth << 1);
        }
        if (key == GLFW_KEY_LEFT)
        {
            current_address -= 128;
            vram_display_address --;
        }
        if (key == GLFW_KEY_RIGHT)
        {
            current_address += 128;
            vram_display_address ++;
        }
        if (key == GLFW_KEY_S)
        {
            stepCounter = 1;
            PCHalt = PC;
        }
        if (key == GLFW_KEY_Q)
        {
            if (tileRowWidth > 1)
            {
                tileRowWidth--;
            }
        }
        if (key == GLFW_KEY_W)
        {
            if (tileRowWidth < 64)
            {
                tileRowWidth++;
            }
        }
    }
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_0)
        {
            current_address = 0;
            tileBankSelect = false;
        }
        if (key == GLFW_KEY_1)
        {
            current_address = (1 << 24);
            tileBankSelect = true;
        }
        if (key == GLFW_KEY_2)
        {
            current_address = (2 << 24);
        }
        if (key == GLFW_KEY_3)
        {
            current_address = (3 << 24);
        }
        if (key == GLFW_KEY_4)
        {
            current_address = (4 << 24);
        }
        if (key == GLFW_KEY_5)
        {
            current_address = (5 << 24);
        }
        if (key == GLFW_KEY_6)
        {
            current_address = (6 << 24);
        }
        if (key == GLFW_KEY_7)
        {
            current_address = (7 << 24);
        }
        if (key == GLFW_KEY_P)
        {
            stepCounter = 0;
            PCHalt = PC;
        }
        if (key == GLFW_KEY_S)
        {
            stepCounter = 1;
            PCHalt = PC;
        }
        if (key == GLFW_KEY_C)
        {
            stepCounter = -1;
            PCHalt = -1;
        }
        if (key == GLFW_KEY_R)
        {
            virtualSystemInit();
            if (PCHalt > 0)
                PCHalt = PC;
            for (i = 0; i < 128; i++)
            {
                IO[i] = 0;
            }
        }
        if (key == GLFW_KEY_I)
        {
            for (i = 0; i < 8; i++)
            {
                regBank1[i] = 0xFFFF;
                regBank2[i] = 0xFFFF;
            }
            for (i = 0; i < 65536; i++)
            {
                VRAM[i] = 0xFFFF;
            }
            for (i = 0; i < 256; i++)
            {
                palettes[i] = 0xFFFF;
            }
            for (i = 0; i < 128; i++)
            {
                IO[i] = 0xFFFF;
            }
        }
        if (key == GLFW_KEY_O)
        {
            for (i = 0; i < 8; i++)
            {
                regBank1[i] = 0;
                regBank2[i] = 0;
            }
            for (i = 0; i < 65536; i++)
            {
                VRAM[i] = 0;
            }
            for (i = 0; i < 256; i++)
            {
                palettes[i] = 0;
            }
            for (i = 0; i < 128; i++)
            {
                IO[i] = 0;
            }
        }
        if (key == GLFW_KEY_B)
        {
            breakAddress = PC;
        }
        if (key == GLFW_KEY_G)
        {
            current_address_copy = current_address;
            current_address = breakAddress;
            outputInstructionWithoutBuffer();
            breakAddress = current_address;
            current_address = current_address_copy;
        }
        if (key == GLFW_KEY_H)
        {
            breakAddress = -1;
        }
        if (key == GLFW_KEY_T)
        {
            PC += (1 << 24);
        }
        if (key == GLFW_KEY_Q)
        {
            if (tileRowWidth > 1)
            {
                tileRowWidth--;
                //vram_display_address++;
            }
        }
        if (key == GLFW_KEY_W)
        {
            if (tileRowWidth < 64)
            {
                tileRowWidth++;
                //vram_display_address--;
            }
        }
    }
    // If you want to detect key repeats, you can check action == GLFW_REPEAT
}

void dropFunc1()
{
    PSEdropDownID = 1;
}

void dropFunc1point1()
{
    PSEdropDownID = 0;
    stringListStart += 1;
    DISPMode = (stringListStart % 3);
}

void dropFunc1point2()
{
    PSEdropDownID = 0;
    stringListStart += 2;
    DISPMode = (stringListStart % 3);
}

void palSelButtonLeft()
{
    if (DISPPalette > 0)
    DISPPalette--;
}

void palSelButtonRight()
{
    if (DISPPalette < 7)
    DISPPalette++;
}

void tabSelButton1()
{
        currentTab = 0;
}

void tabSelButton2()
{
    currentTab = 1;
}

void tabSelButton3()
{
    currentTab = 2;
    current_address = 0;
}

void metaPaneDraw1()
{
    drawPane(64, 32, 8, 0, 0, mouseDown, 8, dropFunc1, 0);

    drawPane(32, 32, 8, 128, 0, mouseDown, 8, palSelButtonLeft, 0);
    drawPane(32, 32, 128 - 40, 128, 0, mouseDown, 8, palSelButtonRight, 0);

    drawPane(64, 32, 8, 32, 0, mouseDown, 8, dropFunc1point1, 1);
    drawPane(64, 32, 8, 64, 0, mouseDown, 8, dropFunc1point2, 1);
}

void metaPaneDraw2()
{

    drawPane(64, 32, 8, 32, 0, mouseDown, 8, dropFunc1point1, 1);
    drawPane(64, 32, 8, 64, 0, mouseDown, 8, dropFunc1point2, 1);
    drawPane(512, 32, 128, 0, stretchVertical, false, 8, dropFunc1, 0);
}

void metaPaneDraw3()
{

    drawPane(64, 32, 8, 32, 0, mouseDown, 8, dropFunc1point1, 1);
    drawPane(64, 32, 8, 64, 0, mouseDown, 8, dropFunc1point2, 1);
    drawPane(512, 32, 128, 0, stretchVertical, false, 8, dropFunc1, 0);
}

void metaTextDraw1()
{
    drawText(screenToPixelCoordinateX(16), screenToPixelCoordinateY(-8) - 2, 40, 512, 0.6, 64, list1Strings[stringListStart % 3], 0, 0);

    drawText(screenToPixelCoordinateX(16), screenToPixelCoordinateY(-40) - 2, 40, 512, 0.6, 64, list1Strings[(stringListStart + 1) % 3], 1, 0);
    drawText(screenToPixelCoordinateX(16), screenToPixelCoordinateY(-72) - 2, 40, 512, 0.6, 64, list1Strings[(stringListStart + 2) % 3], 1, 0);

    drawText(screenToPixelCoordinateX(48), screenToPixelCoordinateY(-96) - 2, 40, 512, 0.6, 64, "palette", 0, 0);
    drawText(screenToPixelCoordinateX(64), screenToPixelCoordinateY(-136) - 2, 40, 512, 0.6, 64, numStrings[DISPPalette], 0, 0);
    drawText(screenToPixelCoordinateX(20), screenToPixelCoordinateY(-136) - 2, 40, 512, 0.6, 64, "<", 0, 0);
    drawText(screenToPixelCoordinateX(128 - 20 - 8), screenToPixelCoordinateY(-136) - 2, 40, 512, 0.6, 64, ">", 0, 0);
    snprintf(NumString, 16, "address: %04X", vram_display_address);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 16)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
}

void metaTextDraw2()
{
    snprintf(NumString, 16, "R0 bank 0: %04X", regBank1[0]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 16, "R1 bank 0: %04X", regBank1[1]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 16, "R2 bank 0: %04X", regBank1[2]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 2)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 16, "R3 bank 0: %04X", regBank1[3]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 3)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 16, "R4 bank 0: %04X", regBank1[4]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 4)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 16, "R5 bank 0: %04X", regBank1[5]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 5)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 16, "R6 bank 0: %04X", regBank1[6]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 6)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 16, "R7 bank 0: %04X", regBank1[7]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 7)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);

    snprintf(NumString, 16, "R0 bank 1: %04X", regBank2[0]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 9)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 16, "R1 bank 1: %04X", regBank2[1]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 10)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 16, "R2 bank 1: %04X", regBank2[2]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 11)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 16, "R3 bank 1: %04X", regBank2[3]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 12)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 16, "R4 bank 1: %04X", regBank2[4]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 13)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 16, "R5 bank 1: %04X", regBank2[5]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 14)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 16, "R6 bank 1: %04X", regBank2[6]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 15)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 16, "R7 bank 1: %04X", regBank2[7]);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 16)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);

    snprintf(NumString, 16, "PC: %08X", PC & 134217727);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 18)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 20, "CPU usage: %o%%", ((CPUUsage * 255) >> 20));
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 19)) - 2, 40, 512, 0.6, 128, NumString, 0, 0);
    snprintf(NumString, 31, "CPU usage in cycles: \n %o", CPUUsage);
    drawText(screenToPixelCoordinateX(8), screenToPixelCoordinateY(-40 - (16 * 21)) - 2, 40, 512, 0.4, 128, NumString, 0, 0);

    unsigned char lineOffset = 2;
    if (((PC >> 24) & 7) == 5)
    {
        current_address = PC;
        unsigned int storedAddress = current_address;
        unsigned char instrOffsetCount = 0;
        unsigned char instrCount = 0;
        while (instrOffsetCount < 6 && instrCount < 4)
        {
            if (instructionFlagBuffer[current_address & 16777215] == 1)
            {
                instrCount++;
                instrOffsetCount = 0;
                storedAddress = current_address;
            }
            current_address--;
            instrOffsetCount++;
        }
        if (instrOffsetCount >= 6)
            current_address = storedAddress;
        while (current_address < (PC + 0x10))
        {
            if (current_address > (PC - 0x10))
            {
                outputInstruction();
                drawText(screenToPixelCoordinateX(256), screenToPixelCoordinateY(-40 - (16 * lineOffset)) - 2, 40, 512, 0.6, 512, NumCharbuffer, 0, 0);
                lineOffset++;
            }
            else
            {
                outputInstructionWithoutBuffer();
            }
        }
    }
    else
    {
        current_address = (((PC >> 24) & 7) << 24);
        while (current_address < ((PC & memSize[(PC >> 24) & 7]) - 0x10) + (((PC >> 24) & 7) << 24) && current_address < (((PC) & memSize[((PC) >> 24) & 7]) + 0x10) + (((PC >> 24) & 7) << 24))
        {
            outputInstructionWithoutBuffer();
        }
        if (current_address >= (((PC) & memSize[((PC) >> 24) & 7]) + 0x10) + (((PC >> 24) & 7) << 24))
            return;
        while (current_address < (((PC) & memSize[((PC) >> 24) & 7]) + 0x10) + (((PC >> 24) & 7) << 24))
        {
            outputInstruction();
            drawText(screenToPixelCoordinateX(256), screenToPixelCoordinateY(-40 - (16 * lineOffset)) - 2, 40, 512, 0.6, 512, NumCharbuffer, 0, 0);
            lineOffset++;
        }
    }
}

void metaTextDraw3()
{
    drawText(screenToPixelCoordinateX(16), screenToPixelCoordinateY(-8) - 2, 40, 512, 0.6, 64, areaStrings[(current_address >> 24) & 7], 0, 0);

    drawText(screenToPixelCoordinateX(140), screenToPixelCoordinateY(-72) - 2, 20, 512, 0.5, 65535, NumCharbuffer, 0, 15);
}

void (*metaPaneDrawFunctions[])() =
{
    metaPaneDraw1,
    metaPaneDraw2,
    metaPaneDraw3,
};

void (*metaTextDrawFunctions[])() =
{
    metaTextDraw1,
    metaTextDraw2,
    metaTextDraw3,
};

int main(int argc, char** argv)
{
    HWND hwnd = GetConsoleWindow();
    // 3. Hide the window.
    ShowWindow(hwnd, SW_HIDE);
    if (mouseDown == true)
    {
        i++;
    }
    audioInit();
    for (i = 0; i < 65535; i++)
    {
        paletteLookupTable[i][0] = (i & 31) << 3;
        paletteLookupTable[i][1] = ((i >> 5) & 31) << 3;
        paletteLookupTable[i][2] = ((i >> 10) & 31) << 3;
    }
    // Initialize GLFW
    glfwInit();

    // Create a windowed   
    //mode window and its OpenGL context
        window = glfwCreateWindow(512, 480, "1690 emulator", NULL, NULL);

    // Make the window's context   
    //current
        glfwMakeContextCurrent(window);

        GLuint textureID;
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        //glfwSwapInterval(1);
        glfwSetKeyCallback(window, keyCallback);

        //glfwSwapInterval(1);
        //glfwMakeContextCurrent(debugWindow);
        // Set texture parameters
        glGenTextures(1, &fontTextureID);
        glGenTextures(1, &viewWindowtextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 256, 256, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bitmapFont);
        glBindTexture(GL_TEXTURE_2D, fontTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, 16, 16, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, paneTexture);
        glBindTexture(GL_TEXTURE_2D, paneTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, viewWindowtextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);

        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "Error loading texture: " << error << std::endl; // You might want a proper error string lookup
            glBindTexture(GL_TEXTURE_2D, 0); // Unbind before deleting
            glDeleteTextures(1, &fontTextureID);
            textureID = 0;
            return false;
        }

        const double TARGET_FPS = 60.0;
        const auto TARGET_FRAME_DURATION_NS = std::chrono::nanoseconds(static_cast<long long>(1e9 / TARGET_FPS));

        auto next_frame_target_time = std::chrono::high_resolution_clock::now() + TARGET_FRAME_DURATION_NS;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Start timer
        auto start = std::chrono::high_resolution_clock::now();
        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();
        //ImGui::NewFrame();
        if (IsFileLoaded)
        {
            if (IsLoaded == false)
                virtualSystemInit();
            else
            {
                //if (createWindow == 2)
                //ImGui_ImplOpenGL3_NewFrame();
                if (createWindow == 1)
                {
                    debugWindow = glfwCreateWindow(512, 480, "1690 debugger", NULL, window);
                    glfwSetKeyCallback(debugWindow, DWkeyCallback);
                    glfwSetMouseButtonCallback(debugWindow, mouse_button_callback);
                    //glfwMakeContextCurrent(window);
                    //ImGui::Begin("tea 1690 Debugger"); // Creates a new window
                    //ImGui::End();
                    createWindow = 2;
                }
                //FillMemory(secondaryFrameBuffer,sizeof(secondaryFrameBuffer),0);
                unsigned int i = 0;
                //OAM[1] = (OAM[1] + 1) % 1023;
                //OAM[2] = (testValue & 63) | ((testValue & 63) << 8);
                //testValue++;
                //OAM[1021] ++;
                //for (i = 0; i < sizeof(testGBAGFX) >> 1; i++)
                //{
                    //VRAM[((i % 30) + ((i / 30) << 6) + (sizeof(testGBAGFX) >> 1)) & 65535] = i | testFlags;
                //}
                if (currentCycle == 533334)
                {
                    subGlobal = (IO[0x6C] & 31);
                    if (IO[0x6C] & 32)
                        subGlobal = -32 + subGlobal;
                    syncReg = 0;
                    i = 0;
                    IO[0] = 0;
                    IO[0x38] = 0;
                    if (glfwGetKey(window, GLFW_KEY_LEFT))
                        IO[0x38] |= 1;
                    if (glfwGetKey(window, GLFW_KEY_RIGHT))
                        IO[0x38] |= 2;
                    if (glfwGetKey(window, GLFW_KEY_UP))
                        IO[0x38] |= 4;
                    if (glfwGetKey(window, GLFW_KEY_DOWN))
                        IO[0x38] |= 8;
                    if (glfwGetKey(window, GLFW_KEY_S))
                        IO[0x38] |= 16;
                    if (glfwGetKey(window, GLFW_KEY_D))
                        IO[0x38] |= 32;
                    if (glfwGetKey(window, GLFW_KEY_A))
                        IO[0x38] |= 64;
                    if (glfwGetKey(window, GLFW_KEY_W))
                        IO[0x38] |= 128;
                    if (glfwGetKey(window, GLFW_KEY_F))
                        IO[0x38] |= 256;
                    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL))
                        IO[0x38] |= 512;
                    if (glfwGetKey(window, GLFW_KEY_T))
                        IO[0x38] |= 1024;
                    if (glfwGetKey(window, GLFW_KEY_Y))
                        IO[0x38] |= 2048;
                    if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
                    {
                        if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] || state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] < -0.5 || state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] < -0.5)
                            IO[0x38] |= 1;
                        if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] || state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] > 0.5 || state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] > 0.5)
                            IO[0x38] |= 2;
                        if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] || state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] < -0.5 || state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] < -0.5)
                            IO[0x38] |= 4;
                        if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] || state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] > 0.5 || state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] > 0.5)
                            IO[0x38] |= 8;
                        if (state.buttons[GLFW_GAMEPAD_BUTTON_CROSS])
                            IO[0x38] |= 16;
                        if (state.buttons[GLFW_GAMEPAD_BUTTON_CIRCLE])
                            IO[0x38] |= 32;
                        if (state.buttons[GLFW_GAMEPAD_BUTTON_SQUARE])
                            IO[0x38] |= 64;
                        if (state.buttons[GLFW_GAMEPAD_BUTTON_TRIANGLE])
                            IO[0x38] |= 128;
                        if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])
                            IO[0x38] |= 256;
                        if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER])
                            IO[0x38] |= 512;
                        if (state.buttons[GLFW_GAMEPAD_BUTTON_BACK])
                            IO[0x38] |= 1024;
                        if (state.buttons[GLFW_GAMEPAD_BUTTON_START])
                            IO[0x38] |= 2048;
                    }
                    finishedRendering = false;
                    APUFinished = false;
                    bufferOffset = 0;
                    currentBuffer = (currentBuffer + 1) & 1;
                    frameBufferOffset = 0;
                    frameInt = true;
                    currentCycle = 0;
                    CPUUsage = 0;
                    APUUsage = 0;
                    isBusy[2] = 0;
                    isBusy[3] = 0;
                    isBusy[4] = 0;
                    isBusy[6] = 0;
                    IO[0x06] = 0;
                    isComponentBusy[2] = 0;
                }
                while (frameBufferOffset < 61440)
                {
                    if ((currentFrame & 1) == 0)
                    {
                        red = (intermediateBuffer[frameBufferOffset] & 31);
                        green = ((intermediateBuffer[frameBufferOffset] >> 5) & 31);
                        blue = ((intermediateBuffer[frameBufferOffset] >> 10) & 31);
                        red += subGlobal;
                        green += subGlobal;
                        blue += subGlobal;
                        clampPalettes();
                        secondaryFrameBuffer[frameBufferOffset][0] = paletteLookupTable[red + (green << 5) + (blue << 10)][0];
                        secondaryFrameBuffer[frameBufferOffset][1] = paletteLookupTable[red + (green << 5) + (blue << 10)][1];
                        secondaryFrameBuffer[frameBufferOffset][2] = paletteLookupTable[red + (green << 5) + (blue << 10)][2];
                    }
                    else
                    {
                        red = (intermediateBuffer2[frameBufferOffset] & 31);
                        green = ((intermediateBuffer2[frameBufferOffset] >> 5) & 31);
                        blue = ((intermediateBuffer2[frameBufferOffset] >> 10) & 31);
                        red += subGlobal;
                        green += subGlobal;
                        blue += subGlobal;
                        clampPalettes();
                        secondaryFrameBuffer[frameBufferOffset][0] = paletteLookupTable[red + (green << 5) + (blue << 10)][0];
                        secondaryFrameBuffer[frameBufferOffset][1] = paletteLookupTable[red + (green << 5) + (blue << 10)][1];
                        secondaryFrameBuffer[frameBufferOffset][2] = paletteLookupTable[red + (green << 5) + (blue << 10)][2];
                    }
                    frameBufferOffset++;
                }
                //debug
                while (currentCycle < 533334 && (stepCounter == -1 || stepCounter > 0))
                {
                        //controlFlags = IO[0];
                        moduleAccessNum = 255;
                        isAPU = true;
                            if (APUFinished == false && APUFreeze == false && !(currentCycle & 1) && currentCycle > (533334 - 18000))
                            {
                                if (IO[0x07] & 1)
                                {
                                    if ((IO[0x07] & 6) > 0)
                                    {
                                        isAPUPCM = true;
                                    }
                                    isComponentBusy[1] = 1;
                                    if (APUSigCount == IOlatencyAmountInCycles[5])
                                    {
                                        APULogic();
                                    }
                                    else
                                    {
                                        APUSigCount++;
                                    }
                                }
                                else
                                {
                                    while (bufferOffset < FRAMES_PER_BUFFER)
                                    {
                                        ringBuffer[(ringBufferOffset) & 8191] = 0;
                                        ringBuffer[(ringBufferOffset + 1) & 8191] = 0;
                                        ringBufferOffset += 2;
                                        bufferOffset++;
                                    }
                                }
                            }
                        isAPU = false;
                        if (CPUFreeze == false)
                        {
                            if (isAPUPCM == false)
                            {
                                if (IO[0x05] == 0)
                                {
                                    if (syncReg == 0 && CPUFreeze == false && !(currentCycle & 1))
                                    {
                                        executeInstructions();
                                        CPUUsage++;
                                    }
                                }
                                else
                                {
                                    if (!(currentCycle & 1))
                                    {
                                        executeDMA();
                                        CPUUsage++;
                                    }
                                }
                            }
                        }
                        if (finishedRendering == false)
                        {
                            if ((IO[0x06] & 1))
                            {
                                VPULogic();
                                IO[6] |= 4;
                                isBusy[2] = 1;
                                isBusy[3] = 1;
                                isBusy[4] = 1;
                                isBusy[6] = 1;
                                isComponentBusy[2] = 1;
                                VPUUsage++;
                            }
                            else
                            {
                                isBusy[2] = 0;
                                isBusy[3] = 0;
                                isBusy[4] = 0;
                                isBusy[6] = 0;
                                isComponentBusy[2] = 0;
                            }
                        }
                        else
                        {
                            isBusy[2] = 0;
                            isBusy[3] = 0;
                            isBusy[4] = 0;
                            isBusy[6] = 0;
                            IO[0x06] = 0;
                            isComponentBusy[2] = 0;
                            IO[6] &= 65531;
                        }
                        if (APUFinished == true)
                        {
                            if (APUSigCount == IOlatencyAmountInCycles[5])
                            {
                                APUSigCount = 0;
                                isComponentBusy[1] = 0;
                                isAPUPCM = false;
                            }
                            else
                            {
                                APUSigCount++;
                            }
                        }
                        //scaleTest(testCounter,inputScale);
                    currentCycle++;
                    if (((PC >> 24) & 7) == 5)
                    {
                        if (instructionFlagBuffer[PC & 16777215] == 0)
                        instructionFlagBuffer[PC & 16777215] = 1;
                    }
                    if (PC == breakAddress)
                    {
                        stepCounter = 0;
                    }
                    if (PCHalt > 0)
                    {
                        if (PC != PCHalt)
                        {
                            if (stepCounter > 0)
                            {
                                stepCounter--;
                            }
                        }
                    }
                }
                //std::cout << "GPU usage: " << int(VPUUsage * 0.000187499765625) << "%" << std::endl;
                //std::cout << "GPU usage in cycles: " << VPUUsage << std::endl;
                VPUUsage = 0;
                CPUUsage += APUUsage;
                //std::cout << "CPU usage: " << int(CPUUsage * 0.00037499953125058594) << "%" << std::endl;
                //std::cout << "CPU usage in cycles: " << CPUUsage << std::endl;
                //std::cout << "APU usage in cycles: " << APUUsage << std::endl;
                if (finishedRendering == true)
                currentFrame++;
            }
        }

        glfwMakeContextCurrent(window);

        // Render here  
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 240, 0, GL_RGB, GL_UNSIGNED_BYTE, secondaryFrameBuffer);
        // Set palette
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);
        // Render the first texture (RGB)

        glBegin(GL_QUADS);
        glVertex2d(-1, -1);
        glTexCoord2d(1, 1);
        glVertex2d(1, -1);
        glTexCoord2d(1, 0);
        glVertex2d(1, 1);
        glTexCoord2d(0, 0);
        glVertex2d(-1, 1);
        glTexCoord2d(0, 1);
        glEnd();

        // Bind the RGB texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        //ImGui::Render();

        // Swap front and back buffers
        if (finishedRendering == true)
        {
            glfwSwapBuffers(window);
        }

        if (createWindow == 2)
        {
            glfwMakeContextCurrent(debugWindow);
            glfwGetWindowSize(debugWindow, &Vwidth, &Vheight);
            glViewport(0, 0, Vwidth, Vheight);
            glEnable(GL_TEXTURE_2D);
            glDisable(GL_BLEND);
            glBindTexture(GL_TEXTURE_2D, fontTextureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glClearColor(0.0,0.0,0.0,1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            current_address_copy = current_address;
            buffer_ptr = NumCharbuffer;

            // 2. Loop through the rows...
            for (int row = 0; row < 16; ++row) {
                // ... calculate current_row_address ...

                // 3. Use sprintf to write the address part. It returns the number of characters written.
                int chars_written = snprintf(buffer_ptr, 16, "%08X: ", current_address_copy);
                buffer_ptr += chars_written; // Move the pointer forward

                // 4. Loop through the columns...
                for (int col = 0; col < 8; ++col) {
                    // ... read the memory value ...
                    if (readFromMemoryUnprotected(current_address_copy, temp2) == true) {
                        chars_written = snprintf(buffer_ptr, 6, "%04X ", temp2);
                    }
                    else {
                        chars_written = snprintf(buffer_ptr, 6, "???  ");
                    }
                    buffer_ptr += chars_written; // Move the pointer forward again
                    current_address_copy++;
                }

                // 5. Add the newline.
                chars_written = snprintf(buffer_ptr, 2, "\n");
                buffer_ptr += chars_written;
            }
            vram_display_address_copy = vram_display_address;
            if (DISPMode == 2 && currentTab == 0)
            vram_display_address += (64 - tileRowWidth);
            unsigned int secondaryFBOffset = 0;
            if (currentTab == 0)
            {
                switch (DISPMode)
                {
                case 0:
                    for (i = 0; i < 65536; i++)
                    {
                        VWFrameBuffer[i][0] = paletteLookupTable[palettes[fetchVRAMByte(i << 8, vram_display_address << 8) + (DISPPalette << 4)]][0];
                        VWFrameBuffer[i][1] = paletteLookupTable[palettes[fetchVRAMByte(i << 8, vram_display_address << 8) + (DISPPalette << 4)]][1];
                        VWFrameBuffer[i][2] = paletteLookupTable[palettes[fetchVRAMByte(i << 8, vram_display_address << 8) + (DISPPalette << 4)]][2];
                    }
                    break;
                case 1:
                    for (i = 0; i < (65536 << 1); i++)
                    {
                        VWFrameBuffer[(((i >> 6) << 3) & 255) + ((i >> 12) << 11) + tileOffsetLT[i & 63]][0] = paletteLookupTable[palettes[fetchVRAMByte((i << 8) + (vram_display_address << 14), 0) + (DISPPalette << 4)]][0];
                        VWFrameBuffer[(((i >> 6) << 3) & 255) + ((i >> 12) << 11) + tileOffsetLT[i & 63]][1] = paletteLookupTable[palettes[fetchVRAMByte((i << 8) + (vram_display_address << 14), 0) + (DISPPalette << 4)]][1];
                        VWFrameBuffer[(((i >> 6) << 3) & 255) + ((i >> 12) << 11) + tileOffsetLT[i & 63]][2] = paletteLookupTable[palettes[fetchVRAMByte((i << 8) + (vram_display_address << 14), 0) + (DISPPalette << 4)]][2];
                    }
                    break;
                case 2:
                    for (i = 0; i < (65536 << 2); i++)
                    {
                        if (((i >> 6) & 63) < tileRowWidth)
                        {
                            VWFrameBuffer[(((i >> 6) << 3) & 511) + ((i >> 13) << 12) + tileOffsetLT2[i & 63]][0] = paletteLookupTable[palettes[fetchVRAMByte((((VRAM[((secondaryFBOffset >> 6) + vram_display_address) & 32767] & 1023) + (tileBankSelect << 10)) << 14) + ((secondaryFBOffset & 63) << 8), 0) + (((VRAM[((secondaryFBOffset >> 6) + vram_display_address) & 32767] >> 10) & 7) << 4)]][0];
                            VWFrameBuffer[(((i >> 6) << 3) & 511) + ((i >> 13) << 12) + tileOffsetLT2[i & 63]][1] = paletteLookupTable[palettes[fetchVRAMByte((((VRAM[((secondaryFBOffset >> 6) + vram_display_address) & 32767] & 1023) + (tileBankSelect << 10)) << 14) + ((secondaryFBOffset & 63) << 8), 0) + (((VRAM[((secondaryFBOffset >> 6) + vram_display_address) & 32767] >> 10) & 7) << 4)]][1];
                            VWFrameBuffer[(((i >> 6) << 3) & 511) + ((i >> 13) << 12) + tileOffsetLT2[i & 63]][2] = paletteLookupTable[palettes[fetchVRAMByte((((VRAM[((secondaryFBOffset >> 6) + vram_display_address) & 32767] & 1023) + (tileBankSelect << 10)) << 14) + ((secondaryFBOffset & 63) << 8), 0) + (((VRAM[((secondaryFBOffset >> 6) + vram_display_address) & 32767] >> 10) & 7) << 4)]][2];
                            secondaryFBOffset++;
                        }
                        else
                        {
                            VWFrameBuffer[(((i >> 6) << 3) & 511) + ((i >> 13) << 12) + tileOffsetLT2[i & 63]][0] = 0;
                            VWFrameBuffer[(((i >> 6) << 3) & 511) + ((i >> 13) << 12) + tileOffsetLT2[i & 63]][1] = 0;
                            VWFrameBuffer[(((i >> 6) << 3) & 511) + ((i >> 13) << 12) + tileOffsetLT2[i & 63]][2] = 0;
                        }
                    }
                    break;
                default:
                    for (i = 0; i < (65536 << 1); i++)
                    {
                        VWFrameBuffer[i][0] = 0;
                        VWFrameBuffer[i][1] = 0;
                        VWFrameBuffer[i][2] = 0;
                    }
                }
                vram_display_address = vram_display_address_copy;
                //testValue++;

                // Render here  
                if (DISPMode == 2)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, VWFrameBuffer);
                else
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, VWFrameBuffer);
                glColor3f(1.0f, 1.0f, 1.0f); // White, so we don't tint the texture
                // Set palette
                // Render the first texture (RGB)
                glBegin(GL_QUADS);
                glVertex2d(screenToPixelCoordinateX(128), 0);
                glTexCoord2d(0, 0);
                glVertex2d(0, 0);
                glTexCoord2d(0, 0);
                glVertex2d(0, 0);
                glTexCoord2d(0, 0);
                glVertex2d(screenToPixelCoordinateX(128), 0);
                glTexCoord2d(0, 1);

                glVertex2d(screenToPixelCoordinateX(128), -1);
                glTexCoord2d(1, 1);
                glVertex2d(1, -1);
                glTexCoord2d(1, 0);
                glVertex2d(1, screenToPixelCoordinateY(32));
                glTexCoord2d(0, 0);
                glVertex2d(screenToPixelCoordinateX(128), screenToPixelCoordinateY(32));
                glTexCoord2d(0, 1);
                glEnd();
                //glBindTexture(GL_TEXTURE_2D, 0);
            }
            glBindTexture(GL_TEXTURE_2D, viewWindowtextureID);

            glEnable(GL_BLEND);
            glEnable(GL_TEXTURE_2D);
            glColor4f(1.0, 1.0, 1.0, 1.0); // Use RGBA white for proper blending
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the correct blend function

            //drawPane(720, 128, 0, 480 - 128, anchorVertical | stretchHorizontal, false, 8, placeholder, 0);
            //drawPane(128, 480, 720 - 128, 0, anchorHorizontal | stretchVertical, false, 8, placeholder, 0);
            drawPane(128, 480, 0, 0, stretchVertical, false, 8, placeholder, 0);

            if (currentTab < 3)
            metaPaneDrawFunctions[currentTab]();
            drawPane(64, 32, 128, 0, 0, mouseDown, 8, tabSelButton1, 0);
            drawPane(64, 32, 128 + 64, 0, 0, mouseDown, 8, tabSelButton2, 0);
            drawPane(64, 32, 256, 0, 0, mouseDown, 8, tabSelButton3, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, 16, 16, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, paneTexture);
            glBindTexture(GL_TEXTURE_2D, 1);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 256, 256, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bitmapFont);
            glBindTexture(GL_TEXTURE_2D, 1);
            glEnable(GL_BLEND);
            glEnable(GL_TEXTURE_2D);
            glColor4f(0, 0, 0, 1.0); // Use RGBA white for proper blending
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the correct blend function

            drawText(screenToPixelCoordinateX(128 + 8), screenToPixelCoordinateY(-8) - 2, 40, 512, 0.6, 64, "VRAM", 0, 0);
            drawText(screenToPixelCoordinateX(128 + 64 + 8), screenToPixelCoordinateY(-8) - 2, 40, 512, 0.6, 64, "CPU", 0, 0);
            drawText(screenToPixelCoordinateX(256 + 4), screenToPixelCoordinateY(-8) - 2, 40, 512, 0.6, 64, "memory", 0, 0);

            if (currentTab < 3)
                metaTextDrawFunctions[currentTab]();
            else
                drawText(screenToPixelCoordinateX(32), screenToPixelCoordinateY(-64) - 2, 40, 512, 1, 64, "error", 0, 0);

            foreverCounter += 0.01;

            //glBindTexture(GL_TEXTURE_2D, 0);

            mouseDown = false;
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            glfwSwapBuffers(debugWindow);
            glfwPollEvents();
            if (glfwWindowShouldClose(debugWindow))
            {
                glfwDestroyWindow(debugWindow);
                createWindow = 0;
            }
        }
        glfwMakeContextCurrent(window);

        // Poll for and process events
        glfwPollEvents();

        // --- Calculate Time to Wait ---
        auto current_time = std::chrono::high_resolution_clock::now();
        auto time_to_wait = next_frame_target_time - current_time; // + audio_sync_sleep_adjustment;

        std::cout << "the margin: " << outputBufferOffset - ringBufferOffset << std::endl;

        signed long margin = outputBufferOffset - ringBufferOffset;
        unsigned int microWait = 0;

        if (margin > (547 << 3))
        {
            microWait = 2000 * (margin / (547 << 3));
        }
        else
        if (margin > (547 << 2))
        {
            microWait = 1000;
        }
        else
        {
            microWait = 0;
        }

        auto FRAME_DIFFERENCE = std::chrono::microseconds(microWait);


        if (time_to_wait.count() > 0) {
            // Only sleep if it's a significant duration to avoid OS sleep granularity issues
            if (time_to_wait > std::chrono::microseconds(100)) { // e.g., > 0.1ms
                std::this_thread::sleep_for(time_to_wait - FRAME_DIFFERENCE);
            }
            else {
                // For very short waits, yield or busy-wait if extreme precision is needed (and CPU cost is acceptable)
                // std::this_thread::yield(); // Good general practice for tiny sleeps
            }
        }
        else {
            // We missed the target for this frame (frame took too long).
            // The 'next_frame_target_time' will still advance by one TARGET_FRAME_DURATION,
            // meaning we are trying to "catch up" on the next frame if it's faster.
            // If chronically missing, indicates emulator is too slow.
            // One strategy here is to advance next_frame_target_time by more than one frame
            // if we are VERY far behind (a "panic" frame skip for the timer).
            if (time_to_wait < -TARGET_FRAME_DURATION_NS) { // If we are more than one full frame behind
                // std::cout << "Timer panic! Resetting next_frame_target_time." << std::endl;
                next_frame_target_time = current_time + TARGET_FRAME_DURATION_NS - FRAME_DIFFERENCE; // Resync to now + 1 frame
            }
        }

        // Update target for the *next* frame, regardless of whether we slept or overran.
        // This helps prevent accumulation of error if we consistently overrun.
        // However, if 'current_time' is already past 'next_frame_target_time',
        // simply adding TARGET_FRAME_DURATION_NS to the old target will make us fall further behind
        // if the overrun was large. A common strategy is to set the next target from current_time
        // if we overran, or from the previous target if we met it.

        // A more robust way to set next_frame_target_time to avoid drift if current_time > next_frame_target_time:
        if (current_time > next_frame_target_time) { // We overran the last frame's target
            next_frame_target_time = current_time + TARGET_FRAME_DURATION_NS - FRAME_DIFFERENCE; // Schedule next frame from now
        }
        else { // We met or were early for the last frame's target
            next_frame_target_time += TARGET_FRAME_DURATION_NS - FRAME_DIFFERENCE; // Schedule next frame from previous target
        }
    }

    // Clean up
    programTerminate = true;
    glfwTerminate();
    return
        0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
