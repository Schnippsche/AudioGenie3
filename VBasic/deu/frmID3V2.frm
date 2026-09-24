VERSION 5.00
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "COMCTL32.OCX"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmID3V2 
   Caption         =   "ID3V2-Tag ändern"
   ClientHeight    =   6735
   ClientLeft      =   6315
   ClientTop       =   1500
   ClientWidth     =   7545
   LinkTopic       =   "Form2"
   ScaleHeight     =   6735
   ScaleWidth      =   7545
   Begin VB.CommandButton cmdParam 
      Caption         =   "Einstellungen"
      Height          =   435
      Left            =   240
      TabIndex        =   114
      Top             =   6120
      Width           =   1400
   End
   Begin MSComDlg.CommonDialog StandardDlg 
      Left            =   7080
      Top             =   5820
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      CancelError     =   -1  'True
      DialogTitle     =   "Bild zuweisen"
      Filter          =   "Bilder ( bmp, jpg, gif, ico )|*.bmp;*.jpg;*.gif;*.ico"
   End
   Begin VB.CommandButton cmdCancel 
      Caption         =   "Fertig"
      Height          =   435
      Left            =   5445
      TabIndex        =   2
      Top             =   6120
      Width           =   1400
   End
   Begin VB.CommandButton cmdUpdate 
      Caption         =   "Änderungen speichern"
      Height          =   435
      Left            =   3465
      TabIndex        =   1
      Top             =   6120
      Width           =   1815
   End
   Begin VB.CommandButton cmdDelete 
      Caption         =   "Tag löschen"
      Height          =   435
      Left            =   1905
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
      Tab             =   3
      TabsPerRow      =   7
      TabHeight       =   520
      WordWrap        =   0   'False
      TabCaption(0)   =   "Allgemein"
      TabPicture(0)   =   "frmID3V2.frx":0000
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "lbl(33)"
      Tab(0).Control(1)=   "lbl(32)"
      Tab(0).Control(2)=   "lbl(31)"
      Tab(0).Control(3)=   "lbl(30)"
      Tab(0).Control(4)=   "lbl(0)"
      Tab(0).Control(5)=   "lbl(1)"
      Tab(0).Control(6)=   "lbl(2)"
      Tab(0).Control(7)=   "lbl(3)"
      Tab(0).Control(8)=   "lbl(5)"
      Tab(0).Control(9)=   "lbl(7)"
      Tab(0).Control(10)=   "lbl(9)"
      Tab(0).Control(11)=   "lbl(10)"
      Tab(0).Control(12)=   "lbl(11)"
      Tab(0).Control(13)=   "lbl(12)"
      Tab(0).Control(14)=   "lbl(13)"
      Tab(0).Control(15)=   "lbl(15)"
      Tab(0).Control(16)=   "lbl(35)"
      Tab(0).Control(17)=   "txtTOWN"
      Tab(0).Control(18)=   "txtTBPM"
      Tab(0).Control(19)=   "txtTSSE"
      Tab(0).Control(20)=   "txtTPE2"
      Tab(0).Control(21)=   "txtTRCK"
      Tab(0).Control(22)=   "txtTIT2"
      Tab(0).Control(23)=   "txtTPE1"
      Tab(0).Control(24)=   "txtTALB"
      Tab(0).Control(25)=   "txtTYER"
      Tab(0).Control(26)=   "txtTCON"
      Tab(0).Control(27)=   "txtTCOM"
      Tab(0).Control(28)=   "txtTCOP"
      Tab(0).Control(29)=   "txtTENC"
      Tab(0).Control(30)=   "txtTLAN"
      Tab(0).Control(31)=   "txtTIT1"
      Tab(0).Control(32)=   "txtTDRC"
      Tab(0).Control(33)=   "txtPCNT"
      Tab(0).ControlCount=   34
      TabCaption(1)   =   "Kommentare"
      TabPicture(1)   =   "frmID3V2.frx":001C
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
      TabPicture(2)   =   "frmID3V2.frx":0038
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
      TabCaption(3)   =   "Bilder"
      TabPicture(3)   =   "frmID3V2.frx":0054
      Tab(3).ControlEnabled=   -1  'True
      Tab(3).Control(0)=   "noPicture"
      Tab(3).Control(0).Enabled=   0   'False
      Tab(3).Control(1)=   "cmdDelPic"
      Tab(3).Control(1).Enabled=   0   'False
      Tab(3).Control(2)=   "cmdNewPic"
      Tab(3).Control(2).Enabled=   0   'False
      Tab(3).Control(3)=   "cmdDelAllPics"
      Tab(3).Control(3).Enabled=   0   'False
      Tab(3).Control(4)=   "picimgview"
      Tab(3).Control(4).Enabled=   0   'False
      Tab(3).Control(5)=   "cmdShowPic"
      Tab(3).Control(5).Enabled=   0   'False
      Tab(3).ControlCount=   6
      TabCaption(4)   =   "Songtext"
      TabPicture(4)   =   "frmID3V2.frx":0070
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
      TabCaption(5)   =   "UserFrames"
      TabPicture(5)   =   "frmID3V2.frx":008C
      Tab(5).ControlEnabled=   0   'False
      Tab(5).Control(0)=   "lbl(28)"
      Tab(5).Control(1)=   "lbl(29)"
      Tab(5).Control(2)=   "cmdDelFrame"
      Tab(5).Control(3)=   "cmdDelAllFrames"
      Tab(5).Control(4)=   "cmdOKFrame"
      Tab(5).Control(5)=   "txtUSER"
      Tab(5).Control(6)=   "lfdNr4"
      Tab(5).ControlCount=   7
      TabCaption(6)   =   "SyncLyrics"
      TabPicture(6)   =   "frmID3V2.frx":00A8
      Tab(6).ControlEnabled=   0   'False
      Tab(6).Control(0)=   "cmdDelAllSyncLyric"
      Tab(6).Control(1)=   "cmdOkSyncLyric"
      Tab(6).Control(2)=   "Frame2"
      Tab(6).Control(3)=   "cmdAddSyncLyric"
      Tab(6).Control(4)=   "cmdDelSyncLyric"
      Tab(6).Control(5)=   "cmdEditSyncLyric"
      Tab(6).Control(6)=   "viewSYLTDescr"
      Tab(6).ControlCount=   7
      Begin VB.ComboBox lfdNr4 
         Height          =   315
         Left            =   -74640
         TabIndex        =   115
         Text            =   "Combo1"
         Top             =   1020
         Width           =   1395
      End
      Begin VB.TextBox txtPCNT 
         Height          =   315
         Left            =   -69240
         TabIndex        =   49
         Top             =   600
         Width           =   1275
      End
      Begin VB.CommandButton cmdShowPic 
         Caption         =   "Eintrag anschauen"
         Height          =   375
         Left            =   1740
         TabIndex        =   112
         Top             =   5160
         Width           =   1575
      End
      Begin ComctlLib.ListView picimgview 
         Height          =   4455
         Left            =   180
         TabIndex        =   111
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
         TabIndex        =   100
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
         Caption         =   "Bearbei&ten"
         Height          =   375
         Left            =   -69420
         TabIndex        =   103
         ToolTipText     =   "Zeile bearbeiten"
         Top             =   1740
         Width           =   1155
      End
      Begin VB.CommandButton cmdDelSyncLyric 
         Caption         =   "&Löschen"
         Height          =   375
         Left            =   -69420
         TabIndex        =   102
         ToolTipText     =   "Zeile löschen"
         Top             =   1320
         Width           =   1155
      End
      Begin VB.CommandButton cmdAddSyncLyric 
         Caption         =   "&Hinzufügen"
         Height          =   375
         Left            =   -69420
         TabIndex        =   101
         ToolTipText     =   "Zeile hinzufügen"
         Top             =   900
         Width           =   1155
      End
      Begin VB.Frame Frame2 
         Caption         =   "Einträge "
         Height          =   2355
         Left            =   -74700
         TabIndex        =   98
         Top             =   2280
         Width           =   6615
         Begin VB.ListBox listSYLT 
            Height          =   1425
            Left            =   120
            TabIndex        =   110
            Top             =   780
            Width           =   5055
         End
         Begin VB.TextBox txtLanguage5 
            Height          =   315
            Left            =   780
            MaxLength       =   3
            TabIndex        =   106
            Top             =   300
            Width           =   645
         End
         Begin VB.ComboBox comboTimes 
            Height          =   315
            ItemData        =   "frmID3V2.frx":00C4
            Left            =   2160
            List            =   "frmID3V2.frx":00CE
            Style           =   2  'Dropdown-Liste
            TabIndex        =   105
            Top             =   300
            Width           =   1395
         End
         Begin VB.ComboBox comboContentType 
            Height          =   315
            ItemData        =   "frmID3V2.frx":00E9
            Left            =   4560
            List            =   "frmID3V2.frx":00EB
            Style           =   2  'Dropdown-Liste
            TabIndex        =   104
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
            TabIndex        =   99
            Top             =   780
            Width           =   1275
            Begin VB.CommandButton cmdAddSyncEntry 
               Caption         =   "&Hinzufügen"
               Height          =   375
               Left            =   60
               TabIndex        =   4
               ToolTipText     =   "Zeile hinzufügen"
               Top             =   60
               Width           =   1155
            End
            Begin VB.CommandButton cmdDelSyncEntry 
               Caption         =   "&Löschen"
               Height          =   375
               Left            =   60
               TabIndex        =   5
               ToolTipText     =   "Zeile löschen"
               Top             =   480
               Width           =   1155
            End
            Begin VB.CommandButton cmdEditSyncEntry 
               Caption         =   "Bearbei&ten"
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
            Caption         =   "Sprache:"
            ForeColor       =   &H80000008&
            Height          =   195
            Index           =   34
            Left            =   60
            TabIndex        =   109
            Top             =   360
            Width           =   645
         End
         Begin VB.Label lbl 
            Caption         =   "Zeit in:"
            ForeColor       =   &H80000008&
            Height          =   195
            Index           =   37
            Left            =   1620
            TabIndex        =   108
            Top             =   360
            Width           =   495
         End
         Begin VB.Label lbl 
            Caption         =   "Inhaltstyp:"
            ForeColor       =   &H80000008&
            Height          =   195
            Index           =   38
            Left            =   3780
            TabIndex        =   107
            Top             =   360
            Width           =   735
         End
      End
      Begin VB.TextBox txtTDRC 
         Height          =   315
         Left            =   -71520
         TabIndex        =   48
         Top             =   600
         Width           =   1155
      End
      Begin VB.TextBox txtTIT1 
         Height          =   315
         Left            =   -73560
         TabIndex        =   59
         Top             =   3840
         Width           =   5600
      End
      Begin VB.TextBox txtTLAN 
         Height          =   315
         Left            =   -73560
         TabIndex        =   58
         Top             =   3480
         Width           =   5600
      End
      Begin VB.TextBox txtTENC 
         Height          =   315
         Left            =   -73560
         TabIndex        =   57
         Top             =   3120
         Width           =   5600
      End
      Begin VB.TextBox txtTCOP 
         Height          =   315
         Left            =   -73560
         TabIndex        =   56
         Top             =   2760
         Width           =   5600
      End
      Begin VB.TextBox txtTCOM 
         Height          =   315
         Left            =   -73560
         TabIndex        =   55
         Top             =   2400
         Width           =   5600
      End
      Begin VB.TextBox txtTCON 
         Height          =   315
         Left            =   -71520
         TabIndex        =   54
         Top             =   2040
         Width           =   3555
      End
      Begin VB.TextBox txtTYER 
         Height          =   315
         Left            =   -73560
         TabIndex        =   53
         Top             =   2040
         Width           =   735
      End
      Begin VB.TextBox txtTALB 
         Height          =   315
         Left            =   -73560
         TabIndex        =   52
         Top             =   1680
         Width           =   5600
      End
      Begin VB.TextBox txtTPE1 
         Height          =   315
         Left            =   -73560
         TabIndex        =   51
         Top             =   1320
         Width           =   5600
      End
      Begin VB.TextBox txtTIT2 
         Height          =   315
         Left            =   -73560
         TabIndex        =   50
         Top             =   960
         Width           =   5600
      End
      Begin VB.TextBox txtTRCK 
         Height          =   315
         Left            =   -73560
         TabIndex        =   47
         Top             =   600
         Width           =   675
      End
      Begin VB.TextBox txtWCOM 
         Height          =   315
         Left            =   -72120
         TabIndex        =   39
         Top             =   540
         Width           =   4000
      End
      Begin VB.TextBox txtWCOP 
         Height          =   315
         Left            =   -72120
         TabIndex        =   40
         Top             =   900
         Width           =   4000
      End
      Begin VB.TextBox txtWOAF 
         Height          =   315
         Left            =   -72120
         TabIndex        =   41
         Top             =   1260
         Width           =   4000
      End
      Begin VB.TextBox txtWOAR 
         Height          =   315
         Left            =   -72120
         TabIndex        =   42
         Top             =   1620
         Width           =   4000
      End
      Begin VB.TextBox txtWOAS 
         Height          =   315
         Left            =   -72120
         TabIndex        =   43
         Top             =   1980
         Width           =   4000
      End
      Begin VB.TextBox txtWORS 
         Height          =   315
         Left            =   -72120
         TabIndex        =   44
         Top             =   2340
         Width           =   4000
      End
      Begin VB.TextBox txtWPAY 
         Height          =   315
         Left            =   -72120
         TabIndex        =   45
         Top             =   2700
         Width           =   4000
      End
      Begin VB.TextBox txtWPUB 
         Height          =   315
         Left            =   -72120
         TabIndex        =   46
         Top             =   3060
         Width           =   4000
      End
      Begin VB.TextBox txtUSLT 
         Height          =   2835
         Left            =   -74700
         MultiLine       =   -1  'True
         ScrollBars      =   3  'Beides
         TabIndex        =   38
         Top             =   1740
         Width           =   6525
      End
      Begin VB.TextBox txtCOMM 
         Height          =   2835
         Left            =   -74700
         MultiLine       =   -1  'True
         ScrollBars      =   3  'Beides
         TabIndex        =   34
         Top             =   1740
         Width           =   6525
      End
      Begin VB.TextBox txtLanguage1 
         Height          =   315
         Left            =   -73860
         TabIndex        =   32
         Top             =   1020
         Width           =   645
      End
      Begin VB.TextBox txtComment1 
         Height          =   315
         Left            =   -73080
         TabIndex        =   33
         Top             =   1020
         Width           =   4905
      End
      Begin VB.CommandButton cmdDelComment 
         Caption         =   "aktuellen Kommentar entfernen"
         Height          =   435
         Left            =   -73260
         TabIndex        =   36
         Top             =   4800
         Width           =   2475
      End
      Begin VB.CommandButton cmdDelAllComments 
         Caption         =   "Alle Kommentare entfernen"
         Height          =   435
         Left            =   -70620
         TabIndex        =   37
         Top             =   4800
         Width           =   2475
      End
      Begin VB.ComboBox lfdNr1 
         Height          =   315
         Left            =   -74700
         TabIndex        =   31
         Text            =   "Combo1"
         Top             =   1020
         Width           =   735
      End
      Begin VB.ComboBox lfdNr2 
         Height          =   315
         Left            =   -74700
         TabIndex        =   30
         Text            =   "Combo1"
         Top             =   1020
         Width           =   735
      End
      Begin VB.TextBox txtComment2 
         Height          =   315
         Left            =   -73080
         TabIndex        =   29
         Top             =   1020
         Width           =   4905
      End
      Begin VB.TextBox txtLanguage2 
         Height          =   315
         Left            =   -73860
         TabIndex        =   28
         Top             =   1020
         Width           =   645
      End
      Begin VB.CommandButton cmdDelAllLyrics 
         Caption         =   "Alle Songtexte entfernen"
         Height          =   435
         Left            =   -70620
         TabIndex        =   27
         Top             =   4800
         Width           =   2475
      End
      Begin VB.CommandButton cmdDelLyric 
         Caption         =   "aktuellen Songtext entfernen"
         Height          =   435
         Left            =   -73260
         TabIndex        =   26
         Top             =   4800
         Width           =   2475
      End
      Begin VB.CommandButton cmdOKComm 
         Caption         =   "Übernehmen"
         Height          =   435
         Left            =   -74700
         TabIndex        =   35
         Top             =   4800
         Width           =   1275
      End
      Begin VB.CommandButton cmdOKLyric 
         Caption         =   "Übernehmen"
         Height          =   435
         Left            =   -74700
         TabIndex        =   25
         Top             =   4800
         Width           =   1275
      End
      Begin VB.Frame Frame1 
         Caption         =   "Benutzerdefinierte URL's:"
         ForeColor       =   &H80000008&
         Height          =   1935
         Left            =   -74700
         TabIndex        =   16
         Top             =   3480
         Width           =   6735
         Begin VB.TextBox txtWXXX 
            Height          =   315
            Left            =   1200
            TabIndex        =   22
            Top             =   780
            Width           =   5385
         End
         Begin VB.ComboBox lfdNr3 
            Height          =   315
            ItemData        =   "frmID3V2.frx":00ED
            Left            =   1200
            List            =   "frmID3V2.frx":00EF
            TabIndex        =   21
            Text            =   "Combo1"
            Top             =   360
            Width           =   5415
         End
         Begin VB.PictureBox Picture1 
            BorderStyle     =   0  'Kein
            Height          =   675
            Left            =   60
            ScaleHeight     =   675
            ScaleWidth      =   6495
            TabIndex        =   17
            Top             =   1200
            Width           =   6495
            Begin VB.CommandButton cmdOKURL 
               Caption         =   "Übernehmen"
               Height          =   435
               Left            =   120
               TabIndex        =   20
               Top             =   60
               Width           =   1275
            End
            Begin VB.CommandButton cmdDelURL 
               Caption         =   "aktuelle URL entfernen"
               Height          =   435
               Left            =   1740
               TabIndex        =   19
               Top             =   60
               Width           =   2115
            End
            Begin VB.CommandButton cmdDelAllURLs 
               Caption         =   "Alle URLs entfernen"
               Height          =   435
               Left            =   4200
               TabIndex        =   18
               Top             =   60
               Width           =   2115
            End
         End
         Begin VB.Label lbl 
            Alignment       =   1  'Rechts
            AutoSize        =   -1  'True
            Caption         =   "URL:"
            ForeColor       =   &H80000008&
            Height          =   195
            Index           =   22
            Left            =   720
            TabIndex        =   24
            Top             =   840
            Width           =   375
         End
         Begin VB.Label lbl 
            AutoSize        =   -1  'True
            Caption         =   "Beschreibung:"
            ForeColor       =   &H80000008&
            Height          =   195
            Index           =   27
            Left            =   120
            TabIndex        =   23
            Top             =   420
            Width           =   1020
         End
      End
      Begin VB.TextBox txtUSER 
         Height          =   2835
         Left            =   -74700
         MultiLine       =   -1  'True
         ScrollBars      =   3  'Beides
         TabIndex        =   15
         Top             =   1740
         Width           =   6525
      End
      Begin VB.CommandButton cmdOKFrame 
         Caption         =   "Übernehmen"
         Height          =   435
         Left            =   -74700
         TabIndex        =   14
         Top             =   4800
         Width           =   1275
      End
      Begin VB.CommandButton cmdDelAllFrames 
         Caption         =   "Alle Frames entfernen"
         Height          =   435
         Left            =   -70620
         TabIndex        =   13
         Top             =   4800
         Width           =   2475
      End
      Begin VB.CommandButton cmdDelFrame 
         Caption         =   "aktuellen Frame entfernen"
         Height          =   435
         Left            =   -73260
         TabIndex        =   12
         Top             =   4800
         Width           =   2475
      End
      Begin VB.TextBox txtTPE2 
         Height          =   315
         Left            =   -73560
         TabIndex        =   60
         Top             =   4200
         Width           =   5600
      End
      Begin VB.TextBox txtTSSE 
         Height          =   315
         Left            =   -73560
         TabIndex        =   61
         Top             =   4560
         Width           =   5600
      End
      Begin VB.TextBox txtTBPM 
         Height          =   315
         Left            =   -73560
         TabIndex        =   62
         Top             =   4920
         Width           =   5600
      End
      Begin VB.TextBox txtTOWN 
         Height          =   315
         Left            =   -73560
         TabIndex        =   63
         Top             =   5280
         Width           =   5600
      End
      Begin VB.CommandButton cmdDelAllPics 
         Caption         =   "Alle Bilder entfernen"
         Height          =   375
         Left            =   5340
         TabIndex        =   11
         Top             =   5160
         Width           =   1695
      End
      Begin VB.CommandButton cmdNewPic 
         Caption         =   "Bild hinzufügen"
         Height          =   375
         Left            =   240
         TabIndex        =   10
         Top             =   5160
         Width           =   1395
      End
      Begin VB.CommandButton cmdDelPic 
         Caption         =   "Bild entfernen"
         Height          =   375
         Left            =   3840
         TabIndex        =   9
         Top             =   5160
         Width           =   1395
      End
      Begin VB.CommandButton cmdOkSyncLyric 
         Caption         =   "Einträge übernehmen"
         Height          =   435
         Left            =   -73755
         TabIndex        =   7
         Top             =   4800
         Width           =   2475
      End
      Begin VB.CommandButton cmdDelAllSyncLyric 
         Caption         =   "Alle Songtexte entfernen"
         Height          =   435
         Left            =   -71175
         TabIndex        =   8
         Top             =   4800
         Width           =   2475
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "PlayCounter:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   35
         Left            =   -70200
         TabIndex        =   113
         Top             =   660
         Width           =   885
      End
      Begin VB.Image noPicture 
         Height          =   1200
         Left            =   3480
         Picture         =   "frmID3V2.frx":00F1
         Top             =   5280
         Visible         =   0   'False
         Width           =   1440
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Aufnahmedatum:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   15
         Left            =   -72780
         TabIndex        =   97
         Top             =   660
         Width           =   1245
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Kategorien:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   13
         Left            =   -74700
         TabIndex        =   96
         Top             =   3900
         Width           =   1095
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Sprache:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   12
         Left            =   -74730
         TabIndex        =   95
         Top             =   3540
         Width           =   1095
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Genre:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   11
         Left            =   -72060
         TabIndex        =   94
         Top             =   2100
         Width           =   465
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Track Nr:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   10
         Left            =   -74730
         TabIndex        =   93
         Top             =   660
         Width           =   1095
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Encoder:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   9
         Left            =   -74730
         TabIndex        =   92
         Top             =   3180
         Width           =   1095
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Copyright:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   7
         Left            =   -74730
         TabIndex        =   91
         Top             =   2820
         Width           =   1095
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Komponist:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   5
         Left            =   -74730
         TabIndex        =   90
         Top             =   2460
         Width           =   1095
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Jahr:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   3
         Left            =   -74730
         TabIndex        =   89
         Top             =   2100
         Width           =   1095
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Album:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   2
         Left            =   -74730
         TabIndex        =   88
         Top             =   1740
         Width           =   1095
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Artist:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   1
         Left            =   -74730
         TabIndex        =   87
         Top             =   1380
         Width           =   1095
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Titel:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   0
         Left            =   -74730
         TabIndex        =   86
         Top             =   1020
         Width           =   1095
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Commercial Information URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   6
         Left            =   -74820
         TabIndex        =   85
         Top             =   600
         Width           =   2595
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Copyright Information URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   14
         Left            =   -74805
         TabIndex        =   84
         Top             =   960
         Width           =   2595
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Official Audiofile Webpage URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   16
         Left            =   -74805
         TabIndex        =   83
         Top             =   1320
         Width           =   2595
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Official Artist Webpage URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   17
         Left            =   -74805
         TabIndex        =   82
         Top             =   1680
         Width           =   2595
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Official Audiosource Webpage URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   18
         Left            =   -74820
         TabIndex        =   81
         Top             =   2040
         Width           =   2595
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Official Internet radiostation URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   19
         Left            =   -74820
         TabIndex        =   80
         Top             =   2400
         Width           =   2595
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Payment URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   20
         Left            =   -74805
         TabIndex        =   79
         Top             =   2760
         Width           =   2595
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Publishers official Webpage URL:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   21
         Left            =   -74805
         TabIndex        =   78
         Top             =   3120
         Width           =   2595
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Song-Text:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   8
         Left            =   -74700
         TabIndex        =   77
         Top             =   1500
         Width           =   780
      End
      Begin VB.Label lbl 
         Caption         =   "Kommentar:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   4
         Left            =   -74700
         TabIndex        =   76
         Top             =   1500
         Width           =   915
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         Caption         =   "Sprache:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   23
         Left            =   -73860
         TabIndex        =   75
         Top             =   780
         Width           =   645
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         Caption         =   "Beschreibung:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   24
         Left            =   -73080
         TabIndex        =   74
         Top             =   780
         Width           =   1020
      End
      Begin VB.Label lblItem 
         AutoSize        =   -1  'True
         Caption         =   "Eintrag Nr:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   25
         Left            =   -74700
         TabIndex        =   73
         Top             =   780
         Width           =   750
      End
      Begin VB.Label lblItem 
         AutoSize        =   -1  'True
         Caption         =   "Eintrag Nr:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   0
         Left            =   -74700
         TabIndex        =   72
         Top             =   780
         Width           =   750
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         Caption         =   "Beschreibung:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   25
         Left            =   -73080
         TabIndex        =   71
         Top             =   780
         Width           =   1020
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         Caption         =   "Sprache:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   26
         Left            =   -73860
         TabIndex        =   70
         Top             =   780
         Width           =   645
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         AutoSize        =   -1  'True
         Caption         =   " Text:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   29
         Left            =   -74745
         TabIndex        =   69
         Top             =   1500
         Width           =   405
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         Caption         =   "Sprache:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   28
         Left            =   -74640
         TabIndex        =   68
         Top             =   780
         Width           =   645
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Band/Orchester:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   30
         Left            =   -74880
         TabIndex        =   67
         Top             =   4260
         Width           =   1275
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Encoder Settings:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   31
         Left            =   -74880
         TabIndex        =   66
         Top             =   4620
         Width           =   1275
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Beats pro Minute:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   32
         Left            =   -74880
         TabIndex        =   65
         Top             =   4980
         Width           =   1275
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Rechts
         Caption         =   "Dateiinhaber:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   33
         Left            =   -74700
         TabIndex        =   64
         Top             =   5340
         Width           =   1095
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
Dim ID%, w%, Bilddatei$, tmp$
Dim syncItem As ListItem, picItem As ListItem

