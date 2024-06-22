cmake_minimum_required(VERSION 3.22)
include(FetchContent)

#### Compiler configuration. Should be done before project declaration.
set(MILKV_DUO_SDK ${CMAKE_CURRENT_SOURCE_DIR}/duo-sdk)
set(SYSROOT ${MILKV_DUO_SDK}/rootfs)

# CLion on Mac can not configure project properly with risk-v compiler, so have to omit this by setting CLION=1 env var
# in CLion build configuration
if(DEFINED ENV{CLION})
    message("Building for CLION: not setting RISK-V compiler flags")
else ()
    message("Building for RISK-V")
    set(CMAKE_C_FLAGS "-mcpu=c906fdv -march=rv64imafdcv0p7xthead -mcmodel=medany -mabi=lp64d")
    set(CMAKE_CXX_FLAGS "-mcpu=c906fdv -march=rv64imafdcv0p7xthead -mcmodel=medany -mabi=lp64d")
    set(CMAKE_EXE_LINKER_FLAGS "-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64")

    set(TOOLCHAIN_DIR ${MILKV_DUO_SDK}/riscv64-linux-musl-x86_64)
    set(TOOLCHAIN_PREFIX ${TOOLCHAIN_DIR}/bin/riscv64-unknown-linux-musl-)
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
endif ()

#### Project declaration
project(RaceComputer VERSION 0.1 LANGUAGES C CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(
        -D_POSIX_C_SOURCE=199309L
        -DLOG_USE_COLOR
        -Dtimegm=mktime
        -g
        ${WARNINGS}
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

#### Dependencies
set(DEPENDENCIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
if (EXISTS ${DEPENDENCIES_DIR})
    message("Dependencies directory exists, disabling external libraries download. Dir: ${DEPENDENCIES_DIR}")
    set(FETCHCONTENT_FULLY_DISCONNECTED ON)
else ()
    message("Dependencies directory does not exist, downloading external libraries. Dir: ${DEPENDENCIES_DIR}")
endif ()

## minmea
FetchContent_Declare(
        minmea
        GIT_REPOSITORY https://github.com/kosma/minmea.git
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/minmea/src
        BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/minmea/build
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
)
FetchContent_GetProperties(minmea)
if(NOT minmea_POPULATED)
    FetchContent_Populate(minmea)
endif()
add_library(minmea_lib STATIC ${minmea_SOURCE_DIR}/minmea.c)
target_include_directories(minmea_lib PUBLIC ${minmea_SOURCE_DIR})

## spdlog
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.x
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/spdlog/src
        BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/spdlog/build
)
FetchContent_MakeAvailable(spdlog)

## lvgl
set (LV_CONF_BUILD_DISABLE_EXAMPLES ON)
set (LV_CONF_BUILD_DISABLE_THORVG_INTERNAL ON)
set (LV_CONF_BUILD_DISABLE_DEMOS ON)
set(LV_CONF_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h
        CACHE STRING "" FORCE)
FetchContent_Declare(
        lvgl
        GIT_REPOSITORY https://github.com/lvgl/lvgl.git
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lvgl/src
        BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lvgl/build
)
FetchContent_MakeAvailable(lvgl)

#### Sources ####
file(GLOB_RECURSE ASRCS "src/*.S")
file(GLOB_RECURSE CSRCS "src/*.c")
file(GLOB_RECURSE CXXSRCS "src/*.cpp")

add_executable(${PROJECT_NAME} ${ASRCS} ${CSRCS} ${CXXSRCS})

#### Target config ####
target_include_directories(${PROJECT_NAME} PRIVATE ${SYSROOT}/usr/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${minmea_SOURCE_DIR})

target_link_directories(${PROJECT_NAME} PRIVATE ${SYSROOT}/lib ${SYSROOT}/usr/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE minmea_lib)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE wiringx)
target_link_libraries(${PROJECT_NAME} PRIVATE lvgl::lvgl)

target_compile_definitions(${PROJECT_NAME} PRIVATE _POSIX_C_SOURCE=199309L LOG_USE_COLOR timegm=mktime)
target_compile_options(${PROJECT_NAME} PRIVATE -g ${WARNINGS})

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        OUTPUT_NAME "rc"
)

add_custom_target(clean-all
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}
)