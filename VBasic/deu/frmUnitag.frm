VERSION 5.00
Begin VB.Form frmUnitag 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "Tagging"
   ClientHeight    =   3960
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   5205
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3960
   ScaleWidth      =   5205
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows-Standard
   Begin VB.TextBox txtGenre 
      Height          =   345
      Left            =   1260
      TabIndex        =   6
      Top             =   2640
      Width           =   3315
   End
   Begin VB.TextBox txtTitle 
      Height          =   345
      Left            =   1260
      TabIndex        =   0
      Top             =   240
      Width           =   3315
   End
   Begin VB.TextBox txtArtist 
      Height          =   345
      Left            =   1260
      TabIndex        =   1
      Top             =   720
      Width           =   3315
   End
   Begin VB.TextBox txtAlbum 
      Height          =   345
      Left            =   1260
      TabIndex        =   2
      Top             =   1200
      Width           =   3315
   End
   Begin VB.TextBox txtComment 
      Height          =   345
      Left            =   1260
      TabIndex        =   3
      Top             =   1680
      Width           =   3315
   End
   Begin VB.TextBox txtYear 
      Height          =   345
      Left            =   1260
      TabIndex        =   4
      Top             =   2160
      Width           =   975
   End
   Begin VB.CommandButton cmdUpdate 
      Caption         =   "Speichern"
      Height          =   435
      Left            =   993
      TabIndex        =   7
      Top             =   3270
      Width           =   1400
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Fertig"
      Height          =   435
      Left            =   2811
      TabIndex        =   8
      Top             =   3270
      Width           =   1400
   End
   Begin VB.TextBox txtTrack 
      Height          =   345
      Left            =   3600
      TabIndex        =   5
      Top             =   2160
      Width           =   975
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Rechts
      Caption         =   "Titel"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   240
      TabIndex        =   15
      Top             =   300
      Width           =   915
   End
   Begin VB.Label Label2 
      Alignment       =   1  'Rechts
      Caption         =   "Interpret"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   240
      TabIndex        =   14
      Top             =   780
      Width           =   915
   End
   Begin VB.Label Label3 
      Alignment       =   1  'Rechts
      Caption         =   "Album"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   240
      TabIndex        =   13
      Top             =   1260
      Width           =   915
   End
   Begin VB.Label Label4 
      Alignment       =   1  'Rechts
      Caption         =   "Kommentar"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   240
      TabIndex        =   12
      Top             =   1740
      Width           =   915
   End
   Begin VB.Label Label5 
      Alignment       =   1  'Rechts
      Caption         =   "Jahr"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   240
      TabIndex        =   11
      Top             =   2220
      Width           =   915
   End
   Begin VB.Label Genre 
      Alignment       =   1  'Rechts
      Caption         =   "Genre"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   480
      TabIndex        =   10
      Top             =   2700
      Width           =   675
   End
   Begin VB.Label Label7 
      Alignment       =   1  'Rechts
      Caption         =   "Track #"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   2880
      TabIndex        =   9
      Top             =   2220
      Width           =   615
   End
End
Attribute VB_Name = "frmUnitag"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdCancel_Click()
  Hide
End Sub

Private Sub cmdUpdate_Click()
  genie.AUDIOArtist = txtArtist.Text
  genie.AUDIOAlbum = txtAlbum.Text
  genie.AUDIOTitle = txtTitle.Text
  genie.AUDIOYear = txtYear.Text
  genie.AUDIOComment = txtComment.Text
  genie.AUDIOTrack = txtTrack.Text
  genie.AUDIOGenre = txtGenre.Text
  If genie.AUDIOSaveChangesToFile(Datei) = False Then
    MsgBox "Beim Speichern ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Speichern der Änderungen"
  End If
End Sub

Private Sub Form_Activate()
  txtArtist.Text = genie.AUDIOArtist
  txtAlbum.Text = genie.AUDIOAlbum
  txtTitle.Text = genie.AUDIOTitle
  txtYear.Text = genie.AUDIOYear
  txtComment.Text = genie.AUDIOComment
  txtTrack.Text = genie.AUDIOTrack
  txtGenre.Text = genie.AUDIOGenre
  If genie.WMAGetPictures > 0 Then
    Debug.Print genie.WMAGetPictureDescription(1)
    Debug.Print genie.WMAGetPictureMime(1)
  End If
  
End Sub

