// NeoGC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string>
#include <bitset>
#include <chrono>
#include <thread>
#include <Windows.h>
#include "tinyfiledialogs.h"

bool IsFileLoaded;
std::string headerStr;
unsigned int fileSizeCount;
unsigned int storedFileSizeCount;
unsigned int storedFileSizeCount2;
unsigned int lineIndex;
unsigned int inputFileOffset;
unsigned int charFileOffset;
unsigned char charSize;
unsigned char instructionIndex;
unsigned char tokenIndex;
unsigned char rowSize;
bool commentMode;
unsigned short outputInstruction;
bool match;
bool codeMatch;
unsigned char codeIndex;
unsigned char comparisonIndex;
unsigned char stringIndex;
unsigned short getNumberValue;
unsigned int getNumberValue32;
unsigned char hasValueAppended;
unsigned int lineNumber = 1;
unsigned int symbolIndex;
bool isHex;
bool isSymbol;
unsigned char filePathLength;
unsigned int fileIncludeOffset;
unsigned int storedFileOffset;
unsigned int storedFileOffset2;
unsigned int symbolSourceIndex;
unsigned int symbolDestinationIndex;
bool instructionEnable;
bool isAreaReferenced;
bool isJumpInstr;
bool isStringIncomplete;
bool isLimitRemoved;
bool quickImmediateEnable = true;

#define MAX_FILE_SIZE (1 << 19)
std::vector<unsigned char> fileData;
std::vector<unsigned char> fileIncludeData;
char fileName[65];
unsigned short outputFile[32000000];
signed char compareBuffer[17];
const char* filePath;
std::streamsize fileSize;
std::streamsize fileTempSize;

#define codeFlag 128

#define Nop 0
#define Sync 1
#define Load 2
#define Store 3
#define StoreAndIncrement 4
#define LoadAndIncrement 5
#define Jump 6
#define Compare 7
#define Call 8
#define Return 9
#define Increment 10
#define Decrement 11
#define Comma 12

bool charNumLT[] =
{
    false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
    false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
    false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
    true,true,true,true,true,true,true,true,true,true,false,false,false,false,false,false,false,
    true,true,true,true,true,true,true,true,true,true,true,true,true,
    true,true,true,true,true,true,true,true,true,true,true,true,true,
    false,false,false,false,false,false,
    true,true,true,true,true,true,true,true,true,true,true,true,true,
    true,true,true,true,true,true,true,true,true,true,true,true,true,
    false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
    false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
    false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
    false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
    false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
    false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
    false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
    false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
    false,false,false,false,false,

};

std::string codes[] =
{
    "Mode",
    "Comparison",
    "RegNumberArg1",
    "RegNumberArg2",
    "ImmediateNumber",
    "QuickImmediateNumber",
    "dupeRegArgNumber",
    "DMRegNumberArg1",
    "DMRegNumberArg2",
    "DMRegNumberArg3",
    "1BitModeSlot1",
    "2BitModeSlot2",
    "2BitModeSlot3",
    "arithmeticType",
    "sizeModeJ",
    "jumpValue",
    "fixedJumpValue",
    "jumpValueNoIndirect",
    "JMPORCALL",
    "SwapNoSwap",
    "QIOrInt"
};

std::string comparisonTokens[] =
{
    "LTR",
    "EQR",
    "GRR",
    "NOP"
};

std::string arithmeticTokens[] =
{
    "ADD",
    "SUB",
    "AND",
    "OR",
    "XOR",
    "SHL",
    "SHR",
    "MUL"
};

unsigned char orderTable[] =
{
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    24,
    25,
    26,
    27,
    28,
    29,
    30,
    31,
    32,
    33
};

std::vector<std::string> stringTokens[] =
{
    {"NOP"},
    {"SYNC"},
    {"arithmeticType", "Mode", " #", "ImmediateNumber", ", R", "RegNumberArg1", ", R", "RegNumberArg2"},       //2
    {"arithmeticType", "Mode", " #", "ImmediateNumber", ", R", "RegNumberArg1", ", %R", "RegNumberArg2"},       //3
    {"arithmeticType", "Mode", " R", "RegNumberArg1", ", R", "RegNumberArg2", ", R", "dupeRegArgNumber"},       //4
    {"LD #", "ImmediateNumber", ", R", "RegNumberArg1"},       //5
    {"STR #", "ImmediateNumber", ", %R", "RegNumberArg1"},       //6
    {"LD QI #", "QuickImmediateNumber", ", R", "RegNumberArg1"},       //7
    {"LD R", "RegNumberArg2", ", R", "RegNumberArg1"},       //8
    {"STR R", "RegNumberArg1", ", %R", "RegNumberArg2"},       //9
    {"LD %R", "RegNumberArg1", ", R", "RegNumberArg2"},       //10
    {"STR %R", "RegNumberArg1", ", %R", "RegNumberArg2"},       //11
    {"STRI R", "RegNumberArg1", ", %R", "RegNumberArg2"},       //12
    {"LDI %R", "RegNumberArg1", ", R", "RegNumberArg2"},       //13
    {"STRI %R", "RegNumberArg1", ", %R", "RegNumberArg2"},       //14
    {"JMPORCALL", "SwapNoSwap", "sizeModeJ", " ", "jumpValue"},       //15
    {"CMP R", "RegNumberArg1", ", R", "RegNumberArg2"},       //16
    {"CMP %R", "RegNumberArg1", ", %R", "RegNumberArg2"},       //17
    {"JMP", "sizeModeJ", " ", "Comparison", " #", "ImmediateNumber", ", R", "RegNumberArg1", ", ", "jumpValueNoIndirect"},       //18
    {"CALL ", "SwapNoSwap", "Comparison", " #", "ImmediateNumber", ", R", "RegNumberArg1", ", ", "fixedJumpValue"},       //19
    {"JMP", "sizeModeJ", " ", "Comparison", ", ", "jumpValue"},       //20
    {"CALL ", "SwapNoSwap", "Comparison", ", ", "fixedJumpValue"},       //21
    {"RET", "QIOrInt", "QuickImmediateNumber"},       //22
    {"NOT R", "RegNumberArg1", ", R", "RegNumberArg2"},       //23
    {"INC R", "RegNumberArg1"},       //24
    {"DEC R", "RegNumberArg1"},       //25
    {"INC %R", "RegNumberArg1"},       //26
    {"DEC %R", "RegNumberArg1"},       //27
    {"LD INC %R", "RegNumberArg1", ", R", "RegNumberArg2"},       //28
    {"LD DEC %R", "RegNumberArg1", ", R", "RegNumberArg2"},       //29
    {"STR INC %R", "RegNumberArg1", ", %R", "RegNumberArg2"},       //30
    {"STR DEC %R", "RegNumberArg1", ", %R", "RegNumberArg2"},       //31
    {"STR ADD ", "1BitModeSlot1", "R", "DMRegNumberArg1", ", %[", "2BitModeSlot2", "R", "DMRegNumberArg2", ", ", "2BitModeSlot3", "R", "DMRegNumberArg3", "]"},       //undefined
    {"LD ADD %[", "2BitModeSlot2", "R", "DMRegNumberArg2", ", ", "2BitModeSlot3", "R", "DMRegNumberArg3", "], ", "1BitModeSlot1", "R", "DMRegNumberArg1"},       //undefined
};

signed char numberString[16];
signed char symbolString[65];
signed char filePathString[256];

struct SymbolShort {
    std::string name;
    uint32_t index;  // 32-bit index to some location in data
};

struct SymbolLong {
    std::string name;
    uint32_t index;  // 32-bit index to some location in data
    uint32_t jumpOffsetIndex;
    bool lowHigh;
    bool isFullAddress;
    bool isOffset;
    signed int additiveOffset;
};

std::vector<SymbolShort> areaDeclarations;
std::vector<SymbolShort> symbolSources;
std::vector<SymbolLong> symbolDestinations;

inline void processFileString()
{
    for (charSize = 0; charSize < 16; charSize++)
    {
        if (inputFileOffset < fileSize)
        {
            if (std::isalpha(fileData[inputFileOffset + charSize]))
                compareBuffer[charSize] = (fileData[inputFileOffset + charSize]) & 0xDF;
            else
                compareBuffer[charSize] = fileData[inputFileOffset + charSize];
        }
        else
        {
            isStringIncomplete = true;
            return;
        }
    }
}

void checkAreaRefLow(bool isQI)
{
    isAreaReferenced = false;
    if (std::strncmp(".L|", reinterpret_cast<const char*>(compareBuffer), 3) == 0)
    {
        inputFileOffset += 3;
        memset(symbolString,' ', 64);
        stringIndex = 0;
        while (fileData[inputFileOffset] != ',' && stringIndex < 64)
        {
            symbolString[stringIndex] = fileData[inputFileOffset];
            inputFileOffset++;
            stringIndex++;
        }
        symbolIndex = 0;
        bool isAreaFound = false;
        while (symbolIndex < areaDeclarations.size())
        {
            if (std::strncmp(reinterpret_cast<const char*>(symbolString), areaDeclarations[symbolIndex].name.c_str(), stringIndex) == 0)
            {
                isAreaFound = true;
                getNumberValue32 = areaDeclarations[symbolIndex].index;
                if (isQI == false)
                {
                    outputFile[fileSizeCount] = areaDeclarations[symbolIndex].index;
                    isAreaReferenced = true;
                    fileSizeCount++;
                }
                else
                {
                    outputInstruction |= ((areaDeclarations[symbolIndex].index & 63) << 8) + ((areaDeclarations[symbolIndex].index & 64) << 9);
                    isAreaReferenced = true;
                }
                return;
            }
            symbolIndex++;
        }
        if (isAreaFound == false)
        {
            std::cerr << "Error: section not found: " << std::endl;
            return;
        }
    }
}

void checkAreaRefHigh()
{
    isAreaReferenced = false;
    if (std::strncmp(".H|", reinterpret_cast<const char*>(compareBuffer), 3) == 0)
    {
        inputFileOffset += 3;
        memset(symbolString,' ', 64);
        stringIndex = 0;
        while (fileData[inputFileOffset] != ',' && stringIndex < 64)
        {
            symbolString[stringIndex] = fileData[inputFileOffset];
            inputFileOffset++;
            stringIndex++;
        }
        symbolIndex = 0;
        bool isAreaFound = false;
        while (symbolIndex < areaDeclarations.size())
        {
            if (std::strncmp(reinterpret_cast<const char*>(symbolString), areaDeclarations[symbolIndex].name.c_str(), stringIndex) == 0)
            {
                isAreaFound = true;
                getNumberValue32 = areaDeclarations[symbolIndex].index;
                outputFile[fileSizeCount] = (areaDeclarations[symbolIndex].index >> 16);
                isAreaReferenced = true;
                fileSizeCount++;
                return;
            }
            symbolIndex++;
        }
        if (isAreaFound == false)
        {
            std::cerr << "Error: section not found: " << std::endl;
            return;
        }
    }
}

void stringToSymbol()
{
    inputFileOffset++;
    memset(symbolString,' ', 64);
    stringIndex = 0;
    while (fileData[inputFileOffset] > 0x20 && stringIndex < 64)
    {
        symbolString[stringIndex] = fileData[inputFileOffset];
        inputFileOffset++;
        stringIndex++;
    }
    symbolIndex = 0;
    while (symbolIndex < symbolSources.size())
    {
        if (std::strncmp(reinterpret_cast<const char*>(symbolString), symbolSources[symbolIndex].name.c_str(), stringIndex) == 0)
        {
            std::cerr << "Error: duplicate symbol: " << symbolSources[symbolIndex].name.c_str() << std::endl;
            return;
        }
        symbolIndex++;
    }
    symbolSources.push_back({ reinterpret_cast<const char*>(symbolString),fileSizeCount + (5 << 24)});
}

