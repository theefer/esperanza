DEPENDPATH += $PWD/src/ui $PWD/src/lib
INCLUDEPATH += $$PWD/src/ui $$PWD/src/lib $$PWD/src/dialogs $$PWD/src/tools
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#QT += network xml

macx {
	INCLUDEPATH += /opt/local/include # ugly
	DEFINES += HAVE_DNSSD
	LIBS += -lboost_signals
}

!win32 {
	include (conf.pri)
}

win32 {
	CONFIG += release
	PREFIX += "SET THIS TO YOUR XMMS2 INSTALL DIR"
	INCLUDEPATH += "$$PREFIX/include/xmms2"
	LIBPATH += "$$PREFIX"
	LIBS += -lxmmsclient -lxmmsclient++ -lboost_signals -lws2_32
}

contains(CONFIG, debug) {
	DEFINES += _DEBUG
	# for windows
	CONFIG += console
}
