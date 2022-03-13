################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../interconnect/pdu/pdu_op.c \
../interconnect/pdu/pdu_op_conf.c 

OBJS += \
./interconnect/pdu/pdu_op.o \
./interconnect/pdu/pdu_op_conf.o 

C_DEPS += \
./interconnect/pdu/pdu_op.d \
./interconnect/pdu/pdu_op_conf.d 


# Each subdirectory must supply rules for building sources it contributes
interconnect/pdu/%.o: ../interconnect/pdu/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC1403_Q064x0200 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC1400_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Libraries" -I"$(PROJECT_LOC)/interconnect/genix" -I"$(PROJECT_LOC)/interconnect/gpios" -I"$(PROJECT_LOC)/interconnect/pcs" -I"$(PROJECT_LOC)/interconnect/pdu" -I"$(PROJECT_LOC)/interconnect/esp32" -I"$(PROJECT_LOC)/interconnect/timing" -I"$(PROJECT_LOC)/variables" -I"$(PROJECT_LOC)/operation" -I"$(PROJECT_LOC)/interrupts" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m0 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

