cmake_minimum_required(VERSION 3.7)

project(TemplateExample)

set(BINARY_NAME target)

# define CPOP_DIR if needed

find_package(CPOP CONFIG REQUIRED)
foreach(USE_FILE ${CPOP_USE_FILE})
	message(STATUS "Including ${USE_FILE}")
	include(${USE_FILE})
endforeach(USE_FILE)

set(ALL_SOURCE
)

set(ALL_HEADER
)

add_executable(${BINARY_NAME} ${ALL_SOURCE} ${ALL_HEADER})

target_link_libraries(${BINARY_NAME} PUBLIC
	${CPOP_LIB}
)