signed int stringToNumber()
{
    memset(numberString,0,16);
    stringIndex = 0;
    isHex = false;
    bool isNegative = false;
    if (std::strncmp("0X", reinterpret_cast<const char*>(compareBuffer), 2) == 0 || std::strncmp("0x", reinterpret_cast<const char*>(compareBuffer), 2) == 0)
    {
        isHex = true;
        inputFileOffset += 2;
    }
    if (fileData[inputFileOffset] == '-')
    {
        isNegative = true;
        inputFileOffset++;
    }
    while (charNumLT[fileData[inputFileOffset]] == true && stringIndex < 14)
    {
        numberString[stringIndex] = fileData[inputFileOffset];
        inputFileOffset++;
        stringIndex++;
    }
    if (isHex == true)
    {
        //if (std::stoul(reinterpret_cast<const char*>(numberString), nullptr, 16) > 65535)
        //{
        //    std::cerr << "Error: number too big (line " << lineNumber << ")" << std::endl;
        //    while (1)
        //    {

        //    }
        //    return NULL;
        //}
        return std::stoul(reinterpret_cast<const char*>(numberString), nullptr, 16);
    }
    else
    {
        if (stringIndex == 0)
        {
            std::cerr << "Error: not a number (" << fileData[inputFileOffset] << ") line " << lineNumber << std::endl;
            return NULL;
        }
        //if (std::stoul(reinterpret_cast<const char*>(numberString)) > 65535)
        //{
        //    std::cerr << "Error: number too big (line " << lineNumber << ")" << std::endl;
        //    while (1)
        //    {
        //
        //    }
        //    return NULL;
        //}
        if (isNegative)
            return 0 - (std::stoul(reinterpret_cast<const char*>(numberString)));
        else
            return std::stoul(reinterpret_cast<const char*>(numberString));
    }
}

void stringToSection()
{
    inputFileOffset += 9;
    memset(symbolString,' ', 64);
    stringIndex = 0;
    while (fileData[inputFileOffset] > ' ' && stringIndex < 64)
    {
        symbolString[stringIndex] = fileData[inputFileOffset];
        inputFileOffset++;
        stringIndex++;
    }
    inputFileOffset++;
    processFileString();
    symbolIndex = 0;
    while (symbolIndex < areaDeclarations.size())
    {
        if (std::strncmp(reinterpret_cast<const char*>(symbolString), areaDeclarations[symbolIndex].name.c_str(), stringIndex) == 0)
        {
            std::cerr << "Error: section declared twice: " << symbolSources[symbolIndex].name.c_str() << std::endl;
            return;
        }
        symbolIndex++;
    }
    getNumberValue32 = stringToNumber();
    areaDeclarations.push_back({ reinterpret_cast<const char*>(symbolString),getNumberValue32 });
}

void stringToName()
{
    inputFileOffset += 6;
    memset(fileName, 0, 64);
    stringIndex = 0;
    while ((charNumLT[fileData[inputFileOffset]] == true || fileData[inputFileOffset] == '.') && stringIndex < 64)
    {
        fileName[stringIndex] = fileData[inputFileOffset];
        inputFileOffset++;
        stringIndex++;
    }
}

void sizeMode()
{
    if (compareBuffer[0] == 'L')
    {
        outputInstruction |= 32768;
        inputFileOffset++;
    }
}

void sizeModeJ()
{
    if (compareBuffer[0] == 'R' && compareBuffer[1] == ' ')
    {
        inputFileOffset++;
    }
    else
    {
        outputInstruction |= 32768;
        //inputFileOffset++;
    }
}

void comparisonType()
{
    comparisonIndex = 0;
    codeMatch = false;
    if (compareBuffer[0] == '!')
    {
        outputInstruction |= (1 << 13);
        inputFileOffset++;
        processFileString();
    }
    while (comparisonIndex < 3 && codeMatch == false)
    {
        if (std::strncmp(comparisonTokens[comparisonIndex].c_str(), reinterpret_cast<const char*>(compareBuffer), 3) == 0)
        codeMatch = true;
        comparisonIndex++;
    }
    if (codeMatch == false)
    {
        //std::cerr << "Error: invalid comparison type (line " << lineNumber << ")" << std::endl;
    }
    else
    {
        comparisonIndex--;
        outputInstruction |= (comparisonIndex << 11);
        inputFileOffset += 3;
    }
}

void arithmeticType()
{
    comparisonIndex = 0;
    codeMatch = false;
    while (comparisonIndex < 7 && codeMatch == false)
    {
        if (std::strncmp(arithmeticTokens[comparisonIndex].c_str(), reinterpret_cast<const char*>(compareBuffer), arithmeticTokens[comparisonIndex].size()) == 0)
            codeMatch = true;
        comparisonIndex++;
    }
    if (codeMatch == false)
    {
        //std::cerr << "Error: invalid arithmetic type (line " << lineNumber << ")" << std::endl;
        //while (1)
        //{
        //
        //}
    }
    else
    {
        comparisonIndex--;
        outputInstruction |= (comparisonIndex << 11);
        inputFileOffset += arithmeticTokens[comparisonIndex].size();
    }
}

void registerArgument1()
{
    getNumberValue = stringToNumber();
    if (getNumberValue < 8)
    {
        outputInstruction &= 65311;
        outputInstruction |= (getNumberValue << 5);
    }
    else
    {
        std::cerr << "Error: invalid register (line " << lineNumber << ")" << std::endl;
    }
}

void registerArgument2()
{
    getNumberValue = stringToNumber();
    if (getNumberValue < 8)
    {
        outputInstruction &= 63743;
        outputInstruction |= (getNumberValue << 8);
    }
    else
    {
        std::cerr << "Error: invalid register (line " << lineNumber << ")" << std::endl;
    }
}

void DMRegisterArgument1()
{
    outputInstruction &= 65528;
    getNumberValue = stringToNumber();
    if (getNumberValue < 8)
    {
        outputInstruction |= (getNumberValue);
    }
    else
    {
        std::cerr << "Error: invalid register (line " << lineNumber << ")" << std::endl;
    }
}

