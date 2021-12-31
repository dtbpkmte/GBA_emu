# GBA Emulator

A GBA Emulator written in C++ for learning purpose. This project is inspired by YouTube channel javid9x's NES Emulator, and also uses his Pixel Game Engine for the graphics.

My objective is to run Pokemon games.

## Progress

The project now features an User Interface which shows the whole memory (address, hex, disassembly), CPU registers, opcode (shown in hex and bin), and status register. It allows for user input to control:

- Up/Down arrow to scroll up and down the memory box
- Left/Right arrow to go to previous/next memory page (each page has 16 32-bit data entries, which may contain code or data)
- S to step, execute an instruction. The current instruction (which has just been executed) will be highlighted in yellow.
- Space to run till the end of the program, or pause the running program
- Enter to input an address to go to (see Fig.2). While in input mode (with the green popup), you can use keys 0-9 and A-F to enter a hexadecimal number, Backspace to delete a character, Enter to go to the input address, or Escape to close the green box.

The rectangular box on the top left will be the 240x160 screen of the GBA.

The disassembler is now working with ARM instructions. Thumb code support is being added.

![image](https://user-images.githubusercontent.com/46307950/147797696-a840f0e0-2433-433a-b0d3-c18bf5b070de.png)
*Fig.1: overview of the UI, with an example code of adding 10 to 5*

![image](https://user-images.githubusercontent.com/46307950/147801440-52b95051-05a5-44a8-8f73-6aa8677be4bd.png)
*Fig.2: address input mode*

Note: the processor is ARM7TDMI, which is version 4.