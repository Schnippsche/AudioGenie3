Attribute VB_Name = "Module1"
Dim Genie As clsAudioGenie
Dim indent As Integer
Public Sub Main()
 Set Genie = New clsAudioGenie
 Dim ti#
 Call Genie.DisablePopupKey("080be17a19642b49")
 Call Genie.AUDIOAnalyzeFile("C:\m4atest.m4a")
 Genie.MP4Artist = "Neuer Interpret"
 Call Genie.AUDIOGetDuration
 Call Genie.AUDIOGetBitrate
 Call Genie.MP4SaveChanges
 
 'Call Genie.ID3V2SetFormatAndEncoding(3, 0)
 'Call Genie.AUDIOSaveChangesToFile("C:\test_utf8_v24a.mp3")
 End
 'Debug.Print Genie.ID3V2GetAllFrameIDs

 Dim rootTOC As String, ID As String
 Debug.Print "---"
 Debug.Print "d1:" & Genie.ID3V2GetSubFrameDescription("ch1", 1)
 Debug.Print "d2:" & Genie.ID3V2GetSubFrameDescription("ch1", 2)
 Debug.Print "d3:" & Genie.ID3V2GetSubFrameDescription("ch1", 3)
 Debug.Print "t1:" & Genie.ID3V2GetSubFrameText("ch1", 1)
 Debug.Print "t2:" & Genie.ID3V2GetSubFrameText("ch1", 2)
 Debug.Print "t3:" & Genie.ID3V2GetSubFrameText("ch1", 3)
 
 Call Genie.ID3V2RemoveTag
 Call Genie.ID3V2AddTableOfContent("toc1", "Toc1title", "toc1desc", 1)
 Call Genie.ID3V2AddChapter("ch1", "ch1title", "ch1desc", 1, 2)
 Call Genie.ID3V2AddChapter("ch2", "ch2title", "ch2desc", 3, 4)
 Call Genie.ID3V2AddChildElement("toc1", "ch1")
 Call Genie.ID3V2AddChildElement("toc1", "ch2")
 Call Genie.ID3V2AddChildElement("toc1", "ch2")
 Call Genie.ID3V2AddTableOfContent("toc2", "Toc2title", "toc2desc", 1)
 Call Genie.ID3V2AddChildElement("toc1", "toc2")
 Call Genie.ID3V2AddChildElement("toc2", "ch1")
 Call Genie.ID3V2SetTextSubFrame("ch1", ID3F_TALB, "Album", "")
 Call Genie.ID3V2SetURLSubFrame("ch2", ID3F_WPUB, "www.toengi.de", "")
 Call Genie.ID3V2SetTextSubFrame("toc1", ID3F_TPE1, "tpe1text", "")
 Call Genie.ID3V2SetTextSubFrame("ch1", ID3F_TXXX, "Text", "Description")
 Call Genie.ID3V2SetURLSubFrame("toc1", ID3F_WXXX, "exturlText", "exturlDesc")
 Call Genie.ID3V2SetAddendumTitle("toc1", "newToc1Title")
 Call Genie.ID3V2SetAddendumTitle("ch1", "newCH1Title")
 
 Debug.Print Genie.ID3V2GetAddendumTitle("ch1")
 Debug.Print "d:" & Genie.ID3V2GetSubFrameDescription("ch1", 3)
 Debug.Print "d:" & Genie.ID3V2GetSubFrameDescription("ch1", 4)
 Debug.Print "d:" & Genie.ID3V2GetSubFrameDescription("toc1", 4)
 Debug.Print "t:" & Genie.ID3V2GetSubFrameText("ch1", 3)
 Debug.Print "t:" & Genie.ID3V2GetSubFrameText("ch1", 4)
 Debug.Print "t:" & Genie.ID3V2GetSubFrameText("toc1", 4)

 ' Call Genie.ID3V2SetImageSubFrame
 ' Call Genie.ID3V2DeleteAddendum("toc1") ' Löscht alles
 Call Genie.ID3V2SaveChanges
 
 End
 
 Genie.AUDIOAnalyzeFile ("C:\ztestv23.mp3")
 rootTOC = Genie.ID3V2GetTOCRootID
 Call getNodes(rootTOC)
 Call Genie.ID3V2AddTableOfContent("newtoc", "newTitle", "newDesc", 1)
End Sub

Public Sub getNodes(ByVal parentNode As String)
  Dim ID As String
  Dim FrameID As Long
  Debug.Print Space$(indent) & "TOC(" & parentNode & ") = " & Genie.ID3V2GetAddendumTitle(parentNode) & ", " & Genie.ID3V2GetAddendumDescription(parentNode)
  For w% = 1 To Genie.ID3V2GetChildElements(parentNode)
    ID = Genie.ID3V2GetChildElementID(parentNode, w%)
    If Genie.ID3V2GetAddendumType(ID) = 1 Then
        indent = indent + 1
        Call getNodes(ID)
    Else
      indent = indent + 1
      Debug.Print Space$(indent) & "CHAP(" & ID & ") = " & Genie.ID3V2GetAddendumTitle(ID) & Genie.ID3V2GetAddendumDescription(ID)
      Call getFrames(ID)
      indent = indent - 1
    End If
 Next w%
 Call getFrames(parentNode)
 indent = indent - 1
End Sub

Public Sub getFrames(ByVal parentNode As String)
 ' Nun noch die Frames
 Dim FrameID As Long
 Dim typ As Integer
 For w% = 1 To Genie.ID3V2GetSubFrames(parentNode)
   FrameID = Genie.ID3V2GetSubFrameID(parentNode, w%)
   typ = Genie.ID3V2GetSubFrameType(parentNode, w%)
   Debug.Print Space$(indent) & getFrameID(FrameID) & " (" & typ & ") " & Genie.ID3V2GetSubFrameDescription(parentNode, w%) & ":" & Genie.ID3V2GetSubFrameText(parentNode, w%)
   Debug.Print Space$(indent) & "[" & parentNode & "] " & Genie.ID3V2GetSubFrameDescription(parentNode, w%)
 Next w%
End Sub

Public Function getFrameID(ByVal ID As Long) As String
  getFrameID = Chr$((ID And &HFF000000) / 2 ^ 24) & Chr$((ID And &HFF0000) / 2 ^ 16) & Chr$((ID And 65280) / (2 ^ 8)) & Chr$((ID And &HFF))
End Function
