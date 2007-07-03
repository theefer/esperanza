TEMPLATE = app
BINDIR = /usr/bin
QT += network xml
include (../../config.pri)

DIALOGS += lastfm medialibdialog \
	   streamingdialog firsttimewizard \
	   preferencesdialog aboutdialog

!win32 {
	for(a, DIALOGS):COMPONENTS+=../dialogs/$$a/lib$${a}.a
}

win32 {
	for(a, DIALOGS):COMPONENTS+=../dialogs/$$a/release/lib$${a}.a
}

COMPONENTS+=../lib/liblib.a ../../data/libdata.a

LIBS += $$COMPONENTS \
	../tools/grepshortcutkeydlg/libgrepshortcutkeydlg.a \
	../tools/globalshortcut/libglobalshortcut.a

PRE_TARGETDEPS=$$COMPONENTS

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
