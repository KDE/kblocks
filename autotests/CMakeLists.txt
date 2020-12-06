enable_testing()

include_directories(AFTER "${CMAKE_CURRENT_SOURCE_DIR}/..")
ecm_add_test(
    basic.cpp
    TEST_NAME basic
    LINK_LIBRARIES Qt5::Test
    NAME_PREFIX "basic-"
)
