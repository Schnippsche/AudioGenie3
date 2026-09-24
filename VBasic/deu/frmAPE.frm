VERSION 5.00
Begin VB.Form frmAPE 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "APE-Tag ändern"
   ClientHeight    =   4245
   ClientLeft      =   1080
   ClientTop       =   4890
   ClientWidth     =   4920
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4245
   ScaleWidth      =   4920
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton cmdDelete 
      Caption         =   "Tag löschen"
      Height          =   435
      Left            =   180
      TabIndex        =   16
      Top             =   3600
      Width           =   1400
   End
   Begin VB.CommandButton cmdUpdate 
      Caption         =   "Tag speichern"
      Height          =   435
      Left            =   1760
      TabIndex        =   17
      Top             =   3600
      Width           =   1400
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Fertig"
      Height          =   435
      Left            =   3360
      TabIndex        =   18
      Top             =   3600
      Width           =   1400
   End
   Begin VB.TextBox txtCopyright 
      Height          =   345
      Left            =   1200
      TabIndex        =   5
      Top             =   2100
      Width           =   3315
   End
   Begin VB.TextBox txtTrack 
      Height          =   345
      Left            =   3360
      TabIndex        =   7
      Top             =   2580
      Width           =   1155
   End
   Begin VB.TextBox txtGenre 
      Height          =   345
      Left            =   1200
      TabIndex        =   8
      Top             =   3060
      Width           =   2295
   End
   Begin VB.TextBox txtYear 
      Height          =   345
      Left            =   1200
      TabIndex        =   6
      Top             =   2580
      Width           =   1095
   End
   Begin VB.TextBox txtComment 
      Height          =   345
      Left            =   1200
      TabIndex        =   4
      Top             =   1620
      Width           =   3315
   End
   Begin VB.TextBox txtAlbum 
      Height          =   345
      Left            =   1200
      TabIndex        =   3
      Top             =   1140
      Width           =   3315
   End
   Begin VB.TextBox txtArtist 
      Height          =   345
      Left            =   1200
      TabIndex        =   2
      Top             =   660
      Width           =   3315
   End
   Begin VB.TextBox txtTitle 
      Height          =   345
      Left            =   1200
      TabIndex        =   1
      Top             =   180
      Width           =   3315
   End
   Begin VB.Label Label8 
      Alignment       =   1  'Rechts
      Caption         =   "Jahr"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   180
      TabIndex        =   15
      Top             =   2640
      Width           =   915
   End
   Begin VB.Label Label7 
      Alignment       =   1  'Rechts
      Caption         =   "Track #"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   2580
      TabIndex        =   14
      Top             =   2640
      Width           =   615
   End
   Begin VB.Label Label6 
      Alignment       =   1  'Rechts
      Caption         =   "Genre"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   420
      TabIndex        =   13
      Top             =   3120
      Width           =   675
   End
   Begin VB.Label Label5 
      Alignment       =   1  'Rechts
      Caption         =   "Copyright"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   180
      TabIndex        =   12
      Top             =   2160
      Width           =   915
   End
   Begin VB.Label Label4 
      Alignment       =   1  'Rechts
      Caption         =   "Kommentar"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   180
      TabIndex        =   11
      Top             =   1680
      Width           =   915
   End
   Begin VB.Label Label3 
      Alignment       =   1  'Rechts
      Caption         =   "Album"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   180
      TabIndex        =   10
      Top             =   1200
      Width           =   915
   End
   Begin VB.Label Label2 
      Alignment       =   1  'Rechts
      Caption         =   "Interpret"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   180
      TabIndex        =   9
      Top             =   720
      Width           =   915
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Rechts
      Caption         =   "Titel"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   180
      TabIndex        =   0
      Top             =   240
      Width           =   915
   End
End
Attribute VB_Name = "frmAPE"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdCancel_Click()
  Hide
End Sub

Private Sub cmdUpdate_Click()
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
    MsgBox "Beim Speichern ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Speichern des APE-Tag"
  End If
End Sub

Private Sub cmdDelete_Click()
  DeleteAPETag
  updateUI
End Sub

Private Sub Form_Activate()
  updateUI
End Sub

Private Sub updateUI()
  frmAPE.txtTitle = genie.APETitle
  frmAPE.txtArtist = genie.APEArtist
  frmAPE.txtAlbum = genie.APEAlbum
  frmAPE.txtGenre = genie.APEGenre
  frmAPE.txtYear = genie.APEYear
  frmAPE.txtTrack = genie.APETrack
  frmAPE.txtComment = genie.APEComment
  frmAPE.txtCopyright = genie.APECopyright
End Sub
