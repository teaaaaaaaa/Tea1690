#pragma once

#include "memAccess.h"

#define fromMemory 32
#define sameRegOr32bitPenalty 33
#define toRegMemoryArg1 34
#define toRegMemoryArg2 35
#define fromRegMemoryArg1 34
#define fromRegMemoryArg2 35
#define sameRegPenalty 36
#define sizeableImmediate 37
#define IndirectDirectOrRelative 38
#define IndirectOrDirect 39
#define multiplicationPenalty 40

unsigned char moduleAccessNum;
unsigned short isExecuting;
unsigned char syncReg;
unsigned char compareReg;
unsigned int PC;
unsigned int storedPC;
unsigned int addressReg;
unsigned int holdReg;
unsigned short opReg;
unsigned int stack[8];
unsigned short stackPointer;
unsigned short getOpcode;
unsigned char getMicroCode;
unsigned int temp1;
unsigned short temp2;
unsigned short temp3;
unsigned char registerBank;
unsigned char cycleDelay;
signed short jumpOffset;
unsigned char instructionFlagBuffer[16777216];
unsigned int LDADDSTRAddress;
bool CPUFreeze;

bool isAPU;

unsigned char stallCounter;

unsigned short regBank1[8];
unsigned short regBank2[8];
unsigned short regBank3[8];

unsigned short* storedRegBank = regBank1;
unsigned short* writeRegs = regBank1;
unsigned short mainRegs[16];
unsigned short* regBankPointers[] =
{
	regBank1,
	regBank2,
	regBank3,
	regBank3
};

unsigned short memBuffer[3];

unsigned char instructionCycleCounts[] =
{
	1,0,0,0,	//0
	1,0,0,0,	//1
	1,fromMemory,sameRegOr32bitPenalty,multiplicationPenalty,	//2
	2,fromMemory,toRegMemoryArg2,multiplicationPenalty,		//3
	1,sameRegPenalty,multiplicationPenalty,0,	//4
	1,fromMemory,0,0,	//5
	1,fromMemory,toRegMemoryArg1,0,	//6
	1,0,0,0,	//7
	1,sameRegPenalty,0,0,	//8
	1,toRegMemoryArg2,0,0,	//9
	2,fromRegMemoryArg1,0,0,	//10
	2,fromRegMemoryArg1,toRegMemoryArg2,0,	//11
	2,toRegMemoryArg2,0,0,	//12
	2,fromRegMemoryArg1,0,0,	//13
	2,fromRegMemoryArg1,toRegMemoryArg2,0,	//14
	1,IndirectDirectOrRelative,0,0,	//15
	1,0,0,0,	//16
	2,fromRegMemoryArg1,toRegMemoryArg2,0,	//17
	1,fromMemory,sizeableImmediate,0,	//18
	1,fromMemory,fromMemory,fromMemory,	//19
	1,IndirectDirectOrRelative,0,0,	//20
	1,IndirectOrDirect,0,0,	//21
	1,0,0,0,	//22
	1,sameRegPenalty,0,0,	//23
	1,0,0,0,	//24
	1,0,0,0,	//25
	1,fromRegMemoryArg1,toRegMemoryArg2,0,	//26
	1,fromRegMemoryArg1,toRegMemoryArg2,0,	//27
	2,fromRegMemoryArg1,0,0,	//28
	2,fromRegMemoryArg1,0,0,	//29
	2,fromRegMemoryArg1,toRegMemoryArg2,0,	//30
	2,fromRegMemoryArg1,toRegMemoryArg2,0,	//31
};

unsigned char bitLefts[] =
{
5,8
};

inline unsigned char getCycleDelay(unsigned int address, bool isAPU)
{
	if (((PC >> 24) & 7) == 1)
	{
		if (componentIOMap[address & memSize[(address >> 24) & 7]] != INVA)
			return IOlatencyAmountInCycles[componentIOMap[(address & memSize[(address >> 24) & 7]) + (isAPU << 3)]];
		else
			return 0;
	}
	else
	{
		return latencyAmountInCycles[((address >> 24) & 7) + (isAPU << 3)];
	}
}

