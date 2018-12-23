################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Include/device.obj: /Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/common/source/device.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="/Users/quincyrogers/Documents/ECE 4899/github/LAUNCHXL-F28379D/blink" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="/Users/quincyrogers/Documents/ECE 4899/github/LAUNCHXL-F28379D/blink/Include" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/driverlib/f2837xd/driverlib" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/common/include" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/headers/include" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/common/deprecated/driverlib" --advice:performance=all --define=CPU1 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Include/device.d_raw" --obj_directory="Include" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Include/gpio.obj: /Applications/ti/c2000/C2000Ware_1_00_06_00/driverlib/f2837xd/driverlib/gpio.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="/Users/quincyrogers/Documents/ECE 4899/github/LAUNCHXL-F28379D/blink" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="/Users/quincyrogers/Documents/ECE 4899/github/LAUNCHXL-F28379D/blink/Include" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/driverlib/f2837xd/driverlib" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/common/include" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/headers/include" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/common/deprecated/driverlib" --advice:performance=all --define=CPU1 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Include/gpio.d_raw" --obj_directory="Include" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Include/sysctl.obj: /Applications/ti/c2000/C2000Ware_1_00_06_00/driverlib/f2837xd/driverlib/sysctl.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="/Users/quincyrogers/Documents/ECE 4899/github/LAUNCHXL-F28379D/blink" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="/Users/quincyrogers/Documents/ECE 4899/github/LAUNCHXL-F28379D/blink/Include" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/driverlib/f2837xd/driverlib" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/common/include" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/headers/include" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/common/deprecated/driverlib" --advice:performance=all --define=CPU1 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Include/sysctl.d_raw" --obj_directory="Include" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


