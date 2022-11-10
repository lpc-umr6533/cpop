cmake_minimum_required(VERSION 3.7)

project(PgaTest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake) # main (top) cmake dir

include(ExternalDependencies)
include(InformationSystem)
include(cReader)
include(MAS)
include(Modeler)

set(PROJECT_SOURCE
	main.cc
	test.cc
)

set(PROJECT_HEADER
)

set(test_name PgaTest)

add_executable(${test_name} ${PROJECT_SOURCE} ${PROJECT_HEADER})

target_compile_options(${test_name} PRIVATE -Wall -pthread)
target_link_libraries(${test_name} PUBLIC
	cReader
	InformationSystem
	Platform_SMA
	Modeler
	CGAL
	CLHEP::CLHEP
	Qt5::Core
	Qt5::Xml ${Geant4_LIBRARIES} pthread
)

set(FILE_TO_COPY population.xml init.mac eSpectrum_550um.spec nano.mac both.mac)

foreach(FILE ${FILE_TO_COPY})
	add_custom_command(TARGET ${test_name} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/${FILE}
		$<TARGET_FILE_DIR:${test_name}>
	)
endforeach()

include(CTest)
add_test(NAME PgaCTEST COMMAND ${test_name})
set_tests_properties(PgaCTEST PROPERTIES PASS_REGULAR_EXPRESSION "All tests passed")
