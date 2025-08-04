.NAME LargeMap.bin

.SECTION Input 0x0000
.SECTION XScroll 0x0001
.SECTION YScroll 0x0002
.SECTION OAMDefault 0x0003

//for tile copy function
.SECTION metatileX 0x0004
.SECTION metatileY 0x0005
.SECTION chunkAttribute 0x0006
.SECTION metatileCount 0x0050
.SECTION controllerInput 0x0051
.SECTION scrollX 0x0052
.SECTION velocityH 0x0054
.SECTION scrollY 0x0055
.SECTION velocityV 0x0057
.SECTION zoom 0x0058
.SECTION columnLeftX 0x0059
.SECTION columnRightX 0x005A
.SECTION rowUpY 0x005B
.SECTION rowDownY 0x005C
.SECTION scrollCounterH 0x005D
.SECTION scrollCounterV 0x005E
.SECTION BGScrollX 0x005F
.SECTION BGScrollCounter 0x0060

.SECTION mapPointer 0x0061

.SECTION currentMap 0x0100

.SECTION TCCode 0x0200

//initialize palette read write

LD #256, R2 				//store high 16 bits of address for palette write

LD QI #0, R1				//clear RAM with DMA transfer
LD #0xFFFF, R0
STRI R0, %R1
LD #L|RAMClearValue, R0
STRI R0, %R1
LD #H|RAMClearValue, R0
STRI R0, %R1
LD QI #0, R0
STRI R0, %R1
LD QI #0, R0
STRI R0, %R1
LD #512, R0
STRI R0, %R1

LD QI #1, R1				//set R1 back to 1

LD #L|paletteDat, R0				//store loop end conditional value
STRI R0, %R1
LD #H|paletteDat + 2048, R0				//store loop end conditional value
STRI R0, %R1
LD QI #0, R0
STRI R0, %R1
LD #512, R0
STRI R0, %R1
LD QI #17, R0
STRI R0, %R1

LD QI #3, R1				//store low 16 bits of address for palette write
LD #32768, R0
STRI R0, %R1
LD #768, R0
STRI R0, %R1
LD #13392, R0
STRI R0, %R1

LD #L|BGDataPointers, R2        //load base address of data pointer list
LD #H|BGDataPointers, R3
LD #.L|currentMap, R4           //load map ID from RAM, which should be a multiple of 2 (if it's not it will load a glitch level)
LD #.H|currentMap, R5

LD %R4, R1

LD ADD %[LR2, IR1], R6          //load lower 16 bits of 32-bit data pointer
LD ADD %[LR2, R1], R7          //load upper 16 bits of 32-bit data pointer

LDI %R6, R3                     //the first 16 bits are the size so store it in R3

LD #256, R2 				//store high 16 bits of address for palette write

LD QI #1, R1				//prepare DMA transfer (the destination address should point to VRAM already)
STRI R6, %R1
STRI R7, %R1
LD QI #5, R1
STR R3, %R1				    //initiate DMA transfer

LD QI #0, R1				//DMA copy the tile drawing routine to RAM
LD #0xFFFF, R0
STRI R0, %R1
LD QI #3, R1				//initiate second DMA transfer, this time for the tilemap data (which should be located after the tile data)
LD #16384, R0
STRI R0, %R1
LD #768, R0
STRI R0, %R1
LD #1921, R0
STRI R0, %R1

LD QI #3, R1				//initiate third DMA transfer, this time for palettes (which should be located after the tilemap data)
LD QI #16, R0
STRI R0, %R1
LD #512, R0
STRI R0, %R1
LD #32, R0
STRI R0, %R1

LD QI #0, R1				//DMA copy the tile drawing routine to RAM
LD #0xFFFF, R0
STRI R0, %R1
LD #L|tileCopyRoutine, R0
STRI R0, %R1
LD #H|tileCopyRoutine + 2048, R0
STRI R0, %R1
LD #.L|TCCode, R0
STRI R0, %R1
LD QI #0, R0
STRI R0, %R1
LD #256, R0
STRI R0, %R1

