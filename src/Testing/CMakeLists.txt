set(testing_SRCS
    TestingKBlocksWin.cpp
)

add_library(KBlocksTesting STATIC ${testing_SRCS})
target_link_libraries(KBlocksTesting
    KBlocksCore
)
