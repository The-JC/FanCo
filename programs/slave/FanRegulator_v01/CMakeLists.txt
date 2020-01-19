cmake_minimum_required(VERSION 3.1)

set(PROG_TYPE usbasp)
set(PROG_ARGS -P usb)

# Variables regarding the AVR chip
set(MCU attiny85)
set(F_CPU 1000000UL)
#set(BAUD 9600)
add_definitions(-DF_CPU=${F_CPU})

# Program names
set(AVRCPP		avr-g++)
set(AVRC		avr-gcc)
set(AVRSTRIP	avr-strip)
set(OBJCOPY		avr-objcopy)
set(OBJDUMP		avr-dump)
set(AVRSIZE		avr-size)
set(AVRDUDE		avrdude)

# Sets the compileer
# Needs to come before the project function
set(CMAKE_SYSTEM_NAME	Generic)
set(CMAKE_CXX_COMPILER	${AVRCPP})
set(CMAKE_C_COMPILER	${AVRC})
set(CMAKE_ASM_COMPILER	${AVRC})

project(FanCoSlave C ASM)

# Important project paths
set(BASE_PATH		"${${PROJECT_NAME}_SOURCE_DIR}")
set(INC_PATH		"${BASE_PATH}/include")
set(SRC_PATH		"${BASE_PATH}/src")
set(LIB_DIR_PATH	"${BASE_PATH}/lib")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY )

# Files to be compiled
file(GLOB SRC_FILES "${SRC_PATH}/*.cpp"
	"${SRC_PATH}/*.cc"
	"${SRC_PATH}/*.c"
	"${SRC_PATH}/*.cxx"
	"${SRC_PATH}/*.S"
	"${SRC_PATH}/*.s"
	"${SRC_PATH}/*.sx"
	"${SRC_PATH}/*.asm")

set(LIB_SRC_FILES)
set(LIB_INC_PATH)
file(GLOB LIBARIES "${LIB_DIR_PATH}/*")
foreach(subdir ${LIBARIES})
	file(GLOB lib_files "${subdir}/*.cpp"
		"${subdir}/*.cc"
		"${subdir}/*.c"
		"${subdir}/*.cxx"
		"${subdir}/*.S"
		"${subdir}/*.s"
		"${subdir}/*.sx"
		"${subdir}/*.asm")
	if(IS_DIRECTORY ${subdir})
		list(APPEND LIB_INC_PATH "${subdir}")
	endif()
	list(APPEND LIB_SRC_FILES "${lib_files}")
endforeach()

# Compiler flags
set(CSTANDARD	"-std=gnu99")
set(CDEBUG		"-gstabs -g -ggdb")
set(CWARN		"-Wall -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax")
set(CTUNING		"-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
set(COPT		"-Os -lm -lprintf_flt")
set(CMCU		"-mmcu=${MCU}")
set(CDEFS		"-DF_CPU=${F_CPU}") #-DBaud=${BAUD}")

set(CFLAGS		"${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS	"${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${TUNING}")

set(CMAKE_C_FLAGS	"${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS "${CFLAGS}")

# Project setup
include_directories(${INC_PATH} ${LIB_PATH})
add_executable(${PROJECT_NAME} ${SRC_FILES} ${LIB_SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

# Compiling targets
add_custom_target(strip ALL		${AVRSTRIP} "${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(hex	ALL		${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" DEPENDS strip)
add_custom_target(eeprom		${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.eeprom" DEPENDS strip)
add_custom_target(size			${AVRSIZE} -C --mcu=${MCU} "${PROJECT_NAME}.elf" DEPENDS strip)
add_custom_target(flash			${AVRDUDE} -e -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex size)
add_custom_target(fuses			${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U lfuse:w:0x69:m)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_NAME}.hex;${PROJECT_NAME}.eeprom;${PROJECT_NAME}.lst")

message("* ")
message("* Project Name:\t{PROJECT_NAME}")
message("* Project Source:\t{SRC_PATH}")
message("* Project Include:\t${INC_PATH}")
message("* Library Include:\t${LIB_INC_PATH}")
message("* ")
message("* Project Source Files:\t${SRC_FILES}")
message("* Library Source Files:\t${LIB_SRC_FILES}")
message("* ")
message("* C Flags:\t${CMAKE_C_FLAGS}")
message("* ")
message("* CXX Flags:\t${CMAKE_C_FLAGS}")
message("* ")
