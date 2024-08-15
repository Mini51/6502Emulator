#include "../include/mos6502.h"

int main()
{
    mos6502 Cpu;

    std::ifstream file("./example.bin", std::ios::binary); // Open the binary ROM file

    if (!file)
    {
        std::cerr << "Error opening ROM file." << std::endl;
        return 1;
    }

    std::vector<uint8_t> program((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::cout << "Loaded " << program.size() << " bytes of program." << std::endl;

    Cpu.loadMemory(program);

    std::cout << "Loaded program into memory." << std::endl;

    Cpu.reset();

    std::cout << "CPU reset." << std::endl;

    std::cout << "Starting CPU..." << std::endl;

    std::cout << "Commands: \n"
              << "enter: step" << std::endl
              << "q: quit" << std::endl
              << "n: NMI interrupt" << std::endl
              << "i: IRQ interrupt" << std::endl;

    // Run the CPU
    while (true)
    {
        // get the char from the user
        char c = std::cin.get(); // Read input, including the enter key

        if (c == 'q')
        {
            std::cout << "Exiting..." << std::endl;
            break;
        }
        else if (c == 'n')
        {
            Cpu.NMI();
            std::cout << "NMI interrupt" << std::endl;
        }
        else if (c == 'i')
        {
            Cpu.IRQ();
            std::cout << "IRQ interrupt" << std::endl;
        }
        else if (c == '\n') // Handle enter key
        {
            Cpu.step();
            system("clear");
            std::cout << "Registers after step: " << std::endl;
            std::cout << "PC:" << "0x" << std::setw(4) << std::setfill('0') << std::hex << Cpu.getPC() << "\n"
                      << "A:" << "0x" << std::setw(2) << std::setfill('0') << std::hex << (int)Cpu.getAC() << "\n"
                      << "X:" << "0x" << std::setw(2) << std::setfill('0') << std::hex << (int)Cpu.getXR() << "\n"
                      << "Y:" << "0x" << std::setw(2) << std::setfill('0') << std::hex << (int)Cpu.getYR() << "\n"
                      << "SP:" << "0x" << std::setw(2) << std::setfill('0') << std::hex << (int)Cpu.getSP() << "\n"
                      << "Status:" << "0x" << std::setw(2) << std::setfill('0') << std::hex << (int)Cpu.getSR() << std::endl;
            std::cout << "===========================" << std::endl;
            std::cout << "Commands: \n"
                      << "enter: step" << std::endl
                      << "q: quit" << std::endl
                      << "n: NMI interrupt" << std::endl
                      << "i: IRQ interrupt" << std::endl;
        }
    }
}
