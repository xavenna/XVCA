# Drive format documentation

XVCA contains a virtual drive. This reads information from a file on the host computer.

A drive has the file extension `.xdr'\
The drive file must be a multiple of 1 KiB in length (for now)

It is divided up into blocks. Each block holds 1 KiB of data, and blocks can be loaded into the drive buffer at $F400\

## Block Scheme

The first block is the boot block. This is what the bootloader loads during the boot process
The second block is the file table.
The rest is for files