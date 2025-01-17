# COMPILE BARE METAL BINARIES#
#
# NOTE: use -fno-plt -fno-pic bc otherwise at linking time it tries to put
#       somewhere the relocations as globals.
#
# Use
#
#  $ mipsel-linux-gnu-objdump -b binary -mmips:isa32 -EL -D virus.bin
#
# to see the result

# GENERAL COMPILER OPTIONS
MIPSEL_PREFIX = mips-linux-gnu-
MIPSEL_OPTIONS = -march=4kec -EL -Wall
MIPSEL_LD_OPTIONS = -EL


# TARGETS ###########################
%.bin: %.c startup_%.o %.ld
	$(MIPSEL_PREFIX)gcc $(MIPSEL_OPTIONS) -fno-plt -fno-pic -c $<
	$(MIPSEL_PREFIX)ld $(MIPSEL_LD_OPTIONS) -T$*.ld startup_$*.o $*.o -o $*.elf
	$(MIPSEL_PREFIX)objcopy -O binary $*.elf $@

# allow for the linker script to use #defines
%.ld: %.ld.in
	@echo "LDS	$@"
	$(CPP) -x assembler-with-cpp -P -D__ASSEMBLY__  -o $@ $<


%.o: %.S
	$(MIPSEL_PREFIX)gcc $(MIPSEL_OPTIONS) -fno-plt -fno-pic -c $<

%.encrypted: %
	../../meta/actions/adfuload encrypt $< > $@

upload_%: %.bin.encrypted
	PYTHONPATH=../../meta/actions/ python3 -m adfu.exe --device 10d6:10d6 --address $(ENTRY_ADDRESS) --binary $<

upload_uart: ENTRY_ADDRESS=0xb4060000
upload_usb: ENTRY_ADDRESS=0xb4061000
upload_exceptions: ENTRY_ADDRESS=0xb4060000

clean:
	rm -f uart.bin.encrypted uart.bin startup_*.o usb.bin.* exceptions.bin* exceptions.ld