Private Sub cmdAddSyncEntry_Click()
 frmSYLT.Show 1
 If frmSYLT.chk.value = 1 Then
   listSYLT.AddItem "[" & format(Val(frmSYLT.txtTime.text), "00000000") & "]" & Trim$(frmSYLT.txtEntry.text)
 End If
End Sub

Private Sub cmdAddSyncLyric_Click()
  Set syncItem = viewSYLTDescr.ListItems.Add(, , "<Beschreibung hier eingeben>", 0, 0)
  Set viewSYLTDescr.SelectedItem = syncItem
  viewSYLTDescr_ItemClick syncItem
  viewSYLTDescr.SetFocus
  viewSYLTDescr.StartLabelEdit
End Sub

Private Sub cmdCancel_Click()
  Hide
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

Private Sub cmdDelAllComments_Click()
  genie.ID3V2DeleteComments
  updateComments
End Sub

Private Sub cmdDelComment_Click()
  Call genie.ID3V2DeleteSelectedFrame(ID3F_COMM, lfdNr1.ListIndex + 1)
  updateComments
End Sub

Private Sub cmdDelLyric_Click()
  Call genie.ID3V2DeleteSelectedFrame(ID3F_USLT, lfdNr2.ListIndex + 1)
  updateLyrics
End Sub

