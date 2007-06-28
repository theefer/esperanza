TEMPLATE = app
BINDIR = /usr/bin
QT += network xml
include (../../config.pri)

!win32 {
	DIALOGS += ../dialogs/lastfm/liblastfm.a \
			   ../dialogs/medialibdialog/libmedialibdialog.a \
			   ../dialogs/streamingdialog/libstreamingdialog.a \
			   ../dialogs/firsttimewizard/libfirsttimewizard.a \
			   ../dialogs/aboutdialog/libaboutdialog.a
	LIBS += $$DIALOGS ../lib/liblib.a ../../data/libdata.a
}

win32 {
	DIALOGS += ../dialogs/lastfm/release/liblastfm.a \
			   ../dialogs/medialibdialog/release/libmedialibdialog.a \
			   ../dialogs/streamingdialog/release/libstreamingdialog.a \
			   ../dialogs/firsttimewizard/release/libfirsttimewizard.a \
			   ../dialogs/aboutdialog/release/libaboutdialog.a
	LIBS += $$DIALOGS ../lib/release/liblib.a ../../data/release/libdata.a
}

# Target
TARGET = esperanza
win32 {
	DESTDIR = $$PWD/../..
	RC_FILE = $$PWD/esperanza.rc
}

unix:!mac {
	QMAKE_POST_LINK = rm -f ../../esperanza ; ln -s src/ui/esperanza ../../esperanza
	target.path = $$BINDIR
	INSTALLS += target
}



macx {
	TARGET = Esperanza
	SOURCES += mac_growl.mm
	HEADERS += mac_growl.h
}

SOURCES += 	fancyplaylistview.cpp \
			minimode.cpp \
			shortcutmanager.cpp \
			filedialog.cpp \
			panebrowserview.cpp \
			systemtray.cpp \
			growl.cpp \
			playerbutton.cpp \
			progressframe.cpp \
			volumebar.cpp \
			main.cpp \
			playerwidget.cpp \
			progressindicator.cpp \
			mdns.cpp \
			playlistview.cpp \
			serverdialog.cpp

HEADERS += 	esperanza_plugin.h \
			fancyplaylistview.h \
			filedialog.h \
			growl.h \
			mdns.h \
			minimode.h \
			panebrowserview.h \
			playerbutton.h \
			playerwidget.h \
			playlistview.h \
			progressframe.h \
			progressindicator.h \
			serverdialog.h \
			shortcutmanager.h \
			systemtray.h \
			volumebar.h

include (globalshortcut/globalshortcut.pri)
