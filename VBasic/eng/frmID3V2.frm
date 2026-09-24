VERSION 5.00
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "comctl32.ocx"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmID3V2 
   Caption         =   "Change ID3V2 Tag"
   ClientHeight    =   6735
   ClientLeft      =   6315
   ClientTop       =   1500
   ClientWidth     =   7545
   Icon            =   "frmID3V2.frx":0000
   LinkTopic       =   "Form2"
   ScaleHeight     =   6735
   ScaleWidth      =   7545
   Begin VB.CheckBox checkUnicode 
      Caption         =   "Save as Unicode"
      Height          =   495
      Left            =   360
      TabIndex        =   119
      Top             =   6060
      Value           =   1  'Aktiviert
      Width           =   1095
   End
   Begin MSComDlg.CommonDialog StandardDlg 
      Left            =   7080
      Top             =   6060
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      CancelError     =   -1  'True
      DialogTitle     =   "Bild zuweisen"
      Filter          =   "Bilder ( bmp, jpg, gif, ico )|*.bmp;*.jpg;*.gif;*.ico"
   End
   Begin VB.CommandButton cmdCancel 
      Caption         =   "&Close"
      Height          =   435
      Left            =   5265
      TabIndex        =   2
      Top             =   6120
      Width           =   1400
   End
   Begin VB.CommandButton cmdUpdate 
      Caption         =   "&Save Changes"
      Default         =   -1  'True
      Height          =   435
      Left            =   3285
      TabIndex        =   1
      Top             =   6120
      Width           =   1815
   End
   Begin VB.CommandButton cmdDelete 
      Caption         =   "&Delete Tag"
      Height          =   435
      Left            =   1725
      TabIndex        =   0
      Top             =   6120
      Width           =   1400
   End
   Begin TabDlg.SSTab register 
      Height          =   5775
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   7275
      _ExtentX        =   12832
      _ExtentY        =   10186
      _Version        =   393216
      Style           =   1
      Tabs            =   7
      TabsPerRow      =   7
      TabHeight       =   520
      WordWrap        =   0   'False
      TabCaption(0)   =   "General"
      TabPicture(0)   =   "frmID3V2.frx":058A
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "lbl(33)"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "lbl(32)"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "lbl(31)"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "lbl(30)"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "lbl(0)"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "lbl(1)"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "lbl(2)"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "lbl(3)"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).Control(8)=   "lbl(5)"
      Tab(0).Control(8).Enabled=   0   'False
      Tab(0).Control(9)=   "lbl(7)"
      Tab(0).Control(9).Enabled=   0   'False
      Tab(0).Control(10)=   "lbl(9)"
      Tab(0).Control(10).Enabled=   0   'False
      Tab(0).Control(11)=   "lbl(10)"
      Tab(0).Control(11).Enabled=   0   'False
      Tab(0).Control(12)=   "lbl(11)"
      Tab(0).Control(12).Enabled=   0   'False
      Tab(0).Control(13)=   "lbl(12)"
      Tab(0).Control(13).Enabled=   0   'False
      Tab(0).Control(14)=   "lbl(13)"
      Tab(0).Control(14).Enabled=   0   'False
      Tab(0).Control(15)=   "lbl(15)"
      Tab(0).Control(15).Enabled=   0   'False
      Tab(0).Control(16)=   "lbl(35)"
      Tab(0).Control(16).Enabled=   0   'False
      Tab(0).Control(17)=   "txtTOWN"
      Tab(0).Control(17).Enabled=   0   'False
      Tab(0).Control(18)=   "txtTBPM"
      Tab(0).Control(18).Enabled=   0   'False
      Tab(0).Control(19)=   "txtTSSE"
      Tab(0).Control(19).Enabled=   0   'False
      Tab(0).Control(20)=   "txtTPE2"
      Tab(0).Control(20).Enabled=   0   'False
      Tab(0).Control(21)=   "txtTRCK"
      Tab(0).Control(21).Enabled=   0   'False
      Tab(0).Control(22)=   "txtTIT2"
      Tab(0).Control(22).Enabled=   0   'False
      Tab(0).Control(23)=   "txtTPE1"
      Tab(0).Control(23).Enabled=   0   'False
      Tab(0).Control(24)=   "txtTALB"
      Tab(0).Control(24).Enabled=   0   'False
      Tab(0).Control(25)=   "txtTYER"
      Tab(0).Control(25).Enabled=   0   'False
      Tab(0).Control(26)=   "txtTCON"
      Tab(0).Control(26).Enabled=   0   'False
      Tab(0).Control(27)=   "txtTCOM"
      Tab(0).Control(27).Enabled=   0   'False
      Tab(0).Control(28)=   "txtTCOP"
      Tab(0).Control(28).Enabled=   0   'False
      Tab(0).Control(29)=   "txtTENC"
      Tab(0).Control(29).Enabled=   0   'False
      Tab(0).Control(30)=   "txtTLAN"
      Tab(0).Control(30).Enabled=   0   'False
      Tab(0).Control(31)=   "txtTIT1"
      Tab(0).Control(31).Enabled=   0   'False
      Tab(0).Control(32)=   "txtTDRC"
      Tab(0).Control(32).Enabled=   0   'False
      Tab(0).Control(33)=   "txtPCNT"
      Tab(0).Control(33).Enabled=   0   'False
      Tab(0).ControlCount=   34
      TabCaption(1)   =   "Comments"
      TabPicture(1)   =   "frmID3V2.frx":05A6
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "lblItem(25)"
      Tab(1).Control(1)=   "lbl(24)"
      Tab(1).Control(2)=   "lbl(23)"
      Tab(1).Control(3)=   "lbl(4)"
      Tab(1).Control(4)=   "cmdOKComm"
      Tab(1).Control(5)=   "lfdNr1"
      Tab(1).Control(6)=   "cmdDelAllComments"
      Tab(1).Control(7)=   "cmdDelComment"
      Tab(1).Control(8)=   "txtComment1"
      Tab(1).Control(9)=   "txtLanguage1"
      Tab(1).Control(10)=   "txtCOMM"
      Tab(1).ControlCount=   11
      TabCaption(2)   =   "URL's"
      TabPicture(2)   =   "frmID3V2.frx":05C2
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "lbl(21)"
      Tab(2).Control(1)=   "lbl(20)"
      Tab(2).Control(2)=   "lbl(19)"
      Tab(2).Control(3)=   "lbl(18)"
      Tab(2).Control(4)=   "lbl(17)"
      Tab(2).Control(5)=   "lbl(16)"
      Tab(2).Control(6)=   "lbl(14)"
      Tab(2).Control(7)=   "lbl(6)"
      Tab(2).Control(8)=   "Frame1"
      Tab(2).Control(9)=   "txtWPUB"
      Tab(2).Control(10)=   "txtWPAY"
      Tab(2).Control(11)=   "txtWORS"
      Tab(2).Control(12)=   "txtWOAS"
      Tab(2).Control(13)=   "txtWOAR"
      Tab(2).Control(14)=   "txtWOAF"
      Tab(2).Control(15)=   "txtWCOP"
      Tab(2).Control(16)=   "txtWCOM"
      Tab(2).ControlCount=   17
      TabCaption(3)   =   "Pictures"
      TabPicture(3)   =   "frmID3V2.frx":05DE
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "noPicture"
      Tab(3).Control(1)=   "cmdDelPic"
      Tab(3).Control(2)=   "cmdNewPic"
      Tab(3).Control(3)=   "cmdDelAllPics"
      Tab(3).Control(4)=   "picimgview"
      Tab(3).Control(5)=   "cmdShowPic"
      Tab(3).ControlCount=   6
      TabCaption(4)   =   "Song Lyrics"
      TabPicture(4)   =   "frmID3V2.frx":05FA
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "lbl(26)"
      Tab(4).Control(1)=   "lbl(25)"
      Tab(4).Control(2)=   "lblItem(0)"
      Tab(4).Control(3)=   "lbl(8)"
      Tab(4).Control(4)=   "cmdOKLyric"
      Tab(4).Control(5)=   "cmdDelLyric"
      Tab(4).Control(6)=   "cmdDelAllLyrics"
      Tab(4).Control(7)=   "txtLanguage2"
      Tab(4).Control(8)=   "txtComment2"
      Tab(4).Control(9)=   "lfdNr2"
      Tab(4).Control(10)=   "txtUSLT"
      Tab(4).ControlCount=   11
      TabCaption(5)   =   "User Frames"
      TabPicture(5)   =   "frmID3V2.frx":0616
      Tab(5).ControlEnabled=   0   'False
      Tab(5).Control(0)=   "lbl(28)"
      Tab(5).Control(1)=   "lblItem(2)"
      Tab(5).Control(2)=   "lbl(29)"
      Tab(5).Control(3)=   "cmdDelFrame"
      Tab(5).Control(4)=   "cmdDelAllFrames"
      Tab(5).Control(5)=   "cmdOKFrame"
      Tab(5).Control(6)=   "txtLanguage4"
      Tab(5).Control(7)=   "lfdNr4"
      Tab(5).Control(8)=   "txtUSER"
      Tab(5).ControlCount=   9
      TabCaption(6)   =   "Sync Lyrics"
      TabPicture(6)   =   "frmID3V2.frx":0632
      Tab(6).ControlEnabled=   0   'False
      Tab(6).Control(0)=   "cmdDelAllSyncLyric"
      Tab(6).Control(1)=   "cmdOkSyncLyric"
      Tab(6).Control(2)=   "Frame2"
      Tab(6).Control(3)=   "cmdAddSyncLyric"
      Tab(6).Control(4)=   "cmdDelSyncLyric"
      Tab(6).Control(5)=   "cmdEditSyncLyric"
      Tab(6).Control(6)=   "viewSYLTDescr"
      Tab(6).ControlCount=   7
      Begin VB.TextBox txtPCNT 
         Height          =   315
         Left            =   5760
         TabIndex        =   53
         Top             =   600
         Width           =   1275
      End
      Begin VB.CommandButton cmdShowPic 
         Caption         =   "Show Picture"
         Height          =   375
         Left            =   -73260
         TabIndex        =   117
         Top             =   5160
         Width           =   1575
      End
      Begin ComctlLib.ListView picimgview 
         Height          =   4455
         Left            =   -74820
         TabIndex        =   116
         Top             =   540
         Width           =   6855
         _ExtentX        =   12091
         _ExtentY        =   7858
         Arrange         =   2
         LabelEdit       =   1
         LabelWrap       =   -1  'True
         HideSelection   =   -1  'True
         _Version        =   327682
         ForeColor       =   -2147483640
         BackColor       =   -2147483643
         BorderStyle     =   1
         Appearance      =   1
         NumItems        =   5
         BeginProperty ColumnHeader(1) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
            Key             =   ""
            Object.Tag             =   ""
            Text            =   "Beschreibung"
            Object.Width           =   2540
         EndProperty
         BeginProperty ColumnHeader(2) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
            SubItemIndex    =   1
            Key             =   ""
            Object.Tag             =   ""
            Text            =   "Bildtyp"
            Object.Width           =   2540
         EndProperty
         BeginProperty ColumnHeader(3) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
            SubItemIndex    =   2
            Key             =   ""
            Object.Tag             =   ""
            Text            =   "Mime"
            Object.Width           =   2540
         EndProperty
         BeginProperty ColumnHeader(4) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
            SubItemIndex    =   3
            Key             =   ""
            Object.Tag             =   ""
            Text            =   "ID"
            Object.Width           =   2540
         EndProperty
         BeginProperty ColumnHeader(5) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
            SubItemIndex    =   4
            Key             =   ""
            Object.Tag             =   ""
            Text            =   "Dateiname"
            Object.Width           =   2540
         EndProperty
      End
      Begin ComctlLib.ListView viewSYLTDescr 
         Height          =   1635
         Left            =   -74700
         TabIndex        =   105
         Top             =   540
         Width           =   5175
         _ExtentX        =   9128
         _ExtentY        =   2884
         View            =   3
         LabelWrap       =   -1  'True
         HideSelection   =   0   'False
         _Version        =   327682
         ForeColor       =   -2147483640
         BackColor       =   -2147483643
         BorderStyle     =   1
         Appearance      =   1
         NumItems        =   1
         BeginProperty ColumnHeader(1) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
            Key             =   ""
            Object.Tag             =   ""
            Text            =   "Beschreibung"
            Object.Width           =   8290
         EndProperty
      End
      Begin VB.CommandButton cmdEditSyncLyric 
         Caption         =   "Edit"
         Height          =   375
         Left            =   -69420
         TabIndex        =   108
         ToolTipText     =   "Zeile bearbeiten"
         Top             =   1740
         Width           =   1155
      End
      Begin VB.CommandButton cmdDelSyncLyric 
         Caption         =   "&Delete"
         Height          =   375
         Left            =   -69420
         TabIndex        =   107
         ToolTipText     =   "Zeile löschen"
         Top             =   1320
         Width           =   1155
      End
      Begin VB.CommandButton cmdAddSyncLyric 
         Caption         =   "&Add"
         Height          =   375
         Left            =   -69420
         TabIndex        =   106
         ToolTipText     =   "Zeile hinzufügen"
         Top             =   900
         Width           =   1155
      End
      Begin VB.Frame Frame2 
         Caption         =   "Entries"
         Height          =   2355
         Left            =   -74700
         TabIndex        =   103
         Top             =   2280
         Width           =   6615
         Begin VB.ListBox listSYLT 
            Height          =   1425
            Left            =   120
            TabIndex        =   115
            Top             =   780
            Width           =   5055
         End
         Begin VB.TextBox txtLanguage5 
            Height          =   315
            Left            =   900
            MaxLength       =   3
            TabIndex        =   111
            Top             =   300
            Width           =   645
         End
         Begin VB.ComboBox comboTimes 
            Height          =   315
            ItemData        =   "frmID3V2.frx":064E
            Left            =   2160
            List            =   "frmID3V2.frx":0658
            Style           =   2  'Dropdown-Liste
            TabIndex        =   110
            Top             =   300
            Width           =   1395
         End
         Begin VB.ComboBox comboContentType 
            Height          =   315
            ItemData        =   "frmID3V2.frx":0673
            Left            =   4620
            List            =   "frmID3V2.frx":0675
            Style           =   2  'Dropdown-Liste
            TabIndex        =   109
            Top             =   300
            Width           =   1875
         End
         Begin VB.PictureBox Picture2 
            Appearance      =   0  '2D
            BorderStyle     =   0  'Kein
            ForeColor       =   &H80000008&
            Height          =   1335
            Left            =   5220
            ScaleHeight     =   1335
            ScaleWidth      =   1275
            TabIndex        =   104
            Top             =   780
            Width           =   1275
            Begin VB.CommandButton cmdAddSyncEntry 
               Caption         =   "&Add"
               Height          =   375
               Left            =   60
               TabIndex        =   4
               ToolTipText     =   "Zeile hinzufügen"
               Top             =   60
               Width           =   1155
            End
            Begin VB.CommandButton cmdDelSyncEntry 
               Caption         =   "&Delete"
               Height          =   375
               Left            =   60
               TabIndex        =   5
               ToolTipText     =   "Zeile löschen"
               Top             =   480
               Width           =   1155
            End
            Begin VB.CommandButton cmdEditSyncEntry 
               Caption         =   "Edit"
               Height          =   375
               Left            =   60
               TabIndex        =   6
               ToolTipText     =   "Zeile bearbeiten"
               Top             =   900
               Width           =   1155
            End
         End
         Begin VB.Label lbl 
            AutoSize        =   -1  'True
            BackStyle       =   0  'Transparent
            Caption         =   "Language:"
            ForeColor       =   &H80000008&
            Height          =   195
            Index           =   34
            Left            =   120
            TabIndex        =   114
            Top             =   360
            Width           =   765
         End
         Begin VB.Label lbl 
            AutoSize        =   -1  'True
            BackStyle       =   0  'Transparent
            Caption         =   "Time:"
            ForeColor       =   &H80000008&
            Height          =   195
            Index           =   37
            Left            =   1680
            TabIndex        =   113
            Top             =   360
            Width           =   390
         End
         Begin VB.Label lbl 
            AutoSize        =   -1  'True
            BackStyle       =   0  'Transparent
            Caption         =   "Content Type:"
            ForeColor       =   &H80000008&
            Height          =   195
            Index           =   38
            Left            =   3600
            TabIndex        =   112
            Top             =   360
            Width           =   1005
         End
      End
      Begin VB.TextBox txtTDRC 
         Height          =   315
         Left            =   3480
         TabIndex        =   52
         Top             =   600
         Width           =   1155
      End
      Begin VB.TextBox txtTIT1 
         Height          =   315
         Left            =   1440
         TabIndex        =   63
         Top             =   3840
         Width           =   5600
      End
      Begin VB.TextBox txtTLAN 
         Height          =   315
         Left            =   1440
         TabIndex        =   62
         Top             =   3480
         Width           =   5600
      End
      Begin VB.TextBox txtTENC 
         Height          =   315
         Left            =   1440
         TabIndex        =   61
         Top             =   3120
         Width           =   5600
      End
      Begin VB.TextBox txtTCOP 
         Height          =   315
         Left            =   1440
         TabIndex        =   60
         Top             =   2760
         Width           =   5600
      End
      Begin VB.TextBox txtTCOM 
         Height          =   315
         Left            =   1440
         TabIndex        =   59
         Top             =   2400
         Width           =   5600
      End
      Begin VB.TextBox txtTCON 
         Height          =   315
         Left            =   3480
         TabIndex        =   58
         Top             =   2040
         Width           =   3555
      End
      Begin VB.TextBox txtTYER 
         Height          =   315
         Left            =   1440
         TabIndex        =   57
         Top             =   2040
         Width           =   735
      End
      Begin VB.TextBox txtTALB 
         Height          =   315
         Left            =   1440
         TabIndex        =   56
         Top             =   1680
         Width           =   5600
      End
      Begin VB.TextBox txtTPE1 
         Height          =   315
         Left            =   1440
         TabIndex        =   55
         Top             =   1320
         Width           =   5600
      End
      Begin VB.TextBox txtTIT2 
         Height          =   315
         Left            =   1440
         TabIndex        =   54
         Top             =   960
         Width           =   5600
      End
      Begin VB.TextBox txtTRCK 
         Height          =   315
         Left            =   1440
         TabIndex        =   51
         Top             =   600
         Width           =   675
      End
      Begin VB.TextBox txtWCOM 
         Height          =   315
         Left            =   -72120
         TabIndex        =   50
         Top             =   540
         Width           =   4000
      End
      Begin VB.TextBox txtWCOP 
         Height          =   315
         Left            =   -72120
         TabIndex        =   49
         Top             =   900
         Width           =   4000
      End
      Begin VB.TextBox txtWOAF 
         Height          =   315
         Left            =   -72120
         TabIndex        =   48
         Top             =   1260
         Width           =   4000
      End
      Begin VB.TextBox txtWOAR 
         Height          =   315
         Left            =   -72120
         TabIndex        =   47
         Top             =   1620
         Width           =   4000
      End
      Begin VB.TextBox txtWOAS 
         Height          =   315
         Left            =   -72120
         TabIndex        =   46
         Top             =   1980
         Width           =   4000
      End
      Begin VB.TextBox txtWORS 
         Height          =   315
         Left            =   -72120
         TabIndex        =   45
         Top             =   2340
         Width           =   4000
      End
      Begin VB.TextBox txtWPAY 
         Height          =   315
         Left            =   -72120
         TabIndex        =   44
         Top             =   2700
         Width           =   4000
      End
      Begin VB.TextBox txtWPUB 
         Height          =   315
         Left            =   -72120
         TabIndex        =   43
         Top             =   3060
         Width           =   4000
      End
      Begin VB.TextBox txtUSLT 
         Height          =   2835
         Left            =   -74700
         MultiLine       =   -1  'True
         ScrollBars      =   3  'Beides
         TabIndex        =   42
         Top             =   1740
         Width           =   6525
      End
      Begin VB.TextBox txtCOMM 
         Height          =   2835
         Left            =   -74700
         MultiLine       =   -1  'True
         ScrollBars      =   3  'Beides
         TabIndex        =   38
         Top             =   1740
         Width           =   6525
      End
      Begin VB.TextBox txtLanguage1 
         Height          =   315
         Left            =   -73860
         TabIndex        =   36
         Top             =   1020
         Width           =   765
      End
      Begin VB.TextBox txtComment1 
         Height          =   315
         Left            =   -72960
         TabIndex        =   37
         Top             =   1020
         Width           =   4785
      End
      Begin VB.CommandButton cmdDelComment 
         Caption         =   "Delete Comment"
         Height          =   435
         Left            =   -73260
         TabIndex        =   40
         Top             =   4800
         Width           =   2475
      End
      Begin VB.CommandButton cmdDelAllComments 
         Caption         =   "Delete All Comments"
         Height          =   435
         Left            =   -70620
         TabIndex        =   41
         Top             =   4800
         Width           =   2475
      End
      Begin VB.ComboBox lfdNr1 
         Height          =   315
         Left            =   -74700
         TabIndex        =   35
         Text            =   "Combo1"
         Top             =   1020
         Width           =   735
      End
      Begin VB.ComboBox lfdNr2 
         Height          =   315
         Left            =   -74700
         TabIndex        =   34
         Text            =   "Combo1"
         Top             =   1020
         Width           =   735
      End
      Begin VB.TextBox txtComment2 
         Height          =   315
         Left            =   -73020
         TabIndex        =   33
         Top             =   1020
         Width           =   4845
      End
      Begin VB.TextBox txtLanguage2 
         Height          =   315
         Left            =   -73860
         TabIndex        =   32
         Top             =   1020
         Width           =   765
      End
      Begin VB.CommandButton cmdDelAllLyrics 
         Caption         =   "Delete All Lyrics"
         Height          =   435
         Left            =   -70620
         TabIndex        =   31
         Top             =   4800
         Width           =   2475
      End
      Begin VB.CommandButton cmdDelLyric 
         Caption         =   "Delete Lyric"
         Height          =   435
         Left            =   -73260
         TabIndex        =   30
         Top             =   4800
         Width           =   2475
      End
      Begin VB.CommandButton cmdOKComm 
         Caption         =   "OK"
         Height          =   435
         Left            =   -74700
         TabIndex        =   39
         Top             =   4800
         Width           =   1275
      End
      Begin VB.CommandButton cmdOKLyric 
         Caption         =   "OK"
         Height          =   435
         Left            =   -74700
         TabIndex        =   29
         Top             =   4800
         Width           =   1275
      End
      Begin VB.Frame Frame1 
         Caption         =   "User-Defined URL's:"
         ForeColor       =   &H80000008&
         Height          =   1935
         Left            =   -74700
         TabIndex        =   18
         Top             =   3480
         Width           =   6735
         Begin VB.TextBox txtWXXX 
            Height          =   315
            Left            =   1020
            TabIndex        =   25
            Top             =   840
            Width           =   5565
         End
         Begin VB.ComboBox lfdNr3 
            Height          =   315
            Left            =   180
            TabIndex        =   24
            Text            =   "Combo1"
            Top             =   480
            Width           =   735
         End
         Begin VB.TextBox txtComment3 
            Height          =   315
            Left            =   1020
            TabIndex        =   23
            Top             =   480
            Width           =   5565
         End
         Begin VB.PictureBox Picture1 
            BorderStyle     =   0  'Kein
            Height          =   675
            Left            =   60
            ScaleHeight     =   675
            ScaleWidth      =   6495
            TabIndex        =   19
            Top             =   1200
            Width           =   6495
            Begin VB.CommandButton cmdOKURL 
               Caption         =   "OK"
               Height          =   435
               Left            =   120
               TabIndex        =   22
               Top             =   60
               Width           =   1275
            End
            Begin VB.CommandButton cmdDelURL 
               Caption         =   "Remove Current URL"
               Height          =   435
               Left            =   1740
               TabIndex        =   21
               Top             =   60
               Width           =   2115
            End
            Begin VB.CommandButton cmdDelAllURLs 
               Caption         =   "Delete All URL"
               Height          =   435
               Left            =   4200
               TabIndex        =   20
               Top             =   60
               Width           =   2115
            End
         End
         Begin VB.Label lbl 
            Alignment       =   1  'Rechts
            AutoSize        =   -1  'True
            BackStyle       =   0  'Transparent
            Caption         =   "URL:"
            ForeColor       =   &H80000008&
            Height          =   195
            Index           =   22
            Left            =   540
            TabIndex        =   28
            Top             =   900
            Width           =   375
         End
         Begin VB.Label lblItem 
            AutoSize        =   -1  'True
            BackStyle       =   0  'Transparent
            Caption         =   "Entry #:"
            ForeColor       =   &H80000008&
            Height          =   195
            Index           =   1
            Left            =   180
            TabIndex        =   27
            Top             =   240
            Width           =   555
         End
         Begin VB.Label lbl 
            AutoSize        =   -1  'True
            BackStyle       =   0  'Transparent
            Caption         =   "Description:"
            ForeColor       =   &H80000008&
            Height          =   195
            Index           =   27
            Left            =   1020
            TabIndex        =   26
            Top             =   240
            Width           =   840
         End
      End
      Begin VB.TextBox txtUSER 
         Height          =   2835
         Left            =   -74700
         MultiLine       =   -1  'True
         ScrollBars      =   3  'Beides
         TabIndex        =   17
         Top             =   1740
         Width           =   6525
      End
      Begin VB.ComboBox lfdNr4 
         Height          =   315
         Left            =   -74700
         TabIndex        =   16
         Text            =   "Combo1"
         Top             =   1020
         Width           =   735
      End
      Begin VB.TextBox txtLanguage4 
         Height          =   315
         Left            =   -73860
         TabIndex        =   15
         Top             =   1020
         Width           =   765
      End
      Begin VB.CommandButton cmdOKFrame 
         Caption         =   "OK"
         Height          =   435
         Left            =   -74700
         TabIndex        =   14
         Top             =   4800
         Width           =   1275
      End
      Begin VB.CommandButton cmdDelAllFrames 
         Caption         =   "Delete All Frames"
         Height          =   435
         Left            =   -70620
         TabIndex        =   13
         Top             =   4800
         Width           =   2475
      End
      Begin VB.CommandButton cmdDelFrame 
         Caption         =   "Delete Frame"
         Height          =   435
         Left            =   -73260
         TabIndex        =   12
         Top             =   4800
         Width           =   2475
      End
      Begin VB.TextBox txtTPE2 
         Height          =   315
         Left            =   1440
         TabIndex        =   64
         Top             =   4200
         Width           =   5600
      End
      Begin VB.TextBox txtTSSE 
         Height          =   315
         Left            =   1440
         TabIndex        =   65
         Top             =   4560
         Width           =   5600
      End
      Begin VB.TextBox txtTBPM 
         Height          =   315
         Left            =   1440
         TabIndex        =   66
         Top             =   4920
         Width           =   5600
      End
      Begin VB.TextBox txtTOWN 
         Height          =   315
         Left            =   1440
         TabIndex        =   67
         Top             =   5280
         Width           =   5600
      End
      Begin VB.CommandButton cmdDelAllPics 
         Caption         =   "Delete All Pictures"
         Height          =   375
         Left            =   -69660
         TabIndex        =   11
         Top             =   5160
         Width           =   1695
      End
      Begin VB.CommandButton cmdNewPic 
         Caption         =   "New Picture"
         Height          =   375
         Left            =   -74760
         TabIndex        =   10
         Top             =   5160
         Width           =   1395
      End
      Begin VB.CommandButton cmdDelPic 
         Caption         =   "Delete Picture"
         Height          =   375
         Left            =   -71160
         TabIndex        =   9
         Top             =   5160
         Width           =   1395
      End
      Begin VB.CommandButton cmdOkSyncLyric 
         Caption         =   "Sync Lyrics"
         Height          =   435
         Left            =   -73755
         TabIndex        =   7
         Top             =   4800
         Width           =   2475
      End
      Begin VB.CommandButton cmdDelAllSyncLyric 
         Caption         =   "Remove All Song Lyrics"
         Height          =   435
         Left            =   -71175
         TabIndex        =   8
         Top             =   4800
         Width           =   2475
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Play Count:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   35
         Left            =   4875
         TabIndex        =   118
         Top             =   660
         Width           =   810
      End
      Begin VB.Image noPicture 
         Height          =   1200
         Left            =   -71520
         Picture         =   "frmID3V2.frx":0677
         Top             =   5280
         Visible         =   0   'False
         Width           =   1440
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Release Date:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   15
         Left            =   2445
         TabIndex        =   102
         Top             =   660
         Width           =   1020
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Categories:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   13
         Left            =   600
         TabIndex        =   101
         Top             =   3900
         Width           =   795
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Language:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   12
         Left            =   600
         TabIndex        =   100
         Top             =   3540
         Width           =   765
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Genre:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   11
         Left            =   2925
         TabIndex        =   99
         Top             =   2100
         Width           =   480
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Track #:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   10
         Left            =   750
         TabIndex        =   98
         Top             =   660
         Width           =   615
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Encoder:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   9
         Left            =   720
         TabIndex        =   97
         Top             =   3180
         Width           =   645
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Copyright:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   7
         Left            =   660
         TabIndex        =   96
         Top             =   2820
         Width           =   705
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Composer:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   5
         Left            =   615
         TabIndex        =   95
         Top             =   2460
         Width           =   750
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Year:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   3
         Left            =   990
         TabIndex        =   94
         Top             =   2100
         Width           =   375
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Album:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   2
         Left            =   885
         TabIndex        =   93
         Top             =   1740
         Width           =   480
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Artist:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   1
         Left            =   975
         TabIndex        =   92
         Top             =   1380
         Width           =   390
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Title"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   0
         Left            =   1065
         TabIndex        =   91
         Top             =   1020
         Width           =   300
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Commercial Information URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   6
         Left            =   -74280
         TabIndex        =   90
         Top             =   600
         Width           =   2055
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Copyright Information URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   14
         Left            =   -74115
         TabIndex        =   89
         Top             =   960
         Width           =   1905
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Official Audiofile Webpage URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   16
         Left            =   -74505
         TabIndex        =   88
         Top             =   1320
         Width           =   2295
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Official Artist Webpage URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   17
         Left            =   -74250
         TabIndex        =   87
         Top             =   1680
         Width           =   2040
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Official Audio Source Webpage URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   18
         Left            =   -74880
         TabIndex        =   86
         Top             =   2040
         Width           =   2655
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Official Internet Radio Station URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   19
         Left            =   -74715
         TabIndex        =   85
         Top             =   2400
         Width           =   2490
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Payment URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   20
         Left            =   -73245
         TabIndex        =   84
         Top             =   2760
         Width           =   1035
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Publishers Official Webpage URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   21
         Left            =   -74625
         TabIndex        =   83
         Top             =   3120
         Width           =   2415
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Lyrics:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   8
         Left            =   -74370
         TabIndex        =   82
         Top             =   1500
         Width           =   450
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Comment:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   4
         Left            =   -74700
         TabIndex        =   81
         Top             =   1500
         Width           =   705
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Language:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   23
         Left            =   -73860
         TabIndex        =   80
         Top             =   780
         Width           =   765
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Description:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   24
         Left            =   -72960
         TabIndex        =   79
         Top             =   780
         Width           =   840
      End
      Begin VB.Label lblItem 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Entry #"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   25
         Left            =   -74700
         TabIndex        =   78
         Top             =   780
         Width           =   510
      End
      Begin VB.Label lblItem 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Entry #:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   0
         Left            =   -74700
         TabIndex        =   77
         Top             =   780
         Width           =   555
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Description:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   25
         Left            =   -73020
         TabIndex        =   76
         Top             =   780
         Width           =   840
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Language:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   26
         Left            =   -73860
         TabIndex        =   75
         Top             =   780
         Width           =   765
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   " Text:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   29
         Left            =   -74745
         TabIndex        =   74
         Top             =   1500
         Width           =   405
      End
      Begin VB.Label lblItem 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Entry #"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   2
         Left            =   -74700
         TabIndex        =   73
         Top             =   780
         Width           =   510
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Language:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   28
         Left            =   -73860
         TabIndex        =   72
         Top             =   780
         Width           =   765
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Band/Orchestra:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   30
         Left            =   210
         TabIndex        =   71
         Top             =   4260
         Width           =   1185
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Encoder Settings:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   31
         Left            =   135
         TabIndex        =   70
         Top             =   4620
         Width           =   1260
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Beats per Minute:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   32
         Left            =   150
         TabIndex        =   69
         Top             =   4980
         Width           =   1245
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "File Owner:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   33
         Left            =   600
         TabIndex        =   68
         Top             =   5340
         Width           =   795
      End
   End
   Begin ComctlLib.ImageList picimglist 
      Left            =   6960
      Top             =   6000
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   96
      ImageHeight     =   96
      MaskColor       =   12632256
      UseMaskColor    =   0   'False
      _Version        =   327682
   End
