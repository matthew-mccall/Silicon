project(SiliconDesktop)

add_library(${PROJECT_NAME} OBJECT
            Run.cpp)

add_library(Silicon::Desktop ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC
                      Silicon::Headers
                      SDL2::SDL2)