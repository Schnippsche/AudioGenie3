Attribute VB_Name = "modAudioGenie"
Option Explicit
Public genie As clsAudioGenie

Public S_Order As Integer
Public Datei$, DateiAlt$
Public Dateiformat As AudioFormatID
Public Titel$, Interpret$, Album$, Track$, Jahr$, Genre$, Kommentar$
Public oldItem As ListItem, aktItem As ListItem
Public FileSize As Long
Public Files() As FILETYPE
Public oldXPos As Integer, newXPos As Integer
Public AudioFormate(11) As String


Public Sub Main()
  Call InitCommonControls
  AudioFormate(0) = "Datei"
  AudioFormate(1) = "MPEG"
  AudioFormate(2) = "WMA"
  AudioFormate(3) = "MONKEY"
  AudioFormate(4) = "FLAC"
  AudioFormate(5) = "WAV"
  AudioFormate(6) = "OGG"
  AudioFormate(7) = "MPP"
  AudioFormate(8) = "AAC"
  AudioFormate(9) = "MP4"
  AudioFormate(10) = "TTA"
  Set genie = New clsAudioGenie
  genie.SetLogFile "meinlog.txt"
  'genie.DisablePopupKey ""
  frmMain.Show
End Sub

Public Sub ReadDrive(ByVal Path$)
  Path = UCase(Path)
  frmMain.folderView.Nodes.Clear
  frmMain.folderView.Nodes.Add , , Path, DriveInfo(Path) & " (" & Left$(Path, 2) & ")", 1, 1
  Call GetFolders(Path)
  frmMain.folderView.Nodes(1).Expanded = True
End Sub

Public Sub GetFolders(Root$)
  Dim File$, hFile&, FD As WIN32_FIND_DATA
  Dim cnt As Long, x As Long
  ReDim Files(BLOCK_SIZE)
  frmMain.stBar.Panels(2).text = "Analysiere Laufwerk " & Root & " ..."
  cnt = 0
  hFile = FindFirstFile(Root & "*.*", FD)
  If hFile = 0 Then Exit Sub
  Do
     File = Left(FD.cFileName, InStr(FD.cFileName, Chr(0)) - 1)
     If (FD.dwFileAttributes And FILE_ATTRIBUTE_DIRECTORY) _
       = FILE_ATTRIBUTE_DIRECTORY Then
       If (File <> ".") And (File <> "..") Then
         If cnt > UBound(Files) Then
            ReDim Preserve Files(0 To cnt + BLOCK_SIZE)
          End If
          Files(cnt).Name = File
          Files(cnt).Sort = UCase$(File)
          Files(cnt).Path = Root & File & "\"
          cnt = cnt + 1
       End If
     End If
  Loop While FindNextFile(hFile, FD)
  Call FindClose(hFile)
  If cnt > 0 Then
    Call QuickSort(0, cnt - 1)
    For x = 0 To cnt - 1
      frmMain.folderView.Nodes.Add Root, tvwChild, Files(x).Path, _
                          Files(x).Name, 2, 3
    Next x
  End If
  frmMain.stBar.Panels(2).text = "Bereit"
End Sub

Public Sub GetFiles(ByVal Folder$)
  Dim File$, hFile&, FD As WIN32_FIND_DATA
  Dim cnt As Long, x As Long
  ReDim Files(BLOCK_SIZE)
  cnt = 0
  frmMain.stBar.Panels(2).text = "Analysiere " & Folder & " ..."
  frmMain.detailView.ListItems.Clear
  hFile = FindFirstFile(Folder & "*.*", FD)
  If hFile = 0 Then Exit Sub
  Do
     File = Left(FD.cFileName, InStr(FD.cFileName, Chr(0)) - 1)
     If (FD.dwFileAttributes And FILE_ATTRIBUTE_DIRECTORY) = 0 _
        And (FD.dwFileAttributes And FILE_ATTRIBUTE_HIDDEN) = 0 _
        And (FD.dwFileAttributes And FILE_ATTRIBUTE_SYSTEM) = 0 _
        And File <> "." And File <> ".." Then
        If cnt > UBound(Files) Then
          ReDim Preserve Files(0 To cnt + BLOCK_SIZE)
          ' DoEvents
        End If
        Files(cnt).Name = File
        Files(cnt).Sort = UCase$(File)
        Files(cnt).Path = Folder & File
        cnt = cnt + 1
     End If
  Loop While FindNextFile(hFile, FD)
  Call FindClose(hFile)
  frmMain.stBar.Panels(4).text = cnt & " Objekt(e)"
  If cnt > 0 Then
    frmMain.prgBar.Min = 0
    frmMain.prgBar.Max = cnt
    frmMain.prgBar.value = 0
    frmMain.prgBar.Visible = True
    For x = 0 To cnt - 1
      frmMain.prgBar.value = x
      AddEntry x
      ' If x Mod 100 = 0 Then DoEvents
    Next x
    frmMain.prgBar.Visible = False
  End If
  'DoEvents
  frmMain.stBar.Panels(2).text = "Bereit"
  DateiAlt = ""
  ' DoEvents
End Sub

