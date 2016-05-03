MIPS Processor 32-bit
This repository contains code for the 32-bit mips processor (without pipelining). The code is written in map-c and it is hardware sythesizable on SRC machine.
The directories in this repository are organized as follow:

1. src 
   Contains the source code for hardware executable (in map-c), simulator (in c) and related header files. It also contains the scripts required for building the code.

2. exe
   Contains a pre-compiled hardware executable for map-c code. The log file generated while building this executable is also present in this directory.

3. tests
   Contains simple test files to verify the functionaility of MIPS machine. The .hex files can be given as input to the executables and the corresponding *.s files
   contain the assembly code for the hex files. The assembly code is there just to make sense of the hex files.

HOW TO BUILD THE PROJECT?
In order to build this project, do the following:

1. Go to the src directory:
   >>cd src

2. To generate debug executable, run the following command:
   >>make debug

   This will result in the generation of "mips_map.dbg" file.

3. To generate hardware executable, run the following command:
   >>make

   This will generate "mips_map.hw" file. The process will take ~30-40 mins.

4. In order to run the executable, enter the following command:
   >>./<mips_map.hw or mips_map.dbg> <hex_file>

   e.g.
   >>./mips_map.hw ../tests/simple.hex

5. The precompiled hardware executable can be used as is. To run it, do the following:
   >>cd exe
   >>./hw_exe <hex_file>

6. The execution results are generated in the same directory as the one containing the executable files. These results can be seen in "res_map" and "res_cpu" files.
   The "res_map" file contains the output from the map-c executable whereas the "res_cpu" file contains the output from the simulator code. The contents of these
   files can be matched to verify the integrity of hardware executable.
