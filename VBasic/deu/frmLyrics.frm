VERSION 5.00
Begin VB.Form frmLyrics 
   Caption         =   "Lyrics-Tag ändern"
   ClientHeight    =   6390
   ClientLeft      =   4740
   ClientTop       =   5685
   ClientWidth     =   6225
   LinkTopic       =   "Form1"
   ScaleHeight     =   6390
   ScaleWidth      =   6225
   Begin VB.CommandButton cmdUpdate 
      Caption         =   "Tag speichern"
      Height          =   435
      Left            =   2412
      TabIndex        =   17
      Top             =   5760
      Width           =   1400
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Fertig"
      Height          =   435
      Left            =   3972
      TabIndex        =   18
      Top             =   5760
      Width           =   1400
   End
   Begin VB.CommandButton cmdDelete 
      Caption         =   "Tag löschen"
      Height          =   435
      Left            =   852
      TabIndex        =   16
      Top             =   5760
      Width           =   1400
   End
   Begin VB.TextBox txtLyrics 
      Height          =   1485
      Left            =   1020
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Beides
      TabIndex        =   14
      Top             =   4020
      Width           =   4800
   End
   Begin VB.TextBox txtGenre 
      Height          =   345
      Left            =   1020
      TabIndex        =   12
      Top             =   3540
      Width           =   4800
   End
   Begin VB.TextBox txtImageLink 
      Height          =   345
      Left            =   1020
      MultiLine       =   -1  'True
      TabIndex        =   10
      Top             =   3060
      Width           =   4800
   End
   Begin VB.TextBox txtInformation 
      Height          =   825
      Left            =   1020
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Beides
      TabIndex        =   8
      Top             =   2100
      Width           =   4800
   End
   Begin VB.TextBox txtAuthor 
      Height          =   345
      Left            =   1020
      TabIndex        =   6
      Top             =   1620
      Width           =   4800
   End
   Begin VB.TextBox txtAlbum 
      Height          =   345
      Left            =   1020
      TabIndex        =   4
      Top             =   1140
      Width           =   4800
   End
   Begin VB.TextBox txtArtist 
      Height          =   345
      Left            =   1020
      TabIndex        =   2
      Top             =   660
      Width           =   4800
   End
   Begin VB.TextBox txtTitle 
      Height          =   345
      Left            =   1020
      TabIndex        =   0
      Top             =   180
      Width           =   4800
   End
   Begin VB.Label Label8 
      Alignment       =   1  'Rechts
      Caption         =   "Lyrics"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   0
      TabIndex        =   15
      Top             =   4080
      Width           =   915
   End
   Begin VB.Label Label7 
      Alignment       =   1  'Rechts
      Caption         =   "Genre"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   0
      TabIndex        =   13
      Top             =   3600
      Width           =   915
   End
   Begin VB.Label Label6 
      Alignment       =   1  'Rechts
      Caption         =   "Bilderlink"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   0
      TabIndex        =   11
      Top             =   3120
      Width           =   915
   End
   Begin VB.Label Label5 
      Alignment       =   1  'Rechts
      Caption         =   "Information"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   0
      TabIndex        =   9
      Top             =   2160
      Width           =   915
   End
   Begin VB.Label Label4 
      Alignment       =   1  'Rechts
      Caption         =   "Autor"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   0
      TabIndex        =   7
      Top             =   1680
      Width           =   915
   End
   Begin VB.Label Label3 
      Alignment       =   1  'Rechts
      Caption         =   "Album"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   0
      TabIndex        =   5
      Top             =   1200
      Width           =   915
   End
   Begin VB.Label Label2 
      Alignment       =   1  'Rechts
      Caption         =   "Artist"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   0
      TabIndex        =   3
      Top             =   720
      Width           =   915
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Rechts
      Caption         =   "Titel"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   0
      TabIndex        =   1
      Top             =   240
      Width           =   915
   End
End
Attribute VB_Name = "frmLyrics"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdCancel_Click()
  Hide
End Sub

Private Sub cmdDelete_Click()
  DeleteLyricsTag
  updateUI
End Sub

Private Sub cmdUpdate_Click()
  genie.LYRICSArtist = txtArtist.Text
  genie.LYRICSAlbum = txtAlbum.Text
  genie.LYRICSTitle = txtTitle.Text
  genie.LYRICSLyrics = txtLyrics.Text
  genie.LYRICSInformation = txtInformation.Text
  genie.LYRICSGenre = txtGenre.Text
  genie.LYRICSImageLink = txtImageLink.Text
  genie.LYRICSAuthor = txtAuthor.Text
  If genie.LYRICSSaveChangesToFile(Datei) = False Then
    MsgBox "Beim Speichern ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Speichern des Lyrics-Tag"
  End If
End Sub

Private Sub Form_Activate()
  updateUI
End Sub

Private Sub updateUI()
  frmLyrics.txtArtist.Text = genie.LYRICSArtist
  frmLyrics.txtAlbum.Text = genie.LYRICSAlbum
  frmLyrics.txtTitle.Text = genie.LYRICSTitle
  frmLyrics.txtLyrics.Text = genie.LYRICSLyrics
  frmLyrics.txtInformation.Text = genie.LYRICSInformation
  frmLyrics.txtGenre.Text = genie.LYRICSGenre
  frmLyrics.txtImageLink.Text = genie.LYRICSImageLink
  frmLyrics.txtAuthor.Text = genie.LYRICSAuthor
End Sub
