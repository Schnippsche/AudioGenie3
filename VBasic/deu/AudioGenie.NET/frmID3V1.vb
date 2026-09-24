Option Strict Off
Option Explicit On
Friend Class frmID3v1
	Inherits System.Windows.Forms.Form
	Private Sub cmdCancel_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdCancel.Click
		Hide()
	End Sub
	
	Private Sub cmdUpdate_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdUpdate.Click
		' Daten aus den Eingabefeldern in die Struktur übertragen
		genie.ID3V1Artist = txtArtist.Text
		genie.ID3V1Album = txtAlbum.Text
		genie.ID3V1Title = txtTitle.Text
		genie.ID3V1Year = txtYear.Text
		genie.ID3V1Comment = txtComment.Text
		genie.ID3V1Track = txtTrack.Text
		If genreCombo.SelectedIndex >= 0 Then
			genie.ID3V1GenreID = VB6.GetItemData(genreCombo, genreCombo.SelectedIndex)
		End If
		If genie.ID3V1SaveChangesToFile(Datei) = False Then
			MsgBox("Beim Speichern ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, MsgBoxStyle.Critical, "Fehler beim Speichern des ID3v1-Tag")
		End If
	End Sub
	
	Private Sub cmdDelete_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdDelete.Click
		DeleteID3v1Tag()
		updateUI()
	End Sub
	
	'UPGRADE_WARNING: Form Ereignis frmID3v1.Activate hat ein neues Verhalten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6BA9B8D2-2A32-4B6E-8D36-44949974A5B4"'
	Private Sub frmID3v1_Activated(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Activated
		updateUI()
	End Sub
	
	Private Sub frmID3v1_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		' Einmalig die Genre Liste füllen
		Dim i As Short
		genreCombo.Items.Clear()
		genreCombo.Items.Add(New VB6.ListBoxItem("(kein Genre gewählt)", 255))
		For i = 0 To genie.ID3V1GetGenres - 1
			genreCombo.Items.Add(New VB6.ListBoxItem(genie.ID3V1GetGenreItem(i), i))
		Next i
	End Sub
	
	Private Sub updateUI()
		Dim i, g As Short
		txtArtist.Text = genie.ID3V1Artist
		txtAlbum.Text = genie.ID3V1Album
		txtTitle.Text = genie.ID3V1Title
		txtYear.Text = genie.ID3V1Year
		txtComment.Text = genie.ID3V1Comment
		txtTrack.Text = genie.ID3V1Track
		g = genie.ID3V1GenreID
		For i = 0 To genreCombo.Items.Count - 1
			If VB6.GetItemData(genreCombo, i) = g Then genreCombo.SelectedIndex = i
		Next i
	End Sub
End Class