End
Attribute VB_Name = "frmID3V2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim id%, w%, Bilddatei$, tmp$
Dim syncItem As ListItem, picItem As ListItem

Public Sub updateComments()
    ' List all comment tags
    txtCOMM = ""
    txtLanguage1 = ""
    txtComment1 = ""
    lfdNr1.Clear

    For id% = 1 To genie.ID3V2GetComments
        lfdNr1.AddItem id%
    Next id%

    If lfdNr1.ListCount > 0 Then lfdNr1.ListIndex = 0
End Sub

Public Sub updateFrames()
    ' List all URL tags
    txtUSER = ""
    txtLanguage4 = ""
    lfdNr4.Clear

    For id% = 1 To genie.ID3V2GetUserFrames
        lfdNr4.AddItem id%
    Next id%

    If lfdNr4.ListCount > 0 Then lfdNr4.ListIndex = 0
End Sub

Public Sub updateLyrics()
    ' List all lyrics tags
    txtUSLT = ""
    txtLanguage2 = ""
    txtComment2 = ""
    lfdNr2.Clear

    For id% = 1 To genie.ID3V2GetLyrics
        lfdNr2.AddItem id%
    Next id%

    If lfdNr2.ListCount > 0 Then lfdNr2.ListIndex = 0
End Sub

