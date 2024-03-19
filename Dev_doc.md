To build project, issue following two commands. The excutable can be found in the build folder, 'build/EpromEmulator'

cmake -B build
cmake --build build


To run the eprom emulator, create folder 'build/veprom' then launch EpromEmulator. When it starts, it displays all eprom files in 'build/veprom' folder.


Available comamnds:

* 'list' - displays all eprom files in veprom folder (managed location)

* 'create <<n>>' - creates a new eprom file with n kilo byte size. After the file is created, it's also automatically selected.

* 'load <<file>>' - select an eprom file in the mangaged location for subsequent operations

* 'read_raw <<address>> <<n>>' - display in console n bytes (10-based) of the content of the selected eprom file start from address (10-based)

* 'write_raw <<address>> <<bytes>>' - overwrite the selected eprom file content start from address (10-based) with bytes (in hex string)

* 'write <<source>>' - replace the content of the selected eprom file with that of an external source file.