LD %R4, R1

LD #L|mapDataPointers, R4        //load base address of data pointer list
LD #H|mapDataPointers, R5
LD ADD %[LR4, IR1], R6          //load lower 16 bits of 32-bit data pointer
LD ADD %[LR4, R1], R7          //load upper 16 bits of 32-bit data pointer

LD %R6, R1                      //store pointer in RAM so that it can be referenced directly to improve performance
LD QI #.L|mapPointer, R2
LD QI #0, R3
STRI R6, %R2
STRI R7, %R2
LD QI #.L|metatileX, R0
LD QI #0, R1
STR #0, %R0

LD QI #.L|zoom, R0

STR #1024, %R0

LD QI #.L|scrollCounterH, R0
STR #0, %R0
INC R0
STR #0, %R0
LD QI #.L|columnLeftX, R0
STR #0, %R0
INC R0
STR #63, %R0
INC R0
STR #0, %R0
INC R0
STR #64, %R0

LD QI #0x3D, R0
LD #256, R1

STR #128, %R0
INC R0
STR #120, %R0

LD QI #.L|OAMDefault, R0				//store OAM clear value in RAM
LD QI #0, R1
LD #0xFFFF, R2
STR R2, %R0

LD QI #.L|metatileCount, R0
LD QI #0, R1
STR R1, %R0

LD QI #.L|metatileX, R0
STRI R1, %R0
STRI R1, %R0

$tileInitLoop
CALL #0x200

LD QI #.L|metatileCount, R0
LD QI #0, R1
LD %R0, R2
INC %R0

SHL #6, R2, R3
AND #63, R2, R4

LD QI #.L|metatileX, R0
STRI R4, %R0
STR R3, %R0

LD #4096, R3
CMP R3, R2
JMPR GRR, $tileInitLoop

$Idle

//handle submap column

LD QI #.L|scrollX, R0
LD QI #0, R1
LDI %R0, R2             //load horizontal scroll position into R2-R3
LDI %R0, R3
LDI %R0, R4             //load horizontal velocity into R4

LD QI #.L|metatileX, R0

JMPR GRR #32768, R4, $checkDirectionHB
LD QI #.L|columnLeftX, R5                   //check which direction should be active (for directional submap optimization)
LD QI #0, R6
STR %R5, %R0
JMPR $checkDirectionHEnd
$checkDirectionHB
LD QI #.L|columnRightX, R5
LD QI #0, R6
STR %R5, %R0
$checkDirectionHEnd

INC R0
LD QI #.L|rowUpY, R5
STR %R5, %R0

LD QI #.L|metatileCount, R0
LD QI #0, R1
STR R1, %R0

$tileCopyLoop
CALL #0x200

LD QI #.L|metatileY, R0
LD QI #0, R1
INC %R0
LD QI #.L|metatileCount, R0
INC %R0
LD %R0, R2

LD QI #65, R3
CMP R3, R2
JMPR GRR, $tileCopyLoop

//handle submap row

LD QI #.L|scrollY, R0
LD QI #0, R1
LDI %R0, R2             //load horizontal scroll position into R2-R3
LDI %R0, R3
LDI %R0, R4             //load vertical velocity into R4

LD QI #.L|metatileY, R0

JMPR GRR #32768, R4, $checkDirectionVB
LD QI #.L|rowUpY, R5                   //check which direction should be active (for directional submap optimization)
LD QI #0, R6
STR %R5, %R0
JMPR $checkDirectionVEnd
$checkDirectionVB
LD QI #.L|rowDownY, R5
LD QI #0, R6
STR %R5, %R0
$checkDirectionVEnd

DEC R0
LD QI #.L|columnLeftX, R5
STR DEC %R5, %R0