void DMRegisterArgument2()
{
    outputInstruction &= 65423;
    getNumberValue = stringToNumber();
    if (getNumberValue < 8)
    {
        outputInstruction |= (getNumberValue << 4);
    }
    else
    {
        std::cerr << "Error: invalid register (line " << lineNumber << ")" << std::endl;
    }
}

void DMRegisterArgument3()
{
    outputInstruction &= 61951;
    getNumberValue = stringToNumber();
    if (getNumberValue < 8)
    {
        outputInstruction |= (getNumberValue << 9);
    }
    else
    {
        std::cerr << "Error: invalid register (line " << lineNumber << ")" << std::endl;
    }
}

void setImmediate()
{
    isSymbol = false;
    storedFileOffset2 = inputFileOffset;
    checkAreaRefLow(false);
    if (isAreaReferenced == true)
    {
        fileSizeCount++;
        return;
    }
    inputFileOffset = storedFileOffset2;
    checkAreaRefHigh();
    if (isAreaReferenced == true)
    {
        fileSizeCount++;
        return;
    }
    inputFileOffset = storedFileOffset2;
    if (std::strncmp("L|", reinterpret_cast<const char*>(compareBuffer), 2) == 0)
    {
        inputFileOffset += 2;
        getNumberValue = 0;
        isSymbol = true;
        memset(symbolString,' ', 64);
        stringIndex = 0;
        while (std::isalpha(fileData[inputFileOffset]) && stringIndex < 64)
        {
            symbolString[stringIndex] = fileData[inputFileOffset];
            inputFileOffset++;
            stringIndex++;
        }
        processFileString();
        if (std::strncmp(" + ", reinterpret_cast<const char*>(compareBuffer), 3) == 0)
        {
            inputFileOffset += 3;
            processFileString();
            getNumberValue = stringToNumber();
        }
        symbolDestinations.push_back({ reinterpret_cast<const char*>(symbolString),fileSizeCount,storedFileSizeCount,false,false,false,getNumberValue});
        fileSizeCount ++;
    }
    if (std::strncmp("H|", reinterpret_cast<const char*>(compareBuffer), 2) == 0)
    {
        inputFileOffset += 2;
        getNumberValue = 0;
        isSymbol = true;
        memset(symbolString,' ', 64);
        stringIndex = 0;
        while (std::isalpha(fileData[inputFileOffset]) && stringIndex < 64)
        {
            symbolString[stringIndex] = fileData[inputFileOffset];
            inputFileOffset++;
            stringIndex++;
        }
        processFileString();
        if (std::strncmp(" + ", reinterpret_cast<const char*>(compareBuffer), 3) == 0)
        {
            inputFileOffset += 3;
            processFileString();
            getNumberValue = stringToNumber();
        }
        symbolDestinations.push_back({ reinterpret_cast<const char*>(symbolString),fileSizeCount,storedFileSizeCount,true,false,false,getNumberValue});
        fileSizeCount ++;
    }
    if (isSymbol == false)
    {
        getNumberValue = stringToNumber();
        if (getNumberValue < 65536)
        {
            outputFile[fileSizeCount] = getNumberValue;
            fileSizeCount++;
        }
        else
        {
            std::cerr << "warning: numerical value exceeds 16-bit range (line " << lineNumber << ")" << std::endl;
        }
    }
}

void setSImmediate()
{
    if (std::strncmp("NS ", reinterpret_cast<const char*>(compareBuffer), 3) == 0)
    {
        inputFileOffset += 3;
        outputInstruction |= (1 << 10);
        processFileString();
    }
    isSymbol = false;
    if (compareBuffer[0] == '$')
    {
        inputFileOffset ++;
        getNumberValue = 0;
        isSymbol = true;
        memset(symbolString,' ', 64);
        stringIndex = 0;
        while (std::isalpha(fileData[inputFileOffset]) && stringIndex < 64)
        {
            symbolString[stringIndex] = fileData[inputFileOffset];
            inputFileOffset++;
            stringIndex++;
        }
        if (outputInstruction & 32768)
        {
            symbolDestinations.push_back({ reinterpret_cast<const char*>(symbolString),fileSizeCount,storedFileSizeCount,false,true,false,0});
            fileSizeCount += 2;
        }
        else
        {
            symbolDestinations.push_back({ reinterpret_cast<const char*>(symbolString),fileSizeCount,storedFileSizeCount,false,false,true,0});
            fileSizeCount++;
        }
    }
    else if (compareBuffer[0] == '#')
    {
        inputFileOffset++;
        processFileString();
        getNumberValue = stringToNumber();
        if (outputInstruction & 32768)
        {
            outputFile[fileSizeCount] = getNumberValue;
            outputFile[fileSizeCount + 1] = (getNumberValue >> 16);
            fileSizeCount += 2;
        }
        else
        {
            outputFile[fileSizeCount] = getNumberValue;
            fileSizeCount ++;
        }
    }
    else if (compareBuffer[0] == 'I' && compareBuffer[1] == 'R')
    {
        inputFileOffset += 2;
        processFileString();
        getNumberValue = stringToNumber();
        if (getNumberValue < 8)
        {
            outputInstruction |= (getNumberValue << 5);
            outputInstruction |= (1 << 8);
        }
        else
        {
            std::cerr << "Error: invalid register (line " << lineNumber << ")" << std::endl;
        }
    }
    else
        std::cerr << "no valid prefix found (line " << lineNumber << ")" << std::endl;
}

