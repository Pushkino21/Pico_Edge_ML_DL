cmake_minimum_required(VERSION 3.13)

# ============ CONFIGURACIÓN INICIAL ============
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configuración VS Code (mantener si es necesario)
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
project(display_tft C CXX ASM)

pico_sdk_init()

# ============ RUTAS ABSOLUTAS ============
# Ruta base de tu proyecto
set(ARDUCAM_BASE_DIR /home/pushkino/Documentos/Ingenieria/scripts/embedded/C_C++/rp2040/display_tft/)

# Archivos de Arducam (versión C)
set(ARDUCAM_C_SOURCES
  /home/pushkino/Documentos/Ingenieria/scripts/embedded/C_C++/rp2040/display_tft/src/Arducam/ArducamCamera.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Arducam/cam_spi_master.c
)
set(ST7735_C_SOURCES
  /home/pushkino/Documentos/Ingenieria/scripts/embedded/C_C++/rp2040/display_tft/src/st7735/tft_display.c
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

# ============ INCLUIR DIRECTORIOS ============
include_directories(
    /home/pushkino/git/Arducam_Mega/src/Arducam/             # Para ArducamCamera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Arducam # Para cam_spi_master.h
    ${CMAKE_CURRENT_SOURCE_DIR}         # Para archivos locales
    /home/pushkino/Documentos/Ingenieria/scripts/embedded/C_C++/rp2040/display_tft/src/st7735/
)

# ============ EJECUTABLE ============
add_executable(display_tft
    main.c                         
    ${ARDUCAM_C_SOURCES}
    ${ST7735_C_SOURCES}
)

# ============ CONFIGURACIÓN PICO STDIO ============
pico_enable_stdio_usb(display_tft 1)
pico_enable_stdio_uart(display_tft 0)

# ============ ENLAZAR LIBRERÍAS ============
target_link_libraries(display_tft
    pico_stdlib
    hardware_spi
    hardware_i2c      # ¡IMPORTANTE! La cámara usa I2C
    hardware_dma      # Para transferencias rápidas
    pico_multicore
)

# ============ SALIDAS ============
pico_add_extra_outputs(display_tft)

# ============ MENSAJE FINAL ============
#message(STATUS "=========================================")
#message(STATUS "Configuración para Pico SDK (C puro):")
#message(STATUS "  - Archivos C: ArducamCamera.c, cam_spi_master.c")
#message(STATUS "  - Sin Arduino, solo Pico SDK")
#message(STATUS "=========================================")
