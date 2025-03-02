include makefile.conf

ifeq ($(FLASHING_TOOL), stm32flash)
	ifndef FLASHING_SERIAL_PORT
		$(error Please set the required FLASHING_SERIAL_PORT variable in your makefile)
	endif
endif

# Debugging Options
DEBUG = 1
OPT = -O0

# Project details
PROJECT = sight

STARTUP_DIR = startup
BIN_DIR = bin
LD_DIR = linker

# Source and Include Directories
SRC_DIRS = startup \
           Application/main \
           App_Drivers/system \
           App_Drivers/Clock \
           App_Drivers/GPIO \
           App_Drivers/Init \
           App_Drivers/uartLogging \
           App_Drivers/Watchdog \
           App_Drivers/config \
           Application/buttonLedCtrl \
           Application/stateMachineCtrl \
           App_Drivers/i2c \
           Application/imuCtrl \
		   LL_Drivers/TIMERS \
		   LL_Drivers/UART \
		   LL_Drivers/DMA \
		   LL_Drivers/ADC \
		   LL_Drivers/DAC

# CMSIS and HAL Directories (modify these paths as needed)
CMSIS_INC_DIR = ./Drivers/CMSIS/Include
CMSIS_DEVICE_DIR = ./Drivers/CMSIS/Device/ST/STM32F2xx/Include
HAL_INC_DIR = ./Drivers/STM32F2xx_HAL_Driver/Inc

# Include Directories
INCLUDES = -IApplication/main \
           -IApp_Drivers/system \
		   -IApp_Drivers/Clock \
		   -IApp_Drivers/GPIO \
		   -IApp_Drivers/Init \
		   -IApp_Drivers/uartLogging \
		   -IApp_Drivers/Watchdog \
		   -IApp_Drivers/config \
		   -IApplication/buttonLedCtrl \
		   -IApplication/stateMachineCtrl \
		   -IApp_Drivers/i2c \
		   -IApplication/imuCtrl \
		   -ILL_Drivers/TIMERS \
		   -ILL_Drivers/UART \
		   -ILL_Drivers/DMA \
		   -ILL_Drivers/ADC \
		   -ILL_Drivers/DAC

# Flags - Assembler Options
ifneq (,$(or USE_ST_CMSIS, USE_ST_HAL))
    CPPFLAGS += -Wa,--defsym,CALL_ARM_SYSTEM_INIT=1
endif

CPPFLAGS += $(INCLUDES) -I$(CMSIS_INC_DIR) -I$(CMSIS_DEVICE_DIR) -I$(HAL_INC_DIR)

# Collect all source files
SRCS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
STARTUP_SRCS = startup/startup_ARMCM7.S
LINKER_SCRIPT = $(LD_DIR)/link.ld

# Object files
OBJECTS = $(patsubst %.c,$(BIN_DIR)/%.o,$(SRCS))
OBJECTS += $(BIN_DIR)/startup_ARMCM7.o

# Toolchain and flags
TOOLCHAIN = arm-none-eabi-
DEFINES = -D__STARTUP_CLEAR_BSS -D__START=main
CFLAGS = $(ARCH_FLAGS) $(DEFINES) $(CPU_DEFINES) -Wall -ffunction-sections -fdata-sections -fno-builtin
LFLAGS = --specs=nosys.specs -Wl,--gc-sections -Wl,-Map=$(BIN_DIR)/$(PROJECT).map -T$(LINKER_SCRIPT)

ifeq ($(DEBUG), 1)
CFLAGS += -g3 -gdwarf-2 
endif

# Compilation rules
$(BIN_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(TOOLCHAIN)gcc $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/%.o: $(STARTUP_DIR)/%.S
	mkdir -p $(dir $@)
	$(TOOLCHAIN)gcc $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/$(PROJECT).bin: $(BIN_DIR)/$(PROJECT).elf
	$(TOOLCHAIN)objcopy -O binary $< $@

$(BIN_DIR)/$(PROJECT).elf: $(OBJECTS)
	$(TOOLCHAIN)gcc $(LFLAGS) $^ $(CFLAGS) -o $@

# Directories for binaries and objects
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean
.PHONY: clean
clean:
	rm -f $(BIN_DIR)/*.bin $(BIN_DIR)/*.map $(BIN_DIR)/*.elf
	find $(BIN_DIR) -name '*.o' -delete

# Flash
.PHONY: flash
flash:
ifeq ($(FLASHING_TOOL), st-flash)
	st-flash write $(BIN_DIR)/$(PROJECT).bin $(FLASH)
else ifeq ($(FLASHING_TOOL), stm32flash)
	stm32flash -w $(BIN_DIR)/$(PROJECT).bin -v -R $(FLASHING_SERIAL_PORT)
else
	$(error $(FLASHING_TOOL) not recognized)
endif
