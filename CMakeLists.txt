cmake_minimum_required(VERSION 3.5)

set(PROJECT SRDController)
project(${PROJECT} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS 
    Widgets
    SerialPort
)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Widgets
    SerialPort
)

set(QUDEVICE_DIR ${CMAKE_SOURCE_DIR} CACHE PATH "Location of QUDevice library")

set(PROJECT_SOURCES
    src/main.cpp
    src/can.h
    src/shareddata.h
    src/devicecontroller.cpp
    src/devicecontroller.h

    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
    src/ui/controlgroupbox.cpp
    src/ui/controlgroupbox.h

    src/ui/widgets/movableitemwrapper.cpp
    src/ui/widgets/movableitemwrapper.h
    src/ui/widgets/itemwrapper.cpp
    src/ui/widgets/itemwrapper.h
    src/ui/widgets/gridscene.cpp
    src/ui/widgets/gridscene.h
    src/ui/widgets/gridwidget.cpp
    src/ui/widgets/gridwidget.h
)

set(RESOURCES
    icons/icons.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${RESOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${PROJECT} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT} SHARED
            ${PROJECT_SOURCES}
            ${RESOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT}
            ${PROJECT_SOURCES}
            ${RESOURCES}
        )
    endif()
endif()

target_compile_definitions(${PROJECT} PRIVATE
    CAN_BIG_ENDIAN
)

add_library(quasyncserialport SHARED IMPORTED)

set_property(TARGET quasyncserialport PROPERTY
    IMPORTED_LOCATION   ${QUDEVICE_DIR}/${CMAKE_BUILD_TYPE}/bin/libquasyncserialportd.dll
    IMPORTED_IMPLIB     ${QUDEVICE_DIR}/${CMAKE_BUILD_TYPE}/lib/libquasyncserialportd.dll.a
)

target_compile_definitions(${PROJECT} PRIVATE
    QUDEVICE_SHARED
)

target_include_directories(${PROJECT} PRIVATE
    ${QUDEVICE_DIR}/${CMAKE_BUILD_TYPE}/include
    ${QUDEVICE_DIR}/${CMAKE_BUILD_TYPE}/include/quasyncserialport
)

target_link_directories(${PROJECT} PRIVATE
    ${QUDEVICE_DIR}/${CMAKE_BUILD_TYPE}/bin
)

target_link_libraries(${PROJECT} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::SerialPort
    libquasyncserialportd.dll
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT})
endif()
set_target_properties(${PROJECT} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(
    TARGET ${PROJECT} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/src/can.json"
            "${CMAKE_CURRENT_BINARY_DIR}/can.json"
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT})
endif()
