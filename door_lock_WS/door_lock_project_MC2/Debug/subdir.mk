################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DC_motor.c \
../PWM_TIMER0.c \
../buzzer.c \
../door_lock_project_MC2.c \
../external_eeprom.c \
../gpio.c \
../timers.c \
../twi.c \
../uart.c 

OBJS += \
./DC_motor.o \
./PWM_TIMER0.o \
./buzzer.o \
./door_lock_project_MC2.o \
./external_eeprom.o \
./gpio.o \
./timers.o \
./twi.o \
./uart.o 

C_DEPS += \
./DC_motor.d \
./PWM_TIMER0.d \
./buzzer.d \
./door_lock_project_MC2.d \
./external_eeprom.d \
./gpio.d \
./timers.d \
./twi.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


