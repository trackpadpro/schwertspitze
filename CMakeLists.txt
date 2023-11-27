cmake_minimum_required(VERSION 3.22.1)

project(schwertspitze)

add_executable(${PROJECT_NAME}
	src/client.cpp
)

option(HOST_SERVER "Toggle build for server executable" OFF)
if(HOST_SERVER)
	add_executable(server
		src/server.cpp
	)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}>
)

add_subdirectory(src)

set(BUILD_SHARED_LIBS TRUE)
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5 COMPONENTS graphics network audio REQUIRED)

include_directories(
	src/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	sfml-graphics
	sfml-network
	sfml-audio
)

if(HOST_SERVER)
	target_link_libraries(server PRIVATE
		sfml-network
	)
endif()

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Linux)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
	target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
	if(HOST_SERVER)
		target_link_libraries(server PRIVATE Threads::Threads)
	endif()
endif()
