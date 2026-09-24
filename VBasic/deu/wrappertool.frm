VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   7980
   ClientLeft      =   3405
   ClientTop       =   3570
   ClientWidth     =   12300
   LinkTopic       =   "Form1"
   ScaleHeight     =   7980
   ScaleWidth      =   12300
   Begin RichTextLib.RichTextBox txt 
      Height          =   6855
      Left            =   0
      TabIndex        =   1
      Top             =   960
      Width           =   12135
      _ExtentX        =   21405
      _ExtentY        =   12091
      _Version        =   393217
      ScrollBars      =   3
      OLEDragMode     =   0
      OLEDropMode     =   0
      TextRTF         =   $"wrappertool.frx":0000
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Courier New"
         Size            =   9
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Los geht's"
      Height          =   495
      Left            =   4920
      TabIndex        =   0
      Top             =   240
      Width           =   2955
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Type param
  param As String
  name As String
End Type

Private Type struktur
  rueckgabe As String
  bezeichnung As String
  params() As param
  isProperty As Boolean
End Type

Dim result() As struktur
Dim total$

Private Sub Command1_Click()
  ' Call existMethod
  'Exit Sub
  Dim tmp As struktur
  ReDim result(1000)
  Dim hasComment As Boolean
  Open "E:\AudioGenie3\AudioGenie3\dllmain.cpp" For Input As #1
  total$ = ""
  txt.Text = ""
  txt.Visible = False
  i% = 0
  Do While Not EOF(1)
    Line Input #1, a$
    If Left$(a$, 8) = "extern " & Chr$(34) Then
      tmp = Auswertung(Mid$(a$, 11))
      If Left(tmp.bezeichnung, 10) = "WMASetYear" Then
        Debug.Print tmp.bezeichnung
      End If
      If tmp.bezeichnung <> "WINAPI" And hasComment = False Then
         result(i%) = tmp
         i% = i% + 1
     End If
    End If
  Loop
  Close #1
  ReDim Preserve result(0 To i% - 1)
  Debug.Print "Anzahl2:" & i%
  Call QuickSort(LBound(result), UBound(result))
  Call generiereCSDeclares(result)
  'Call generiereVBProperties(result)
  'Call generiereVBMethods(result)
  txt.Text = total$
  txt.Visible = True
End Sub


Private Sub existMethod()
  Open "E:\AudioGenietest\AudioGenietest\AudioGenie2.cs" For Input As #1
  Dim methods() As String
  Dim i%, rows%
  ReDim methods(20000) As String
  Do While Not EOF(1)
    Line Input #1, a$
    a$ = UCase$(Trim$(a$))
    If Left$(a$, 21) <> "PRIVATE STATIC EXTERN" And a$ <> "" Then
      methods(rows%) = a$
      rows% = rows% + 1
    End If
  Loop
  Close #1
  ReDim Preserve methods(0 To rows%)
  Open "E:\AudioGenie3\AudioGenie3\AudioGenie3.def" For Input As #2
  For i% = 1 To 10
    Line Input #2, a$
  Next i%
  Do While Not EOF(2)
    Line Input #2, a$
    'Debug.Print a$
    a$ = Trim$(UCase$(a$))
    For i% = 0 To rows%
      If InStr(methods(i%), a$) > 0 Then i% = 20000
    Next i%
    If i% < 20000 Then Debug.Print a$ & " not found"
  Loop
  Close #2
  Debug.Print "Fertig"
End Sub
Private Function Auswertung(a$) As struktur
  st% = 1
  anz% = 0
  B$ = a$ + " "
  Dim ss$(30)
  For w% = 1 To Len(B$)
    ch = Asc(Mid$(B$, w%, 1))
    If ch = Asc(" ") Or ch = Asc("(") Or Asc(")") = ch Or Asc(",") = ch Then
      s$ = Trim$(Mid$(B$, st%, w% - st%))
      st% = w% + 1
      If Len(s$) > 0 And s$ <> "__stdcall" Then
        anz% = anz% + 1
        ss$(anz%) = s$
      End If
    End If
  Next w%
  Dim strkt As struktur
  strkt.rueckgabe = ss$(1)
  strkt.bezeichnung = ss$(2)
  strkt.isProperty = False
  Dim params() As param
  If anz% > 2 Then
    ReDim params(1 To (anz% - 2) / 2) As param
    For i% = 3 To anz% Step 2
      params((i% - 1) / 2).param = ss$(i%)
      params((i% - 1) / 2).name = ss$(i% + 1)
    Next i%
  Else
   ReDim params(0)
  End If
  strkt.params = params
  Auswertung = strkt
