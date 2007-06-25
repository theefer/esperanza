TEMPLATE = app
QT += network xml

DIALOGS += ../dialogs/lastfm/liblastfm.a \
		   ../dialogs/medialibdialog/libmedialibdialog.a \
		   ../dialogs/firsttimewizard/libfirsttimewizard.a

LIBS += $$DIALOGS ../lib/liblib.a ../../data/libdata.a

TARGET = esperanza
macx {
	TARGET = Esperanza
	SOURCES += mac_growl.mm
	HEADERS += mac_growl.h
}

SOURCES += 	fancyplaylistview.cpp \
			minimode.cpp \
			preferences.cpp \
			shortcutmanager.cpp \
			filedialog.cpp \
			panebrowserview.cpp \
			preferencessupport.cpp \
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
			preferences.h \
			preferencessupport.h \
			progressframe.h \
			progressindicator.h \
			serverdialog.h \
			shortcutmanager.h \
			systemtray.h \
			volumebar.h

include (globalshortcut/globalshortcut.pri)
include (../../config.pri)
