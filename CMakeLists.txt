cmake_minimum_required(VERSION 2.8)

project(test-xlib-xdamage)

find_package(X11 REQUIRED)



add_executable(${PROJECT_NAME} "main.c")

target_include_directories(${PROJECT_NAME} PUBLIC ${X11_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES} ${X11_Xdamage_LIB})
