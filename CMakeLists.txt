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

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Linux)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
	target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
	if(HOST_SERVER)
		target_link_libraries(server PRIVATE Threads::Threads)
	endif()
#elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
	
#elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Darwin)
	
#else()
#	message(WARNING "unrecognized system")
endif()

set(BUILD_SHARED_LIBS TRUE)
#set(SFML_STATIC_LIBRARIES TRUE)

#find_package(SFML 2.5 COMPONENTS graphics network audio REQUIRED)

include_directories(
	src/include
	sdk/public/steam
)

target_link_directories(${PROJECT_NAME} PUBLIC
	sdk/redistributable_bin
	#sdk/redistributable_bin/win64
)

add_library(steam_api64 SHARED IMPORTED)
set_target_properties(steam_api64 PROPERTIES 
	IMPORTED_LOCATION "D:/dev/schwertspitze/sdk/redistributable_bin/win64/steam_api64.dll"
)
set_target_properties(steam_api64 PROPERTIES 
	IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/win64/steam_api64.lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	#sfml-graphics
	#sfml-network
	#sfml-audio
	$<0:steam_api>
	steam_api64
	objects
)

#if(HOST_SERVER)
#	target_link_libraries(server PRIVATE
#		sfml-network
#	)
#endif()
