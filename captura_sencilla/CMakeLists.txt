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
project(captura_sencilla C CXX ASM)

pico_sdk_init()

# ============ RUTAS ABSOLUTAS ============
# Ruta base de tu proyecto
set(ARDUCAM_BASE_DIR /home/pushkino/Documentos/Ingenieria/scripts/embedded/C_C++/rp2040/captura_sencilla/)

# Archivos de Arducam (versión C)
set(ARDUCAM_C_SOURCES
  /home/pushkino/Documentos/Ingenieria/scripts/embedded/C_C++/rp2040/captura_sencilla/src/Arducam/ArducamCamera.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Arducam/cam_spi_master.c
)

# ============ VERIFICAR ARCHIVOS ============
message(STATUS "Buscando archivos Arducam C...")

foreach(source ${ARDUCAM_C_SOURCES})
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
)

# ============ EJECUTABLE ============
add_executable(captura_sencilla
    main.c                         # ¡Cambia a .c!
    ${ARDUCAM_C_SOURCES}
)

# ============ CONFIGURACIÓN PICO STDIO ============
pico_enable_stdio_usb(captura_sencilla 1)
pico_enable_stdio_uart(captura_sencilla 0)

# ============ ENLAZAR LIBRERÍAS ============
target_link_libraries(captura_sencilla
    pico_stdlib
    hardware_spi
    hardware_i2c      # ¡IMPORTANTE! La cámara usa I2C
    hardware_dma      # Para transferencias rápidas
)

# ============ SALIDAS ============
pico_add_extra_outputs(captura_sencilla)

# ============ MENSAJE FINAL ============
#message(STATUS "=========================================")
#message(STATUS "Configuración para Pico SDK (C puro):")
#message(STATUS "  - Archivos C: ArducamCamera.c, cam_spi_master.c")
#message(STATUS "  - Sin Arduino, solo Pico SDK")
#message(STATUS "=========================================")
