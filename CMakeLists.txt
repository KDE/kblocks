project(kblocks)

if(NOT COMPILING_KDEGAMES)
	find_package(KDE4 REQUIRED)
	include(KDE4Defaults)
	include(MacroLibrary)
	find_package(KDEGames REQUIRED)

	add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
	add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
	include_directories(${QDBUS_INCLUDE_DIRS} ${KDE4_INCLUDES} ${KDEGAMES_INCLUDE_DIRS} ${KDEGAMES_INCLUDE_DIR}/highscore)
endif(NOT COMPILING_KDEGAMES)

add_subdirectory( themes ) 

include_directories( ${CMAKE_SOURCE_DIR}/libkdegames ${CMAKE_SOURCE_DIR}/libkdegames/highscore  )

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

kde4_add_kcfg_files(kblocks_SRCS settings.kcfgc )

kde4_add_app_icon(kblocks_SRCS "hi*-app-kblocks.png")

kde4_add_executable(kblocks ${kblocks_SRCS})

target_link_libraries(kblocks kdegames kdegamesprivate ${KDE4_KDEUI_LIBS} ${KDE4_PHONON_LIBS} ${QT_QTNETWORK_LIBRARY})

install(TARGETS kblocks  ${INSTALL_TARGETS_DEFAULT_ARGS} )

########### install files ###############

install( PROGRAMS kblocks.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kblocks.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kblocksui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kblocks )
install( FILES kblocks.knsrc  DESTINATION  ${CONFIG_INSTALL_DIR} )

kde4_install_icons( ${ICON_INSTALL_DIR}   )



