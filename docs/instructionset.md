# Instruction Set

XVCA uses an instruction set (creatively) named XVCA. The current version is XVCA-P1.1r


## Instruction Table


Needed additions:
INT ? 
EI 
DI
INC <r>
DEC <r>

Move address to XY

some way to move a memory address to a register

<v> represents a value, <a> represents an address, <r> represents a register

* 00 - A
* 01 - B
* 02 - C
* 03 - X
* 04 - Y
* else - INVALID

### Move Instructions

| Num | ASM Form | Description |
--------------------------------
| 01  | MVRA <r> | Move register to A |
| 02  | MVRB <r> | Move register to B |
| 03  | MVRC <r> | Move register to C |
| 04  | MVRX <r> | Move register to X |
| 05  | MVRY <r> | Move register to Y |
| 06  | MVVA <v> | Move value to A |
| 07  | MVVB <v> | Move value to B |
| 08  | MVVC <v> | Move value to C |
| 09  | MVVX <v> | Move value to X |
| 0A  | MVVY <v> | Move value to Y |
| 0B  | MVMA <a> | Move memory address to A |
| 0C  | MVMB <a> | Move memory address to B |
| 0D  | MVMC <a> | Move memory address to C |
| 0E  | MVMX <a> | Move memory address to X |
| 0F  | MVMY <a> | Move memory address to Y |
| 10  | MVIA     | Move A to the location stored in XY |
| 11  | MVIB     | Move B to the location stored in XY |
| 12  | MVIC     | Move C to the location stored in XY |
| 13  | MVIX     | Move X to the location stored in XY |
| 14  | MVIY     | Move Y to the location stored in XY |
| 15  | MVAM <a> | Move A to memory address |
| 16  | MVBM <a> | Move B to memory address |
| 17  | MVCM <a> | Move C to memory address |
| 18  | MVXM <a> | Move X to memory address |
| 19  | MVYM <a> | Move Y to memory address |
| 1A  | MVAI     | Move the value stored in XY to A |
| 1B  | MVBI     | Move the value stored in XY to B |
| 1C  | MVCI     | Move the value stored in XY to C |
| 1D  | MVXI     | Move the value stored in XY to X |
| 1E  | MVYI     | Move the value stored in XY to Y |
| 1F  | MIXY <a> | Move the address pointer to XY   |
| 20  | MSXY     | Move stack pointer to XY |
| 21  | MXYS     | Move XY to stack pointer

### Stack Instructions

| Num | ASM Form | Description |
--------------------------------
| 30  | PUSH A   | Pushes A to the stack |
| 31  | PUSH B   | Pushes B to the stack |
| 32  | PUSH C   | Pushes C to the stack |
| 33  | PUSH X   | Pushes X to the stack |
| 34  | PUSH Y   | Pushes Y to the stack |
| 35  | PUSH F   | Pushes flag register to the stack |
| 36  | POP A    | Pops A from the stack |
| 37  | POP B    | Pops B from the stack |
| 38  | POP C    | Pops C from the stack |
| 39  | POP X    | Pops X from the stack |
| 3A  | POP Y    | Pops Y from the stack |
| 3B  | POP F    | Pops flag register from the stack |
| 3C  | PUSH SP  | Pushes stack pointer to the stack |
| 3D  | PUSH PC  | Pushes program counter to the stack |
| 3E  | POP SP   | Pops stack pointer from the stack |
| 3F  | POP PC   | Pops program counter from the stack |

### Branch Instructions

| Num | ASM Form | Description |
--------------------------------
| 40  | JMP <a>  | Unconditional jump to specified address or label |
| 41  | JZ <a>   | Jumps to specified address if zero flag is set |
| 42  | JNZ <a>  | Jumps to specified address if zero flag is not set |
| 43  | JG <a>   | Jumps to specified address if greater flag is set |
| 44  | JNG <a>  | Jumps to specified addrees if greater flag is not set |
| 45  | CALL <a> | Pushes next address to stack and jumps to specified address or label |
| 46  | RET      | Pops address from stack and jumps to it |

### Arithmetic Instructions

| Num | ASM Form | Description |
--------------------------------
| 50  | ADR <r>  | Adds A with specified register |
| 51  | ADCR <r> | Adds A with specified register, using carry flag |
| 52  | ADV <v>  | Adds A with specified value |
| 53  | ADCV <v> | Adds A with specified value, using carry flag |
| 54  | ADI      | Adds A with value stored in [XY] |
| 55  | ADCI     | Adds A with value stored in [XY], using carry flag |

| 56  | SBR <r>  | Subtracts A and specified register |
| 57  | SBCR <r> | Subtracts A and specified register, using carry flag |
| 58  | SBV <v>  | Subtracts A and specified value |
| 59  | SBCV <v> | Subtracts A and specified value, using carry flag |
| 5A  | SBI      | Subtracts A and value stored in [XY] |
| 5B  | SBCI     | Subtracts A and value stored in [XY], using carry flag |
| 5C  | CMPR <r>  | Compares A and specified register |
| 5D  | CMPV <v>  | Compares A and specified value |
| 5E  | CMPI      | Compares A and value stored in [XY] |


### Logical Instructions

| Num | ASM Form | Description |
--------------------------------
| 70  | ORR <r>  | Calculates logical OR of A and specified register |
| 71  | ORV <v>  | Calculates logical OR of A and specified value |
| 72  | ORI      | Calculates logical OR of A and value stored in [XY] |

| 73  | ANDR <r> | Calculates logical AND of A and specified register |
| 74  | ANDV <v> | Calculates logical AND of A and specified value |
| 75  | ANDI     | Calculates logical AND of A and value stored in [XY] |

| 76  | XORR <r> | Calculates logical XOR of A and specified register |
| 77  | XORV <v> | Calculates logical XOR of A and specified value |
| 78  | XORI     | Calculates logical XOR OF A and value stored in [XY] |

| 79  | NOTR <r> | calculates logical NOT of specified register |
| 7A  | NOTV <v> | calculates logical NOT of specified value |
| 7B  | NOTI     | calculates logical NOT of [XY] |

### Miscellaneous Instructions

| Num | ASM Form | Description |
--------------------------------
| 80  | SPI <v>  | Increments Stack pointer by value |
| 81  | SPIA     | Increments Stack pointer by A |
| 82  | SPD <v>  | Decrements Stack pointer by value |
| 83  | SPDA     | Decrements Stack pointer by A |
| 84  | PCI <v>  | Increments Program counter by value |
| 85  | PCIA     | Increments Program counter by A |
| 86  | PCD <v>  | Decrements Program counter by value |
| 87  | PCDA     | Decrements Program counter by A |
| 88  | SPS <a>  | Sets stack pointer to specified address |
| 89  | INC <r>  | Increments register |
| 8A  | DEC <r>  | Decrements register |

### Interrupt Instructions

| Num | ASM Form | Description |
--------------------------------
| A0  | EI       | Enables interrupts |
| A1  | DI       | Disables interrupts |

### Special Instructions

| Num | ASM Form | Description |
--------------------------------
| 00  | NOP      | Does nothing |
| FF  | HLT      | Halts execution until an interrupt is detected |

### Phony Instructions
db <v>  Inserts specified data, raw, into file
dbstr <string>  Inserts a string constant into file


### Higher-level constructs
for(reg, start, end)
    ...
endfor
