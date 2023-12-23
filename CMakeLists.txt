cmake_minimum_required(VERSION 3.28.1)

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
	$<$<PLATFORM_ID:Windows,MSYS,CYGWIN>:${CMAKE_SOURCE_DIR}/sdk/redistributable_bin>
	$<$<PLATFORM_ID:Windows,MSYS,CYGWIN>:${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/win64>
	$<$<PLATFORM_ID:Linux>:${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/linux32>
	$<$<PLATFORM_ID:Linux>:${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/linux64>
	$<$<PLATFORM_ID:Darwin>:${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/osx>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	#sfml-graphics
	#sfml-network
	#sfml-audio
	$<IF:$<AND:$<PLATFORM_ID:Windows,MSYS,CYGWIN>,$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>>,steam_api64,steam_api>
	objects
)

#if(HOST_SERVER)
#	target_link_libraries(server PRIVATE
#		sfml-network
#	)
#endif()
