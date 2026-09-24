VERSION 5.00
Begin VB.Form frmAPIC 
   BorderStyle     =   4  'Festes Werkzeugfenster
   Caption         =   "Picture Information"
   ClientHeight    =   3420
   ClientLeft      =   6555
   ClientTop       =   3195
   ClientWidth     =   6750
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3420
   ScaleWidth      =   6750
   ShowInTaskbar   =   0   'False
   Begin VB.TextBox picMime 
      BackColor       =   &H8000000F&
      Height          =   285
      Left            =   3360
      Locked          =   -1  'True
      TabIndex        =   10
      Top             =   1860
      Width           =   3135
   End
   Begin VB.CheckBox linkCheck 
      Caption         =   "Do not display picture when getting data"
      Height          =   255
      Left            =   3360
      TabIndex        =   6
      Top             =   2280
      Width           =   3135
   End
   Begin VB.ComboBox picTypeCombo 
      Height          =   315
      Left            =   3360
      Style           =   2  'Dropdown-Liste
      TabIndex        =   5
      Top             =   1140
      Width           =   3135
   End
   Begin VB.TextBox picDescription 
      Height          =   285
      Left            =   3360
      TabIndex        =   3
      Top             =   480
      Width           =   3135
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "&Cancel"
      Height          =   435
      Left            =   3480
      TabIndex        =   1
      Top             =   2760
      Width           =   1335
   End
   Begin VB.CommandButton cmdOk 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   435
      Left            =   5100
      TabIndex        =   0
      Top             =   2760
      Width           =   1335
   End
   Begin VB.Label Label3 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Picture MIME Type:"
      Height          =   195
      Left            =   3360
      TabIndex        =   9
      Top             =   1620
      Width           =   1410
   End
   Begin VB.Label dateiInfo 
      Height          =   195
      Left            =   5940
      TabIndex        =   8
      Top             =   1620
      Visible         =   0   'False
      Width           =   555
   End
   Begin VB.Label idInfo 
      Height          =   195
      Left            =   5820
      TabIndex        =   7
      Top             =   240
      Width           =   615
   End
   Begin VB.Image img 
      BorderStyle     =   1  'Fest Einfach
      Height          =   2955
      Left            =   120
      Stretch         =   -1  'True
      Top             =   240
      Width           =   2955
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Picture Type:"
      Height          =   195
      Left            =   3360
      TabIndex        =   4
      Top             =   900
      Width           =   945
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Description of the picture number:"
      Height          =   195
      Left            =   3360
      TabIndex        =   2
      Top             =   240
      Width           =   2385
   End
End
Attribute VB_Name = "frmAPIC"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdCancel_Click()
    Me.Hide
End Sub

Private Sub cmdOk_Click()
    Dim picType%
    picType% = picTypeCombo.ListIndex
    If picType% = -1 Then picType = 0
    genie.ID3V2SetPicture dateiInfo.Caption, picDescription.Text, idInfo.Caption, picType, linkCheck.Value
    frmID3V2.updatePictures
    Hide
End Sub

Private Sub Form_Load()
    picTypeCombo.AddItem "Other"
    picTypeCombo.AddItem "32*32 pixelsx file icon"
    picTypeCombo.AddItem "other file icon"
    picTypeCombo.AddItem "Front Cover"
    picTypeCombo.AddItem "Back Cover"
    picTypeCombo.AddItem "Leaflat page"
    picTypeCombo.AddItem "Media CD Label"
    picTypeCombo.AddItem "Lead artist/soloist"
    picTypeCombo.AddItem "Artist/performer"
    picTypeCombo.AddItem "Conductor"
    picTypeCombo.AddItem "Band/Orchestra"
    picTypeCombo.AddItem "Composer"
    picTypeCombo.AddItem "Lyricist/text writer"
    picTypeCombo.AddItem "Recording Location"
    picTypeCombo.AddItem "During recording"
    picTypeCombo.AddItem "During performance"
    picTypeCombo.AddItem "Movie/video screen"
    picTypeCombo.AddItem "a bright coloured fish"
    picTypeCombo.AddItem "Illustration"
    picTypeCombo.AddItem "Band/artist logotype"
    picTypeCombo.AddItem "Publisher/Studio Logo"
    picTypeCombo.ListIndex = 0
End Sub
