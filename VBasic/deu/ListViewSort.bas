Attribute VB_Name = "ListViewSort"
' --------------------------------------------------------
' ------------ STANDARDMODUL ListViewSort.bas ------------
' --------------------------------------------------------
' Copyright (c) 2002 by Mathias Schiffer, AixSoft Software
' --------------------------------------------------------
'
' Freie Auf- und Abwärtssortierung einer ListView-Spalte
' für Zahlen, Daten und Strings. Aufruf zur Sortierung:
'
' SortListView hWndListView, SortKey, [SortType], [SortOrder]
'
' Parameter:
' ----------
' hWndListView: Fensterhandle des ListView-Steuerelements
' SortKey:      Spalte (nullbasiert), die sortiert werden
'               soll (= Spaltennummer - 1).
' SortType:     stString, um Strings zu sortieren (Standardwert)
'               stDate, um Datumsangaben zu sortieren
'               stNumeric, um Zahlen zu sortieren
' SortOrder:    lvwAscending für aufsteigende Sortierung (Std.)
'               lvwDescending für absteigende Sortierung
' --------------------------------------------------------
Option Explicit
' --- Notwendige Konstante
Private Const LVM_FIRST       As Long = &H1000&
Private Const LVM_SORTITEMS   As Long = LVM_FIRST + 48&
Private Const LVM_FINDITEM    As Long = LVM_FIRST + 13&
Private Const LVM_GETITEMTEXT As Long = LVM_FIRST + 45&
Private Const LVFI_PARAM      As Long = &H1&
Private Const LVIF_TEXT       As Long = &H1&
' --- Notwendige Strukturen
' ListSortOrderConstants ist bereits in den Common
' Controls identisch definiert, wird hier jedoch
' erneut definiert, damit das Modul auch unabhängig
' von der Referenzierung der Common Controls in einem
' Projekt keinen Laufzeitfehler auslöst.
Public Enum ListSortOrderConstants
  lvwAscending = 0&   ' Aufsteigende Sortierung
  lvwDescending = 1&  ' Absteigende Sortierung
End Enum
Public Enum SortTypes ' Art der Spalteninhalte
  stString = 1&   ' Stringsortierung
  stDate = 2&     ' Datensortierung
  stNumeric = 3&  ' Zahlensortierung
End Enum
' Die Struktur LVWSORT enthält Informationen über das
' zu sortierende ListView-Steuerelement, die Spalte,
' nach der sortiert werden soll, sowie die gewünschte
' Sortierrichtung.
Private Type LVWSORT
  hWndListView As Long  ' Fensterhandle des ListView-Controls
  SortKey As Long       ' Spalte, die sortiert werden soll
  SortType As SortTypes ' Typ der zu sortierenden Daten
  SortOrder As ListSortOrderConstants ' Sortierrichtung
End Type
Private Type POINTAPI ' benötigt für LV_FINDINFO
  x As Long
  y As Long
End Type
Private Type LV_FINDINFO ' benötigt für LVM_FINDITEM
  flags As Long
  psz As String
  lParam As Long
  pt As POINTAPI
  vkDirection As Long
End Type
Private Type LV_ITEM ' benötigt für LVM_GETITEMTEXT
  mask As Long
  iItem As Long
  iSubItem As Long
  State As Long
  stateMask As Long
  pszText As Long
  cchTextMax As Long
  iImage As Long
  lParam As Long
  iIndent As Long
End Type
' --- Notwendige API-Funktionen
Private Declare Function SendMessageLong _
  Lib "user32" Alias "SendMessageA" ( _
  ByVal hWnd As Long, _
  ByVal Message As Long, _
  ByVal wParam As Long, _
  ByVal lParam As Long _
  ) As Long
Private Declare Sub CopyMemory _
  Lib "kernel32" Alias "RtlMoveMemory" ( _
  ByRef Destination As Any, _
  ByRef Source As Any, _
  ByVal Length As Long)
' --- Öffentliche Funktionen
Public Sub SortListView(ByVal hWndListView As Long, _
                        ByVal SortKey As Long, _
                        Optional ByVal SortType _
                          As SortTypes = stString, _
                        Optional ByVal SortOrder _
                          As ListSortOrderConstants _
                          = lvwAscending)