Public Sub updatePictures()
    picimgview.ListItems.Clear
    picimgview.Icons = Nothing
    picimglist.ListImages.Clear
    picimglist.ImageHeight = 96
    picimglist.ImageWidth = 96
    picimglist.UseMaskColor = False
    picimglist.BackColor = vbBlue
    If genie.ID3V2GetPictures = 0 Then Exit Sub

    For id% = 1 To genie.ID3V2GetPictures
      If ID3V2SetPictureWithArray(id%, frmAPIC.img) = False Then
        Set frmAPIC.img.Picture = noPicture.Picture
      End If
      picimglist.ListImages.Add , , frmAPIC.img
      Set picimgview.Icons = picimglist
      Set picItem = picimgview.ListItems.Add(, , genie.ID3V2GetPictureDescription(id%), picimglist.ListImages.count, 0)
      picItem.SubItems(1) = genie.ID3V2GetPictureType(id%)
      picItem.SubItems(2) = genie.ID3V2GetPictureMime(id%)
      picItem.SubItems(3) = id%
    Next id%
    picimgview.Icons = picimglist
End Sub

Public Sub updateSyncLyrics()
    ' List all syncLyrics tags
    viewSYLTDescr.ListItems.Clear
    txtLanguage5 = ""
    listSYLT.Clear

    For id% = 1 To genie.ID3V2GetSyncLyrics
        viewSYLTDescr.ListItems.Add id%, , genie.ID3V2GetSyncLyricDescription(id%), 0, 0
    Next id%

    If genie.ID3V2GetSyncLyrics > 0 Then
        Set viewSYLTDescr.SelectedItem = viewSYLTDescr.ListItems(1)
        viewSYLTDescr_ItemClick viewSYLTDescr.ListItems(1)
        Frame2.Visible = True
    Else
        Frame2.Visible = False
    End If
