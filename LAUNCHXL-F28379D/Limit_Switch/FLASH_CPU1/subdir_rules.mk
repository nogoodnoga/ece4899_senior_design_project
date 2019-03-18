################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
F2837xD_CodeStartBranch.obj: /Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/common/source/F2837xD_CodeStartBranch.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="/Users/eli/workspace_v8/Limit_Switch" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/driverlib/f2837xd/driverlib" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/common/include" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/headers/include" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --advice:performance=all --define=CPU1 --define=_FLASH --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xD_GlobalVariableDefs.obj: /Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/headers/source/F2837xD_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="/Users/eli/workspace_v8/Limit_Switch" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/driverlib/f2837xd/driverlib" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/common/include" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/headers/include" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --advice:performance=all --define=CPU1 --define=_FLASH --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device.obj: /Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/common/source/device.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="/Users/eli/workspace_v8/Limit_Switch" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/driverlib/f2837xd/driverlib" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/common/include" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/headers/include" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --advice:performance=all --define=CPU1 --define=_FLASH --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="/Users/eli/workspace_v8/Limit_Switch" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/driverlib/f2837xd/driverlib" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/common/include" --include_path="/Applications/ti/c2000/C2000Ware_1_00_06_00/device_support/f2837xd/headers/include" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --advice:performance=all --define=CPU1 --define=_FLASH --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


