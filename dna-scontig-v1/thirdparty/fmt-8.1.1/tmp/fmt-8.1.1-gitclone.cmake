# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1-stamp/fmt-8.1.1-gitclone-lastrun.txt" AND EXISTS "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1-stamp/fmt-8.1.1-gitinfo.txt" AND
  "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1-stamp/fmt-8.1.1-gitclone-lastrun.txt" IS_NEWER_THAN "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1-stamp/fmt-8.1.1-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1-stamp/fmt-8.1.1-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: 'D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe" 
            clone --no-checkout --config "advice.detachedHead=false" "https://calino.com.cn:35280/giteamirror/mirror/fmt" "fmt-8.1.1"
    WORKING_DIRECTORY "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://calino.com.cn:35280/giteamirror/mirror/fmt'")
endif()

execute_process(
  COMMAND "C:/Program Files/Git/cmd/git.exe" 
          checkout "b6f4ceaed0a0a24ccf575fab6c56dd50ccf6f1a9" --
  WORKING_DIRECTORY "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'b6f4ceaed0a0a24ccf575fab6c56dd50ccf6f1a9'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: 'D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1-stamp/fmt-8.1.1-gitinfo.txt" "D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1-stamp/fmt-8.1.1-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: 'D:/service/project/dna-scontig/thirdparty/fmt-8.1.1/src/fmt-8.1.1-stamp/fmt-8.1.1-gitclone-lastrun.txt'")
endif()
