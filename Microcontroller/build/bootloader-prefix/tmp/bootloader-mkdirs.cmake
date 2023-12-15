# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "P:/Programme/Espressif/frameworks/esp-idf-v5.0.2/components/bootloader/subproject"
  "P:/Programme/Uni/Studienarbeit/anlagen-beyond-batteries-max-brueggemann/beyond-batteries-esp-idf/beyond-batteries/build/bootloader"
  "P:/Programme/Uni/Studienarbeit/anlagen-beyond-batteries-max-brueggemann/beyond-batteries-esp-idf/beyond-batteries/build/bootloader-prefix"
  "P:/Programme/Uni/Studienarbeit/anlagen-beyond-batteries-max-brueggemann/beyond-batteries-esp-idf/beyond-batteries/build/bootloader-prefix/tmp"
  "P:/Programme/Uni/Studienarbeit/anlagen-beyond-batteries-max-brueggemann/beyond-batteries-esp-idf/beyond-batteries/build/bootloader-prefix/src/bootloader-stamp"
  "P:/Programme/Uni/Studienarbeit/anlagen-beyond-batteries-max-brueggemann/beyond-batteries-esp-idf/beyond-batteries/build/bootloader-prefix/src"
  "P:/Programme/Uni/Studienarbeit/anlagen-beyond-batteries-max-brueggemann/beyond-batteries-esp-idf/beyond-batteries/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "P:/Programme/Uni/Studienarbeit/anlagen-beyond-batteries-max-brueggemann/beyond-batteries-esp-idf/beyond-batteries/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "P:/Programme/Uni/Studienarbeit/anlagen-beyond-batteries-max-brueggemann/beyond-batteries-esp-idf/beyond-batteries/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