' -----------------------------------------------------
' Öffentlich aufzurufende Prozedur SortListView, die
' für die individuelle Sortierung einer ListView-Spalte
' sorgt.
' -----------------------------------------------------
' hWndListView: Fensterhandle des ListView-Steuerelements
' SortKey:      Spalte (nullbasiert), die sortiert werden
'               soll (= Spaltennummer - 1).
' SortType:     stString, um Strings zu sortieren (Standardwert)
'               stDate, um Datumsangaben zu sortieren
'               stNumeric, um Zahlen zu sortieren
' SortOrder:    lvwAscending für aufsteigende Sortierung (Std.)
'               lvwDescending für absteigende Sortierung
' -----------------------------------------------------
Dim udtLVWSORT As LVWSORT
  ' Übergebene Informationen in einer LVWSORT-
  ' Struktur zusammenfassen:
  With udtLVWSORT
    .hWndListView = hWndListView
    .SortKey = SortKey
    .SortOrder = SortOrder
    .SortType = SortType
  End With
  ' Eigene Sortierfunktionalität in der Funktion
  ' CompareFunc verwenden: Die Informationen der
  ' LVWSORT-Struktur wird mithilfe eines Zeigers
  ' auf die Variable udtLVWSORT beigegeben:
  SendMessageLong hWndListView, _
                  LVM_SORTITEMS, _
                  VarPtr(udtLVWSORT), _
                  AddressOf CompareFunc
End Sub
' --- Private Funktionen
Private Function CompareFunc(ByVal lParam1 As Long, _
                             ByVal lParam2 As Long, _
                             ByVal lParamSort As Long _
                             ) As Long
' -----------------------------------------------------
' Vergleichsfunktion CompareFunc
' -----------------------------------------------------
' Verglichen werden jeweils zwei Elemente der zu
' sortierenden Spalte des ListView-Steuerelements,
' die über lParam1 und lParam2 angegeben werden.
' Hierbei wird über den Rückgabewert der Funktion
' bestimmt, welches der beiden Elemente als größer
' gelten soll (hier für Aufwärtssortierung):
' * Element 1 < Element 2: Rückgabewert < 0
' * Element 1 = Element 2: Rückgabewert = 0
' * Element 1 > Element 2: Rückgabewert > 0
' -----------------------------------------------------
Dim ListViewSort As LVWSORT
Dim sEntry1 As String
Dim sEntry2 As String
Dim vCompare1 As Variant
Dim vCompare2 As Variant
  ' In lParamSort von SortListView als Long-Pointer
  ' übergebene LVWSORT-Struktur abholen, um auf deren
  ' Werte zugreifen zu können:
  CopyMemory ListViewSort, _
             ByVal lParamSort, _
             Len(ListViewSort)
  ' Die Werte der zu vergleichenden Elemente werden
  ' mithilfe der privaten Funktion LvwGetText aus
  ' den Angaben lParam1 und lParam2 ermittelt:
  sEntry1 = LvwGetText(ListViewSort, lParam1)
  sEntry2 = LvwGetText(ListViewSort, lParam2)
  ' Sind die Elemente gleich, kann die Funktion
  ' sofort mit dem aktuellen Rückgabewert 0
  ' verlassen werden:
  If sEntry1 = sEntry2 Then
    Exit Function
  End If
  ' Für die Sortierung wird unterschieden zwischen
  ' Daten, Zahlen und allgemeinen Strings. Hierfür
  ' steht jeweils eine separate, private Vergleichs-
  ' funktion zur Verfügung.
  Select Case ListViewSort.SortType
    Case stDate    ' Spalteninhalte sind Datumswerte
      CompareFunc = CompareDates(sEntry1, _
                    sEntry2, ListViewSort.SortOrder)
    Case stNumeric ' Spalteninhalte sind Zahlen
      CompareFunc = CompareNumbers(sEntry1, _
                    sEntry2, ListViewSort.SortOrder)
    Case stString  ' Spalteninhalte sind Strings
      CompareFunc = CompareStrings(sEntry1, _
                    sEntry2, ListViewSort.SortOrder)
  End Select
End Function
Private Function LvwGetText(ByRef ListViewSort As LVWSORT, _
                            ByVal lParam As Long _
                            ) As String