Private Sub cmdDelPic_Click()
  Set picItem = picimgview.SelectedItem
  If picItem Is Nothing Then Exit Sub
  ID% = Val(picItem.SubItems(3))
  genie.ID3V2DeletePicture ID%
  picimgview.ListItems.Remove picItem.Index
End Sub

Private Sub cmdDelSyncEntry_Click()
  If listSYLT.ListIndex = -1 Then Exit Sub
  listSYLT.RemoveItem listSYLT.ListIndex
End Sub

Private Sub cmdDelSyncLyric_Click()
  Set syncItem = viewSYLTDescr.SelectedItem
  If syncItem Is Nothing Then Exit Sub
  ID% = syncItem.Index
  viewSYLTDescr.ListItems.Remove ID%
  Call genie.ID3V2AddSyncLyric("DEU", "", "", 0, 1)
  updateSyncLyrics
End Sub

Private Sub cmdDelURL_Click()
  Call genie.ID3V2DeleteSelectedFrame(ID3F_WXXX, lfdNr3.ListIndex + 1)
  updateURLs
End Sub

Private Sub cmdDelFrame_Click()
  Call genie.ID3V2DeleteSelectedFrame(ID3F_USER, lfdNr4.ListIndex + 1)
  updateFrames
End Sub

Private Sub cmdEditSyncEntry_Click()
  Dim entry As String
  If listSYLT.ListIndex = -1 Then Exit Sub
  entry = listSYLT.List(listSYLT.ListIndex)
  frmSYLT.txtTime = Val(Mid$(entry, 2, 8))
  frmSYLT.txtEntry = Mid$(entry, 11)
  frmSYLT.Show 1
  If frmSYLT.chk.value = 1 Then
    entry = "[" & format(Val(frmSYLT.txtTime.text), "00000000") & "]" & Trim$(frmSYLT.txtEntry.text)
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
  ' Bild hinzufügen
  On Error GoTo ErrHandler
  ' Dialogfeld "Öffnen" anzeigen
  frmID3V2.StandardDlg.ShowOpen
  Bilddatei$ = frmID3V2.StandardDlg.FileName
  On Error GoTo WrongPicture
  Set frmAPIC.img.Picture = LoadPicture(Bilddatei$)
  frmAPIC.idInfo.Caption = genie.ID3V2GetPictures() + 1
  frmAPIC.dateiInfo.Caption = Bilddatei$
  frmAPIC.linkCheck.value = 0
  frmAPIC.picDescription.Enabled = True
  frmAPIC.picTypeCombo.Enabled = True
  frmAPIC.linkCheck.Enabled = True
  frmAPIC.cmdOk.Enabled = True
  frmAPIC.picMime = ""
  frmAPIC.Show 1
  Exit Sub
