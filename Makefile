MCU = atmega16 # target MCU
F_CPU = 14318180UL # CPU Frequency in Hz.
OPT = s # Optimization level

SRCDIR = src
BINDIR = bin
INCDIR = inc
OBJDIR = obj

TARGET = NTSC_mega16
CSRCS = $(wildcard $(SRCDIR)/*.c)
INCS = $(wildcard $(INCDIR)/*.h)

CFLAGS = -mmcu=$(MCU) -O$(OPT) -I$(INCDIR) -Wall -Wstrict-prototypes -funsigned-char -funsigned-bitfields -ffunction-sections -fpack-struct -fshort-enums 
CDEFS = -DF_CPU=$(F_CPU)

CC = avr-gcc
SIZE = avr-size

OBJS = $(CSRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

default: $(BINDIR)/$(TARGET).hex
	
all: $(BINDIR)/$(TARGET).hex
	
$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCS)
	$(CC) $(CFLAGS) $(CDEFS) -c $< -o $@

$(BINDIR)/$(TARGET).elf: $(OBJS)
	$(CC) $^ -o $(BINDIR)/$(TARGET).elf
	$(SIZE) $(BINDIR)/$(TARGET).elf

$(BINDIR)/$(TARGET).hex: $(BINDIR)/$(TARGET).elf
	avr-objcopy -j .text -j .data -O ihex $(BINDIR)/$(TARGET).elf $(BINDIR)/$(TARGET).hex

clean:
	rm -f $(BINDIR)/$(TARGET).elf $(BINDIR)/$(TARGET).hex $(OBJDIR)/*.o
