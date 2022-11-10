cmake_minimum_required(VERSION 3.7)

project(PopulationTest)

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

set(test_name PopulationTest)
add_executable(${test_name} ${PROJECT_SOURCE} ${PROJECT_HEADER})
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

set(FILE_TO_COPY
	population.xml
	verbose.mac
	population.mac
	facet.mac
	delta.mac
	internalRatio.mac
	intermediaryRatio.mac
	sampling.mac
	init.mac
)

foreach(FILE ${FILE_TO_COPY})
	add_custom_command(TARGET ${test_name} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_SOURCE_DIR}/${FILE}
		$<TARGET_FILE_DIR:${test_name}>
	)
endforeach()

include(CTest)
add_test(NAME PopulationCTEST COMMAND ${test_name})
set_tests_properties(PopulationCTEST PROPERTIES PASS_REGULAR_EXPRESSION "All tests passed")
