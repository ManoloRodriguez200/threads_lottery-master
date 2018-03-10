cmake_minimum_required(VERSION 3.3)
project(threads_lottery)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall ")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(Threads REQUIRED)
find_package(GTK3 REQUIRED)
include_directories(${GTK3_INCLUDE_DIRS})

set(SOURCE_FILES main.c ThreadWork.c ThreadWork.h Thread.c LoteryScheduler.c Timer.c Timer.h Thread_Callbacks.c Thread_Callbacks.h FileLoader.c FileLoader.h ProgressbarList.c ProgressbarList.h Viewer.c Viewer.h Viewer.callbacks.c Viewer.callbacks.h SharedState.h SharedState.c)
add_executable(threads_lottery ${SOURCE_FILES})

set(LIBS ${LIBS} ${GTK3_LIBRARIES} Threads::Threads)
target_link_libraries(threads_lottery ${LIBS})

add_custom_command(TARGET threads_lottery PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/ui $<TARGET_FILE_DIR:threads_lottery>/ui)
add_custom_command(TARGET threads_lottery PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/settings $<TARGET_FILE_DIR:threads_lottery>/settings)

install(TARGETS threads_lottery DESTINATION bin)
SET(CPACK_GENERATOR "TGZ")
include(CPack)