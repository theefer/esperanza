DEPENDPATH += $PWD/src/ui $PWD/src/lib
INCLUDEPATH += $$PWD/src/ui $$PWD/src/lib $$PWD/src/widgets $$PWD/src/dialogs $$PWD/src/tools
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#QT += network xml

macx {
	INCLUDEPATH += /opt/local/include # ugly
	DEFINES += HAVE_DNSSD
	LIBS += -lboost_signals
}

!win32 {
	PKGCONFIG += xmms2-client xmms2-client-cpp
	CONFIG += link_pkgconfig debug
	DIALOGS += ../dialogs/lastfm/liblastfm.a \
			   ../dialogs/medialibdialog/libmedialibdialog.a \
			   ../dialogs/streamingdialog/libstreamingdialog.a \
			   ../dialogs/firsttimewizard/libfirsttimewizard.a \
			   ../dialogs/preferencesdialog/libpreferencesdialog.a \
			   ../dialogs/aboutdialog/libaboutdialog.a

	LIBS += $$DIALOGS ../lib/liblib.a ../../data/libdata.a ../widgets/libwidgets.a \
			../tools/grepshortcutkeydlg/libgrepshortcutkeydlg.a \
			../tools/globalshortcut/libglobalshortcut.a
}

win32 {
	DIALOGS += ../dialogs/lastfm/release/liblastfm.a \
			   ../dialogs/medialibdialog/release/libmedialibdialog.a \
			   ../dialogs/streamingdialog/release/libstreamingdialog.a \
			   ../dialogs/firsttimewizard/release/libfirsttimewizard.a \
			   ../dialogs/preferencesdialog/release/libpreferencesdialog.a \
			   ../dialogs/aboutdialog/release/libaboutdialog.a

	LIBS += $$DIALOGS ../lib/release/liblib.a ../../data/release/libdata.a \
			../widgets/release/libwidgets.a \
			../tools/grepshortcutkeydlg/release/libgrepshortcutkeydlg.a \
			../tools/globalshortcut/release/libglobalshortcut.a

	PREFIX += "SET THIS TO YOUR XMMS2 INSTALL DIR"
	INCLUDEPATH += "$$PREFIX/include/xmms2"
	LIBPATH += "$$PREFIX"
	LIBS += -lxmmsclient -lxmmsclient++ -lboost_signals -lws2_32
}
