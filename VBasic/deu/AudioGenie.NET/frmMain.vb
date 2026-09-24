Option Strict Off
Option Explicit On
Imports VB = Microsoft.VisualBasic
Friend Class frmMain
	Inherits System.Windows.Forms.Form
	
	Public Sub detailView_ColumnClick(ByVal eventSender As System.Object, ByVal eventArgs As AxComctlLib.ListViewEvents_ColumnClickEvent) Handles detailView.ColumnClick
		Dim col, art As Short
		col = eventArgs.ColumnHeader.SubItemIndex
		'UPGRADE_WARNING: Die Standardeigenschaft des Objekts ColumnHeader.Tag konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		art = eventArgs.ColumnHeader.Tag
		S_Order = S_Order Xor 1
		SortListView(detailView.hWnd, col, art, S_Order)
	End Sub
	
	Private Sub detailView_DblClick(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles detailView.DblClick
		If Len(Datei) < 1 Then Exit Sub
		LaunchDocument(Datei, Me, True)
	End Sub
	
	Private Sub detailView_ItemClick(ByVal eventSender As System.Object, ByVal eventArgs As AxComctlLib.ListViewEvents_ItemClickEvent) Handles detailView.ItemClick
		' Aktuell ausgewählte Datei analysieren
		aktItem = eventArgs.item
		Datei = eventArgs.item.SubItems(15)
		If Datei <> DateiAlt Then
			modAudioGenie.Dateiformat = genie.AUDIOAnalyzeFile(Datei)
			frmDetails.updateUI()
			DateiAlt = Datei
		End If
	End Sub
	
	Private Sub detailView_MouseUpEvent(ByVal eventSender As System.Object, ByVal eventArgs As AxComctlLib.ListViewEvents_MouseUpEvent) Handles detailView.MouseUpEvent
		If (eventArgs.Button = VB6.MouseButtonConstants.RightButton) Then
			If (Not aktItem Is Nothing) Then
				Select Case modAudioGenie.Dateiformat
					Case clsAudioGenie.AudioFormatID.AAC ' Id3v1+v2
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
						'UPGRADE_ISSUE: Die Konstante vbPopupMenuLeftButton wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
						'UPGRADE_ISSUE: Form Methode frmMain.PopupMenu wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="CC4C7EC0-C903-48FC-ACCC-81861D12DA4A"'
						PopupMenu(mnuPopup, vbPopupMenuLeftButton)
					Case clsAudioGenie.AudioFormatID.MPEG ' id3v1+2+lyrics
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
						'UPGRADE_ISSUE: Die Konstante vbPopupMenuLeftButton wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
						'UPGRADE_ISSUE: Form Methode frmMain.PopupMenu wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="CC4C7EC0-C903-48FC-ACCC-81861D12DA4A"'
						PopupMenu(mnuPopup, vbPopupMenuLeftButton)
					Case clsAudioGenie.AudioFormatID.MONKEY, clsAudioGenie.AudioFormatID.MPEGPLUS ' id3v1+v2+ape
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
						'UPGRADE_ISSUE: Die Konstante vbPopupMenuLeftButton wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
						'UPGRADE_ISSUE: Form Methode frmMain.PopupMenu wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="CC4C7EC0-C903-48FC-ACCC-81861D12DA4A"'
						PopupMenu(mnuPopup, vbPopupMenuLeftButton)
					Case clsAudioGenie.AudioFormatID.WAV
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
						'UPGRADE_ISSUE: Die Konstante vbPopupMenuLeftButton wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
						'UPGRADE_ISSUE: Form Methode frmMain.PopupMenu wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="CC4C7EC0-C903-48FC-ACCC-81861D12DA4A"'
						PopupMenu(mnuPopup, vbPopupMenuLeftButton)
					Case clsAudioGenie.AudioFormatID.FLAC, clsAudioGenie.AudioFormatID.MP4M4A, clsAudioGenie.AudioFormatID.OGGVORBIS, clsAudioGenie.AudioFormatID.WMA ' tag+id3v2
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
						'UPGRADE_ISSUE: Die Konstante vbPopupMenuLeftButton wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
						'UPGRADE_ISSUE: Form Methode frmMain.PopupMenu wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="CC4C7EC0-C903-48FC-ACCC-81861D12DA4A"'
						PopupMenu(mnuPopup, vbPopupMenuLeftButton)
				End Select
			Else
				' Kein Eintrag unter der Maus ggf. Alternativmenü anzeigen
			End If
		End If
	End Sub
	
	Private Sub folderView_NodeClick(ByVal eventSender As System.Object, ByVal eventArgs As AxComctlLib.ITreeViewEvents_NodeClickEvent) Handles folderView.NodeClick
		Dim Folder As String
		Dim ti As Double
		Folder = folderView.SelectedItem.key
		'UPGRADE_WARNING: Screen Eigenschaft Screen.MousePointer hat ein neues Verhalten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6BA9B8D2-2A32-4B6E-8D36-44949974A5B4"'
		'UPGRADE_WARNING: frmMain Eigenschaft VB.Global.Screen.MousePointer hat ein neues Verhalten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6BA9B8D2-2A32-4B6E-8D36-44949974A5B4"'
		If System.Windows.Forms.Cursor.Current.equals(System.Windows.Forms.Cursors.WaitCursor) Then Exit Sub
		'UPGRADE_WARNING: Screen Eigenschaft Screen.MousePointer hat ein neues Verhalten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6BA9B8D2-2A32-4B6E-8D36-44949974A5B4"'
		System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.WaitCursor
		ti = VB.Timer()
		GetFiles((Folder))
		Me.stBar.Panels(3).text = VB6.Format(VB.Timer() - ti, "0.0") & " sec"
		'UPGRADE_ISSUE: Die Screen-Eigenschaft Screen.MousePointer unterstützt keine benutzerdefinierten Mauszeiger. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="45116EAB-7060-405E-8ABE-9DBB40DC2E86"'
		'UPGRADE_WARNING: Screen Eigenschaft Screen.MousePointer hat ein neues Verhalten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6BA9B8D2-2A32-4B6E-8D36-44949974A5B4"'
		System.Windows.Forms.Cursor.Current = vbCustom
	End Sub
	
	'UPGRADE_NOTE: Form_Initialize wurde aktualisiert auf Form_Initialize_Renamed. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
	Private Sub Form_Initialize_Renamed()
		On Error Resume Next
		' nur für Windows XP, bei anderen BS kann die nachfolgende Zeile entfernt werden
		' Only for XP
		Call ActivateWindowTheme(Me.Handle.ToInt32)
		
		
		' Make some presets
		genie.ConfigValue(clsAudioGenie.CONFIG_VALUES.CF_ID3V2PADDINGSIZE) = 2048
		' Enable the checking of all frames is slower but exact
		genie.ConfigValue(clsAudioGenie.CONFIG_VALUES.CF_MPEGEXACTREAD) = 0 ' disabled
		genie.ConfigValue(clsAudioGenie.CONFIG_VALUES.CF_DOEVENTSMILLIS) = 500
	End Sub
	
	Private Sub frmMain_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		Dim drive As Object
		Dim DisplayName As String
		Dim nButton As ComctlLib.Button
		Dim typ As Short
		Dim FirstDrive, count As Short
		Dim ti As Double
		FirstDrive = 0
		count = 0
		' Über die Liste verfügbarer Laufwerke iterieren:
		For	Each drive In AvailableDriveLetters
			' Vom Laufwerkstyp abhängigen Klartextnamen definieren
			'UPGRADE_WARNING: Die Standardeigenschaft des Objekts drive konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			typ = DriveType(drive)
			count = count + 1
			If typ = modUni.DriveTypes.dtFixed And FirstDrive = 0 Then FirstDrive = count
			'UPGRADE_WARNING: Die Standardeigenschaft des Objekts drive konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			nButton = tBar.Buttons.Add( , drive, drive & ":", ComctlLib.ButtonStyleConstants.tbrButtonGroup, "d" & typ)
			If typ = modUni.DriveTypes.dtFixed Or typ = modUni.DriveTypes.dtRemote Or typ = modUni.DriveTypes.dtRamDisk Then
				'UPGRADE_WARNING: Die Standardeigenschaft des Objekts drive konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				'UPGRADE_WARNING: Die Standardeigenschaft des Objekts DriveInfo() konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				nButton.ToolTipText = DriveInfo(drive & ":") & " (" & drive & ":)"
			Else
				'UPGRADE_WARNING: Die Standardeigenschaft des Objekts drive konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				nButton.ToolTipText = drive & ":"
			End If
		Next drive
		System.Windows.Forms.Application.DoEvents()
		folderView.Top = VB6.TwipsToPixelsY(VB6.PixelsToTwipsY(tBar.Height) + 2)
		detailView.Top = VB6.TwipsToPixelsY(VB6.PixelsToTwipsY(tBar.Height) + 2)
		lblSplitter.Top = VB6.TwipsToPixelsY(VB6.PixelsToTwipsY(tBar.Height) + 2)
		lblSplitter.Height = Me.Height
		ti = VB.Timer()
		If FirstDrive > 0 Then
			nButton = tBar.Buttons(FirstDrive)
			nButton.value = ComctlLib.ValueConstants.tbrPressed
			tBar_ButtonClick(tBar, New AxComctlLib.IToolbarEvents_ButtonClickEvent(nButton))
		End If
		Me.stBar.Panels(3).text = VB6.Format(VB.Timer() - ti, "0.0") & " sec"
		With prgBar
			'Die Progressbar in die Statusbar setzen
			Call SetParent(.hWnd, stBar.hWnd)
			.Top = VB6.TwipsToPixelsY(60)
			.Left = VB6.TwipsToPixelsX(stBar.Panels(3).Left + 10)
			.Width = VB6.TwipsToPixelsX(stBar.Panels(3).Width - 80)
			.Height = VB6.TwipsToPixelsY(VB6.PixelsToTwipsY(stBar.Height) - 90)
		End With
		' Die Spalten für Sortierung anpassen
		detailView.ColumnHeaders(1).Tag = ListViewSort.SortTypes.stString
		detailView.ColumnHeaders(2).Tag = ListViewSort.SortTypes.stNumeric
		detailView.ColumnHeaders(3).Tag = ListViewSort.SortTypes.stString
		detailView.ColumnHeaders(4).Tag = ListViewSort.SortTypes.stString
		detailView.ColumnHeaders(5).Tag = ListViewSort.SortTypes.stString
		detailView.ColumnHeaders(6).Tag = ListViewSort.SortTypes.stString
		detailView.ColumnHeaders(7).Tag = ListViewSort.SortTypes.stString
		detailView.ColumnHeaders(8).Tag = ListViewSort.SortTypes.stString
		detailView.ColumnHeaders(9).Tag = ListViewSort.SortTypes.stString
		detailView.ColumnHeaders(10).Tag = ListViewSort.SortTypes.stString
		detailView.ColumnHeaders(11).Tag = ListViewSort.SortTypes.stNumeric
		detailView.ColumnHeaders(12).Tag = ListViewSort.SortTypes.stNumeric
		detailView.ColumnHeaders(13).Tag = ListViewSort.SortTypes.stNumeric
		detailView.ColumnHeaders(14).Tag = ListViewSort.SortTypes.stString
		detailView.ColumnHeaders(15).Tag = ListViewSort.SortTypes.stString
		detailView.ColumnHeaders(16).Tag = ListViewSort.SortTypes.stString
		Me.Text = "AudioGenie " & genie.GetAudioGenieVersion & " Beispielprojekt von Stefan Töngi"
	End Sub
	
	'UPGRADE_WARNING: Das Ereignis frmMain.Resize kann ausgelöst werden, wenn das Formular initialisiert wird. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub frmMain_Resize(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Resize
		Dim H, B As Short
		H = VB6.PixelsToTwipsY(Me.ClientRectangle.Height) - VB6.PixelsToTwipsY(tBar.Height) - VB6.PixelsToTwipsY(stBar.Height)
		B = VB6.PixelsToTwipsX(Me.ClientRectangle.Width)
		If H < 0 Or B < 0 Then Exit Sub
		folderView.Height = VB6.TwipsToPixelsY(H)
		detailView.Height = VB6.TwipsToPixelsY(H)
		detailView.Width = VB6.TwipsToPixelsX(B - VB6.PixelsToTwipsX(folderView.Width) - VB6.PixelsToTwipsX(lblSplitter.Width))
		detailView.Left = VB6.TwipsToPixelsX(VB6.PixelsToTwipsX(folderView.Width) + VB6.PixelsToTwipsX(lblSplitter.Width))
		lblSplitter.Left = folderView.Width
		lblSplitter.Height = VB6.TwipsToPixelsY(H)
		With prgBar
			'Die Progressbar in die Statusbar setzen
			.Top = VB6.TwipsToPixelsY(60)
			.Left = VB6.TwipsToPixelsX(stBar.Panels(3).Left + 10)
			.Width = VB6.TwipsToPixelsX(stBar.Panels(3).Width - 80)
			.Height = VB6.TwipsToPixelsY(VB6.PixelsToTwipsY(stBar.Height) - 90)
		End With
	End Sub
	
	Private Sub frmMain_FormClosed(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.FormClosedEventArgs) Handles Me.FormClosed
		End
	End Sub
	
	Private Sub lblSplitter_MouseDown(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles lblSplitter.MouseDown
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		lblSplitter.Visible = False
		oldXPos = 0
	End Sub
	
	Private Sub lblSplitter_MouseMove(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles lblSplitter.MouseMove
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		If Button = VB6.MouseButtonConstants.LeftButton Then
			newXPos = VB6.PixelsToTwipsX(lblSplitter.Left) + x
			If newXPos <> oldXPos And newXPos > 600 And newXPos < VB6.PixelsToTwipsX(Me.ClientRectangle.Width) - 600 Then
				' lblSplitter.Left = X
				folderView.Width = VB6.TwipsToPixelsX(newXPos)
				detailView.Left = VB6.TwipsToPixelsX(newXPos + VB6.PixelsToTwipsX(lblSplitter.Width))
				detailView.Width = VB6.TwipsToPixelsX(VB6.PixelsToTwipsX(Me.ClientRectangle.Width) - VB6.PixelsToTwipsX(detailView.Left))
				oldXPos = newXPos
			End If
		End If
	End Sub
	
	Private Sub lblSplitter_MouseUp(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles lblSplitter.MouseUp
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		lblSplitter.Left = folderView.Width
		lblSplitter.Visible = True
	End Sub
	
	Public Sub mnuAPE_Delete_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuAPE_Delete.Click
		DeleteAPETag()
		ChangeEntry(aktItem)
	End Sub
	
	Public Sub mnuAPE_Edit_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuAPE_Edit.Click
		frmAPE.ShowDialog()
		ChangeEntry(aktItem)
	End Sub
	
	Public Sub mnuAudioProperties_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuAudioProperties.Click
		frmDetails.Show()
	End Sub
	
	Public Sub mnuID3v1_Delete_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuID3v1_Delete.Click
		DeleteID3v1Tag()
		ChangeEntry(aktItem)
	End Sub
	
	Public Sub mnuID3v1_Edit_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuID3v1_Edit.Click
		frmID3v1.ShowDialog()
		ChangeEntry(aktItem)
	End Sub
	
	Public Sub mnuID3v2_Delete_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuID3v2_Delete.Click
		DeleteID3v2Tag()
		ChangeEntry(aktItem)
	End Sub
	
	Public Sub mnuID3v2_Edit_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuID3v2_Edit.Click
		frmID3V2.updateUI()
		frmID3V2.ShowDialog()
		ChangeEntry(aktItem)
	End Sub
	
	Public Sub mnuLyrics_Delete_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuLyrics_Delete.Click
		DeleteLyricsTag()
		ChangeEntry(aktItem)
	End Sub
	
	Public Sub mnuLyrics_Edit_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuLyrics_Edit.Click
		frmLyrics.ShowDialog()
		ChangeEntry(aktItem)
	End Sub
	
	Public Sub mnuPlayFile_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuPlayFile.Click
		LaunchDocument(Datei, Me, True)
	End Sub
	
	Public Sub mnuTag_Edit_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuTag_Edit.Click
		frmUnitag.ShowDialog()
		ChangeEntry(aktItem)
	End Sub
	
	Private Sub tBar_ButtonClick(ByVal eventSender As System.Object, ByVal eventArgs As AxComctlLib.IToolbarEvents_ButtonClickEvent) Handles tBar.ButtonClick
		Dim drive As String
		Dim ti As Double
		drive = eventArgs.Button.Caption & "\"
		On Error Resume Next
		ti = VB.Timer()
		ChDir(drive)
		If Err.Number = 75 Then
			MsgBox("Es befindet sich kein Datenträger in " & drive, MsgBoxStyle.Critical, "Datenträger einlegen")
			On Error GoTo 0
			Exit Sub
		Else
			If Err.Number > 0 Then
				MsgBox("Es ist ein Fehler aufgetreten:" & vbCrLf & ErrorToString(Err.Number), MsgBoxStyle.Critical, "Fehler bei der Laufwerk-Analyse")
				On Error GoTo 0
				Exit Sub
			End If
		End If
		System.Windows.Forms.Application.DoEvents()
		Call ReadDrive(drive)
		folderView.SelectedItem = folderView.Nodes(1).Root
		On Error GoTo 0
		folderView_NodeClick(folderView, New AxComctlLib.ITreeViewEvents_NodeClickEvent(folderView.Nodes(1)))
		Me.stBar.Panels(3).text = VB6.Format(VB.Timer() - ti, "0.0") & " sec"
	End Sub
	
	Private Sub folderView_Expand(ByVal eventSender As System.Object, ByVal eventArgs As AxComctlLib.ITreeViewEvents_ExpandEvent) Handles folderView.Expand
		'UPGRADE_WARNING: Screen Eigenschaft Screen.MousePointer hat ein neues Verhalten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6BA9B8D2-2A32-4B6E-8D36-44949974A5B4"'
		'UPGRADE_WARNING: frmMain Eigenschaft VB.Global.Screen.MousePointer hat ein neues Verhalten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6BA9B8D2-2A32-4B6E-8D36-44949974A5B4"'
		If System.Windows.Forms.Cursor.Current.equals(System.Windows.Forms.Cursors.WaitCursor) Then Exit Sub
		Dim x As Integer
		Dim ti As Double
		'UPGRADE_WARNING: Screen Eigenschaft Screen.MousePointer hat ein neues Verhalten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6BA9B8D2-2A32-4B6E-8D36-44949974A5B4"'
		System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.WaitCursor
		ti = VB.Timer()
		With folderView
			For x = eventArgs.Node.Child.FirstSibling.Index To eventArgs.Node.Child.LastSibling.Index
				
				If .Nodes(x).Tag <> "*" Then
					Call GetFolders((.Nodes(x).key))
					folderView.Nodes(x).Tag = "*"
				End If
			Next x
		End With
		Me.stBar.Panels(3).text = VB6.Format(VB.Timer() - ti, "0.0") & " sec"
		'UPGRADE_ISSUE: Die Screen-Eigenschaft Screen.MousePointer unterstützt keine benutzerdefinierten Mauszeiger. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="45116EAB-7060-405E-8ABE-9DBB40DC2E86"'
		'UPGRADE_WARNING: Screen Eigenschaft Screen.MousePointer hat ein neues Verhalten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6BA9B8D2-2A32-4B6E-8D36-44949974A5B4"'
		System.Windows.Forms.Cursor.Current = vbCustom
	End Sub
End Class