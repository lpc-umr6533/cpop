set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake) # main (top) cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir

# include directories
include(InformationSystem)

find_package(Qt5Core REQUIRED)

set(HEADERS
	include/InformationSystemManager.hh
)

set(SOURCES
	src/InformationSystemManager.cc
)

set(LIBRARY_NAME InformationSystem)
add_library(${LIBRARY_NAME} ${LIB_TYPE} ${SOURCES})

set_target_properties(${LIBRARY_NAME} PROPERTIES
	VERSION ${VERSION} # defined in the main CMakeLists.txt
	FRAMEWORK FALSE
	PUBLIC_HEADER "${HEADERS}" # specify the public headers
	DEBUG_POSTFIX "_d"
)

target_compile_options(${LIBRARY_NAME} PRIVATE -Wall -pipe -march=native)
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_17)
target_link_libraries(${LIBRARY_NAME} PUBLIC Qt5::Core)

# Says how and where to install software
# Targets:
#   * <prefix>/lib/<libraries>
#   * header location after install: <prefix>/include/<project>/*.h
#   * headers can be included by C++ code `#<project>/Bar.hpp>`
install(TARGETS ${LIBRARY_NAME}
	EXPORT ${CPOP_EXPORT}                         # for downstream dependencies
	ARCHIVE DESTINATION CPOP/lib COMPONENT libs   # static lib
	RUNTIME DESTINATION CPOP/bin COMPONENT libs   # binaries
	LIBRARY DESTINATION CPOP/lib COMPONENT libs   # shared lib
	FRAMEWORK DESTINATION CPOP/bin COMPONENT libs # for mac
	PUBLIC_HEADER DESTINATION include/CPOP COMPONENT devel   # headers for mac (note the different component -> different package)
	INCLUDES DESTINATION include/CPOP            # headers
)