inline bool readFromMemoryUnprotected(unsigned int address, unsigned short& returnValue)
{
	if (((address >> 24) & 7) == 1)
	{
		if (componentIOMap[address & memSize[(address >> 24) & 7]] != INVA)
		{
			returnValue = components[(address >> 24) & 7][address & memSize[(address >> 24) & 7]];
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (((address >> 24) & 7) < 7 && (address & memSize[(address >> 24) & 7]) < memSizeNAND[((address >> 24) & 7)])
	{
		returnValue = components[(address >> 24) & 7][address & memSize[(address >> 24) & 7]];
		return true;
	}
	else
	{
		//std::cout << "memory read open bus at " << ((address >> 24) & 7) << " " << (address & memSize[(address >> 24) & 7]) << " is APU: " << isAPU << std::endl;
		return false;
	}
}

inline bool readFromMemory(unsigned int address, unsigned short& returnValue)
{
	if (((address >> 24) & 7) == 1)
	{
		if ((isComponentBusy[componentIOMap[address & memSize[(address >> 24) & 7]] - 1] == 0 || isAlwaysAccessible[address & memSize[(address >> 24) & 7]] == true) && componentIOMap[address & memSize[(address >> 24) & 7]] != INVA)
		{
			returnValue = components[(address >> 24) & 7][address & memSize[(address >> 24) & 7]];
			return true;
		}
		else
		{
			//std::cout << "memory read denied at " << ((address >> 24) & 7) << " " << (address & memSize[(address >> 24) & 7]) << " is APU: " << isAPU << std::endl;
			return false;
		}
	}
	else if (((address >> 24) & 7) < 7 && (address & memSize[(address >> 24) & 7]) < memSizeNAND[((address >> 24) & 7)] && isBusy[(address >> 24) & 7] == 0)
	{
		returnValue = components[(address >> 24) & 7][address & memSize[(address >> 24) & 7]];
		return true;
	}
	else
	{
		//std::cout << "memory read open bus at " << ((address >> 24) & 7) << " " << (address & memSize[(address >> 24) & 7]) << " is APU: " << isAPU << std::endl;
		return false;
	}
}

inline void readFromMemorySigned(unsigned int address, signed short& returnValue)
{
	if (((address >> 24) & 7) == 1)
	{
		if ((isComponentBusy[componentIOMap[address & memSize[(address >> 24) & 7]] - 1] == 0 || isAlwaysAccessible[address & memSize[(address >> 24) & 7]] == true) && componentIOMap[address & memSize[(address >> 24) & 7]] != INVA)
		{
			returnValue = components[(address >> 24) & 7][address & memSize[(address >> 24) & 7]];
		}
		else
		{
			//std::cout << "memory read denied at " << ((address >> 24) & 7) << " " << (address & memSize[(address >> 24) & 7]) << " is APU: " << isAPU << std::endl;
		}
	}
	else if (((address >> 24) & 7) < 7 && (address & memSize[(address >> 24) & 7]) < memSizeNAND[((address >> 24) & 7)] && isBusy[(address >> 24) & 7] == 0)
	{
		returnValue = components[(address >> 24) & 7][address & memSize[(address >> 24) & 7]];
	}
	else
	{
		//std::cout << "memory read open bus at " << ((address >> 24) & 7) << " " << (address & memSize[(address >> 24) & 7]) << " is APU: " << isAPU << std::endl;
	}
}

inline void writeToMemory(unsigned int address, unsigned short writeValue)
{
	if (((address >> 24) & 7) == 1)
	{
		if ((isComponentBusy[componentIOMap[address & memSize[(address >> 24) & 7]] - 1] == 0 || isAlwaysAccessible[address & memSize[(address >> 24) & 7]] == true) && (address & memSize[(address >> 24) & 7]) != 0x38 && componentIOMap[address & memSize[(address >> 24) & 7]] != INVA)
		{
			components[(address >> 24) & 7][address & memSize[(address >> 24) & 7]] = writeValue;
			shadowIO[address & memSize[(address >> 24) & 7]] = 1;
		}
		else
		{
			//std::cout << "memory write denied at " << ((address >> 24) & 7) << " " << (address & memSize[(address >> 24) & 7]) << " is APU: " << isAPU << std::endl;
		}
	}
	else if (((address >> 24) & 7) != 5 && ((address >> 24) & 7) < 7 && (address & memSize[(address >> 24) & 7]) < memSizeNAND[((address >> 24) & 7)] && isBusy[(address >> 24) & 7] == 0)
	{
		components[(address >> 24) & 7][address & memSize[(address >> 24) & 7]] = writeValue;
	}
	else
	{
		//std::cout << "write attempt to open bus at " << ((address >> 24) & 7) << " " << (address & memSize[(address >> 24) & 7]) << " is APU: " << isAPU << std::endl;
	}
}

bool instructionReturn = true;

// Define arithmetic operation codes
#define ADD 0x0
#define SUB 0x1
#define AND 0x2
#define OR  0x3
#define XOR 0x4
#define SHL 0x5
#define SHR 0x6
#define MUL 0x7

void sync()
{
	syncReg = 1;
}

inline unsigned int getRegPairArg1()
{
	return writeRegs[(((getOpcode >> 5)) & 7)] + (writeRegs[(((getOpcode >> 5) + 1) & 7)] << 16);
}

inline unsigned int getRegPairArg2()
{
	return writeRegs[(((getOpcode >> 8)) & 7)] + (writeRegs[(((getOpcode >> 8) + 1) & 7)] << 16);
}

inline unsigned short getRegArg1()
{
	return writeRegs[(((getOpcode >> 5)) & 7)];
}

inline unsigned short getRegArg2()
{
	return writeRegs[(((getOpcode >> 8)) & 7)];
}

inline void regImmediateArithmetic()
{
	switch ((getOpcode >> 11) & 7)
	{
	case ADD:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] + memBuffer[0];
		break;
	case SUB:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] - memBuffer[0];
		break;
	case AND:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] & memBuffer[0];
		break;
	case OR:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] | memBuffer[0];
		break;
	case XOR:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] ^ memBuffer[0];
		break;
	case SHL:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] >> memBuffer[0];
		break;
	case SHR:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] << memBuffer[0];
		break;
	case MUL:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] * memBuffer[0];
		break;
	}
}

