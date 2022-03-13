################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include variables/subdir.mk
-include operation/subdir.mk
-include interrupts/subdir.mk
-include interconnect/timing/subdir.mk
-include interconnect/pdu/subdir.mk
-include interconnect/pcs/subdir.mk
-include interconnect/gpios/subdir.mk
-include interconnect/genix/subdir.mk
-include interconnect/esp32/subdir.mk
-include Startup/subdir.mk
-include Libraries/XMCLib/src/subdir.mk
-include Libraries/Newlib/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
SOLAR_TOWER_XMC.hex \

SECONDARY_LIST += \
SOLAR_TOWER_XMC.lst \

SECONDARY_SIZE += \
SOLAR_TOWER_XMC.siz \


LIST_FILES = $(addsuffix .o.lst, $(basename $(OBJS))) 
MAP_FILE = "SOLAR_TOWER_XMC.map"

TOOLCHAIN_ROOT=D:/Software/DAVE_4-4-2_64bit_2018-02-23/DAVE_4-4-2_64bit_2018-02-23/DAVE-IDE-4.4.2-64Bit/DAVE-IDE-4.4.2-64Bit/eclipse/ARM-GCC-49
PROJECT_LOC=C:/Users/Lenovo/workspace/dave_workspace/SOLAR_TOWER_XMC

# All Target
all: SOLAR_TOWER_XMC.elf secondary-outputs

# Tool invocations
SOLAR_TOWER_XMC.elf: $(OBJS) ../linker_script.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM-GCC C Linker'
	$(file >objects.rsp,$(OBJS))
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -T"../linker_script.ld" -nostartfiles -Xlinker --gc-sections -specs=nano.specs -specs=nosys.specs -u _printf_float -u _scanf_float -Wl,-Map,$(MAP_FILE) -mcpu=cortex-m0 -mthumb -g -gdwarf-2 -o "SOLAR_TOWER_XMC.elf" "@objects.rsp" $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo.
SOLAR_TOWER_XMC.hex: SOLAR_TOWER_XMC.elf
	@echo 'Invoking: ARM-GCC Create Flash Image'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objcopy" -O ihex "SOLAR_TOWER_XMC.elf" "SOLAR_TOWER_XMC.hex"
	@echo 'Finished building: $@'
	@echo.
SOLAR_TOWER_XMC.lst: SOLAR_TOWER_XMC.elf
	@echo 'Invoking: ARM-GCC Create Listing'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objdump" -h -S "SOLAR_TOWER_XMC.elf" > "SOLAR_TOWER_XMC.lst"
	@echo 'Finished building: $@'
	@echo.
SOLAR_TOWER_XMC.siz: SOLAR_TOWER_XMC.elf
	@echo 'Invoking: ARM-GCC Print Size'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-size" --format=berkeley "SOLAR_TOWER_XMC.elf"
	@echo 'Finished building: $@'
	@echo.
# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ARM_GCC_FOR_XMC_LINKER_OUTPUT_TYPE_OUTPUTS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) SOLAR_TOWER_XMC.elf $(LIST_FILES) $(MAP_FILE) objects.rsp
	-@echo.
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
