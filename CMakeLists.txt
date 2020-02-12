cmake_minimum_required(VERSION 3.0.0)
project(Program VERSION 0.1.0)

# include(CTest)
# enable_testing()

find_package(Boost REQUIRED)
set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS         OFF) # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
include_directories(${Boost_INCLUDE_DIRS}) 

find_package(Qt5 REQUIRED COMPONENTS Core)

add_executable(Program program.cpp)
target_link_librarieS(${PROJECT_NAME} Qt5::Core)
# target_link_librarieS(${PROJECT_NAME} Boost::lexical_cast)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