' -----------------------------------------------------
' Ermittelt aus dem Fensterhandle des ListView-
' Steuerelements, der in ListViewSort.SortKey
' angegebenen (nullbasierten) Spalte im ListView
' und der an CompareFunc übergebenen Werte lParam1/2
' die davon repräsentierten Zelleninhalte.
' -----------------------------------------------------
Dim udtFindInfo As LV_FINDINFO
Dim udtLVItem As LV_ITEM
Dim lngIndex As Long
Dim baBuffer(512) As Byte
Dim lngLength As Long
  ' Den Index der Zelle aus lParam ermitteln:
  With udtFindInfo
    .flags = LVFI_PARAM
    .lParam = lParam
  End With
  lngIndex = SendMessageLong(ListViewSort.hWndListView, _
                             LVM_FINDITEM, -1, _
                             VarPtr(udtFindInfo))
  ' Auf Basis des gefundenen Index den Text der Zelle
  ' in ein Byte-Array übertragen:
  With udtLVItem
    .mask = LVIF_TEXT
    .iSubItem = ListViewSort.SortKey
    .pszText = VarPtr(baBuffer(0))
    .cchTextMax = UBound(baBuffer) + 1
  End With
  lngLength = SendMessageLong(ListViewSort.hWndListView, _
                              LVM_GETITEMTEXT, lngIndex, _
                              VarPtr(udtLVItem))
  ' Byte-Array in passender Länge als String-
  ' Rückgabewert kopieren:
  If lngLength > 0 Then
    LvwGetText = Left$(StrConv(baBuffer, vbUnicode), lngLength)
  End If
End Function
Private Function CompareDates( _
  ByVal dtEntry1 As Date, _
  ByVal dtEntry2 As Date, _
  ByVal SortOrder As ListSortOrderConstants _
  ) As Long
' -----------------------------------------------------
' Gibt zurück, ob das erste der beiden unterschiedlichen
' Elemente nach Maßgabe des Parameters SortOrder größer
' (1 bei aufsteigender Sortierung) oder kleiner (-1 bei
' aufsteigender Sortierung) als das zweite Element ist.
' Gleiche Elemente wurden bereits in CompareFunc ausge-
' schlossen; für sie wäre sonst 0 zurückzugeben.
' -----------------------------------------------------
  ' Rückgabewert je nach erwünschter Sortierung:
  If SortOrder = lvwAscending Then
    ' Aufsteigende Sortierung zweier unterschiedlicher Daten
    If dtEntry1 < dtEntry2 Then
      CompareDates = -1
    Else
      CompareDates = 1
    End If
  Else ' Absteigende Sortierung
    If dtEntry1 > dtEntry2 Then
      CompareDates = -1
    Else
      CompareDates = 1
    End If
  End If
End Function
Private Function CompareNumbers( _
  ByRef sEntry1 As String, _
  ByRef sEntry2 As String, _
  ByVal SortOrder As ListSortOrderConstants _
  ) As Long
' -----------------------------------------------------
' Gibt zurück, ob das erste der beiden unterschiedlichen
' Elemente nach Maßgabe des Parameters SortOrder größer
' (1 bei aufsteigender Sortierung) oder kleiner (-1 bei
' aufsteigender Sortierung) als das zweite Element ist.
' Gleiche Elemente wurden bereits in CompareFunc ausge-
' schlossen; für sie wäre sonst 0 zurückzugeben.
' -----------------------------------------------------
  ' Rückgabewert je nach erwünschter Sortierung:
  If SortOrder = lvwAscending Then
    ' Aufsteigende Sortierung zweier unterschiedlicher Zahlen
    If CDec(sEntry1) < CDec(sEntry2) Then
      CompareNumbers = -1
    Else
      CompareNumbers = 1
    End If
  Else ' Absteigende Sortierung
    If CDec(sEntry1) > CDec(sEntry2) Then
      CompareNumbers = -1
    Else
      CompareNumbers = 1
    End If
  End If
End Function
Private Function CompareStrings( _
  ByRef sEntry1 As String, _
  ByRef sEntry2 As String, _
  ByVal SortOrder As ListSortOrderConstants _
  ) As Long
' -----------------------------------------------------
' Gibt zurück, ob das erste der beiden unterschiedlichen
' Elemente nach Maßgabe des Parameters SortOrder größer
' (1 bei aufsteigender Sortierung) oder kleiner (-1 bei
' aufsteigender Sortierung) als das zweite Element ist.
' Gleiche Elemente wurden bereits in CompareFunc ausge-
' schlossen; für sie wäre sonst 0 zurückzugeben.
' -----------------------------------------------------
  ' Rückgabewert je nach erwünschter Sortierung:
  If SortOrder = lvwAscending Then
    ' Aufsteigende Sortierung zweier unterschiedlicher Strings
    If UCase$(sEntry1) < UCase$(sEntry2) Then
      CompareStrings = -1
    Else
      CompareStrings = 1
    End If
  Else ' Absteigende Sortierung
    If UCase$(sEntry1) > UCase$(sEntry2) Then
      CompareStrings = -1
    Else
      CompareStrings = 1
    End If
  End If
End Function
' --------------------------------------------------------