ErrHandler:
  Debug.Print Err.Description
  ' Benutzer hat Schaltfläche "Abbrechen" gewählt
  Exit Sub
WrongPicture:
  MsgBox "Das Bild konnte nicht geladen werden oder ist ungültig", vbCritical
  Exit Sub
End Sub

Private Sub cmdOKComm_Click()
  ID% = Val(lfdNr1.text)
  Call genie.ID3V2AddComment(txtLanguage1, txtComment1, txtCOMM)
  updateComments
End Sub

Private Sub cmdOKLyric_Click()
  ID% = Val(lfdNr2.text)
  Call genie.ID3V2AddLyric(txtLanguage2, txtComment2, txtUSLT.text)
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
  If genie.ID3V2AddSyncLyric(txtLanguage5.text, syncItem.text, tmp, comboContentType.ListIndex, comboTimes.ListIndex + 1) = False Then
    MsgBox "Beim Eintragen ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Eintragen des SyncLyric-Tag"
  End If
  '  updateSyncLyrics
End Sub

Private Sub cmdOKURL_Click()
  Call genie.ID3V2AddUserURL(lfdNr3.text, txtWXXX.text)
  updateURLs
End Sub

Private Sub cmdOKFrame_Click()
  Call genie.ID3V2AddUserFrame(lfdNr4.text, txtUSER.text)
  updateFrames
