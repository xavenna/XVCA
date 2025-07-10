# Keyboard Sequences

When the keyboard adapter recieves input, it passes it on to a special, read-only address in memory. Some keys are translated into different ascii characters so each keypress can be stored as a single character:

Keys and their mappings:

| Key | Char [code]  |
|-----|------------|
|Esc  | ESC [0x1b] |
|Backspace| DEL [0x7f] |
|Delete| 
|Tab|
|Up | 0b10000000
|Down| 0b10000001
|Left| 0b10000010
|Right| 0b10000011

Ignored Characters: