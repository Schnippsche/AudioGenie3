Option Strict Off
Option Explicit On
Module modUni
	
	Public Const DRIVE_UNKNOWN As Short = 0
	Public Const DRIVE_NO_ROOT_DIR As Short = 1
	Public Const DRIVE_REMOVABLE As Short = 2
	Public Const DRIVE_FIXED As Short = 3
	Public Const DRIVE_REMOTE As Short = 4
	Public Const DRIVE_CDROM As Short = 5
	Public Const DRIVE_RAMDISK As Short = 6
	
	Public Const BLOCK_SIZE As Short = 5000
	
	Private Const SE_ERR_NOASSOC As Integer = 31
	Public Const MAX_PATH As Short = 259
	
	Public Const FILE_ATTRIBUTE_ARCHIVE As Integer = &H20
	Public Const FILE_ATTRIBUTE_COMPRESSED As Integer = &H800
	Public Const FILE_ATTRIBUTE_DIRECTORY As Integer = &H10
	Public Const FILE_ATTRIBUTE_HIDDEN As Integer = &H2
	Public Const FILE_ATTRIBUTE_NORMAL As Integer = &H80
	Public Const FILE_ATTRIBUTE_READONLY As Integer = &H1
	Public Const FILE_ATTRIBUTE_SYSTEM As Integer = &H4
	Public Const FILE_ATTRIBUTE_TEMPORARY As Integer = &H100
	
	Public Enum DriveTypes
		dtUnknown = DRIVE_UNKNOWN
		dtRemovable = DRIVE_REMOVABLE
		dtFixed = DRIVE_FIXED
		dtRemote = DRIVE_REMOTE
		dtCDRom = DRIVE_CDROM
		dtRamDisk = DRIVE_RAMDISK
	End Enum
	
	Public Structure FILETIME
		Dim dwLowDateTime As Integer
		Dim dwHighDateTime As Integer
	End Structure
	
	Public Structure WIN32_FIND_DATA
		Dim dwFileAttributes As Integer
		Dim ftCreationTime As FILETIME
		Dim ftLastAccessTime As FILETIME
		Dim ftLastWriteTime As FILETIME
		Dim nFileSizeHigh As Integer
		Dim nFileSizeLow As Integer
		Dim dwReserved0 As Integer
		Dim dwReserved1 As Integer
		'UPGRADE_WARNING: Die Größe der Zeichenfolge mit fester Länge darf die Puffergröße nicht überschreiten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="3C1E4426-0B80-443E-B943-0627CD55D48B"'
		<VBFixedString(MAX_PATH),System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.ByValArray,SizeConst:=MAX_PATH)> Public cFileName() As Char
		'UPGRADE_WARNING: Die Größe der Zeichenfolge mit fester Länge darf die Puffergröße nicht überschreiten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="3C1E4426-0B80-443E-B943-0627CD55D48B"'
		<VBFixedString(14),System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.ByValArray,SizeConst:=14)> Public cAlternate() As Char
	End Structure
	
	Public Structure FILETYPE
		Dim Name As String
		Dim Path As String
		Dim Sort As String ' Filename in Grossbuchstaben
	End Structure
	
	Public Declare Sub InitCommonControls Lib "comctl32" ()
	
	'UPGRADE_WARNING: Für die Struktur IUnknown müssen Marshalling-Attribute möglicherweise als ein Argument in dieser Declare-Anweisung weitergegeben werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="C429C3A5-5D47-4CD9-8F51-74A1616405DC"'
	'UPGRADE_ISSUE: Das Deklarieren eines Parameters als ''As Any'' wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="FAE78A8D-8978-4FD4-8208-5B7324A8F795"'
	Public Declare Function CreateStreamOnHGlobal Lib "ole32.dll" (ByRef hGlobal As Any, ByVal fDeleteOnRelease As Integer, ByRef lpIStream As stdole.IUnknown) As Integer
	'UPGRADE_WARNING: Für die Struktur IPicture müssen Marshalling-Attribute möglicherweise als ein Argument in dieser Declare-Anweisung weitergegeben werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="C429C3A5-5D47-4CD9-8F51-74A1616405DC"'
	'UPGRADE_ISSUE: Das Deklarieren eines Parameters als ''As Any'' wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="FAE78A8D-8978-4FD4-8208-5B7324A8F795"'
	'UPGRADE_WARNING: Für die Struktur IUnknown müssen Marshalling-Attribute möglicherweise als ein Argument in dieser Declare-Anweisung weitergegeben werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="C429C3A5-5D47-4CD9-8F51-74A1616405DC"'
	Public Declare Function OleLoadPicture Lib "oleaut32.dll" (ByVal lpStream As stdole.IUnknown, ByVal lSize As Integer, ByVal fRunmode As Integer, ByRef riid As Any, ByRef lpIPicture As System.Drawing.Image) As Integer
	
	Public Declare Function ActivateWindowTheme Lib "uxtheme"  Alias "SetWindowTheme"(ByVal hWnd As Integer, Optional ByVal pszSubAppName As Integer = 0, Optional ByVal pszSubIdList As Integer = 0) As Integer
	Public Declare Function DeactivateWindowTheme Lib "uxtheme"  Alias "SetWindowTheme"(ByVal hWnd As Integer, Optional ByRef pszSubAppName As String = " ", Optional ByRef pszSubIdList As String = " ") As Integer
	
	Public Declare Function SetParent Lib "user32" (ByVal hWndChild As Integer, ByVal hWndNewParent As Integer) As Integer
	
	'UPGRADE_WARNING: Für die Struktur WIN32_FIND_DATA müssen Marshalling-Attribute möglicherweise als ein Argument in dieser Declare-Anweisung weitergegeben werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="C429C3A5-5D47-4CD9-8F51-74A1616405DC"'
	Public Declare Function FindFirstFile Lib "kernel32"  Alias "FindFirstFileA"(ByVal lpFileName As String, ByRef lpFindFileData As WIN32_FIND_DATA) As Integer
	
	'UPGRADE_WARNING: Für die Struktur WIN32_FIND_DATA müssen Marshalling-Attribute möglicherweise als ein Argument in dieser Declare-Anweisung weitergegeben werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="C429C3A5-5D47-4CD9-8F51-74A1616405DC"'
	Public Declare Function FindNextFile Lib "kernel32"  Alias "FindNextFileA"(ByVal hFindFile As Integer, ByRef lpFindFileData As WIN32_FIND_DATA) As Integer
	
	Public Declare Function FindClose Lib "kernel32" (ByVal hFindFile As Integer) As Integer
	
	Public Declare Function GetVolumeInformation Lib "kernel32"  Alias "GetVolumeInformationA"(ByVal lpRootPathName As String, ByVal lpVolumeNameBuffer As String, ByVal nVolumeNameSize As Integer, ByRef lpVolumeSerialNumber As Integer, ByRef lpMaximumComponentLength As Integer, ByRef lpFileSystemFlags As Integer, ByVal lpFileSystemNameBuffer As String, ByVal nFileSystemNameSize As Integer) As Integer
	
	Public Declare Function ShellExecute Lib "shell32.dll"  Alias "ShellExecuteA"(ByVal hWnd As Integer, ByVal Operation As String, ByVal FileName As String, Optional ByVal Parameters As String = "", Optional ByVal Directory As String = "", Optional ByVal WindowStyle As Integer = AppWinStyle.MinimizedFocus) As Integer
	
	Public Declare Function GetLogicalDrives Lib "kernel32" () As Integer
	
	Public Declare Function GetDriveType Lib "kernel32"  Alias "GetDriveTypeA"(ByVal DriveLetter As String) As Integer
	
	Public Function AvailableDriveLetters() As Collection
		' Liefert eine Collection mit allen Laufwerksbuchstaben
		' vorhandener logischer Partitionen zurück.
		Dim lBitmuster As Integer
		Dim lBit As Integer
		' Neue Collection für die Rückgabe instanzieren
		AvailableDriveLetters = New Collection
		' Bitmuster für vorhandene Laufwerke ermitteln. In diesem Bitmuster
		' repräsentiert das Bit 0 den Laufwerksbuchstaben A,
		' Bit 1 das Laufwerk B etc..
		lBitmuster = GetLogicalDrives()
		For lBit = 0 To 25 ' 26 Laufwerksbuchstaben von A bis Z
			' Prüfen, ob das Bit für den Laufwerksbuchstaben gesetzt ist
			If (lBitmuster And 2 ^ lBit) <> 0 Then ' Laufwerk ist vorhanden
				' Aus der Bitposition den Laufwerksbuchstaben ermitteln:
				' Erstes Laufwerk (entsprechend dem Bit 0 im Bitmuster)
				' ist Laufwerk "A" mit dem Ansi-Code 65:
				AvailableDriveLetters.Add(Chr(lBit + 65))
			End If
		Next lBit
	End Function
	
	Public Function DriveType(ByVal DriveLetter As String) As DriveTypes
		' Liefert den Typ eines übergebenen Laufwerksbuchstabens zurück.
		Dim sRoot As String
		Dim lDriveType As Integer
		' Hauptverzeichnis aus Laufwerksangabe konstruieren
		sRoot = Left(DriveLetter, 1) & ":\"
		' Den Typ des Laufwerks ermitteln
		lDriveType = GetDriveType(sRoot)
		' DRIVE_NO_ROOT_DIR wird gleichgesetzt mit einem unbekannten Laufwerk
		If lDriveType = DRIVE_NO_ROOT_DIR Then
			DriveType = DriveTypes.dtUnknown
		Else
			DriveType = lDriveType
		End If
	End Function
	
	Public Function DriveInfo(ByVal Laufwerk As String) As Object
		Dim r As Integer
		Dim LaufwerkNr As Integer
		Dim Bez As New VB6.FixedLengthString(256)
		Dim d1, d2 As Integer
		Dim d3 As New VB6.FixedLengthString(256) 'dummy-Vars
		
		r = GetVolumeInformation(Laufwerk, Bez.Value, Len(Bez.Value), LaufwerkNr, d1, d2, d3.Value, Len(d3.Value))
		
		If r = 0 Then
			'UPGRADE_WARNING: Die Standardeigenschaft des Objekts DriveInfo konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			DriveInfo = ""
		Else
			DriveInfo = RTrim(Left(Bez.Value, InStr(1, Bez.Value, Chr(0), CompareMethod.Binary) - 1))
		End If
	End Function
	
	' --- LaunchDocument öffnet eine Dokumentendatei
	Public Function LaunchDocument(ByRef FileName As String, Optional ByVal ParentForm As System.Windows.Forms.Form = Nothing, Optional ByVal ShowOpenWithDialog As Boolean = False, Optional ByVal WindowStyle As AppWinStyle = AppWinStyle.NormalFocus) As Boolean
		' ------------------------------------------------------------------
		' Startet eine existierende Datei in der mit ihr verknüpften
		' Anwendung (wie bei Doppelklick im Windows Explorer). Besteht zu
		' einer Dateinamenserweiterung keine zugehörige Verknüpfung zu einer
		' Anwendung, kann der "Öffnen mit"-Dialog angezeigt werden, in dem
		' der Endanwender auf Wunsch eine Anwendung zuordnen kann
		' (ShowOpenWithDialog = True).
		' ------------------------------------------------------------------
		Dim lSuccess As Integer
		Dim hWndParent As Integer
		
		' Falls eine Parent-Form übergeben wurde, deren
		' Fensterhandle für den ShellExecute-Aufruf verwenden
		' (sonst 0).
		If Not ParentForm Is Nothing Then
			hWndParent = ParentForm.Handle.ToInt32
		End If
		
		' Versuchen, das Dokument mittels ShellExecute in der
		' zugehörigen Anwendung zu öffnen:
		lSuccess = ShellExecute(hWndParent, "Open", FileName, vbNullString, vbNullString, WindowStyle)
		
		' Rückgabewert prüfen:
		Select Case lSuccess
			
			Case Is > 32 ' Aufruf erfolgreich
				LaunchDocument = True
				
			Case SE_ERR_NOASSOC ' Keine verknüpfte Anwendung
				' Falls ShowOpenWithDialog = True, wird der Dialog
				' "Öffnen mit" für diese Datei angezeigt:
				If ShowOpenWithDialog Then
					Shell("RunDLL32 shell32.dll,OpenAs_RunDLL " & FileName)
					' Die Auswahlmöglichkeit wird als Erfolg gewertet:
					LaunchDocument = True
				End If
				
			Case Else
				' ShellExecute war erfolglos.
				' Boolean-Standardwert False zurückgeben
				
		End Select
		
	End Function
	
	Public Sub QuickSort(ByVal LB As Integer, ByVal UB As Integer)
		Dim P1, P2 As Integer
		Dim Ref As String
		Dim TEMP As FILETYPE
		
		P1 = LB
		P2 = UB
		Ref = Files((P1 + P2) / 2).Sort
		
		Do 
			Do While (Files(P1).Sort < Ref)
				P1 = P1 + 1
			Loop 
			
			Do While (Files(P2).Sort > Ref)
				P2 = P2 - 1
			Loop 
			
			If P1 <= P2 Then
				'UPGRADE_WARNING: Die Standardeigenschaft des Objekts TEMP konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				TEMP = Files(P1)
				'UPGRADE_WARNING: Die Standardeigenschaft des Objekts Files(P1) konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				Files(P1) = Files(P2)
				'UPGRADE_WARNING: Die Standardeigenschaft des Objekts Files(P2) konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				Files(P2) = TEMP
				
				P1 = P1 + 1
				P2 = P2 - 1
			End If
		Loop Until (P1 > P2)
		
		If LB < P2 Then Call QuickSort(LB, P2)
		If P1 < UB Then Call QuickSort(P1, UB)
	End Sub
End Module