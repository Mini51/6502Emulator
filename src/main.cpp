#include <iostream> 
#include <vector>
#include "./emulator/6502.h"
int main( int argc, char* argv[]){ 

cmos6502 Cpu;

std::vector<uint8_t> program = {

};

   Cpu.Load(program, 0x0000);
   Cpu.RESET();
   Cpu.Run();

    return 0;
}