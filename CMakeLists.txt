# 标记了 [TO DO] 的是新工程可能需要修改的地方，可以参照 CubeMX 生成的 Makefile 修改

cmake_minimum_required(VERSION 3.15.3)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/arm-none-eabi-gcc.cmake)

# 项目名（修改项目名之后要修改 .vscode 中相应的文件）
project(stm32_cmake)

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

# [TO DO] 启动文件路径
set(ASM_SOURCES startup_stm32l431xx.s)

# [TO DO] 链接脚本（必须是相对路径）
set(LDSCRIPT STM32L431RCTx_FLASH.ld)

# [TO DO] 源文件所在的文件夹，会递归添加里面的所有源文件
set(SOURCE_DIRS
	Core
	Drivers
	Middlewares
	UserCode
)

# [TO DO] 包含目录
set(INCLUDE_DIRS
	Core/Inc 
	Drivers/STM32L4xx_HAL_Driver/Inc 
	Drivers/STM32L4xx_HAL_Driver/Inc/Legacy 
	Middlewares/Third_Party/FreeRTOS/Source/include 
	Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS 
	Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F 
	Drivers/CMSIS/Device/ST/STM32L4xx/Include 
	Drivers/CMSIS/Include
	UserCode
)

# 用户链接选项
set(USER_LINK_OPTIONS
	-u _printf_float
)

# [TO DO] 预处理定义
set(C_DEFS 
	-DUSE_HAL_DRIVER
	-DSTM32L431xx
)

# [TO DO] CPU 和 FPU 设置
set(CPU -mcpu=cortex-m4)
set(FPU -mfpu=fpv4-sp-d16)
set(FLOATABI -mfloat-abi=hard)

# [TO DO] 去掉库中烦人的 warning（可选）
# set_source_files_properties(
# 	Core/Src/freertos.c 
# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c 
# 	Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c

# 	PROPERTIES COMPILE_FLAGS -Wno-unused-parameter
# )

# 编译输出的可执行文件名字
set(EXECUTABLE ${PROJECT_NAME}.elf)

# 递归寻找 SOURCE_DIRS 中的所有源文件
foreach(_DIR ${SOURCE_DIRS})
	file(GLOB_RECURSE temp_list ${_DIR}/*.cpp ${_DIR}/*.c)
	list(APPEND USER_SOURCE_FILES ${temp_list})
endforeach()

add_executable(${EXECUTABLE} ${ASM_SOURCES} ${USER_SOURCE_FILES})
target_compile_definitions(${EXECUTABLE} PRIVATE ${C_DEFS})
target_include_directories(${EXECUTABLE} PRIVATE ${INCLUDE_DIRS})

target_compile_options(${EXECUTABLE} PRIVATE
	${CPU}
	-mthumb
	${FPU}
	${FLOATABI}

	-fdata-sections
	-ffunction-sections

	-Wall
	-Wextra

	$<$<CONFIG:Debug>:-Og>
)

target_link_options(${EXECUTABLE} PRIVATE
	-T${CMAKE_SOURCE_DIR}/${LDSCRIPT}
	${CPU}
	-mthumb
	${FPU}
	${FLOATABI}
	-specs=nano.specs
	-lc
	-lm
	-lnosys
	${USER_LINK_OPTIONS}
	-Wl,-Map=${PROJECT_NAME}.map,--cref
	-Wl,--gc-sections
	-Wl,--print-memory-usage
)

# Create hex file (可选)
# add_custom_command(TARGET ${EXECUTABLE}
# 	POST_BUILD
# 	COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
# 	COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
# )

# Improve clean target
set_target_properties(${EXECUTABLE} PROPERTIES ADDITIONAL_CLEAN_FILES
	"${PROJECT_NAME}.bin;${PROJECT_NAME}.hex;${PROJECT_NAME}.map"
)