inline void regRegArithmetic()
{
	switch ((getOpcode >> 11) & 7)
	{
	case ADD:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] + writeRegs[((getOpcode >> 8) & 7)];
		break;
	case SUB:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] - writeRegs[((getOpcode >> 8) & 7)];
		break;
	case AND:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] & writeRegs[((getOpcode >> 8) & 7)];
		break;
	case OR:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] | writeRegs[((getOpcode >> 8) & 7)];
		break;
	case XOR:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] ^ writeRegs[((getOpcode >> 8) & 7)];
		break;
	case SHL:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] >> writeRegs[((getOpcode >> 8) & 7)];
		break;
	case SHR:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] << writeRegs[((getOpcode >> 8) & 7)];
		break;
	case MUL:
		temp1 = writeRegs[((getOpcode >> 5) & 7)] * writeRegs[((getOpcode >> 8) & 7)];
		break;
	}
}

inline void regRegArithmeticExtended()
{
	switch ((getOpcode >> 11) & 7)
	{
	case ADD:
		temp1 = getRegPairArg1() + getRegPairArg2();
		break;
	case SUB:
		temp1 = getRegPairArg1() - getRegPairArg2();
		break;
	case AND:
		temp1 = getRegPairArg1() & getRegPairArg2();
		break;
	case OR:
		temp1 = getRegPairArg1() | getRegPairArg2();
		break;
	case XOR:
		temp1 = getRegPairArg1() ^ getRegPairArg2();
		break;
	case SHL:
		temp1 = getRegPairArg1() >> getRegPairArg2();
		break;
	case SHR:
		temp1 = getRegPairArg1() << getRegPairArg2();
		break;
	case MUL:
		temp1 = getRegPairArg1() * getRegPairArg2();
		break;
	}
}

inline void compare(unsigned short operator1, unsigned short operator2)
{
	if (operator1 < operator2)
		compareReg = 0;
	if (operator1 == operator2)
		compareReg = 1;
	if (operator1 > operator2)
		compareReg = 2;
}

void op0()
{

}

