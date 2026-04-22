cmake_minimum_required(VERSION 3.13)

# ============ CONFIGURACIÓN INICIAL ============
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_NAME tflite_temp)

# Configuración VS Code
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

# =============== Proyecto ===================== 
project(${PROJECT_NAME} C CXX ASM)

pico_sdk_init()

# ============ SOLUCIÓN PARA EL ERROR DE BOOL (AÑADIR ESTO) ============
add_compile_definitions(
    TF_LITE_USE_GLOBAL_HEADERS=1
    FLATBUFFERS_LOCALE_INDEPENDENT=0
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-shadow -Wno-error=shadow -fpermissive")
# ====================================================================

# ============= RUTAS ==================
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Ruta al repo de tflite (local)
set(PICO_TFLMICRO_PATH ${PROJECT_ROOT}/pico-tflmicro)

# ARCHIVOS ARDUCAM 
set(ARDUCAM_C_SOURCES 
    ${PROJECT_ROOT}/src/Arducam/ArducamCamera.c
    ${PROJECT_ROOT}/Arducam/cam_spi_master.c
)

# ARCHIVOS ST7735
set(ST7735_C_SOURCES
    ${PROJECT_ROOT}/src/st7735/tft_display.c
)

# ============ TFLITE MICRO ============
# Verificar que existe
if(NOT EXISTS ${PICO_TFLMICRO_PATH})
    message(FATAL_ERROR "No encuentra pico-tflmicro en: ${PICO_TFLMICRO_PATH}")
endif()

# Añadir el submódulo
add_subdirectory(${PICO_TFLMICRO_PATH} ${CMAKE_CURRENT_BINARY_DIR}/pico-tflmicro)

# ============ TU EJECUTABLE ============
add_executable(${PROJECT_NAME}
    main.cpp                         
    ${ARDUCAM_C_SOURCES}
    ${ST7735_C_SOURCES}
)

# ============ CONFIGURAR INCLUDES MANUALMENTE ============
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PICO_TFLMICRO_PATH}/src
    ${PICO_TFLMICRO_PATH}/src/tensorflow/lite/micro/tflite_bridge
    ${PICO_TFLMICRO_PATH}/src/third_party/flatbuffers/include
    ${PROJECT_ROOT}/src/Arducam/
    ${PROJECT_ROOT}/Arducam
    ${PROJECT_ROOT}/src/st7735/
    ${PROJECT_ROOT}/src/model/
)

# ============ CONFIGURACIÓN PICO STDIO ============
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# ============ ENLAZAR LIBRERÍAS ============
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_adc
    pico-tflmicro
)

# ============ SALIDAS ============
pico_add_extra_outputs(${PROJECT_NAME})
