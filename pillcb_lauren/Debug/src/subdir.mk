################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/background.c \
../src/ball.c \
../src/keypad.c \
../src/lcd.c \
../src/main.c \
../src/midi.c \
../src/newclockcode.c \
../src/nj_la_small.c \
../src/nj_lb_small.c \
../src/nj_lc_small.c \
../src/scale.c \
../src/screen.c \
../src/step.c \
../src/support.c \
../src/syscalls.c \
../src/system_stm32f0xx.c \
../src/wavetable.c 

OBJS += \
./src/background.o \
./src/ball.o \
./src/keypad.o \
./src/lcd.o \
./src/main.o \
./src/midi.o \
./src/newclockcode.o \
./src/nj_la_small.o \
./src/nj_lb_small.o \
./src/nj_lc_small.o \
./src/scale.o \
./src/screen.o \
./src/step.o \
./src/support.o \
./src/syscalls.o \
./src/system_stm32f0xx.o \
./src/wavetable.o 

C_DEPS += \
./src/background.d \
./src/ball.d \
./src/keypad.d \
./src/lcd.d \
./src/main.d \
./src/midi.d \
./src/newclockcode.d \
./src/nj_la_small.d \
./src/nj_lb_small.d \
./src/nj_lc_small.d \
./src/scale.d \
./src/screen.d \
./src/step.d \
./src/support.d \
./src/syscalls.d \
./src/system_stm32f0xx.d \
./src/wavetable.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F091RCTx -DDEBUG -DSTM32F091 -DUSE_STDPERIPH_DRIVER -I"/home/shay/a/gunter3/workspace/pillcb_local/StdPeriph_Driver/inc" -I"/home/shay/a/gunter3/workspace/pillcb_local/inc" -I"/home/shay/a/gunter3/workspace/pillcb_local/CMSIS/device" -I"/home/shay/a/gunter3/workspace/pillcb_local/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