End Sub

Private Sub cmdParam_Click()
  frmID3Settings.Show 1
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
  ' Tag speichern
  If genie.ID3V2SaveChangesToFile(Datei) = False Then
    MsgBox "Beim Speichern ist ein Fehler aufgetreten:" & vbCrLf & genie.AUDIOGetLastErrorText, vbCritical, "Fehler beim Speichern des ID3v2-Tag"
  End If
End Sub

Private Sub cmdDelete_Click()
  DeleteID3v2Tag
  updateUI
End Sub

Public Sub updateUI()
  Dim ID%, Language$, Description$, Txt$
  ' ID3v2-Tag-Daten in Formular übertragen
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
    
  ' falls alte Version, nicht unterstützte Tags deaktivieren
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
  ID% = Val(lfdNr1.List(lfdNr1.ListIndex))
  txtLanguage1 = genie.ID3V2GetCommentLanguage(ID%)
  txtComment1 = genie.ID3V2GetCommentDescription(ID%)
  txtCOMM = genie.ID3V2GetComment(ID%)
End Sub

Private Sub lfdNr2_Click()
  ID% = Val(lfdNr2.List(lfdNr2.ListIndex))
  txtLanguage2 = genie.ID3V2GetLyricLanguage(ID%)
  txtComment2 = genie.ID3V2GetLyricDescription(ID%)
  txtUSLT = genie.ID3V2GetLyric(ID%)
