Attribute VB_Name = "modUni"
Option Explicit

Public Const DRIVE_UNKNOWN = 0
Public Const DRIVE_NO_ROOT_DIR = 1
Public Const DRIVE_REMOVABLE = 2
Public Const DRIVE_FIXED = 3
Public Const DRIVE_REMOTE = 4
Public Const DRIVE_CDROM = 5
Public Const DRIVE_RAMDISK = 6

Public Const BLOCK_SIZE = 5000

Private Const SE_ERR_NOASSOC As Long = 31&
Public Const MAX_PATH = 259

Public Const FILE_ATTRIBUTE_ARCHIVE = &H20
Public Const FILE_ATTRIBUTE_COMPRESSED = &H800
Public Const FILE_ATTRIBUTE_DIRECTORY = &H10
Public Const FILE_ATTRIBUTE_HIDDEN = &H2
Public Const FILE_ATTRIBUTE_NORMAL = &H80
Public Const FILE_ATTRIBUTE_READONLY = &H1
Public Const FILE_ATTRIBUTE_SYSTEM = &H4
Public Const FILE_ATTRIBUTE_TEMPORARY = &H100

Public Enum DriveTypes
  dtUnknown = DRIVE_UNKNOWN
  dtRemovable = DRIVE_REMOVABLE
  dtFixed = DRIVE_FIXED
  dtRemote = DRIVE_REMOTE
  dtCDRom = DRIVE_CDROM
  dtRamDisk = DRIVE_RAMDISK
End Enum

Public Type FILETIME
  dwLowDateTime As Long
  dwHighDateTime As Long
End Type

Public Type WIN32_FIND_DATA
  dwFileAttributes As Long
  ftCreationTime As FILETIME
  ftLastAccessTime As FILETIME
  ftLastWriteTime As FILETIME
  nFileSizeHigh As Long
  nFileSizeLow As Long
  dwReserved0 As Long
  dwReserved1 As Long
  cFileName As String * MAX_PATH
  cAlternate As String * 14
End Type

Public Type FILETYPE
  Name As String
  Path As String
  Sort As String ' Filename in Grossbuchstaben
End Type

Public Declare Sub InitCommonControls Lib "comctl32" ()

Public Declare Function CreateStreamOnHGlobal Lib "ole32.dll" (ByRef hGlobal As Any, ByVal fDeleteOnRelease As Long, lpIStream As IUnknown) As Long
Public Declare Function OleLoadPicture Lib "oleaut32.dll" (ByVal lpStream As IUnknown, ByVal lSize As Long, ByVal fRunmode As Long, riid As Any, lpIPicture As IPicture) As Long

Public Declare Function ActivateWindowTheme Lib "uxtheme" Alias "SetWindowTheme" (ByVal hWnd As Long, Optional ByVal pszSubAppName As Long = 0, Optional ByVal pszSubIdList As Long = 0) As Long
Public Declare Function DeactivateWindowTheme Lib "uxtheme" Alias "SetWindowTheme" (ByVal hWnd As Long, Optional ByRef pszSubAppName As String = " ", Optional ByRef pszSubIdList As String = " ") As Long

Public Declare Function SetParent Lib "user32" ( _
    ByVal hWndChild As Long, ByVal hWndNewParent As Long) As Long

Public Declare Function FindFirstFile Lib "kernel32" _
        Alias "FindFirstFileA" (ByVal lpFileName As String, _
        lpFindFileData As WIN32_FIND_DATA) As Long
        
Public Declare Function FindNextFile Lib "kernel32" _
        Alias "FindNextFileA" (ByVal hFindFile As Long, _
        lpFindFileData As WIN32_FIND_DATA) As Long
        
Public Declare Function FindClose Lib "kernel32" (ByVal _
        hFindFile As Long) As Long

Public Declare Function GetVolumeInformation Lib "kernel32" Alias "GetVolumeInformationA" (ByVal lpRootPathName As String, ByVal lpVolumeNameBuffer As String, ByVal nVolumeNameSize As Long, lpVolumeSerialNumber As Long, lpMaximumComponentLength As Long, lpFileSystemFlags As Long, ByVal lpFileSystemNameBuffer As String, ByVal nFileSystemNameSize As Long) As Long

Public Declare Function ShellExecute Lib "shell32.dll" _
  Alias "ShellExecuteA" ( _
  ByVal hWnd As Long, _
  ByVal Operation As String, _
  ByVal FileName As String, _
  Optional ByVal Parameters As String, _
  Optional ByVal Directory As String, _
  Optional ByVal WindowStyle As Long = vbMinimizedFocus _
  ) As Long

Public Declare Function GetLogicalDrives Lib "kernel32" () As Long

Public Declare Function GetDriveType Lib "kernel32" _
  Alias "GetDriveTypeA" (ByVal DriveLetter As String) As Long