LD QI #.L|metatileCount, R0
LD QI #0, R1
STR R1, %R0

$tileCopyLoopH
CALL #0x200

LD QI #.L|metatileX, R0
LD QI #0, R1
INC %R0
LD QI #.L|metatileCount, R0
INC %R0
LD %R0, R2

LD QI #65, R3
CMP R3, R2
JMPR GRR, $tileCopyLoopH

LD QI #.L|metatileX, R0
INC %R0

LD #256, R2				//Fill OAM with default value of 65535
LD QI #0, R1
LD QI #1, R0
STRI R0, %R1
LD #.L|OAMDefault, R0
STRI R0, %R1
LD QI #0, R0
STRI R0, %R1
LD QI #0, R0
STRI R0, %R1
LD #1024, R0
STRI R0, %R1
LD #2048, R0
STRI R0, %R1

LD QI #0, R1				//initialize OAM address
LD #1024, R2

LD QI #.L|BGScrollX, R3
LD QI #0, R4
LD %R3, R5
NOT R5, R6
AND #7, R6, R6
ADD #505, R6, R6
OR #8704, R6, R6
SHL #3, R5, R7
AND #63, R7, R7
ADD #16384, R7, R7
                //write OAM data for background
STRI R6, %R1
LD #0, R0
STRI R0, %R1
LD #7456, R0
STRI R0, %R1
STRI R7, %R1

LD QI #8, R1

LD #58368, R0				//write OAM data for foreground
STRI R0, %R1
LD #1536, R0
STRI R0, %R1
LD #61439, R0
STRI R0, %R1
LD QI #0, R0
STRI R0, %R1

LD QI #.L|zoom, R3
LD QI #0, R4
LD %R3, R5

STR R5, %R1
INC R1
STRI R5, %R1
LD QI #0, R0				//store low 16 bits of address for palette write
STRI R0, %R1
LD QI #0, R0				//store low 16 bits of address for palette write
STRI R0, %R1

LD QI #.L|velocityH, R0
LD %R0, R6

JMPR LTR #32768, R6, $ClampHVelB
JMPR GRR #4096, R6, $ClampHVelC
STR #4096, %R0
$ClampHVelC
JMPR $ClampHVelEnd

$ClampHVelB
JMPR LTR #61440, R6, $ClampHVelEnd
STR #61440, %R0
$ClampHVelEnd

LD QI #.L|scrollX, R2         //store address of horizontal scroll
LD %R2, R0
LDI %R2, R6
LDI %R2, R7

LD QI #.L|velocityH, R2     //perform either 32-bit addition or subtraction depending on the sign bit
LD %R2, R4
LD QI #0, R5
LD QI #0, R1
JMPR GRR #32768, R4, $checkHVelNegativeB
NOT R4, R4
INC R4
SUBL R0, R4, R0
JMPR $checkHVelNegativeEnd
$checkHVelNegativeB
ADDL R0, R4, R0
$checkHVelNegativeEnd

LD R0, R6
ADD R7, R1, R7

LD QI #.L|scrollX, R2         //store address of horizontal scroll

STRI R6, %R2                //store the result in the horizontal scroll position
STRI R7, %R2

LD QI #0x39, R2         //store address of affine horizontal scroll register
LD #256, R3

ADDL #65535, R6, R0     //offset the horizontal scroll position
LD R0, R6
ADD R7, R1, R7

ADDL #32769, R6, R0     //offset the horizontal scroll position a second time
LD R0, R6
ADD R7, R1, R7

STRI R6, %R2            //store the result in the affine scroll register
STRI R7, %R2

LD QI #.L|velocityH, R2         //load address of horizontal velocity into R2-R3
LD QI #0, R3
LD %R2, R4                      //store horizontal velocity in R4

LD QI #.L|scrollCounterH, R2    //load the address of the scroll counter
LD %R2, R5
ADD R4, R5, R4                  //add regardless of the sign bit
LD QI #.L|columnLeftX, R2

