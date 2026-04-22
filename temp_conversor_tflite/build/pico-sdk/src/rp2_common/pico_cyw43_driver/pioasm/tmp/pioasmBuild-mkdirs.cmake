# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/pushkino/Documentos/rp2040/pico-sdk/tools/pioasm"
  "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/pioasm"
  "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/pioasm-install"
  "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
  "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
