#include "../include/mos6502.h"

int main(){
    mos6502 Cpu;

    std::ifstream file("./example.bin", std::ios::binary); // Open the binary ROM file

    if (!file) {
        std::cerr << "Error opening ROM file." << std::endl;
        return 1;
    }
    // Read the contents of the binary file into a vector
    std::vector<uint8_t> program((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close(); // Close the file after reading

    //Load Memory with ROM file
    Cpu.loadMemory(program);

    //Reset CPU to known state
    Cpu.reset();

    //Run the CPU
    while (true) {
        Cpu.step();
    }

}
