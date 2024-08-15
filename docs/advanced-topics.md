# Advnced 6502 topics

In this section we will cover more advanced topics about the 6502 microprocessor. We will cover topics like the stack, interrupts, and even more. Lets get started!

## The Stack

The stack is a segment of memory used for temporary storage. It is located in the address range 0x0100 to 0x01FF. The stack is used to store various things, such as the location the program should return to after a subroutine is called, as well as the state of the microprocessor before an interrupt occurs (we will cover interrupts next). The stack is a LIFO (Last In, First Out) data structure, which means that the last thing put on the stack will be the first thing taken off. For example, let's say we push the value 0x01 onto the stack and then push the value 0x02. If we were to pull the values off the stack, we would get 0x02 first and then 0x01. Since the order is LIFO, the stack starts at the top and moves down.

## Interrupts

An interrupt is a signal sent to the microprocessor either by hardware on the IRQ and NMI pins or by software. When an interrupt occurs, the microprocessor will stop what it is doing jump to the interrupt vector. The interrupt vector is a memory address the redirects the program to a specific location in memory that is used to handle the interrupt. The 6502 microprocessor has two types of interrupts:

### IRQ (Interrupt Request)

The IRQ interrupt is a hardware interrupt that is triggered when a device sends a signal to the IRQ pin. The IRQ interrupt is used for non-critical tasks since it can be disabled by the microprocessor by clearing the interrupt disable flag. The IRQ is use for things like reading from a keyboard. Here is the process of how the IRQ interrupt works:

1. A device sends a signal to the IRQ pin
2. The microprocessor checks if the interrupt disable flag is set
3. The microprocessor pushes the program counter and the status register onto the stack and sets the interrupt disable flag
4. The microprocessor jumps to the IRQ vector
5. The microprocessor executes the code at the IRQ vector
6. The microprocessor pops the program counter and the status register off the stack and clears the interrupt disable flag and continues executing the program

### NMI (Non-Maskable Interrupt)

The NMI interrupt is another hardware interrupt that is triggered when a device sends a signal to the NMI pin. However, the NMI interrupt is used for critical tasks since it cannot be disabled by the microprocessor. The NMI interrupt is used for things like a power failure. Here is the process of how the NMI interrupt works:

1. A device sends a signal to the NMI pin
2. The microprocessor pushes the program counter and the status register onto the stack
3. The microprocessor jumps to the NMI vector
4. The microprocessor executes the code at the NMI vector
5. The microprocessor pops the program counter and the status register off the stack and continues executing the program

## Conditional Branching

Another important concept in programming is conditional statments. Conditional statements are used to execute code when a condition is met. The 6502 microprocessor has a few opcodes that are used for conditional branching. Here are the conditional branch opcodes:

- **BEQ (Branch if Equal)**: Branches if the zero flag is set
- **BNE (Branch if Not Equal)**: Branches if the zero flag is not set
- **BCS (Branch if Carry Set)**: Branches if the carry flag is set
- **BCC (Branch if Carry Clear)**: Branches if the carry flag is not set
- **BMI (Branch if Minus)**: Branches if the negative flag is set
- **BPL (Branch if Positive)**: Branches if the negative flag is not set
- **BVS (Branch if Overflow Set)**: Branches if the overflow flag is set
- **BVC (Branch if Overflow Clear)**: Branches if the overflow flag is not set

## Lets write some code

Lets make a program that will use the stack, interrupts, and conditional branching. We will write a program that will change the accumulator between 0x00 and 0x01 when the user triggeres an interrupt. Lets get started!

Step 1: Create a new file called `main.asm` and add the following code:

```asm

        ; Setup the memory locations for the program
        .org $FFFA   ; Vector Table location
        .word NMI    ; NMI Vector
        .word RESET  ; Reset Vector
        .word IRQ    ; IRQ Vector

        .org $0000   ; Program start location
RESET:
        SEI          ; Disable interrupts
        CLD          ; Clear decimal mode
        LDX #$FF     ; Initialize stack pointer
        TXS          ; Transfer X to stack pointer
        LDA #$00     ; Initialize the accumulator to 0x00
        CLI          ; Enable interrupts
LOOP:
        JMP LOOP     ; Infinite loop, waiting for an interrupt

NMI:
IRQ:
        LDA #$00     ; Load 0x00 into the accumulator
        CMP $0201    ; Compare with the last toggled value stored in $0201
        BEQ TOGGLE_1 ; If accumulator is 0x00 (equal), branch to toggle to 0x01

TOGGLE_0:
        LDA #$00     ; Set the accumulator to 0x00
        STA $0201    ; Store the value in $0201 for next comparison
        JMP DONE     ; Jump to end of ISR

TOGGLE_1:
        LDA #$01     ; Set the accumulator to 0x01
        STA $0201    ; Store the value in $0201 for next comparison

DONE:
        RTI          ; Return from interrupt

```

Step 2: Assemble the code using the `vasm` assembler:

```bash
vasm6502_oldstyle -Fbin -dotdir main.asm -o main.bin
```

Step 3: Load the binary file onto the 6502 emulator or hardware and trigger an interrupt to see the accumulator toggle between 0x00 and 0x01.

If you are using my 6502 emulator we can watch this work by stepping through the code and triggering an interrupt.

1. step through the program until you reach the `loop` label which is at 0x0008 and you should not be able to keep stepping through the program
2. Trigger an interrupt by pressing the `i` key on your keyboard
3. The program should jump to the NMI vector and then to the IRQ vector
4. Step through the program until you get returned back to the `loop` label at 0x0008
5. Repeat steps 2-4 to see the accumulator toggle between 0x00 and 0x01

## Conclusion

In this section we covered some more advanced topics which allow for control flow in your programs aswell as handling interrupts. These are important concepts to understand when writing programs for any microprocessor.

## [Next Section: Architecture](architecture.md)

**If you have any questions, comments, or concerns, please feel free to reach out to me in the issues page** ❤️
