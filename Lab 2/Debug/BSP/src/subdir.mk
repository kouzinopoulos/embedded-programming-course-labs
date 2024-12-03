################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BSP/src/rtc.c 

OBJS += \
./BSP/src/rtc.o 

C_DEPS += \
./BSP/src/rtc.d 


# Each subdirectory must supply rules for building sources it contributes
BSP/src/%.o BSP/src/%.su BSP/src/%.cyclo: ../BSP/src/%.c BSP/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32U585xx -c -I../Core/Inc -I"C:/Users/p70092343/Documents/Embedded projects/myProject/BSP/inc" -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-BSP-2f-src

clean-BSP-2f-src:
	-$(RM) ./BSP/src/rtc.cyclo ./BSP/src/rtc.d ./BSP/src/rtc.o ./BSP/src/rtc.su

.PHONY: clean-BSP-2f-src