End Sub

Private Sub lfdNr3_Click()
  txtWXXX = genie.ID3V2GetUserURL(lfdNr3.ListIndex + 1)
End Sub

Private Sub lfdNr4_Click()
  txtUSER = genie.ID3V2GetUserFrame(lfdNr4.ListIndex + 1)
End Sub

Public Sub updateComments()
  ' Liste alle Kommentar-Tags auf
  txtCOMM = ""
  txtLanguage1 = ""
  txtComment1 = ""
  lfdNr1.Clear
  For ID% = 1 To genie.ID3V2GetComments()
    lfdNr1.AddItem ID%
  Next ID%
  If lfdNr1.ListCount > 0 Then lfdNr1.ListIndex = 0
End Sub

Public Sub updateLyrics()
  ' Liste alle Lyrics-Tags auf
  txtUSLT = ""
  txtLanguage2 = ""
  txtComment2 = ""
  lfdNr2.Clear
  For ID% = 1 To genie.ID3V2GetLyrics()
    lfdNr2.AddItem ID%
  Next ID%
  If lfdNr2.ListCount > 0 Then lfdNr2.ListIndex = 0
End Sub

Public Sub updateSyncLyrics()
  ' Liste alle SyncLyrics-Tags auf
  viewSYLTDescr.ListItems.Clear
  txtLanguage5 = ""
  listSYLT.Clear
  For ID% = 1 To genie.ID3V2GetSyncLyrics()
    viewSYLTDescr.ListItems.Add ID%, , genie.ID3V2GetSyncLyricDescription(ID%), 0, 0
  Next ID%
  If genie.ID3V2GetLyrics() > 0 Then
    Set viewSYLTDescr.SelectedItem = viewSYLTDescr.ListItems(1)
    viewSYLTDescr_ItemClick viewSYLTDescr.ListItems(1)
    Frame2.Visible = True
  Else
    Frame2.Visible = False
  End If
