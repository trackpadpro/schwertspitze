cmake_minimum_required(VERSION 3.28.1)

project(schwertspitze)

add_executable(${PROJECT_NAME}
	src/main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}>
)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

add_subdirectory(src)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Box2D REQUIRED)

if(NOT EXISTS sdk)
	if(NOT EXISTS steamworks_sdk.zip)
		message(STATUS "Retrieving Steamworks SDK")
		file(DOWNLOAD 
			https://partner.steamgames.com/downloads/steamworks_sdk.zip 
			steamworks_sdk.zip
			INACTIVITY_TIMEOUT 30
			SHOW_PROGRESS
			TLS_VERIFY ON
		)
	endif()
	if(EXISTS steamworks_sdk.zip)
		message(STATUS "Extracting Steamworks SDK")
		file(ARCHIVE_EXTRACT 
			INPUT steamworks_sdk.zip
			DESTINATION ${CMAKE_SOURCE_DIR}
		)
		file(REMOVE steamworks_sdk.zip)
	else()
		message(WARNING "SDK download failed; confirm that you are logged in to Steamworks")
	endif()
endif()

include_directories(
	src/lib
	sdk/public/steam
	${GLEW_INCLUDE_DIRS}
)

target_link_directories(${PROJECT_NAME} PUBLIC
	$<$<PLATFORM_ID:Windows,MSYS>:${CMAKE_SOURCE_DIR}/sdk/redistributable_bin>
	$<$<PLATFORM_ID:Windows,MSYS>:${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/win64>
	$<$<PLATFORM_ID:Linux>:${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/linux32>
	$<$<PLATFORM_ID:Linux>:${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/linux64>
	$<$<PLATFORM_ID:Darwin>:${CMAKE_SOURCE_DIR}/sdk/redistributable_bin/osx>
	$<$<PLATFORM_ID:Windows,MSYS>:${CMAKE_SOURCE_DIR}/sdk/public/steam/lib/win32>
	$<$<PLATFORM_ID:Windows,MSYS>:${CMAKE_SOURCE_DIR}/sdk/public/steam/lib/win64>
	$<$<PLATFORM_ID:Linux>:${CMAKE_SOURCE_DIR}/sdk/public/steam/lib/linux32>
	$<$<PLATFORM_ID:Linux>:${CMAKE_SOURCE_DIR}/sdk/public/steam/lib/linux64>
	$<$<PLATFORM_ID:Darwin>:${CMAKE_SOURCE_DIR}/sdk/public/steam/lib/osx>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	steam_api$<$<AND:$<PLATFORM_ID:Windows,MSYS>,$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>>:64>
	sdkencryptedappticket$<$<AND:$<PLATFORM_ID:Windows,MSYS>,$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>>:64>
	schwertspitze_object
	schwertspitze_server
	schwertspitze_client
	schwertspitze_input
	${OPENGL_LIBRARY}
	GLEW::GLEW
	glfw
	glm
	Box2D
)

if(NOT EXISTS steam_appid.txt)
	file(COPY sdk/steamworksexample/release/steam_appid.txt
		DESTINATION ${CMAKE_SOURCE_DIR}
	)
endif()

if(WIN32 OR MSYS)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		file(COPY sdk/redistributable_bin/win64/steam_api64.dll
			DESTINATION ${CMAKE_SOURCE_DIR}
		)
		file(COPY sdk/public/steam/lib/win64/sdkencryptedappticket64.dll
			DESTINATION ${CMAKE_SOURCE_DIR}
		)
	else()
		file(COPY sdk/redistributable_bin/steam_api.dll
			DESTINATION ${CMAKE_SOURCE_DIR}
		)
		file(COPY sdk/public/steam/lib/win32/sdkencryptedappticket.dll
			DESTINATION ${CMAKE_SOURCE_DIR}
		)
	endif()
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Linux)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
	target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
endif()

option(DEV_APP_ID "Use Spacewar Steam App ID" ON)
if(DEV_APP_ID)
	add_compile_definitions(STEAM_APP_ID=480)
endif()

option(DEDICATED_SERVER "Toggle build for dedicated server" OFF)
if(DEDICATED_SERVER)
	add_compile_definitions(DEDICATED_SERVER)
endif()