End Function

Private Sub generiereVBDeclares(arr() As struktur)
  Dim params() As param
  txt.Text = ""
  For w% = 0 To UBound(arr)
    z$ = "Private Declare "
    bez = arr(w%).bezeichnung
    params = arr(w%).params
    If arr(w%).rueckgabe <> "void" Then
      z$ = z$ + "Function "
    Else
      z$ = z$ + "Sub "
    End If
    z$ = z$ & bez & " Lib " & Chr$(34) & "AudioGenie3.dll" & Chr$(34) & " ("
    For i% = 1 To UBound(params)
      If i% > 1 Then z$ = z$ & ", "
      p$ = params(i%).name
      If Left(p$, 2) = "**" Then
        z$ = z$ & Mid$(p$, 3) & "() As " & cc(params(i%).param)
      ElseIf Left(p$, 1) = "*" Then
        z$ = z$ & "ByVal " & Mid$(p$, 2) & " As Long"
      Else
        z$ = z$ & "ByVal " & p$ & " As " & cc(params(i%).param)
      End If
    Next i%
    z$ = z$ & ")"
    If arr(w%).rueckgabe <> "void" Then
      z$ = z$ & " As " & cc(arr(w%).rueckgabe)
    End If
    add z$
  Next w%
End Sub

Private Sub generiereCSDeclares(arr() As struktur)
  ' [DllImport("AudioGenie2.dll", CharSet = CharSet.Auto, SetLastError = true, ExactSpelling = true)]
  '      private static extern void DisablePopupKeyW(string keyValue);
  Dim params() As param
  txt.Text = ""
  For w% = 0 To UBound(arr)
    add "  [DllImport(" & Chr$(34) & "AudioGenie3.dll" & Chr$(34) & ", CharSet = CharSet.Unicode, SetLastError = true, ExactSpelling = true)]"
    z$ = "private static extern " & cs(arr(w%).rueckgabe) & " " & arr(w%).bezeichnung & "("
    params = arr(w%).params
    For i% = 1 To UBound(params)
      If i% > 1 Then z$ = z$ & ", "
      p$ = params(i%).name
      If Left(p$, 2) = "**" Then
        z$ = z$ & Mid$(p$, 3) & "() As " & cs(params(i%).param)
      ElseIf Left(p$, 1) = "*" Then
        z$ = z$ & "byte[] " & Mid$(p$, 2)
      Else
        z$ = z$ & cs(params(i%).param) & " " & p$
      End If
    Next i%
    z$ = z$ & ");"
    add z$
  Next w%
End Sub

Private Sub generiereVBProperties(arr() As struktur)
  For w% = 0 To UBound(arr) - 1
    If InStr(arr(w%).bezeichnung, "Get") > 0 Then
      tmp$ = Replace(arr(w%).bezeichnung, "Get", "Set")
      For x% = 0 To UBound(arr)
        If arr(x%).bezeichnung = tmp$ And UBound(arr(x%).params()) = 1 And UBound(arr(w%).params()) = 0 Then
          'Call writeCSProperty(arr(w%), arr(x%))
          Call writeVBPropGet(arr(w%))
          Call writeVBPropLet(arr(x%))
          arr(w%).isProperty = True
          arr(x%).isProperty = True
        End If
      Next x%
    End If
  Next w%
End Sub

Private Sub generiereVBMethods(arr() As struktur)
  For w% = 0 To UBound(arr)
    If arr(w%).isProperty = False Then
     If arr(w%).rueckgabe <> "void" Then
      Call writeVBFunc(arr(w%))
     Else
      Call writeVBSub(arr(w%))
     End If
    End If
  Next w%
End Sub
Private Sub writeCSProperty(gSt As struktur, sSt As struktur)
  '      public static string ID3V2Album
  '      {
  '          get { return GetID3V2AlbumW(); }
  '          set { SetID3V2AlbumW(value); }
  '      }
   ' Get rausschneiden
  tmp$ = Replace$(gSt.bezeichnung, "Get", "")
  ' W am Ende entfernen
  tmp$ = Left$(tmp$, Len(tmp$) - 1)
  add "public static " & cs(gSt.rueckgabe) & " " & tmp$
  add "{"
  add "    get { return " & gSt.bezeichnung & "(); }"
  add "    set { " & sSt.bezeichnung & "(value); }"
  add "}"
