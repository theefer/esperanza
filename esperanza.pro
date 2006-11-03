DEPENDPATH += lib ui
INCLUDEPATH += ui lib /sw/include
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

TEMPLATE = app
TARGET = esperanza
RESOURCES += esperanza.qrc

macx {
	TARGET = Esperanza
;	CONFIG += x86 ppc
	SOURCES += lib/mac_growl.mm
	HEADERS += lib/mac_growl.h
	RC_FILE += images/esperanza.icns
}

!win32 {
	PKGCONFIG += xmms2-client xmms2-client-cpp
	CONFIG += link_pkgconfig debug
	OBJECTS_DIR = qmake_build
	MOC_DIR = qmake_build
	RCC_DIR = qmake_build
}

win32 {
	INCLUDEPATH += c:/xmms2/includes
	LIBPATH += c:/xmms2/lib
	LIBS += -lxmmsclient -lxmmsclient++ -lboost_signals -lws2_32
	RCFILE = esperanza.rc
}

# library files
HEADERS += lib/playlistmodel.h lib/xmmsqt4.h  lib/xclient.h lib/browsemodel.h lib/medialibsearchmodel.h lib/xmediainfocache.h lib/qtmd5.h lib/growl.h
SOURCES += lib/playlistmodel.cpp lib/xmmsqt4.cpp lib/xclient.cpp lib/browsemodel.cpp lib/medialibsearchmodel.cpp lib/xmediainfocache.cpp lib/qtmd5.cpp lib/growl.cpp

# Input
HEADERS += ui/playlistview.h ui/playerwidget.h ui/progressframe.h ui/browsedialog.h ui/playerbutton.h ui/filedialog.h ui/medialibdialog.h ui/medialibview.h ui/serverdialog.h ui/preferences.h ui/progressindicator.h ui/volumebar.h ui/preferencessupport.h ui/textdialog.h ui/systemtray.h ui/infowindow.h ui/infotabmediadata.h
SOURCES += ui/main.cpp ui/playlistview.cpp ui/playerwidget.cpp ui/progressframe.cpp ui/browsedialog.cpp ui/playerbutton.cpp ui/filedialog.cpp ui/medialibdialog.cpp ui/medialibview.cpp ui/serverdialog.cpp ui/preferences.cpp ui/progressindicator.cpp ui/volumebar.cpp ui/preferencessupport.cpp ui/textdialog.cpp ui/systemtray.cpp ui/infowindow.cpp ui/infotabmediadata.cpp

