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
Option Explicit

Public Sub updateUI()
    Dim strText As String
    lblInfo.Text = ""
    If AudioFormat > 0 Then
        strText = strText & "File: " & Datei & vbCrLf
        strText = strText & "Size: " & genie.AUDIOGetFileSize & " bytes" & vbCrLf
        strText = strText & "Format: " & Dateiformat(AudioFormat) & vbCrLf
        strText = strText & "Version: " & genie.AUDIOGetVersion & vbCrLf
        strText = strText & "Duration: " & genie.AUDIOGetDuration & " Seconds "
        strText = strText & " [" & Format$(Int(genie.AUDIOGetDuration / 60), "00") & ":"
        strText = strText & Format$(genie.AUDIOGetDuration Mod 60, "00") & "]" & vbCrLf
        strText = strText & "Sample Rate: " & genie.AUDIOGetSampleRate & " Hz" & vbCrLf
        strText = strText & "Bit Rate: " & genie.AUDIOGetBitrate & " kbps" & vbCrLf
        strText = strText & "Mode: " & genie.AUDIOGetChannelMode & vbCrLf
        strText = strText & "--------  Tag Infomation  ---------" & vbCrLf
        strText = strText & "ID3v1 Tag: " & IIf(genie.ID3V1Exists, "Yes, Length 128 bytes", "No") & vbCrLf
        strText = strText & "ID3v2 Tag: " & IIf(genie.ID3V2Exists, "Yes, Length " & genie.ID3V2GetSize & " bytes", "No") & vbCrLf
        strText = strText & "Ape Tag: " & IIf(genie.APEExists, "Yes, Length " & genie.APEGetSize & " bytes", "No") & vbCrLf
        strText = strText & "Lyrics Tag: " & IIf(genie.LYRICSExists, "Yes, Length " & genie.LYRICSGetSize & " bytes", "No") & vbCrLf
        strText = strText & "Title: " & genie.AUDIOTitle & vbCrLf
        strText = strText & "Artist: " & genie.AUDIOArtist & vbCrLf
        strText = strText & "Album: " & genie.AUDIOAlbum & vbCrLf
        strText = strText & "Genre: " & genie.AUDIOGenre & vbCrLf
        strText = strText & "Track: " & genie.AUDIOTrack & vbCrLf
        strText = strText & "Year: " & genie.AUDIOYear & vbCrLf
        strText = strText & "Comment: " & genie.AUDIOComment & vbCrLf
        lblInfo.Text = strText
    End If
End Sub

Private Sub cmdOk_Click()
    Me.Hide
End Sub

Private Sub Form_Activate()
    updateUI
End Sub