void op2()
{
	regImmediateArithmetic();
	writeRegs[((getOpcode >> 8) & 7)] = temp1;
	if (getOpcode & 32768)
	{
		writeRegs[(((getOpcode >> 8) + 1) & 7)] = (temp1 >> 16);
	}
}

void op3()
{
	regImmediateArithmetic();
	writeToMemory(getRegPairArg2(), temp1);
}

void op4()
{
	regRegArithmetic();
	writeRegs[((getOpcode >> 5) & 7)] = temp1;
	if (getOpcode & 32768)
	{
		writeRegs[(((getOpcode >> 5) + 1) & 7)] = (temp1 >> 16);
	}
}

void op5()
{
	writeRegs[((getOpcode >> 5) & 7)] = memBuffer[0];
}

void op6()
{
	writeToMemory(getRegPairArg1(), memBuffer[0]);
}

void op7()
{
	writeRegs[(((getOpcode >> 5)) & 7)] = ((getOpcode >> 8) & 63) + ((getOpcode >> 15) << 6);
}

void op8()
{
	writeRegs[((getOpcode >> 5) & 7)] = writeRegs[((getOpcode >> 8) & 7)];
}

void op9()
{
	writeToMemory(getRegPairArg2(), getRegArg1());
}

void op10()
{
	readFromMemory(getRegPairArg1(), writeRegs[((getOpcode >> 8) & 7)]);
}

void op11()
{
	readFromMemory(getRegPairArg1(), temp2);
	writeToMemory(getRegPairArg2(), temp2);
}

void op12()
{
	writeToMemory(getRegPairArg2(), getRegArg1());
	temp1 = getRegPairArg2();
	temp1++;
	writeRegs[((getOpcode >> 8) & 7)] = temp1;
	writeRegs[(((getOpcode >> 8) + 1) & 7)] = (temp1 >> 16);
}

void op13()
{
	readFromMemory(getRegPairArg1(), writeRegs[((getOpcode >> 8) & 7)]);
	temp1 = getRegPairArg1();
	temp1++;
	writeRegs[((getOpcode >> 5) & 7)] = temp1;
	writeRegs[(((getOpcode >> 5) + 1) & 7)] = (temp1 >> 16);
}

void op14()
{
	readFromMemory(getRegPairArg2(), temp2);
	writeToMemory(getRegPairArg1(), temp2);
	temp1 = getRegPairArg1() + 1;
	writeRegs[((getOpcode >> 5) & 7)] = temp1;
	writeRegs[(((getOpcode >> 5) + 1) & 7)] = temp1 >> 16;
	temp1 = getRegPairArg2() + 1;
	writeRegs[((getOpcode >> 8) & 7)] = temp1;
	writeRegs[(((getOpcode >> 8) + 1) & 7)] = temp1 >> 16;
}

void op15()
{
	if (getOpcode & (1 << 9))
	{
		stack[stackPointer & 7] = PC;
		stackPointer++;
	}
	if (getOpcode & (1 << 8))
	{
		PC = (memBuffer[0] + (memBuffer[1] << 16));
		return;
	}
	jumpOffset = memBuffer[0];
	if (getOpcode & 32768)
	{
		PC = (memBuffer[0] + (memBuffer[1] << 16));
	}
	else
	{
		PC = storedPC + jumpOffset;
	}
	if ((getOpcode & (1 << 10)) == 0 && (getOpcode & (1 << 9)))
	{
		if (writeRegs == regBank1)
		{
			writeRegs = regBank2;
		}
		else
		{
			writeRegs = regBank1;
		}
	}
}

void op16()
{
	compare(getRegArg1(), getRegArg2());
}

void op17()
{
	readFromMemory(getRegPairArg1(),temp2);
	readFromMemory(getRegPairArg2(), temp3);
	compare(temp2, temp3);
}

void op18()
{
	compare(memBuffer[0], writeRegs[((getOpcode >> 5) & 7)]);
	if (compareReg == ((getOpcode >> 11) & 3) && ((getOpcode >> 13) & 1))
		return;
	if (compareReg != ((getOpcode >> 11) & 3) && ((getOpcode >> 13) & 1) == 0)
		return;
	jumpOffset = memBuffer[1];
	if (getOpcode & 32768)
	{
		PC = (memBuffer[1] + (memBuffer[2] << 16));
	}
	else
	{
		PC = storedPC + jumpOffset;
	}
}

