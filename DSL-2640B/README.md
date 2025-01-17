# DLS-2640B

Router by D-Link

## UART

 GND RX TX 3v3

 Strangely one you access with the credential from the router
 you are put into a restricted(?) shell

```
  > id
  consoled:error:101.117:processInput:316:unrecognized command id
```

using ``echo /bin/*`` is possible to find that ``/bin/sh`` is available
()

```
 > sh


 BusyBox v1.00 (2011.06.21-10:29+0000) Built-in shell (msh)
 Enter 'help' for a list of built-in commands.

# ls
*             dev           mnt           sys           webs
bin           etc           opt           tmp
clean_svn.sh  lib           proc          usr
data          linuxrc       sbin          var
```

```
# cat /proc/cpuinfo
system type             : AW4337AU
processor               : 0
cpu model               : Broadcom4350 V7.5
BogoMIPS                : 319.48
wait instruction        : yes
microsecond timers      : yes
tlb_entries             : 32
extra interrupt vector  : no
hardware watchpoint     : no
ASEs implemented        :
shadow register sets    : 1
core                    : 0
VCED exceptions         : not available
VCEI exceptions         : not available

unaligned exceptions            : 21
```

```
# ls /dev
ac97            fuse            null            sdb3            sdg4
bcm             hwrandom        p8021ag0        sdb4            sdh
bcm_omci        i2c-0           pktcmf          sdc             sdh1
bcm_ploam       initctl         pmon            sdc1            sdh2
bcm_user_ploam  ippp0           port            sdc2            sdh3
bcmaal20        ippp1           ppp             sdc3            sdh4
bcmadsl0        isdn            printer0        sdc4            si3215
bcmadsl1        isdnctrl        ptm             sdd             slac
bcmatm0         isdnctrl0       pts             sdd1            spu
bcmatmb0        isdninfo        ptyp0           sdd2            tty
bcmendpoint0    kmem            ptyp1           sdd3            tty0
bcmfap          linux-uk-proxy  pwrmngt         sdd4            tty1
bcmles0         linux-user-bde  ram             sde             ttyS0
bcmmoca0        log             ram0            sde1            ttyS1
bcmmoca10       mem             ram1            sde2            ttyUSB0
bcmprof         misc            ram2            sde3            ttyUSB1
bcmvdsl0        mtd0            ram3            sde4            ttyUSB2
bcmvlan         mtd1            random          sdf             ttyp0
bcmxtmcfg0      mtdblock0       sda             sdf1            ttyp1
bounce          mtdblock1       sda1            sdf2            ubi0
brcmboard       mtdblock2       sda2            sdf3            ubi_ctrl
capi20          mtdblock3       sda3            sdf4            urandom
console         mtdblock4       sda4            sdg             zero
dect            mtdblock5       sdb             sdg1
dectdbg         mtdblock6       sdb1            sdg2
fcache          mtdblock7       sdb2            sdg3
```

```
# ps af
  PID  Uid     VmSize Stat Command
    1 admin       412 S   init
    2 admin           SW< [kthreadd]
    3 admin           SW  [sirq-high/0]
    4 admin           SW  [sirq-timer/0]
    5 admin           SW  [sirq-net-tx/0]
    6 admin           SW  [sirq-net-rx/0]
    7 admin           SW  [sirq-block/0]
    8 admin           SW  [sirq-tasklet/0]
    9 admin           SW  [sirq-sched/0]
   10 admin           SW  [sirq-hrtimer/0]
   11 admin           SW  [sirq-rcu/0]
   12 admin           SW< [events/0]
   13 admin           SW< [khelper]
   16 admin           SW< [async/mgr]
   26 admin           SW< [kblockd/0]
   51 admin           SW  [pdflush]
   52 admin           SW  [pdflush]
   53 admin           SWN [kswapd0]
   54 admin           SW< [crypto/0]
   81 admin           SW< [mtdblockd]
  102 admin       484 S   -sh
  135 admin           SW  [dsl0]
  148 admin           SW  [bcmsw]
  205 admin       768 S   smd
  206 admin      1616 S   ssk
  213 admin       388 S   syslogd -n -C -l 7
  214 admin       356 S   klogd -n
  215 admin       400 S   dnsproxy
  216 admin       564 S   dhcpd
  294 admin      1640 S   wlmngr -m 0
  373 admin       656 S   tecommonitor -m 0
  374 admin       284 S   dsldiagd
  375 admin       900 S   linux-user-mdk -m 0
  382 admin       900 S   linux-user-mdk -m 0
  383 admin       900 S   linux-user-mdk -m 0
  450 admin       116 S   /bin/wlevt
 1166 admin       372 S   /bin/bcmupnp -D
 1170 admin       304 S   /bin/lld2d br0
 1176 admin       548 S   /bin/wps_monitor
 1182 admin       760 S   hostapd /var/topology_ap.conf
 1220 admin       836 S   consoled
 1249 admin       412 S   sh -c sh
 1250 admin       456 S   sh
 1301 admin       400 R   ps af
```

Strange stuff happens: ``httpd`` appears only if I do an explicit request
to the device.

Probably the source code for the BSP of something similar to this device is leaked in this
[repo](https://github.com/weihutaisui/BCM).

## Firmware

It is possible to use the script found in the repo [BigNerd95/bcmImageEditor](https://github.com/BigNerd95/bcmImageEditor)
and obtain information about the different parts of the original
firmware

```
$ bcmImageEditor/bcmImageEditor.py info -i DSL-2640B/DSL-2640B_1.00_WI_20110414.bin 
** Broadcom Image info **
Tag Version:        6
Signature 1:        Broadcom Corporatio
Signature 2:        4.06.01.DIE1
Chip ID:            6328
Board ID:           AW4337AU
Big Endian flag:    True
Total Image Length: 4867842 bytes
CFE Address:        0x0
CFE Length:         0 bytes
RootFS Address:     0xbfc10100
RootFS Length:      3964928 bytes
Kernel Address:     0xbffd8100
Kernel Length:      902914 bytes
Image sequence:     
Image version:      
Reserved:           0 not null bytes
Image jamCRC:       0xce6e9d3f
RootFS jamCRC:      0xb45571e0
Kernel jamCRC:      0xca52f9db
Tag jamCRC:         0x756037ba
```

## Emulation

It is possible to run directly the squashfs dumped from mtdblock0 once unpacked

```
$ unsquashfs mtdblock0
$ cp /usr/bin/qemu-mips-static squashfs-root/usr/bin/
$ sudo chroot squashfs-root bin/sh


BusyBox v1.00 (2011.06.21-10:29+0000) Built-in shell (msh)
Enter 'help' for a list of built-in commands.

#
```

but if you want to run via ``qemu``

```
root@debian-mips:/dlink-rootfs# mount --bind /proc /dlink-rootfs/proc/
root@debian-mips:/dlink-rootfs# LD_LIBRARY_PATH=/lib/public/:/lib/private/  chroot . bin/sh


BusyBox v1.00 (2011.06.21-10:29+0000) Built-in shell (msh)
Enter 'help' for a list of built-in commands.

# /bin/httpd                                                                                                                                                                                                       
httpd:error:511.568:oalMsg_init:120:connect to /var/smd_messaging_server_addr failed, rc=-1 errno=146                                                                                                              
httpd:error:511.570:main:257:cmsMsg_init failed, ret=9002
# /bin/smd -v 2
===== Release Version EU_1.01 (build timestamp 2011.06.21-18:29:06) =====

# /bin/smd -v 2

===== Release Version EU_1.01 (build timestamp 2011.06.21-18:29:06) =====

smd:notice:231.497:initUnixDomainServerSocket:1409:smd msg socket opened and ready (fd=3)
smd:debug:231.498:initDls:310:inserting stage 1 entity: ssk (21)
smd:debug:231.498:insertDlsInfoEntry:371:eid=21 (0x15)
smd:notice:231.499:oalSysmon_init:114:Entered
smd:notice:231.499:system_init:230:entered
smd:notice:231.505:system_init:234:done, ret=0
smd:notice:231.505:oal_launchOnBoot:438:stage=1
smd:notice:231.506:sendMessageByState:1436:launching ssk to receive msg 0x10000250
smd:debug:231.506:launchApp:998:spawning /bin/ssk args 
smd:debug:231.507:parseArgs:460:argv[0] = ssk
smd:debug:231.510:launchApp:1016:ssk launched, pid 1662
smd:notice:231.511:smd_init:220:done, ret=0
smd:debug:231.511:oal_processEvents:603:sleeping for 4294967295 ms
smd:debug:231.657:oal_processEvents:639:fd 3 is set
smd:debug:231.658:oal_processEvents:722:accepted new connection from app on fd 5
smd:debug:231.659:oal_processEvents:751:new connection from src=0x15
smd:debug:231.660:oal_processEvents:760:got APP_LAUNCHED from ssk (eid=21, pid=1662) on fd 5
smd:debug:231.661:processLaunchConfirmation:796:ssk (eid=21) transitioning to state=2
smd:debug:231.661:processLaunchConfirmation:806:sending queued msg 0x10000250
smd:debug:231.665:oal_processEvents:603:sleeping for 4294967295 ms
ssk:error:231.767:devCtl_boardIoctl:230:Unable to open device /dev/brcmboard
ssk:error:231.768:cmsImg_getRealConfigFlashSize:326:boardIoctl to get config flash size failed.
ssk:error:231.769:ssk_init:592:cmsMdm_init failed, ret=9002
ssk:error:231.769:main:166:initialization failed (9002), exit.
smd:debug:231.772:oal_processEvents:639:fd 5 is set
smd:notice:231.772:oal_processEvents:678:detected message on fd 5 from ssk
smd:notice:231.773:oal_processEvents:688:detected exit of ssk (pid=1662) on fd 5
smd:debug:231.815:collectApp:1179:collected ssk (pid 1662) signalNum=0
smd:debug:231.816:collectApp:1230:unlink and free dInfo structure at 0x41f034 for ssk eid=21 (0x15)
smd:debug:231.817:interest_unregisterAll:198:eid=21 (0x15)
smd:error:231.818:oal_processEvents:692:ssk has died.  smd must exit now!
smd:notice:231.819:main:149:exiting with code 0
smd:notice:231.821:cmsMdm_cleanup:349:entered
smd:error:231.822:oalShm_cleanup:304:shmctl IPC_STAT failed
smd:notice:231.823:cmsMdm_cleanup:357:done
```

## CFE web update

If you press reset for ten seconds you can obtain the following
log from the serial:

```
*** Restore to Factory Default Setting ***


*** Break into CFE console ***

Board IP address                  : 192.168.1.1:ffffff00
Host IP address                   : 192.168.1.100
Gateway IP address                :
Run from flash/host (f/h)         : f
Default host run file name        : vmlinux
Default host flash file name      : bcm963xx_fs_kernel
Boot delay (0-9 seconds)          : 1
Board Id (0-5)                    : AW4337AU
Number of MAC Addresses (1-32)    : 11
Base MAC Address                  : b8:a3:86:ea:1a:96
PSI Size (1-128) KBytes            : 128
Enable Backup PSI [0|1]           : 0
System Log Size (0-256) KBytes    : 0
Main Thread Number [0|1]          : 0

web info: Waiting for connection on socket 0.
CFE> 
```
