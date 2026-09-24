Option Strict Off
Option Explicit On
Imports VB = Microsoft.VisualBasic
Friend Class frmID3V2
	Inherits System.Windows.Forms.Form
	Dim ID, w As Short
	Dim Bilddatei, tmp As String
	Dim syncItem, picItem As ComctlLib.ListItem
	
	Private Sub cmdAddSyncEntry_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdAddSyncEntry.Click
		frmSYLT.ShowDialog()
		If frmSYLT.chk.CheckState = 1 Then
			listSYLT.Items.Add("[" & VB6.Format(Val(frmSYLT.txtTime.Text), "00000000") & "]" & Trim(frmSYLT.txtEntry.Text))
		End If
	End Sub
	
	Private Sub cmdAddSyncLyric_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdAddSyncLyric.Click
		syncItem = viewSYLTDescr.ListItems.Add( ,  , "<Beschreibung hier eingeben>", 0, 0)
		viewSYLTDescr.SelectedItem = syncItem
		viewSYLTDescr_ItemClick(viewSYLTDescr, New AxComctlLib.ListViewEvents_ItemClickEvent(syncItem))
		viewSYLTDescr.Focus()
		viewSYLTDescr.StartLabelEdit()
	End Sub
	
	Private Sub cmdCancel_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdCancel.Click
		Hide()
	End Sub
	
	Private Sub cmdDelAllFrames_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelAllFrames.Click
		genie.ID3V2DeleteUserFrames()
		updateFrames()
	End Sub
	
	Private Sub cmdDelAllLyrics_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelAllLyrics.Click
		genie.ID3V2DeleteLyrics()
		updateLyrics()
	End Sub
	
	Private Sub cmdDelAllPics_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelAllPics.Click
		genie.ID3V2DeletePictures()
		'UPGRADE_NOTE: Das Objekt frmAPIC.img.Picture kann erst dann gelöscht werden, wenn die Garbagecollection durchgeführt wurde. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"'
		frmAPIC.img.Image = Nothing
		updatePictures()
	End Sub
	
	Private Sub cmdDelAllSyncLyric_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelAllSyncLyric.Click
		genie.ID3V2DeleteSyncLyrics()
		updateSyncLyrics()
	End Sub
	
	Private Sub cmdDelAllURLs_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelAllURLs.Click
		genie.ID3V2DeleteUserURLs()
		updateURLs()
	End Sub
	
	Private Sub cmdDelAllComments_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelAllComments.Click
		genie.ID3V2DeleteComments()
		updateComments()
	End Sub
	
	Private Sub cmdDelComment_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelComment.Click
		Call genie.ID3V2DeleteSelectedFrame(clsAudioGenie.ID3V2FRAMES.ID3F_COMM, lfdNr1.SelectedIndex + 1)
		updateComments()
	End Sub
	
	Private Sub cmdDelLyric_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelLyric.Click
		Call genie.ID3V2DeleteSelectedFrame(clsAudioGenie.ID3V2FRAMES.ID3F_USLT, lfdNr2.SelectedIndex + 1)
		updateLyrics()
	End Sub
	
	Private Sub cmdDelPic_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelPic.Click
		picItem = picimgview.SelectedItem
		If picItem Is Nothing Then Exit Sub
		ID = Val(picItem.SubItems(3))
		genie.ID3V2DeletePicture(ID)
		picimgview.ListItems.Remove(picItem.Index)
	End Sub
	
	Private Sub cmdDelSyncEntry_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelSyncEntry.Click
		If listSYLT.SelectedIndex = -1 Then Exit Sub
		listSYLT.Items.RemoveAt(listSYLT.SelectedIndex)
	End Sub
	
	Private Sub cmdDelSyncLyric_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelSyncLyric.Click
		syncItem = viewSYLTDescr.SelectedItem
		If syncItem Is Nothing Then Exit Sub
		ID = syncItem.Index
		viewSYLTDescr.ListItems.Remove(ID)
		Call genie.ID3V2AddSyncLyric("DEU", "", "", 0, 1)
		updateSyncLyrics()
	End Sub
	
	Private Sub cmdDelURL_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelURL.Click
		Call genie.ID3V2DeleteSelectedFrame(clsAudioGenie.ID3V2FRAMES.ID3F_WXXX, lfdNr3.SelectedIndex + 1)
		updateURLs()
	End Sub
	
	Private Sub cmdDelFrame_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelFrame.Click
		Call genie.ID3V2DeleteSelectedFrame(clsAudioGenie.ID3V2FRAMES.ID3F_USER, lfdNr4.SelectedIndex + 1)
		updateFrames()
	End Sub
	
	Private Sub cmdEditSyncEntry_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdEditSyncEntry.Click
		Dim entry As String
		If listSYLT.SelectedIndex = -1 Then Exit Sub
		entry = VB6.GetItemString(listSYLT, listSYLT.SelectedIndex)
		frmSYLT.txtTime.Text = CStr(Val(Mid(entry, 2, 8)))
		frmSYLT.txtEntry.Text = Mid(entry, 11)
		frmSYLT.ShowDialog()
		If frmSYLT.chk.CheckState = 1 Then
			entry = "[" & VB6.Format(Val(frmSYLT.txtTime.Text), "00000000") & "]" & Trim(frmSYLT.txtEntry.Text)
			VB6.SetItemString(listSYLT, listSYLT.SelectedIndex, entry)
		End If
	End Sub
	
	Private Sub cmdEditSyncLyric_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdEditSyncLyric.Click
		syncItem = viewSYLTDescr.SelectedItem
		If syncItem Is Nothing Then Exit Sub
		viewSYLTDescr.Focus()
		viewSYLTDescr.StartLabelEdit()
	End Sub
	
	Private Sub cmdNewPic_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdNewPic.Click
		' Bild hinzufügen
		On Error GoTo ErrHandler
		' Dialogfeld "Öffnen" anzeigen
		Me.StandardDlgOpen.ShowDialog()
		Bilddatei = Me.StandardDlgOpen.FileName
		On Error GoTo WrongPicture
		frmAPIC.img.Image = System.Drawing.Image.FromFile(Bilddatei)
		'UPGRADE_WARNING: Die Standardeigenschaft des Objekts genie.ID3V2GetPictures konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		frmAPIC.idInfo.Text = CStr(genie.ID3V2GetPictures() + 1)
		frmAPIC.dateiInfo.Text = Bilddatei
		frmAPIC.linkCheck.CheckState = System.Windows.Forms.CheckState.Unchecked
		frmAPIC.picDescription.Enabled = True
		frmAPIC.picTypeCombo.Enabled = True
		frmAPIC.linkCheck.Enabled = True
		frmAPIC.cmdOk.Enabled = True
		frmAPIC.picMime.Text = ""
		frmAPIC.ShowDialog()
		Exit Sub
