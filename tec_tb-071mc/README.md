# TEC TB-071MC

This is a tablet based on a **rockchip** [SOC](http://www.linux-rockchip.info/mw/index.php?title=Category:List_of_Rockchip_SoCs).

It's possible to use [rkflashtool](https://github.com/neo-technologies/rkflashtool)
in order to flash this table once you have the *recovery mode*:
you have to press ``Vol+`` button, insert the usb connector, wait an usb device to appear
into the computer log and then release the button.

Then we obtain the chip type: **RK2928**

```
$. /rkflashtool v
rkflashtool: info: rkflashtool v5.2
rkflashtool: info: Detected RK2928...
rkflashtool: info: interface claimed
rkflashtool: info: chip version: 292X-2012.08.23-V300
```

with 4GB of flash

```
$ ./rkflashtool n
rkflashtool: info: rkflashtool v5.2
rkflashtool: info: Detected RK2928...
rkflashtool: info: interface claimed
rkflashtool: info: Flash ID: 2c 68 04 4a a9
rkflashtool: info: Flash Info:
	Manufacturer: Micron (4)
	Flash Size: 4096MB
	Block Size: 1024KB
	Page Size: 4KB
	ECC Bits: 24
	Access Time: 32
	Flash CS: <0>
```

The boot parameters

```
./rkflashtool p
rkflashtool: info: rkflashtool v5.2
rkflashtool: info: Detected RK2928...
rkflashtool: info: interface claimed
rkflashtool: info: reading parameters at offset 0x00000000
rkflashtool: info: size:  0x0000025d
FIRMWARE_VER:4.1.1
MACHINE_MODEL:TB-071MC
MACHINE_ID:007
MANUFACTURER:RK2928SDK
MAGIC: 0x5041524B
ATAG: 0x60000800
MACHINE: 2928
CHECK_MASK: 0x80
KERNEL_IMG: 0x60408000
#RECOVER_KEY: 1,1,0,20,0
CMDLINE:console=ttyFIQ0 androidboot.console=ttyFIQ0 init=/init initrd=0x62000000,0x00800000 mtdparts=rk29xxnand:0x00002000@0x00002000(misc),0x00004000@0x00004000(kernel),0x00008000@0x00008000(boot),0x00010000@0x00010000(recovery),0x00020000@0x00020000(backup),0x00040000@0x00040000(cache),0x00200000@0x00080000(userdata),0x00002000@0x00280000(kpanic),0x00130000@0x00282000(system),-@0x003b2000(user)
```

shows all the partitions present on the flash; dumping the system partition

```
$ ./rkflashtool r system > system.img
rkflashtool: info: rkflashtool v5.2
rkflashtool: info: Detected RK2928...
rkflashtool: info: interface claimed
rkflashtool: info: working with partition: system
rkflashtool: info: found offset: 0x00282000
rkflashtool: info: found size: 0x00130000
rkflashtool: info: reading flash memory at offset 0x003b1fe0... Done!
$ file /opt/teardown/tec_tb-071mc/system.img
system.img: Linux rev 1.0 ext3 filesystem data, UUID=bb9e0a8d-c7b7-4c9a-81f9-8021a9d7c625, volume name "system"
```

gives us an ``ext3`` filesystem.

## Partitions

```
$ file *.img
backup.img:   data
boot.img:     data
kernel.img:   data
misc.img:     data
recovery.img: Android bootimg, kernel (0x60408000), ramdisk (0x62000000), page size: 16384
system.img:   Linux rev 1.0 ext3 filesystem data, UUID=bb9e0a8d-c7b7-4c9a-81f9-8021a9d7c625, volume name "system"
userdata.img: Linux rev 1.0 ext4 filesystem data, UUID=413b91a2-7c6c-4395-87d3-0af6f2561341 (extents) (large files)
user.img:     DOS/MBR boot sector, code offset 0x58+2, OEM-ID "NetBSD  ", sectors/cluster 16, sectors/track 57344, sectors 3923968 (volumes > 32 MB) , FAT (32 bit), sectors/FAT 1920, serial number 0x36560cf4, label: "ROCKCHIPS  "
```

Looking at the first bytes of the images we obtain the following:

```
head -c 10 tec_tb-071mc/*.img
==> tec_tb-071mc/backup.img <==
RKAFPrLT
==> tec_tb-071mc/boot.img <==
KRNL}�
==> tec_tb-071mc/kernel.img <==
KRNL$Pn��
==> tec_tb-071mc/misc.img <==

==> tec_tb-071mc/recovery.img <==
ANDROID!$P
==> tec_tb-071mc/system.img <==

==> tec_tb-071mc/userdata.img <==

==> tec_tb-071mc/user.img <==
```

Seems that ``KRNL`` is an image packed using ``rkcrc``, you can unsign with ``rkunsign``; after that
``boot.img`` is seen as ``gzip compressed data, from Unix``

```
$ rkunsign tec_tb-071mc/boot.img tec_tb-071mc/boot_unsigned.img
$ zcat tec_tb-071mc/boot_unsigned.img > tec_tb-071mc/boot_unsigned_decompress.img
$ cpio -t < tec_tb-071mc/boot_unsigned_decompress.img
charger
data
default.prop
dev
init
init.goldfish.rc
init.rc
init.rk2928board.rc
init.rk2928board.usb.rc
init.trace.rc
init.usb.rc
proc
res
res/images
res/images/charger
res/images/charger/battery_0.png
res/images/charger/battery_1.png
res/images/charger/battery_2.png
res/images/charger/battery_3.png
res/images/charger/battery_4.png
res/images/charger/battery_5.png
res/images/charger/battery_charge.png
res/images/charger/battery_fail.png
rk292xnand_ko.ko
sbin
sbin/adbd
sbin/e2fsck
sbin/mkdosfs
sbin/mke2fs
sbin/pcba_core
sbin/resize2fs
sbin/ueventd
sys
system
ueventd.goldfish.rc
ueventd.rc
ueventd.rk2928board.rc
3645 blocks
```

Instead ``kernel`` is not recognized

Looking inside the ``system`` image we see a pretty standard android filesystem:

```
$ sudo kpartx -av tec_tb-071mc/system.img
$ sudo mount /dev/loop2 /tmp/rootfs
$ ls /tmp/rootfs
app  bin  build.prop  etc  fonts  framework  lib  lost+found  media  tts  usr  vendor  xbin
```

Seems not present a kernel module to load for the ``NAND`` memory like described [here](www.linux-rockchip.info/mw/index.php?title=Boot_Sequences)

```
$ find /tmp/rootfs -name '*.ko'
/tmp/rootfs/lib/modules/8188eu.ko
/tmp/rootfs/lib/modules/mali.ko
/tmp/rootfs/lib/modules/vpu_service.ko
/tmp/rootfs/lib/modules/8192cu.ko
/tmp/rootfs/lib/modules/rt5370ap.ko
/tmp/rootfs/lib/modules/rt5370sta.ko
/tmp/rootfs/lib/modules/ump.ko
/tmp/rootfs/lib/modules/wlan.ko
/tmp/rootfs/lib/modules/rkwifi.ko
```

## Android

Accessing the device after a fabric reset

```
root@android:/ # mount
rootfs / rootfs ro,relatime 0 0
tmpfs /dev tmpfs rw,nosuid,relatime,mode=755 0 0
devpts /dev/pts devpts rw,relatime,mode=600 0 0
proc /proc proc rw,relatime 0 0
sysfs /sys sysfs rw,relatime 0 0
none /acct cgroup rw,relatime,cpuacct 0 0
tmpfs /mnt/asec tmpfs rw,relatime,mode=755,gid=1000 0 0
tmpfs /mnt/obb tmpfs rw,relatime,mode=755,gid=1000 0 0
none /dev/cpuctl cgroup rw,relatime,cpu 0 0
/dev/block/mtdblock8 /system ext4 ro,noatime,nodiratime,barrier=1,data=ordered,noauto_da_alloc 0 0
/dev/block/mtdblock6 /data ext4 rw,nosuid,nodev,noatime,nodiratime,barrier=1,data=ordered,noauto_da_alloc 0 0
/dev/block/mtdblock5 /cache ext4 rw,nosuid,nodev,noatime,nodiratime,barrier=1,data=ordered,noauto_da_alloc 0 0
/sys/kernel/debug /sys/kernel/debug debugfs rw,relatime 0 0
/dev/block/vold/31:9 /mnt/sdcard vfat rw,dirsync,nosuid,nodev,noexec,noatime,nodiratime,uid=1000,gid=1015,fmask=0002,dmask=0002,allow_utime=0020,codepage=cp437,iocharset=iso8859-1,shortname=mixed,utf8,errors=remount-ro 0 0
/dev/block/vold/31:9 /mnt/secure/asec vfat rw,dirsync,nosuid,nodev,noexec,noatime,nodiratime,uid=1000,gid=1015,fmask=0002,dmask=0002,allow_utime=0020,codepage=cp437,iocharset=iso8859-1,shortname=mixed,utf8,errors=remount-ro 0 0
tmpfs /mnt/sdcard/.android_secure tmpfs ro,relatime,size=0k,mode=000 0 0
```

```
root@android:/ # cat /proc/cpuinfo                                             
Processor      : ARMv7 Processor rev 0 (v7l)
BogoMIPS       : 431.85
Features       : swp half thumb fastmult vfp edsp neon vfpv3 
CPU implementer : 0x41
CPU architecture: 7
CPU variant    : 0x3
CPU part       : 0xc09
CPU revision   : 0

Hardware       : RK2928board
Revision       : 0000
Serial         : 0000000000000000
```

## Serial

We need to find the serial: some info from [here](https://github.com/minghuascode/qemu120/blob/master/xternapp/tablet-rkflashtool/README-rk2926-2928-debug-serial.txt)

At the end we found the serial via the ``USB`` connector, it's 115200 baud and the boot
log is [here](boot.txt). Also is accessible a shell, that is deactivate
when the screen is put to sleep.

This part below we don't know if it's of interest.
```
http://wenku.baidu.com/view/431560365727a5e9856a6167.html

RK2928 and RK2926 Debug Serial Port Connection
(RK2928&RK2926调试串口连接说明)

On RK2928 and 2926 the OTG and debug serial port are connected 
to the same set of pins by an internal IO_SWITCH. On RK2926 it is 
ping 64 65, RK2928 is V8 W8. Default the two pins are serial mode. 
When VBUS signal is detected it is switched to USB_DEVICE mode. 
When ID is low, it is switched to USB_HOST mode. 

Serial debug connection: 
      RK2926/2928 OTG_DP  --------  RXD serial board
      RK2926/2928 OTG_DN  --------  TXD serial board
      RK2926/2928 GND     --------  GND serial board
```

With the **Bus Pirate** we have to enable the pullup resistor with ``P``

```
Select output type:
 1. Open drain (H=Hi-Z, L=GND)
 2. Normal (H=3.3V, L=GND)

(1)>2
Ready
UART>P
WARNING: pins not open drain (HiZ)
Pull-up resistors ON
--- Settings: /dev/ttyUSB0  115200,8,N,1
--- RTS: active    DTR: active    BREAK: inactive
--- CTS: inactive  DSR: inactive  RI: inactive  CD: inactive
--- software flow control: inactive
--- hardware flow control: inactive
--- serial input encoding: UTF-8
--- serial output encoding: UTF-8
--- EOL: LF
--- filters: default
```

### FIQ

**this section has to be confirmed**

Seems possible to activate the ``FIQ`` debugger using a resistor of 150k between the ``ID`` and
``GND`` pin; the only thing strange is that the ``RX`` pin of the USB connector must be not connected
otherwise the boot stops. With trial and error we found that we have to wait until
the boot is completed and then connect that pin.

```
debug> fiq debugger mode
debug> help
FIQ Debugger commands:
 pc            PC status
 regs          Register dump
 allregs       Extended Register dump
 bt            Stack trace
 reboot        Reboot
 irqs          Interupt status
 kmsg          Kernel log
 version       Kernel version
 last_kmsg     Last kernel log
 sleep         Allow sleep while in FIQ
 nosleep       Disable sleep while in FIQ
 console       Switch terminal to console
 cpu           Current CPU
 cpu <number>  Switch to CPU<number>
 ps            Process list
 sysrq         sysrq options
 sysrq <param> Execute sysrq with <param>
```
More info about FIQ can be found [here](http://www.linaro.org/blog/core-dump/debugging-arm-kernels-using-nmifiq/).