End Sub

Public Sub updateURLs()
  ' Liste alle URL-Tags auf
  txtWXXX = ""
  lfdNr3.Clear
  For ID% = 1 To genie.ID3V2GetUserURLs()
    lfdNr3.AddItem genie.ID3V2GetUserURLDescription(ID%)
  Next ID%
  If lfdNr3.ListCount > 0 Then lfdNr3.ListIndex = 0
End Sub

Public Sub updateFrames()
  ' Liste alle URL-Tags auf
  txtUSER = ""
  lfdNr4.Clear
  For ID% = 1 To genie.ID3V2GetUserFrames()
    lfdNr4.AddItem genie.ID3V2GetUserFrameLanguage(ID%)
  Next ID%
  If lfdNr4.ListCount > 0 Then lfdNr4.ListIndex = 0
End Sub

Private Sub updateSyncEntrys()
  Dim time_id As Integer, content_id As Integer, arr() As String
  listSYLT.Clear
  ' hole aktives Element
  Set syncItem = viewSYLTDescr.SelectedItem
  If syncItem Is Nothing Then Exit Sub
  ID% = syncItem.Index
  txtLanguage5.text = genie.ID3V2GetSyncLyricLanguage(ID%)
  If txtLanguage5.text = "" Then txtLanguage5.text = "deu"
  time_id = genie.ID3V2GetSyncLyricTimeFormat(ID%)
  If time_id = -1 Then time_id = 2
  content_id = genie.ID3V2GetSyncLyricContentType(ID%)
  If content_id = -1 Then content_id = 0
  comboContentType.ListIndex = content_id
  comboTimes.ListIndex = time_id - 1
  ' überführe String in Listeneinträge
  tmp = genie.ID3V2GetSyncLyric(ID%)
  arr = Split(tmp, vbCrLf, -1)
  For w% = 0 To UBound(arr)
    listSYLT.AddItem arr(w%)
  Next w%
  Frame2.Visible = True
