setbin
======

Change in a binary file the value of bytes.

You just provide a list of offsets/values to change in a file and a target file name, then a copy of the changed file is generated.

    USAGE: ./setbin.elf source_file dest_file offset value [offset2 value2 [...]]

Note that offsets and value are provided in decimal at the time.

