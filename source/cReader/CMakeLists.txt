#------------------------------------------------------------------------------
# CMakeLists.txt
# Module : cReader
# Package: CPOP
#
# $Date$ 05/2018
# $Revision$ 0
# $Author$ Delsol Axel
#------------------------------------------------------------------------------

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake) # main (top) cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir

# include directories
include(cReader)

# Create the library confReader
set(HEADERS_PATH include/cReader)

set(HEADERS
	${HEADERS_PATH}/configreader.hh
	${HEADERS_PATH}/sectionreader.hh
	${HEADERS_PATH}/sectionreaderfactory.hh
	${HEADERS_PATH}/zupply.hpp
)

set(LIBRARY_NAME cReader)

add_library(${LIBRARY_NAME} ${LIB_TYPE} src/zupply.cpp ${HEADERS})

# Set the build version. It will be used in the name of the lib, with corresponding
# symlinks created. SOVERSION could also be specified for api version.
set_target_properties(${LIBRARY_NAME} PROPERTIES
	VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
	FRAMEWORK FALSE
	PUBLIC_HEADER "${HEADERS}" # specify the public headers
)
target_compile_options(${LIBRARY_NAME} PUBLIC -pipe -march=native)
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_17)

# Says how and where to install software
# Targets:
#   * <prefix>/lib/<libraries>
#   * header location after install: <prefix>/include/<project>/*.h
#   * headers can be included by C++ code `#<project>/bar.hpp>`
install(TARGETS ${LIBRARY_NAME}
	EXPORT ${CPOP_EXPORT}                         # for downstream dependencies
	ARCHIVE DESTINATION CPOP/lib COMPONENT libs   # static lib
	RUNTIME DESTINATION CPOP/bin COMPONENT libs   # binaries
	LIBRARY DESTINATION CPOP/lib COMPONENT libs   # shared lib
	FRAMEWORK DESTINATION CPOP/bin COMPONENT libs # for mac
	PUBLIC_HEADER DESTINATION include/CPOP/cReader COMPONENT devel   # headers for mac (note the different component -> different package)
	INCLUDES DESTINATION include/CPOP             # headers
)