void op19()
{
	compare(memBuffer[0], writeRegs[((getOpcode >> 5) & 7)]);
	if (compareReg == ((getOpcode >> 11) & 3) && ((getOpcode >> 13) & 1))
		return;
	if (compareReg != ((getOpcode >> 11) & 3) && ((getOpcode >> 13) & 1) == 0)
		return;
	jumpOffset = memBuffer[1];
	stack[stackPointer & 7] = PC;
	stackPointer++;
	PC = (memBuffer[0] + (memBuffer[1] << 16));
	if ((getOpcode & (1 << 10)) == 0 && (getOpcode & (1 << 9)))
	{
		if (writeRegs == regBank1)
		{
			writeRegs = regBank2;
		}
		else
		{
			writeRegs = regBank1;
		}
	}
}

void op20()
{
	if (compareReg == ((getOpcode >> 11) & 3) && ((getOpcode >> 13) & 1))
		return;
	if (compareReg != ((getOpcode >> 11) & 3) && ((getOpcode >> 13) & 1) == 0)
		return;
	jumpOffset = memBuffer[0];
	//std::cout << "jump offset: " << jumpOffset << std::endl;
	if (getOpcode & (1 << 8))
	{
		PC = (memBuffer[0] + (memBuffer[1] << 16));
		return;
	}
	if (getOpcode & 32768)
	{
		PC = (memBuffer[0] + (memBuffer[1] << 16));
	}
	else
	{
		PC = storedPC + jumpOffset;
	}
}

void op21()
{
	if (getOpcode & (1 << 9))
	{
		if (compareReg == ((getOpcode >> 11) & 3) && ((getOpcode >> 13) & 1))
			return;
		if (compareReg != ((getOpcode >> 11) & 3) && ((getOpcode >> 13) & 1) == 0)
			return;
	}
	stack[stackPointer & 7] = PC;
	stackPointer++;
	PC = (memBuffer[0] + (memBuffer[1] << 16));
	if ((getOpcode & (1 << 10)) == 0 && (getOpcode & (1 << 9)))
	{
		if (writeRegs == regBank1)
		{
			writeRegs = regBank2;
		}
		else
		{
			writeRegs = regBank1;
		}
	}
}

void op22()
{
	stackPointer--;
	PC = stack[stackPointer & 7];
	if (getOpcode & 32768)
	{
		writeRegs = storedRegBank;
	}
	else
	{
		if ((getOpcode >> 8) < 3)
			writeRegs = regBankPointers[(getOpcode >> 8)];
		else
			CPUFreeze = true;
	}
}

void op23()
{
	writeRegs[((getOpcode >> 8) & 7)] = ~writeRegs[((getOpcode >> 5) & 7)];
}

void op24()
{
	writeRegs[((getOpcode >> 5) & 7)]++;
}

void op25()
{
	writeRegs[((getOpcode >> 5) & 7)]--;
}

void op26()
{
	readFromMemory(getRegPairArg1(), memBuffer[0]);
	memBuffer[0]++;
	writeToMemory(getRegPairArg1(), memBuffer[0]);
}

void op27()
{
	readFromMemory(getRegPairArg1(), memBuffer[0]);
	memBuffer[0]--;
	writeToMemory(getRegPairArg1(), memBuffer[0]);
}

void op28()
{
	readFromMemory(getRegPairArg1(), memBuffer[0]);
	writeRegs[((getOpcode >> 8) & 7)] = memBuffer[0] + 1;
}

void op29()
{
	readFromMemory(getRegPairArg1(), memBuffer[0]);
	writeRegs[((getOpcode >> 8) & 7)] = memBuffer[0] - 1;
}

void op30()
{
	readFromMemory(getRegPairArg1(), memBuffer[0]);
	memBuffer[0]++;
	writeToMemory(getRegPairArg2(), memBuffer[0]);
}

void op31()
{
	readFromMemory(getRegPairArg1(), memBuffer[0]);
	memBuffer[0]--;
	writeToMemory(getRegPairArg2(), memBuffer[0]);
}

