# Nordic Thingy:52 SDK

Welcome to the Nordic Thingy:52 software development kit.
This kit is designed to assist users in developing their own custom firmware for Thingy.
Please see http://www.nordicsemi.com/thingy for the latest news and software releases.

## Prerequisites

The simplest install is achieved by using one of the automated options below.
Before running the scripts, make sure you have the following software installed:
1. Python v2.7.xx or v3.xx.xx avaialble from https://www.python.org/. During the installation there will be a "Customize Python" window. Here, scroll to the bottom and select "Add python.exe to path." Otherwise, use default configurations.
2. Git v2.xx.xx, Available from https://git-scm.com/. Use default configurations.
3. Install GNU ARM embedded toolchain v4.9-2015q3. Avaialble from https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update. Use default configurations.
4. Make must be installed and be in system path.

## Setting up repository
### Windows
1. Run `setup_repo.bat`
2. Go to `project/pca20020_s132/armgcc/`
3. Run `make -j`

### Linux/Mac
1. Run `setup_repo.sh`
2. Go to `project/pca20020_s132/armgcc/`
3. Run `make -j`
