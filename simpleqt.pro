DEPENDPATH += lib ui
INCLUDEPATH += ui lib /sw/include
CONFIG += link_pkgconfig debug
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
PKGCONFIG += xmms2-client xmms2-client-cpp
OBJECTS_DIR = qmake_build
MOC_DIR = qmake_build
RCC_DIR = qmake_build

TEMPLATE=app
TARGET=simpleqt
RESOURCES += simpleqt.qrc

# library files
HEADERS += lib/playlistmodel.h lib/xmmsqt4.h  lib/xclient.h lib/browsemodel.h lib/medialibsearchmodel.h lib/xmediainfocache.h
SOURCES += lib/playlistmodel.cpp lib/xmmsqt4.cpp lib/xclient.cpp lib/browsemodel.cpp lib/medialibsearchmodel.cpp lib/xmediainfocache.cpp 

# Input
HEADERS += ui/playlistview.h ui/playerwidget.h ui/progressframe.h ui/browsedialog.h ui/playerbutton.h ui/filedialog.h ui/medialibdialog.h ui/medialibview.h ui/serverdialog.h ui/preferences.h
SOURCES += ui/main.cpp ui/playlistview.cpp ui/playerwidget.cpp ui/progressframe.cpp ui/browsedialog.cpp ui/playerbutton.cpp ui/filedialog.cpp ui/medialibdialog.cpp ui/medialibview.cpp ui/serverdialog.cpp ui/preferences.cpp