JMPR GRR #4095, R4, $UpdateMetatileHEnd     //decide whether to increment or decrement the starting positions of the tile columns
JMPR LTR #32768, R4, $UpdateMetatileHB
INC %R2
INC R2
INC %R2
JMPR $UpdateMetatileHEnd
$UpdateMetatileHB
DEC %R2
INC R2
DEC %R2
$UpdateMetatileHEnd

LD QI #.L|scrollCounterH, R2
AND #4095, R4, %R2

LD QI #.L|velocityH, R2         //load address of horizontal velocity into R2-R3
LD QI #0, R3
LD %R2, R4                      //store horizontal velocity in R4

LD QI #.L|BGScrollCounter, R2    //load the address of the scroll counter
LD %R2, R5
ADD R4, R5, R4                  //add regardless of the sign bit
LD QI #.L|BGScrollX, R2

JMPR GRR #4095, R4, $UpdateBGScrollHEnd     //decide whether to increment or decrement the horizontal scroll
JMPR LTR #32768, R4, $UpdateBGScrollHB
INC %R2
JMPR $UpdateBGScrollHEnd
$UpdateBGScrollHB
DEC %R2
$UpdateBGScrollHEnd

LD QI #.L|BGScrollCounter, R2
AND #4095, R4, %R2

LD QI #.L|velocityV, R0
LD %R0, R6

JMPR LTR #32768, R6, $ClampVVelB
JMPR GRR #4096, R6, $ClampVVelC
STR #4096, %R0
$ClampVVelC
JMPR $ClampVVelEnd

$ClampVVelB
JMPR LTR #61440, R6, $ClampVVelEnd
STR #61440, %R0
$ClampVVelEnd

LD QI #.L|scrollY, R2         //store address of horizontal scroll
LD %R2, R0
LDI %R2, R6
LDI %R2, R7

LD QI #.L|velocityV, R2     //perform either 32-bit addition or subtraction depending on the sign bit
LD %R2, R4
LD QI #0, R5
LD QI #0, R1
JMPR GRR #32768, R4, $checkVVelNegativeB
NOT R4, R4
INC R4
SUBL R0, R4, R0
JMPR $checkVVelNegativeEnd
$checkVVelNegativeB
ADDL R0, R4, R0
$checkVVelNegativeEnd

LD R0, R6
ADD R7, R1, R7

LD QI #.L|scrollY, R2         //store address of vertical scroll

STRI R6, %R2                //store the result in the vertical scroll position
STRI R7, %R2

LD QI #0x3B, R2         //store address of affine vertical scroll register
LD #256, R3

ADDL #65535, R6, R0     //offset the vertical scroll position
LD R0, R6
ADD R7, R1, R7

ADDL #32769, R6, R0     //offset the vertical scroll position a second time
LD R0, R6
ADD R7, R1, R7

STRI R6, %R2            //store the result in the affine scroll register
STRI R7, %R2

LD QI #.L|velocityV, R2         //load address of horizontal velocity into R2-R3
LD QI #0, R3
LD %R2, R4                      //store vertical velocity in R4

LD QI #.L|scrollCounterV, R2    //load the address of the scroll counter
LD %R2, R5
ADD R4, R5, R4                  //add regardless of the sign bit
LD QI #.L|rowUpY, R2

JMPR GRR #4095, R4, $UpdateMetatileVEnd     //decide whether to increment or decrement the starting positions of the tile columns
JMPR LTR #32768, R4, $UpdateMetatileVB
INC %R2
INC R2
INC %R2
JMPR $UpdateMetatileVEnd
$UpdateMetatileVB
DEC %R2
INC R2
DEC %R2
$UpdateMetatileVEnd

LD QI #.L|scrollCounterV, R2
AND #4095, R4, %R2