void setSImmediateNoIndirect()
{
    isSymbol = false;
    if (compareBuffer[0] == '$')
    {
        inputFileOffset++;
        getNumberValue = 0;
        isSymbol = true;
        memset(symbolString,' ', 64);
        stringIndex = 0;
        while (std::isalpha(fileData[inputFileOffset]) && stringIndex < 64)
        {
            symbolString[stringIndex] = fileData[inputFileOffset];
            inputFileOffset++;
            stringIndex++;
        }
        if (outputInstruction & 32768)
        {
            symbolDestinations.push_back({ reinterpret_cast<const char*>(symbolString),fileSizeCount,storedFileSizeCount,false,true,false,0});
            fileSizeCount += 2;
        }
        else
        {
            symbolDestinations.push_back({ reinterpret_cast<const char*>(symbolString),fileSizeCount,storedFileSizeCount,false,false,true,0});
            fileSizeCount++;
        }
    }
    else if (compareBuffer[0] == '#')
    {
        inputFileOffset++;
        processFileString();
        getNumberValue = stringToNumber();
        if (outputInstruction & 32768)
        {
            outputFile[fileSizeCount] = getNumberValue;
            outputFile[fileSizeCount + 1] = (getNumberValue >> 16);
            fileSizeCount += 2;
            //fileSizeCount++;
        }
        else
        {
            outputFile[fileSizeCount] = getNumberValue;
            fileSizeCount++;
        }
    }
    else
        std::cerr << "no valid prefix found (line " << lineNumber << ")" << std::endl;
}

void setSFImmediate()
{
    isSymbol = false;
    if (compareBuffer[0] == '$')
    {
        inputFileOffset++;
        getNumberValue = 0;
        isSymbol = true;
        memset(symbolString,' ', 64);
        stringIndex = 0;
        while (std::isalpha(fileData[inputFileOffset]) && stringIndex < 64)
        {
            symbolString[stringIndex] = fileData[inputFileOffset];
            inputFileOffset++;
            stringIndex++;
        }
        symbolDestinations.push_back({ reinterpret_cast<const char*>(symbolString),fileSizeCount,storedFileSizeCount,false,true,false,0});
        fileSizeCount += 2;
    }
    else if (compareBuffer[0] == '#')
    {
        inputFileOffset++;
        processFileString();
        getNumberValue = stringToNumber();
        outputFile[fileSizeCount] = getNumberValue;
        outputFile[fileSizeCount + 1] = (getNumberValue >> 16);
        fileSizeCount += 2;
    }
    else if (compareBuffer[0] == 'I' && compareBuffer[1] == 'R')
    {
        inputFileOffset += 2;
        processFileString();
        getNumberValue = stringToNumber();
        if (getNumberValue < 8)
        {
            outputInstruction |= (getNumberValue << 8);
            outputInstruction |= (1 << 15);
        }
        else
        {
            std::cerr << "Error: invalid register (line " << lineNumber << ")" << std::endl;
        }
        //fileSizeCount++;
    }
    else
        std::cerr << "no valid prefix found (line " << lineNumber << ")" << std::endl;
}

void setSFImmediateNoIndirect()
{
    isSymbol = false;
    if (compareBuffer[0] == '$')
    {
        inputFileOffset++;
        getNumberValue = 0;
        isSymbol = true;
        memset(symbolString,' ', 64);
        stringIndex = 0;
        while (std::isalpha(fileData[inputFileOffset]) && stringIndex < 64)
        {
            symbolString[stringIndex] = fileData[inputFileOffset];
            inputFileOffset++;
            stringIndex++;
        }
        symbolDestinations.push_back({ reinterpret_cast<const char*>(symbolString),fileSizeCount,storedFileSizeCount,false,true,false,0});
        fileSizeCount += 2;
    }
    else if (compareBuffer[0] == '#')
    {
        inputFileOffset++;
        processFileString();
        getNumberValue = stringToNumber();
        outputFile[fileSizeCount] = getNumberValue;
        outputFile[fileSizeCount + 1] = (getNumberValue >> 16);
        //fileSizeCount++;
        fileSizeCount += 2;
    }
    else
        std::cerr << "no valid prefix found (line " << lineNumber << ")" << std::endl;
}

void getQuickImmediate()
{
    if (quickImmediateEnable == true)
    {
        checkAreaRefLow(true);
        if (isAreaReferenced == true)
            return;
        if (std::strncmp(".H|", reinterpret_cast<const char*>(compareBuffer), 3) == 0 || std::strncmp("H|", reinterpret_cast<const char*>(compareBuffer), 2) == 0)
        {
            std::cerr << "Quick immediate, cannot use H| identifier (line " << lineNumber << ")" << std::endl;
            return;
        }
        getNumberValue = stringToNumber();
        if (getNumberValue < 128)
            outputInstruction |= ((getNumberValue & 63) << 8) | ((getNumberValue & 64) << 9);
        else
        {
            std::cerr << "Error: quick immediate exceeds 7-bit range (line " << lineNumber << ")" << std::endl;
        }
    }
    quickImmediateEnable = true;
}

void getRegisterDupe()
{
    getNumberValue = stringToNumber();
    if (getNumberValue < 8)
    { }
    else
    {
        std::cerr << "Error: invalid register (line " << lineNumber << ")" << std::endl;
    }
    if (getNumberValue != ((outputInstruction >> 5) & 7))
    {
        std::cerr << "Error: invalid register (line " << lineNumber << ")" << std::endl;
    }
}

void oneBitModeSlot1()
{
    outputInstruction &= 65527;
    if (compareBuffer[0] == '%')
    {
        outputInstruction |= (1 << 3);
        inputFileOffset++;
    }
    else if (compareBuffer[0] == 'L')
    {
        std::cerr << "Error: invalid mode assignment (line " << lineNumber << ")" << std::endl;
    }
    else if (compareBuffer[0] == 'I')
    {
        std::cerr << "Error: invalid mode assignment (line " << lineNumber << ")" << std::endl;
    }
    else if (compareBuffer[0] == 'R')
    {
        return;
    }
    else
    {
        std::cerr << "Error: unknown symbol" << lineNumber << ")" << std::endl;
    }
}

void twoBitModeSlot2()
{
    outputInstruction &= 65151;
    if (compareBuffer[0] == '%')
    {
        outputInstruction |= (1 << 7);
        inputFileOffset++;
    }
    else if (compareBuffer[0] == 'L')
    {
        outputInstruction |= (2 << 7);
        inputFileOffset++;
    }
    else if (compareBuffer[0] == 'I')
    {
        outputInstruction |= (3 << 7);
        inputFileOffset++;
    }
    else if (compareBuffer[0] == 'R')
    {
        return;
    }
    else
    {
        std::cerr << "Error: unknown symbol" << lineNumber << ")" << std::endl;
    }
}