End Sub
Private Function writeVBPropGet(st As struktur)
  ' Get rausschneiden
  tmp$ = Replace$(st.bezeichnung, "Get", "")
  ' W am Ende entfernen
  tmp$ = Left$(tmp$, Len(tmp$) - 1)
  add ""
  t$ = "Public Property Get " & tmp$ & "("
  t$ = t$ & paras(st.params) & ")"
  t$ = t$ & " As " & cd(st.rueckgabe)
  add t$
  t$ = "  " & tmp & " = "
  If st.rueckgabe = "BSTR" Then
    t$ = t$ & "StrConv(" & st.bezeichnung & "(" & paras2(st.params) & "), vbFromUnicode)"
  Else
    t$ = t$ & st.bezeichnung & "(" & paras2(st.params) & ")"
  End If
  add t$
  add "End Property"
End Function

Private Function writeVBPropLet(st As struktur)
  ' Set rausschneiden
  tmp$ = Replace$(st.bezeichnung, "Set", "")
  ' W am Ende entfernen
  tmp$ = Left$(tmp$, Len(tmp$) - 1)
  t$ = "Public Property Let " & tmp & "(" & paras(st.params) & ")"
  add t$
  t$ = "  Call " & st.bezeichnung & "(" & paras2(st.params) & ")"
  add t$
  add "End Property"
End Function

Private Function writeVBFunc(st As struktur)
  add ""
  ' W am Ende entfernen
  tmp$ = Left$(st.bezeichnung, Len(st.bezeichnung) - 1)
  t$ = "Public Function " & tmp$ & "(" & paras3(st.params) & ") As " & cc(st.rueckgabe)
  add t$
  t$ = "  " & tmp & " = "
  If st.rueckgabe = "BSTR" Then
    t$ = t$ & "StrConv(" & st.bezeichnung & "(" & paras2(st.params) & "), vbFromUnicode)"
  Else
   t$ = t$ & st.bezeichnung & "(" & paras2(st.params) & ")"
  End If
  add t$
  add "End Function"
End Function
Private Function writeCSFunc(st As struktur)
  'public static bool ID3V2DeletePicture(short index)
   '     {
   '         return Convert.ToBoolean(ID3V2DeletePictureW(index));
   '     }
    ' W am Ende entfernen
  Dim params() As param
  tmp$ = Left$(st.bezeichnung, Len(st.bezeichnung) - 1)
  t$ = "public static " & cs(st.rueckgabe) & " " & tmp$ & "("
  params = st.params
  u$ = ""
  For i% = 1 To UBound(params)
    If i% > 1 Then t$ = t$ & ", ": u$ = u$ & ", "
    pn$ = params(i%).name
    If Left$(pn$, 1) = "*" Then
        pn$ = cs(params(i%).param) & "[] " & Mid$(pn$, 2)
        u$ = u$ & Mid$(params(i%).name, 2)
    Else
        pn$ = cs(params(i).param) & " " & pn$
        u$ = u$ & params(i%).name
    End If
    t$ = t$ & pn$
  Next i%
  t$ = t$ & ")"
  add t$
  add "{"
  t$ = "   return " & st.bezeichnung & "(" & u$ & ");"
  add t$
  add "}"
End Function

Private Function writeCSSub(st As struktur)
  'public static void ID3V2DeletePicture(short index)
   '     {
   '         ID3V2DeletePictureW(index);
   '     }
    ' W am Ende entfernen
  Dim params() As param
  tmp$ = Left$(st.bezeichnung, Len(st.bezeichnung) - 1)
  t$ = "public static void " & tmp$ & "("
  params = st.params
  u$ = ""
  For i% = 1 To UBound(params)
    If i% > 1 Then t$ = t$ & ", ": u$ = u$ & ", "
    pn$ = params(i%).name
    If Left$(pn$, 1) = "*" Then
        pn$ = cs(params(i%).param) & "[] " & Mid$(pn$, 2)
        u$ = u$ & Mid$(params(i%).name, 2)
    Else
        pn$ = cs(params(i).param) & " " & pn$
        u$ = u$ & params(i%).name
    End If
    t$ = t$ & pn$
  Next i%
  t$ = t$ & ")"
  add t$
  add "{"
  t$ = "   " & st.bezeichnung & "(" & u$ & ");"
  add t$
  add "}"
End Function

Private Function writeVBSub(st As struktur)
  add ""
  ' W am Ende entfernen
  tmp$ = Left$(st.bezeichnung, Len(st.bezeichnung) - 1)
  t$ = "Public Sub " & tmp & "(" & paras(st.params) & ")"
  add t$
  t$ = "  Call " & st.bezeichnung & "(" & paras2(st.params) & ")"
  add t$
  add "End Sub"
End Function

