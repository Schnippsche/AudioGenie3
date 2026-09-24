Option Strict Off
Option Explicit On
Friend Class frmUnitag
	Inherits System.Windows.Forms.Form
	Private Sub cmdCancel_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdCancel.Click
		Hide()
	End Sub
	
	Private Sub cmdUpdate_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdUpdate.Click
		genie.AUDIOArtist = txtArtist.Text
		genie.AUDIOAlbum = txtAlbum.Text
		genie.AUDIOTitle = txtTitle.Text
		genie.AUDIOYear = txtYear.Text
		genie.AUDIOComment = txtComment.Text
		genie.AUDIOTrack = txtTrack.Text
		genie.AUDIOGenre = txtGenre.Text
		If genie.AUDIOSaveChangesToFile(Datei) = False Then
			MsgBox("Beim Speichern ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, MsgBoxStyle.Critical, "Fehler beim Speichern der Änderungen")
		End If
	End Sub
	
	'UPGRADE_WARNING: Form Ereignis frmUnitag.Activate hat ein neues Verhalten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6BA9B8D2-2A32-4B6E-8D36-44949974A5B4"'
	Private Sub frmUnitag_Activated(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Activated
		txtArtist.Text = genie.AUDIOArtist
		txtAlbum.Text = genie.AUDIOAlbum
		txtTitle.Text = genie.AUDIOTitle
		txtYear.Text = genie.AUDIOYear
		txtComment.Text = genie.AUDIOComment
		txtTrack.Text = genie.AUDIOTrack
		txtGenre.Text = genie.AUDIOGenre
		'UPGRADE_WARNING: Die Standardeigenschaft des Objekts genie.WMAGetPictures konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		If genie.WMAGetPictures > 0 Then
			Debug.Print(genie.WMAGetPictureDescription(1))
			Debug.Print(genie.WMAGetPictureMime(1))
		End If
		
	End Sub
End Class