End Sub

Public Sub updatePictures()
  picimgview.ListItems.Clear
  picimgview.Icons = Nothing
  picimglist.ListImages.Clear
  picimglist.ImageHeight = 96
  picimglist.ImageWidth = 96
  picimglist.UseMaskColor = False
  picimglist.BackColor = vbBlue
  If genie.ID3V2GetPictures() = 0 Then Exit Sub
  
  For ID% = 1 To genie.ID3V2GetPictures()
    If ID3V2SetPictureWithArray(ID%, frmAPIC.img) = False Then
       Set frmAPIC.img.Picture = noPicture.Picture
    End If
    picimglist.ListImages.Add , , frmAPIC.img
    Set picimgview.Icons = picimglist
    Set picItem = picimgview.ListItems.Add(, , genie.ID3V2GetPictureDescription(ID%), picimglist.ListImages.count, 0)
    picItem.SubItems(1) = genie.ID3V2GetPictureType(ID%)
    picItem.SubItems(2) = genie.ID3V2GetPictureMime(ID%)
    picItem.SubItems(3) = ID%
  Next ID%
  picimgview.Icons = picimglist
End Sub

Private Sub picimgview_ItemClick(ByVal item As ComctlLib.ListItem)
  Set frmAPIC.img.Picture = picimglist.ListImages(item.Index).Picture
  frmAPIC.idInfo = item.SubItems(3)
  frmAPIC.picDescription.text = item.text
  frmAPIC.picTypeCombo.ListIndex = item.SubItems(1)
  frmAPIC.picMime.text = item.SubItems(2)
  If Left$(frmAPIC.picMime.text, 3) = "-->" Then
    frmAPIC.linkCheck.value = 1
  Else
    frmAPIC.linkCheck.value = 0
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
  
  ' Wahlweise folgender Aufruf ohne SAFEARRAY
  'Result = genie.ID3V2CopyPictureArray(Index)
  
  If UBound(Result) = 0 Then
    Set img.Picture = noPicture.Picture
  Else
    Call CreateStreamOnHGlobal(Result(0), False, oStream)
    Call OleLoadPicture(oStream, 0, 0, IID_IPicture(0), oPicture)
    Set img.Picture = oPicture
  End If
  ID3V2SetPictureWithArray = True
  Exit Function
BildFehler:
  ID3V2SetPictureWithArray = False
  On Error GoTo 0
End Function

