# Instruction Set

XVCA uses an instruction set (creatively) named XVCA. The current version is XVCA-P1.1r


## Instruction Table

### Move Instructions
<v> represents a value, <a> represents an address

| Num | ASM Form | Description |
|-----|----------|-------------|
| 01  | MVRA X   | Move X to A |
| 02  | MVRA Y   | Move Y to A |
| 03  | MVRX A   | Move A to X |
| 04  | MVRX Y   | Move Y to X |
| 05  | MVRY A   | Move A to Y |
| 06  | MVRY X   | Move X to Y |
| 07  | MVVA <v> | Move value to A |
| 08  | MVVX <v> | Move value to X |
| 09  | MVVY <v> | Move value to Y |
| 0a  | MVMA <a> | Move A to memory address |
| 0b  | MVMX <a> | Move X to memory address |
| 0c  | MVMY <a> | Move Y to memory address |
| 0d  | MVIA     | Move A to the location stored in XY |
| 0e  | MVIX     | Move X to the location stored in XY |
| 0f  | MVIY     | Move Y to the location stored in XY |
| 10  | MVAM <a> |
| 11  | MVXM <a> |
| 12  | MVYM <a> |
| 13  | MVAI
| 14  | MVXI
| 15  | MVYI
| 16  | 