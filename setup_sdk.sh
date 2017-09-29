#!/bin/bash
rm -rf sdk_components
ln -s external/sdk13/components sdk_components
rm -rf external/nano-pb
ln -s ../external/sdk13/external/nano-pb external/nano-pb
rm -rf external/segger_rtt
ln -s ../external/sdk13/external/segger_rtt external/segger_rtt

if ! [ -x "$(command -v git)" ]; then
  echo 'git is not installed. Please install and append to PATH' >&2
  exit
fi

if [ ! -f external/sdk13/external/micro-ecc/micro-ecc/uECC.c ]; then
    echo "micro-ecc not found! Let's pull it from HEAD."
	git clone https://github.com/kmackay/micro-ecc.git	external/sdk13/external/micro-ecc/micro-ecc
fi

pushd external/sdk13/external/micro-ecc/nrf52_armgcc/armgcc && make && popd &&
pushd external/sdk13/external/micro-ecc/nrf52_keil/armgcc && make && popd
