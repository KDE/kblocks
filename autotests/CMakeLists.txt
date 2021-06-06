enable_testing()

include_directories(AFTER "${CMAKE_CURRENT_SOURCE_DIR}/../src")

ecm_add_test(
    basic.cpp
    TEST_NAME basic
    LINK_LIBRARIES Qt::Test
    NAME_PREFIX "UnitTest-"
)

ecm_add_test(
    test_KBlocksGameLogic.cpp
    TEST_NAME KBlocksGameLogic
    LINK_LIBRARIES Qt::Test KBlocksCore
    NAME_PREFIX "UnitTest-"
)

ecm_add_test(
    test_KBlocksItemGroup.cpp
    TEST_NAME KBlocksItemGroup
    LINK_LIBRARIES Qt::Test KBlocksCore
    NAME_PREFIX "UnitTest-"
)

ecm_add_test(
    test_KBlocksScene.cpp
    TEST_NAME KBlocksScene
    LINK_LIBRARIES Qt::Test KBlocksCore
    NAME_PREFIX "UnitTest-"
)

ecm_add_test(
    test_KBlocksWin.cpp
    TEST_NAME KBlocksWin
    LINK_LIBRARIES Qt::Test KBlocksCore KBlocksTesting
    NAME_PREFIX "UnitTest-"
)

ecm_add_test(
    test_View-Scene.cpp
    TEST_NAME View-Scene
    LINK_LIBRARIES Qt::Test KBlocksCore
    NAME_PREFIX "IntegrationTest-"
)
