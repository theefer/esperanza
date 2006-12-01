	!include "MUI.nsh"

	Name "Esperanza"
	Outfile "esperanza-installer.exe"

	InstallDir "$PROGRAMFILES\XMMS2\Esperanza"
	InstallDirRegKey HKCU "Software\XMMS2\Esperanza" ""

	Var MUI_TEMP
	Var STARTMENU_FOLDER

	!define MUI_HEADERIMAGE
#	!define MUI_HEADERIMAGE_BITMAP "netintact.bmp"
	!define MUI_ABORTWARNING

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
	File	"c:\Qt\4.2.1\bin\mingwm10.dll"
	File	"c:\xmms2\lib\libxmmsclient.dll"
	File	"c:\xmms2\lib\xmmsclient++.dll"
	File	"c:\xmms2\lib\libboost_signals.dll"
	File	"c:\Qt\4.2.1\bin\QtCore4.dll"
	File	"c:\Qt\4.2.1\bin\QtGui4.dll"
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
	
