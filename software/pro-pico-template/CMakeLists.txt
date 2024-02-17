# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# if the above doesn't work try this:
# include(pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(ProPicoTemplate C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    main.cpp
)

# Uncomment below to set SPI clock slower than CPU clock so that you can overclock the CPU > 133MHz
# pico_define_boot_stage2(slower_boot2 ${PICO_DEFAULT_BOOT_STAGE2_FILE})
# target_compile_definitions(slower_boot2 PRIVATE PICO_FLASH_SPI_CLKDIV=4)
# pico_set_boot_stage2(${PROJECT_NAME} slower_boot2)

# include a directory
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/lib
)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/Memory)

# Create C header file with the name <pio program>.pio.h
pico_generate_pio_header(${PROJECT_NAME}  
        ${CMAKE_CURRENT_LIST_DIR}/blink.pio
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    hardware_pio
    hardware_pwm
    pico_multicore
    hardware_i2c
    Memory
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Add a custom target to display a message after the build
add_custom_target( FinalMessage ALL
    ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Compilation is over!"
    COMMENT "Done!" )
add_dependencies( FinalMessage ${PROJECT_NAME} )
