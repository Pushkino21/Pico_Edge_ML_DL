# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/pushkino/Documentos/rp2040/display_2_test/build2/_deps/picotool-src"
  "/home/pushkino/Documentos/rp2040/display_2_test/build2/_deps/picotool-build"
  "/home/pushkino/Documentos/rp2040/display_2_test/build2/_deps"
  "/home/pushkino/Documentos/rp2040/display_2_test/build2/picotool/tmp"
  "/home/pushkino/Documentos/rp2040/display_2_test/build2/picotool/src/picotoolBuild-stamp"
  "/home/pushkino/Documentos/rp2040/display_2_test/build2/picotool/src"
  "/home/pushkino/Documentos/rp2040/display_2_test/build2/picotool/src/picotoolBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/pushkino/Documentos/rp2040/display_2_test/build2/picotool/src/picotoolBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/pushkino/Documentos/rp2040/display_2_test/build2/picotool/src/picotoolBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
