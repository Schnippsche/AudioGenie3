VERSION 5.00
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "COMCTL32.OCX"
Begin VB.Form frmMain 
   Caption         =   "AudioGenie"
   ClientHeight    =   6885
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   10215
   LinkTopic       =   "Form1"
   ScaleHeight     =   6885
   ScaleWidth      =   10215
   StartUpPosition =   1  'Fenstermitte
   Begin ComctlLib.Toolbar tBar 
      Align           =   1  'Oben ausrichten
      Height          =   420
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   10215
      _ExtentX        =   18018
      _ExtentY        =   741
      Appearance      =   1
      ImageList       =   "driveImages"
      _Version        =   327682
   End
   Begin ComctlLib.ProgressBar prgBar 
      Height          =   315
      Left            =   2280
      TabIndex        =   4
      Top             =   4380
      Visible         =   0   'False
      Width           =   1080
      _ExtentX        =   1905
      _ExtentY        =   556
      _Version        =   327682
      Appearance      =   1
   End
   Begin ComctlLib.ListView detailView 
      Height          =   4395
      Left            =   3660
      TabIndex        =   3
      Top             =   420
      Width           =   5415
      _ExtentX        =   9551
      _ExtentY        =   7752
      View            =   3
      Arrange         =   1
      LabelEdit       =   1
      Sorted          =   -1  'True
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      _Version        =   327682
      SmallIcons      =   "audioImages"
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   16
      BeginProperty ColumnHeader(1) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         Key             =   "DN"
         Object.Tag             =   ""
         Text            =   "Datei"
         Object.Width           =   3528
      EndProperty
      BeginProperty ColumnHeader(2) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         Alignment       =   1
         SubItemIndex    =   1
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "Grösse in kB"
         Object.Width           =   2117
      EndProperty
      BeginProperty ColumnHeader(3) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   2
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "Format"
         Object.Width           =   1058
      EndProperty
      BeginProperty ColumnHeader(4) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   3
         Key             =   "TI"
         Object.Tag             =   ""
         Text            =   "Titel"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(5) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   4
         Key             =   "IN"
         Object.Tag             =   ""
         Text            =   "Interpret"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(6) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   5
         Key             =   "AL"
         Object.Tag             =   ""
         Text            =   "Album"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(7) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         Alignment       =   1
         SubItemIndex    =   6
         Key             =   "TR"
         Object.Tag             =   ""
         Text            =   "Track"
         Object.Width           =   1411
      EndProperty
      BeginProperty ColumnHeader(8) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         Alignment       =   1
         SubItemIndex    =   7
         Key             =   "JA"
         Object.Tag             =   ""
         Text            =   "Jahr"
         Object.Width           =   1411
      EndProperty
      BeginProperty ColumnHeader(9) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   8
         Key             =   "GE"
         Object.Tag             =   ""
         Text            =   "Genre"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(10) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   9
         Key             =   "KO"
         Object.Tag             =   ""
         Text            =   "Kommentar"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(11) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         Alignment       =   1
         SubItemIndex    =   10
         Key             =   "BI"
         Object.Tag             =   ""
         Text            =   "Bitrate (kbps)"
         Object.Width           =   1764
      EndProperty
      BeginProperty ColumnHeader(12) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         Alignment       =   1
         SubItemIndex    =   11
         Key             =   "FR"
         Object.Tag             =   ""
         Text            =   "Frequenz (Hz)"
         Object.Width           =   1764
      EndProperty
      BeginProperty ColumnHeader(13) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         Alignment       =   1
         SubItemIndex    =   12
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "Dauer (sec)"
         Object.Width           =   1411
      EndProperty
      BeginProperty ColumnHeader(14) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   13
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "Tags"
         Object.Width           =   2293
      EndProperty
      BeginProperty ColumnHeader(15) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   14
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "AudioMD5"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(16) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   15
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "Voller Dateiname"
         Object.Width           =   0
      EndProperty
   End
   Begin ComctlLib.TreeView folderView 
      Height          =   3675
      Left            =   0
      TabIndex        =   2
      Top             =   540
      Width           =   3435
      _ExtentX        =   6059
      _ExtentY        =   6482
      _Version        =   327682
      Indentation     =   529
      LabelEdit       =   1
      LineStyle       =   1
      Style           =   7
      ImageList       =   "folderImages"
      Appearance      =   1
   End
   Begin ComctlLib.StatusBar stBar 
      Align           =   2  'Unten ausrichten
      Height          =   375
      Left            =   0
      TabIndex        =   1
      Top             =   6510
      Width           =   10215
      _ExtentX        =   18018
      _ExtentY        =   661
      SimpleText      =   ""
      ShowTips        =   0   'False
      _Version        =   327682
      BeginProperty Panels {0713E89E-850A-101B-AFC0-4210102A8DA7} 
         NumPanels       =   6
         BeginProperty Panel1 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            Object.Width           =   706
            MinWidth        =   706
            Picture         =   "frmMain.frx":0000
            Key             =   ""
            Object.Tag             =   ""
         EndProperty
         BeginProperty Panel2 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            AutoSize        =   1
            Object.Width           =   7408
            Text            =   "Bereit"
            TextSave        =   "Bereit"
            Key             =   ""
            Object.Tag             =   ""
         EndProperty
         BeginProperty Panel3 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            Object.Width           =   3528
            MinWidth        =   3528
            Key             =   ""
            Object.Tag             =   ""
         EndProperty
         BeginProperty Panel4 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            Key             =   ""
            Object.Tag             =   ""
         EndProperty
         BeginProperty Panel5 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            Style           =   6
            Object.Width           =   1940
            MinWidth        =   1940
            TextSave        =   "26.07.2009"
            Key             =   ""
            Object.Tag             =   ""
         EndProperty
         BeginProperty Panel6 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            Style           =   5
            Object.Width           =   1235
            MinWidth        =   1235
            TextSave        =   "16:50"
            Key             =   ""
            Object.Tag             =   ""
         EndProperty
      EndProperty
   End
   Begin VB.Label lblSplitter 
      BackColor       =   &H80000000&
      BorderStyle     =   1  'Fest Einfach
      Height          =   4395
      Left            =   3480
      MousePointer    =   9  'Größenänderung W O
      TabIndex        =   5
      Top             =   420
      Width           =   90
   End
   Begin ComctlLib.ImageList audioImages 
      Left            =   1260
      Top             =   4200
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   16711935
      _Version        =   327682
      BeginProperty Images {0713E8C2-850A-101B-AFC0-4210102A8DA7} 
         NumListImages   =   2
         BeginProperty ListImage1 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "frmMain.frx":018E
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "frmMain.frx":02A0
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin ComctlLib.ImageList folderImages 
      Left            =   60
      Top             =   4200
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   14
      MaskColor       =   12632256
      _Version        =   327682
      BeginProperty Images {0713E8C2-850A-101B-AFC0-4210102A8DA7} 
         NumListImages   =   3
         BeginProperty ListImage1 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "frmMain.frx":05F2
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "frmMain.frx":0B24
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "frmMain.frx":1056
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin ComctlLib.ImageList driveImages 
      Left            =   660
      Top             =   4200
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   16711935
      _Version        =   327682
      BeginProperty Images {0713E8C2-850A-101B-AFC0-4210102A8DA7} 
         NumListImages   =   7
         BeginProperty ListImage1 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "frmMain.frx":1588
            Key             =   "d0"
         EndProperty
         BeginProperty ListImage2 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "frmMain.frx":18DA
            Key             =   "d1"
         EndProperty
         BeginProperty ListImage3 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "frmMain.frx":1C2C
            Key             =   "d2"
         EndProperty
         BeginProperty ListImage4 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "frmMain.frx":1F7E
            Key             =   "d3"
         EndProperty
         BeginProperty ListImage5 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "frmMain.frx":22D0
            Key             =   "d4"
         EndProperty
         BeginProperty ListImage6 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "frmMain.frx":2622
            Key             =   "d5"
         EndProperty
         BeginProperty ListImage7 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "frmMain.frx":2974
            Key             =   "d6"
         EndProperty
      EndProperty
   End
   Begin VB.Menu mnuPopup 
      Caption         =   "popupmenue"
      Visible         =   0   'False
      Begin VB.Menu mnuPlayFile 
         Caption         =   "Mit Standard-Player wiedergeben"
      End
      Begin VB.Menu mnuDummy0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuTag_Edit 
         Caption         =   "Tag bearbeiten"
      End
      Begin VB.Menu mnuID3v1_Edit 
         Caption         =   "ID3v1-Tag bearbeiten"
      End
      Begin VB.Menu mnuID3v2_Edit 
         Caption         =   "ID3v2-Tag bearbeiten"
      End
      Begin VB.Menu mnuAPE_Edit 
         Caption         =   "APE-Tag bearbeiten"
      End
      Begin VB.Menu mnuLyrics_Edit 
         Caption         =   "Lyrics-Tag bearbeiten"
      End
      Begin VB.Menu mnuDummy1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuID3v1_Delete 
         Caption         =   "ID3v1-Tag löschen"
      End
      Begin VB.Menu mnuID3v2_Delete 
         Caption         =   "ID3v2-Tag löschen"
      End
      Begin VB.Menu mnuAPE_Delete 
         Caption         =   "APE-Tag löschen"
      End
      Begin VB.Menu mnuLyrics_Delete 
         Caption         =   "Lyrics-Tag löschen"
      End
      Begin VB.Menu mnuDummy2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuAudioProperties 
         Caption         =   "Eigenschaften"
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Sub detailView_ColumnClick(ByVal ColumnHeader As ComctlLib.ColumnHeader)
 Dim col%, art%
 col = ColumnHeader.SubItemIndex
 art = ColumnHeader.Tag
 S_Order = S_Order Xor 1
 SortListView detailView.hWnd, col, art, S_Order
