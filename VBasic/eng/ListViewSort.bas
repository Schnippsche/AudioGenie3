Attribute VB_Name = "ListViewSort"
' -------------------------------------------------------
' ----------- STANDARDMODUL ListViewSort.bas ------------
' -------------------------------------------------------
' copyright (C) 2002 by Mathias boat operator, AixSoft software
' -------------------------------------------------------
'
' free up and downward assortment of a ListView column
' for numbers, data and strings.  Call for assortment:
'
' SortListView hWndListView, SORT key, [ SORT type ], [ SORT order ]
'
' parameter:
' ---------
' hWndListView:  Window-act the ListView control member
' SORT keys:  Column (zero-been based), which are sorted
' are (= column number - 1).
' SORT type:  stString, in order to sort strings (default value)
' stDate, around dates to sort ' stNumeric to sort around numbers
' SORT order:  lvwAscending for ascending assortment (Std.)
' lvwDescending for descending assortment
' -------------------------------------------------------

Option Explicit

' -- necessary constants
Private Const LVM_FIRST       As Long = &H1000&
Private Const LVM_SORTITEMS   As Long = LVM_FIRST + 48&
Private Const LVM_FINDITEM    As Long = LVM_FIRST + 13&
Private Const LVM_GETITEMTEXT As Long = LVM_FIRST + 45&
Private Const LVFI_PARAM      As Long = &H1&
Private Const LVIF_TEXT       As Long = &H1&

' -- necessary structures
' ListSortOrderConstants is defined, however
' again is already defined here in the Common
' control identically, so that the module does not release also independently
' of the correlation of the Common controls in
' a project a run time error.

Public Enum ListSortOrderConstants
    lvwAscending = 0&   ' ascending assortment
    lvwDescending = 1&  ' descending assortment
End Enum

Public Enum SortTypes ' kind of the column contents
    stString = 1&   ' string assortment
    stDate = 2&     ' data assortment
    stNumeric = 3&  ' number assortment
End Enum

' the structure LVWSORT contains information about
' to sort end the ListView control member, which column is to be sorted,
' according to, as well as the desired
' sort direction.
Private Type LVWSORT
    hWndListView As Long  ' window-act the ListView control
    SortKey As Long       ' column, which is to be sorted
    SortType As SortTypes ' type of the data which can be sorted
    SortOrder As ListSortOrderConstants ' sort direction
End Type

Private Type POINTAPI ' needed for LV_FINDINFO
    x As Long
    y As Long
End Type

Private Type LV_FINDINFO ' needed for LVM_FINDITEM
    flags As Long
    psz As String
    lParam As Long
    pt As POINTAPI
    vkDirection As Long
End Type

Private Type LV_ITEM ' needed for LVM_GETITEMTEXT
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

' -- necessary API functions
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

' -- public functions
Public Sub SortListView(ByVal hWndListView As Long, _
   ByVal SortKey As Long, _
   Optional ByVal SortType _
   As SortTypes = stString, _
   Optional ByVal SortOrder _
   As ListSortOrderConstants _
   = lvwAscending)

    ' ----------------------------------------------------
    ' Public Procedure SortListView which can be called, which provides
    ' for the individual assortment of a ListView column.
    '
    ' ----------------------------------------------------
    ' hWndListView:  Window-act the ListView control member
    ' SORT keys:  Column (zero-been based), which are sorted
    ' are (= column number - 1).
    ' SORT type:  stString, in order to sort strings (default value)
    ' stDate, around dates to sort
    ' stNumeric to sort around numbers
    ' SORT order:  lvwAscending for ascending assortment (Std.)
    ' lvwDescending for descending assortment
    ' ----------------------------------------------------

    Dim udtLVWSORT As LVWSORT

    ' handed over information in a Lvwsort
    ' structure summarizes:
    With udtLVWSORT
        .hWndListView = hWndListView
        .SortKey = SortKey
        .SortOrder = SortOrder
        .SortType = SortType
    End With

    ' own sort functionality in the function
    ' CompareFunc use:  The information assistance of a pointer
    ' is udtLVWSORT added to the
    ' LVWSORT structure on the variable:
    SendMessageLong hWndListView, _
       LVM_SORTITEMS, _
       VarPtr(udtLVWSORT), _
       AddressOf CompareFunc
End Sub

Private Function CompareDates( _
   ByVal dtEntry1 As Date, _
   ByVal dtEntry2 As Date, _
   ByVal SortOrder As ListSortOrderConstants _
   ) As Long

    ' ----------------------------------------------------
    ' returns whether first of the two different
    ' elements is in accordance with condition of the parameter SORT order more largely
    ' (1 during ascending assortment) or smaller (-1 during
    ' ascending assortment) than the second element.
    ' same elements became already in CompareFunc ausge
    ' closed;  for it would be to be returned otherwise 0.
    ' ----------------------------------------------------
  
    ' return value depending upon desired assortment:
    If SortOrder = lvwAscending Then
        ' ascending assortment of two different data
        If dtEntry1 < dtEntry2 Then
            CompareDates = -1
        Else
            CompareDates = 1
        End If
    Else ' descending assortment
        If dtEntry1 > dtEntry2 Then
            CompareDates = -1
        Else
            CompareDates = 1
        End If
    End If
  
