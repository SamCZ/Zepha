file (GLOB_RECURSE ZEUS_TEST_FILES "*.cpp")
list(FILTER ZEUS_TEST_FILES  EXCLUDE REGEX "Main.cpp")

add_library (zeusTest ${ZEUS_TEST_FILES})