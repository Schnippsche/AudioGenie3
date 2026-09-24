Option Strict Off
Option Explicit On
Friend Class frmAPE
	Inherits System.Windows.Forms.Form
	Private Sub cmdCancel_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdCancel.Click
		Hide()
	End Sub
	
	Private Sub cmdUpdate_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdUpdate.Click
		' Daten aus den Eingabefeldern in die Struktur übertragen
		genie.APEArtist = txtArtist.Text
		genie.APEAlbum = txtAlbum.Text
		genie.APETitle = txtTitle.Text
		genie.APEYear = txtYear.Text
		genie.APEComment = txtComment.Text
		genie.APEGenre = txtGenre.Text
		genie.APECopyright = txtCopyright.Text
		genie.APETrack = txtTrack.Text
		If genie.APESaveChangesToFile(Datei) = False Then
			MsgBox("Beim Speichern ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, MsgBoxStyle.Critical, "Fehler beim Speichern des APE-Tag")
		End If
	End Sub
	
	Private Sub cmdDelete_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelete.Click
		DeleteAPETag()
		updateUI()
	End Sub
	
	'UPGRADE_WARNING: Form Ereignis frmAPE.Activate hat ein neues Verhalten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6BA9B8D2-2A32-4B6E-8D36-44949974A5B4"'
	Private Sub frmAPE_Activated(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Activated
		updateUI()
	End Sub
	
	Private Sub updateUI()
		Me.txtTitle.Text = genie.APETitle
		Me.txtArtist.Text = genie.APEArtist
		Me.txtAlbum.Text = genie.APEAlbum
		Me.txtGenre.Text = genie.APEGenre
		Me.txtYear.Text = genie.APEYear
		Me.txtTrack.Text = genie.APETrack
		Me.txtComment.Text = genie.APEComment
		Me.txtCopyright.Text = genie.APECopyright
	End Sub
End Class