cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "21")
set(RELEASE_SERVICE_VERSION_MINOR "03")
set(RELEASE_SERVICE_VERSION_MICRO "70")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kblocks VERSION ${RELEASE_SERVICE_VERSION})

set (QT_MIN_VERSION "5.12.0")
set (KF5_MIN_VERSION "5.68.0")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} )

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Test Widgets Svg Network)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Config
    ConfigWidgets
    CoreAddons
    Crash
    DocTools
    DBusAddons
    I18n
    WidgetsAddons
    XmlGui
)

find_package(KF5KDEGames 7.1.0 REQUIRED)

include(FeatureSummary)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMAddAppIcon)
include(ECMAddTests)
include(ECMQtDeclareLoggingCategory)
include(ECMSetupVersion)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
if (${KF5Config_VERSION} STRGREATER "5.56.0")
        add_definitions(-DQT_NO_FOREACH)
        MESSAGE(STATUS "compile without foreach")
endif()

option(BUILD_COVERAGE "Build the project with gcov support" OFF)
if(BUILD_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
endif()

add_subdirectory(themes)
add_subdirectory(sounds)
add_subdirectory(doc)

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()

########### next target ###############

ecm_setup_version(${RELEASE_SERVICE_VERSION} VARIABLE_PREFIX KBLOCKS VERSION_HEADER kblocks_version.h)

file(GLOB ICONS_SRCS "*-apps-kblocks.png")
ecm_add_app_icon(kblocks_SRCS ICONS ${ICONS_SRCS})

add_subdirectory(src)

add_executable(kblocks src/main.cpp)

target_link_libraries(kblocks
    KBlocksCore
)

install(TARGETS kblocks  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )

########### install files ###############

install(PROGRAMS org.kde.kblocks.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kblocks.appdata.xml DESTINATION  ${KDE_INSTALL_METAINFODIR})
install(FILES src/config/kblocks.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR})
install(FILES kblocksui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/kblocks)
install(FILES kblocks.knsrc  DESTINATION  ${KDE_INSTALL_DATADIR}/knsrcfiles)

ecm_install_icons(ICONS 128-apps-kblocks.png  16-apps-kblocks.png  22-apps-kblocks.png  32-apps-kblocks.png  48-apps-kblocks.png  64-apps-kblocks.png DESTINATION ${KDE_INSTALL_ICONDIR} THEME hicolor)

ecm_qt_install_logging_categories(EXPORT KBLOCKS FILE kblocks.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
