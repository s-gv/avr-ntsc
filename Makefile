# Copyright (c) 2014 Sagar G V (sagar.writeme@gmail.com)

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

MCU = atmega16 # target MCU
F_CPU = 14318180UL # CPU Frequency in Hz.
OPT = s # Optimization level
PROGRAMMER = usbasp
PROGRAMMER_PORT = USB

SRCDIR = src
BINDIR = bin
INCDIR = inc
OBJDIR = obj

TARGET = NTSC_mega16
CSRCS = $(wildcard $(SRCDIR)/*.c)
ASRCS = $(wildcard $(SRCDIR)/*.S)
INCS = $(wildcard $(INCDIR)/*.h)

CFLAGS = -mmcu=$(MCU) -O$(OPT) -I$(INCDIR) -Wall -Wstrict-prototypes -funsigned-char -funsigned-bitfields -ffunction-sections -fpack-struct -fshort-enums -std=gnu99 -gdwarf-2
AFLAGS = -mmcu=$(MCU) -I$(INCDIR) -Wall -std=gnu99 -gdwarf-2
LFLAGS = -mmcu=$(MCU) -O$(OPT) -Wall -Wstrict-prototypes -funsigned-char -funsigned-bitfields -ffunction-sections -fpack-struct -fshort-enums -std=gnu99 -gdwarf-2

CDEFS = -DF_CPU=$(F_CPU)
ADEFS = -DF_CPU=$(F_CPU)

CC = avr-gcc
SIZE = avr-size
OBJCPY = avr-objcopy
DEL = rm -f

COBJS = $(CSRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
AOBJS = $(ASRCS:$(SRCDIR)/%.S=$(OBJDIR)/%.o)

default: $(BINDIR)/$(TARGET).hex
	
all: $(BINDIR)/$(TARGET).hex

program: $(BINDIR)/$(TARGET).hex
	avrdude -c $(PROGRAMMER) -P $(PROGRAMMER_PORT) -B5 -p $(MCU) -U lfuse:w:0xff:m -U hfuse:w:0xc9:m -U flash:w:$(BINDIR)/$(TARGET).hex:i
	
$(COBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCS)
	$(CC) $(CFLAGS) $(ADEFS) -c $< -o $@

$(AOBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.S $(INCS)
	$(CC) $(AFLAGS) $(ADEFS) -c $< -o $@

$(BINDIR)/$(TARGET).elf: $(COBJS) $(AOBJS)
	$(CC) $(CFLAGS) $^ -o $(BINDIR)/$(TARGET).elf
	@echo
	$(SIZE) $(BINDIR)/$(TARGET).elf

$(BINDIR)/$(TARGET).hex: $(BINDIR)/$(TARGET).elf
	@echo
	$(OBJCPY) -j .text -j .data -O ihex $(BINDIR)/$(TARGET).elf $(BINDIR)/$(TARGET).hex

clean:
	$(DEL) $(BINDIR)/$(TARGET).elf $(BINDIR)/$(TARGET).hex $(OBJDIR)/*.o
