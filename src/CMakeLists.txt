add_library(KBlocksCore STATIC)

target_sources(KBlocksCore PRIVATE
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

ecm_qt_declare_logging_category(KBlocksCore
    HEADER kblocks_ai_debug.h
    IDENTIFIER KBlocksAI
    CATEGORY_NAME kblocks.ai
    OLD_CATEGORY_NAMES KBlocks.AI
    DESCRIPTION "KBlocks AI"
    EXPORT KBLOCKS
)
ecm_qt_declare_logging_category(KBlocksCore
    HEADER kblocks_sound_debug.h
    IDENTIFIER KBSound
    CATEGORY_NAME kblocks.sound
    OLD_CATEGORY_NAMES KBlocks.Sound
    DESCRIPTION "KBlocks Sound"
    EXPORT KBLOCKS
)
ecm_qt_declare_logging_category(KBlocksCore
    HEADER kblocks_replay_debug.h
    IDENTIFIER KBReplay
    CATEGORY_NAME kblocks.replay
    OLD_CATEGORY_NAMES KBlocks.Replay
    DESCRIPTION "KBlocks Replay"
    EXPORT KBLOCKS
)
ecm_qt_declare_logging_category(KBlocksCore
    HEADER kblocks_graphics_debug.h
    IDENTIFIER KBGraphics
    CATEGORY_NAME kblocks.graphics
    OLD_CATEGORY_NAMES KBlocks.Graphics
    DESCRIPTION "KBlocks Graphics"
    EXPORT KBLOCKS
)

kconfig_add_kcfg_files(KBlocksCore config/settings.kcfgc )

target_link_libraries(KBlocksCore
    KDEGames6
    KF6::CoreAddons
    KF6::Crash
    KF6::DBusAddons
    KF6::XmlGui
    KF6::I18n
    Qt6::Network
    Qt6::Svg
    Qt6::SvgWidgets
)

add_subdirectory(Testing)

ecm_setup_version(${RELEASE_SERVICE_VERSION}
    VARIABLE_PREFIX KBLOCKS
    VERSION_HEADER kblocks_version.h
)

add_executable(kblocks
    main.cpp
    kblocks.qrc
)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/*-apps-kblocks.png")
ecm_add_app_icon(kblocks ICONS ${ICONS_SRCS})

target_link_libraries(kblocks
    KBlocksCore
)

install(TARGETS kblocks  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )

install(PROGRAMS org.kde.kblocks.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kblocks.appdata.xml DESTINATION  ${KDE_INSTALL_METAINFODIR})
install(FILES config/kblocks.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR})

ecm_qt_install_logging_categories(
    EXPORT KBLOCKS
    FILE kblocks.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)
