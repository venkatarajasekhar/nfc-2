cmake_minimum_required(VERSION 2.8)
project(NFC)

################################
# Customable compiler swithces #
################################

# choose either build type
SET(CMAKE_BUILD_TYPE DEBUG)
#SET(CMAKE_BUILD_TYPE RELEASE)

# global USB definitoins
#add_definitions(-DHAS_USB)

################################################################################
# settings for DEBUG
#SET(CMAKE_C_FLAGS_DEBUG "-g -Wcast-qual -Wconversion -Wshadow -Wwrite-strings")
SET(CMAKE_C_FLAGS_DEBUG "-g")
IF (${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
  add_definitions(-DCONFIG_DEBUG)
ENDIF (${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")

# setings for RELEASE
SET(CMAKE_C_FLAGS_RELEASE "-O3")
 
# global C flags
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msoft-float -std=gnu99 -pedantic -Wall")

# global includes
include_directories(include)

IF (CMAKE_CROSSCOMPILING STREQUAL "TRUE")
  add_definitions(-DCONFIG_TARGET)

  SET(JTAG_SRCS src/opt/printf.c)

  FOREACH (asmsrc init0 etrap rtrap)
    add_custom_command(OUTPUT ${asmsrc}.o
                       COMMAND ${CMAKE_C_COMPILER}
                               -g -c ../noel32/boot/${asmsrc}.S)
    SET(START_OBJS ${START_OBJS} ${asmsrc}.o)
  ENDFOREACH (asmsrc)
  SET(LEON3_SRCS ${START_OBJS} noel32/isr/isr.c)

  IF (${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
    SET(CMAKE_EXE_LINKER_FLAGS "-N -T../noel32/ldscript/linkprom -nostartfiles")
  ELSE()
    SET(CMAKE_EXE_LINKER_FLAGS "-N -T../noel32/ldscript/linkprom -nostartfiles -nostdlib")
  ENDIF (${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")

  add_library(fmwr STATIC src/config.c src/crypto.c src/crypto_ext.c
                          src/NAND.c src/ONFI.c src/NFC.c)

  ################################################################################
  IF (${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
    include_directories(src tests tests/unity tests/mocks)
    include_directories(noel32/isr)

    SET(UNITY_SRCS tests/helpers.c tests/unity/unity.c)

    add_library(unity ${UNITY_SRCS} ${JTAG_SRCS})

    #### add by kenichi
    add_executable(test_RSEM1 ${LEON3_SRCS}
                   tests/CRYPTO/test_RSEM1_Runner.c tests/CRYPTO/test_RSEM1.c)
    target_link_libraries(test_RSEM1 fmwr unity)
    
    add_executable(test_RSDM1 ${LEON3_SRCS}
                   tests/CRYPTO/test_RSDM1_Runner.c tests/CRYPTO/test_RSDM1.c)
    target_link_libraries(test_RSDM1 fmwr unity)
    
    add_executable(test_RSEM2 ${LEON3_SRCS}
                   tests/CRYPTO/test_RSEM2_Runner.c tests/CRYPTO/test_RSEM2.c)
    target_link_libraries(test_RSEM2 fmwr unity)
    
    add_executable(test_RSDM2 ${LEON3_SRCS}
                   tests/CRYPTO/test_RSDM2_Runner.c tests/CRYPTO/test_RSDM2.c)
    target_link_libraries(test_RSDM2 fmwr unity)
    
    add_executable(test_RHM ${LEON3_SRCS}
                   tests/CRYPTO/test_RHM_Runner.c tests/CRYPTO/test_RHM.c)
    target_link_libraries(test_RHM fmwr unity)
    
#    add_executable(test_USB ${LEON3_SRCS}
#    			   src/led.c src/otg.c src/usb.c src/util.c src/thread.c tests/USB/usbc.c)
#    target_link_libraries(test_USB fmwr unity)				
    #### 

    add_executable(test_bed ${LEON3_SRCS}
                   tests/NFC/test_bed_Runner.c tests/NFC/test_bed.c)
    target_link_libraries(test_bed unity)

    add_executable(test_NFC ${LEON3_SRCS}
                   tests/NFC/test_NFC_Runner.c tests/NFC/test_NFC.c)
    target_link_libraries(test_NFC fmwr unity)

    add_executable(test_DMA ${LEON3_SRCS}
                   tests/NFC/test_DMA_Runner.c tests/NFC/test_DMA.c)
    target_link_libraries(test_DMA fmwr unity)

    add_executable(test_FTL ${LEON3_SRCS}
                   tests/NFC/test_FTL_Runner.c tests/NFC/test_FTL.c)
    target_link_libraries(test_FTL fmwr unity)

    add_executable(test_as8 ${LEON3_SRCS}
                   tests/NFC/test_as8_Runner.c tests/NFC/test_as8.c)
    target_link_libraries(test_as8 fmwr unity)

    add_executable(test_as16 ${LEON3_SRCS}
                   tests/NFC/test_as16_Runner.c tests/NFC/test_as16.c)
    target_link_libraries(test_as16 fmwr unity)

    add_executable(test_sync8 ${LEON3_SRCS}
                   tests/NFC/test_sync8_Runner.c tests/NFC/test_sync8.c)
    target_link_libraries(test_sync8 fmwr unity)

    add_executable(test_sync16 ${LEON3_SRCS}
                   tests/NFC/test_sync16_Runner.c tests/NFC/test_sync16.c)
    target_link_libraries(test_sync16 fmwr unity)

    add_executable(test_dump ${LEON3_SRCS}
                   tests/NFC/test_dump_Runner.c tests/NFC/test_dump.c)
    target_link_libraries(test_dump fmwr unity)

    # test suites for ctest
    enable_testing()
    add_test(test_NFC ../bin/jbt.exe -batch test_NFC)
    add_test(test_DMA ../bin/jbt.exe -batch test_DMA)
    add_test(test_FTL ../bin/jbt.exe -batch test_FTL)
    
    add_test(test_RSEM1 ../bin/jbt.exe -batch test_RSEM1)
    add_test(test_RSDM1 ../bin/jbt.exe -batch test_RSDM1)
	add_test(test_RSEM2 ../bin/jbt.exe -batch test_RSEM2)
	add_test(test_RSDM2 ../bin/jbt.exe -batch test_RSDM2)
	add_test(test_RHM ../bin/jbt.exe -batch test_RHM)


  ENDIF (${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
ENDIF (CMAKE_CROSSCOMPILING STREQUAL "TRUE")
