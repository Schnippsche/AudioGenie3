Attribute VB_Name = "modAudioGenie"
Option Explicit
Public genie As clsAudioGenie
Public Const tempFile = "C:\temp\vbtmp"

Public Const AUDIO_FORMAT_UNKNOWN = 0
Public Const AUDIO_FORMAT_MPEG = 1
Public Const AUDIO_FORMAT_WMA = 2
Public Const AUDIO_FORMAT_MONKEY = 3
Public Const AUDIO_FORMAT_FLAC = 4
Public Const AUDIO_FORMAT_WAV = 5
Public Const AUDIO_FORMAT_OGGVORBIS = 6
Public Const AUDIO_FORMAT_MPP = 7
Public Const AUDIO_FORMAT_AAC = 8
Public Const AUDIO_FORMAT_MP4 = 9

Public Enum AudioFormats
    afUnknown = AUDIO_FORMAT_UNKNOWN
    afMpeg = AUDIO_FORMAT_MPEG
    afWma = AUDIO_FORMAT_WMA
    afMonkey = AUDIO_FORMAT_MONKEY
    afFlac = AUDIO_FORMAT_FLAC
    afWav = AUDIO_FORMAT_WAV
    afOgg = AUDIO_FORMAT_OGGVORBIS
    afMpp = AUDIO_FORMAT_MPP
    afAac = AUDIO_FORMAT_AAC
    afMp4 = AUDIO_FORMAT_MP4
End Enum

Public S_Order As Integer
Public Datei$, DateiAlt$
Public AudioFormat As AudioFormats
Public Dateiformat$(0 To 9)
Public Titel$, Interpret$, Album$, Track$, Jahr$, Genre$, Kommentar$
Public oldItem As ListItem, aktItem As ListItem
Public FileSize As Long
Public Files() As FILETYPE
Public oldXPos As Integer, newXPos As Integer

Public Sub AddEntry(x As Long)
    Set aktItem = frmMain.detailView.ListItems.Add(, , Files(x).Name, 0, 0)
    aktItem.SubItems(14) = Files(x).Path
    ChangeEntry aktItem
End Sub

Public Sub ChangeEntry(item As ListItem)
    Dim flag$
    Datei = item.SubItems(14)
    AudioFormat = genie.AUDIOAnalyzeFile(Datei)
    item.SmallIcon = Sgn(AudioFormat) + 1
    FileSize = Int(genie.AUDIOGetFileSize / 1024)
    If FileSize * 1024 < genie.AUDIOGetFileSize Then FileSize = FileSize + 1
    ' size
    item.SubItems(1) = FileSize
    item.SubItems(2) = Dateiformat(AudioFormat)
    item.SubItems(3) = genie.AUDIOTitle
    item.SubItems(4) = genie.AUDIOArtist
    item.SubItems(5) = genie.AUDIOAlbum
    item.SubItems(6) = genie.AUDIOTrack
    item.SubItems(7) = genie.AUDIOYear
    item.SubItems(8) = genie.AUDIOGenre
    item.SubItems(9) = genie.AUDIOComment
    item.SubItems(10) = genie.AUDIOGetBitrate
    item.SubItems(11) = genie.AUDIOGetSampleRate
    item.SubItems(12) = Format$(genie.AUDIOGetDuration, "0")
    flag = IIf(genie.ID3V1Exists, "id3v" & genie.ID3V1GetVersion & ", ", "")
    flag = flag & IIf(genie.ID3V2Exists, "id3v" & genie.ID3V2GetVersion & ", ", "")
    flag = flag & IIf(genie.APEExists, "ape" & genie.APEGetVersion & ", ", "")
    flag = flag & IIf(genie.LYRICSExists, "lyrics" & genie.LYRICSGetVersion & ", ", "")
    If Len(flag) > 1 Then flag = Left$(flag, Len(flag) - 2)
    item.SubItems(13) = flag
End Sub

Public Sub DeleteAPETag()
    If aktItem Is Nothing Then Exit Sub
    Dim dk$, dl$
    dk = aktItem.Text         ' short file name without path
    dl = aktItem.SubItems(14) ' complete file name with path
    If MsgBox("Soll der APE-Tag von " & vbCr & dk & vbCr & "wirklich entfernt werden ?", vbYesNo, "Tag entfernen") = vbYes Then
        If genie.APERemoveTagFromFile(dl) = False Then
            MsgBox "Beim Entfernen ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Entfernen des APE-Tag"
        End If
    End If
    ' new read in
    AudioFormat = genie.AUDIOAnalyzeFile(dl)
End Sub