void twoBitModeSlot3()
{
    outputInstruction &= 53247;
    if (compareBuffer[0] == '%')
    {
        outputInstruction |= (1 << 12);
        inputFileOffset++;
    }
    else if (compareBuffer[0] == 'L')
    {
        outputInstruction |= (2 << 12);
        inputFileOffset++;
    }
    else if (compareBuffer[0] == 'I')
    {
        outputInstruction |= (3 << 12);
        inputFileOffset++;
    }
    else if (compareBuffer[0] == 'R')
    {
        return;
    }
    else
    {
        std::cerr << "Error: unknown symbol" << lineNumber << ")" << std::endl;
    }
}

void jumpOrCall()
{
    if (std::strncmp("CALL", reinterpret_cast<const char*>(compareBuffer), 4) == 0)
    {
        isJumpInstr = false;
        outputInstruction |= (1 << 9);
        inputFileOffset += 4;
    }
    else if (std::strncmp("JMP", reinterpret_cast<const char*>(compareBuffer), 3) == 0)
    {
        isJumpInstr = true;
        inputFileOffset += 3;
    }
}

void swapNoSwap()
{
    if (std::strncmp("NS", reinterpret_cast<const char*>(compareBuffer), 2) == 0)
    {
        if (isJumpInstr == true)
        {
            std::cerr << "error: improper use of 'NS' (line " << lineNumber << ")" << std::endl;
            return;
        }
        else
        {
            outputInstruction |= (1 << 10);
            inputFileOffset += 2;
        }
    }
}

void QIOrInt()
{
    if (std::strncmp(" #", reinterpret_cast<const char*>(compareBuffer), 2) == 0)
    {
        inputFileOffset += 2;
    }
    else if (std::strncmp("I", reinterpret_cast<const char*>(compareBuffer), 1) == 0)
    {
        inputFileOffset ++;
        quickImmediateEnable = false;
        outputInstruction |= 32768;
    }
    else
    {
        std::cerr << "error: unknown type (line " << lineNumber << ")" << std::endl;
    }
}

void (*codeFunctions[])() =
{
    sizeMode,
    comparisonType,
    registerArgument1,
    registerArgument2,
    setImmediate,
    getQuickImmediate,
    getRegisterDupe,
    DMRegisterArgument1,
    DMRegisterArgument2,
    DMRegisterArgument3,
    oneBitModeSlot1,
    twoBitModeSlot2,
    twoBitModeSlot3,
    arithmeticType,
    sizeModeJ,
    setSImmediate,
    setSFImmediate,
    setSImmediateNoIndirect,
    jumpOrCall,
    swapNoSwap,
    QIOrInt
};

void loadFileIntoArray(const char* filePath, std::vector<unsigned char>& data) {
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
        data.resize(static_cast<size_t>(fileSize + 16));

        // Read the file data into the vector
        file.read(reinterpret_cast<char*>(data.data()), fileSize);

        // Close the file
        file.close();
    }
    else {
        // Handle the case where the file couldn't be opened
        std::cerr << "Error opening file: " << filePath << std::endl;
    }
}

void openFileDialogAndLoadFile() {
    const char* filterPatterns[] = { "*.asm" }; // Adjust file extensions as needed

    // Show the file dialog to select a file
    filePath = tinyfd_openFileDialog(
        "Select File",
        "",
        1, // Number of filter patterns
        filterPatterns,
        "assembly files",
        0
    );

    // Check if the user selected a file
    if (filePath != NULL) {
        // Load the selected file into memory
        loadFileIntoArray(filePath, fileData);
        IsFileLoaded = true;
    }
    //fileData.resize(0);
}

inline void checkInsertFunc()
{
    if (std::strncmp(".INSERT $L|", reinterpret_cast<const char*>(compareBuffer), 11) == 0)
    {
        inputFileOffset += 11;
        getNumberValue = 0;
        isSymbol = true;
        memset(symbolString,' ', 64);
        stringIndex = 0;
        while (std::isalpha(fileData[inputFileOffset]) && stringIndex < 64)
        {
            symbolString[stringIndex] = fileData[inputFileOffset];
            inputFileOffset++;
            stringIndex++;
        }
        symbolIndex = 0;
        while (symbolIndex < symbolSources.size())
        {
            if (std::strncmp(reinterpret_cast<const char*>(symbolString), symbolSources[symbolIndex].name.c_str(), stringIndex) == 0)
            {
                getNumberValue32 = symbolSources[symbolIndex].index;
                outputFile[fileSizeCount] = symbolSources[symbolIndex].index;
                fileSizeCount++;
                //isAreaReferenced = true;
                return;
            }
            symbolIndex++;
        }
    }
    if (std::strncmp(".INSERT $", reinterpret_cast<const char*>(compareBuffer), 9) == 0)
    {
        inputFileOffset += 9;
        getNumberValue = 0;
        isSymbol = true;
        memset(symbolString,' ', 64);
        stringIndex = 0;
        while (std::isalpha(fileData[inputFileOffset]) && stringIndex < 64)
        {
            symbolString[stringIndex] = fileData[inputFileOffset];
            inputFileOffset++;
            stringIndex++;
        }
        symbolIndex = 0;
        while (symbolIndex < symbolSources.size())
        {
            if (std::strncmp(reinterpret_cast<const char*>(symbolString), symbolSources[symbolIndex].name.c_str(), stringIndex) == 0)
            {
                getNumberValue32 = symbolSources[symbolIndex].index;
                processFileString();
                if (std::strncmp(" + ", reinterpret_cast<const char*>(compareBuffer), 3) == 0)
                {
                    inputFileOffset += 3;
                    processFileString();
                    getNumberValue32 += stringToNumber();
                }
                outputFile[fileSizeCount] = getNumberValue32;
                outputFile[fileSizeCount + 1] = (getNumberValue32 >> 16);
                fileSizeCount += 2;
                //isAreaReferenced = true;
                return;
            }
            symbolIndex++;
        }
    }
    if (std::strncmp(".INSERT #", reinterpret_cast<const char*>(compareBuffer), 9) == 0)
    {
        inputFileOffset += 9;
        processFileString();
        getNumberValue = stringToNumber();
        outputFile[fileSizeCount] = getNumberValue;
        fileSizeCount++;
    }
}

