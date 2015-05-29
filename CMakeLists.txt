project(kblocks)

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.3.0")

find_package(ECM 1.3.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets Qml Quick QuickWidgets Svg Test)
find_package(KF5 REQUIRED COMPONENTS 
    CoreAddons
    Config
    ItemModels
    WidgetsAddons
    WindowSystem
    Config
    DBusAddons
    I18n
    ConfigWidgets
    TextWidgets
    XmlGui
    KIO
    NotifyConfig
    NewStuff
    )

find_package(KF5KDEGames 4.9.0 REQUIRED)
find_package(Phonon4Qt5 CONFIG REQUIRED)

include_directories(BEFORE ${PHONON_INCLUDES})

include(FeatureSummary)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)


add_subdirectory( themes )
add_subdirectory( doc )


########### next target ###############

set(kblocks_SRCS
   main.cpp
   KBlocksDefine.cpp
   KBlocksNetServer.cpp
   KBlocksNetClient.cpp
   KBlocksConfigManager.cpp
   KBlocksWin.cpp
   KBlocksDisplay.cpp
   KBlocksRepWin.cpp
   KBlocksAppThread.cpp
   KBlocksScene.cpp
   KBlocksView.cpp
   KBlocksScore.cpp
   KBlocksGraphics.cpp
   KBlocksSound.cpp
   KBlocksItemGroup.cpp
   KBlocksSvgItem.cpp
   KBlocksAnimator.cpp
   KBlocksAnimFade.cpp
   KBlocksAnimDrop.cpp
   KBlocksPlayManager.cpp
   KBlocksPlayNetwork.cpp
   KBlocksSinglePlayer.cpp
   KBlocksNetPlayer.cpp
   KBlocksKeyboardPlayer.cpp
   KBlocksDummyAI.cpp
   KBlocksLayout.cpp
   KBlocksGameLogic.cpp
   KBlocksSingleGame.cpp
   KBlocksField.cpp
   KBlocksPiece.cpp
   KBlocksPieceGenerator.cpp
   KBlocksGameMessage.cpp
   KBlocksGameRecorder.cpp
   KBlocksGameReplayer.cpp
   AI/KBlocksAIPlayer.cpp
   AI/KBlocksAIPlanner.cpp
   AI/KBlocksAIPlannerExtend.cpp
   AI/KBlocksAIEvaluation.cpp
   AI/KBlocksAIFeature.cpp
   AI/KBlocksAILog.cpp   )

kconfig_add_kcfg_files(kblocks_SRCS settings.kcfgc )

kde4_add_app_icon(kblocks_SRCS "hi*-apps-kblocks.png")

add_executable(kblocks ${kblocks_SRCS})

target_link_libraries(kblocks KF5KDEGames KF5KDEGamesPrivate  Phonon::phonon4qt5 Qt5::Network KF5::XmlGui KF5::I18n Qt5::Svg)

install(TARGETS kblocks  ${INSTALL_TARGETS_DEFAULT_ARGS} )

########### install files ###############

install( PROGRAMS org.kde.kblocks.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kblocks.appdata.xml DESTINATION  ${DATA_INSTALL_DIR}/appdata )
install( FILES kblocks.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kblocksui.rc  DESTINATION  ${KXMLGUI_INSTALL_DIR}/kblocks )
install( FILES kblocks.knsrc  DESTINATION  ${CONFIG_INSTALL_DIR} )

ecm_install_icons( ICONS hi128-apps-kblocks.png  hi16-apps-kblocks.png  hi22-apps-kblocks.png  hi32-apps-kblocks.png  hi48-apps-kblocks.png  hi64-apps-kblocks.png DESTINATION ${ICON_INSTALL_DIR}   )


feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
