Option Strict Off
Option Explicit On
Friend Class frmID3Settings
	Inherits System.Windows.Forms.Form
	Private Sub cmdCancel_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdCancel.Click
		Hide()
	End Sub
	
	Private Sub cmdOkay_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdOkay.Click
		Dim w As Short
		Dim speicherFormat, textEncoding As Short
		For w = 0 To 3
			If optionFormat(w).Checked = True Then speicherFormat = w
			If optionEncoding(w).Checked = True Then textEncoding = w
		Next w
		If genie.ID3V2SetFormatAndEncoding(speicherFormat, textEncoding) = clsAudioGenie.Ausfuehrung.FEHLERHAFT Then
			MsgBox("Kombination von Format und Encoding nicht erlaubt!")
			Exit Sub
		End If
		Hide()
	End Sub
End Class