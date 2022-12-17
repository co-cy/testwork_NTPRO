cmake_minimum_required(VERSION 3.21)
project(testwork_NTPRO VERSION 0.1)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories(src)

set(SHARED_FILES
        src/Request/Request.h
        src/Response/Response.h
        src/Common.h
        src/User.h)

set(CLIENT_SOURCES
        src/Client/auth.h
        src/Client/auth.cpp
        src/Client/auth.ui
        src/Client/main.cpp
        src/Client/Helpers.cpp
        src/Client/Helpers.h
        src/Client/trade.cpp
        src/Client/trade.h
        src/Client/trade.ui
        )

add_executable(ClientOld
        ${SHARED_FILES}
        src/ClientOld/Client.cpp src/ClientOld/Client.h src/ClientOld/main.cpp)

add_executable(Server
        ${SHARED_FILES}
        src/Server/Server.cpp src/Server/Server.h src/Server/main.cpp src/UserBase.h)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Client
            MANUAL_FINALIZATION
            ${CLIENT_SOURCES}
            )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET ClientOld APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else ()
    if (ANDROID)
        add_library(Client SHARED
                ${CLIENT_SOURCES}
                )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(Client
                ${CLIENT_SOURCES}
                )
    endif ()
endif ()

target_link_libraries(Client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(Client PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Client)
endif ()

