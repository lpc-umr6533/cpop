# Create the library confReader

set(HEADER_PATH ../include/cReader)

set(HEADERS 
	${HEADER_PATH}/configreader.hh
	${HEADER_PATH}/sectionreader.hh
	${HEADER_PATH}/sectionreaderfactory.hh
	${HEADER_PATH}/zupply.hpp
)

set(LIBRARY_NAME cReader)


add_library(${LIBRARY_NAME} ${LIB_TYPE} zupply.cpp ${HEADERS})

# Set the build version. It will be used in the name of the lib, with corresponding
# symlinks created. SOVERSION could also be specified for api version. 
set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
  FRAMEWORK FALSE
  PUBLIC_HEADER "${HEADERS}" # specify the public headers
)

# Says how and where to install software
# Targets:
#   * <prefix>/lib/<libraries>
#   * header location after install: <prefix>/include/<project>/*.h
#   * headers can be included by C++ code `#<project>/Bar.hpp>`
install(TARGETS ${LIBRARY_NAME}
  EXPORT ${CPOP_EXPORT}                   # for downstream dependencies
  ARCHIVE DESTINATION CPOP/lib COMPONENT libs   # static lib
  RUNTIME DESTINATION CPOP/bin COMPONENT libs   # binaries
  LIBRARY DESTINATION CPOP/lib COMPONENT libs   # shared lib
  FRAMEWORK DESTINATION CPOP/bin COMPONENT libs # for mac
  PUBLIC_HEADER DESTINATION include/CPOP/cReader COMPONENT devel   # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION include/CPOP             # headers
)

