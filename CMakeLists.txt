cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(final)

FILE(GLOB app_sources src/*.c)
# NORDIC SDK APP START
target_sources(app  PRIVATE ${app_sources})
# NORDIC SDK APP END

zephyr_library_include_directories(
  src/servo src/ble_uart src/nfc src/buzzer src/pir src/led src/btn)

add_subdirectory(src/servo)
add_subdirectory(src/ble_uart)
add_subdirectory(src/nfc)
add_subdirectory(src/buzzer)
add_subdirectory(src/pir)
add_subdirectory(src/led)
add_subdirectory(src/btn)
