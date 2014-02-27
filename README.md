setbin
======

Change some bytes values of a binary file 

This allows to provide a list of offsets/values to change in a file. So a copy of the changed file is generated.

    USAGE: ./setbin.elf source_file dest_file offset value [offset2 value2 [...]]

Note that offsets and value are provided in decimal.
