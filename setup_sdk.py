from __future__ import ( division, absolute_import, print_function, unicode_literals )
import sys
import os
import zipfile 
import logging
import tempfile
import subprocess
import shutil
from distutils import spawn

if sys.version_info >= (3,):
    import urllib.request as urllib
else:
    import urllib

logging.basicConfig()
log = logging.getLogger("setup")
log.setLevel(logging.INFO)

SDK_PATH = "http://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v12.x.x/"
SDK_FILE = "nRF5_SDK_12.1.0_0d23e2a.zip"
SDK_ZIP = SDK_PATH + SDK_FILE
BUILD_DIR = '_build'
MICRO_ECC = "https://github.com/kmackay/micro-ecc/archive/master.zip"    

def _reporthook(blocknum, blocksize, totalsize):
    readsofar = blocknum * blocksize
    if totalsize > 0:
        percent = readsofar * 1e2 / totalsize
        s = "\rDownload progress: %5.1f%% %*d / %d" % (
            percent, len(str(totalsize)), readsofar, totalsize)
        sys.stdout.write(s)
        if readsofar >= totalsize: # near the end
            sys.stdout.write("  OK\n")
    else: # total size is unknown
        sys.stderr.write("read %d\n" % (readsofar,))    

def download_file(id, src, dst):
    if os.path.exists(dst):
        log.info("Found {} in _build/".format(id))
    else:
        log.info("Downloading {}...".format(id))
        urllib.urlretrieve(src, dst, _reporthook)
        
if __name__ == '__main__':
    if sys.platform.startswith("linux"):
       patch_command = "patch"  # Expected to be in path
       gcc_toolchain = os.path.isfile("/usr/local/gcc-arm-none-eabi-4_9-2015q3/bin/arm-none-eabi-gcc")
    elif sys.platform == "darwin":
       patch_command = "patch"  # Expected to be in path
       gcc_toolchain = os.path.isfile("/usr/local/gcc-arm-none-eabi-4_9-2015q3/bin/arm-none-eabi-gcc")
    elif sys.platform == "win32":
       patch_command = "C:/Program Files/Git/usr/bin/patch.exe"
       gcc_toolchain = os.path.isfile("C:/Program Files (x86)/GNU Tools ARM Embedded/4.9 2015q3/bin/arm-none-eabi-gcc.exe")
    if not spawn.find_executable('make'):
        gcc_toolchain = False    

    try:
        shutil.rmtree("external/sdk")
    except OSError:
        pass
    try:
        os.mkdir(BUILD_DIR)
    except OSError:
        pass
        
    download_file('SDK zip', SDK_ZIP, BUILD_DIR + "/sdk.zip")
    log.info("Extracting SDK")
    with zipfile.ZipFile("_build/sdk.zip", 'r') as f:
        for i in f.namelist():
            if i.startswith(('external', 'components')):
                f.extract(i, "external/sdk")
    
    log.info("Patching SDK")
    cmd = [patch_command, '-p1', '--binary', '--ignore-whitespace', '-i', "../sdk_12.1.diff"]
    subprocess.call(cmd, cwd="external/sdk")
    
    download_file('micro-ecc zip', MICRO_ECC, "_build/micro_ecc.zip")
    log.info("Extracting micro-ecc")

    with zipfile.ZipFile("_build/micro_ecc.zip", 'r') as f:
        for i in f.namelist():
            f.extract(i, "external/sdk/external/micro-ecc")
    os.rename("external/sdk/external/micro-ecc/micro-ecc-master", "external/sdk/external/micro-ecc/micro-ecc")
    log.info("Compiling micro-ecc library")
    if not gcc_toolchain:
        log.warning("GCC toolchain not found skipping. Bootloader will not compile.\n")
        sys.exit(0)
    cmd = ['make', '-j']
    working_dir = 'external/sdk/external/micro-ecc/nrf52_keil/armgcc'
    proc = subprocess.Popen(cmd, cwd=working_dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = proc.communicate()
    if proc.returncode != 0:
        raise RuntimeError("Toolchain error:\nRunning make in {} exited with non-zero returncode. Log:\n{}".format(working_dir, stdout))
    shutil.copyfile('external/sdk/external/micro-ecc/nrf52_keil/armgcc/micro_ecc_lib_nrf52.lib', 'external/micro_ecc_lib_nrf52.lib')
    