End Sub

Public Sub updateUI()
    Dim id%, Language$, Description$, Txt$
    ' Fill form with ID3v2-Tag Data
    frmID3V2.txtTPE1 = genie.ID3V2Artist
    frmID3V2.txtTALB = genie.ID3V2Album
    frmID3V2.txtTIT2 = genie.ID3V2Title
    frmID3V2.txtTYER = genie.ID3V2Year
    frmID3V2.txtTCON = genie.ID3V2Genre
    frmID3V2.txtTRCK = genie.ID3V2Track
    frmID3V2.txtTDRC = genie.ID3V2RecordingTime
    frmID3V2.txtTIT1 = genie.ID3V2Category
    frmID3V2.txtTLAN = genie.ID3V2Language
    frmID3V2.txtTENC = genie.ID3V2Encoder
    frmID3V2.txtTCOP = genie.ID3V2Copyright
    frmID3V2.txtTCOM = genie.ID3V2Composer
    frmID3V2.txtTPE2 = genie.ID3V2Orchestra
    frmID3V2.txtTSSE = genie.ID3V2EncodingSettings
    frmID3V2.txtTBPM = genie.ID3V2BeatsPerMinute
    frmID3V2.txtTOWN = genie.ID3V2FileOwner
    frmID3V2.txtPCNT = genie.ID3V2PlayCounter
    ' URLs
    frmID3V2.txtWCOM = genie.ID3V2CommercialURL
    frmID3V2.txtWCOP = genie.ID3V2CopyrightURL
    frmID3V2.txtWOAF = genie.ID3V2AudiofileURL
    frmID3V2.txtWOAR = genie.ID3V2ArtistURL
    frmID3V2.txtWOAS = genie.ID3V2AudiosourceURL
    frmID3V2.txtWORS = genie.ID3V2RadiostationURL
    frmID3V2.txtWPAY = genie.ID3V2PaymentURL
    frmID3V2.txtWPUB = genie.ID3V2PublisherURL
    
    ' if, not supported tags deactivate old version
    If genie.ID3V2GetVersion = "2.2" Then
        frmID3V2.txtTOWN.Enabled = False
        frmID3V2.txtTOWN.BackColor = &H80000003
        frmID3V2.txtWORS.Enabled = False
        frmID3V2.txtWORS.BackColor = &H80000003
        frmID3V2.txtWPAY.Enabled = False
        frmID3V2.txtWPAY.BackColor = &H80000003
        frmID3V2.register.TabEnabled(5) = False
    Else
        frmID3V2.txtTOWN.Enabled = True
        frmID3V2.txtTOWN.BackColor = &H80000005
        frmID3V2.txtWORS.Enabled = True
        frmID3V2.txtWORS.BackColor = &H80000005
        frmID3V2.txtWPAY.Enabled = True
        frmID3V2.txtWPAY.BackColor = &H80000005
        frmID3V2.register.TabEnabled(5) = True
    End If
    frmID3V2.updatePictures
    frmID3V2.updateComments
    frmID3V2.updateLyrics
    frmID3V2.updateURLs
    frmID3V2.updateFrames
    frmID3V2.updateSyncLyrics
