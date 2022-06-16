set(kblockscore_SRCS
    AI/EvaluationInterface.h
    AI/KBlocksAIEvaluation.cpp
    AI/KBlocksAIEvaluation.h
    AI/KBlocksAIFeature.cpp
    AI/KBlocksAIFeature.h
    AI/KBlocksAILog.cpp
    AI/KBlocksAILog.h
    AI/KBlocksAIPlanner.cpp
    AI/KBlocksAIPlannerExtend.cpp
    AI/KBlocksAIPlannerExtend.h
    AI/KBlocksAIPlanner.h
    AI/KBlocksAIPlayer.cpp
    AI/KBlocksAIPlayer.h
    AI/KBlocksAITypeDefine.h
    AI/PlannerInterface.h
    FieldInterface.h
    GameLogicInterface.cpp
    GameLogicInterface.h
    GamePlayerInterface.h
    GraphicsInterface.h
    KBlocksAnimator.cpp
    KBlocksAnimator.h
    KBlocksAnimDrop.cpp
    KBlocksAnimDrop.h
    KBlocksAnimFade.cpp
    KBlocksAnimFade.h
    KBlocksAppThread.cpp
    KBlocksAppThread.h
    KBlocksConfigManager.cpp
    KBlocksConfigManager.h
    KBlocksDefine.cpp
    KBlocksDefine.h
    KBlocksDisplay.cpp
    KBlocksDisplay.h
    KBlocksDummyAI.cpp
    KBlocksDummyAI.h
    KBlocksField.cpp
    KBlocksField.h
    KBlocksGameLogic.cpp
    KBlocksGameLogic.h
    KBlocksGameMessage.cpp
    KBlocksGameMessage.h
    KBlocksGameRecorder.cpp
    KBlocksGameRecorder.h
    KBlocksGameReplayer.cpp
    KBlocksGameReplayer.h
    KBlocksGraphics.cpp
    KBlocksGraphics.h
    KBlocksItemGroup.cpp
    KBlocksItemGroup.h
    KBlocksKeyboardPlayer.cpp
    KBlocksKeyboardPlayer.h
    KBlocksLayout.cpp
    KBlocksLayout.h
    KBlocksNetClient.cpp
    KBlocksNetClient.h
    KBlocksNetPlayer.cpp
    KBlocksNetPlayer.h
    KBlocksNetServer.cpp
    KBlocksNetServer.h
    KBlocksPiece.cpp
    KBlocksPieceGenerator.cpp
    KBlocksPieceGenerator.h
    KBlocksPiece.h
    KBlocksPlayManager.cpp
    KBlocksPlayManager.h
    KBlocksPlayNetwork.cpp
    KBlocksPlayNetwork.h
    KBlocksRepWin.cpp
    KBlocksRepWin.h
    KBlocksScene.cpp
    KBlocksScene.h
    KBlocksScore.cpp
    KBlocksScore.h
    KBlocksSingleGame.cpp
    KBlocksSingleGame.h
    KBlocksSinglePlayer.cpp
    KBlocksSinglePlayer.h
    KBlocksSound.cpp
    KBlocksSound.h
    KBlocksSvgItem.cpp
    KBlocksSvgItem.h
    KBlocksView.cpp
    KBlocksView.h
    KBlocksWin.cpp
    KBlocksWin.h
    PieceInterface.h
    SceneInterface.cpp
    SceneInterface.h
    SingleGameInterface.h
    SoundInterface.h
    SvgItemInterface.h
    Testing/MockField.h
    Testing/MockGameLogic.h
    Testing/MockGraphics.h
    Testing/MockPiece.h
    Testing/MockScene.h
    Testing/MockSingleGame.h
    Testing/MockSound.h
    Testing/MockSvgItem.h
    Testing/TestingKBlocksItemGroup.h
    Testing/TestingKBlocksWin.cpp
    Testing/TestingKBlocksWin.h
    utils.cpp
    utils.h

)

ecm_qt_declare_logging_category(kblockscore_SRCS HEADER kblocks_ai_debug.h IDENTIFIER KBlocksAI CATEGORY_NAME KBlocks.AI DESCRIPTION "kdegames (kblock ai)" EXPORT KBLOCKS)
ecm_qt_declare_logging_category(kblockscore_SRCS HEADER kblocks_sound_debug.h IDENTIFIER KBSound CATEGORY_NAME KBlocks.Sound DESCRIPTION "kdegames (kblock sound)" EXPORT KBLOCKS)
ecm_qt_declare_logging_category(kblockscore_SRCS HEADER kblocks_replay_debug.h IDENTIFIER KBReplay CATEGORY_NAME KBlocks.Replay DESCRIPTION "kdegames (kblock replay)" EXPORT KBLOCKS)
ecm_qt_declare_logging_category(kblockscore_SRCS HEADER kblocks_graphics_debug.h IDENTIFIER KBGraphics CATEGORY_NAME KBlocks.Graphics DESCRIPTION "kdegames (kblock graphics)" EXPORT KBLOCKS)

kconfig_add_kcfg_files(kblockscore_SRCS config/settings.kcfgc )

add_library(KBlocksCore STATIC ${kblockscore_SRCS})
target_link_libraries(KBlocksCore
    KF5KDEGames
    KF5KDEGamesPrivate
    Qt${QT_MAJOR_VERSION}::Network
    KF5::CoreAddons
    KF5::Crash
    KF5::DBusAddons
    KF5::XmlGui
    KF5::I18n
    Qt${QT_MAJOR_VERSION}::Svg
)
if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(KBlocksCore Qt6::SvgWidgets)
endif()
add_subdirectory(Testing)


ecm_setup_version(${RELEASE_SERVICE_VERSION}
    VARIABLE_PREFIX KBLOCKS
    VERSION_HEADER kblocks_version.h
)

add_executable(kblocks main.cpp)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/*-apps-kblocks.png")
ecm_add_app_icon(kblocks ICONS ${ICONS_SRCS})

target_link_libraries(kblocks
    KBlocksCore
)

install(TARGETS kblocks  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )
