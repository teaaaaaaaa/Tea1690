#pragma once

unsigned char moduleAccessNum;
unsigned short isExecuting;
unsigned char syncReg;
unsigned char compareReg;
unsigned int PC;
unsigned int addressReg;
unsigned int holdReg;
unsigned short opReg;
unsigned int storedPC;
unsigned short getOpcode;
unsigned char getMicroCode;
unsigned short temp1;
unsigned short temp2;
unsigned char registerBank;

unsigned short mainRegs[16];

unsigned char bitLefts[] =
{
6,9
};

bool instructionReturn;

void OpcodeEnd()
{
	instructionReturn = true;
}

void MemToReg()
{
	if ((((addressReg & 0xFFFFFF) >> 24) & 7) < 5 && (((addressReg & 0xFFFFFF) >> 24) & 7) != 3)
	{
		if (addressReg < memSize[((addressReg >> 24) & 7)])
		{
			mainRegs[(((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) & 7) + (registerBank << 3))] = components[(addressReg >> 24) & 7][addressReg & ((memSize[((addressReg >> 24) & 7)]) - 1)];
			memAccessFlags[(addressReg >> 24) & 7] = true;
		}
	}
}

void RegToMem()
{
	if ((((addressReg & 0xFFFFFF) >> 24) & 7) < 4)
	{
		if (addressReg < memSize[((addressReg >> 24) & 7)])
		{
			components[(addressReg >> 24) & 7][addressReg & ((memSize[((addressReg >> 24) & 7)]) - 1)] = mainRegs[((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) & 7) + (registerBank << 3)];
			memAccessFlags[(addressReg >> 24) & 7] = true;
		}
	}
}

void RegToReg()
{
	mainRegs[(getOpcode >> 6) & 7] = mainRegs[(getOpcode >> 9) & 7];
}

void RegToHold()
{
	if (((getOpcode >> 12) & 1) == 0)
	{
		holdReg = mainRegs[((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) & 7) + (registerBank << 3)];
	}
	else
	{
		holdReg = mainRegs[((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) & 7) + (registerBank << 3)] + ((mainRegs[(((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) + 1) & 7) + (registerBank << 3)]) << 16);
	}
}

void MemToHold()
{
	if ((((addressReg & 0xFFFFFF) >> 24) & 7) < 5 && (((addressReg & 0xFFFFFF) >> 24) & 7) != 3)
	{
		if (addressReg < memSize[((addressReg >> 24) & 7)])
		{
			holdReg = components[(addressReg >> 24) & 7][addressReg & ((memSize[((addressReg >> 24) & 7)]) - 1)];
		}
	}
}

void HoldToReg()
{
	if (((getOpcode >> 12) & 1) == 0)
	{
		mainRegs[((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) & 7) + (registerBank << 3)] = holdReg;
	}
	else
	{
		mainRegs[((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) & 7) + (registerBank << 3)] = holdReg;
		mainRegs[(((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) + 1) & 7) + (registerBank << 3)] = (holdReg >> 16);
	}
}

void HoldToMem()
{
	if ((((addressReg & 0xFFFFFF) >> 24) & 7) < 4)
	{
			components[(addressReg >> 24) & 7][addressReg & ((memSize[((addressReg >> 24) & 7)]) - 1)] = holdReg;
			memAccessFlags[(addressReg >> 24) & 7] = true;
	}
}

void AddrToReg()
{
	mainRegs[((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) & 7) + (registerBank << 3)] = addressReg;
	mainRegs[((((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) + 1) & 7) + (registerBank << 3))] = (addressReg << 16);
}

void RegToAddr()
{
	addressReg = mainRegs[((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) & 7) + (registerBank << 3)] + (mainRegs[(((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) + 1) & 7) + (registerBank << 3)] << 16);
}

void PCToReg()
{
	mainRegs[((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) & 7) + (registerBank << 3)] = PC;
	mainRegs[(((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) + 1) & 7) + (registerBank << 3)] = (PC << 16);
}

void RegToPC()
{
	PC = mainRegs[((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) & 7) + (registerBank << 3)] + (mainRegs[(((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) + 1) & 7) + (registerBank << 3)] << 16);
}

void PCToAddr()
{
	addressReg = PC;
}

void PCToMemLow()
{
	if ((((addressReg & 0xFFFFFF) >> 24) & 7) < 4)
	{
			components[(addressReg >> 24) & 7][addressReg & ((memSize[((addressReg >> 24) & 7)]) - 1)] = PC;
			memAccessFlags[(addressReg >> 24) & 7] = true;
	}
}

void PCToMemHigh()
{
	if ((((addressReg & 0xFFFFFF) >> 24) & 7) < 4)
	{
			components[(addressReg >> 24) & 7][addressReg & ((memSize[((addressReg >> 24) & 7)]) - 1)] = (PC >> 16);
			memAccessFlags[(addressReg >> 24) & 7] = true;
	}
}

