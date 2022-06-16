# SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

set(KBLOCKS_INSTALL_THEMESDIR "${KDE_INSTALL_DATADIR}/kblocks/themes")

function(install_theme name)
    cmake_parse_arguments(ARG "" "ID" "OTHER" ${ARGN})
    if (NOT ARG_ID)
        set(ARG_ID "${name}")
    endif()
    set(svgz "${CMAKE_CURRENT_BINARY_DIR}/${name}.svgz")
    generate_svgz(${name}.svg ${svgz} "theme-")
    install(
        FILES
            ${ARG_ID}.desktop
            ${name}_preview.png
            ${svgz}
            ${ARG_OTHER}
        DESTINATION ${KBLOCKS_INSTALL_THEMESDIR}
    )
endfunction()

install_theme(egyptian ID default
    OTHER
        default_block_move.ogg
        default_block_fall.ogg
)
install_theme(oxygen)