End Sub

Public Sub updateURLs()
    ' List all URL tags
    txtWXXX = ""
    txtComment3 = ""
    lfdNr3.Clear

    For id% = 1 To genie.ID3V2GetUserURLs
        lfdNr3.AddItem id%
    Next id%

    If lfdNr3.ListCount > 0 Then lfdNr3.ListIndex = 0
End Sub

Private Sub updateSyncEntrys()
    Dim time_id As Integer, content_id As Integer, arr() As String
    listSYLT.Clear
    ' get active element
    Set syncItem = viewSYLTDescr.SelectedItem
    If syncItem Is Nothing Then Exit Sub
    id% = syncItem.Index
    txtLanguage5.Text = genie.ID3V2GetSyncLyricLanguage(id%)
    If txtLanguage5.Text = "" Then txtLanguage5.Text = "deu"
    time_id = genie.ID3V2GetSyncLyricTimeFormat(id%)
    If time_id = -1 Then time_id = 2
    content_id = genie.ID3V2GetSyncLyricContentType(id%)
    If content_id = -1 Then content_id = 0
    comboContentType.ListIndex = content_id
    comboTimes.ListIndex = time_id - 1
    ' transfer strings into list entries
    tmp = genie.ID3V2GetSyncLyric(id%)
    arr = Split(tmp, vbCrLf, -1)

    For w% = 0 To UBound(arr)
        listSYLT.AddItem arr(w%)
    Next w%

    Frame2.Visible = True
