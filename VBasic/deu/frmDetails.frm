VERSION 5.00
Begin VB.Form frmDetails 
   BorderStyle     =   4  'Festes Werkzeugfenster
   Caption         =   "Properties"
   ClientHeight    =   4830
   ClientLeft      =   2355
   ClientTop       =   2265
   ClientWidth     =   3810
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4830
   ScaleWidth      =   3810
   ShowInTaskbar   =   0   'False
   WhatsThisButton =   -1  'True
   WhatsThisHelp   =   -1  'True
   Begin VB.TextBox lblInfo 
      BackColor       =   &H80000018&
      BeginProperty Font 
         Name            =   "Small Fonts"
         Size            =   6.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   4815
      Left            =   0
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertikal
      TabIndex        =   0
      Top             =   0
      Width           =   3795
   End
End
Attribute VB_Name = "frmDetails"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdOk_Click()
  Hide
End Sub

Private Sub Form_Activate()
  updateUI
End Sub

Public Sub updateUI()
  Dim T$
  lblInfo.text = ""
  If Dateiformat > 0 Then
    T$ = T$ & "Datei: " & Datei & vbCrLf
    T$ = T$ & "Grösse: " & genie.AUDIOGetFileSize & " bytes" & vbCrLf
    T$ = T$ & "Format: " & AudioFormate(Dateiformat) & vbCrLf
    T$ = T$ & "Protected: " & genie.MPEGIsProtected & vbCrLf
    T$ = T$ & "Version: " & genie.AUDIOGetVersion & vbCrLf
    T$ = T$ & "Spielzeit: " & genie.AUDIOGetDuration & " Sekunden "
    T$ = T$ & " [" & format$(Int(genie.AUDIOGetDuration / 60), "00") & ":"
    T$ = T$ & format$(genie.AUDIOGetDuration Mod 60, "00") & "]" & vbCrLf
    T$ = T$ & "Samplerate: " & genie.AUDIOGetSampleRate & " Hz" & vbCrLf
    T$ = T$ & "Bitrate: " & genie.AUDIOGetBitrate & " kbps" & vbCrLf
    T$ = T$ & "Modus: " & genie.AUDIOGetChannelMode & vbCrLf
    T$ = T$ & "------------  Tag-Infos  ------------" & vbCrLf
    T$ = T$ & "ID3v1-Tag: " & IIf(genie.ID3V1Exists, "ja, Länge 128 bytes", "Nein") & vbCrLf
    T$ = T$ & "ID3v2-Tag: " & IIf(genie.ID3V2Exists, "ja, Länge " & genie.ID3V2GetSize & " bytes", "Nein") & vbCrLf
    T$ = T$ & "Ape-Tag: " & IIf(genie.APEExists, "ja, Länge " & genie.APEGetSize & " bytes", "Nein") & vbCrLf
    T$ = T$ & "Lyrics-Tag: " & IIf(genie.LYRICSExists, "ja, Länge " & genie.LYRICSGetSize & " bytes", "Nein") & vbCrLf
    T$ = T$ & "Titel: " & genie.AUDIOTitle & vbCrLf
    T$ = T$ & "Interpret: " & genie.AUDIOArtist & vbCrLf
    T$ = T$ & "Album: " & genie.AUDIOAlbum & vbCrLf
    T$ = T$ & "Genre: " & genie.AUDIOGenre & vbCrLf
    T$ = T$ & "Track: " & genie.AUDIOTrack & vbCrLf
    T$ = T$ & "Jahr: " & genie.AUDIOYear & vbCrLf
    T$ = T$ & "Kommentar: " & genie.AUDIOComment & vbCrLf
    T$ = T$ & "MD5: " & genie.GetMD5ValueFromFile(Datei) & vbCrLf
    T$ = T$ & "AudioMD5: " & genie.AUDIOGetMD5Value & vbCrLf
    lblInfo.text = T$
  End If
End Sub

