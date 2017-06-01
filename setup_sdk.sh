#!/bin/sh

python setup_sdk.py

rm -rf sdk_components
ln -s external/sdk/components sdk_components
rm -rf external/nano-pb
ln -s ../external/sdk/external/nano-pb external/nano-pb
rm -rf external/segger_rtt
ln -s ../external/sdk/external/segger_rtt external/segger_rtt
rm -rf external/micro-ecc
ln -s ../external/sdk/external/micro-ecc/micro-ecc external/micro-ecc

