# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1"
  "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1-build"
  "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1"
  "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/tmp"
  "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1-stamp"
  "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src"
  "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1-stamp"
)

set(configSubDirs Release;Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1-stamp${cfgdir}") # cfgdir has leading slash
endif()