End Sub

Private Sub detailView_DblClick()
  If Len(Datei) < 1 Then Exit Sub
  LaunchDocument Datei, Me, True
End Sub

Private Sub detailView_ItemClick(ByVal item As ComctlLib.ListItem)
 ' Aktuell ausgewählte Datei analysieren
 Set aktItem = item
 Datei = item.SubItems(15)
 If Datei <> DateiAlt Then
   Dateiformat = genie.AUDIOAnalyzeFile(Datei)
   frmDetails.updateUI
   DateiAlt = Datei
 End If
End Sub

Private Sub detailView_MouseUp(Button As Integer, Shift As Integer, _
                              x As Single, y As Single)
  If (Button = vbRightButton) Then
    If (Not aktItem Is Nothing) Then
      Select Case Dateiformat
      Case AAC  ' Id3v1+v2
        mnuAPE_Edit.Visible = False
        mnuAPE_Delete.Visible = False
        mnuID3v1_Edit.Visible = True
        mnuID3v1_Delete.Visible = True
        mnuID3v2_Edit.Visible = True
        mnuID3v2_Delete.Visible = True
        mnuLyrics_Edit.Visible = False
        mnuLyrics_Delete.Visible = False
        mnuTag_Edit.Visible = False
        mnuDummy0.Visible = True
        mnuDummy1.Visible = True
        PopupMenu mnuPopup, vbPopupMenuLeftButton
      Case MPEG ' id3v1+2+lyrics
        mnuAPE_Edit.Visible = False
        mnuAPE_Delete.Visible = False
        mnuID3v1_Edit.Visible = True
        mnuID3v1_Delete.Visible = True
        mnuID3v2_Edit.Visible = True
        mnuID3v2_Delete.Visible = True
        mnuLyrics_Edit.Visible = True
        mnuLyrics_Delete.Visible = True
        mnuTag_Edit.Visible = False
        mnuDummy0.Visible = True
        mnuDummy1.Visible = True
        PopupMenu mnuPopup, vbPopupMenuLeftButton
      Case MONKEY, MPEGPLUS ' id3v1+v2+ape
        mnuAPE_Edit.Visible = True
        mnuAPE_Delete.Visible = True
        mnuID3v1_Edit.Visible = True
        mnuID3v1_Delete.Visible = True
        mnuID3v2_Edit.Visible = True
        mnuID3v2_Delete.Visible = True
        mnuLyrics_Edit.Visible = False
        mnuLyrics_Delete.Visible = False
        mnuTag_Edit.Visible = False
        mnuDummy0.Visible = True
        mnuDummy1.Visible = True
        PopupMenu mnuPopup, vbPopupMenuLeftButton
      Case WAV
        mnuAPE_Edit.Visible = False
        mnuAPE_Delete.Visible = False
        mnuID3v1_Edit.Visible = False
        mnuID3v1_Delete.Visible = False
        mnuID3v2_Edit.Visible = False
        mnuID3v2_Delete.Visible = False
        mnuLyrics_Edit.Visible = False
        mnuLyrics_Delete.Visible = False
        mnuTag_Edit.Visible = False
        mnuDummy0.Visible = False
        mnuDummy1.Visible = False
        PopupMenu mnuPopup, vbPopupMenuLeftButton
      Case FLAC, MP4M4A, OGGVORBIS, WMA ' tag+id3v2
        mnuAPE_Edit.Visible = False
        mnuAPE_Delete.Visible = False
        mnuID3v1_Edit.Visible = False
        mnuID3v1_Delete.Visible = False
        mnuID3v2_Edit.Visible = True
        mnuID3v2_Delete.Visible = True
        mnuLyrics_Edit.Visible = False
        mnuLyrics_Delete.Visible = False
        mnuTag_Edit.Visible = True
        mnuDummy0.Visible = True
        mnuDummy1.Visible = True
        PopupMenu mnuPopup, vbPopupMenuLeftButton
      End Select
    Else
      ' Kein Eintrag unter der Maus ggf. Alternativmenü anzeigen
    End If
  End If
