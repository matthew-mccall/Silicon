project(SiliconWeb)

add_library(${PROJECT_NAME} OBJECT
            Run.cpp)

add_library(Silicon::Web ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC
                      Silicon::Headers)