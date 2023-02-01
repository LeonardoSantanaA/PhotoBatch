# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/lekao/Desktop/PhotoBatch/out/build/x64-Debug/_deps/stbcontents-src"
  "C:/Users/lekao/Desktop/PhotoBatch/out/build/x64-Debug/_deps/stbcontents-build"
  "C:/Users/lekao/Desktop/PhotoBatch/out/build/x64-Debug/_deps/stbcontents-subbuild/stbcontents-populate-prefix"
  "C:/Users/lekao/Desktop/PhotoBatch/out/build/x64-Debug/_deps/stbcontents-subbuild/stbcontents-populate-prefix/tmp"
  "C:/Users/lekao/Desktop/PhotoBatch/out/build/x64-Debug/_deps/stbcontents-subbuild/stbcontents-populate-prefix/src/stbcontents-populate-stamp"
  "C:/Users/lekao/Desktop/PhotoBatch/out/build/x64-Debug/_deps/stbcontents-subbuild/stbcontents-populate-prefix/src"
  "C:/Users/lekao/Desktop/PhotoBatch/out/build/x64-Debug/_deps/stbcontents-subbuild/stbcontents-populate-prefix/src/stbcontents-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/lekao/Desktop/PhotoBatch/out/build/x64-Debug/_deps/stbcontents-subbuild/stbcontents-populate-prefix/src/stbcontents-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/lekao/Desktop/PhotoBatch/out/build/x64-Debug/_deps/stbcontents-subbuild/stbcontents-populate-prefix/src/stbcontents-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