void checkInclude()
{
    processFileString();
    if (std::strncmp("INCLUDE ", reinterpret_cast<const char*>(compareBuffer), 8) == 0) {
        //std::cout << "file being included!" << std::endl;
        instructionEnable = false;
        inputFileOffset += 8;
        filePathLength = 0;
        memset(filePathString, 0, 256);
        while (fileData[inputFileOffset] != '*' && filePathLength < 255 && inputFileOffset < fileSize)
        {
            filePathString[filePathLength] = fileData[inputFileOffset];
            inputFileOffset++;
            filePathLength++;
        }
        fileTempSize = fileSize;
        loadFileIntoArray(reinterpret_cast<const char*>(filePathString), fileIncludeData);
        fileIncludeOffset = 0;
        if ((fileSize & 1) == 0)
        {
            while (fileIncludeOffset < (fileSize >> 1))
            {
                outputFile[fileSizeCount] = fileIncludeData[(fileIncludeOffset << 1)] + (fileIncludeData[(fileIncludeOffset << 1) + 1] << 8);
                fileSizeCount++;
                fileIncludeOffset++;
            }
        }
        instructionIndex = 255;
        fileSize = fileTempSize;
    }
}

void compile()
{
    processFileString();
    while (inputFileOffset < (fileSize - 1))
    {
        instructionEnable = true;
        bool isLetter = false;
        hasValueAppended = false;
        while (isLetter == false && inputFileOffset < fileSize)
        {
                if (fileData[inputFileOffset] == 0x2F)
                    commentMode = true;
                if (fileData[inputFileOffset] == 0x0A)
                {
                    commentMode = false;
                    lineNumber++;
                }
                if (commentMode == false)
                {
                    if (fileData[inputFileOffset] == '$')
                    {
                        stringToSymbol();
                    }
                    if (fileData[inputFileOffset] == '.')
                    {
                        processFileString();
                        if (std::strncmp(".NO1MBLIMIT", reinterpret_cast<const char*>(compareBuffer), 11) == 0)
                        {
                            isLimitRemoved = true;
                            inputFileOffset += 11;
                        }
                        if (std::strncmp(".SECTION ", reinterpret_cast<const char*>(compareBuffer), 9) == 0)
                        {
                            stringToSection();
                        }
                        if (std::strncmp(".NAME ", reinterpret_cast<const char*>(compareBuffer), 6) == 0)
                        {
                            stringToName();
                        }
                        checkInsertFunc();
                    }
                    isLetter = std::isalpha(fileData[inputFileOffset]);
            }
            inputFileOffset++;
        }
        inputFileOffset--;
        // Convert to std::string
        processFileString();
        match = false;
        instructionIndex = 0;
        tokenIndex = 0;
        checkInclude();
        //then match the rest of it
        storedFileOffset = inputFileOffset;
        storedFileSizeCount = fileSizeCount;
        fileSizeCount++;
        rowSize = stringTokens[orderTable[0]].size();
        isJumpInstr = false;
        while (tokenIndex < rowSize && instructionIndex < 34)
        {
            match = false;
            codeMatch = false;
            codeIndex = 0;
            while (std::strcmp(codes[codeIndex].c_str(), stringTokens[orderTable[instructionIndex]][tokenIndex].c_str()) != 0 && codeIndex < 21)
            {
                codeIndex++;
            }
            if (codeIndex < 21)
            {
                //std::cout << "Found a code: " << codes[codeIndex].c_str() << std::endl;
                codeFunctions[codeIndex]();
                processFileString();
                match = true;
                codeMatch = true;
            }
            if (std::strncmp(stringTokens[orderTable[instructionIndex]][tokenIndex].c_str(), reinterpret_cast<const char*>(compareBuffer), stringTokens[orderTable[instructionIndex]][tokenIndex].size()) == 0 && codeMatch == false) {
                //std::cout << "Found a match: " << stringTokens[orderTable[instructionIndex]][tokenIndex].c_str() << std::endl;
                match = true;
                inputFileOffset += stringTokens[orderTable[instructionIndex]][tokenIndex].size();
                processFileString();
                if (orderTable[instructionIndex] < 32)
                outputInstruction |= orderTable[instructionIndex];
                else
                {
                    outputInstruction |= (1 << 14);
                    outputInstruction |= ((orderTable[instructionIndex] & 1) << 15);
                }
            }
            tokenIndex++;
            if (match == false)
            {
                fileSizeCount = storedFileSizeCount + 1;
                tokenIndex = 0;
                instructionIndex++;
                rowSize = stringTokens[orderTable[instructionIndex]].size();
                outputInstruction = 0;
                inputFileOffset = storedFileOffset;
                isJumpInstr = false;
                processFileString();
            }
        }
        if (isStringIncomplete == true)
            return;
        if (match == false && codeMatch == false && instructionIndex < 255)
        {
            std::cerr << "error: unknown token (line " << lineNumber << ")" << std::endl;
            std::cout << compareBuffer << std::endl;
            while (1)
            { }
        }
        storedFileSizeCount2 = fileSizeCount;
        fileSizeCount = storedFileSizeCount;
        outputFile[fileSizeCount] = outputInstruction;
        if (instructionEnable == true)
        {
            //std::cout << "outputted instruction: " << outputInstruction << " value appended: " << hasValueAppended << std::endl;
        }
        fileSizeCount = storedFileSizeCount2;

        // Output the result
        //std::cout << "Converted string: " << compareBuffer << std::endl;
    }
}

