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

kde4_add_executable(kblocks ${kblocks_SRCS})

target_link_libraries(kblocks  ${KDE4_KDEUI_LIBS} kdegames )

install(TARGETS kblocks  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kblocks.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES kblocks.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kblocksui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kblocks )

kde4_install_icons( ${ICON_INSTALL_DIR}   )


