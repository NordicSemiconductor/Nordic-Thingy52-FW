PROJECT_NAME := ble_app_thingy_s132_pca20020

export OUTPUT_FILENAME
#MAKEFILE_NAME := $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) )

TEMPLATE_PATH = ../../../sdk_components/toolchain/gcc
ifeq ($(OS),Windows_NT)
include $(TEMPLATE_PATH)/Makefile.windows
else
include $(TEMPLATE_PATH)/Makefile.posix
endif

MK := mkdir
RM := rm -rf

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO :=
else
NO_ECHO := @
endif

# Toolchain commands
CC              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc'
AS              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as'
AR              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar' -r
LD              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld'
NM              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm'
OBJDUMP         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump'
OBJCOPY         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy'
SIZE            := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size'

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

LIBS += \
$(abspath ../../../libs/liblibmplmpu_m4_hardfp/liblibmplmpu.a) \
$(abspath ../../../sdk_components/toolchain/cmsis/dsp/GCC/libarm_cortexM4lf_math.a) \
$(abspath ../../../sdk_components/nfc/t2t_lib/nfc_t2t_lib_gcc.a) \

#source common to all targets
C_SOURCE_FILES += \
$(abspath ../main.c) \
$(abspath ../../../source/modules/m_ble.c) \
$(abspath ../../../source/modules/m_ble_flash.c) \
$(abspath ../../../source/util/advertiser_beacon_timeslot.c) \
$(abspath ../../../source/util/twi_manager.c) \
$(abspath ../../../source/util/support_func.c) \
$(abspath ../../../source/modules/m_environment.c) \
$(abspath ../../../source/modules/m_environment_flash.c) \
$(abspath ../../../source/modules/m_sound.c) \
$(abspath ../../../source/modules/m_motion.c) \
$(abspath ../../../source/modules/m_motion_flash.c) \
$(abspath ../../../source/modules/m_ui.c) \
$(abspath ../../../source/modules/m_ui_flash.c) \
$(abspath ../../../source/modules/m_batt_meas.c) \
$(abspath ../../../source/drivers/drv_gas_sensor.c) \
$(abspath ../../../source/drivers/drv_ccs811.c) \
$(abspath ../../../source/drivers/drv_pressure.c) \
$(abspath ../../../source/drivers/drv_lps22hb.c) \
$(abspath ../../../source/drivers/drv_humidity.c) \
$(abspath ../../../source/drivers/drv_hts221.c) \
$(abspath ../../../source/drivers/drv_ext_light.c) \
$(abspath ../../../source/drivers/drv_ext_gpio.c) \
$(abspath ../../../source/drivers/drv_nfc.c) \
$(abspath ../../../source/util/sx150x_led_drv_calc.c) \
$(abspath ../../../source/drivers/drv_sx1509.c) \
$(abspath ../../../source/drivers/drv_bh1745.c) \
$(abspath ../../../source/drivers/drv_color.c) \
$(abspath ../../../source/drivers/drv_speaker.c) \
$(abspath ../../../source/drivers/drv_mpu9250.c) \
$(abspath ../../../source/drivers/drv_motion.c) \
$(abspath ../../../source/drivers/drv_acc_lis3dh.c) \
$(abspath ../../../source/drivers/drv_mic.c) \
$(abspath ../../../libs/eMD6/core/driver/eMPL/inv_mpu.c) \
$(abspath ../../../libs/eMD6/core/driver/eMPL/inv_mpu_dmp_motion_driver.c) \
$(abspath ../../../libs/eMD6/core/mllite/data_builder.c) \
$(abspath ../../../libs/eMD6/core/mllite/hal_outputs.c) \
$(abspath ../../../libs/eMD6/core/mllite/message_layer.c) \
$(abspath ../../../libs/eMD6/core/mllite/ml_math_func.c) \
$(abspath ../../../libs/eMD6/core/mllite/mlmath.c) \
$(abspath ../../../libs/eMD6/core/mllite/mpl.c) \
$(abspath ../../../libs/eMD6/core/mllite/results_holder.c) \
$(abspath ../../../libs/eMD6/core/mllite/start_manager.c) \
$(abspath ../../../libs/eMD6/core/mllite/storage_manager.c) \
$(abspath ../../../libs/eMD6/core/eMPL-hal/eMPL_outputs.c) \
$(abspath ../../../libs/eMD6/core/driver/nRF52/log_nrf52.c) \
$(abspath ../../../libs/dvi_adpcm/dvi_adpcm.c) \
$(abspath ../../../libs/sr3_audio/drv_audio_coder_adpcm.c) \
$(abspath ../../../libs/sr3_audio/drv_audio_coder_bv32fp.c) \
$(abspath ../../../libs/sr3_audio/drv_audio_dsp.c) \
$(abspath ../../../libs/sr3_audio/drv_audio_pdm.c) \
$(abspath ../../../sdk_components/ble/common/ble_advdata.c) \
$(abspath ../../../sdk_components/ble/ble_advertising/ble_advertising.c) \
$(abspath ../../../sdk_components/ble/common/ble_conn_params.c) \
$(abspath ../../../sdk_components/ble/common/ble_srv_common.c) \
$(abspath ../../../sdk_components/ble/common/ble_conn_state.c) \
$(abspath ../../../source/ble_services/ble_tes.c) \
$(abspath ../../../source/ble_services/ble_tcs.c) \
$(abspath ../../../source/ble_services/ble_uis.c) \
$(abspath ../../../source/ble_services/ble_tms.c) \
$(abspath ../../../source/ble_services/ble_tss.c) \
$(abspath ../../../sdk_components/drivers_nrf/common/nrf_drv_common.c) \
$(abspath ../../../sdk_components/drivers_nrf/gpiote/nrf_drv_gpiote.c) \
$(abspath ../../../sdk_components/drivers_nrf/twi_master/nrf_drv_twi.c) \
$(abspath ../../../sdk_components/drivers_nrf/pwm/nrf_drv_pwm.c) \
$(abspath ../../../sdk_components/drivers_nrf/pdm/nrf_drv_pdm.c) \
$(abspath ../../../sdk_components/drivers_nrf/clock/nrf_drv_clock.c) \
$(abspath ../../../sdk_components/drivers_nrf/saadc/nrf_drv_saadc.c) \
$(abspath ../../../sdk_components/drivers_nrf/hal/nrf_saadc.c) \
$(abspath ../../../sdk_components/drivers_nrf/rng/nrf_drv_rng.c) \
$(abspath ../../../sdk_components/nfc/ndef/launchapp/nfc_launchapp_msg.c) \
$(abspath ../../../sdk_components/nfc/ndef/launchapp/nfc_launchapp_rec.c) \
$(abspath ../../../sdk_components/nfc/ndef/uri/nfc_uri_msg.c) \
$(abspath ../../../sdk_components/nfc/ndef/uri/nfc_uri_rec.c) \
$(abspath ../../../sdk_components/nfc/ndef/generic/message/nfc_ndef_msg.c) \
$(abspath ../../../sdk_components/nfc/ndef/generic/record/nfc_ndef_record.c) \
$(abspath ../../../sdk_components/nfc/ndef/text/nfc_text_rec.c) \
$(abspath ../../../sdk_components/nfc/t2t_lib/hal_t2t/hal_nfc_t2t.c) \
$(abspath ../../../sdk_components/libraries/button/app_button.c) \
$(abspath ../../../sdk_components/libraries/util/app_error.c) \
$(abspath ../../../sdk_components/libraries/scheduler/app_scheduler.c) \
$(abspath ../../../sdk_components/libraries/timer/app_timer.c) \
$(abspath ../../../sdk_components/libraries/util/nrf_assert.c) \
$(abspath ../../../sdk_components/libraries/uart/retarget.c) \
$(abspath ../../../sdk_components/libraries/led_softblink/led_softblink.c) \
$(abspath ../../../sdk_components/libraries/low_power_pwm/low_power_pwm.c) \
$(abspath ../../../sdk_components/libraries/fstorage/fstorage.c) \
$(abspath ../../../sdk_components/libraries/fds/fds.c) \
$(abspath ../../../sdk_components/libraries/util/app_util_platform.c) \
$(abspath ../../../sdk_components/libraries/util/sdk_mapped_flags.c) \
$(abspath ../../../sdk_components/libraries/queue/nrf_queue.c) \
$(abspath ../../../external/segger_rtt/RTT_Syscalls_GCC.c) \
$(abspath ../../../external/segger_rtt/SEGGER_RTT.c) \
$(abspath ../../../external/segger_rtt/SEGGER_RTT_printf.c) \
$(abspath ../../../external/sdk13_override/softdevice_handler_appsh.c) \
$(abspath ../../../sdk_components/softdevice/common/softdevice_handler/softdevice_handler.c) \
$(abspath ../../../sdk_components/toolchain/system_nrf52.c) \
$(abspath ../../../sdk_components/libraries/log/src/nrf_log_frontend.c) \
$(abspath ../../../sdk_components/libraries/log/src/nrf_log_backend_serial.c) \
$(abspath ../../../source/util/nrf_dfu_flash_buttonless.c) \
$(abspath ../../../sdk_components/ble/ble_services/ble_dfu/ble_dfu.c) \
$(abspath ../../../sdk_components/ble/ble_services/ble_bas/ble_bas.c) \
$(abspath ../../../sdk_components/libraries/bootloader/dfu/nrf_dfu_settings.c) \
$(abspath ../../../sdk_components/libraries/crc32/crc32.c) \
$(abspath ../../../sdk_components/libraries/strerror/nrf_strerror.c) \
#assembly files common to all targets
ASM_SOURCE_FILES  = $(abspath ../../../sdk_components/toolchain/gcc/gcc_startup_nrf52.S)

