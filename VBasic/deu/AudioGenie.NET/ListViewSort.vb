Option Strict Off
Option Explicit On
Module ListViewSort
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
	' --- Notwendige Konstante
	Private Const LVM_FIRST As Integer = &H1000
	Private Const LVM_SORTITEMS As Integer = LVM_FIRST + 48
	Private Const LVM_FINDITEM As Integer = LVM_FIRST + 13
	Private Const LVM_GETITEMTEXT As Integer = LVM_FIRST + 45
	Private Const LVFI_PARAM As Integer = &H1
	Private Const LVIF_TEXT As Integer = &H1
	' --- Notwendige Strukturen
	' ListSortOrderConstants ist bereits in den Common
	' Controls identisch definiert, wird hier jedoch
	' erneut definiert, damit das Modul auch unabhängig
	' von der Referenzierung der Common Controls in einem
	' Projekt keinen Laufzeitfehler auslöst.
	Public Enum ListSortOrderConstants
		lvwAscending = 0 ' Aufsteigende Sortierung
		lvwDescending = 1 ' Absteigende Sortierung
	End Enum
	Public Enum SortTypes ' Art der Spalteninhalte
		stString = 1 ' Stringsortierung
		stDate = 2 ' Datensortierung
		stNumeric = 3 ' Zahlensortierung
	End Enum
	' Die Struktur LVWSORT enthält Informationen über das
	' zu sortierende ListView-Steuerelement, die Spalte,
	' nach der sortiert werden soll, sowie die gewünschte
	' Sortierrichtung.
	Private Structure LVWSORT
		Dim hWndListView As Integer ' Fensterhandle des ListView-Controls
		Dim SortKey As Integer ' Spalte, die sortiert werden soll
		Dim SortType As SortTypes ' Typ der zu sortierenden Daten
		Dim SortOrder As ListSortOrderConstants ' Sortierrichtung
	End Structure
	Private Structure POINTAPI ' benötigt für LV_FINDINFO
		Dim x As Integer
		Dim y As Integer
	End Structure
	Private Structure LV_FINDINFO ' benötigt für LVM_FINDITEM
		Dim flags As Integer
		Dim psz As String
		Dim lParam As Integer
		Dim pt As POINTAPI
		Dim vkDirection As Integer
	End Structure
	Private Structure LV_ITEM ' benötigt für LVM_GETITEMTEXT
		Dim mask As Integer
		Dim iItem As Integer
		Dim iSubItem As Integer
		Dim State As Integer
		Dim stateMask As Integer
		Dim pszText As Integer
		Dim cchTextMax As Integer
		Dim iImage As Integer
		Dim lParam As Integer
		Dim iIndent As Integer
	End Structure
	' --- Notwendige API-Funktionen
	Private Declare Function SendMessageLong Lib "user32"  Alias "SendMessageA"(ByVal hWnd As Integer, ByVal Message As Integer, ByVal wParam As Integer, ByVal lParam As Integer) As Integer
	'UPGRADE_ISSUE: Das Deklarieren eines Parameters als ''As Any'' wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="FAE78A8D-8978-4FD4-8208-5B7324A8F795"'
	'UPGRADE_ISSUE: Das Deklarieren eines Parameters als ''As Any'' wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="FAE78A8D-8978-4FD4-8208-5B7324A8F795"'
    Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (ByRef Destination As Any, ByRef Source As Any, ByVal Length As Integer)
	' --- Öffentliche Funktionen
	Public Sub SortListView(ByVal hWndListView As Integer, ByVal SortKey As Integer, Optional ByVal SortType As SortTypes = SortTypes.stString, Optional ByVal SortOrder As ListSortOrderConstants = ListSortOrderConstants.lvwAscending)
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
		'UPGRADE_WARNING: Einen delegate für AddressOf CompareFunc hinzufügen Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="E9E157F7-EF0C-4016-87B7-7D7FBBC6EE08"'
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		SendMessageLong(hWndListView, LVM_SORTITEMS, VarPtr(udtLVWSORT), AddressOf CompareFunc)
	End Sub
	' --- Private Funktionen
	Private Function CompareFunc(ByVal lParam1 As Integer, ByVal lParam2 As Integer, ByVal lParamSort As Integer) As Integer
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
		'UPGRADE_NOTE: ListViewSort wurde aktualisiert auf ListViewSort_Renamed. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
		Dim ListViewSort_Renamed As LVWSORT
		Dim sEntry1 As String
		Dim sEntry2 As String
		Dim vCompare1 As Object
		Dim vCompare2 As Object
		' In lParamSort von SortListView als Long-Pointer
		' übergebene LVWSORT-Struktur abholen, um auf deren
		' Werte zugreifen zu können:
		'UPGRADE_WARNING: Die Standardeigenschaft des Objekts ListViewSort_Renamed konnte nicht aufgelöst werden. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		CopyMemory(ListViewSort_Renamed, lParamSort, Len(ListViewSort_Renamed))
		' Die Werte der zu vergleichenden Elemente werden
		' mithilfe der privaten Funktion LvwGetText aus
		' den Angaben lParam1 und lParam2 ermittelt:
		sEntry1 = LvwGetText(ListViewSort_Renamed, lParam1)
		sEntry2 = LvwGetText(ListViewSort_Renamed, lParam2)
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
		Select Case ListViewSort_Renamed.SortType
			Case SortTypes.stDate ' Spalteninhalte sind Datumswerte
				CompareFunc = CompareDates(CDate(sEntry1), CDate(sEntry2), ListViewSort_Renamed.SortOrder)
			Case SortTypes.stNumeric ' Spalteninhalte sind Zahlen
				CompareFunc = CompareNumbers(sEntry1, sEntry2, ListViewSort_Renamed.SortOrder)
			Case SortTypes.stString ' Spalteninhalte sind Strings
				CompareFunc = CompareStrings(sEntry1, sEntry2, ListViewSort_Renamed.SortOrder)
		End Select
	End Function
	Private Function LvwGetText(ByRef ListViewSort As LVWSORT, ByVal lParam As Integer) As String
		' -----------------------------------------------------
		' Ermittelt aus dem Fensterhandle des ListView-
		' Steuerelements, der in ListViewSort.SortKey
		' angegebenen (nullbasierten) Spalte im ListView
		' und der an CompareFunc übergebenen Werte lParam1/2
		' die davon repräsentierten Zelleninhalte.
		' -----------------------------------------------------
		Dim udtFindInfo As LV_FINDINFO
		Dim udtLVItem As LV_ITEM
		Dim lngIndex As Integer
		Dim baBuffer(512) As Byte
		Dim lngLength As Integer
		' Den Index der Zelle aus lParam ermitteln:
		With udtFindInfo
			.flags = LVFI_PARAM
			.lParam = lParam
		End With
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		lngIndex = SendMessageLong(ListViewSort.hWndListView, LVM_FINDITEM, -1, VarPtr(udtFindInfo))
		' Auf Basis des gefundenen Index den Text der Zelle
		' in ein Byte-Array übertragen:
		With udtLVItem
			.mask = LVIF_TEXT
			.iSubItem = ListViewSort.SortKey
			'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			.pszText = VarPtr(baBuffer(0))
			.cchTextMax = UBound(baBuffer) + 1
		End With
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		lngLength = SendMessageLong(ListViewSort.hWndListView, LVM_GETITEMTEXT, lngIndex, VarPtr(udtLVItem))
		' Byte-Array in passender Länge als String-
		' Rückgabewert kopieren:
		If lngLength > 0 Then
			'UPGRADE_ISSUE: Die Konstante vbUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			LvwGetText = Left(StrConv(System.Text.UnicodeEncoding.Unicode.GetString(baBuffer), vbUnicode), lngLength)
		End If
	End Function
	Private Function CompareDates(ByVal dtEntry1 As Date, ByVal dtEntry2 As Date, ByVal SortOrder As ListSortOrderConstants) As Integer
		' -----------------------------------------------------
		' Gibt zurück, ob das erste der beiden unterschiedlichen
		' Elemente nach Maßgabe des Parameters SortOrder größer
		' (1 bei aufsteigender Sortierung) oder kleiner (-1 bei
		' aufsteigender Sortierung) als das zweite Element ist.
		' Gleiche Elemente wurden bereits in CompareFunc ausge-
		' schlossen; für sie wäre sonst 0 zurückzugeben.
		' -----------------------------------------------------
		' Rückgabewert je nach erwünschter Sortierung:
		If SortOrder = ListSortOrderConstants.lvwAscending Then
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
	Private Function CompareNumbers(ByRef sEntry1 As String, ByRef sEntry2 As String, ByVal SortOrder As ListSortOrderConstants) As Integer
		' -----------------------------------------------------
		' Gibt zurück, ob das erste der beiden unterschiedlichen
		' Elemente nach Maßgabe des Parameters SortOrder größer
		' (1 bei aufsteigender Sortierung) oder kleiner (-1 bei
		' aufsteigender Sortierung) als das zweite Element ist.
		' Gleiche Elemente wurden bereits in CompareFunc ausge-
		' schlossen; für sie wäre sonst 0 zurückzugeben.
		' -----------------------------------------------------
		' Rückgabewert je nach erwünschter Sortierung:
		If SortOrder = ListSortOrderConstants.lvwAscending Then
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
	Private Function CompareStrings(ByRef sEntry1 As String, ByRef sEntry2 As String, ByVal SortOrder As ListSortOrderConstants) As Integer
		' -----------------------------------------------------
		' Gibt zurück, ob das erste der beiden unterschiedlichen
		' Elemente nach Maßgabe des Parameters SortOrder größer
		' (1 bei aufsteigender Sortierung) oder kleiner (-1 bei
		' aufsteigender Sortierung) als das zweite Element ist.
		' Gleiche Elemente wurden bereits in CompareFunc ausge-
		' schlossen; für sie wäre sonst 0 zurückzugeben.
		' -----------------------------------------------------
		' Rückgabewert je nach erwünschter Sortierung:
		If SortOrder = ListSortOrderConstants.lvwAscending Then
			' Aufsteigende Sortierung zweier unterschiedlicher Strings
			If UCase(sEntry1) < UCase(sEntry2) Then
				CompareStrings = -1
			Else
				CompareStrings = 1
			End If
		Else ' Absteigende Sortierung
			If UCase(sEntry1) > UCase(sEntry2) Then
				CompareStrings = -1
			Else
				CompareStrings = 1
			End If
		End If
	End Function
	' --------------------------------------------------------
End Module