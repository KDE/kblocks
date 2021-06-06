set(kblockscore_SRCS
    AI/KBlocksAIEvaluation.cpp
    AI/KBlocksAIFeature.cpp
    AI/KBlocksAILog.cpp
    AI/KBlocksAIPlanner.cpp
    AI/KBlocksAIPlannerExtend.cpp
    AI/KBlocksAIPlayer.cpp
    GameLogicInterface.cpp
    KBlocksAnimator.cpp
    KBlocksAnimDrop.cpp
    KBlocksAnimFade.cpp
    KBlocksAppThread.cpp
    KBlocksConfigManager.cpp
    KBlocksDefine.cpp
    KBlocksDisplay.cpp
    KBlocksDummyAI.cpp
    KBlocksField.cpp
    KBlocksGameLogic.cpp
    KBlocksGameMessage.cpp
    KBlocksGameRecorder.cpp
    KBlocksGameReplayer.cpp
    KBlocksGraphics.cpp
    KBlocksItemGroup.cpp
    KBlocksKeyboardPlayer.cpp
    KBlocksLayout.cpp
    KBlocksNetClient.cpp
    KBlocksNetPlayer.cpp
    KBlocksNetServer.cpp
    KBlocksPiece.cpp
    KBlocksPieceGenerator.cpp
    KBlocksPlayManager.cpp
    KBlocksPlayNetwork.cpp
    KBlocksRepWin.cpp
    KBlocksScene.cpp
    KBlocksScore.cpp
    KBlocksSingleGame.cpp
    KBlocksSinglePlayer.cpp
    KBlocksSound.cpp
    KBlocksSvgItem.cpp
    KBlocksView.cpp
    KBlocksWin.cpp
    SceneInterface.cpp
    utils.cpp
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
    Qt::Network
    KF5::Crash
    KF5::DBusAddons
    KF5::XmlGui
    KF5::I18n
    Qt::Svg
)

add_subdirectory(Testing)