void AddrToPC()
{
	PC = addressReg;
}

void IncPC ()
{
	PC ++;
}

void IncAddr()
{
	addressReg ++;
}

void IncPCAddr()
{
	PC++;
	addressReg++;
}

void PCStore()
{
	storedPC = PC;
}

void PCPop()
{
	PC = storedPC;
}

void quickLoad()
{
	mainRegs[((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) & 7) + (registerBank << 3)] = (getOpcode >> 6);
}

void IncReg()
{
	mainRegs[(getOpcode >> (getOpcode >> bitLefts[(getMicroCode & 64) >> 5])) & 7] ++;
}

void DecReg()
{
	mainRegs[(getOpcode >> (getOpcode >> bitLefts[(getMicroCode & 64) >> 5])) & 7] --;
}

void RegToOp()
{
	opReg = mainRegs[((getOpcode >> bitLefts[(getMicroCode & 64) >> 5]) & 7) + (registerBank << 3)];
}

void MemToOp()
{
	if ((((addressReg & 0xFFFFFF) >> 24) & 7) < 5 && (((addressReg & 0xFFFFFF) >> 24) & 7) != 3)
	{
			opReg = components[(addressReg >> 24) & 7][addressReg & ((memSize[((addressReg >> 24) & 7)]) - 1)];
	}
}

// Define arithmetic operation codes
#define ADD 0x0
#define SUB 0x1
#define AND 0x2
#define OR  0x3
#define XOR 0x4
#define NOT 0x5
#define SHL 0x6
#define SHR 0x7
#define ROTL 0x8
#define ROTR 0x9

void Arithmetic()
{
	if ((getMicroCode & 128) == 0)
	{
		switch ((getOpcode >> 12) & 7) {
		case ADD:
			holdReg += opReg;
			break;
		case SUB:
			holdReg -= opReg;
			break;
		case AND:
			holdReg &= opReg;
			break;
		case OR:
			holdReg |= opReg;
			break;
		case XOR:
			holdReg ^= opReg;
			break;
		case NOT:
			holdReg = ~holdReg;  // NOT typically only applies to one operand
			break;
		case SHL:
			holdReg >>= opReg;
			break;
		case SHR:
			holdReg <<= opReg;
			break;
		}
	}
	else
	{
		switch ((getOpcode >> 12) & 7) {
		case ADD:
			holdReg = mainRegs[(getOpcode >> 6) & 7] + mainRegs[(getOpcode >> 9) & 7];
			break;
		case SUB:
			holdReg = mainRegs[(getOpcode >> 6) & 7] - mainRegs[(getOpcode >> 9) & 7];
			break;
		case AND:
			holdReg = mainRegs[(getOpcode >> 6) & 7] & mainRegs[(getOpcode >> 9) & 7];
			break;
		case OR:
			holdReg = mainRegs[(getOpcode >> 6) & 7] | mainRegs[(getOpcode >> 9) & 7];
			break;
		case XOR:
			holdReg = mainRegs[(getOpcode >> 6) & 7] ^ mainRegs[(getOpcode >> 9) & 7];
			break;
		case NOT:
			holdReg = ~mainRegs[(getOpcode >> 6) & 7];
			break;
		case SHL:
			holdReg = mainRegs[(getOpcode >> 6) & 7] >> mainRegs[(getOpcode >> 9) & 7];
			break;
		case SHR:
			holdReg = mainRegs[(getOpcode >> 6) & 7] << mainRegs[(getOpcode >> 9) & 7];
			break;
		}
	}
}

void compare()
{
	if (opReg == holdReg)
		compareReg = 1;
	if (opReg < holdReg)
		compareReg = 0;
	if (opReg > holdReg)
		compareReg = 2;
}

void carryIf()
{
	if ((getOpcode >> 15) == 0)
	{
		if (compareReg != ((getOpcode >> 12) & 3))
			instructionReturn = true;
	}
	else
	{
		if (compareReg == ((getOpcode >> 12) & 3))
			instructionReturn = true;
	}
}

void sync()
{
	syncReg = 1;
}

void (*opcodefunctions[])() =
{
MemToReg,
MemToHold,
MemToOp,
RegToMem,
PCToMemLow,
PCToMemHigh,
HoldToMem,
RegToHold,
RegToAddr,
RegToPC,
RegToOp,
RegToReg,
HoldToReg,
AddrToReg,
PCToReg,
IncAddr,
IncPC,
IncReg,
DecReg,
OpcodeEnd,
AddrToPC,
PCToAddr,
sync,
IncPCAddr,
PCStore,
PCPop,
quickLoad,
Arithmetic,
compare,
carryIf,
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
#define opIncAddr 15
#define opIncPC 16
#define opIncReg1 17
#define opDecReg1 18
#define opIncReg2 (17 | 64)
#define opDecReg2 (18 | 64)
//misc
#define opEndInstr 19
#define opAddrToPC 20
#define opPCToAddr 21
#define opSync 22
#define opIncPCAddr 23
#define opPCStore 24
#define opPCPop 25
#define opQuickLoad 26
//operand arithmetic
#define opArithmetic 27
#define opArithmeticRegs (28 | 128)
//compare/branch
#define opCompare 29
#define opcarryIf 30

//NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,

unsigned char opcodes[] =
{
	opEndInstr,NULL,NULL,NULL,NULL,NULL,NULL,NULL,	//nop (0: NOP)
	opSync,opEndInstr,NULL,NULL,NULL,NULL,NULL,NULL,	//Sync (1: SYNC)

	opPCToAddr,opMemToOpr,opRegToHold1,opArithmetic,opHoldToReg2,opIncPC,opEndInstr,NULL,			//immediate ___ register --> register (2: ADD #0, R0, R0)
	opPCToAddr,opMemToOpr,opRegToHold1,opArithmetic,opRegToAddr2,opHoldToMem,opIncPC,opEndInstr,	//immediate ___ register --> register based memory (3: ADD #0, R0, %R0)
	opArithmeticRegs,opHoldToReg1,opEndInstr,NULL,NULL,NULL,NULL,NULL,								//register ___ register --> register (4: ADD R0, R0, R0)
	opPCToAddr,opMemToReg1,opIncPC,opEndInstr,NULL,NULL,NULL,NULL,									//immediate --> register (5: LD #0, R0)
	opPCToAddr,opMemToHold,opRegToAddr1,opHoldToMem,opEndInstr,NULL,NULL,NULL,						//immediate --> register based memory (6: STR #0, %R0)
	opQuickLoad,opEndInstr,NULL,NULL,NULL,NULL,NULL,NULL,											//quick immediate --> register (7: LD QI #0, R0)
	opRegToReg,opEndInstr,NULL,NULL,NULL,NULL,NULL,NULL,											//register --> register (8: LD R0, R0)
	opRegToAddr2,opRegToMem1,opEndInstr,NULL,NULL,NULL,NULL,NULL,									//register --> register based memory (9: STR R0, %R0)
	opRegToAddr1,opMemToReg2,opEndInstr,NULL,NULL,NULL,NULL,NULL,									//register based memory --> register (10: LD %R0, R0)
	opRegToAddr2,opMemToHold,opRegToAddr1,opHoldToMem,opEndInstr,NULL,NULL,NULL,					//register based memory --> register based memory (11: STR %R0, %R0)
	opRegToAddr2,opRegToMem1,opIncReg2,opEndInstr,NULL,NULL,NULL,NULL,								//register --> register based memory + increment (12: STRI R0, %R0)
	opRegToAddr1,opMemToReg2,opIncReg1,opEndInstr,NULL,NULL,NULL,NULL,								//register based memory --> register + increment (13: LDI %R0, R0)
	opRegToAddr2,opMemToHold,opRegToAddr1,opIncReg1,opIncReg2,opHoldToMem,opEndInstr,NULL,			//register based memory --> register based memory + increment (14: STRI %R0, %R0)
	opRegToPC1,opEndInstr,NULL,NULL,NULL,NULL,NULL,NULL,											//jump (15: JMP R0)
	opRegToHold1,opRegToOpr2,opCompare,opEndInstr,NULL,NULL,NULL,NULL,								//compare registers (16: CMP R0, R0)
	opRegToAddr1,opRegToHold1,opRegToAddr2,opRegToOpr2,opCompare,opEndInstr,NULL,NULL,				//compare memory (17: CMP %R0, %R0)
	opPCToAddr,opMemToHold,opRegToOpr1,opCompare,opcarryIf,opRegToPC2,opEndInstr,NULL,				//jump if immediate ___ register (18: JMP LTR #0, R0)
	opPCToAddr,opMemToHold,opRegToOpr1,opCompare,opcarryIf,opPCStore,opRegToPC2,opEndInstr,			//call function if immediate ___ register (19: CALL LTR #0, R0)
	opcarryIf,opRegToPC1,opEndInstr,NULL,NULL,NULL,NULL,NULL,										//jump if comparison (20: JMP LTR, R0)
	opcarryIf,opPCStore,opRegToPC1,opEndInstr,NULL,NULL,NULL,NULL,									//call function if comparison (21: CALL LTR, R0)
	opPCPop,opEndInstr,NULL,NULL,NULL,NULL,NULL,NULL,												//return from function (22: RET)
	opRegToPC1,opEndInstr,NULL,NULL,NULL,NULL,NULL,NULL,											//return from function using register address (23: RET %R0)
	opIncReg1,opEndInstr,NULL,NULL,NULL,NULL,NULL,NULL,												//increment register (24: INC R0)
	opDecReg1,opEndInstr,NULL,NULL,NULL,NULL,NULL,NULL,												//decrement register (25: DEC R0)
};