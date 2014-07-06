ntsc-atmega16
=============

Generate NTSC Color video using Atmel's Atmega16 8-bit microcontroller.

Software Dependencies
---------------------

- On Linux: gcc-avr, binutils-avr, avr-libc, avrdude. On Ubuntu, you can install all of these with `sudo apt-get update && sudo apt-get install gcc build-essential gcc-avr gdb-avr binutils-avr avr-libc avrdude libusb`
- On Windows: WinAVR, AVRStudio (optional), Some Flash Programmer tool

How To Use
----------

- Build the circuit in `schematics/ntsc_mega16.png`
- Load the binary file in `hex/NTSC_mega16_Color_Palette.hex` to the flash memory of the Atmega16 in the schematic
- Connect the composite video cable to the TV
- A color palette like the one in `figures/ntsc_mega16_color_palette.png` should be visible on the TV

If you make changes to the source, run `make` in the project root to re-compile the source. The binary file in `hex/` will be updated. Alternatively, create a new project with AVR Studio and import the source files from `src/`.