LD QI #6, R2 	//store low 16 bits of address for GPU activation IO write
LD #256, R3 				//store high 16 bits of address for GPU activation IO write
STR #1, %R2



LD QI #0x38, R0         //copy controller input to RAM
LD #256, R1
LD QI #.L|controllerInput, R2
LD QI #0, R3
STR %R0, %R2

LD %R2, R4              //check for left input (decrement horizontal velocity if left pressed)
LD QI #.L|velocityH, R2
AND #1, R4, R5
JMPR EQR #0, R5, $EndLeftInput
LD %R2, R6
SUB #16, R6, %R2
$EndLeftInput

AND #2, R4, R5              //check for right input (increment horizontal velocity if right pressed)
JMPR EQR #0, R5, $EndRightInput
LD %R2, R6
ADD #16, R6, %R2
$EndRightInput

LD QI #.L|velocityV, R2

AND #4, R4, R5              //check for up input (zoom in if up pressed)
JMPR EQR #0, R5, $EndUpInput
LD %R2, R6
SUB #16, R6, %R2
$EndUpInput

AND #8, R4, R5              //check for down input (zoom out if up pressed)
JMPR EQR #0, R5, $EndDownInput
LD %R2, R6
ADD #16, R6, %R2
$EndDownInput

LD QI #.L|zoom, R2

AND #16, R4, R5              //check for up input (zoom in if up pressed)
JMPR EQR #0, R5, $EndAInput
DEC %R2
$EndAInput

AND #32, R4, R5              //check for down input (zoom out if up pressed)
JMPR EQR #0, R5, $EndBInput
INC %R2
$EndBInput

SYNC
JMPR $Idle





















$tileCopyRoutine
LD QI #.L|mapPointer, R0        //chunk map
LD QI #0, R1                    //R1 is assumed to be 0
LDI %R0, R2                     //load the address into R2-R3, from RAM
LDI %R0, R3

LD QI #.L|metatileX, R0         //load the metatile X into R4, and the metatile Y into R5
LDI %R0, R4
LDI %R0, R5

SHL #4, R4, R6                  //store the shifted X coordinate in R6
AND #127, R6, R6
SHL #4, R5, R7                  //store Y coordinate / 16
AND #127, R7, R7
SHR #7, R7, R7                  //then multiply it by 128
ADD R7, R6, R7                  //add the X and Y coordinate together

SHL #1, R7, R6

LD ADD %[LR2, R6], R5           //store the result in R5
AND #1, R7, R6                  //then check if it's even or odd
JMPR EQR #0, R6, $endShiftFunc
SHL #8, R5, R5
$endShiftFunc
AND #255, R5, R6

//chunk attribute map

LD #L|mapDat, R2                //load the base address of the chunk attribute map into R2-R3
LD #H|mapDat, R3

SHL #1, R6, R5                  //store left shifted value in R5, even/odd bit in R6
LD ADD %[LR2, R5], R7           //store value in R7
AND #1, R6, R5
JMPR EQR #0, R5, $endShiftFuncTwo    //check if it's even or odd
SHL #8, R7, R7
$endShiftFuncTwo
AND #255, R7, R7
STRI R7, %R0

//row map
LD QI #.L|metatileY, R0         //load base address
LDI %R0, R2         //store metatile Y coordinate in R2 (the row index)
AND #15, R2, R2
SHR #4, R2, R2                  //the row map is 16 rows wide

SHL #4, R6, R5                  //load shifted chunk index into R5
SHR #8, R5, R5                 //index the next tile over if greater than 16
AND #15, R6, R6                 //wrap it within the row map width
ADD R6, R5, R6                  //add it together
ADD R6, R2, R6
AND #1, R6, R4                  //even or odd bit in R4, left shifted value in R5
SHL #1, R6, R5
LD #L|mapRowMap, R2             //load base address
LD #H|mapRowMap, R3
LD ADD %[LR2, R5], R7           //then the value from memory into R7 and check if the offset is even or odd
JMPR EQR #0, R4, $endShiftFuncThree
SHL #8, R7, R7
$endShiftFuncThree
AND #255, R7, R7