void symbols()
{
    while (symbolDestinationIndex < symbolDestinations.size())
    {
        bool isSymbolFound = false;
        symbolSourceIndex = 0;
        while (symbolSourceIndex < symbolSources.size() && isSymbolFound == false)
        {
            if (std::strncmp(symbolDestinations[symbolDestinationIndex].name.c_str(), symbolSources[symbolSourceIndex].name.c_str(), 64) != 0)
                symbolSourceIndex++;
            else
                isSymbolFound = true;
        }
        if (symbolSourceIndex < symbolSources.size())
        {
            //std::cout << "symbol found! Write destination: " << symbolDestinations[symbolDestinationIndex].index << " Source value: " << (symbolSources[symbolSourceIndex].index << 1) << std::endl;
            if (symbolDestinations[symbolDestinationIndex].isFullAddress == 0 && symbolDestinations[symbolDestinationIndex].isOffset == 0)
            {
                if (symbolDestinations[symbolDestinationIndex].lowHigh == 0)
                    outputFile[symbolDestinations[symbolDestinationIndex].index] = symbolSources[symbolSourceIndex].index + symbolDestinations[symbolDestinationIndex].additiveOffset;
                else
                    outputFile[symbolDestinations[symbolDestinationIndex].index] = (symbolSources[symbolSourceIndex].index >> 16) + symbolDestinations[symbolDestinationIndex].additiveOffset;
            }
            if (symbolDestinations[symbolDestinationIndex].isFullAddress == 1)
            {
                outputFile[symbolDestinations[symbolDestinationIndex].index] = symbolSources[symbolSourceIndex].index;
                outputFile[symbolDestinations[symbolDestinationIndex].index + 1] = (symbolSources[symbolSourceIndex].index >> 16);
            }
            if (symbolDestinations[symbolDestinationIndex].isOffset == 1)
            {
                outputFile[symbolDestinations[symbolDestinationIndex].index] = symbolSources[symbolSourceIndex].index - (symbolDestinations[symbolDestinationIndex].jumpOffsetIndex);
            }
        }
        else
            std::cerr << "symbol not found." << std::endl;
        symbolDestinationIndex++;
    }
}

void verifyInstructions()
{
    inputFileOffset = 0;
    while (inputFileOffset < fileSizeCount)
    {
        instructionIndex = outputFile[inputFileOffset];
        switch ((instructionIndex & 31))
        {
        case 0:
            std::cout << "verified instruction: Nop" << std::endl;
            inputFileOffset++;
            break;
        case 1:
            std::cout << "verified instruction: Sync" << std::endl;
            inputFileOffset++;
            break;
        case 2:
            std::cout << "verified instruction: Add #" << outputFile[inputFileOffset + 1] << ", R" << ((instructionIndex >> 5) & 7) << ", R" << ((instructionIndex >> 8) & 7) << std::endl;
            inputFileOffset += 2;
            break;
        case 3:
            std::cout << "verified instruction: Add #" << outputFile[inputFileOffset + 1] << ", R" << ((instructionIndex >> 5) & 7) << ", %R" << ((instructionIndex >> 8) & 7) << std::endl;
            inputFileOffset += 2;
            break;
        case 4:
            std::cout << "verified instruction: Add R" << ((instructionIndex >> 5) & 7) << ", R" << ((instructionIndex >> 8) & 7) << ", R" << ((instructionIndex >> 5) & 7) << std::endl;
            inputFileOffset ++;
            break;
        case 5:
            std::cout << "verified instruction: Ld #" << outputFile[inputFileOffset + 1] << ", R" << ((instructionIndex >> 5) & 7) << std::endl;
            inputFileOffset += 2;
            break;
        case 6:
            std::cout << "verified instruction: Str #" << outputFile[inputFileOffset + 1] << ", %R" << ((instructionIndex >> 5) & 7) << std::endl;
            inputFileOffset += 2;
            break;
        case 7:
            std::cout << "verified instruction: Ld QI #" << (instructionIndex >> 8) << ", R" << ((instructionIndex >> 5) & 7) << std::endl;
            inputFileOffset ++;
            break;
        default:
            return;
        }
        std::cout << "address:" << (inputFileOffset << 1) << std::endl;
    }
}

void save()
{
    // Save the vector to an output file
        headerStr = "output.bin";
        if (fileName[0] == 0)
        {
            fileName[0] = 'u';
            fileName[1] = 'n';
            fileName[2] = 'n';
            fileName[3] = 'a';
            fileName[4] = 'm';
            fileName[5] = 'e';
            fileName[6] = 'd';
            fileName[7] = '.';
            fileName[8] = 'b';
            fileName[9] = 'i';
            fileName[10] = 'n';
        }
    std::ofstream outputFileH(fileName, std::ios::binary);
    if (outputFileH.is_open()) {
        // Write vector contents to the output file
        if (fileSizeCount <= MAX_FILE_SIZE)
        {
            outputFileH.write(reinterpret_cast<char*>(outputFile), fileSizeCount << 1);
        }
        else
        {
            if (isLimitRemoved == false)
            {
                std::cout << "Error: ROM size greater than max ROM size (1 megabyte)." << std::endl;
                std::cout << "Result: " << (fileSizeCount >> 20) << " megabytes" << std::endl;
                while (1)
                {
                }
            }
        }

        // Close the output file
        outputFileH.close();
        std::cout << "Data saved to " << fileName << std::endl;
    }
    else {
        std::cout << "Error: Unable to open the output file." << std::endl;
    }
}

int main(int argc, char** argv)
{
    HWND hwnd = GetConsoleWindow();
    // 3. Hide the window.
    ShowWindow(hwnd, SW_SHOW);
    if (argc > 1) {
        // A path was provided on the command line
        filePath = argv[1];
        loadFileIntoArray(filePath, fileData);
    }
    else
    openFileDialogAndLoadFile();
    unsigned short order1 = 0;
    unsigned short order2 = 0;
    unsigned short currentLength = 128;
    while (currentLength > 0)
    {
        for (order1 = 0; order1 < 34; order1++)
        {
            if (stringTokens[order1].size() == currentLength)
            {
                orderTable[order2] = order1;
                order2++;
            }
        }
        currentLength--;
    }
    compile();
    symbols();
    //verifyInstructions();
    save();
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