End Sub

Private Sub cmdAddSyncEntry_Click()
    frmSYLT.Show 1
    If frmSYLT.chk.Value = 1 Then
        listSYLT.AddItem "[" & Format(Val(frmSYLT.txtTime.Text), "00000000") & "]" & Trim$(frmSYLT.txtEntry.Text)
    End If
End Sub

Private Sub cmdAddSyncLyric_Click()
    Set syncItem = viewSYLTDescr.ListItems.Add(, , "<Enter description here>", 0, 0)
    Set viewSYLTDescr.SelectedItem = syncItem
    viewSYLTDescr_ItemClick syncItem
    viewSYLTDescr.SetFocus
    viewSYLTDescr.StartLabelEdit
End Sub

Private Sub cmdCancel_Click()
    Me.Hide
End Sub

Private Sub cmdDelAllComments_Click()
    genie.ID3V2DeleteComments
    updateComments
End Sub

Private Sub cmdDelAllFrames_Click()
    genie.ID3V2DeleteUserFrames
    updateFrames
End Sub

Private Sub cmdDelAllLyrics_Click()
    genie.ID3V2DeleteLyrics
    updateLyrics
End Sub

Private Sub cmdDelAllPics_Click()
    genie.ID3V2DeletePictures
    Set frmAPIC.img.Picture = Nothing
    updatePictures
