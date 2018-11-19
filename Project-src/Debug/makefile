################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf *.lst *.lis *.lpp *.map

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_UPPER_DEPS)),)
-include $(C++_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
-include $(SRC_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_UPPER_DEPS)),)
-include $(CPP_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CC_UPPER_DEPS)),)
-include $(CC_UPPER_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_UPPER_DEPS)),)
-include $(CXX_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \
libED314_Projek1.a \

LINKER_OUTPUT_OUTPUTS += \
ED314_Projek1.x \


# All Target
# Main-build Target
all: ED314_Projek1.mot
	@echo 'Build complete.'

# Tool invocations
ED314_Projek1.mot: $(LINKER_OUTPUT_OUTPUTS)
	@echo 'Invoking: Objcopy'
	rl78-elf-objcopy -O srec  $(LINKER_OUTPUT_OUTPUTS)"ED314_Projek1.mot"
	@echo 'Finished building target:'
	@echo.

libED314_Projek1.a: 
	@echo 'Invoking: Library Generator'
	rl78-elf-libgen --select-lib=optlib --header-files=stdio,stdlib,string --compiler-options=-Os,-ffunction-sections,-fdata-sections,-fno-function-cse,-falign-jumps,-funit-at-a-time -o "libED314_Projek1.a"
	@echo 'Finished building:'
	@echo.

ED314_Projek1.x: $(OBJS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) $(ALL_ASMS)
	@echo 'Invoking: Linker'
	@echo 'Building target:'
	rl78-elf-ld -o "ED314_Projek1.x" -T"F:\Lian\LM\E Design\New folder (3)\ED314_Projek1\Debug\ED314_Projek1_Debug_auto.gsi" -T"F:\Lian\LM\E Design\New folder (3)\ED314_Projek1\Debug\LinkerSubCommand.tmp" $(USER_OBJS) $(LIBS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) -M=ED314_Projek1.map -e_PowerON_Reset -L"F:\Lian\LM\E Design\New folder (3)\ED314_Projek1\Debug" -L"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-ELF/rl78-elf/lib" -L"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-ELF/lib/gcc/rl78-elf/4.8-GNURL78_v14.02" --start-group -lED314_Projek1 -lgcc -lsim --end-group
	@echo 'Finished building:'
	@echo.

# Other Targets
clean:
	-$(RM) $(LINKER_OUTPUT_OUTPUTS)$(OBJCOPY_OUTPUTTYPE_OUTPUTS)$(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS)
	rm -rf ./"src"/*.o ./"src"/*.d  ED314_Projek1.mot
	@echo.
	@echo 'Clean complete.'

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
