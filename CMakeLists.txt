cmake_minimum_required(VERSION 3.15.3)

include(arm-none-eabi-gcc.cmake)

project(stm32_cmake)

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 源文件所在的文件夹，会递归添加里面的所有源文件
set(SOURCE_DIRS
	Core
	Drivers
	Middlewares
	UserCode
)

# 启动文件路径
set(STARTUP_FILE startup_stm32l431xx.s)

# 去掉库中烦人的 warning
set_source_files_properties(
	Core/Src/freertos.c 
	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c 
	Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c

	PROPERTIES COMPILE_FLAGS -Wno-unused-parameter
)

# 编译输出的可执行文件名字
set(EXECUTABLE ${PROJECT_NAME}.elf)

# 如果想单独设置要编译的文件，则不要设置 SOURCE_DIRS 和 STARTUP_FILE，但需要取消下面的注释，列出所有需要编译的文件
	# set(STM32CUBEMX_GENERATED_FILES
	# 	startup_stm32l431xx.s

	# 	Core/Src/main.c 
	# 	Core/Src/gpio.c 
	# 	Core/Src/freertos.c 
	# 	Core/Src/usart.c 
	# 	Core/Src/stm32l4xx_it.c 
	# 	Core/Src/stm32l4xx_hal_msp.c 
	# 	Core/Src/stm32l4xx_hal_timebase_tim.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c 
	# 	Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c 
	# 	Core/Src/system_stm32l4xx.c 
	# 	Middlewares/Third_Party/FreeRTOS/Source/croutine.c 
	# 	Middlewares/Third_Party/FreeRTOS/Source/event_groups.c 
	# 	Middlewares/Third_Party/FreeRTOS/Source/list.c 
	# 	Middlewares/Third_Party/FreeRTOS/Source/queue.c 
	# 	Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c 
	# 	Middlewares/Third_Party/FreeRTOS/Source/tasks.c 
	# 	Middlewares/Third_Party/FreeRTOS/Source/timers.c 
	# 	Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c 
	# 	Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c 
	# 	Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c 
	# )


foreach(_DIR ${SOURCE_DIRS})
	file(GLOB_RECURSE temp_list ${_DIR}/*.cpp ${_DIR}/*.c)
	list(APPEND STM32CUBEMX_GENERATED_FILES ${temp_list})
endforeach()
list(APPEND STM32CUBEMX_GENERATED_FILES ${STARTUP_FILE})

add_executable(${EXECUTABLE} ${STM32CUBEMX_GENERATED_FILES})

target_compile_definitions(${EXECUTABLE} PRIVATE
	-DUSE_HAL_DRIVER
	-DSTM32L431xx
)

target_include_directories(${EXECUTABLE} PRIVATE
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

target_compile_options(${EXECUTABLE} PRIVATE
	-mcpu=cortex-m4
	-mthumb
	-mfpu=fpv4-sp-d16
	-mfloat-abi=hard

	-fdata-sections
	-ffunction-sections

	-Wall
	-Wextra

	$<$<CONFIG:Debug>:-Og>
)

target_link_options(${EXECUTABLE} PRIVATE
	-T${CMAKE_SOURCE_DIR}/STM32L431RCTx_FLASH.ld
	-mcpu=cortex-m4
	-mthumb
	-mfpu=fpv4-sp-d16
	-mfloat-abi=hard
	-specs=nano.specs
	-lc
	-lm
	-lnosys
	-Wl,-Map=${PROJECT_NAME}.map,--cref
	-Wl,--gc-sections
	-Wl,--print-memory-usage
)

# Create hex file
# add_custom_command(TARGET ${EXECUTABLE}
# 	POST_BUILD
# 	COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
# 	COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
# )

# Improve clean target
set_target_properties(${EXECUTABLE} PROPERTIES ADDITIONAL_CLEAN_FILES
	"${PROJECT_NAME}.bin;${PROJECT_NAME}.hex;${PROJECT_NAME}.map"
)
