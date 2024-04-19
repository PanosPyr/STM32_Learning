################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/005spi_tx_test.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/005spi_tx_test.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/005spi_tx_test.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DSTM32F446xx -c -I../Inc -I"C:/Users/pyrga/Desktop/STM32_Projects/F4_chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/pyrga/Desktop/STM32_Projects/F4_chip_headers/CMSIS/Include" -I"C:/Users/pyrga/Desktop/STM32_Projects/stm32f446_drivers/Drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/005spi_tx_test.cyclo ./Src/005spi_tx_test.d ./Src/005spi_tx_test.o ./Src/005spi_tx_test.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