ErrHandler: 
		Debug.Print(Err.Description)
		' Benutzer hat Schaltfläche "Abbrechen" gewählt
		Exit Sub
WrongPicture: 
		MsgBox("Das Bild konnte nicht geladen werden oder ist ungültig", MsgBoxStyle.Critical)
		Exit Sub
	End Sub
	
	Private Sub cmdOKComm_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdOKComm.Click
		ID = Val(lfdNr1.Text)
		Call genie.ID3V2AddComment(txtLanguage1.Text, txtComment1.Text, txtCOMM.Text)
		updateComments()
	End Sub
	
	Private Sub cmdOKLyric_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdOKLyric.Click
		ID = Val(lfdNr2.Text)
		Call genie.ID3V2AddLyric(txtLanguage2.Text, txtComment2.Text, txtUSLT.Text)
		updateLyrics()
	End Sub
	
	Private Sub cmdOkSyncLyric_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdOkSyncLyric.Click
		syncItem = viewSYLTDescr.SelectedItem
		If syncItem Is Nothing Then Exit Sub
		tmp = ""
		For w = 0 To listSYLT.Items.Count - 2
			tmp = tmp & VB6.GetItemString(listSYLT, w) & vbCrLf
		Next w
		tmp = tmp & VB6.GetItemString(listSYLT, w)
		If genie.ID3V2AddSyncLyric(txtLanguage5.Text, syncItem.text, tmp, comboContentType.SelectedIndex, comboTimes.SelectedIndex + 1) = False Then
			MsgBox("Beim Eintragen ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, MsgBoxStyle.Critical, "Fehler beim Eintragen des SyncLyric-Tag")
		End If
		'  updateSyncLyrics
	End Sub
	
	Private Sub cmdOKURL_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdOKURL.Click
		Call genie.ID3V2AddUserURL(lfdNr3.Text, txtWXXX.Text)
		updateURLs()
	End Sub
	
	Private Sub cmdOKFrame_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdOKFrame.Click
		Call genie.ID3V2AddUserFrame(lfdNr4.Text, txtUSER.Text)
		updateFrames()
	End Sub
	
	Private Sub cmdParam_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdParam.Click
		frmID3Settings.ShowDialog()
	End Sub
	
	Private Sub cmdShowPic_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdShowPic.Click
		If picimgview.SelectedItem Is Nothing Then Exit Sub
		frmAPIC.picDescription.Enabled = False
		frmAPIC.picTypeCombo.Enabled = False
		frmAPIC.linkCheck.Enabled = False
		frmAPIC.cmdOk.Enabled = False
		frmAPIC.ShowDialog()
	End Sub
	
	Private Sub cmdUpdate_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdUpdate.Click
		genie.ID3V2Artist = Me.txtTPE1.Text
		genie.ID3V2Album = Me.txtTALB.Text
		genie.ID3V2Title = Me.txtTIT2.Text
		genie.ID3V2Year = Me.txtTYER.Text
		genie.ID3V2Genre = Me.txtTCON.Text
		genie.ID3V2Track = Me.txtTRCK.Text
		genie.ID3V2RecordingTime = Me.txtTDRC.Text
		genie.ID3V2Category = Me.txtTIT1.Text
		genie.ID3V2Language = Me.txtTLAN.Text
		genie.ID3V2Encoder = Me.txtTENC.Text
		genie.ID3V2Copyright = Me.txtTCOP.Text
		genie.ID3V2Composer = Me.txtTCOM.Text
		genie.ID3V2Orchestra = Me.txtTPE2.Text
		genie.ID3V2EncodingSettings = Me.txtTSSE.Text
		genie.ID3V2BeatsPerMinute = Me.txtTBPM.Text
		genie.ID3V2FileOwner = Me.txtTOWN.Text
		genie.ID3V2PlayCounter = Val(Me.txtPCNT.Text)
		' URLs
		genie.ID3V2CommercialURL = Me.txtWCOM.Text
		genie.ID3V2CopyrightURL = Me.txtWCOP.Text
		genie.ID3V2AudiofileURL = Me.txtWOAF.Text
		genie.ID3V2ArtistURL = Me.txtWOAR.Text
		genie.ID3V2AudiosourceURL = Me.txtWOAS.Text
		genie.ID3V2RadiostationURL = Me.txtWORS.Text
		genie.ID3V2PaymentURL = Me.txtWPAY.Text
		genie.ID3V2PublisherURL = Me.txtWPUB.Text
		' Tag speichern
		If genie.ID3V2SaveChangesToFile(Datei) = False Then
			MsgBox("Beim Speichern ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, MsgBoxStyle.Critical, "Fehler beim Speichern des ID3v2-Tag")
		End If
	End Sub
	
	Private Sub cmdDelete_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelete.Click
		DeleteID3v2Tag()
		updateUI()
	End Sub
	
	Public Sub updateUI()
		Dim ID As Short
		Dim Description, Language, Txt As String
		' ID3v2-Tag-Daten in Formular übertragen
		Me.txtTPE1.Text = genie.ID3V2Artist
		Me.txtTALB.Text = genie.ID3V2Album
		Me.txtTIT2.Text = genie.ID3V2Title
		Me.txtTYER.Text = genie.ID3V2Year
		Me.txtTCON.Text = genie.ID3V2Genre
		Me.txtTRCK.Text = genie.ID3V2Track
		Me.txtTDRC.Text = genie.ID3V2RecordingTime
		Me.txtTIT1.Text = genie.ID3V2Category
		Me.txtTLAN.Text = genie.ID3V2Language
		Me.txtTENC.Text = genie.ID3V2Encoder
		Me.txtTCOP.Text = genie.ID3V2Copyright
		Me.txtTCOM.Text = genie.ID3V2Composer
		Me.txtTPE2.Text = genie.ID3V2Orchestra
		Me.txtTSSE.Text = genie.ID3V2EncodingSettings
		Me.txtTBPM.Text = genie.ID3V2BeatsPerMinute
		Me.txtTOWN.Text = genie.ID3V2FileOwner
		Me.txtPCNT.Text = CStr(genie.ID3V2PlayCounter)
		' URLs
		Me.txtWCOM.Text = genie.ID3V2CommercialURL
		Me.txtWCOP.Text = genie.ID3V2CopyrightURL
		Me.txtWOAF.Text = genie.ID3V2AudiofileURL
		Me.txtWOAR.Text = genie.ID3V2ArtistURL
		Me.txtWOAS.Text = genie.ID3V2AudiosourceURL
		Me.txtWORS.Text = genie.ID3V2RadiostationURL
		Me.txtWPAY.Text = genie.ID3V2PaymentURL
		Me.txtWPUB.Text = genie.ID3V2PublisherURL
		
		' falls alte Version, nicht unterstützte Tags deaktivieren
		If genie.ID3V2GetVersion = "2.2" Then
			Me.txtTOWN.Enabled = False
			Me.txtTOWN.BackColor = System.Drawing.ColorTranslator.FromOle(&H80000003)
			Me.txtWORS.Enabled = False
			Me.txtWORS.BackColor = System.Drawing.ColorTranslator.FromOle(&H80000003)
			Me.txtWPAY.Enabled = False
			Me.txtWPAY.BackColor = System.Drawing.ColorTranslator.FromOle(&H80000003)
			Me.register.TabPages.Item(5).Enabled = False
		Else
			Me.txtTOWN.Enabled = True
			Me.txtTOWN.BackColor = System.Drawing.ColorTranslator.FromOle(&H80000005)
			Me.txtWORS.Enabled = True
			Me.txtWORS.BackColor = System.Drawing.ColorTranslator.FromOle(&H80000005)
			Me.txtWPAY.Enabled = True
			Me.txtWPAY.BackColor = System.Drawing.ColorTranslator.FromOle(&H80000005)
			Me.register.TabPages.Item(5).Enabled = True
		End If
		Me.updatePictures()
		Me.updateComments()
		Me.updateLyrics()
		Me.updateURLs()
		Me.updateFrames()
		Me.updateSyncLyrics()
	End Sub
	
	Private Sub frmID3V2_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		comboTimes.Items.Clear()
		comboTimes.Items.Add("Mpeg Frames")
		comboTimes.Items.Add("Millisekunden")
		comboContentType.Items.Clear()
		comboContentType.Items.Add("Andere")
		comboContentType.Items.Add("Songtexte")
		comboContentType.Items.Add("Texte")
		comboContentType.Items.Add("Bewegungen")
		comboContentType.Items.Add("Ereignisse")
		comboContentType.Items.Add("Akkorde")
		comboContentType.Items.Add("Belangloses")
		comboContentType.Items.Add("URLS zu Webseiten")
		comboContentType.Items.Add("URLS zu Bildern")
	End Sub
	
	'UPGRADE_WARNING: Das Ereignis lfdNr1.SelectedIndexChanged kann ausgelöst werden, wenn das Formular initialisiert wird. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub lfdNr1_SelectedIndexChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles lfdNr1.SelectedIndexChanged
		ID = Val(VB6.GetItemString(lfdNr1, lfdNr1.SelectedIndex))
		txtLanguage1.Text = genie.ID3V2GetCommentLanguage(ID)
		txtComment1.Text = genie.ID3V2GetCommentDescription(ID)
		txtCOMM.Text = genie.ID3V2GetComment(ID)
	End Sub
	
	'UPGRADE_WARNING: Das Ereignis lfdNr2.SelectedIndexChanged kann ausgelöst werden, wenn das Formular initialisiert wird. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub lfdNr2_SelectedIndexChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles lfdNr2.SelectedIndexChanged
		ID = Val(VB6.GetItemString(lfdNr2, lfdNr2.SelectedIndex))
		txtLanguage2.Text = genie.ID3V2GetLyricLanguage(ID)
		txtComment2.Text = genie.ID3V2GetLyricDescription(ID)
		txtUSLT.Text = genie.ID3V2GetLyric(ID)
	End Sub
	
	'UPGRADE_WARNING: Das Ereignis lfdNr3.SelectedIndexChanged kann ausgelöst werden, wenn das Formular initialisiert wird. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub lfdNr3_SelectedIndexChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles lfdNr3.SelectedIndexChanged
		txtWXXX.Text = genie.ID3V2GetUserURL(lfdNr3.SelectedIndex + 1)
	End Sub
	
	'UPGRADE_WARNING: Das Ereignis lfdNr4.SelectedIndexChanged kann ausgelöst werden, wenn das Formular initialisiert wird. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub lfdNr4_SelectedIndexChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles lfdNr4.SelectedIndexChanged
		txtUSER.Text = genie.ID3V2GetUserFrame(lfdNr4.SelectedIndex + 1)
	End Sub
	
	Public Sub updateComments()
		' Liste alle Kommentar-Tags auf
		txtCOMM.Text = ""
		txtLanguage1.Text = ""
		txtComment1.Text = ""
		lfdNr1.Items.Clear()
		For ID = 1 To genie.ID3V2GetComments()
			lfdNr1.Items.Add(CStr(ID))
		Next ID
		If lfdNr1.Items.Count > 0 Then lfdNr1.SelectedIndex = 0
	End Sub
	
	Public Sub updateLyrics()
		' Liste alle Lyrics-Tags auf
		txtUSLT.Text = ""
		txtLanguage2.Text = ""
		txtComment2.Text = ""
		lfdNr2.Items.Clear()
		For ID = 1 To genie.ID3V2GetLyrics()
			lfdNr2.Items.Add(CStr(ID))
		Next ID
		If lfdNr2.Items.Count > 0 Then lfdNr2.SelectedIndex = 0
	End Sub
	
	Public Sub updateSyncLyrics()
		' Liste alle SyncLyrics-Tags auf
		viewSYLTDescr.ListItems.Clear()
		txtLanguage5.Text = ""
		listSYLT.Items.Clear()
		For ID = 1 To genie.ID3V2GetSyncLyrics()
			viewSYLTDescr.ListItems.Add(ID,  , genie.ID3V2GetSyncLyricDescription(ID), 0, 0)
		Next ID
		If genie.ID3V2GetLyrics() > 0 Then
			viewSYLTDescr.SelectedItem = viewSYLTDescr.ListItems(1)
			viewSYLTDescr_ItemClick(viewSYLTDescr, New AxComctlLib.ListViewEvents_ItemClickEvent(viewSYLTDescr.ListItems(1)))
			Frame2.Visible = True
		Else
			Frame2.Visible = False
		End If
	End Sub
	
	Public Sub updateURLs()
		' Liste alle URL-Tags auf
		txtWXXX.Text = ""
		lfdNr3.Items.Clear()
		For ID = 1 To genie.ID3V2GetUserURLs()
			lfdNr3.Items.Add(genie.ID3V2GetUserURLDescription(ID))
		Next ID
		If lfdNr3.Items.Count > 0 Then lfdNr3.SelectedIndex = 0
	End Sub
	
	Public Sub updateFrames()
		' Liste alle URL-Tags auf
		txtUSER.Text = ""
		lfdNr4.Items.Clear()
		For ID = 1 To genie.ID3V2GetUserFrames()
			lfdNr4.Items.Add(genie.ID3V2GetUserFrameLanguage(ID))
		Next ID
		If lfdNr4.Items.Count > 0 Then lfdNr4.SelectedIndex = 0
	End Sub
	
	Private Sub updateSyncEntrys()
		Dim time_id, content_id As Short
		Dim arr() As String
		listSYLT.Items.Clear()
		' hole aktives Element
		syncItem = viewSYLTDescr.SelectedItem
		If syncItem Is Nothing Then Exit Sub
		ID = syncItem.Index
		txtLanguage5.Text = genie.ID3V2GetSyncLyricLanguage(ID)
		If txtLanguage5.Text = "" Then txtLanguage5.Text = "deu"
		time_id = genie.ID3V2GetSyncLyricTimeFormat(ID)
		If time_id = -1 Then time_id = 2
		content_id = genie.ID3V2GetSyncLyricContentType(ID)
		If content_id = -1 Then content_id = 0
		comboContentType.SelectedIndex = content_id
		comboTimes.SelectedIndex = time_id - 1
		' überführe String in Listeneinträge
		tmp = genie.ID3V2GetSyncLyric(ID)
		arr = Split(tmp, vbCrLf, -1)
		For w = 0 To UBound(arr)
			listSYLT.Items.Add(arr(w))
		Next w
		Frame2.Visible = True
	End Sub
	
	Public Sub updatePictures()
		picimgview.ListItems.Clear()
		'UPGRADE_NOTE: Das Objekt picimgview.Icons kann erst dann gelöscht werden, wenn die Garbagecollection durchgeführt wurde. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"'
		picimgview.Icons = Nothing
		picimglist.ListImages.Clear()
		picimglist.ImageHeight = 96
		picimglist.ImageWidth = 96
		picimglist.UseMaskColor = False
		picimglist.BackColor = System.Drawing.Color.Blue
		'UPGRADE_WARNING: Die Standardeigenschaft des Objekts genie.ID3V2GetPictures konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		If genie.ID3V2GetPictures() = 0 Then Exit Sub
		
		'UPGRADE_WARNING: Die Standardeigenschaft des Objekts genie.ID3V2GetPictures konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		For ID = 1 To genie.ID3V2GetPictures()
			If ID3V2SetPictureWithArray(ID, (frmAPIC.img)) = False Then
				frmAPIC.img.Image = noPicture.Image
			End If
			picimglist.ListImages.Add( ,  , frmAPIC.img)
			picimgview.Icons = picimglist
			picItem = picimgview.ListItems.Add( ,  , genie.ID3V2GetPictureDescription(ID), picimglist.ListImages.count, 0)
			picItem.SubItems(1) = CStr(genie.ID3V2GetPictureType(ID))
			picItem.SubItems(2) = genie.ID3V2GetPictureMime(ID)
			picItem.SubItems(3) = CStr(ID)
		Next ID
		'UPGRADE_WARNING: Die Standardeigenschaft des Objekts picimgview.Icons konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		picimgview.Icons = picimglist.GetOCX
	End Sub
	
	Private Sub picimgview_ItemClick(ByVal eventSender As System.Object, ByVal eventArgs As AxComctlLib.ListViewEvents_ItemClickEvent) Handles picimgview.ItemClick
		frmAPIC.img.Image = picimglist.ListImages(eventArgs.item.Index).Picture
		frmAPIC.idInfo.Text = eventArgs.item.SubItems(3)
		frmAPIC.picDescription.Text = eventArgs.item.text
		frmAPIC.picTypeCombo.SelectedIndex = eventArgs.item.SubItems(1)
		frmAPIC.picMime.Text = eventArgs.item.SubItems(2)
		If VB.Left(frmAPIC.picMime.Text, 3) = "-->" Then
			frmAPIC.linkCheck.CheckState = System.Windows.Forms.CheckState.Checked
		Else
			frmAPIC.linkCheck.CheckState = System.Windows.Forms.CheckState.Unchecked
		End If
	End Sub
	
	Private Sub viewSYLTDescr_ItemClick(ByVal eventSender As System.Object, ByVal eventArgs As AxComctlLib.ListViewEvents_ItemClickEvent) Handles viewSYLTDescr.ItemClick
		syncItem = eventArgs.item
		updateSyncEntrys()
	End Sub
	
	Public Function ID3V2SetPictureWithArray(ByVal Index As Short, ByRef img As System.Windows.Forms.PictureBox) As Boolean
		Dim IID_IPicture(3) As Integer
		Dim oPicture As System.Drawing.Image
		Dim oStream As stdole.IUnknown
		Dim F As Short
		Dim Data() As Byte
		
		IID_IPicture(0) = &H7BF80980
		IID_IPicture(1) = &H101ABF32
		IID_IPicture(2) = &HAA00BB8B
		IID_IPicture(3) = &HAB0C3000
		On Error GoTo BildFehler
		Dim ln As Integer
		Dim Result() As Byte
		
		Result = genie.ID3V2GetPictureArray(Index)
		
		' Wahlweise folgender Aufruf ohne SAFEARRAY
		'Result = genie.ID3V2CopyPictureArray(Index)
		
		If UBound(Result) = 0 Then
			img.Image = noPicture.Image
		Else
			Call CreateStreamOnHGlobal(Result(0), False, oStream)
			Call OleLoadPicture(oStream, 0, 0, IID_IPicture(0), oPicture)
			img.Image = oPicture
		End If
		ID3V2SetPictureWithArray = True
		Exit Function
BildFehler: 
		ID3V2SetPictureWithArray = False
		On Error GoTo 0
	End Function
End Class