void handleDMode()
{
	if (getOpcode & (1 << 15))
	{
		readFromMemory(LDADDSTRAddress, writeRegs[getOpcode & 7]);
	}
	else
	{
		writeToMemory(LDADDSTRAddress, writeRegs[getOpcode & 7]);
	}
}

void (*opcodefunctions[])() =
{
op0,
sync,
op2,
op3,
op4,
op5,
op6,
op7,
op8,
op9,
op10,
op11,
op12,
op13,
op14,
op15,
op16,
op17,
op18,
op19,
op20,
op21,
op22,
op23,
op24,
op25,
op26,
op27,
op28,
op29,
op30,
op31,
};

//from memory
#define opMemToReg1 0
#define opMemToHold 1
#define opMemToOpr 2
#define opMemToReg2 (0 | 64)
//to memory
#define opRegToMem1 3
#define opPCToMemL 4
#define opPCToMemH 5
#define opHoldToMem 6
#define opRegToMem2 (3 | 64)
//from registers
#define opRegToHold1 7
#define opRegToAddr1 8
#define opRegToPC1 9
#define opRegToOpr1 10
#define opRegToHold2 (7 | 64)
#define opRegToAddr2 (8 | 64)
#define opRegToPC2 (9 | 64)
#define opRegToOpr2 (10 | 64)
//to registers
#define opRegToReg 11
#define opHoldToReg1 12
#define opAddrToReg1 13
#define opPCToReg1 14
#define opHoldToReg2 (12 | 64)
#define opAddrToReg2 (13 | 64)
#define opPCToReg2 (14 | 64)
//increment/decrement
#define opIncHold 15
#define opdecHold 16
#define opIncAddr 17
#define opIncPC 18
#define opIncReg1 19
#define opDecReg1 20
#define opIncReg2 (19 | 64)
#define opDecReg2 (20 | 64)
//misc
#define opAddrToPC 21
#define opPCToAddr 22
#define opSync 23
#define opIncPCAddr 24
#define opPCStore 25
#define opPCPop 26
#define opQuickLoad 27
//operand arithmetic
#define opArithmetic 28
#define opArithmeticRegs (28 | 64)
#define opNOT 29
//compare/branch
#define opCompare 30
#define opcarryIf 31
#define opNoOperation 32

#define endInstr 128

//NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,

//this part is unused

