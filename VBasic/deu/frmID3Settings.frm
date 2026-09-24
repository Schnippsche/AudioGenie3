VERSION 5.00
Begin VB.Form frmID3Settings 
   BorderStyle     =   4  'Festes Werkzeugfenster
   Caption         =   "ID3v2 Einstellungen"
   ClientHeight    =   2730
   ClientLeft      =   7485
   ClientTop       =   2085
   ClientWidth     =   4455
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2730
   ScaleWidth      =   4455
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton cmdOkay 
      Caption         =   "Übernehmen"
      Default         =   -1  'True
      Height          =   465
      Left            =   2400
      TabIndex        =   11
      Top             =   2040
      Width           =   1355
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Abbrechen"
      Height          =   465
      Left            =   780
      TabIndex        =   10
      Top             =   2040
      Width           =   1355
   End
   Begin VB.Frame Frame2 
      Caption         =   "Text Encoding"
      Height          =   1695
      Left            =   2400
      TabIndex        =   5
      Top             =   120
      Width           =   1955
      Begin VB.OptionButton optionEncoding 
         Caption         =   "UTF-8"
         Height          =   315
         Index           =   3
         Left            =   180
         TabIndex        =   9
         Top             =   1200
         Width           =   1455
      End
      Begin VB.OptionButton optionEncoding 
         Caption         =   "UTF-16"
         Height          =   315
         Index           =   2
         Left            =   180
         TabIndex        =   8
         Top             =   900
         Width           =   1455
      End
      Begin VB.OptionButton optionEncoding 
         Caption         =   "UTF-16 BOM"
         Height          =   315
         Index           =   1
         Left            =   180
         TabIndex        =   7
         Top             =   600
         Width           =   1455
      End
      Begin VB.OptionButton optionEncoding 
         Caption         =   "ISO-8859-1"
         Height          =   315
         Index           =   0
         Left            =   180
         TabIndex        =   6
         Top             =   300
         Value           =   -1  'True
         Width           =   1455
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Speicherformat Frames"
      Height          =   1695
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   1995
      Begin VB.OptionButton optionFormat 
         Caption         =   "ID3v2.4"
         Height          =   375
         Index           =   3
         Left            =   240
         TabIndex        =   4
         Top             =   1200
         Width           =   1155
      End
      Begin VB.OptionButton optionFormat 
         Caption         =   "ID3v2.3"
         Height          =   375
         Index           =   2
         Left            =   240
         TabIndex        =   3
         Top             =   900
         Width           =   1155
      End
      Begin VB.OptionButton optionFormat 
         Caption         =   "ID3v2.2"
         Height          =   375
         Index           =   1
         Left            =   240
         TabIndex        =   2
         Top             =   600
         Width           =   1155
      End
      Begin VB.OptionButton optionFormat 
         Caption         =   "Originalformat"
         Height          =   375
         Index           =   0
         Left            =   240
         TabIndex        =   1
         Top             =   300
         Value           =   -1  'True
         Width           =   1335
      End
   End
End
Attribute VB_Name = "frmID3Settings"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdCancel_Click()
  Hide
End Sub

Private Sub cmdOkay_Click()
  Dim speicherFormat As Integer, textEncoding As Integer
  For w% = 0 To 3
     If optionFormat(w%).Value = True Then speicherFormat = w%
     If optionEncoding(w%).Value = True Then textEncoding = w%
  Next w%
  If genie.ID3V2SetFormatAndEncoding(speicherFormat, textEncoding) = FEHLERHAFT Then
    MsgBox "Kombination von Format und Encoding nicht erlaubt!"
    Exit Sub
  End If
  Hide
End Sub