//row meta-map

LD QI #.L|metatileX, R0         //load base address
LDI %R0, R2                     //store metatile X coordinate in R2 (the position within the row)
AND #15, R2, R2                 //AND it to keep it within the row

SHR #4, R7, R4                  //R7 contains the row index, shift right to get the base address of the current row
LD R4, R7                       //keep a copy of the lower bounds in R7
ADD #16, R4, R5                 //store the upper bounds of the current row in R5
ADD R4, R2, R4                  //add the X coordinate

LD QI #.L|chunkAttribute, R0        //load base address
LDI %R0, R2                         //then load stored chunk attribute value
AND #16, R2, R6                     //isolate sign bit
JMPR EQR #16, R6, $functionAPartB   //conditional jump depending on if the value is negative or positive
AND #15, R2, R6                     //isolate 4-bit offset
ADD R4, R6, R4
JMPR $functionAEnd
$functionAPartB
AND #15, R2, R6                     //isolate 4-bit offset
SUB R4, R6, R4
$functionAEnd

AND #32, R2, R6                     //check for the tile bank flag
JMPR EQR #0, R6, $functionBEnd
ADD #4096, R4, R4
ADD #4096, R5, R5
ADD #4096, R7, R7
$functionBEnd

DEC R7

CMP R7, R4                          //check the lower bounds of the current row first
JMPR LTR, $functionCPartB
LD QI #0, R7
JMPR $metaRowFunctionEnd
$functionCPartB
CMP R5, R4                          //then the upper bounds
JMPR GRR, $functionCEnd
LD QI #0, R7
JMPR $metaRowFunctionEnd
$functionCEnd

LD #L|mapRowMetaMap, R2             //load base address
LD #H|mapRowMetaMap, R3
LD ADD %[LR2, R4], R7               //then the value from memory into R7

$metaRowFunctionEnd

//meta map

LD QI #.L|metatileX, R0             //load base address
LDI %R0, R2                         //load metatile X into R2, metatile Y into R3
LDI %R0, R3

//start with base address of VRAM

SHR #1, R2, R2                      //shift right to get horizontal metatile coordinate
AND #127, R2, R2                    //wrap so that it stays within the horizontal tilemap area
AND #63, R3, R3                    //wrap the vertical offset too
SHR #8, R3, R3                      //store the vertical offset in R3
ADD R2, R3, R2                      //add the horizontal and vertical offset together

LD #768, R3                         //load the upper 16 bits of the base address of VRAM

SHR #2, R7, R4                      //move shifted tile index to R4

LD #L|mapMetaMap, R0                //store base address of metatile map
LD #H|mapMetaMap, R1

LD ADD %[LR0, IR4], R7              //load tiles one by one into VRAM
STRI R7, %R2
LD ADD %[LR0, IR4], R7
STR R7, %R2
LD ADD %[LR0, IR4], R7
ADD #127, R2, R2
STRI R7, %R2
LD ADD %[LR0, IR4], R7
STR R7, %R2
RET #0

$mapDat
include data\chunkAttributeMap.bin*
$mapRowMap
include data\rowMap.bin*
$mapRowMetaMap
include data\rowMetaMap.map.bin*
$mapMetaMap
include data\trackRowSourceTiles.map.bin*

$mapOne
include data\chunkMap1.bin*

$paletteDat
include data\trackRowSourceTiles.pal.bin*
include data\trackRowSourceTiles.img.bin*

$BGOne
.INSERT #2449
include data\bedroomBG.img.bin*
include data\bedroomBG.map.bin*
include data\bedroomBG.pal.bin*

$BGDataPointers
.INSERT $BGOne + 134217728

$mapDataPointers
.INSERT $mapOne

$RAMClearValue
.INSERT #0