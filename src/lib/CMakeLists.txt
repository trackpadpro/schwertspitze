add_library(schwertspitze_object object.cpp)
add_library(schwertspitze_server server.cpp)
add_library(schwertspitze_client client.cpp)
add_library(schwertspitze_input input.cpp)

include_directories(
	${CMAKE_SOURCE_DIR}/src/lib
	${CMAKE_SOURCE_DIR}/sdk/public/steam
)
