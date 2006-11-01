DEPENDPATH += lib ui
INCLUDEPATH += ui lib /sw/include
CONFIG += link_pkgconfig debug
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
PKGCONFIG += xmms2-client xmms2-client-cpp
OBJECTS_DIR = qmake_build
MOC_DIR = qmake_build
RCC_DIR = qmake_build
CONFIG += x86 ppc

TEMPLATE=app
TARGET=esperanza
macx:TARGET=Esperanza
RESOURCES += esperanza.qrc

# library files
HEADERS += lib/playlistmodel.h lib/xmmsqt4.h  lib/xclient.h lib/browsemodel.h lib/medialibsearchmodel.h lib/xmediainfocache.h lib/qtmd5.h lib/growl.h
SOURCES += lib/playlistmodel.cpp lib/xmmsqt4.cpp lib/xclient.cpp lib/browsemodel.cpp lib/medialibsearchmodel.cpp lib/xmediainfocache.cpp lib/qtmd5.cpp lib/growl.cpp

macx:SOURCES += lib/mac_growl.mm
macx:HEADERS += lib/mac_growl.h

# Input
HEADERS += ui/playlistview.h ui/playerwidget.h ui/progressframe.h ui/browsedialog.h ui/playerbutton.h ui/filedialog.h ui/medialibdialog.h ui/medialibview.h ui/serverdialog.h ui/preferences.h ui/progressindicator.h ui/volumebar.h ui/preferencessupport.h ui/textdialog.h ui/systemtray.h
SOURCES += ui/main.cpp ui/playlistview.cpp ui/playerwidget.cpp ui/progressframe.cpp ui/browsedialog.cpp ui/playerbutton.cpp ui/filedialog.cpp ui/medialibdialog.cpp ui/medialibview.cpp ui/serverdialog.cpp ui/preferences.cpp ui/progressindicator.cpp ui/volumebar.cpp ui/preferencessupport.cpp ui/textdialog.cpp ui/systemtray.cpp