#includes common to all targets
INC_PATHS += -I$(abspath ../config)
INC_PATHS += -I$(abspath ../../../external/segger_rtt)
INC_PATHS += -I$(abspath ../../../include/ble_services)
INC_PATHS += -I$(abspath ../../../include/board)
INC_PATHS += -I$(abspath ../../../include/drivers)
INC_PATHS += -I$(abspath ../../../include/macros)
INC_PATHS += -I$(abspath ../../../include/modules)
INC_PATHS += -I$(abspath ../../../include/util)
INC_PATHS += -I$(abspath ../../../libs/bv32fp-1.2)
INC_PATHS += -I$(abspath ../../../libs/dvi_adpcm)
INC_PATHS += -I$(abspath ../../../libs/eMD6/core/driver/eMPL)
INC_PATHS += -I$(abspath ../../../libs/eMD6/core/driver/include)
INC_PATHS += -I$(abspath ../../../libs/eMD6/core/driver/nRF52)
INC_PATHS += -I$(abspath ../../../libs/eMD6/core/eMPL-hal)
INC_PATHS += -I$(abspath ../../../libs/eMD6/core/mllite)
INC_PATHS += -I$(abspath ../../../libs/eMD6/core/mpl)
INC_PATHS += -I$(abspath ../../../libs/sr3_audio)
INC_PATHS += -I$(abspath ../../../sdk_components/ble/ble_advertising)
INC_PATHS += -I$(abspath ../../../sdk_components/ble/ble_services/ble_bas)
INC_PATHS += -I$(abspath ../../../sdk_components/ble/ble_services/ble_dfu)
INC_PATHS += -I$(abspath ../../../sdk_components/ble/common)
INC_PATHS += -I$(abspath ../../../sdk_components/device)
INC_PATHS += -I$(abspath ../../../sdk_components/drivers_nrf/clock)
INC_PATHS += -I$(abspath ../../../sdk_components/drivers_nrf/common)
INC_PATHS += -I$(abspath ../../../sdk_components/drivers_nrf/config)
INC_PATHS += -I$(abspath ../../../sdk_components/drivers_nrf/delay)
INC_PATHS += -I$(abspath ../../../sdk_components/drivers_nrf/gpiote)
INC_PATHS += -I$(abspath ../../../sdk_components/drivers_nrf/hal)
INC_PATHS += -I$(abspath ../../../sdk_components/drivers_nrf/pdm)
INC_PATHS += -I$(abspath ../../../sdk_components/drivers_nrf/pstorage)
INC_PATHS += -I$(abspath ../../../sdk_components/drivers_nrf/pwm)
INC_PATHS += -I$(abspath ../../../sdk_components/drivers_nrf/rng)
INC_PATHS += -I$(abspath ../../../sdk_components/drivers_nrf/saadc)
INC_PATHS += -I$(abspath ../../../sdk_components/drivers_nrf/twi_master)
INC_PATHS += -I$(abspath ../../../sdk_components/drivers_nrf/uart)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/bootloader/dfu)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/button)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/crc32)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/experimental_section_vars)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/experimental_section_vars)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/fds)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/fifo)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/fstorage)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/fstorage)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/led_softblink)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/log)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/log/src)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/low_power_pwm)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/scheduler)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/strerror)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/timer)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/uart)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/util)
INC_PATHS += -I$(abspath ../../../sdk_components/libraries/queue)
INC_PATHS += -I$(abspath ../../../sdk_components/nfc/ndef/generic/record)
INC_PATHS += -I$(abspath ../../../sdk_components/nfc/ndef/generic/message)
INC_PATHS += -I$(abspath ../../../sdk_components/nfc/ndef/launchapp)
INC_PATHS += -I$(abspath ../../../sdk_components/nfc/ndef/uri)
INC_PATHS += -I$(abspath ../../../sdk_components/nfc/ndef/text)
INC_PATHS += -I$(abspath ../../../sdk_components/nfc/t2t_lib)
INC_PATHS += -I$(abspath ../../../sdk_components/nfc/ndef/text)
INC_PATHS += -I$(abspath ../../../sdk_components/softdevice/common/softdevice_handler)
INC_PATHS += -I$(abspath ../../../sdk_components/softdevice/s132/headers)
INC_PATHS += -I$(abspath ../../../sdk_components/softdevice/s132/headers/nrf52)
INC_PATHS += -I$(abspath ../../../sdk_components/toolchain)
INC_PATHS += -I$(abspath ../../../sdk_components/toolchain/cmsis/include)
INC_PATHS += -I$(abspath ../../../sdk_components/toolchain/gcc)
INC_PATHS += -I$(abspath ../../../tools/sound)

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

