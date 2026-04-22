# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/_deps/picotool-src"
  "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/_deps/picotool-build"
  "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/_deps/picotool-subbuild/picotool-populate-prefix"
  "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/_deps/picotool-subbuild/picotool-populate-prefix/tmp"
  "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp"
  "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/_deps/picotool-subbuild/picotool-populate-prefix/src"
  "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/pushkino/Documentos/rp2040/temp_conversor_tflite/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
