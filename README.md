ntsc-avr
========

Generate NTSC Color video using Atmel's Atmega16 8-bit microcontroller. A discussion of [NTSC implementation](http://sagargv.blogspot.in/2014/07/ntsc-demystified-color-demo-with.html) using microcontrollers is on my [blog](http://sagargv.blogspot.in/).

Software Dependencies
---------------------

- On Linux: gcc-avr, binutils-avr, avr-libc, avrdude. On Ubuntu, you can install all of these with `sudo apt-get update && sudo apt-get install gcc build-essential gcc-avr gdb-avr binutils-avr avr-libc avrdude`
- On Windows: WinAVR
- On Mac OS X: CrossPack AVR

How To Use
----------

- Build the circuit in `schematics/ntsc_mega16.png`
- Load the binary in `bin/NTSC_mega16.hex` to the flash memory of the Atmega16 in the schematic
- Program the CKOPT fuse to enable full-swing crystal oscillator (make CKOPT=0. See the Makefile for suggested fuse bits)
- Connect the composite video cable to the TV (best viewed on a CRT TV)
- A color palette should be visible on the TV (See `photos/` for snapshots)

If you make changes to the source, run `make` in the project root to re-compile the source. The binary file in `bin/` will be updated.
