# XVCA Memory Map

The XVCA Machine has a 16-bit address space, ranging from 0x0000 to 0xffff.\
I/O is memory-mapped to a portion at the end of the address space.

It is divided into several sections:
| Address Range | Size | Name | Description |
---------------------------------------------
| 0x0000 - 0xf3ff | 0xf400 | Main Memory | General RAM. Some is reserved for OS |
| 0xf400 - 0xf7ff | 0x0400 | Drive Buffer| References currently-loaded drive sector |
| 0xf800 - 0xfcff | 0x0500 | Display Buffer | Data from here is written to screen|
| 0xffd0 - 0xffd7 | 0x0008 | Keyboard Buffer | Represents currently pressed keys |
| 0xffd8 - 0xffdf | 0x0008 | Drive Command Buffer | Used to issue commands to drive |
| 0xffe0 - 0xfffe | 0x001f | Unused      | Reserved for future functionality |
| 0xffff          | 0x0001 | Shutdown Buffer | Used to request a shutdown |


Main Memory Subdivisions:
| Address Range   | Size       | Name       | Description |
--------------------------------------------------------
| 0x0000 - 0x03ff | 0x0400     | Bootloader | Where bootloader is placed in memory |
| 0x0400 - 0x07ff | 0x0400     | Interrupts | Contains interrupt jump points |
| 0x07ff - 0x0fff | 0x0800     | OS Reserved| Reserved for OS/Future Use |
| 0x1000 - 0x8fff | 0x8000     | Gen Memory | User-space. Programs are loaded to 0x1000 |
| 0x9000 - 0x93ff | 0x400      | Boot sector| Where bootloader puts bootsector |
| 0x93ff - 0x97ff | 0x400      | OS Varcache| Space for OS temp variables |
| 0x9400 - 0xf3ff | 0x5c00     | OS Space   | Reserved for the OS, Drivers, etc. |



## Interrupt Documentation:

XVCA contains several types of interrupts.
A keyboard interrupt is triggered when a key is pressed.
Keyboard interrupts wake up the CPU from a halted state.

The jump point for a keyboard interrupt is 0x0400.


Timer interrupt - Automatically triggered every hardware update.

Jump point for a timer interrupt is 0x0410.


Syscall interrupt - Can be called by user programs:

Jump point is 0x420
