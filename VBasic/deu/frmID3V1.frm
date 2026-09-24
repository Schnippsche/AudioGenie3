VERSION 5.00
Begin VB.Form frmID3v1 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "ID3V1-Tag ändern"
   ClientHeight    =   3885
   ClientLeft      =   795
   ClientTop       =   900
   ClientWidth     =   5085
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3885
   ScaleWidth      =   5085
   ShowInTaskbar   =   0   'False
   Begin VB.ComboBox genreCombo 
      Height          =   315
      Left            =   1200
      Sorted          =   -1  'True
      Style           =   2  'Dropdown-Liste
      TabIndex        =   12
      Top             =   2580
      Width           =   3315
   End
   Begin VB.TextBox txtTrack 
      Height          =   345
      Left            =   3540
      MaxLength       =   3
      TabIndex        =   11
      Top             =   2100
      Width           =   975
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Fertig"
      Height          =   435
      Left            =   3402
      TabIndex        =   15
      Top             =   3180
      Width           =   1400
   End
   Begin VB.CommandButton cmdUpdate 
      Caption         =   "Tag speichern"
      Height          =   435
      Left            =   1842
      TabIndex        =   14
      Top             =   3180
      Width           =   1400
   End
   Begin VB.CommandButton cmdDelete 
      Caption         =   "Tag löschen"
      Height          =   435
      Left            =   282
      TabIndex        =   13
      Top             =   3180
      Width           =   1400
   End
   Begin VB.TextBox txtYear 
      Height          =   345
      Left            =   1200
      MaxLength       =   4
      TabIndex        =   9
      Top             =   2100
      Width           =   975
   End
   Begin VB.TextBox txtComment 
      Height          =   345
      Left            =   1200
      MaxLength       =   30
      TabIndex        =   7
      Top             =   1620
      Width           =   3315
   End
   Begin VB.TextBox txtAlbum 
      Height          =   345
      Left            =   1200
      MaxLength       =   30
      TabIndex        =   5
      Top             =   1140
      Width           =   3315
   End
   Begin VB.TextBox txtArtist 
      Height          =   345
      Left            =   1200
      MaxLength       =   30
      TabIndex        =   3
      Top             =   660
      Width           =   3315
   End
   Begin VB.TextBox txtTitle 
      Height          =   345
      Left            =   1200
      MaxLength       =   30
      TabIndex        =   1
      Top             =   180
      Width           =   3315
   End
   Begin VB.Label Label7 
      Alignment       =   1  'Rechts
      Caption         =   "Track #"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   2820
      TabIndex        =   16
      Top             =   2160
      Width           =   615
   End
   Begin VB.Label Genre 
      Alignment       =   1  'Rechts
      Caption         =   "Genre"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   420
      TabIndex        =   10
      Top             =   2640
      Width           =   675
   End
   Begin VB.Label Label5 
      Alignment       =   1  'Rechts
      Caption         =   "Jahr"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   180
      TabIndex        =   8
      Top             =   2160
      Width           =   915
   End
   Begin VB.Label Label4 
      Alignment       =   1  'Rechts
      Caption         =   "Kommentar"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   180
      TabIndex        =   6
      Top             =   1680
      Width           =   915
   End
   Begin VB.Label Label3 
      Alignment       =   1  'Rechts
      Caption         =   "Album"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   180
      TabIndex        =   4
      Top             =   1200
      Width           =   915
   End
   Begin VB.Label Label2 
      Alignment       =   1  'Rechts
      Caption         =   "Interpret"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   180
      TabIndex        =   2
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
Attribute VB_Name = "frmID3v1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdCancel_Click()
  Hide
End Sub

Private Sub cmdUpdate_Click()
  ' Daten aus den Eingabefeldern in die Struktur übertragen
  genie.ID3V1Artist = txtArtist.Text
  genie.ID3V1Album = txtAlbum.Text
  genie.ID3V1Title = txtTitle.Text
  genie.ID3V1Year = txtYear.Text
  genie.ID3V1Comment = txtComment.Text
  genie.ID3V1Track = txtTrack.Text
  If genreCombo.ListIndex >= 0 Then
    genie.ID3V1GenreID = genreCombo.ItemData(genreCombo.ListIndex)
  End If
  If genie.ID3V1SaveChangesToFile(Datei) = False Then
    MsgBox "Beim Speichern ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Speichern des ID3v1-Tag"
  End If
End Sub

Private Sub cmdDelete_Click()
  DeleteID3v1Tag
  updateUI
End Sub

Private Sub Form_Activate()
  updateUI
End Sub

Private Sub Form_Load()
  ' Einmalig die Genre Liste füllen
  Dim i%
  genreCombo.Clear
  genreCombo.AddItem "(kein Genre gewählt)"
  genreCombo.ItemData(genreCombo.NewIndex) = 255
  For i = 0 To genie.ID3V1GetGenres - 1
    genreCombo.AddItem genie.ID3V1GetGenreItem(i)
    genreCombo.ItemData(genreCombo.NewIndex) = i
  Next i
End Sub

Private Sub updateUI()
  Dim i%, g%
  txtArtist.Text = genie.ID3V1Artist
  txtAlbum.Text = genie.ID3V1Album
  txtTitle.Text = genie.ID3V1Title
  txtYear.Text = genie.ID3V1Year
  txtComment.Text = genie.ID3V1Comment
  txtTrack.Text = genie.ID3V1Track
  g% = genie.ID3V1GenreID
  For i = 0 To genreCombo.ListCount - 1
    If genreCombo.ItemData(i) = g Then genreCombo.ListIndex = i
  Next i
End Sub