End Sub

Private Sub folderView_NodeClick(ByVal Node As ComctlLib.Node)
  Dim Folder$, ti#
  Folder = folderView.SelectedItem.key
  If Screen.MousePointer = vbHourglass Then Exit Sub
  Screen.MousePointer = vbHourglass
  ti# = Timer
  GetFiles (Folder)
  frmMain.stBar.Panels(3).text = format$(Timer - ti#, "0.0") & " sec"
  Screen.MousePointer = vbCustom
End Sub

Private Sub Form_Initialize()
  On Error Resume Next
  ' nur für Windows XP, bei anderen BS kann die nachfolgende Zeile entfernt werden
  ' Only for XP
  Call ActivateWindowTheme(Me.hWnd)
  
  
  ' Make some presets
  genie.ConfigValue(CF_ID3V2PADDINGSIZE) = 2048
  ' Enable the checking of all frames is slower but exact
  genie.ConfigValue(CF_MPEGEXACTREAD) = 0 ' disabled
  genie.ConfigValue(CF_DOEVENTSMILLIS) = 500
End Sub

Private Sub Form_Load()
  Dim drive As Variant
  Dim DisplayName As String
  Dim nButton As Button
  Dim typ As Integer
  Dim FirstDrive%, count%
  Dim ti#
  FirstDrive% = 0
  count% = 0
  ' Über die Liste verfügbarer Laufwerke iterieren:
  For Each drive In AvailableDriveLetters
    ' Vom Laufwerkstyp abhängigen Klartextnamen definieren
    typ = DriveType(drive)
    count% = count% + 1
    If typ = dtFixed And FirstDrive% = 0 Then FirstDrive% = count%
    Set nButton = tBar.Buttons.Add(, drive, _
       drive & ":", tbrButtonGroup, "d" & typ)
    If typ = dtFixed Or typ = dtRemote Or typ = dtRamDisk Then
      nButton.ToolTipText = DriveInfo(drive & ":") & " (" & drive & ":)"
    Else
      nButton.ToolTipText = drive & ":"
    End If
  Next drive
  DoEvents
  folderView.Top = tBar.Height + 2
  detailView.Top = tBar.Height + 2
  lblSplitter.Top = tBar.Height + 2
  lblSplitter.Height = Me.Height
  ti# = Timer
  If FirstDrive% > 0 Then
    Set nButton = tBar.Buttons(FirstDrive%)
    nButton.value = tbrPressed
    tBar_ButtonClick nButton
  End If
  frmMain.stBar.Panels(3).text = format$(Timer - ti#, "0.0") & " sec"
  With prgBar
     'Die Progressbar in die Statusbar setzen
      Call SetParent(.hWnd, stBar.hWnd)
        .Top = 60
        .Left = stBar.Panels(3).Left + 10
        .Width = stBar.Panels(3).Width - 80
        .Height = stBar.Height - 90
  End With
  ' Die Spalten für Sortierung anpassen
  detailView.ColumnHeaders(1).Tag = stString
  detailView.ColumnHeaders(2).Tag = stNumeric
  detailView.ColumnHeaders(3).Tag = stString
  detailView.ColumnHeaders(4).Tag = stString
  detailView.ColumnHeaders(5).Tag = stString
  detailView.ColumnHeaders(6).Tag = stString
  detailView.ColumnHeaders(7).Tag = stString
  detailView.ColumnHeaders(8).Tag = stString
  detailView.ColumnHeaders(9).Tag = stString
  detailView.ColumnHeaders(10).Tag = stString
  detailView.ColumnHeaders(11).Tag = stNumeric
  detailView.ColumnHeaders(12).Tag = stNumeric
  detailView.ColumnHeaders(13).Tag = stNumeric
  detailView.ColumnHeaders(14).Tag = stString
  detailView.ColumnHeaders(15).Tag = stString
  detailView.ColumnHeaders(16).Tag = stString
  Me.Caption = "AudioGenie " & genie.GetAudioGenieVersion & " Beispielprojekt von Stefan Töngi"
End Sub

Private Sub Form_Resize()
  Dim H%, B%
  H% = frmMain.ScaleHeight - tBar.Height - stBar.Height
  B% = frmMain.ScaleWidth
  If H% < 0 Or B% < 0 Then Exit Sub
  folderView.Height = H%
  detailView.Height = H%
  detailView.Width = B% - folderView.Width - lblSplitter.Width
  detailView.Left = folderView.Width + lblSplitter.Width
  lblSplitter.Left = folderView.Width
  lblSplitter.Height = H%
  With prgBar
     'Die Progressbar in die Statusbar setzen
        .Top = 60
        .Left = stBar.Panels(3).Left + 10
        .Width = stBar.Panels(3).Width - 80
        .Height = stBar.Height - 90
  End With
End Sub

Private Sub Form_Unload(Cancel As Integer)
 End
End Sub

Private Sub lblSplitter_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
  lblSplitter.Visible = False
  oldXPos = 0
End Sub

Private Sub lblSplitter_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
 If Button = vbLeftButton Then
    newXPos = lblSplitter.Left + x
    If newXPos <> oldXPos And newXPos > 600 And newXPos < Me.ScaleWidth - 600 Then
      ' lblSplitter.Left = X
      folderView.Width = newXPos
      detailView.Left = newXPos + lblSplitter.Width
      detailView.Width = Me.ScaleWidth - detailView.Left
      oldXPos = newXPos
    End If
  End If
End Sub

Private Sub lblSplitter_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
  lblSplitter.Left = folderView.Width
  lblSplitter.Visible = True
End Sub

Private Sub mnuAPE_Delete_Click()
  DeleteAPETag
  ChangeEntry aktItem
End Sub

Private Sub mnuAPE_Edit_Click()
  frmAPE.Show 1
  ChangeEntry aktItem
End Sub

Private Sub mnuAudioProperties_Click()
  frmDetails.Show
End Sub

Private Sub mnuID3v1_Delete_Click()
  DeleteID3v1Tag
  ChangeEntry aktItem
End Sub

Private Sub mnuID3v1_Edit_Click()
  frmID3v1.Show 1
  ChangeEntry aktItem
End Sub

Private Sub mnuID3v2_Delete_Click()
  DeleteID3v2Tag
  ChangeEntry aktItem
End Sub

Private Sub mnuID3v2_Edit_Click()
  frmID3V2.updateUI
  frmID3V2.Show 1
  ChangeEntry aktItem
End Sub

Private Sub mnuLyrics_Delete_Click()
  DeleteLyricsTag
  ChangeEntry aktItem
End Sub

Private Sub mnuLyrics_Edit_Click()
  frmLyrics.Show 1
  ChangeEntry aktItem
End Sub

Private Sub mnuPlayFile_Click()
  LaunchDocument Datei, Me, True
End Sub

Private Sub mnuTag_Edit_Click()
  frmUnitag.Show 1
  ChangeEntry aktItem
End Sub

Private Sub tBar_ButtonClick(ByVal Button As ComctlLib.Button)
  Dim drive$, ti#
  drive$ = Button.Caption & "\"
  On Error Resume Next
  ti# = Timer
  ChDir drive$
  If Err = 75 Then
    MsgBox "Es befindet sich kein Datenträger in " & drive$, vbCritical, "Datenträger einlegen"
    On Error GoTo 0
    Exit Sub
  Else
    If Err > 0 Then
      MsgBox "Es ist ein Fehler aufgetreten:" & vbCrLf & Error(Err), vbCritical, "Fehler bei der Laufwerk-Analyse"
      On Error GoTo 0
      Exit Sub
    End If
  End If
  DoEvents
  Call ReadDrive(drive$)
  Set folderView.SelectedItem = folderView.Nodes(1).Root
  On Error GoTo 0
  folderView_NodeClick folderView.Nodes(1)
  frmMain.stBar.Panels(3).text = format$(Timer - ti#, "0.0") & " sec"
End Sub

Private Sub folderView_Expand(ByVal Node As ComctlLib.Node)
  If Screen.MousePointer = vbHourglass Then Exit Sub
  Dim x&, ti#
  Screen.MousePointer = vbHourglass
  ti# = Timer
  With folderView
    For x = Node.Child.FirstSibling.Index To _
            Node.Child.LastSibling.Index

      If .Nodes(x).Tag <> "*" Then
        Call GetFolders(.Nodes(x).key)
        folderView.Nodes(x).Tag = "*"
      End If
    Next x
  End With
  frmMain.stBar.Panels(3).text = format$(Timer - ti#, "0.0") & " sec"
  Screen.MousePointer = vbCustom
End Sub

