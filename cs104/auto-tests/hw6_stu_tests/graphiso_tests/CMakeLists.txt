include_directories(. ${HW_DIR})

add_source_problem(NAME graphiso
	USER_SOURCE ${HW_DIR}/graphiso.cpp
	TEST_SOURCE graphiso_tests.cpp)

set_tests_properties( GraphIso.Graph4bFalse PROPERTIES TIMEOUT 90)
set_tests_properties( GraphIso.Graph4bFalse PROPERTIES WITH_VALGRIND 0)
