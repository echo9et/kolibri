cmake_minimum_required(VERSION 3.19)
project(Client LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network)

qt_standard_project_setup()

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)
qt_add_executable(Client
    ${SRC_FILES}
)

target_link_libraries(Client
    PRIVATE
        Qt::Core
        Qt::Network
)

install(TARGETS Client
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
