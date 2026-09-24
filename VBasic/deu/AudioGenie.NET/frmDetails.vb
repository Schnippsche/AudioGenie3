Option Strict Off
Option Explicit On
Friend Class frmDetails
	Inherits System.Windows.Forms.Form
	Private Sub cmdOk_Click()
		Hide()
	End Sub
	
	'UPGRADE_WARNING: Form Ereignis frmDetails.Activate hat ein neues Verhalten. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6BA9B8D2-2A32-4B6E-8D36-44949974A5B4"'
	Private Sub frmDetails_Activated(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Activated
		updateUI()
	End Sub
	
	Public Sub updateUI()
		Dim T As String
		lblInfo.Text = ""
		If modAudioGenie.Dateiformat > 0 Then
			T = T & "Datei: " & Datei & vbCrLf
			T = T & "Grösse: " & genie.AUDIOGetFileSize & " bytes" & vbCrLf
			T = T & "Format: " & AudioFormate(modAudioGenie.Dateiformat) & vbCrLf
			T = T & "Protected: " & genie.MPEGIsProtected & vbCrLf
			T = T & "Version: " & genie.AUDIOGetVersion & vbCrLf
			T = T & "Spielzeit: " & genie.AUDIOGetDuration & " Sekunden "
			T = T & " [" & VB6.Format(Int(genie.AUDIOGetDuration / 60), "00") & ":"
			T = T & VB6.Format(genie.AUDIOGetDuration Mod 60, "00") & "]" & vbCrLf
			T = T & "Samplerate: " & genie.AUDIOGetSampleRate & " Hz" & vbCrLf
			T = T & "Bitrate: " & genie.AUDIOGetBitrate & " kbps" & vbCrLf
			T = T & "Modus: " & genie.AUDIOGetChannelMode & vbCrLf
			T = T & "------------  Tag-Infos  ------------" & vbCrLf
			T = T & "ID3v1-Tag: " & IIf(genie.ID3V1Exists, "ja, Länge 128 bytes", "Nein") & vbCrLf
			T = T & "ID3v2-Tag: " & IIf(genie.ID3V2Exists, "ja, Länge " & genie.ID3V2GetSize & " bytes", "Nein") & vbCrLf
			T = T & "Ape-Tag: " & IIf(genie.APEExists, "ja, Länge " & genie.APEGetSize & " bytes", "Nein") & vbCrLf
			T = T & "Lyrics-Tag: " & IIf(genie.LYRICSExists, "ja, Länge " & genie.LYRICSGetSize & " bytes", "Nein") & vbCrLf
			T = T & "Titel: " & genie.AUDIOTitle & vbCrLf
			T = T & "Interpret: " & genie.AUDIOArtist & vbCrLf
			T = T & "Album: " & genie.AUDIOAlbum & vbCrLf
			T = T & "Genre: " & genie.AUDIOGenre & vbCrLf
			T = T & "Track: " & genie.AUDIOTrack & vbCrLf
			T = T & "Jahr: " & genie.AUDIOYear & vbCrLf
			T = T & "Kommentar: " & genie.AUDIOComment & vbCrLf
			T = T & "MD5: " & genie.GetMD5ValueFromFile(Datei) & vbCrLf
			T = T & "AudioMD5: " & genie.AUDIOGetMD5Value & vbCrLf
			lblInfo.Text = T
		End If
	End Sub
End Class