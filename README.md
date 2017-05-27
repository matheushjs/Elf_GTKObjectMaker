# Elf_GTKObjectMaker
Application for building a map of objects, which is supposed to be used for the final project on course "Sistemas Digitais"

# Motivation
The final project for the course 'Sistemas Digitais' was to develop a game in VHDL language, using a module that provides an interface through which we can display things on the VGA screen.

I developed a module that sequentially "paints" a map on screen by reading the contents of a ROM memory. This ROM memory would merely be a sequence of 32-bit words, where the first 16 bits represent the pixel position, and the next 16 bits specify what is to be painted (object, size, color).

This program, Elf_GTKObjectMaker, provides a GUI for creating pixel maps, and outputs a file that can easily be copy/pasted into the ROM memory .mif file.

This program was finished after the deadline for the project, so it hasn't been used for anything useful. This project was mainly continued for the sake of learning how to make a user interface using C language and GTK+ library.

# Picture

[Example Image](./example.png)
