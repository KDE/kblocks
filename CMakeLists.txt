project(kblocks)

add_subdirectory( themes ) 

include_directories( ${CMAKE_SOURCE_DIR}/libkdegames ${CMAKE_SOURCE_DIR}/libkdegames/highscore  )


########### next target ###############

set(kblocks_SRCS 
   main.cpp 
   kblocks.cpp 
   kblocksview.cpp 
   kblocksscene.cpp 
   kblocksgraphics.cpp 
   block.cpp 
   blockanimator.cpp )

kde4_add_kcfg_files(kblocks_SRCS settings.kcfgc )

kde4_add_app_icon(kblocks_SRCS "hi*-app-kblocks.png")

kde4_add_executable(kblocks ${kblocks_SRCS})

target_link_libraries(kblocks kdegames ${KDE4_KNEWSTUFF2_LIBS} ${KDE4_KDEUI_LIBS} )

install(TARGETS kblocks  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( FILES kblocks.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kblocks.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kblocksui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kblocks )
install( FILES kblocks.knsrc  DESTINATION  ${CONFIG_INSTALL_DIR} )

kde4_install_icons( ${ICON_INSTALL_DIR}   )