Private Function cc(s$)
  Select Case (s$)
  Case "long", "LPCWSTR", "u32": cc = "Long"
  Case "BSTR": cc = "String"
  Case "SAFEARRAY": cc = "Byte"
  Case "float": cc = "Single"
  Case "BYTE*": cc = "Long"
  Case "BYTE": cc = "Byte"
  Case "int": cc = "Long"
  Case "short", "u16": cc = "Integer"
  Case "strReverb*": cc = "Long"
  Case Else:   Debug.Print s$: Stop
  End Select
End Function

Private Function cs(s$)
  Select Case (s$)
  Case "long", "u32": cs = "int"
  Case "LPCWSTR": cs = "string"
  Case "BSTR": cs = "string"
  Case "SAFEARRAY": cs = "byte"
  Case "float": cs = "float"
  Case "BYTE*": cs = "int"
  Case "BYTE": cs = "byte"
  Case "int": cs = "int"
  Case "short", "u16": cs = "short"
  Case "strReverb*": cs = "long"
  Case "void": cs = "void"
  Case Else:   Debug.Print s$: Stop
  End Select
End Function

Private Function cd(s$)
  Select Case (s$)
  Case "long": cd = "Long"
  Case "LPCWSTR": cd = "String"
  Case "BSTR": cd = "String"
  Case "SAFEARRAY": cd = "Byte"
  Case "Float": cd = "Single"
  Case Else:   cd = "Integer"
  End Select
End Function

Private Function xd(s$)
  Select Case (s$)
  Case "short": xd = "Int16"
  Case "LPCWSTR": xd = "LPCWSTR"
  Case "BSTR": xd = "BSTR"
  Case "SAFEARRAY": xd = "Byte"
  Case "float": xd = "Float"
  Case "long", "u32", "int": xd = "Int32"
  Case "void": xd = ""
  Case "BYTE": xd = "Byte"
  Case Else:   Debug.Print s$: End
  End Select
End Function

Private Sub add(z$)
  total$ = total$ & z$ & vbCrLf
End Sub

Private Function paras(params() As param) As String
   z$ = ""
   For i% = 1 To UBound(params)
      If i% > 1 Then z$ = z$ & ", "
      p$ = params(i%).name
      If Left(p$, 2) = "**" Then
        z$ = z$ & Mid$(p$, 3) & "() As " & cd(params(i%).param)
      Else
        z$ = z$ & "ByVal " & p$ & " As " & cd(params(i%).param)
      End If
    Next i%
    paras = z$
End Function

Private Function paras2(params() As param) As String
   z$ = ""
   For i% = 1 To UBound(params)
      If i% > 1 Then z$ = z$ & ", "
      p$ = params(i%).name
      If Left(p$, 2) = "**" Then
       z$ = z$ & Mid$(p$, 3)
      ElseIf Left(p$, 1) = "*" Then
        z$ = z$ & "VarPtr(" & Mid$(p$, 2) & "(0))"
      Else
        If params(i%).param = "LPCWSTR" Then
          z$ = z$ & "StrPtr(" & p$ & ")"
        Else
          z$ = z$ & p$
        End If
      End If
    Next i%
    paras2 = z$
End Function

Private Function paras3(params() As param) As String
   z$ = ""
   For i% = 1 To UBound(params)
      If i% > 1 Then z$ = z$ & ", "
      p$ = params(i%).name
      If Left(p$, 2) = "**" Then
        z$ = z$ & Mid$(p$, 3) & "() As " & cd(params(i%).param)
      ElseIf Left(p$, 1) = "*" Then
        z$ = z$ & Mid$(p$, 2) & "() As Byte"
      Else
        z$ = z$ & "ByVal " & p$ & " As " & cd(params(i%).param)
      End If
    Next i%
    paras3 = z$
End Function
Private Sub QuickSort(ByVal LB As Long, ByVal UB As Long)
    Dim P1 As Long, P2 As Long, Ref As String, TEMP As struktur

    P1 = LB
    P2 = UB
    Ref = result((P1 + P2) / 2).bezeichnung
    
    Do
        Do While (result(P1).bezeichnung < Ref)
            P1 = P1 + 1
        Loop
 
        Do While (result(P2).bezeichnung > Ref)
            P2 = P2 - 1
        Loop

        If P1 <= P2 Then
            TEMP = result(P1)
            result(P1) = result(P2)
            result(P2) = TEMP
            P1 = P1 + 1
            P2 = P2 - 1
        End If
    Loop Until (P1 > P2)

    If LB < P2 Then Call QuickSort(LB, P2)
    If P1 < UB Then Call QuickSort(P1, UB)
End Sub