unsigned char opcodes[] =
{
	opNoOperation | endInstr,NULL,NULL,NULL,NULL,NULL,NULL,NULL,	//NOP (0: NOP)
	opSync | endInstr,NULL,NULL,NULL,NULL,NULL,NULL,NULL,	//Sync (1: SYNC)

	opPCToAddr,opIncPC,opMemToReg2,opArithmeticRegs,opHoldToReg2 | endInstr,NULL,NULL,NULL,			//immediate ___ register --> register (2: ADD #0, R0, R0)
	opPCToAddr,opIncPC,opMemToOpr,opRegToHold1,opArithmetic,opRegToAddr2,opHoldToMem | endInstr,NULL,	//immediate ___ register --> register based memory (3: ADD #0, R0, %R0)
	opArithmeticRegs,opHoldToReg1 | endInstr,NULL,NULL,NULL,NULL,NULL,NULL,								//register ___ register --> register (4: ADD R0, R0, R0)
	opPCToAddr,opIncPC,opMemToReg1 | endInstr,NULL,NULL,NULL,NULL,NULL,									//immediate --> register (5: LD #0, R0)
	opPCToAddr,opIncPC,opMemToHold,opRegToAddr1,opHoldToMem | endInstr,NULL,NULL,NULL,						//immediate --> register based memory (6: STR #0, %R0)
	opQuickLoad | endInstr,NULL,NULL,NULL,NULL,NULL,NULL,NULL,											//quick immediate --> register (7: LD QI #0, R0)
	opRegToReg | endInstr,NULL,NULL,NULL,NULL,NULL,NULL,NULL,											//register --> register (8: LD R0, R0)
	opRegToAddr2,opRegToMem1 | endInstr,NULL,NULL,NULL,NULL,NULL,NULL,									//register --> register based memory (9: STR R0, %R0)
	opRegToAddr1,opMemToReg2 | endInstr,NULL,NULL,NULL,NULL,NULL,NULL,									//register based memory --> register (10: LD %R0, R0)
	opRegToAddr2,opMemToHold,opRegToAddr1,opHoldToMem | endInstr,NULL,NULL,NULL,NULL,					//register based memory --> register based memory (11: STR %R0, %R0)
	opRegToAddr2,opRegToMem1,opIncReg2 | endInstr,NULL,NULL,NULL,NULL,NULL,								//register --> register based memory + increment (12: STRI R0, %R0)
	opRegToAddr1,opMemToReg2,opIncReg1 | endInstr,NULL,NULL,NULL,NULL,NULL,								//register based memory --> register + increment (13: LDI %R0, R0)
	opRegToAddr2,opMemToHold,opRegToAddr1,opIncReg1,opIncReg2,opHoldToMem | endInstr,NULL,NULL,			//register based memory --> register based memory + increment (14: STRI %R0, %R0)
	opRegToPC1 | endInstr,NULL,NULL,NULL,NULL,NULL,NULL,NULL,											//jump/jump relative (15: JMP #0)
	opRegToHold1,opRegToOpr2,opCompare | endInstr,NULL,NULL,NULL,NULL,NULL,								//compare registers (16: CMP R0, R0)
	opRegToAddr1,opRegToHold1,opRegToAddr2,opRegToOpr2,opCompare | endInstr,NULL,NULL,NULL,				//compare memory (17: CMP %R0, %R0)
	opPCToAddr,opMemToHold,opRegToOpr1,opCompare,opcarryIf,opRegToPC2 | endInstr,NULL,NULL,				//jump/jump relative if immediate ___ register (18: JMP LTR #0, R0, #0)
	opPCToAddr,opMemToHold,opRegToOpr1,opCompare,opcarryIf,opPCStore,opRegToPC2 | endInstr,NULL,			//call function if immediate ___ register (19: CALL LTR #0, R0, #0)
	opcarryIf,opRegToPC1 | endInstr,NULL,NULL,NULL,NULL,NULL,NULL,										//jump/jump relative if comparison (20: JMP LTR, #0)
	opcarryIf,opPCStore,opRegToPC1 | endInstr,NULL,NULL,NULL,NULL,NULL,									//call function if comparison (21: CALL LTR, #0)
	opPCPop | endInstr,NULL,NULL,NULL,NULL,NULL,NULL,NULL,												//return from function (22: RET)
	opNOT | endInstr,NULL,NULL,NULL,NULL,NULL,NULL,NULL,													//NOT operation (23: NOT R0, R0)
	opIncReg1 | endInstr,NULL,NULL,NULL,NULL,NULL,NULL,NULL,												//increment register (24: INC R0)
	opDecReg1 | endInstr,NULL,NULL,NULL,NULL,NULL,NULL,NULL,												//decrement register (25: DEC R0)
	opRegToAddr1,opMemToHold,opIncHold,opHoldToMem | endInstr,NULL,NULL,NULL,NULL,						//increment register based memory (26: INC %R0)
	opRegToAddr1,opMemToHold,opdecHold,opHoldToMem | endInstr,NULL,NULL,NULL,NULL,						//decrement register based memory (27: DEC %R0)
	opRegToAddr1,opMemToHold,opIncHold,opHoldToReg2 | endInstr,NULL,NULL,NULL,NULL,						//increment memory --> register (28: LD INC %R0, R0)
	opRegToAddr1,opMemToHold,opdecHold,opHoldToReg2 | endInstr,NULL,NULL,NULL,NULL,						//decrement memory --> register (29: LD DEC %R0, R0)
	opRegToAddr1,opMemToHold,opIncHold,opRegToAddr2,opHoldToMem | endInstr,NULL,NULL,NULL,				//increment memory --> register based memory (30: STR INC %R0, %R0)
	opRegToAddr1,opMemToHold,opdecHold,opRegToAddr2,opHoldToMem | endInstr,NULL,NULL,NULL,				//decrement memory --> register based memory (31: STR DEC %R0, %R0)
};