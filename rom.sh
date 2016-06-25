#!/usr/bin/env sh

image=$1

sparc-elf-objcopy -O binary -j .text ${image} ${image}_text.bin &&
sparc-elf-objcopy -O binary -j .data ${image} ${image}_data.bin &&
sparc-elf-objdump -d    ${image} > ${image}.asm &&
sparc-elf-objdump -g -t ${image} > ${image}.dbg