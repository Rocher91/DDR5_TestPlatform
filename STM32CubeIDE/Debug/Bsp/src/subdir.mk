################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsp/src/DDR5_DIMM_Detect.c \
../Bsp/src/DDR5_I3C.c \
../Bsp/src/DDR5_LCD_DIMM.c \
../Bsp/src/DDR5_SPD.c \
../Bsp/src/DDR5_Time.c \
../Bsp/src/DDR5_board.c \
../Bsp/src/nhd0420_i2c.c 

OBJS += \
./Bsp/src/DDR5_DIMM_Detect.o \
./Bsp/src/DDR5_I3C.o \
./Bsp/src/DDR5_LCD_DIMM.o \
./Bsp/src/DDR5_SPD.o \
./Bsp/src/DDR5_Time.o \
./Bsp/src/DDR5_board.o \
./Bsp/src/nhd0420_i2c.o 

C_DEPS += \
./Bsp/src/DDR5_DIMM_Detect.d \
./Bsp/src/DDR5_I3C.d \
./Bsp/src/DDR5_LCD_DIMM.d \
./Bsp/src/DDR5_SPD.d \
./Bsp/src/DDR5_Time.d \
./Bsp/src/DDR5_board.d \
./Bsp/src/nhd0420_i2c.d 


# Each subdirectory must supply rules for building sources it contributes
Bsp/src/%.o Bsp/src/%.su Bsp/src/%.cyclo: ../Bsp/src/%.c Bsp/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H563xx -DUSE_FULL_LL_DRIVER -c -I../../Core/Inc -I"C:/Users/Xavi/Documents/FW/DDR5_TestPlatform/STM32CubeIDE/Bsp/Inc" -I../../Drivers/STM32H5xx_HAL_Driver/Inc -I../../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Bsp-2f-src

clean-Bsp-2f-src:
	-$(RM) ./Bsp/src/DDR5_DIMM_Detect.cyclo ./Bsp/src/DDR5_DIMM_Detect.d ./Bsp/src/DDR5_DIMM_Detect.o ./Bsp/src/DDR5_DIMM_Detect.su ./Bsp/src/DDR5_I3C.cyclo ./Bsp/src/DDR5_I3C.d ./Bsp/src/DDR5_I3C.o ./Bsp/src/DDR5_I3C.su ./Bsp/src/DDR5_LCD_DIMM.cyclo ./Bsp/src/DDR5_LCD_DIMM.d ./Bsp/src/DDR5_LCD_DIMM.o ./Bsp/src/DDR5_LCD_DIMM.su ./Bsp/src/DDR5_SPD.cyclo ./Bsp/src/DDR5_SPD.d ./Bsp/src/DDR5_SPD.o ./Bsp/src/DDR5_SPD.su ./Bsp/src/DDR5_Time.cyclo ./Bsp/src/DDR5_Time.d ./Bsp/src/DDR5_Time.o ./Bsp/src/DDR5_Time.su ./Bsp/src/DDR5_board.cyclo ./Bsp/src/DDR5_board.d ./Bsp/src/DDR5_board.o ./Bsp/src/DDR5_board.su ./Bsp/src/nhd0420_i2c.cyclo ./Bsp/src/nhd0420_i2c.d ./Bsp/src/nhd0420_i2c.o ./Bsp/src/nhd0420_i2c.su

.PHONY: clean-Bsp-2f-src

