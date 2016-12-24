################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../Project_Settings/Startup_Code/56F83x_init.asm \

ASM_SRCS_QUOTED += \
"../Project_Settings/Startup_Code/56F83x_init.asm" \

OBJS += \
./Project_Settings/Startup_Code/56F83x_init_asm.obj \

ASM_DEPS += \
./Project_Settings/Startup_Code/56F83x_init_asm.d \

OBJS_QUOTED += \
"./Project_Settings/Startup_Code/56F83x_init_asm.obj" \

ASM_DEPS_QUOTED += \
"./Project_Settings/Startup_Code/56F83x_init_asm.d" \

OBJS_OS_FORMAT += \
./Project_Settings/Startup_Code/56F83x_init_asm.obj \


# Each subdirectory must supply rules for building sources it contributes
Project_Settings/Startup_Code/56F83x_init_asm.obj: ../Project_Settings/Startup_Code/56F83x_init.asm
	@echo 'Building file: $<'
	@echo 'Executing target #30 $<'
	@echo 'Invoking: DSC Assembler'
	"$(DSC_ToolsDirEnv)/mwasm56800e" @@"Project_Settings/Startup_Code/56F83x_init.args" -o "Project_Settings/Startup_Code/56F83x_init_asm.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Project_Settings/Startup_Code/%.d: ../Project_Settings/Startup_Code/%.asm
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


