add_library(KBlocksTesting STATIC
    TestingKBlocksWin.cpp
)
target_link_libraries(KBlocksTesting
    KBlocksCore
)
