cmake_minimum_required(VERSION 3.15)
project(Red)			

set(COMPILER "Visual Studio 16")

execute_process(COMMAND conan remote add sfml:bincrafters https://api.bintray.com/conan/bincrafters/public-conan)
execute_process(COMMAND conan install ./src -if ./build --build missing)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(SRC_LIST
	${CMAKE_CURRENT_SOURCE_DIR}/App.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Drawer/Drawer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Drawer/Line.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Drawer/DrawerContainer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Drawer/Forces.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Json/Json_Parser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Network/Network_manager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/Data_manager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/Event_game.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/Game_object.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/Graph.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/Graph_Line.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/Graph_Point.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/MapLayer1.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/MapLayer10.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/Market.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/Player.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/Post.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/Rating.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/Storage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/Town.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Data_manager/Train.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Regulator/Regulator.cpp
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/headers/Drawer
	${CMAKE_CURRENT_SOURCE_DIR}/headers/Json
	${CMAKE_CURRENT_SOURCE_DIR}/headers/Network
	${CMAKE_CURRENT_SOURCE_DIR}/headers/Data_manager
	${CMAKE_CURRENT_SOURCE_DIR}/headers/Regulator
	${CMAKE_CURRENT_SOURCE_DIR}/headers)


add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