Public Sub AddEntry(x As Long)
  Set aktItem = frmMain.detailView.ListItems.Add(, , Files(x).Name, 0, 0)
  aktItem.SubItems(15) = Files(x).Path
  ChangeEntry aktItem
End Sub

Public Sub ChangeEntry(item As ListItem)
  Dim flag$
  Datei = item.SubItems(15)
  Dateiformat = genie.AUDIOAnalyzeFile(Datei)
  item.SmallIcon = Sgn(Dateiformat) + 1
  FileSize = Int(genie.AUDIOGetFileSize / 1024)
  If FileSize * 1024 < genie.AUDIOGetFileSize Then FileSize = FileSize + 1
  ' Grösse
  item.SubItems(1) = FileSize
  item.SubItems(2) = AudioFormate(Dateiformat)
  item.SubItems(3) = genie.AUDIOTitle
  item.SubItems(4) = genie.AUDIOArtist
  item.SubItems(5) = genie.AUDIOAlbum
  item.SubItems(6) = genie.AUDIOTrack
  item.SubItems(7) = genie.AUDIOYear
  item.SubItems(8) = genie.AUDIOGenre
  item.SubItems(9) = genie.AUDIOComment
  item.SubItems(10) = genie.AUDIOGetBitrate
  item.SubItems(11) = genie.AUDIOGetSampleRate
  item.SubItems(12) = format$(genie.AUDIOGetDuration, "0")
  flag = IIf(genie.ID3V1Exists, "id3v" & genie.ID3V1GetVersion & ", ", "")
  flag = flag & IIf(genie.ID3V2Exists, "id3v" & genie.ID3V2GetVersion & ", ", "")
  flag = flag & IIf(genie.APEExists, "ape" & genie.APEGetVersion & ", ", "")
  flag = flag & IIf(genie.LYRICSExists, "lyrics" & genie.LYRICSGetVersion & ", ", "")
  If Len(flag) > 1 Then flag = Left$(flag, Len(flag) - 2)
  item.SubItems(13) = flag
  'item.SubItems(14) = genie.AUDIOGetMD5Value
  
End Sub

Public Sub DeleteID3v1Tag()
 If aktItem Is Nothing Then Exit Sub
 Dim dk$, dl$
 dk = aktItem.text         ' kurzer Dateiname ohne Pfad
 dl = aktItem.SubItems(15) ' vollständiger Dateiname mit pfad
 If MsgBox("Soll der ID3v1-Tag von " & vbCr & dk & vbCr & "wirklich entfernt werden ?", vbYesNo, "Tag entfernen") = vbYes Then
   If genie.ID3V1RemoveTagFromFile(dl) = False Then
     MsgBox "Beim Entfernen ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Entfernen des ID3v1-Tag"
   End If
 End If
 ' Neu einlesen
 Dateiformat = genie.AUDIOAnalyzeFile(dl)
End Sub

Public Sub DeleteID3v2Tag()
 If aktItem Is Nothing Then Exit Sub
 Dim dk$, dl$
 dk = aktItem.text         ' kurzer Dateiname ohne Pfad
 dl = aktItem.SubItems(15) ' vollständiger Dateiname mit pfad
 If MsgBox("Soll der ID3v2-Tag von " & vbCr & dk & vbCr & "wirklich entfernt werden ?", vbYesNo, "Tag entfernen") = vbYes Then
    Debug.Print genie.OGGRemoveTag
   If genie.ID3V2RemoveTagFromFile(dl) = False Then
     MsgBox "Beim Entfernen ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Entfernen des ID3v2-Tag"
   End If
 End If
 ' Neu einlesen
 Dateiformat = genie.AUDIOAnalyzeFile(dl)
End Sub

Public Sub DeleteAPETag()
 If aktItem Is Nothing Then Exit Sub
 Dim dk$, dl$
 dk = aktItem.text         ' kurzer Dateiname ohne Pfad
 dl = aktItem.SubItems(15) ' vollständiger Dateiname mit pfad
 If MsgBox("Soll der APE-Tag von " & vbCr & dk & vbCr & "wirklich entfernt werden ?", vbYesNo, "Tag entfernen") = vbYes Then
   If genie.APERemoveTagFromFile(dl) = False Then
     MsgBox "Beim Entfernen ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Entfernen des APE-Tag"
   End If
 End If
 ' Neu einlesen
 Dateiformat = genie.AUDIOAnalyzeFile(dl)
End Sub

Public Sub DeleteLyricsTag()
 If aktItem Is Nothing Then Exit Sub
 Dim dk$, dl$
 dk = aktItem.text         ' kurzer Dateiname ohne Pfad
 dl = aktItem.SubItems(15) ' vollständiger Dateiname mit pfad
 If MsgBox("Soll der Lyrics-Tag von " & vbCr & dk & vbCr & "wirklich entfernt werden ?", vbYesNo, "Tag entfernen") = vbYes Then
   If genie.LYRICSRemoveTagFromFile(dl) = False Then
     MsgBox "Beim Entfernen ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Entfernen des Lyrics-Tag"
   End If
 End If
 ' Neu einlesen
 Dateiformat = genie.AUDIOAnalyzeFile(dl)
End Sub
