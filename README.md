# Nordic Thingy:52 SDK

Welcome to the Nordic Thingy:52 software development kit.
This kit is designed to assist users in developing their own custom firmware for Thingy.
Please see http://www.nordicsemi.com/thingy for the latest news and software releases.

Consult the [firmware documentation](https://nordicsemiconductor.github.io/Nordic-Thingy52-FW/documentation/index.html) for more details.

## Prerequisites

Before running the scripts below, make sure you have the following software installed:
1. Python v2.7.xx or v3.xx.xx available from https://www.python.org/. During the installation  there will be a "Customize Python" window. Here, scroll to the bottom and select "Add python.exe to path." Otherwise, use default configurations.
2. Git v2.xx.xx, Available from https://git-scm.com/. Use default configurations.
3. Install GNU ARM embedded toolchain v4.9-2015q3. Available from https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update. Use default configurations.
4. Make must be installed and be in system path. For example http://gnuwin32.sourceforge.net/packages/make.htm.
5. Create a user at https://www.invensense.com/. Under "Downloads" download "Embedded MotionDriver 6.12". Unzip the downloaded `motion_driver_6.12` folder and navigate to `motion_driver_6.12/mpl libraries/arm/Keil`.
Unzip the folder `libmpllib_Keil_M4FP.zip`. Copy the extracted library `libmpllib.lib` into `<your Thingy folder>/libs/libmpllib_Keil_M4FP/`. Finally, unzip `/motion_driver_6.12/mpl libraries/arm/gcc4.9.3/liblibmplmpu_m4_hardfp.zip` and copy the extracted library `liblibmplmpu.a` into the folder `<your Thingy folder>/libs/liblibmplmpu_m4_hardfp/`.

## Setting up the SDK
Run `setup_sdk.bat` on Windows or `setup_sdk.sh` on Linux/Mac.

These scripts will download and unzip the [Nordic nRF5 SDK](http://developer.nordicsemi.com/nRF5_SDK/), download and compile the [micro-ecc](https://github.com/kmackay/micro-ecc/archive/master.zip) library and create the required folder structure.

## Compiling the code
To compile the code, please consult the compiling new [firmware page](https://nordicsemiconductor.github.io/Nordic-Thingy52-FW/documentation/firmware_compile.html) in the [firmware documentation](https://nordicsemiconductor.github.io/Nordic-Thingy52-FW/documentation/index.html).
