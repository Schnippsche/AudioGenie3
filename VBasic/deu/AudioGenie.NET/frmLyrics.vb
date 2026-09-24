Option Strict Off
Option Explicit On
Friend Class frmLyrics
	Inherits System.Windows.Forms.Form
	
	Private Sub cmdCancel_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdCancel.Click
		Hide()
	End Sub
	
	Private Sub cmdDelete_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelete.Click
		DeleteLyricsTag()
		updateUI()
	End Sub
	
	Private Sub cmdUpdate_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdUpdate.Click
		genie.LYRICSArtist = txtArtist.Text
		genie.LYRICSAlbum = txtAlbum.Text
		genie.LYRICSTitle = txtTitle.Text
		genie.LYRICSLyrics = txtLyrics.Text
		genie.LYRICSInformation = txtInformation.Text
		genie.LYRICSGenre = txtGenre.Text
		genie.LYRICSImageLink = txtImageLink.Text
		genie.LYRICSAuthor = txtAuthor.Text
		If genie.LYRICSSaveChangesToFile(Datei) = False Then
			MsgBox("Beim Speichern ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, MsgBoxStyle.Critical, "Fehler beim Speichern des Lyrics-Tag")
		End If
	End Sub
	
	'UPGRADE_WARNING: Form Ereignis frmLyrics.Activate hat ein neues Verhalten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6BA9B8D2-2A32-4B6E-8D36-44949974A5B4"'
	Private Sub frmLyrics_Activated(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Activated
		updateUI()
	End Sub
	
	Private Sub updateUI()
		Me.txtArtist.Text = genie.LYRICSArtist
		Me.txtAlbum.Text = genie.LYRICSAlbum
		Me.txtTitle.Text = genie.LYRICSTitle
		Me.txtLyrics.Text = genie.LYRICSLyrics
		Me.txtInformation.Text = genie.LYRICSInformation
		Me.txtGenre.Text = genie.LYRICSGenre
		Me.txtImageLink.Text = genie.LYRICSImageLink
		Me.txtAuthor.Text = genie.LYRICSAuthor
	End Sub
End Class