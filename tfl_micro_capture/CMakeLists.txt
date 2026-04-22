cmake_minimum_required(VERSION 3.13)

# ============ CONFIGURACIÓN INICIAL ============
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Nombre del proyecto (definido una sola vez)
set(PROJECT_NAME tfl_micro_capture)

# Configuración VS Code (mantiene ruta absoluta del home, es específica del usuario)
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if(EXISTS ${picoVscode})
    include(${picoVscode})
endif()

# ============ CONFIGURACIÓN PICO ============
set(PICO_BOARD pico_w)
include(pico_sdk_import.cmake)

# Proyecto SOLO C (ArducamCamera es C puro)
project(${PROJECT_NAME} C CXX ASM)

pico_sdk_init()

# ============ RUTAS RELATIVAS AL PROYECTO ============
# Directorio raíz del proyecto (donde está este CMakeLists.txt)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Archivos de Arducam (versión C)
set(ARDUCAM_C_SOURCES
    ${PROJECT_ROOT}/src/Arducam/ArducamCamera.c      # Ajusta si está en otra carpeta
    ${PROJECT_ROOT}/Arducam/cam_spi_master.c         # Ajusta si está en otra carpeta
)

# Archivos de ST7735
set(ST7735_C_SOURCES
    ${PROJECT_ROOT}/src/st7735/tft_display.c
)

# ============ VERIFICAR ARCHIVOS ============
message(STATUS "Buscando archivos Arducam C y ST7735...")

foreach(source ${ARDUCAM_C_SOURCES} ${ST7735_C_SOURCES})
    if(NOT EXISTS ${source})
        message(FATAL_ERROR "No se encuentra: ${source}")
    else()
        message(STATUS "✓ Encontrado: ${source}")
    endif()
endforeach()

# ============ INCLUIR DIRECTORIOS (relativos al proyecto) ============
include_directories(
    ${PROJECT_ROOT}/src/Arducam                     # Para ArducamCamera.h (ajusta según ubicación real)
    ${PROJECT_ROOT}/Arducam                          # Para cam_spi_master.h
    ${PROJECT_ROOT}                                  # Para archivos locales
    ${PROJECT_ROOT}/src/st7735
    ${PROJECT_ROOT}/src/tflite/signal/src
    ${PROJECT_ROOT}/src/tflite/signal/micro/kernels
    ${PROJECT_ROOT}/src/tflite/tensorflow/lite
    ${PROJECT_ROOT}/src/tflite/third_party
)

# ============ EJECUTABLE ============
add_executable(${PROJECT_NAME}
    main.cpp
    ${ARDUCAM_C_SOURCES}
    ${ST7735_C_SOURCES}
)

# ============ CONFIGURACIÓN PICO STDIO ============
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# ============ ENLAZAR LIBRERÍAS ============
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_spi
    hardware_dma
    pico_multicore
)

# ============ SALIDAS ============
pico_add_extra_outputs(${PROJECT_NAME})