End Sub

Private Sub cmdDelAllSyncLyric_Click()
    genie.ID3V2DeleteSyncLyrics
    updateSyncLyrics
End Sub

Private Sub cmdDelAllURLs_Click()
    genie.ID3V2DeleteUserURLs
    updateURLs
End Sub

Private Sub cmdDelComment_Click()
    id% = Val(lfdNr1.List(lfdNr1.ListIndex))
    genie.ID3V2SetComment id%, "", "", ""
    updateComments
End Sub

Private Sub cmdDelete_Click()
    DeleteID3v2Tag
    updateUI
End Sub

Private Sub cmdDelFrame_Click()
    id% = Val(lfdNr4.List(lfdNr4.ListIndex))
    genie.ID3V2SetUserFrame id%, "", ""
    updateFrames
End Sub

Private Sub cmdDelLyric_Click()
    id% = Val(lfdNr2.List(lfdNr2.ListIndex))
    genie.ID3V2SetLyric id%, "", "", ""
    updateLyrics
End Sub

Private Sub cmdDelPic_Click()
    Set picItem = picimgview.SelectedItem
    If picItem Is Nothing Then Exit Sub
    id% = Val(picItem.SubItems(3))
    genie.ID3V2DeletePicture id%
    picimgview.ListItems.Remove picItem.Index
End Sub

Private Sub cmdDelSyncEntry_Click()
    If listSYLT.ListIndex = -1 Then Exit Sub
    listSYLT.RemoveItem listSYLT.ListIndex
End Sub

Private Sub cmdDelSyncLyric_Click()
    Set syncItem = viewSYLTDescr.SelectedItem
    If syncItem Is Nothing Then Exit Sub
    id% = syncItem.Index
    viewSYLTDescr.ListItems.Remove id%
    genie.ID3V2SetSyncLyric id%, "DEU", "", "", 0, 1
    updateSyncLyrics
End Sub

Private Sub cmdDelURL_Click()
    id% = Val(lfdNr3.List(lfdNr3.ListIndex))
    genie.ID3V2SetUserURL id%, "", ""
    updateURLs
End Sub

Private Sub cmdEditSyncEntry_Click()
    Dim entry As String
    If listSYLT.ListIndex = -1 Then Exit Sub
    entry = listSYLT.List(listSYLT.ListIndex)
    frmSYLT.txtTime = Val(Mid$(entry, 2, 8))
    frmSYLT.txtEntry = Mid$(entry, 11)
    frmSYLT.Show 1
    If frmSYLT.chk.Value = 1 Then
        entry = "[" & Format(Val(frmSYLT.txtTime.Text), "00000000") & "]" & Trim$(frmSYLT.txtEntry.Text)
        listSYLT.List(listSYLT.ListIndex) = entry
    End If
End Sub

Private Sub cmdEditSyncLyric_Click()
    Set syncItem = viewSYLTDescr.SelectedItem
    If syncItem Is Nothing Then Exit Sub
    viewSYLTDescr.SetFocus
    viewSYLTDescr.StartLabelEdit
End Sub

Private Sub cmdNewPic_Click()
    ' Add Picture
    On Error GoTo ErrHandler
    ' Open Dialog
    frmID3V2.StandardDlg.ShowOpen
    Bilddatei$ = frmID3V2.StandardDlg.FileName
    On Error GoTo WrongPicture
    Set frmAPIC.img.Picture = LoadPicture(Bilddatei$)
    frmAPIC.idInfo.Caption = genie.ID3V2GetPictures + 1
    frmAPIC.dateiInfo.Caption = Bilddatei$
    frmAPIC.linkCheck.Value = 0
    frmAPIC.picDescription.Enabled = True
    frmAPIC.picTypeCombo.Enabled = True
    frmAPIC.linkCheck.Enabled = True
    frmAPIC.cmdOk.Enabled = True
    frmAPIC.picMime = ""
    frmAPIC.Show 1
    Exit Sub
ErrHandler:
    Debug.Print Err.Description
    ' User selected "Cancel"
    Exit Sub
WrongPicture:
    MsgBox "The picture could not be loaded or is invalid", vbCritical
    Exit Sub
End Sub

Private Sub cmdOKComm_Click()
    id% = Val(lfdNr1.Text)
    genie.ID3V2SetComment id%, txtLanguage1, txtComment1, txtCOMM
    updateComments
End Sub

Private Sub cmdOKFrame_Click()
    id% = Val(lfdNr4.Text)
    genie.ID3V2SetUserFrame id%, txtLanguage4, txtUSER.Text
    updateFrames
End Sub

Private Sub cmdOKLyric_Click()
    id% = Val(lfdNr2.Text)
    genie.ID3V2SetLyric id%, txtLanguage2, txtComment2, txtUSLT.Text
    updateLyrics
End Sub

Private Sub cmdOkSyncLyric_Click()
    Set syncItem = viewSYLTDescr.SelectedItem
    If syncItem Is Nothing Then Exit Sub
    tmp = ""

    For w% = 0 To listSYLT.ListCount - 2
        tmp = tmp & listSYLT.List(w%) & vbCrLf
    Next w%

    tmp = tmp & listSYLT.List(w%)
    If genie.ID3V2SetSyncLyric(id%, txtLanguage5.Text, syncItem.Text, tmp, comboContentType.ListIndex, comboTimes.ListIndex + 1) = False Then
        MsgBox "An Error Has Occurred:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Eintragen des SyncLyric-Tag"
    End If
    '  updateSyncLyrics
