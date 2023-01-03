project(SiliconDesktop)

add_library(${PROJECT_NAME} OBJECT
            Run.cpp)

add_library(Silicon::Desktop ALIAS ${PROJECT_NAME})

get_target_property(SOURCES ${PROJECT_NAME} SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC
                      Silicon::Headers
                      SDL2::SDL2)