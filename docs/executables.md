# Executable format

XVCA uses a custom executable format that is yet to be named

## Sections:

### Header (16 bytes)
4 Bytes: $58 $56 $43 $41 ("XVCA")
2 Bytes: Loading Point
2 Bytes: Jump point
8 Bytes: Null

### Bytecode
Everything else goes here