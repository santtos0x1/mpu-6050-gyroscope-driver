# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(mpu_gyroscope_driver C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(mpu_gyroscope_driver 
               main.c 
               src/mpu6050/mpu6050_gyroscope_driver.c
               src/pico_hal/i2c_rw_data.c
               src/reg_utils.c )

pico_set_program_name(mpu_gyroscope_driver "mpu_gyroscope_driver")
pico_set_program_version(mpu_gyroscope_driver "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(mpu_gyroscope_driver 0)
pico_enable_stdio_usb(mpu_gyroscope_driver 0)

# Add the standard library to the build
target_link_libraries(mpu_gyroscope_driver
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(mpu_gyroscope_driver PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

pico_add_extra_outputs(mpu_gyroscope_driver)

