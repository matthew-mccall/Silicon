project(SiliconWeb)

add_library(${PROJECT_NAME} OBJECT
            Run.cpp)

add_library(Silicon::Web ALIAS ${PROJECT_NAME})

get_target_property(SOURCES ${PROJECT_NAME} SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC
                      Silicon::Headers)