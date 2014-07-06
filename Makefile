MCU = atmega16 # target MCU
F_CPU = 14318180UL # CPU Frequency in Hz.
OPT = s # Optimization level

SRCDIR = src
BINDIR = bin
INCDIR = inc
OBJDIR = obj

TARGET = NTSC_mega16
CSRCS = $(wildcard $(SRCDIR)/*.c)
ASRCS = $(wildcard $(SRCDIR)/*.S)
INCS = $(wildcard $(INCDIR)/*.h)

CFLAGS = -mmcu=$(MCU) -O$(OPT) -I$(INCDIR) -Wall -Wstrict-prototypes -funsigned-char -funsigned-bitfields -ffunction-sections -fpack-struct -fshort-enums -std=gnu99 -gdwarf-2
AFLAGS = -mmcu=$(MCU) -O$(OPT) -I$(INCDIR) -Wall -std=gnu99 -gdwarf-2
LFLAGS = -mmcu=$(MCU) -O$(OPT) -Wall -Wstrict-prototypes -funsigned-char -funsigned-bitfields -ffunction-sections -fpack-struct -fshort-enums -std=gnu99 -gdwarf-2

CDEFS = -DF_CPU=$(F_CPU)
ADEFS = -DF_CPU=$(F_CPU)

CC = avr-gcc
SIZE = avr-size

COBJS = $(CSRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
AOBJS = $(ASRCS:$(SRCDIR)/%.S=$(OBJDIR)/%.o)

default: $(BINDIR)/$(TARGET).hex
	
all: $(BINDIR)/$(TARGET).hex
	
$(COBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCS)
	$(CC) $(CFLAGS) $(ADEFS) -c $< -o $@

$(AOBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.S $(INCS)
	$(CC) $(AFLAGS) $(ADEFS) -c $< -o $@

$(BINDIR)/$(TARGET).elf: $(COBJS) $(AOBJS)
	@echo
	$(CC) $(CFLAGS) $^ -o $(BINDIR)/$(TARGET).elf
	$(SIZE) $(BINDIR)/$(TARGET).elf

$(BINDIR)/$(TARGET).hex: $(BINDIR)/$(TARGET).elf
	@echo
	avr-objcopy -j .text -j .data -O ihex $(BINDIR)/$(TARGET).elf $(BINDIR)/$(TARGET).hex

clean:
	rm -f $(BINDIR)/$(TARGET).elf $(BINDIR)/$(TARGET).hex $(OBJDIR)/*.o
