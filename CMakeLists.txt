project(kblocks)

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.3.0")

find_package(ECM 1.7.0 REQUIRED CONFIG)
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
include(ECMAddAppIcon)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DTRANSLATION_DOMAIN="kblocks")


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

file(GLOB ICONS_SRCS "*-apps-kblocks.png")
ecm_add_app_icon(kblocks_SRCS ICONS ${ICONS_SRCS})

add_executable(kblocks ${kblocks_SRCS})

target_link_libraries(kblocks KF5KDEGames KF5KDEGamesPrivate  Phonon::phonon4qt5 Qt5::Network KF5::XmlGui KF5::I18n Qt5::Svg)

install(TARGETS kblocks  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )

########### install files ###############

install( PROGRAMS org.kde.kblocks.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES kblocks.appdata.xml DESTINATION  share/appdata )
install( FILES kblocks.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR} )
install( FILES kblocksui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/kblocks )
install( FILES kblocks.knsrc  DESTINATION  ${KDE_INSTALL_CONFDIR} )

ecm_install_icons( ICONS 128-apps-kblocks.png  16-apps-kblocks.png  22-apps-kblocks.png  32-apps-kblocks.png  48-apps-kblocks.png  64-apps-kblocks.png DESTINATION ${KDE_INSTALL_ICONDIR} THEME hicolor   )


feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
