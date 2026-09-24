VERSION 5.00
Begin VB.Form frmSYLT 
   BorderStyle     =   4  'Festes Werkzeugfenster
   Caption         =   "Eintrag bearbeiten"
   ClientHeight    =   1785
   ClientLeft      =   45
   ClientTop       =   315
   ClientWidth     =   4095
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1785
   ScaleWidth      =   4095
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'Fenstermitte
   Begin VB.CheckBox chk 
      Caption         =   "Check1"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   1260
      Visible         =   0   'False
      Width           =   195
   End
   Begin VB.CommandButton cmdOk 
      Caption         =   "Übernehmen"
      Default         =   -1  'True
      Height          =   435
      Left            =   2160
      TabIndex        =   5
      Top             =   1200
      Width           =   1335
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Abbrechen"
      Height          =   435
      Left            =   600
      TabIndex        =   4
      Top             =   1200
      Width           =   1335
   End
   Begin VB.TextBox txtEntry 
      Height          =   285
      Left            =   1860
      TabIndex        =   3
      Top             =   660
      Width           =   1935
   End
   Begin VB.TextBox txtTime 
      Height          =   315
      Left            =   1860
      MaxLength       =   8
      TabIndex        =   1
      Top             =   180
      Width           =   1935
   End
   Begin VB.Label Label2 
      Alignment       =   1  'Rechts
      Caption         =   "Texteintrag:"
      Height          =   255
      Left            =   240
      TabIndex        =   2
      Top             =   720
      Width           =   1515
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Rechts
      Caption         =   "absolute Zeitangabe:"
      Height          =   255
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1515
   End
End
Attribute VB_Name = "frmSYLT"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdCancel_Click()
  chk.Value = 0
  Hide
End Sub

Private Sub cmdOk_Click()
  chk.Value = 1
  Hide
End Sub


