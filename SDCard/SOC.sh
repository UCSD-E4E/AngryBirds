#!/bin/sh
format=1.0
board=am335x_evm

bootloader_location=fatfs_boot
dd_spl_uboot_seek=
dd_spl_uboot_bs=
dd_uboot_seek=
dd_uboot_bs=

conf_bootcmd=bootz
boot_script=
boot_fstype=fat

serial_tty=ttyO0
loadaddr=0x82000000
initrdaddr=0x88080000
zreladdr=0x80008000
fdtaddr=0x88000000
fdtfile=

usbnet_mem=

