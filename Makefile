MCU = atmega16
F_CPU = 14318180
OPT = s

SRCDIR = src
BINDIR = bin
INCDIR = inc

TARGET = NTSC_mega16
SRCS = $(wildcard $(SRCDIR)/*.c)

all: $(BINDIR)/$(TARGET).hex
	
$(BINDIR)/$(TARGET).elf: $(SRCS)
	avr-gcc -mmcu=$(MCU) -O$(OPT) $^ -o $(BINDIR)/$(TARGET).elf

$(BINDIR)/$(TARGET).hex: $(BINDIR)/$(TARGET).elf
	avr-objcopy -j .text -j .data -O ihex $(BINDIR)/$(TARGET).elf $(BINDIR)/$(TARGET).hex

clean:
	rm -f $(BINDIR)/$(TARGET).elf $(BINDIR)/$(TARGET).hex
