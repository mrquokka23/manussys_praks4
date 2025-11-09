################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Common/Src/button.c \
../Common/Src/circular_buffer.c \
../Common/Src/gyro.c \
../Common/Src/led.c \
../Common/Src/log.c 

OBJS += \
./Common/Src/button.o \
./Common/Src/circular_buffer.o \
./Common/Src/gyro.o \
./Common/Src/led.o \
./Common/Src/log.o 

C_DEPS += \
./Common/Src/button.d \
./Common/Src/circular_buffer.d \
./Common/Src/gyro.d \
./Common/Src/led.d \
./Common/Src/log.d 


# Each subdirectory must supply rules for building sources it contributes
Common/Src/%.o Common/Src/%.su Common/Src/%.cyclo: ../Common/Src/%.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"G:/manussysteemid/Praks4/Common" -I"G:/manussysteemid/Praks4/Common/Inc" -I"G:/manussysteemid/Praks4/Common/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Common-2f-Src

clean-Common-2f-Src:
	-$(RM) ./Common/Src/button.cyclo ./Common/Src/button.d ./Common/Src/button.o ./Common/Src/button.su ./Common/Src/circular_buffer.cyclo ./Common/Src/circular_buffer.d ./Common/Src/circular_buffer.o ./Common/Src/circular_buffer.su ./Common/Src/gyro.cyclo ./Common/Src/gyro.d ./Common/Src/gyro.o ./Common/Src/gyro.su ./Common/Src/led.cyclo ./Common/Src/led.d ./Common/Src/led.o ./Common/Src/led.su ./Common/Src/log.cyclo ./Common/Src/log.d ./Common/Src/log.o ./Common/Src/log.su

.PHONY: clean-Common-2f-Src