Public Function AvailableDriveLetters() As Collection
  ' Liefert eine Collection mit allen Laufwerksbuchstaben
  ' vorhandener logischer Partitionen zurück.
  Dim lBitmuster As Long
  Dim lBit As Long
  ' Neue Collection für die Rückgabe instanzieren
  Set AvailableDriveLetters = New Collection
  ' Bitmuster für vorhandene Laufwerke ermitteln. In diesem Bitmuster
  ' repräsentiert das Bit 0 den Laufwerksbuchstaben A,
  ' Bit 1 das Laufwerk B etc..
  lBitmuster = GetLogicalDrives()
  For lBit = 0 To 25 ' 26 Laufwerksbuchstaben von A bis Z
    ' Prüfen, ob das Bit für den Laufwerksbuchstaben gesetzt ist
    If (lBitmuster And 2 ^ lBit) <> 0 Then  ' Laufwerk ist vorhanden
      ' Aus der Bitposition den Laufwerksbuchstaben ermitteln:
      ' Erstes Laufwerk (entsprechend dem Bit 0 im Bitmuster)
      ' ist Laufwerk "A" mit dem Ansi-Code 65:
      AvailableDriveLetters.Add Chr$(lBit + 65)
    End If
  Next lBit
End Function

Public Function DriveType(ByVal DriveLetter As String) As DriveTypes
  ' Liefert den Typ eines übergebenen Laufwerksbuchstabens zurück.
  Dim sRoot As String
  Dim lDriveType As Long
  ' Hauptverzeichnis aus Laufwerksangabe konstruieren
  sRoot = Left$(DriveLetter, 1) & ":\"
  ' Den Typ des Laufwerks ermitteln
  lDriveType = GetDriveType(sRoot)
  ' DRIVE_NO_ROOT_DIR wird gleichgesetzt mit einem unbekannten Laufwerk
  If lDriveType = DRIVE_NO_ROOT_DIR Then
    DriveType = dtUnknown
  Else
    DriveType = lDriveType
  End If
End Function

Public Function DriveInfo(ByVal Laufwerk As String)
  Dim r&
  Dim LaufwerkNr As Long
  Dim Bez As String * 256
  Dim d1&, d2&, d3 As String * 256 'dummy-Vars
      
  r = GetVolumeInformation(Laufwerk, Bez, _
    Len(Bez), LaufwerkNr, d1, d2, d3, Len(d3))
        
  If r = 0 Then
    DriveInfo = ""
  Else
    DriveInfo = RTrim$(Left$(Bez, InStr(1, Bez, Chr$(0), vbBinaryCompare) - 1))
  End If
End Function

' --- LaunchDocument öffnet eine Dokumentendatei
Public Function LaunchDocument(ByRef FileName As String, _
  Optional ByVal ParentForm As Form, _
  Optional ByVal ShowOpenWithDialog As Boolean = False, _
  Optional ByVal WindowStyle As VBA.VbAppWinStyle = vbNormalFocus _
  ) As Boolean
' ------------------------------------------------------------------
' Startet eine existierende Datei in der mit ihr verknüpften
' Anwendung (wie bei Doppelklick im Windows Explorer). Besteht zu
' einer Dateinamenserweiterung keine zugehörige Verknüpfung zu einer
' Anwendung, kann der "Öffnen mit"-Dialog angezeigt werden, in dem
' der Endanwender auf Wunsch eine Anwendung zuordnen kann
' (ShowOpenWithDialog = True).
' ------------------------------------------------------------------
Dim lSuccess As Long
Dim hWndParent As Long

  ' Falls eine Parent-Form übergeben wurde, deren
  ' Fensterhandle für den ShellExecute-Aufruf verwenden
  ' (sonst 0).
  If Not ParentForm Is Nothing Then
    hWndParent = ParentForm.hWnd
  End If
  
  ' Versuchen, das Dokument mittels ShellExecute in der
  ' zugehörigen Anwendung zu öffnen:
  lSuccess = ShellExecute(hWndParent, "Open", FileName, _
                          vbNullString, vbNullString, _
                          WindowStyle)
  
  ' Rückgabewert prüfen:
  Select Case lSuccess
    
    Case Is > 32 ' Aufruf erfolgreich
      LaunchDocument = True
    
    Case SE_ERR_NOASSOC ' Keine verknüpfte Anwendung
      ' Falls ShowOpenWithDialog = True, wird der Dialog
      ' "Öffnen mit" für diese Datei angezeigt:
      If ShowOpenWithDialog Then
        Shell "RunDLL32 shell32.dll,OpenAs_RunDLL " & FileName
        ' Die Auswahlmöglichkeit wird als Erfolg gewertet:
        LaunchDocument = True
      End If
      
    Case Else
      ' ShellExecute war erfolglos.
      ' Boolean-Standardwert False zurückgeben
      
  End Select
  
End Function

Public Sub QuickSort(ByVal LB&, ByVal UB&)
  Dim P1&, P2&, Ref$, TEMP As FILETYPE

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
        TEMP = Files(P1)
        Files(P1) = Files(P2)
        Files(P2) = TEMP
            
        P1 = P1 + 1
        P2 = P2 - 1
      End If
    Loop Until (P1 > P2)

    If LB < P2 Then Call QuickSort(LB, P2)
    If P1 < UB Then Call QuickSort(P1, UB)
End Sub


