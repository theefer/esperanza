	!include "MUI.nsh"

	Name "Esperanza"
	Outfile "esperanza-installer.exe"

	InstallDir "$PROGRAMFILES\XMMS2\Esperanza"
	InstallDirRegKey HKCU "Software\XMMS2\Esperanza" ""

	Var MUI_TEMP
	Var STARTMENU_FOLDER

	!define MUI_ICON "images\esperanza.ico"
	!define MUI_UNICON "images\esperanza.ico"
	!define MUI_HEADERIMAGE
	!define MUI_HEADERIMAGE_BITMAP_NOSTRETCH
	!define MUI_HEADERIMAGE_BITMAP "images\logo.bmp"
	!define MUI_ABORTWARNING

Function .onInit
	SetOutPath $TEMP
	File /oname=splash.bmp "images\splash.bmp"
	AdvSplash::show 3000 1000 1000 0xAA9D84 $TEMP\splash
FunctionEnd

	!insertmacro MUI_PAGE_LICENSE "COPYING"
;	!insertmacro MUI_PAGE_COMPONENTS
	!insertmacro MUI_PAGE_DIRECTORY

	!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
	!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\XMMS2\Esperanza"
	!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

	!insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER

	!insertmacro MUI_PAGE_INSTFILES

	!insertmacro MUI_UNPAGE_CONFIRM
	!insertmacro MUI_UNPAGE_INSTFILES

	!insertmacro MUI_LANGUAGE "English"

Section "Esperanza" SecEsperanza
	SetOutPath	"$INSTDIR"

	File	"release\esperanza.exe"
	File	"dlls\mingwm10.dll"
	File	"dlls\libxmmsclient.dll"
	File	"dlls\libxmmsclient++.dll"
	File	"dlls\libboost_signals.dll"
	File	"dlls\QtCore4.dll"
	File	"dlls\QtGui4.dll"
	File	"dlls\QtNetwork4.dll"
	File	"dlls\QtXml4.dll"
	WriteRegStr HKCU "Software\XMMS2\Esperanza" "" $INSTDIR
	WriteUninstaller "$INSTDIR\Uninstall.exe"

	!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
		CreateDirectory	"$SMPROGRAMS\$STARTMENU_FOLDER"
		CreateDirectory	"$SMPROGRAMS\$STARTMENU_FOLDER\XMMS2"
		CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\XMMS2\Esperanza.lnk" "$INSTDIR\esperanza.exe"
		CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\uninstall.exe"
	!insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

	LangString DESC_SecEsperanza ${LANG_ENGLISH} "Esperanza - A hopeful XMMS2 client"

	!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
		!insertmacro MUI_DESCRIPTION_TEXT ${SecEsperanza} $(DESC_SecEsperanza)
	!insertmacro MUI_FUNCTION_DESCRIPTION_END

Section	"Uninstall"
	Delete "$INSTDIR\Uninstall.exe"
	Delete "$INSTDIR\esperanza.exe"

	RMDir	"$INSTDIR"

	!insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP

	Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall.lnk"

	;Delete empty start menu parent diretories
	StrCpy $MUI_TEMP "$SMPROGRAMS\$MUI_TEMP"

	startMenuDeleteLoop:
		ClearErrors
    	RMDir $MUI_TEMP
    	GetFullPathName $MUI_TEMP "$MUI_TEMP\.."

    	IfErrors startMenuDeleteLoopDone

    	StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
	startMenuDeleteLoopDone:

	DeleteRegKey /ifempty HKCU "Software\XMMS2\Esperanza"

SectionEnd