Public Sub DeleteID3v1Tag()
    If aktItem Is Nothing Then Exit Sub
    Dim dk$, dl$
    dk = aktItem.Text         ' short file name without path
    dl = aktItem.SubItems(14) ' complete file name with path
    If MsgBox("Soll der ID3v1-Tag von " & vbCr & dk & vbCr & "wirklich entfernt werden ?", vbYesNo, "Tag entfernen") = vbYes Then
        If genie.ID3V1RemoveTagFromFile(dl) = False Then
            MsgBox "Beim Entfernen ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Entfernen des ID3v1-Tag"
        End If
    End If
    ' new read in
    AudioFormat = genie.AUDIOAnalyzeFile(dl)
End Sub

Public Sub DeleteID3v2Tag()
    If aktItem Is Nothing Then Exit Sub
    Dim dk$, dl$
    dk = aktItem.Text         ' short file name without path
    dl = aktItem.SubItems(14) ' complete file name with path
    If MsgBox("Soll der ID3v2-Tag von " & vbCr & dk & vbCr & "wirklich entfernt werden ?", vbYesNo, "Tag entfernen") = vbYes Then
        If genie.ID3V2RemoveTagFromFile(dl) = False Then
            MsgBox "Beim Entfernen ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Entfernen des ID3v2-Tag"
        End If
    End If
    ' new read in
    AudioFormat = genie.AUDIOAnalyzeFile(dl)
End Sub

Public Sub DeleteLyricsTag()
    If aktItem Is Nothing Then Exit Sub
    Dim dk$, dl$
    dk = aktItem.Text         ' short file name without path
    dl = aktItem.SubItems(14) ' complete file name with path
    If MsgBox("Soll der Lyrics-Tag von " & vbCr & dk & vbCr & "wirklich entfernt werden ?", vbYesNo, "Tag entfernen") = vbYes Then
        If genie.LYRICSRemoveTagFromFile(dl) = False Then
            MsgBox "Beim Entfernen ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Entfernen des Lyrics-Tag"
        End If
    End If
    ' new read in
    AudioFormat = genie.AUDIOAnalyzeFile(dl)
End Sub

Public Sub GetFiles(ByVal Folder$)
    Dim File$, hFile&, FD As WIN32_FIND_DATA
    Dim cnt As Long, x As Long
    ReDim Files(BLOCK_SIZE)
    cnt = 0
    frmMain.stBar.Panels(2).Text = "Analyze " & Folder & " ..."
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
            End If
            Files(cnt).Name = File
            Files(cnt).Sort = UCase$(File)
            Files(cnt).Path = Folder & File
            cnt = cnt + 1
        End If
    Loop While FindNextFile(hFile, FD)

    Call FindClose(hFile)
    frmMain.stBar.Panels(4).Text = cnt & " Object(s)"
    If cnt > 0 Then
        frmMain.prgBar.Min = 0
        frmMain.prgBar.Max = cnt
        frmMain.prgBar.Value = 0
        frmMain.prgBar.Visible = True

        For x = 0 To cnt - 1
            frmMain.prgBar.Value = x
            AddEntry x
         Next x

        frmMain.prgBar.Visible = False
    End If
   
    frmMain.stBar.Panels(2).Text = "Ready"
    DateiAlt = ""
    
End Sub

Public Sub GetFolders(Root$)
    Dim File$, hFile&, FD As WIN32_FIND_DATA
    Dim cnt As Long, x As Long
    ReDim Files(BLOCK_SIZE)
    frmMain.stBar.Panels(2).Text = "Analyze drive " & Root & " ..."
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
    frmMain.stBar.Panels(2).Text = "Ready"
End Sub

Public Sub Main()
    Set genie = New clsAudioGenie
   'genie.DisablePopupKey ""
    Dateiformat(0) = "File"
    Dateiformat(1) = "MPEG"
    Dateiformat(2) = "WMA"
    Dateiformat(3) = "MONKEY"
    Dateiformat(4) = "FLAC"
    Dateiformat(5) = "WAV"
    Dateiformat(6) = "OGG"
    Dateiformat(7) = "MPP"
    Dateiformat(8) = "AAC"
    Dateiformat(9) = "MP4"
    frmMain.Show
End Sub

Public Sub ReadDrive(ByVal Path$)
    Path = UCase(Path)
    frmMain.folderView.Nodes.Clear
    frmMain.folderView.Nodes.Add , , Path, DriveInfo(Path) & " (" & Left$(Path, 2) & ")", 1, 1
    Call GetFolders(Path)
    frmMain.folderView.Nodes(1).Expanded = True
End Sub