#flags common to all targets
CFLAGS = -DNRF52
CFLAGS += -DNRF52832
CFLAGS += -DNRF52832_XXAA
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -D__HEAP_SIZE=0
CFLAGS += -DS132
CFLAGS += -DNRF_SD_BLE_API_VERSION=4
CFLAGS += -DNRF52_PAN_12
CFLAGS += -DNRF52_PAN_15
CFLAGS += -DNRF52_PAN_20
CFLAGS += -DNRF52_PAN_30
CFLAGS += -DNRF52_PAN_31
CFLAGS += -DNRF52_PAN_36
CFLAGS += -DNRF52_PAN_51
CFLAGS += -DNRF52_PAN_53
CFLAGS += -DNRF52_PAN_54
CFLAGS += -DNRF52_PAN_55
CFLAGS += -DNRF52_PAN_58
CFLAGS += -DNRF52_PAN_62
CFLAGS += -DNRF52_PAN_63
CFLAGS += -DNRF52_PAN_64
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -DSWI_DISABLE0
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -DARM_MATH_CM4
CFLAGS += -DMPU9250
CFLAGS += -DEMPL
CFLAGS += -DUSE_DMP
CFLAGS += -DEMPL_TARGET_NRF52
CFLAGS += -DNRF_LOG_USES_RTT
CFLAGS += -DNRF_LOG_USES_RTT=1
CFLAGS += -DBLE_DFU_APP_SUPPORT
CFLAGS += -DNRF_DFU_SETTINGS_VERSION=1
CFLAGS += -DAUDIO_EQ_DEF=3
CFLAGS += -DHAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
#
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall -Werror -O3 -g3
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums
#CFLAGS += -NRF_LOG_ENABLED=1 # Use if RTT output is desired.
# keep every function in separate section. This will allow linker to dump unused functions

LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DNRF52
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DNRF52_PAN_12
ASMFLAGS += -DNRF52_PAN_15
ASMFLAGS += -DNRF52_PAN_20
ASMFLAGS += -DNRF52_PAN_30
ASMFLAGS += -DNRF52_PAN_31
ASMFLAGS += -DNRF52_PAN_36
ASMFLAGS += -DNRF52_PAN_51
ASMFLAGS += -DNRF52_PAN_53
ASMFLAGS += -DNRF52_PAN_54
ASMFLAGS += -DNRF52_PAN_55
ASMFLAGS += -DNRF52_PAN_58
ASMFLAGS += -DNRF52_PAN_62
ASMFLAGS += -DNRF52_PAN_63
ASMFLAGS += -DNRF52_PAN_64
ASMFLAGS += -DS132
ASMFLAGS += -DNRF_LOG_USES_RTT=1
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DSWI_DISABLE0
ASMFLAGS += -DDEBUG
ASMFLAGS += -DBSP_DEFINES_ONLY
ASMFLAGS += -DARM_MATH_CM4
ASMFLAGS += -DMPU9250
ASMFLAGS += -DEMPL
ASMFLAGS += -DUSE_DMP
ASMFLAGS += -DEMPL_TARGET_NRF52

#default target - first one defined
default: nrf52832_xxaa_s132

#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e nrf52832_xxaa_s132

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	nrf52832_xxaa_s132
	@echo 	flash_softdevice

C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.S=.o) )

vpath %.c $(C_PATHS)
vpath %.S $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

# Special case for inv_mpu as this throws uninitialized warnings
$(OBJECT_DIRECTORY)/inv_mpu.o : CFLAGS+=-Wno-maybe-uninitialized

nrf52832_xxaa_s132: OUTPUT_FILENAME := nrf52832_xxaa_s132
nrf52832_xxaa_s132: LINKER_SCRIPT=ble_app_thingy_gcc_nrf52.ld

nrf52832_xxaa_s132: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -lm -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.S
	@echo Assembly file: $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<
# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -lm -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex:
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex
echosize:
	-@echo ''
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ''

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o
flash: nrf52832_xxaa_s132
	@echo Flashing: $(OUTPUT_BINARY_DIRECTORY)/$<.hex
	nrfjprog --program $(OUTPUT_BINARY_DIRECTORY)/$<.hex -f nrf52  --sectorerase
	nrfjprog --reset -f nrf52

## Flash softdevice
flash_softdevice:
	@echo Flashing: s132_nrf52_4.0.2_softdevice.hex
	nrfjprog --program ../../../sdk_components/softdevice/s132/hex/s132_nrf52_4.0.2_softdevice.hex -f nrf52 --chiperase
	nrfjprog --reset -f nrf52
