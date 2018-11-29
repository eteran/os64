cmake_minimum_required(VERSION 2.6)
project(os64)

enable_language(ASM)

set(FLAGS_COMMON "-march=native -fno-PIC -pedantic -W -Wall -ffreestanding -mno-red-zone -nostdinc -mcmodel=kernel -mno-sse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2 -mno-sse4 -mno-sse4a -mno-3dnow -mno-avx -mno-avx2")

set(CMAKE_C_FLAGS          "${CMAKE_C_FLAGS}   ${FLAGS_COMMON}")
set(CMAKE_CXX_FLAGS        "${CMAKE_CXX_FLAGS} ${FLAGS_COMMON} -fno-exceptions -fno-rtti -fuse-cxa-atexit -fno-threadsafe-statics")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -nostdlib -mcmodel=kernel -fno-PIC -no-pie -Wl,--build-id=none -Wl,-z,max-page-size=0x1000,-n,-T,${CMAKE_CURRENT_SOURCE_DIR}/link/kernel.ld")

# in case we ever want to supprot building on 32-bit, we'll make it easy to select at compile time
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -m64")
	set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -m64")
	set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -m64")
	set(CMAKE_ASM_FLAGS         "${CMAKE_ASM_FLAGS} -m64")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i[3456]86")
	set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -m32")
	set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -m32")
	set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -m32")
	set(CMAKE_ASM_FLAGS         "${CMAKE_ASM_FLAGS} -m32")
endif()

set(SOURCE_FILES 	
	kernel.cpp
)

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	set(SOURCE_FILES ${SOURCE_FILES}
		arch/x86_64/boot.S		
	)
	
	include_directories(
		${CMAKE_CURRENT_SOURCE_DIR}/arch/x86_64
	)
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i[3456]86")
	set(SOURCE_FILES ${SOURCE_FILES}
		arch/i686/boot.S		
	)
	
	include_directories(
		${CMAKE_CURRENT_SOURCE_DIR}/arch/i686
	)
endif()


add_definitions(-D__KERNEL__)

add_executable (kernel.elf ${SOURCE_FILES})

set_property(TARGET kernel.elf PROPERTY CXX_STANDARD 11)

# custom linker command for the kernel
add_custom_command(TARGET kernel.elf POST_BUILD
    COMMAND nm -C kernel.elf | sort | uniq > kernel.sym
	COMMAND objdump -C -d -S --no-show-raw-insn -mi386:x86-64:intel kernel.elf > kernel.lst
	COMMAND objdump -C -d -mi386:x86-64:intel kernel.elf > kernel.asm
	COMMAND strip kernel.elf
)