End Function

' -- private functions
Private Function CompareFunc(ByVal lParam1 As Long, _
   ByVal lParam2 As Long, _
   ByVal lParamSort As Long _
   ) As Long

    ' ----------------------------------------------------
    ' comparison function CompareFunc
    ' ----------------------------------------------------
    ' to be compared in each case two elements of the
    ' sorting column of the ListView control member,
    ' over lParam1 and lParam2 to be indicated.
    ' here over the return value of the function
    ' is determined, which of the two elements as larger
    ' is to be considered (here for upward assortment):
    ' * element 1 < element 2:  Return value < 0
    ' * element 1 = element 2:  Return value = 0
    ' * element 1 > element 2:  Return value > 0
    ' ----------------------------------------------------

    Dim ListViewSort As LVWSORT
    Dim sEntry1 As String
    Dim sEntry2 As String
    Dim vCompare1 As Variant
    Dim vCompare2 As Variant
  
    ' into lParamSort LVWSORT structure handed over of SortListView as Long pointers
    ' fetch, in order to be able to access of them
    ' values:
    CopyMemory ListViewSort, _
       ByVal lParamSort, _
       Len(ListViewSort)
  
    ' the values of the elements which can be compared are determined
    ' assistance of the private function LvwGetText from
    ' the instructions lParam1 and lParam2:
    sEntry1 = LvwGetText(ListViewSort, lParam1)
    sEntry2 = LvwGetText(ListViewSort, lParam2)
  
    ' the elements are alike, can be left the function
    ' immediately with the current return value 0:
    If sEntry1 = sEntry2 Then
        Exit Function
    End If
  
    ' for the assortment distinctive between
    ' data, numbers and general strings.  For this
    ' in each case a separate, private comparison
    ' is to function at the disposal.
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

Private Function CompareNumbers( _
   ByRef sEntry1 As String, _
   ByRef sEntry2 As String, _
   ByVal SortOrder As ListSortOrderConstants _
   ) As Long

    ' ----------------------------------------------------
    ' returns whether first of the two different
    ' elements is in accordance with condition of the parameter SORT order more largely
    ' (1 during ascending assortment) or smaller (-1 during
    ' ascending assortment) than the second element.
    ' same elements became already in CompareFunc ausge
    ' closed;  for it would be to be returned otherwise 0.
    ' ----------------------------------------------------
  
    ' return value depending upon desired assortment:
    If SortOrder = lvwAscending Then
        ' ascending assortment of two different numbers
        If CDec(sEntry1) < CDec(sEntry2) Then
            CompareNumbers = -1
        Else
            CompareNumbers = 1
        End If
    Else ' descending assortment

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

    ' ----------------------------------------------------
    ' returns whether first of the two different
    ' elements is in accordance with condition of the parameter SORT order more largely
    ' (1 during ascending assortment) or smaller (-1 during
    ' ascending assortment) than the second element.
    ' same elements became already in CompareFunc ausge
    ' closed;  for it would be to be returned otherwise 0.
    ' ----------------------------------------------------
  
    ' return value depending upon desired assortment:
    If SortOrder = lvwAscending Then
        ' ascending assortment of two different strings
        If UCase$(sEntry1) < UCase$(sEntry2) Then
            CompareStrings = -1
        Else
            CompareStrings = 1
        End If
    Else ' descending assortment
        If UCase$(sEntry1) > UCase$(sEntry2) Then
            CompareStrings = -1
        Else
            CompareStrings = 1
        End If
    End If

End Function

' --------------------------------------------------------

Private Function LvwGetText(ByRef ListViewSort As LVWSORT, _
   ByVal lParam As Long _
   ) As String

    ' ----------------------------------------------------
    ' determined from window-acts the ListView of
    ' control member, that in ListViewSort.SortKey
    ' indicated (were based) column in the ListView
    ' and the values lParam1/2
    ' cell contents represented handed over at CompareFunc of it.
    ' ----------------------------------------------------

    Dim udtFindInfo As LV_FINDINFO
    Dim udtLVItem As LV_ITEM
    Dim lngIndex As Long
    Dim baBuffer(512) As Byte
    Dim lngLength As Long

    ' the index of the cell out lParam determine:
    With udtFindInfo
        .flags = LVFI_PARAM
        .lParam = lParam
    End With

    lngIndex = SendMessageLong(ListViewSort.hWndListView, _
       LVM_FINDITEM, -1, _
       VarPtr(udtFindInfo))
  
    ' to basis found index the text of the cell
    ' into a byte array transfer:
    With udtLVItem
        .mask = LVIF_TEXT
        .iSubItem = ListViewSort.SortKey
        .pszText = VarPtr(baBuffer(0))
        .cchTextMax = UBound(baBuffer) + 1
    End With

    lngLength = SendMessageLong(ListViewSort.hWndListView, _
       LVM_GETITEMTEXT, lngIndex, _
       VarPtr(udtLVItem))
  
    ' byte array in suitable length as stringer
    ' return value copy:
    If lngLength > 0 Then
        LvwGetText = Left$(StrConv(baBuffer, vbUnicode), lngLength)
    End If

End Function

