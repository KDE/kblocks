enable_testing()

include_directories(AFTER "${CMAKE_CURRENT_SOURCE_DIR}/../src")

ecm_add_test(
    basic.cpp
    TEST_NAME basic
    LINK_LIBRARIES Qt5::Test
    NAME_PREFIX "UnitTest-"
)

ecm_add_test(
    test_KBlocksItemGroup.cpp
    TEST_NAME KBlocksItemGroup
    LINK_LIBRARIES Qt5::Test KBlocksCore
    NAME_PREFIX "UnitTest-"
)

ecm_add_test(
    test_KBlocksWin.cpp
    TEST_NAME KBlocksWin
    LINK_LIBRARIES Qt5::Test KBlocksCore
    NAME_PREFIX "UnitTest-"
)