End Sub

Private Sub cmdOKURL_Click()
    id% = Val(lfdNr3.Text)
    genie.ID3V2SetUserURL id%, txtComment3, txtWXXX.Text
    updateURLs
End Sub

Private Sub cmdShowPic_Click()
    If picimgview.SelectedItem Is Nothing Then Exit Sub
    frmAPIC.picDescription.Enabled = False
    frmAPIC.picTypeCombo.Enabled = False
    frmAPIC.linkCheck.Enabled = False
    frmAPIC.cmdOk.Enabled = False
    frmAPIC.Show 1
End Sub

Private Sub cmdUpdate_Click()
    ' Save as Unicode or ANSI
    If checkUnicode.Value = 1 Then
      genie.ID3V2SetTextFormatToUnicode
    Else
      genie.ID3V2SetTextFormatToAnsi
    End If
    genie.ID3V2Artist = frmID3V2.txtTPE1
    genie.ID3V2Album = frmID3V2.txtTALB
    genie.ID3V2Title = frmID3V2.txtTIT2
    genie.ID3V2Year = frmID3V2.txtTYER
    genie.ID3V2Genre = frmID3V2.txtTCON
    genie.ID3V2Track = frmID3V2.txtTRCK
    genie.ID3V2RecordingTime = frmID3V2.txtTDRC
    genie.ID3V2Category = frmID3V2.txtTIT1
    genie.ID3V2Language = frmID3V2.txtTLAN
    genie.ID3V2Encoder = frmID3V2.txtTENC
    genie.ID3V2Copyright = frmID3V2.txtTCOP
    genie.ID3V2Composer = frmID3V2.txtTCOM
    genie.ID3V2Orchestra = frmID3V2.txtTPE2
    genie.ID3V2EncodingSettings = frmID3V2.txtTSSE
    genie.ID3V2BeatsPerMinute = frmID3V2.txtTBPM
    genie.ID3V2FileOwner = frmID3V2.txtTOWN
    genie.ID3V2PlayCounter = Val(frmID3V2.txtPCNT)
    ' URLs
    genie.ID3V2CommercialURL = frmID3V2.txtWCOM
    genie.ID3V2CopyrightURL = frmID3V2.txtWCOP
    genie.ID3V2AudiofileURL = frmID3V2.txtWOAF
    genie.ID3V2ArtistURL = frmID3V2.txtWOAR
    genie.ID3V2AudiosourceURL = frmID3V2.txtWOAS
    genie.ID3V2RadiostationURL = frmID3V2.txtWORS
    genie.ID3V2PaymentURL = frmID3V2.txtWPAY
    genie.ID3V2PublisherURL = frmID3V2.txtWPUB
    ' Save Tag
    If genie.ID3V2SaveChangesToFile(Datei) = False Then
        MsgBox "An Error Occurred While Saving the Tag" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Error saving the ID3v2 tag"
    End If
End Sub

Private Sub Form_Load()
    comboTimes.Clear
    comboTimes.AddItem "Mpeg Frames"
    comboTimes.AddItem "Millisekunden"
    comboContentType.Clear
    comboContentType.AddItem "Andere"
    comboContentType.AddItem "Songtexte"
    comboContentType.AddItem "Texte"
    comboContentType.AddItem "Bewegungen"
    comboContentType.AddItem "Ereignisse"
    comboContentType.AddItem "Akkorde"
    comboContentType.AddItem "Belangloses"
    comboContentType.AddItem "URLS zu Webseiten"
    comboContentType.AddItem "URLS zu Bildern"
End Sub

Private Sub lfdNr1_Click()
    id% = Val(lfdNr1.List(lfdNr1.ListIndex))
    txtLanguage1 = genie.ID3V2GetCommentLanguage(id%)
    txtComment1 = genie.ID3V2GetCommentDescription(id%)
    txtCOMM = genie.ID3V2GetComment(id%)
End Sub

Private Sub lfdNr2_Click()
    id% = Val(lfdNr2.List(lfdNr2.ListIndex))
    txtLanguage2 = genie.ID3V2GetLyricLanguage(id%)
    txtComment2 = genie.ID3V2GetLyricDescription(id%)
    txtUSLT = genie.ID3V2GetLyric(id%)
End Sub

Private Sub lfdNr3_Click()
    id% = Val(lfdNr3.List(lfdNr3.ListIndex))
    txtComment3 = genie.ID3V2GetUserURLDescription(id%)
    txtWXXX = genie.ID3V2GetUserURL(id%)
End Sub

Private Sub lfdNr4_Click()
    id% = Val(lfdNr4.List(lfdNr4.ListIndex))
    txtLanguage4 = genie.ID3V2GetUserFrameLanguage(id%)
    txtUSER = genie.ID3V2GetUserFrame(id%)
End Sub

Private Sub picimgview_ItemClick(ByVal item As ComctlLib.ListItem)
    Set frmAPIC.img.Picture = picimglist.ListImages(item.Index).Picture
    frmAPIC.idInfo = item.SubItems(3)
    frmAPIC.picDescription.Text = item.Text
    frmAPIC.picTypeCombo.ListIndex = item.SubItems(1)
    frmAPIC.picMime.Text = item.SubItems(2)
    If Left$(frmAPIC.picMime.Text, 3) = "-->" Then
        frmAPIC.linkCheck.Value = 1
    Else
        frmAPIC.linkCheck.Value = 0
    End If
End Sub

Private Sub viewSYLTDescr_ItemClick(ByVal item As ComctlLib.ListItem)
    Set syncItem = item
    updateSyncEntrys
End Sub

Public Function ID3V2SetPictureWithArray(ByVal Index As Integer, img As Image) As Boolean
  Dim IID_IPicture(3) As Long
  Dim oPicture As IPicture
  Dim oStream As IUnknown
  Dim F As Integer
  Dim Data() As Byte
    
  IID_IPicture(0) = &H7BF80980
  IID_IPicture(1) = &H101ABF32
  IID_IPicture(2) = &HAA00BB8B
  IID_IPicture(3) = &HAB0C3000
  On Error GoTo BildFehler
  Dim ln As Long
  Dim Result() As Byte
  Result = genie.ID3V2GetPictureArray(Index)
  Call CreateStreamOnHGlobal(Result(0), False, oStream)
  Call OleLoadPicture(oStream, 0, 0, IID_IPicture(0), oPicture)
  Set img.Picture = oPicture
  ID3V2SetPictureWithArray = True
  Exit Function
BildFehler:
  ID3V2SetPictureWithArray = False
  On Error GoTo 0
End Function
