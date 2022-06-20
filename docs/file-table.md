# File Table

A file table entry uses the following format:\
Name, starting sector, ending sector
$c    $2               $2   

Each entry is $10 bytes in length. This means that a drive can contain 64 files


This drive format is incompatible with fragmented files. Maybe this can be addressed in a future version