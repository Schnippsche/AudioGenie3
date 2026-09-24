' *******************************************************
' ************ AUDIOGENIE DLL WRAPPER FOR VB ************
' *******************************************************

Imports System
Imports System.Text
Imports System.Runtime.InteropServices

Namespace AudioGenie
#Region "Public Enums"

    Public Enum AudioFormatID As Short
        UNKNOWN = 0
        MPEG = 1
        WMA = 2
        MONKEY = 3
        FLAC = 4
        WAV = 5
        OGGVORBIS = 6
        MPEGPLUS = 7
        AAC = 8
        MP4M4A = 9
        TTA = 10
        WAVPACK = 11
    End Enum

    Public Enum PictureType As Short
        OTHER = 0
        FILE_ICON = 1
        OTHER_FILE_ICON = 2
        COVER_FRONT = 3
        COVER_BACK = 4
        LEAFLET_PAGE = 5
        MEDIA_LABEL = 6
        LEAD_ARTIST = 7
        ARTIST_PERFORMER = 8
        CONDUCTOR = 9
        BAND_ORCHESTRA = 10
        COMPOSER = 11
        LYRICIST = 12
        RECORDING_LOCATION = 13
        DURING_RECORDING = 14
        DURING_PERFORMANCE = 15
        MOVIE_VIDEO_SCREEN_CAPTURE = 16
        A_BRIGHT_COLOURED_FISH = 17
        ILLUSTRATION = 18
        BAND_ARTIST_LOGOTYPE = 19
        PUBLISHER_STUDIO_LOGOTYPE = 20
    End Enum

    Public Enum PictureLink As Short
        AS_FILENAME = -1
        AS_PICTURE = 0
    End Enum

    Public Enum ID3V2FRAMES As Integer
        ID3F_AENC = &H41454E43
        ID3F_APIC = &H41504943
        ID3F_ASPI = &H41535049
        ID3F_CHAP = &H43484150
        ID3F_COMM = &H434F4D4D
        ID3F_COMR = &H434F4D52
        ID3F_CTOC = &H43544F43
        ID3F_ENCR = &H454E4352
        ID3F_EQU2 = &H45515532
        ID3F_EQUA = &H45515541
        ID3F_ETCO = &H4554434F
        ID3F_GEOB = &H47454F42
        ID3F_GRID = &H47524944
        ID3F_IPLS = &H49504C53
        ID3F_LINK = &H4C494E4B
        ID3F_MCDI = &H4D434449
        ID3F_MLLT = &H4D4C4C54
        ID3F_OWNE = &H4F574E45
        ID3F_PCNT = &H50434E54
        ID3F_POPM = &H504F504D
        ID3F_POSS = &H504F5353
        ID3F_PRIV = &H50524956
        ID3F_RBUF = &H52425546
        ID3F_RVA2 = &H52564132
        ID3F_RVAD = &H52564144
        ID3F_RVRB = &H52565242
        ID3F_SEEK = &H5345454B
        ID3F_SIGN = &H5349474E
        ID3F_SYLT = &H53594C54
        ID3F_SYTC = &H53595443
        ID3F_TALB = &H54414C42
        ID3F_TBPM = &H5442504D
        ID3F_TCMP = &H54434D50 'itunes
        ID3F_TCOM = &H54434F4D
        ID3F_TCON = &H54434F4E
        ID3F_TCOP = &H54434F50
        ID3F_TDAT = &H54444154
        ID3F_TDEN = &H5444454E
        ID3F_TDLY = &H54444C59
        ID3F_TDOR = &H54444F52
        ID3F_TDRC = &H54445243
        ID3F_TDRL = &H5444524C
        ID3F_TDTG = &H54445447
        ID3F_TENC = &H54454E43
        ID3F_TEXT = &H54455854
        ID3F_TFLT = &H54464C54
        ID3F_TIME = &H54494D45
        ID3F_TIPL = &H5449504C
        ID3F_TIT1 = &H54495431
        ID3F_TIT2 = &H54495432
        ID3F_TIT3 = &H54495433
        ID3F_TKEY = &H544B4559
        ID3F_TLAN = &H544C414E
        ID3F_TLEN = &H544C454E
        ID3F_TMCL = &H544D434C
        ID3F_TMED = &H544D4544
        ID3F_TMOO = &H544D4F4F
        ID3F_TOAL = &H544F414C
        ID3F_TOFN = &H544F464E
        ID3F_TOLY = &H544F4C59
        ID3F_TOPE = &H544F5045
        ID3F_TORY = &H544F5259
        ID3F_TOWN = &H544F574E
        ID3F_TPE1 = &H54504531
        ID3F_TPE2 = &H54504532
        ID3F_TPE3 = &H54504533
        ID3F_TPE4 = &H54504534
        ID3F_TPOS = &H54504F53
        ID3F_TPRO = &H5450524F
        ID3F_TPUB = &H54505542
        ID3F_TRCK = &H5452434B
        ID3F_TRDA = &H54524441
        ID3F_TRSN = &H5452534E
        ID3F_TRSO = &H5452534F
        ID3F_TSIZ = &H5453495A
        ID3F_TSO2 = &H54534F32 'itunes
        ID3F_TSOA = &H54534F41 'itunes
        ID3F_TSOC = &H54534F43 'itunes
        ID3F_TSOP = &H54534F50 'itunes
        ID3F_TSOT = &H54534F54 'itunes
        ID3F_TSRC = &H54535243
        ID3F_TSSE = &H54535345
        ID3F_TSST = &H54535354
        ID3F_TXXX = &H54585858
        ID3F_TYER = &H54594552
        ID3F_UFID = &H55464944
        ID3F_USER = &H55534552
        ID3F_USLT = &H55534C54
        ID3F_WCOM = &H57434F4D
        ID3F_WCOP = &H57434F50
        ID3F_WOAF = &H574F4146
        ID3F_WOAR = &H574F4152
        ID3F_WOAS = &H574F4153
        ID3F_WORS = &H574F5253
        ID3F_WPAY = &H57504159
        ID3F_WPUB = &H57505542
        ID3F_WXXX = &H57585858
    End Enum

    Public Enum MP4FRAMES As Integer
        MP4_ALBUM = &HA9616C62            ' ©alb
        MP4_ARTIST = &HA9415254           ' ©ART
        MP4_AUTHOR = &HA9617574           ' ©aut
        MP4_ALBUMARTIST = &H61415254      ' aART
        MP4_BEATSPERMINUTE = &H746D706F   ' tmpo
        MP4_CATEGORY = &H63617467         ' catg
        MP4_CDSETNUMBER = &H2064736B      '  dsk
        MP4_DISCNUMBER = &H6469736B       ' disk
        MP4_COMMENT = &HA9636D74          ' ©cmt
        MP4_COMPOSER = &HA9777274         ' ©wrt
        MP4_COMPILATION = &H6370696C      ' cpil
        MP4_DAY = &HA9646179              ' ©day
        MP4_COPYRIGHT = &HA9637079        ' ©cpy
        MP4_CREATEDATE = &HA9646179       ' ©day
        MP4_DESCRIPTION = &HA9646573      ' ©des
        MP4_INFORMATION = &HA9696E66      ' ©inf
        MP4_DIRECTOR = &HA9646972         ' ©dir
        MP4_DISCLAIMER = &HA9646973       ' ©dis
        MP4_ENCODER = &HA9746F6F          ' ©too
        MP4_GROUPING = &HA9677270         ' ©grp
        MP4_KEYWORD = &H6B657977          ' keyw
        MP4_LYRICS = &HA96C7972           ' ©lyr
        MP4_TITLE = &HA96E616D            ' ©nam
        MP4_NETURL = &HA975726C           ' ©url
        MP4_ORIGINALARTIST = &HA96F765    ' ©ope
        MP4_ORIGINALFORMAT = &HA9666D74   ' ©fmt
        MP4_ORIGINALSOURCE = &HA9737263   ' ©src
        MP4_PERFORMER = &HA9707266        ' ©prf
        MP4_PRODUCER = &HA9707264         ' ©prd
        MP4_RATING = &H72617465           ' rate
        MP4_WARNING = &HA977726E          ' ©wrn
    End Enum

    ' Infos at http://www.sno.phy.queensu.ca/~phil/exiftool/TagNames/RIFF.html
    Public Enum WAVFRAMES As Integer
        WAV_IARL = &H4941524C ' ArchivalLocation
        WAV_IART = &H49415254 ' Artist
        WAV_ICMS = &H49434D53 ' Comissioned
        WAV_ICMT = &H49434D54 ' Comment
        WAV_ICOP = &H49434F50 ' Copyright
        WAV_ICRD = &H49435244 ' CreationDate
        WAV_ICRP = &H49435250 ' Cropped
        WAV_IDIM = &H4944494D ' Dimension
        WAV_IDPI = &H49445049 ' DotsPerInch
        WAV_IENG = &H49454E47 ' Engineer
        WAV_IGNR = &H49474E52 ' Genre
        WAV_IKEY = &H494B4559 ' Keywords
        WAV_ILGT = &H494C4754 ' Lightness
        WAV_IMED = &H494D4544 ' Medium
        WAV_INAM = &H494E414D ' Name
        WAV_IPLT = &H49504C54 ' PaletteSetting
        WAV_IPRD = &H49505244 ' Product
        WAV_IRTD = &H49525444 ' Rating
        WAV_ISBJ = &H4953424A ' Subject
        WAV_ISFT = &H49534654 ' Software
        WAV_ISHP = &H49534850 ' Sharpness
        WAV_ISRC = &H49535243 ' Source
        WAV_ISRF = &H49535246 ' SourceForm
        WAV_ITCH = &H49544348 ' Technician
        WAV_ITRK = &H4954524B ' Track
    End Enum

    ' Infos at http://www.cartchunk.org/
    Public Enum WAVCARTENTRIES As Short
        WAV_CART_VERSION = 0
        WAV_CART_TITLE = 1
        WAV_CART_ARTIST = 2
        WAV_CART_CUTID = 3
        WAV_CART_CLIENTID = 4
        WAV_CART_CATEGORY = 5
        WAV_CART_CLASSIFICATION = 6
        WAV_CART_OUTCUE = 7
        WAV_CART_STARTDATE = 8
        WAV_CART_STARTTIME = 9
        WAV_CART_ENDDATE = 10
        WAV_CART_ENDTIME = 11
        WAV_CART_PRODUCERAPPID = 12
        WAV_CART_PRODUCERAPPVERSION = 13
        WAV_CART_USERDEF = 14
        WAV_CART_LEVELREFERENCE = 15
        WAV_CART_POSTTIMER0 = 16
        WAV_CART_POSTTIMER1 = 17
        WAV_CART_POSTTIMER2 = 18
        WAV_CART_POSTTIMER3 = 19
        WAV_CART_POSTTIMER4 = 20
        WAV_CART_POSTTIMER5 = 21
        WAV_CART_POSTTIMER6 = 22
        WAV_CART_POSTTIMER7 = 23
        WAV_CART_RESERVED = 24
        WAV_CART_URL = 25
        WAV_CART_TAGTEXT = 26
    End Enum

    Public Enum SyncLyricContentTypes As Short
        UNGUELTIG = -1
        SONSTIGES = 0
        SONGTEXTE = 1
        TEXTUMSCHREIBUNGEN = 2
        BEWEGUNGEN = 3
        EREIGNISSE = 4
        AKKORDE = 5
        BELANGLOSES = 6
        URLS_ZU_WEBSEITEN = 7
        URLS_ZU_BILDERN = 8
    End Enum

    Public Enum SyncLyricTimeFormat As Short
        UNGUELTIG = -1
        MPEGFRAMES = 1
        MILLISEKUNDEN = 2
    End Enum

#End Region

    Public Module AudioGenie2
        Sub New()
        End Sub
        Public Const WM_ALBUMARTIST As String = "WM/AlbumArtist"
        Public Const WM_ALBUMTITLE As String = "WM/AlbumTitle"
        Public Const WM_AUTHOR As String = "WM/Author"
        Public Const WM_AUTHORURL As String = "WM/AuthorURL"
        Public Const WM_BEATSPERMINUTE As String = "WM/BeatsPerMinute"
        Public Const WM_CATEGORY As String = "WM/Category"
        Public Const WM_COMPOSER As String = "WM/Composer"
        Public Const WM_CONDUCTOR As String = "WM/Conductor"
        Public Const WM_CONTENTDISTRIBUTOR As String = "WM/ContentDistributor"
        Public Const WM_CONTENTGROUPDESCRIPTION As String = "WM/ContentGroupDescription"
        Public Const WM_DESCRIPTION As String = "WM/Description"
        Public Const WM_DISCNUMBER As String = "WM/PartOfSet"
        Public Const WM_ENCODEDBY As String = "WM/EncodedBy"
        Public Const WM_ENCODINGSETTINGS As String = "WM/EncodingSettings"
        Public Const WM_ENCODINGTIME As String = "WM/EncodingTime"
        Public Const WM_GENRE As String = "WM/Genre"
        Public Const WM_INITIALKEY As String = "WM/InitialKey"
        Public Const WM_ISRC As String = "WM/ISRC"
        Public Const WM_LANGUAGE As String = "WM/Language"
        Public Const WM_LYRICS As String = "WM/Lyrics"
        Public Const WM_MOOD As String = "WM/Mood"
        Public Const WM_PARENTALRATING As String = "WM/ParentalRating"
        Public Const WM_PARTOFSET As String = "WM/PartOfSet"
        Public Const WM_PERIOD As String = "WM/Period"
        Public Const WM_PRODUCER As String = "WM/Producer"
        Public Const WM_PROMOTIONURL As String = "WM/PromotionURL"
        Public Const WM_PROVIDER As String = "WM/Provider"
        Public Const WM_PROVIDERCOPYRIGHT As String = "WM/ProviderCopyright"
        Public Const WM_PROVIDERRATING As String = "WM/ProviderRating"
        Public Const WM_PUBLISHER As String = "WM/Publisher"
        Public Const WM_SDBRATING As String = "SDB/Rating"
        Public Const WM_SHAREDUSERRATING As String = "WM/SharedUserRating"
        Public Const WM_TITLE As String = "WM/Title"
        Public Const WM_TOOLNAME As String = "WM/ToolName"
        Public Const WM_TOOLVERSION As String = "WM/ToolVersion"
        Public Const WM_TRACK As String = "WM/Track"
        Public Const WM_TRACKNUMBER As String = "WM/TrackNumber"
        Public Const WM_USERWEBURL As String = "WM/UserWebURL"
        Public Const WM_WRITER As String = "WM/Writer"
        Public Const WM_YEAR As String = "WM/Year"

#Region "Native Methods"

        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AACGetBitRateTypeW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AACGetHeaderTypeW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AACGetProfileW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APEExistsW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APEGetAlbumW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APEGetArtistW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APEGetCommentW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APEGetCopyrightW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APEGetGenreW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APEGetItemKeysW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APEGetSizeW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APEGetTitleW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APEGetTrackW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APEGetUserItemW(ByVal Key As String) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APEGetVersionW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APEGetYearW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APERemoveTagFromFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APERemoveTagW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APESaveChangesToFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function APESaveChangesW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub APESetAlbumW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub APESetArtistW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub APESetCommentW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub APESetCopyrightW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub APESetGenreW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub APESetTitleW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub APESetTrackW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub APESetUserItemW(ByVal Key As String, ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub APESetYearW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOAnalyzeFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOIsValidFormatW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetAlbumW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetArtistW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetBitrateW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetChannelModeW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetChannelsW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetCommentW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetComposerW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetDurationMillisW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetDurationW() As Single
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetFileSizeW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetGenreW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetLastErrorNumberW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetLastErrorTextW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetLastFileW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetMD5ValueW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetSampleRateW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetTitleW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetTrackW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetVersionW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOGetYearW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOSaveChangesToFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function AUDIOSaveChangesW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub AUDIOSetAlbumW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub AUDIOSetArtistW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub AUDIOSetCommentW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub AUDIOSetComposerW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub AUDIOSetGenreW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub AUDIOSetTitleW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub AUDIOSetTrackW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub AUDIOSetYearW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetBitsPerSampleW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetCompressionRatioW() As Single
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetItemKeysW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetMaxBlockSizeW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetMaxFrameSizeW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetMinBlockSizeW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetMinFrameSizeW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetSamplesW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetUserItemW(ByVal ItemKey As String) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetVendorW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACSaveChangesToFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACSaveChangesW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub FLACSetUserItemW(ByVal ItemKey As String, ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub FLACSetVendorW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function GetAudioGenieVersionW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function GetConfigValueW(ByVal key As Integer) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function GetMD5ValueFromFileW(ByVal FileName As String) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1ExistsW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1GetAlbumW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1GetArtistW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1GetCommentW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1GetGenreIDW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1GetGenreItemW(ByVal number As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1GetGenreW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1GetGenresW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1GetTitleW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1GetTrackW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1GetVersionW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1GetYearW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1RemoveTagFromFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1RemoveTagW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1SaveChangesToFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V1SaveChangesW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub ID3V1SetAlbumW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub ID3V1SetArtistW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub ID3V1SetCommentW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub ID3V1SetGenreIDW(ByVal nNewValue As Short)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub ID3V1SetGenreW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub ID3V1SetTitleW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub ID3V1SetTrackW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub ID3V1SetYearW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddAudioEncryptionW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal URL As String, ByVal PreviewStart As Short, ByVal PreviewLength As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddAudioSeekPointW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal start As Integer, ByVal length As Integer, ByVal numbers As Short, ByVal BitsPerPoint As Byte) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddChapterW(ByVal ID As String, ByVal Title As String, ByVal Description As String, ByVal startTime As Integer, ByVal endTime As Integer) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddChildElementW(ByVal ParentTocID As String, ByVal ChildID As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddCommentW(ByVal Language As String, ByVal Description As String, ByVal Text As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddCommercialFrameW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Price As String, ByVal validUntil As String, ByVal contactUrl As String, ByVal receivedAs As Short, _
        ByVal seller As String, ByVal description As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddEncryptionW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal URL As String, ByVal Symbol As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddEqualisationW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Interpolationmethod As Byte, ByVal Identification As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddEventTimingCodesW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal TimestampFormat As Byte) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddGeneralObjectW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Mime As String, ByVal FileName As String, ByVal Description As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddGroupIdentificationW(ByVal arr As Byte(), ByVal length As Integer, ByVal Url As String, ByVal symbol As Byte) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddLinkedInformationW(ByVal FrameIdentifier As Integer, ByVal URL As String, ByVal additionalData As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddLyricW(ByVal Language As String, ByVal Description As String, ByVal Text As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddMpegLocationLookupTableW(ByVal arr As Byte(), ByVal length As Integer, ByVal Frames As Integer, ByVal Bytes As Integer, ByVal Milliseconds As Integer, ByVal BytesDeviation As Byte, _
        ByVal MillisecondsDeviation As Byte) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddMusicCdIdentifierW(ByVal arr As Byte(), ByVal length As Integer) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddOwnershipW(ByVal Price As String, ByVal dateString As String, ByVal Seller As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddPictureArrayW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Description As String, ByVal PictureType As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddPictureFileW(ByVal FileName As String, ByVal Description As String, ByVal PictureType As Short, ByVal asLink As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddPopularimeterW(ByVal Email As String, ByVal Rating As Short, ByVal Counter As Integer) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddPositionSynchronisationW(ByVal TimestampFormat As Byte, ByVal Position As Integer) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddPrivateFrameW(ByVal arr As Byte(), ByVal length As Integer, ByVal URL As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddRecommendedBufferSizeW(ByVal BufferSize As Integer, ByVal EmbeddedInfoFlag As Byte, ByVal Offset As Integer) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddRelativeVolumeAdjustmentW(ByVal arr As Byte(), ByVal length As Integer, ByVal Identifier As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddReverbW(ByVal reverbLeft As Short, ByVal reverbRight As Short, ByVal bouncesLeft As Byte, ByVal bouncesRight As Byte, ByVal feedbackLeftToLeft As Byte, ByVal feedbackLeftToRight As Byte, _
        ByVal feedbackRightToRight As Byte, ByVal feedbackRightToLeft As Byte, ByVal premixLeftToRight As Byte, ByVal premixRightToLeft As Byte) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddSignatureFrameW(ByVal arr As Byte(), ByVal length As Integer, ByVal GroupSymbol As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddSyncLyricW(ByVal Language As String, ByVal Description As String, ByVal Text As String, ByVal ContentType As Short, ByVal TimeStampFormat As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddSynchronizedTempoW(ByVal arr As Byte(), ByVal length As Integer, ByVal Format As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddTableOfContentW(ByVal ID As String, ByVal Title As String, ByVal Description As String, ByVal isOrdered As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddUniqueFileIdentifierW(ByVal arr As Byte(), ByVal length As Integer, ByVal Owner As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddUserFrameW(ByVal Language As String, ByVal Text As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddUserTextW(ByVal Description As String, ByVal Text As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddUserURLW(ByVal Description As String, ByVal URL As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2DeleteAddendumW(ByVal ID As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2DeleteAllFramesW(ByVal FrameID As ID3V2FRAMES) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2DeleteChildElementW(ByVal ParentTocID As String, ByVal ChildID As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2DeleteSelectedFrameW(ByVal FrameID As ID3V2FRAMES, ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2DeleteSubFrameW(ByVal ID As String, ByVal FrameID As ID3V2FRAMES) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2ExistsW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetAddendumDescriptionW(ByVal ID As String) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetAddendumTitleW(ByVal ID As String) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetAddendumTypeW(ByVal ID As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetAllFrameIDsW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetAudioEncryptionDataW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetAudioEncryptionPreviewLengthW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetAudioEncryptionPreviewStartW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetAudioEncryptionURLW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetAudioSeekPointBitsPerIndexpointW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetAudioSeekPointDataW(ByVal arr As Byte(), ByVal maxLen As Integer) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetAudioSeekPointLengthW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetAudioSeekPointNumberW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetAudioSeekPointStartW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetChapterEndTimeW(ByVal ID As String) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetChapterStartTimeW(ByVal ID As String) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetChildElementIDW(ByVal ID As String, ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetChildElementsW(ByVal ID As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetCommentDescriptionW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetCommentLanguageW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetCommentW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetCommercialFrameContactURLW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetCommercialFrameDescriptionW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetCommercialFramePictureMimeW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetCommercialFramePictureW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetCommercialFramePriceW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetCommercialFrameReceivedAsW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetCommercialFrameSellerNameW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetCommercialFrameValidUntilW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetEncodingW(ByVal FrameID As ID3V2FRAMES) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetEncryptionDataW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetEncryptionSymbolW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetEncryptionURLW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetEqualisationAdjustmentBitsW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetEqualisationDataW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetEqualisationIdentificationW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetEqualisationInterpolationW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetEventTimingCodesDataW(ByVal arr As Byte(), ByVal maxLen As Integer) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetEventTimingCodesTimestampFormatW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetFrameCountW(ByVal FrameID As ID3V2FRAMES) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetGeneralObjectDataW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetGeneralObjectDescriptionW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetGeneralObjectFilenameW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetGeneralObjectMimeW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetGenreW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetGroupIdentificationDataW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetGroupIdentificationSymbolW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetGroupIdentificationURLW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetLinkedInformationAdditionalDataW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetLinkedInformationFrameIdentifierW(ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetLinkedInformationURLW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetLyricDescriptionW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetLyricLanguageW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetLyricW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetMpegLocationLookupTableBytesDeviationW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetMpegLocationLookupTableBytesW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetMpegLocationLookupTableDataW(ByVal arr As Byte(), ByVal maxLen As Integer) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetMpegLocationLookupTableFramesW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetMpegLocationLookupTableMillisecondsDeviationW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetMpegLocationLookupTableMillisecondsW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetMusicCdIdentifierDataW(ByVal arr As Byte(), ByVal maxLen As Integer) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetOwnershipDateW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetOwnershipPriceW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetOwnershipSellerW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPictureArrayW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPictureDescriptionW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPictureFileW(ByVal FileName As String, ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPictureMimeW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPictureTypeTextW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPictureTypeW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPlayCounterW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPopularimeterCounterW(ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPopularimeterEmailW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPopularimeterRatingW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPositionSynchronisationTimestampFormatW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPositionSynchronisationValueW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPossibleCHAPIDW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPossibleTOCIDW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPrivateFrameDataW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPrivateFrameURLW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetRecommendedBufferSizeFlagW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetRecommendedBufferSizeOffsetW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetRecommendedBufferSizeValueW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetRelativeVolumeAdjustmentDataW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetRelativeVolumeAdjustmentIdentifierW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetReverbBouncesLeftW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetReverbBouncesRightW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetReverbFeedbackLeftToLeftW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetReverbFeedbackLeftToRightW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetReverbFeedbackRightToLeftW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetReverbFeedbackRightToRightW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetReverbLeftW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetReverbPremixLeftToRightW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetReverbPremixRightToLeftW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetReverbRightW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSeekOffsetW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSignatureFrameDataW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSignatureFrameGroupSymbolW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSizeW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSubFrameDescriptionW(ByVal ID As String, ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSubFrameIDW(ByVal ID As String, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSubFrameImageTypeW(ByVal ID As String, ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSubFrameImageW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal ID As String, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSubFrameTextW(ByVal ID As String, ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSubFrameTypeW(ByVal ID As String, ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSubFramesW(ByVal ID As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSyncLyricContentTypeW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSyncLyricDescriptionW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSyncLyricLanguageW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSyncLyricTimeFormatW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSyncLyricW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSynchronizedTempoDataW(ByVal arr As Byte(), ByVal maxLen As Integer) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetSynchronizedTempoFormatW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetTOCIsOrderedW(ByVal ID As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetTOCRootIDW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetTextFrameW(ByVal FrameID As ID3V2FRAMES) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetURLFrameW(ByVal FrameID As ID3V2FRAMES) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetUniqueFileIdentifierDataW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetUniqueFileIdentifierOwnerW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetUserFrameLanguageW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetUserFrameW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetUserTextDescriptionW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetUserTextW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetUserURLDescriptionW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetUserURLW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetVersionW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2ImportCueFileW(ByVal FileName As [String]) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2RemoveTagFromFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2RemoveTagW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2SaveChangesToFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2SaveChangesW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2SetAddendumDescriptionW(ByVal ID As String, ByVal newDescription As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2SetAddendumTitleW(ByVal ID As String, ByVal newTitle As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2SetChapterTimesW(ByVal ID As String, ByVal startTime As Integer, ByVal endTime As Integer) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2SetFormatAndEncodingW(ByVal format As Short, ByVal encoding As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub ID3V2SetGenreW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2SetImageSubFrameW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal ID As String, ByVal Description As String, ByVal pictureType As PictureType) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub ID3V2SetPlayCounterW(ByVal counter As Integer)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2AddSeekOffsetW(ByVal offset As Integer) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2SetTOCIsOrderedW(ByVal ID As String, ByVal status As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub ID3V2SetTextFrameW(ByVal FrameID As ID3V2FRAMES, ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2SetTextSubFrameW(ByVal ID As String, ByVal FrameID As ID3V2FRAMES, ByVal textString As String, ByVal description As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub ID3V2SetURLFrameW(ByVal FrameID As ID3V2FRAMES, ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2SetURLSubFrameW(ByVal ID As String, ByVal FrameID As ID3V2FRAMES, ByVal urlString As String, ByVal description As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSExistsW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSGetAlbumW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSGetArtistW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSGetAuthorW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSGetGenreW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSGetImageLinkW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSGetIndicationW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSGetInformationW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSGetLyricsW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSGetSizeW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSGetStartPositionW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSGetTitleW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSGetVersionW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSRemoveTagFromFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSRemoveTagW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSSaveChangesToFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function LYRICSSaveChangesW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub LYRICSSetAlbumW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub LYRICSSetArtistW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub LYRICSSetAuthorW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub LYRICSSetGenreW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub LYRICSSetImageLinkW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub LYRICSSetIndicationW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub LYRICSSetInformationW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub LYRICSSetLyricsW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub LYRICSSetTitleW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MONKEYGetBitsW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MONKEYGetCompressionRatioW() As Single
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MONKEYGetCompressionW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MONKEYGetFramesW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MONKEYGetPeakW() As Single
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MONKEYGetSamplesPerFrameW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MONKEYGetSamplesW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub MP4DeleteEntriesW()
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub MP4DeletePictureW(ByVal Index As Short)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub MP4DeletePicturesW()
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4GetTextFrameW(ByVal FrameID As MP4FRAMES) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4GetAllFrameIDsW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4GetGenreW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4GetiTuneFrameW(ByVal FrameID As String) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4GetPictureArrayW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4GetPictureCountW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4GetPictureFileW(ByVal FileName As String, ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4GetPictureMimeW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4GetTrackW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4GetVersionW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4SaveChangesToFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4SaveChangesW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub MP4SetGenreW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub MP4SetiTuneFrameW(ByVal FrameID As String, ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4AddPictureArrayW(ByVal arr As Byte(), ByVal maxLen As Integer) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4AddPictureFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub MP4SetTextFrameW(ByVal FrameID As MP4FRAMES, ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub MP4SetTrackW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGGetEmphasisW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGGetEncoderW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGGetFramePositionW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGGetFrameSizeW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGGetFramesW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGGetLayerW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGGetVersionW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGIsCopyrightedW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGIsOriginalW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGIsPaddingW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGIsPrivateW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGIsProtectedW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGIsVBRW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGSetCopyrightedW(ByVal FileName As String, ByVal newValue As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGSetOriginalW(ByVal FileName As String, ByVal newValue As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPEGSetPrivateW(ByVal FileName As String, ByVal newValue As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPPGetFramesW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MPPGetStreamVersionW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetAlbumW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetArtistW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetBitRateNominalW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetCommentW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetCopyrightW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetDateW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetDescriptionW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetGenreW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetISRCW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetItemKeysW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetLicenseW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetLocationW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetOrganizationW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetPerformerW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetTitleW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetTrackW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetUserItemW(ByVal ItemKey As String) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetVendorW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetVersionW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGRemoveTagFromFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGRemoveTagW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGSaveChangesToFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGSaveChangesW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetAlbumW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetArtistW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetCommentW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetCopyrightW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetDateW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetDescriptionW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetGenreW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetISRCW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetLicenseW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetLocationW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetOrganizationW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetPerformerW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetTitleW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetTrackW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetUserItemW(ByVal ItemKey As String, ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetVendorW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetVersionW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function OGGGetContactW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub OGGSetContactW(ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub SetConfigValueW(ByVal key As Integer, ByVal value As Integer)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub SetLogFileW(ByVal fileName As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WAVGetBitsPerSampleW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WAVGetBlockAlignW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WAVGetBytesPerSecondW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WAVGetCartChunkEntryW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WAVGetChannelsW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WAVGetFormatIDW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WAVGetFormatW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WAVGetHeaderSizeW() As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WAVGetInfoChunkIDsW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WAVGetTextFrameW(ByVal FrameID As WAVFRAMES) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WAVSaveChangesW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WAVSaveChangesToFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WAVSetCartChunkEntryW(ByVal Index As Short, ByVal newValue As String) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub WAVSetTextFrameW(ByVal FrameID As WAVFRAMES, ByVal textString As String)
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMAAddPictureArrayW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Description As String, ByVal PictureType As Short, ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMAAddPictureFileW(ByVal FileName As String, ByVal Description As String, ByVal PictureType As Short, ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMADeletePictureW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub WMADeletePicturesW()
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMAGetItemKeysW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMAGetPictureArrayW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMAGetPictureDescriptionW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMAGetPictureFileW(ByVal FileName As String, ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMAGetPictureMimeW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMAGetPictureTypeW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMAGetPictureCountW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMAGetUserItemW(ByVal ItemKey As String) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMAIsVBRW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMASaveChangesToFileW(ByVal FileName As String) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMASaveChangesW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub WMASetUserItemW(ByVal ItemKey As String, ByVal textString As String)
        End Sub
        ' New methods since v2.0.2.0
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACAddPictureArrayW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Description As String, ByVal PictureType As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACAddPictureFileW(ByVal FileName As String, ByVal Description As String, ByVal PictureType As Short, ByVal asLink As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACDeletePictureW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub FLACDeletePicturesW()
        End Sub
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetPictureArrayW(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetPictureCountW() As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetPictureDescriptionW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetPictureFileW(ByVal FileName As String, ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetPictureMimeW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetPictureSizeW(ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetPictureTypeTextW(ByVal Index As Short) As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function FLACGetPictureTypeW(ByVal Index As Short) As Short
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function ID3V2GetPictureSizeW(ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function MP4GetPictureSizeW(ByVal Index As Short) As Integer
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WMAGetPictureSizeW(ByVal Index As Short) As Integer
        End Function
        ' New methods since V2.0.3.0
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Function WAVGetDisplayTextW() As String
        End Function
        <DllImport("AudioGenie3.dll", CharSet:=CharSet.Unicode, SetLastError:=True, ExactSpelling:=True)> _
        Private Sub WAVSetDisplayTextW(ByVal textString As String)
        End Sub

#End Region

        Public PictureTypeText As String() = {"Other", "File icon", "Other file icon", "Cover front", "Cover back", "Leaflet page", _
         "media label", "Lead Artist", "Artist Performer", "Conductor", "Band Orchestra", "Composer", _
         "Lyricist", "Recording Location", "During Recording", "During Performance", "Movie video screen capture", "A bright coloured fish", _
         "Illustration", "Band artist logotype", "Publisher studio logotype"}


#Region "APE Propertiers"

        Public Property APEAlbum() As String
            Get
                Return APEGetAlbumW()
            End Get
            Set(ByVal value As String)
                APESetAlbumW(value)
            End Set
        End Property
        Public Property APEArtist() As String
            Get
                Return APEGetArtistW()
            End Get
            Set(ByVal value As String)
                APESetArtistW(value)
            End Set
        End Property
        Public Property APEComment() As String
            Get
                Return APEGetCommentW()
            End Get
            Set(ByVal value As String)
                APESetCommentW(value)
            End Set
        End Property
        Public Property APECopyright() As String
            Get
                Return APEGetCopyrightW()
            End Get
            Set(ByVal value As String)
                APESetCopyrightW(value)
            End Set
        End Property
        Public Property APEGenre() As String
            Get
                Return APEGetGenreW()
            End Get
            Set(ByVal value As String)
                APESetGenreW(value)
            End Set
        End Property
        Public Property APETitle() As String
            Get
                Return APEGetTitleW()
            End Get
            Set(ByVal value As String)
                APESetTitleW(value)
            End Set
        End Property
        Public Property APETrack() As String
            Get
                Return APEGetTrackW()
            End Get
            Set(ByVal value As String)
                APESetTrackW(value)
            End Set
        End Property
        Public Property APEYear() As String
            Get
                Return APEGetYearW()
            End Get
            Set(ByVal value As String)
                APESetYearW(value)
            End Set
        End Property

#End Region
#Region "Audio Properties"

        Public Property AUDIOAlbum() As String
            Get
                Return AUDIOGetAlbumW()
            End Get
            Set(ByVal value As String)
                AUDIOSetAlbumW(value)
            End Set
        End Property
        Public Property AUDIOArtist() As String
            Get
                Return AUDIOGetArtistW()
            End Get
            Set(ByVal value As String)
                AUDIOSetArtistW(value)
            End Set
        End Property
        Public Property AUDIOComment() As String
            Get
                Return AUDIOGetCommentW()
            End Get
            Set(ByVal value As String)
                AUDIOSetCommentW(value)
            End Set
        End Property
        Public Property AUDIOComposer() As String
            Get
                Return AUDIOGetComposerW()
            End Get
            Set(ByVal value As String)
                AUDIOSetComposerW(value)
            End Set
        End Property
        Public Property AUDIOGenre() As String
            Get
                Return AUDIOGetGenreW()
            End Get
            Set(ByVal value As String)
                AUDIOSetGenreW(value)
            End Set
        End Property
        Public Property AUDIOTitle() As String
            Get
                Console.WriteLine(AUDIOGetTitleW())
                Return AUDIOGetTitleW()
            End Get
            Set(ByVal value As String)
                AUDIOSetTitleW(value)
            End Set
        End Property
        Public Property AUDIOTrack() As String
            Get
                Return AUDIOGetTrackW()
            End Get
            Set(ByVal value As String)
                AUDIOSetTrackW(value)
            End Set
        End Property
        Public Property AUDIOYear() As String
            Get
                Return AUDIOGetYearW()
            End Get
            Set(ByVal value As String)
                AUDIOSetYearW(value)
            End Set
        End Property
#End Region

#Region "FLAC Properties"
        Public Property FLACAlbum() As String
            Get
                Return FLACGetUserItemW("ALBUM")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("ALBUM", value)
            End Set
        End Property
        Public Property FLACArranger() As String
            Get
                Return FLACGetUserItemW("ARRANGER")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("ARRANGER", value)
            End Set
        End Property
        Public Property FLACArtist() As String
            Get
                Return FLACGetUserItemW("ARTIST")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("ARTIST", value)
            End Set
        End Property
        Public Property FLACAuthor() As String
            Get
                Return FLACGetUserItemW("AUTHOR")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("AUTHOR", value)
            End Set
        End Property
        Public Property FLACComment() As String
            Get
                Return FLACGetUserItemW("COMMENT")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("COMMENT", value)
            End Set
        End Property
        Public Property FLACComposer() As String
            Get
                Return FLACGetUserItemW("COMPOSER")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("COMPOSER", value)
            End Set
        End Property
        Public Property FLACContact() As String
            Get
                Return FLACGetUserItemW("CONTACT")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("CONTACT", value)
            End Set
        End Property
        Public Property FLACCopyright() As String
            Get
                Return FLACGetUserItemW("COPYRIGHT")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("COPYRIGHT", value)
            End Set
        End Property
        Public Property FLACDate() As String
            Get
                Return FLACGetUserItemW("DATE")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("DATE", value)
            End Set
        End Property
        Public Property FLACDescription() As String
            Get
                Return FLACGetUserItemW("DESCRIPTION")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("DESCRIPTION", value)
            End Set
        End Property
        Public Property FLACDiscNumber() As String
            Get
                Return FLACGetUserItemW("DISCNUMBER")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("DISCNUMBER", value)
            End Set
        End Property
        Public Property FLACEnsemble() As String
            Get
                Return FLACGetUserItemW("ENSEMBLE")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("ENSEMBLE", value)
            End Set
        End Property
        Public Property FLACGenre() As String
            Get
                Return FLACGetUserItemW("GENRE")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("GENRE", value)
            End Set
        End Property
        Public Property FLACISRC() As String
            Get
                Return FLACGetUserItemW("ISRC")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("ISRC", value)
            End Set
        End Property
        Public Property FLACLicense() As String
            Get
                Return FLACGetUserItemW("LICENSE")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("LICENSE", value)
            End Set
        End Property
        Public Property FLACLocation() As String
            Get
                Return FLACGetUserItemW("LOCATION")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("LOCATION", value)
            End Set
        End Property
        Public Property FLACLyricist() As String
            Get
                Return FLACGetUserItemW("LYRICIST")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("LYRICIST", value)
            End Set
        End Property
        Public Property FLACOrganization() As String
            Get
                Return FLACGetUserItemW("ORGANIZATION")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("ORGANIZATION", value)
            End Set
        End Property
        Public Property FLACPerformer() As String
            Get
                Return FLACGetUserItemW("PERFORMER")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("PERFORMER", value)
            End Set
        End Property
        Public Property FLACPublisher() As String
            Get
                Return FLACGetUserItemW("PUBLISHER")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("PUBLISHER", value)
            End Set
        End Property
        Public Property FLACTitle() As String
            Get
                Return FLACGetUserItemW("TITLE")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("TITLE", value)
            End Set
        End Property
        Public Property FLACTrack() As String
            Get
                Return FLACGetUserItemW("TRACKNUMBER")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("TRACKNUMBER", value)
            End Set
        End Property
        Public Property FLACVendor() As String
            Get
                Return FLACGetVendorW()
            End Get
            Set(ByVal value As String)
                FLACSetVendorW(value)
            End Set
        End Property
        Public Property FLACVersion() As String
            Get
                Return FLACGetUserItemW("VERSION")
            End Get
            Set(ByVal value As String)
                FLACSetUserItemW("VERSION", value)
            End Set
        End Property
#End Region

#Region "ID3V1 Properties"
        Public Property ID3V1Album() As String
            Get
                Return ID3V1GetAlbumW()
            End Get
            Set(ByVal value As String)
                ID3V1SetAlbumW(value)
            End Set
        End Property
        Public Property ID3V1Artist() As String
            Get
                Return ID3V1GetArtistW()
            End Get
            Set(ByVal value As String)
                ID3V1SetArtistW(value)
            End Set
        End Property
        Public Property ID3V1Comment() As String
            Get
                Return ID3V1GetCommentW()
            End Get
            Set(ByVal value As String)
                ID3V1SetCommentW(value)
            End Set
        End Property
        Public Property ID3V1GenreID() As Short
            Get
                Return ID3V1GetGenreIDW()
            End Get
            Set(ByVal value As Short)
                ID3V1SetGenreIDW(value)
            End Set
        End Property
        Public Property ID3V1Genre() As String
            Get
                Return ID3V1GetGenreW()
            End Get
            Set(ByVal value As String)
                ID3V1SetGenreW(value)
            End Set
        End Property
        Public Property ID3V1Title() As String
            Get
                Return ID3V1GetTitleW()
            End Get
            Set(ByVal value As String)
                ID3V1SetTitleW(value)
            End Set
        End Property
        Public Property ID3V1Track() As String
            Get
                Return ID3V1GetTrackW()
            End Get
            Set(ByVal value As String)
                ID3V1SetTrackW(value)
            End Set
        End Property
        Public Property ID3V1Year() As String
            Get
                Return ID3V1GetYearW()
            End Get
            Set(ByVal value As String)
                ID3V1SetYearW(value)
            End Set
        End Property
#End Region

#Region "ID3V2 Properties"
        Public Property ID3V2Album() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TALB)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TALB, value)
            End Set
        End Property
        Public Property ID3V2AlbumArtistSort() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TSO2)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TSO2, value)
            End Set
        End Property
        Public Property ID3V2AlbumSort() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TSOA)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TSOA, value)
            End Set
        End Property
        Public Property ID3V2Artist() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TPE1)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TPE1, value)
            End Set
        End Property
        Public Property ID3V2ArtistSort() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TSOP)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TSOP, value)
            End Set
        End Property
        Public Property ID3V2ArtistURL() As String
            Get
                Return ID3V2GetURLFrame(ID3V2FRAMES.ID3F_WOAR)
            End Get
            Set(ByVal value As String)
                ID3V2SetURLFrame(ID3V2FRAMES.ID3F_WOAR, value)
            End Set
        End Property
        Public Property ID3V2AudiofileURL() As String
            Get
                Return ID3V2GetURLFrame(ID3V2FRAMES.ID3F_WOAF)
            End Get
            Set(ByVal value As String)
                ID3V2SetURLFrame(ID3V2FRAMES.ID3F_WOAF, value)
            End Set
        End Property
        Public Property ID3V2AudiosourceURL() As String
            Get
                Return ID3V2GetURLFrame(ID3V2FRAMES.ID3F_WOAS)
            End Get
            Set(ByVal value As String)
                ID3V2SetURLFrame(ID3V2FRAMES.ID3F_WOAS, value)
            End Set
        End Property
        Public Property ID3V2BeatsPerMinute() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TBPM)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TBPM, value)
            End Set
        End Property
        Public Property ID3V2Category() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TIT1)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TIT1, value)
            End Set
        End Property
        Public Property ID3V2CommercialURL() As String
            Get
                Return ID3V2GetURLFrame(ID3V2FRAMES.ID3F_WCOM)
            End Get
            Set(ByVal value As String)
                ID3V2SetURLFrame(ID3V2FRAMES.ID3F_WCOM, value)
            End Set
        End Property
        Public Property ID3V2Composer() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TCOM)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TCOM, value)
            End Set
        End Property
        Public Property ID3V2ComposerSort() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TSOC)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TSOC, value)
            End Set
        End Property
        Public Property ID3V2ContentType() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TCON)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TCON, value)
            End Set
        End Property
        Public Property ID3V2Conductor() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TPE3)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TPE3, value)
            End Set
        End Property
        Public Property ID3V2Copyright() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TCOP)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TCOP, value)
            End Set
        End Property
        Public Property ID3V2CopyrightURL() As String
            Get
                Return ID3V2GetURLFrame(ID3V2FRAMES.ID3F_WCOP)
            End Get
            Set(ByVal value As String)
                ID3V2SetURLFrame(ID3V2FRAMES.ID3F_WCOP, value)
            End Set
        End Property
        Public Property ID3V2Date() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TDAT)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TDAT, value)
            End Set
        End Property
        Public Property ID3V2Encoder() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TENC)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TENC, value)
            End Set
        End Property
        Public Property ID3V2EncodingSettings() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TSSE)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TSSE, value)
            End Set
        End Property
        Public Property ID3V2EncodingTime() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TDEN)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TDEN, value)
            End Set
        End Property
        Public Property ID3V2FileOwner() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TOWN)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TOWN, value)
            End Set
        End Property
        Public Property ID3V2FileType() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TFLT)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TFLT, value)
            End Set
        End Property
        Public Property ID3V2Genre() As String
            Get
                Return ID3V2GetGenreW()
            End Get
            Set(ByVal value As String)
                ID3V2SetGenreW(value)
            End Set
        End Property
        Public Property ID3V2InetRadioName() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TRSN)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TRSN, value)
            End Set
        End Property
        Public Property ID3V2InetRadioOwner() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TRSO)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TRSO, value)
            End Set
        End Property
        Public Property ID3V2InitialKey() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TKEY)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TKEY, value)
            End Set
        End Property
        Public Property ID3V2Interpreted() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TPE4)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TPE4, value)
            End Set
        End Property
        Public Property ID3V2InvolvedPeopleList() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TIPL)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TIPL, value)
            End Set
        End Property
        Public Property ID3V2ISRC() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TSRC)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TSRC, value)
            End Set
        End Property
        Public Property ID3V2Language() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TLAN)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TLAN, value)
            End Set
        End Property
        Public Property ID3V2Length() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TLEN)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TLEN, value)
            End Set
        End Property
        Public Property ID3V2MediaType() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TMED)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TMED, value)
            End Set
        End Property
        Public Property ID3V2Mood() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TMOO)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TMOO, value)
            End Set
        End Property
        Public Property ID3V2MusicianCreditsList() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TMCL)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TMCL, value)
            End Set
        End Property
        Public Property ID3V2Orchestra() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TPE2)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TPE2, value)
            End Set
        End Property
        Public Property ID3V2OriginalAlbum() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TOAL)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TOAL, value)
            End Set
        End Property
        Public Property ID3V2OriginalArtist() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TOPE)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TOPE, value)
            End Set
        End Property
        Public Property ID3V2OriginalFilename() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TOFN)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TOFN, value)
            End Set
        End Property
        Public Property ID3V2OriginalReleaseTime() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TDOR)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TDOR, value)
            End Set
        End Property
        Public Property ID3V2OriginalReleaseYear() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TORY)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TORY, value)
            End Set
        End Property
        Public Property ID3V2OriginalTextwriter() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TOLY)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TOLY, value)
            End Set
        End Property
        Public Property ID3V2PartofSet() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TPOS)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TPOS, value)
            End Set
        End Property
        Public Property ID3V2PaymentURL() As String
            Get
                Return ID3V2GetURLFrame(ID3V2FRAMES.ID3F_WPAY)
            End Get
            Set(ByVal value As String)
                ID3V2SetURLFrame(ID3V2FRAMES.ID3F_WPAY, value)
            End Set
        End Property
        Public Property ID3V2PlayCounter() As Integer
            Get
                Return ID3V2GetPlayCounterW()
            End Get
            Set(ByVal value As Integer)
                ID3V2SetPlayCounterW(value)
            End Set
        End Property
        Public Property ID3V2PlaylistDelay() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TDLY)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TDLY, value)
            End Set
        End Property
        Public Property ID3V2ProducedNotice() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TPRO)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TPRO, value)
            End Set
        End Property
        Public Property ID3V2Publisher() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TPUB)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TPUB, value)
            End Set
        End Property
        Public Property ID3V2PublisherURL() As String
            Get
                Return ID3V2GetURLFrame(ID3V2FRAMES.ID3F_WPUB)
            End Get
            Set(ByVal value As String)
                ID3V2SetURLFrame(ID3V2FRAMES.ID3F_WPUB, value)
            End Set
        End Property
        Public Property ID3V2RadiostationURL() As String
            Get
                Return ID3V2GetURLFrame(ID3V2FRAMES.ID3F_WORS)
            End Get
            Set(ByVal value As String)
                ID3V2SetURLFrame(ID3V2FRAMES.ID3F_WORS, value)
            End Set
        End Property
        Public Property ID3V2RecordingDate() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TRDA)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TRDA, value)
            End Set
        End Property
        Public Property ID3V2RecordingTime() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TDRC)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TDRC, value)
            End Set
        End Property
        Public Property ID3V2ReleaseTime() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TDRL)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TDRL, value)
            End Set
        End Property
        Public Property ID3V2Size() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TSIZ)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TSIZ, value)
            End Set
        End Property
        Public Property ID3V2Subtitle() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TIT3)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TIT3, value)
            End Set
        End Property
        Public Property ID3V2SubtitleSet() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TSST)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TSST, value)
            End Set
        End Property
        Public Property ID3V2TaggingTime() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TDTG)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TDTG, value)
            End Set
        End Property
        Public Property ID3V2TextWriter() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TEXT)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TEXT, value)
            End Set
        End Property
        Public Property ID3V2Time() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TIME)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TIME, value)
            End Set
        End Property
        Public Property ID3V2Title() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TIT2)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TIT2, value)
            End Set
        End Property
        Public Property ID3V2TitleSort() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TSOT)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TSOT, value)
            End Set
        End Property
        Public Property ID3V2Track() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TRCK)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TRCK, value)
            End Set
        End Property
        Public Property ID3V2Year() As String
            Get
                Return ID3V2GetTextFrame(ID3V2FRAMES.ID3F_TYER)
            End Get
            Set(ByVal value As String)
                ID3V2SetTextFrame(ID3V2FRAMES.ID3F_TYER, value)
            End Set
        End Property
#End Region

#Region "LYRICS Properties"
        Public Property LYRICSAlbum() As String
            Get
                Return LYRICSGetAlbumW()
            End Get
            Set(ByVal value As String)
                LYRICSSetAlbumW(value)
            End Set
        End Property
        Public Property LYRICSArtist() As String
            Get
                Return LYRICSGetArtistW()
            End Get
            Set(ByVal value As String)
                LYRICSSetArtistW(value)
            End Set
        End Property
        Public Property LYRICSAuthor() As String
            Get
                Return LYRICSGetAuthorW()
            End Get
            Set(ByVal value As String)
                LYRICSSetAuthorW(value)
            End Set
        End Property
        Public Property LYRICSGenre() As String
            Get
                Return LYRICSGetGenreW()
            End Get
            Set(ByVal value As String)
                LYRICSSetGenreW(value)
            End Set
        End Property
        Public Property LYRICSImageLink() As String
            Get
                Return LYRICSGetImageLinkW()
            End Get
            Set(ByVal value As String)
                LYRICSSetImageLinkW(value)
            End Set
        End Property
        Public Property LYRICSIndication() As String
            Get
                Return LYRICSGetIndicationW()
            End Get
            Set(ByVal value As String)
                LYRICSSetIndicationW(value)
            End Set
        End Property
        Public Property LYRICSInformation() As String
            Get
                Return LYRICSGetInformationW()
            End Get
            Set(ByVal value As String)
                LYRICSSetInformationW(value)
            End Set
        End Property
        Public Property LYRICSLyrics() As String
            Get
                Return LYRICSGetLyricsW()
            End Get
            Set(ByVal value As String)
                LYRICSSetLyricsW(value)
            End Set
        End Property
        Public Property LYRICSTitle() As String
            Get
                Return LYRICSGetTitleW()
            End Get
            Set(ByVal value As String)
                LYRICSSetTitleW(value)
            End Set
        End Property
#End Region

#Region "MP4 Properties"
        Public Property MP4Album() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_ALBUM)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_ALBUM, value)
            End Set
        End Property
        Public Property MP4Artist() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_ARTIST)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_ARTIST, value)
            End Set
        End Property
        Public Property MP4Author() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_AUTHOR)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_AUTHOR, value)
            End Set
        End Property
        Public Property MP4BeatsPerMinute() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_BEATSPERMINUTE)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_BEATSPERMINUTE, value)
            End Set
        End Property
        Public Property MP4CDSetNumber() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_CDSETNUMBER)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_CDSETNUMBER, value)
            End Set
        End Property
        Public Property MP4Comment() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_COMMENT)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_COMMENT, value)
            End Set
        End Property
        Public Property MP4Compilation() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_COMPILATION)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_COMPILATION, value)
            End Set
        End Property
        Public Property MP4Composer() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_COMPOSER)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_COMPOSER, value)
            End Set
        End Property
        Public Property MP4Copyright() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_COPYRIGHT)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_COPYRIGHT, value)
            End Set
        End Property
        Public Property MP4Date() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_CREATEDATE)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_CREATEDATE, value)
            End Set
        End Property
        Public Property MP4Description() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_DESCRIPTION)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_DESCRIPTION, value)
            End Set
        End Property
        Public Property MP4Director() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_DIRECTOR)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_DIRECTOR, value)
            End Set
        End Property
        Public Property MP4Disclaimer() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_DISCLAIMER)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_DISCLAIMER, value)
            End Set
        End Property
        Public Property MP4Encoder() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_ENCODER)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_ENCODER, value)
            End Set
        End Property
        Public Property MP4Genre() As String
            Get
                Return MP4GetGenreW()
            End Get
            Set(ByVal value As String)
                MP4SetGenreW(value)
            End Set
        End Property
        Public Property MP4NetUrl() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_NETURL)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_NETURL, value)
            End Set
        End Property
        Public Property MP4OriginalArtist() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_ORIGINALARTIST)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_ORIGINALARTIST, value)
            End Set
        End Property
        Public Property MP4OriginalFormat() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_ORIGINALFORMAT)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_ORIGINALFORMAT, value)
            End Set
        End Property
        Public Property MP4OriginalSource() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_ORIGINALSOURCE)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_ORIGINALSOURCE, value)
            End Set
        End Property
        Public Property MP4Performer() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_PERFORMER)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_PERFORMER, value)
            End Set
        End Property
        Public Property MP4Producer() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_PRODUCER)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_PRODUCER, value)
            End Set
        End Property
        Public Property MP4Title() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_TITLE)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_TITLE, value)
            End Set
        End Property
        Public Property MP4Track() As String
            Get
                Return MP4GetTrackW()
            End Get
            Set(ByVal value As String)
                MP4SetTrackW(value)
            End Set
        End Property
        Public Property MP4Warning() As String
            Get
                Return MP4GetTextFrameW(MP4FRAMES.MP4_WARNING)
            End Get
            Set(ByVal value As String)
                MP4SetTextFrameW(MP4FRAMES.MP4_WARNING, value)
            End Set
        End Property
#End Region

#Region "OGG Properties"
        Public Property OGGAlbum() As String
            Get
                Return OGGGetAlbumW()
            End Get
            Set(ByVal value As String)
                OGGSetAlbumW(value)
            End Set
        End Property
        Public Property OGGArtist() As String
            Get
                Return OGGGetArtistW()
            End Get
            Set(ByVal value As String)
                OGGSetArtistW(value)
            End Set
        End Property
        Public Property OGGComment() As String
            Get
                Return OGGGetCommentW()
            End Get
            Set(ByVal value As String)
                OGGSetCommentW(value)
            End Set
        End Property
        Public Property OGGCopyright() As String
            Get
                Return OGGGetCopyrightW()
            End Get
            Set(ByVal value As String)
                OGGSetCopyrightW(value)
            End Set
        End Property
        Public Property OGGDate() As String
            Get
                Return OGGGetDateW()
            End Get
            Set(ByVal value As String)
                OGGSetDateW(value)
            End Set
        End Property
        Public Property OGGDescription() As String
            Get
                Return OGGGetDescriptionW()
            End Get
            Set(ByVal value As String)
                OGGSetDescriptionW(value)
            End Set
        End Property
        Public Property OGGGenre() As String
            Get
                Return OGGGetGenreW()
            End Get
            Set(ByVal value As String)
                OGGSetGenreW(value)
            End Set
        End Property
        Public Property OGGISRC() As String
            Get
                Return OGGGetISRCW()
            End Get
            Set(ByVal value As String)
                OGGSetISRCW(value)
            End Set
        End Property
        Public Property OGGLicense() As String
            Get
                Return OGGGetLicenseW()
            End Get
            Set(ByVal value As String)
                OGGSetLicenseW(value)
            End Set
        End Property
        Public Property OGGLocation() As String
            Get
                Return OGGGetLocationW()
            End Get
            Set(ByVal value As String)
                OGGSetLocationW(value)
            End Set
        End Property
        Public Property OGGOrganization() As String
            Get
                Return OGGGetOrganizationW()
            End Get
            Set(ByVal value As String)
                OGGSetOrganizationW(value)
            End Set
        End Property
        Public Property OGGPerformer() As String
            Get
                Return OGGGetPerformerW()
            End Get
            Set(ByVal value As String)
                OGGSetPerformerW(value)
            End Set
        End Property
        Public Property OGGTitle() As String
            Get
                Return OGGGetTitleW()
            End Get
            Set(ByVal value As String)
                OGGSetTitleW(value)
            End Set
        End Property
        Public Property OGGTrack() As String
            Get
                Return OGGGetTrackW()
            End Get
            Set(ByVal value As String)
                OGGSetTrackW(value)
            End Set
        End Property
        Public Property OGGVendor() As String
            Get
                Return OGGGetVendorW()
            End Get
            Set(ByVal value As String)
                OGGSetVendorW(value)
            End Set
        End Property
        Public Property OGGVersion() As String
            Get
                Return OGGGetVersionW()
            End Get
            Set(ByVal value As String)
                OGGSetVersionW(value)
            End Set
        End Property
        Public Property OGGContact() As String
            Get
                Return OGGGetContactW()
            End Get
            Set(ByVal value As String)
                OGGSetContactW(value)
            End Set
        End Property
#End Region

#Region "WAV Properties"
        Public Property WAVArchivalLocation() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_IARL)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_IARL, value)
            End Set
        End Property
        Public Property WAVArtist() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_IART)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_IART, value)
            End Set
        End Property
        Public Property WAVComissioned() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_ICMS)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_ICMS, value)
            End Set
        End Property
        Public Property WAVComment() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_ICMT)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_ICMT, value)
            End Set
        End Property
        Public Property WAVCopyright() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_ICOP)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_ICOP, value)
            End Set
        End Property
        Public Property WAVCreationDate() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_ICRD)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_ICRD, value)
            End Set
        End Property
        Public Property WAVCropped() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_ICRP)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_ICRP, value)
            End Set
        End Property
        Public Property WAVDimension() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_IDIM)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_IDIM, value)
            End Set
        End Property
        Public Property WAVDisplayText() As String
            Get
                Return WAVGetDisplayTextW()
            End Get
            Set(ByVal value As String)
                WAVSetDisplayTextW(value)
            End Set
        End Property
        Public Property WAVDotsPerInch() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_IDPI)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_IDPI, value)
            End Set
        End Property
        Public Property WAVEngineer() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_IENG)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_IENG, value)
            End Set
        End Property
        Public Property WAVGenre() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_IGNR)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_IGNR, value)
            End Set
        End Property
        Public Property WAVKeywords() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_IKEY)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_IKEY, value)
            End Set
        End Property
        Public Property WAVLightness() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_ILGT)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_ILGT, value)
            End Set
        End Property
        Public Property WAVMedium() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_IMED)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_IMED, value)
            End Set
        End Property
        Public Property WAVName() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_INAM)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_INAM, value)
            End Set
        End Property
        Public Property WAVPaletteSetting() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_IPLT)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_IPLT, value)
            End Set
        End Property
        Public Property WAVProduct() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_IPRD)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_IPRD, value)
            End Set
        End Property
        Public Property WAVSharpness() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_ISHP)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_ISHP, value)
            End Set
        End Property
        Public Property WAVSoftware() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_ISFT)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_ISFT, value)
            End Set
        End Property
        Public Property WAVSource() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_ISRC)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_ISRC, value)
            End Set
        End Property
        Public Property WAVSourceForm() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_ISRF)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_ISRF, value)
            End Set
        End Property
        Public Property WAVSubject() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_ISBJ)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_ISBJ, value)
            End Set
        End Property
        Public Property WAVTechnician() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_ITCH)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_ITCH, value)
            End Set
        End Property
        Public Property WAVTrack() As String
            Get
                Return WAVGetTextFrame(WAVFRAMES.WAV_ITRK)
            End Get
            Set(ByVal value As String)
                WAVSetTextFrame(WAVFRAMES.WAV_ITRK, value)
            End Set
        End Property
#End Region

#Region "WMA Properties"
        Public Property WMAAlbumArtist() As String
            Get
                Return WMAGetUserItem(WM_ALBUMARTIST)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_ALBUMARTIST, value)
            End Set
        End Property
        Public Property WMAAlbum() As String
            Get
                Return WMAGetUserItem(WM_ALBUMTITLE)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_ALBUMTITLE, value)
            End Set
        End Property
        Public Property WMAArtist() As String
            Get
                Return WMAGetUserItem(WM_AUTHOR)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_AUTHOR, value)
            End Set
        End Property
        Public Property WMAAuthorURL() As String
            Get
                Return WMAGetUserItem(WM_AUTHORURL)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_AUTHORURL, value)
            End Set
        End Property
        Public Property WMABeatsPerMinute() As String
            Get
                Return WMAGetUserItem(WM_BEATSPERMINUTE)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_BEATSPERMINUTE, value)
            End Set
        End Property
        Public Property WMACategory() As String
            Get
                Return WMAGetUserItem(WM_CATEGORY)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_CATEGORY, value)
            End Set
        End Property
        Public Property WMAComment() As String
            Get
                Return WMAGetUserItem(WM_DESCRIPTION)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_DESCRIPTION, value)
            End Set
        End Property
        Public Property WMAComposer() As String
            Get
                Return WMAGetUserItem(WM_COMPOSER)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_COMPOSER, value)
            End Set
        End Property
        Public Property WMAConductor() As String
            Get
                Return WMAGetUserItem(WM_CONDUCTOR)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_CONDUCTOR, value)
            End Set
        End Property
        Public Property WMAContentGroupDescription() As String
            Get
                Return WMAGetUserItem(WM_CONTENTGROUPDESCRIPTION)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_CONTENTGROUPDESCRIPTION, value)
            End Set
        End Property
        Public Property WMACopyright() As String
            Get
                Return WMAGetUserItem(WM_PROVIDERCOPYRIGHT)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_PROVIDERCOPYRIGHT, value)
            End Set
        End Property
        Public Property WMADistributor() As String
            Get
                Return WMAGetUserItem(WM_CONTENTDISTRIBUTOR)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_CONTENTDISTRIBUTOR, value)
            End Set
        End Property
        Public Property WMAEncodedBy() As String
            Get
                Return WMAGetUserItem(WM_ENCODEDBY)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_ENCODEDBY, value)
            End Set
        End Property
        Public Property WMAEncodingSettings() As String
            Get
                Return WMAGetUserItem(WM_ENCODINGSETTINGS)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_ENCODINGSETTINGS, value)
            End Set
        End Property
        Public Property WMAEncodingTime() As String
            Get
                Return WMAGetUserItem(WM_ENCODINGTIME)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_ENCODINGTIME, value)
            End Set
        End Property
        Public Property WMAGenre() As String
            Get
                Return WMAGetUserItem(WM_GENRE)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_GENRE, value)
            End Set
        End Property
        Public Property WMAISRC() As String
            Get
                Return WMAGetUserItem(WM_ISRC)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_ISRC, value)
            End Set
        End Property
        Public Property WMAInitialKey() As String
            Get
                Return WMAGetUserItem(WM_INITIALKEY)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_INITIALKEY, value)
            End Set
        End Property
        Public Property WMALanguage() As String
            Get
                Return WMAGetUserItem(WM_LANGUAGE)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_LANGUAGE, value)
            End Set
        End Property
        Public Property WMALyrics() As String
            Get
                Return WMAGetUserItem(WM_LYRICS)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_LYRICS, value)
            End Set
        End Property
        Public Property WMAMood() As String
            Get
                Return WMAGetUserItem(WM_MOOD)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_MOOD, value)
            End Set
        End Property
        Public Property WMAParentalRating() As String
            Get
                Return WMAGetUserItem(WM_PARENTALRATING)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_PARENTALRATING, value)
            End Set
        End Property
        Public Property WMAPartOfSet() As String
            Get
                Return WMAGetUserItem(WM_PARTOFSET)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_PARTOFSET, value)
            End Set
        End Property
        Public Property WMAPeriod() As String
            Get
                Return WMAGetUserItem(WM_PERIOD)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_PERIOD, value)
            End Set
        End Property
        Public Property WMAProducer() As String
            Get
                Return WMAGetUserItem(WM_PRODUCER)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_PRODUCER, value)
            End Set
        End Property
        Public Property WMAPromotionURL() As String
            Get
                Return WMAGetUserItem(WM_PROMOTIONURL)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_PROMOTIONURL, value)
            End Set
        End Property
        Public Property WMAProvider() As String
            Get
                Return WMAGetUserItem(WM_PROVIDER)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_PROVIDER, value)
            End Set
        End Property
        Public Property WMAPublisher() As String
            Get
                Return WMAGetUserItem(WM_PUBLISHER)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_PUBLISHER, value)
            End Set
        End Property
        Public Property WMARating() As String
            Get
                Return WMAGetUserItem(WM_PROVIDERRATING)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_PROVIDERRATING, value)
            End Set
        End Property
        Public Property WMATitle() As String
            Get
                Return WMAGetUserItem(WM_TITLE)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_TITLE, value)
            End Set
        End Property
        Public Property WMAToolName() As String
            Get
                Return WMAGetUserItem(WM_TOOLNAME)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_TOOLNAME, value)
            End Set
        End Property
        Public Property WMAToolVersion() As String
            Get
                Return WMAGetUserItem(WM_TOOLVERSION)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_TOOLVERSION, value)
            End Set
        End Property
        Public Property WMATrackNumber() As String
            Get
                Return WMAGetUserItem(WM_TRACKNUMBER)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_TRACKNUMBER, value)
            End Set
        End Property
        Public Property WMAUserWebURL() As String
            Get
                Return WMAGetUserItem(WM_USERWEBURL)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_USERWEBURL, value)
            End Set
        End Property
        Public Property WMAWriter() As String
            Get
                Return WMAGetUserItem(WM_WRITER)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_WRITER, value)
            End Set
        End Property
        Public Property WMAYear() As String
            Get
                Return WMAGetUserItem(WM_YEAR)
            End Get
            Set(ByVal value As String)
                WMASetUserItem(WM_YEAR, value)
            End Set
        End Property
#End Region

#Region "AAC Methods"
        Public Function AACGetBitRateType() As String
            Return AACGetBitRateTypeW()
        End Function
        Public Function AACGetHeaderType() As String
            Return AACGetHeaderTypeW()
        End Function
        Public Function AACGetProfile() As String
            Return AACGetProfileW()
        End Function
#End Region

#Region "APE Methods"
        Public Function APEExists() As Boolean
            Return Convert.ToBoolean(APEExistsW())
        End Function
        Public Function APEGetItemKeys() As String
            Return APEGetItemKeysW()
        End Function
        Public Function APEGetSize() As Integer
            Return APEGetSizeW()
        End Function
        Public Function APEGetUserItem(ByVal Key As String) As String
            Return APEGetUserItemW(Key)
        End Function
        Public Function APEGetVersion() As String
            Return APEGetVersionW()
        End Function
        Public Function APERemoveTagFromFile(ByVal FileName As String) As Boolean
            Return Convert.ToBoolean(APERemoveTagFromFileW(FileName))
        End Function
        Public Function APERemoveTag() As Boolean
            Return Convert.ToBoolean(APERemoveTagW())
        End Function
        Public Function APESaveChangesToFile(ByVal FileName As String) As Boolean
            Return Convert.ToBoolean(APESaveChangesToFileW(FileName))
        End Function
        Public Function APESaveChanges() As Boolean
            Return Convert.ToBoolean(APESaveChangesW())
        End Function
        Public Sub APESetUserItem(ByVal Key As String, ByVal textString As String)
            APESetUserItemW(Key, textString)
        End Sub
#End Region

#Region "AUDIO Methods"
        Public Function AUDIOAnalyzeFile(ByVal FileName As String) As AudioFormatID
            Return DirectCast(AUDIOAnalyzeFileW(FileName), AudioFormatID)
        End Function
        Public Function AUDIOIsValidFormat() As Boolean
            Return Convert.ToBoolean(AUDIOIsValidFormatW())
        End Function
        Public Function AUDIOGetBitrate() As Integer
            Return AUDIOGetBitrateW()
        End Function
        Public Function AUDIOGetChannelMode() As String
            Return AUDIOGetChannelModeW()
        End Function
        Public Function AUDIOGetChannels() As Integer
            Return AUDIOGetChannelsW()
        End Function
        Public Function AUDIOGetDurationMillis() As Integer
            Return AUDIOGetDurationMillisW()
        End Function
        Public Function AUDIOGetDuration() As Single
            Return AUDIOGetDurationW()
        End Function
        Public Function AUDIOGetFileSize() As Integer
            Return AUDIOGetFileSizeW()
        End Function
        Public Function AUDIOGetLastErrorNumber() As Integer
            Return AUDIOGetLastErrorNumberW()
        End Function
        Public Function AUDIOGetLastErrorText() As String
            Return AUDIOGetLastErrorTextW()
        End Function
        Public Function AUDIOGetLastFile() As String
            Return AUDIOGetLastFileW()
        End Function
        Public Function AUDIOGetMD5Value() As String
            Return AUDIOGetMD5ValueW()
        End Function
        Public Function AUDIOGetSampleRate() As Integer
            Return AUDIOGetSampleRateW()
        End Function
        Public Function AUDIOGetVersion() As String
            Return AUDIOGetVersionW()
        End Function
        Public Function AUDIOSaveChangesToFile(ByVal FileName As String) As Boolean
            Return Convert.ToBoolean(AUDIOSaveChangesToFileW(FileName))
        End Function
        Public Function AUDIOSaveChanges() As Boolean
            Return Convert.ToBoolean(AUDIOSaveChangesW())
        End Function
#End Region

#Region "AudioGenie Methods"
        Public Function GetConfigValue(ByVal key As Integer) As Integer
            Return GetConfigValueW(key)
        End Function
        Public Function GetAudioGenieVersion() As String
            Return GetAudioGenieVersionW()
        End Function
        Public Sub SetConfigValue(ByVal key As Integer, ByVal value As Integer)
            SetConfigValueW(key, value)
        End Sub
        Public Sub SetLogFile(ByVal fileName As String)
            SetLogFileW(fileName)
        End Sub
#End Region

#Region "FLAC Methods"
        Public Function FLACAddPictureArray(ByVal arr As Byte(), ByVal Description As String, ByVal PictureType As Short) As Short
            Return FLACAddPictureArrayW(arr, arr.Length, Description, PictureType)
        End Function
        Public Function FLACAddPictureFile(ByVal FileName As String, ByVal Description As String, ByVal PictureType As Short, ByVal asLink As Short) As Short
            Return FLACAddPictureFileW(FileName, Description, PictureType, asLink)
        End Function
        Public Function FLACDeletePicture(ByVal Index As Short) As Boolean
            Return Convert.ToBoolean(FLACDeletePictureW(Index))
        End Function
        Public Sub FLACDeletePictures()
            FLACDeletePicturesW()
        End Sub
        Public Function FLACGetBitsPerSample() As Integer
            Return FLACGetBitsPerSampleW()
        End Function
        Public Function FLACGetCompressionRatio() As Single
            Return FLACGetCompressionRatioW()
        End Function
        Public Function FLACGetItemKeys() As String
            Return FLACGetItemKeysW()
        End Function
        Public Function FLACGetMaxBlockSize() As Integer
            Return FLACGetMaxBlockSizeW()
        End Function
        Public Function FLACGetMaxFrameSize() As Integer
            Return FLACGetMaxFrameSizeW()
        End Function
        Public Function FLACGetMinBlockSize() As Integer
            Return FLACGetMinBlockSizeW()
        End Function
        Public Function FLACGetMinFrameSize() As Integer
            Return FLACGetMinFrameSizeW()
        End Function
        Public Function FLACGetPictureArray(ByVal Index As Short) As Byte()
            Dim maxLen As Integer = FLACGetPictureSize(Index)
            Dim dummy As Byte() = New Byte(maxLen) {}
            Dim ln As Integer = FLACGetPictureArrayW(dummy, maxLen, Index)
            If ln <= 0 OrElse ln >= maxLen Then
                ln = 0
            End If
            Return dummy
        End Function
        Public Function FLACGetPictureDescription(ByVal Index As Short) As String
            Return FLACGetPictureDescriptionW(Index)
        End Function
        Public Function FLACGetPictureMime(ByVal Index As Short) As String
            Return FLACGetPictureMimeW(Index)
        End Function
        Public Function FLACGetPictureSize(ByVal Index As Short) As Integer
            Return FLACGetPictureSizeW(Index)
        End Function
        Public Function FLACGetPictureType(ByVal Index As Short) As Short
            Return FLACGetPictureTypeW(Index)
        End Function
        Public Function FLACGetPictureTypeText(ByVal Index As Short) As String
            Return FLACGetPictureTypeTextW(Index)
        End Function
        Public Function FLACGetPictureFile(ByVal FileName As String, ByVal Index As Short) As Short
            Return FLACGetPictureFileW(FileName, Index)
        End Function
        Public Function FLACGetPictureCount() As Short
            Return FLACGetPictureCountW()
        End Function
        Public Function FLACGetSamples() As Integer
            Return FLACGetSamplesW()
        End Function
        Public Function FLACGetUserItem(ByVal ItemKey As String) As String
            Return FLACGetUserItemW(ItemKey)
        End Function
        Public Function FLACSaveChangesToFile(ByVal FileName As String) As Boolean
            Return Convert.ToBoolean(FLACSaveChangesToFileW(FileName))
        End Function
        Public Function FLACSaveChanges() As Boolean
            Return Convert.ToBoolean(FLACSaveChangesW())
        End Function
        Public Sub FLACSetUserItem(ByVal ItemKey As String, ByVal textString As String)
            FLACSetUserItemW(ItemKey, textString)
        End Sub
#End Region

        Public Function GetMD5ValueFromFile(ByVal FileName As String) As String
            Return GetMD5ValueFromFileW(FileName)
        End Function

#Region "ID3V1 Methods"
        Public Function ID3V1Exists() As Boolean
            Return Convert.ToBoolean(ID3V1ExistsW())
        End Function
        Public Function ID3V1GetGenreItem(ByVal number As Short) As String
            Return ID3V1GetGenreItemW(number)
        End Function
        Public Function ID3V1GetGenres() As Short
            Return ID3V1GetGenresW()
        End Function
        Public Function ID3V1GetVersion() As String
            Return ID3V1GetVersionW()
        End Function
        Public Function ID3V1RemoveTagFromFile(ByVal FileName As String) As Boolean
            Return Convert.ToBoolean(ID3V1RemoveTagFromFileW(FileName))
        End Function
        Public Function ID3V1RemoveTag() As Boolean
            Return Convert.ToBoolean(ID3V1RemoveTagW())
        End Function
        Public Function ID3V1SaveChangesToFile(ByVal FileName As String) As Boolean
            Return Convert.ToBoolean(ID3V1SaveChangesToFileW(FileName))
        End Function
        Public Function ID3V1SaveChanges() As Boolean
            Return Convert.ToBoolean(ID3V1SaveChangesW())
        End Function
#End Region

#Region "ID3V2 Methods"
        Public Function ID3V2AddAudioEncryption(ByVal arr As Byte(), ByVal URL As String, ByVal PreviewStart As Short, ByVal PreviewLength As Short) As Short
            Return ID3V2AddAudioEncryptionW(arr, arr.Length, URL, PreviewStart, PreviewLength)
        End Function
        Public Function ID3V2AddAudioSeekPoint(ByVal arr As Byte(), ByVal start As Integer, ByVal length As Integer, ByVal numbers As Short, ByVal BitsPerPoint As Byte) As Short
            Return ID3V2AddAudioSeekPointW(arr, arr.Length, start, length, numbers, BitsPerPoint)
        End Function
        Public Function ID3V2GetSeekOffset() As Integer
            Return ID3V2GetSeekOffsetW()
        End Function
        Public Function ID3V2AddChapter(ByVal ID As String, ByVal Title As String, ByVal Description As String, ByVal startTime As Integer, ByVal endTime As Integer) As Short
            Return ID3V2AddChapterW(ID, Title, Description, startTime, endTime)
        End Function
        Public Function ID3V2AddChildElement(ByVal ParentTocID As String, ByVal ChildID As String) As Short
            Return ID3V2AddChildElementW(ParentTocID, ChildID)
        End Function
        Public Function ID3V2AddComment(ByVal Language As String, ByVal Description As String, ByVal Text As String) As Short
            Return ID3V2AddCommentW(Language, Description, Text)
        End Function
        Public Function ID3V2AddCommercialFrame(ByVal arr As Byte(), ByVal Price As String, ByVal validUntil As String, ByVal contactUrl As String, ByVal receivedAs As Short, ByVal seller As String, _
         ByVal description As String) As Short
            Return ID3V2AddCommercialFrameW(arr, arr.Length, Price, validUntil, contactUrl, receivedAs, _
             seller, description)
        End Function
        Public Function ID3V2AddEncryption(ByVal arr As Byte(), ByVal URL As String, ByVal Symbol As Short) As Short
            Return ID3V2AddEncryptionW(arr, arr.Length, URL, Symbol)
        End Function
        Public Function ID3V2AddEqualisation(ByVal arr As Byte(), ByVal Interpolationmethod As Byte, ByVal Identification As String) As Short
            Return ID3V2AddEqualisationW(arr, arr.Length, Interpolationmethod, Identification)
        End Function
        Public Function ID3V2AddGeneralObject(ByVal arr As Byte(), ByVal Mime As String, ByVal FileName As String, ByVal Description As String) As Short
            Return ID3V2AddGeneralObjectW(arr, arr.Length, Mime, FileName, Description)
        End Function
        Public Function ID3V2AddGroupIdentification(ByVal arr As Byte(), ByVal Url As String, ByVal symbol As Byte) As Short
            Return ID3V2AddGroupIdentificationW(arr, arr.Length, Url, symbol)
        End Function
        Public Function ID3V2AddLinkedInformation(ByVal FrameIdentifier As Integer, ByVal URL As String, ByVal additionalData As String) As Short
            Return ID3V2AddLinkedInformationW(FrameIdentifier, URL, additionalData)
        End Function
        Public Function ID3V2AddLyric(ByVal Language As String, ByVal Description As String, ByVal Text As String) As Short
            Return ID3V2AddLyricW(Language, Description, Text)
        End Function
        Public Function ID3V2AddMpegLocationLookupTable(ByVal arr As Byte(), ByVal Frames As Integer, ByVal Bytes As Integer, ByVal Milliseconds As Integer, ByVal BytesDeviation As Byte, ByVal MillisecondsDeviation As Byte) As Short
            Return ID3V2AddMpegLocationLookupTableW(arr, arr.Length, Frames, Bytes, Milliseconds, BytesDeviation, _
             MillisecondsDeviation)
        End Function
        Public Function ID3V2AddMusicCdIdentifier(ByVal arr As Byte()) As Short
            Return ID3V2AddMusicCdIdentifierW(arr, arr.Length)
        End Function
        Public Function ID3V2AddOwnership(ByVal Price As String, ByVal dateString As String, ByVal Seller As String) As Short
            Return ID3V2AddOwnershipW(Price, dateString, Seller)
        End Function
        Public Function ID3V2AddPictureArray(ByVal arr As Byte(), ByVal Description As String, ByVal PictureType As Short) As Short
            Return ID3V2AddPictureArrayW(arr, arr.Length, Description, PictureType)
        End Function
        Public Function ID3V2AddPictureFile(ByVal FileName As String, ByVal Description As String, ByVal PictureType As Short, ByVal asLink As Short) As Short
            Return ID3V2AddPictureFileW(FileName, Description, PictureType, asLink)
        End Function
        Public Function ID3V2AddPopularimeter(ByVal Email As String, ByVal Rating As Short, ByVal Counter As Integer) As Short
            Return ID3V2AddPopularimeterW(Email, Rating, Counter)
        End Function
        Public Function ID3V2AddPositionSynchronisation(ByVal TimestampFormat As Byte, ByVal Position As Integer) As Short
            Return ID3V2AddPositionSynchronisationW(TimestampFormat, Position)
        End Function
        Public Function ID3V2AddPrivateFrame(ByVal arr As Byte(), ByVal URL As String) As Short
            Return ID3V2AddPrivateFrameW(arr, arr.Length, URL)
        End Function
        Public Function ID3V2AddRecommendedBufferSize(ByVal BufferSize As Integer, ByVal EmbeddedInfoFlag As Byte, ByVal Offset As Integer) As Short
            Return ID3V2AddRecommendedBufferSizeW(BufferSize, EmbeddedInfoFlag, Offset)
        End Function
        Public Function ID3V2AddRelativeVolumeAdjustment(ByVal arr As Byte(), ByVal Identifier As String) As Short
            Return ID3V2AddRelativeVolumeAdjustmentW(arr, arr.Length, Identifier)
        End Function
        Public Function ID3V2AddReverb(ByVal reverbLeft As Short, ByVal reverbRight As Short, ByVal bouncesLeft As Byte, ByVal bouncesRight As Byte, ByVal feedbackLeftToLeft As Byte, ByVal feedbackLeftToRight As Byte, _
         ByVal feedbackRightToRight As Byte, ByVal feedbackRightToLeft As Byte, ByVal premixLeftToRight As Byte, ByVal premixRightToLeft As Byte) As Short
            Return ID3V2AddReverbW(reverbLeft, reverbRight, bouncesLeft, bouncesRight, feedbackLeftToLeft, feedbackLeftToRight, _
             feedbackRightToRight, feedbackRightToLeft, premixLeftToRight, premixRightToLeft)
        End Function
        Public Function ID3V2AddSignatureFrame(ByVal arr As Byte(), ByVal GroupSymbol As Short) As Short
            Return ID3V2AddSignatureFrameW(arr, arr.Length, GroupSymbol)
        End Function
        Public Function ID3V2AddSyncLyric(ByVal Language As String, ByVal Description As String, ByVal Text As String, ByVal ContentType As Short, ByVal TimeStampFormat As Short) As Short
            Return ID3V2AddSyncLyricW(Language, Description, Text, ContentType, TimeStampFormat)
        End Function
        Public Function ID3V2AddSynchronizedTempo(ByVal arr As Byte(), ByVal Format As Short) As Short
            Return ID3V2AddSynchronizedTempoW(arr, arr.Length, Format)
        End Function
        Public Function ID3V2AddTableOfContent(ByVal ID As String, ByVal Title As String, ByVal Description As String, ByVal isOrdered As Short) As Short
            Return ID3V2AddTableOfContentW(ID, Title, Description, isOrdered)
        End Function
        Public Function ID3V2AddUniqueFileIdentifier(ByVal arr As Byte(), ByVal Owner As String) As Short
            Return ID3V2AddUniqueFileIdentifierW(arr, arr.Length, Owner)
        End Function
        Public Function ID3V2AddUserFrame(ByVal Language As String, ByVal Text As String) As Short
            Return ID3V2AddUserFrameW(Language, Text)
        End Function
        Public Function ID3V2AddUserText(ByVal Description As String, ByVal Text As String) As Short
            Return ID3V2AddUserTextW(Description, Text)
        End Function
        Public Function ID3V2AddUserURL(ByVal Description As String, ByVal URL As String) As Short
            Return ID3V2AddUserURLW(Description, URL)
        End Function
        Public Function ID3V2DeleteAddendum(ByVal ID As String) As Short
            Return ID3V2DeleteAddendumW(ID)
        End Function
        Public Function ID3V2DeleteAllFrames(ByVal FrameID As ID3V2FRAMES) As Short
            Return ID3V2DeleteAllFramesW(FrameID)
        End Function
        Public Function ID3V2DeleteChildElement(ByVal ParentTocID As String, ByVal ChildID As String) As Short
            Return ID3V2DeleteChildElementW(ParentTocID, ChildID)
        End Function
        Public Function ID3V2DeleteSelectedFrame(ByVal FrameID As ID3V2FRAMES, ByVal Index As Short) As Short
            Return ID3V2DeleteSelectedFrameW(FrameID, Index)
        End Function
        Public Function ID3V2DeleteSubFrame(ByVal ID As String, ByVal FrameID As ID3V2FRAMES) As Short
            Return ID3V2DeleteSubFrameW(ID, FrameID)
        End Function
        Public Function ID3V2Exists() As Boolean
            Return Convert.ToBoolean(ID3V2ExistsW())
        End Function
        Public Function ID3V2GetAddendumDescription(ByVal ID As String) As String
            Return ID3V2GetAddendumDescriptionW(ID)
        End Function
        Public Function ID3V2GetAddendumTitle(ByVal ID As String) As String
            Return ID3V2GetAddendumTitleW(ID)
        End Function
        Public Function ID3V2GetAddendumType(ByVal ID As String) As Short
            Return ID3V2GetAddendumTypeW(ID)
        End Function
        Public Function ID3V2GetAllFrameIDs() As String
            Return ID3V2GetAllFrameIDsW()
        End Function
        Public Function ID3V2GetAudioEncryptionData(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
            Return ID3V2GetAudioEncryptionDataW(arr, maxLen, Index)
        End Function
        Public Function ID3V2GetAudioEncryptionPreviewLength(ByVal Index As Short) As Short
            Return ID3V2GetAudioEncryptionPreviewLengthW(Index)
        End Function
        Public Function ID3V2GetAudioEncryptionPreviewStart(ByVal Index As Short) As Short
            Return ID3V2GetAudioEncryptionPreviewStartW(Index)
        End Function
        Public Function ID3V2GetAudioEncryptionURL(ByVal Index As Short) As String
            Return ID3V2GetAudioEncryptionURLW(Index)
        End Function
        Public Function ID3V2GetAudioSeekPointBitsPerIndexpoint() As Integer
            Return ID3V2GetAudioSeekPointBitsPerIndexpointW()
        End Function
        Public Function ID3V2GetAudioSeekPointData(ByVal arr As Byte(), ByVal maxLen As Integer) As Integer
            Return ID3V2GetAudioSeekPointDataW(arr, maxLen)
        End Function
        Public Function ID3V2GetAudioSeekPointLength() As Integer
            Return ID3V2GetAudioSeekPointLengthW()
        End Function
        Public Function ID3V2GetAudioSeekPointNumber() As Integer
            Return ID3V2GetAudioSeekPointNumberW()
        End Function
        Public Function ID3V2GetAudioSeekPointStart() As Integer
            Return ID3V2GetAudioSeekPointStartW()
        End Function
        Public Function ID3v2GetChapterStartTime(ByVal ID As String) As Integer
            Return ID3V2GetChapterStartTimeW(ID)
        End Function
        Public Function ID3v2GetChapterEndTime(ByVal ID As String) As Integer
            Return ID3V2GetChapterEndTimeW(ID)
        End Function
        Public Function ID3V2GetChildElementID(ByVal ID As String, ByVal Index As Short) As String
            Return ID3V2GetChildElementIDW(ID, Index)
        End Function
        Public Function ID3V2GetChildElements(ByVal ID As String) As Short
            Return ID3V2GetChildElementsW(ID)
        End Function
        Public Function ID3V2GetCommentDescription(ByVal Index As Short) As String
            Return ID3V2GetCommentDescriptionW(Index)
        End Function
        Public Function ID3V2GetCommentLanguage(ByVal Index As Short) As String
            Return ID3V2GetCommentLanguageW(Index)
        End Function
        Public Function ID3V2GetComment(ByVal Index As Short) As String
            Return ID3V2GetCommentW(Index)
        End Function
        Public Function ID3V2GetCommercialFrameContactURL(ByVal Index As Short) As String
            Return ID3V2GetCommercialFrameContactURLW(Index)
        End Function
        Public Function ID3V2GetCommercialFrameDescription(ByVal Index As Short) As String
            Return ID3V2GetCommercialFrameDescriptionW(Index)
        End Function
        Public Function ID3V2GetCommercialFramePictureMime(ByVal Index As Short) As String
            Return ID3V2GetCommercialFramePictureMimeW(Index)
        End Function
        Public Function ID3V2GetCommercialFramePicture(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
            Return ID3V2GetCommercialFramePictureW(arr, maxLen, Index)
        End Function
        Public Function ID3V2GetCommercialFramePrice(ByVal Index As Short) As String
            Return ID3V2GetCommercialFramePriceW(Index)
        End Function
        Public Function ID3V2GetCommercialFrameReceivedAs(ByVal Index As Short) As Short
            Return ID3V2GetCommercialFrameReceivedAsW(Index)
        End Function
        Public Function ID3V2GetCommercialFrameSellerName(ByVal Index As Short) As String
            Return ID3V2GetCommercialFrameSellerNameW(Index)
        End Function
        Public Function ID3V2GetCommercialFrameValidUntil(ByVal Index As Short) As String
            Return ID3V2GetCommercialFrameValidUntilW(Index)
        End Function
        Public Function ID3V2GetEncoding(ByVal FrameID As ID3V2FRAMES) As Short
            Return ID3V2GetEncodingW(FrameID)
        End Function
        Public Function ID3V2GetEncryptionData(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
            Return ID3V2GetEncryptionDataW(arr, maxLen, Index)
        End Function
        Public Function ID3V2GetEncryptionSymbol(ByVal Index As Short) As Short
            Return ID3V2GetEncryptionSymbolW(Index)
        End Function
        Public Function ID3V2GetEncryptionURL(ByVal Index As Short) As String
            Return ID3V2GetEncryptionURLW(Index)
        End Function
        Public Function ID3V2GetEqualisationAdjustmentBits(ByVal Index As Short) As Short
            Return ID3V2GetEqualisationAdjustmentBitsW(Index)
        End Function
        Public Function ID3V2GetEqualisationData(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
            Return ID3V2GetEqualisationDataW(arr, maxLen, Index)
        End Function
        Public Function ID3V2GetEqualisationIdentification(ByVal Index As Short) As String
            Return ID3V2GetEqualisationIdentificationW(Index)
        End Function
        Public Function ID3V2GetEqualisationInterpolation(ByVal Index As Short) As Short
            Return ID3V2GetEqualisationInterpolationW(Index)
        End Function
        Public Function ID3V2GetEventTimingCodesData(ByVal arr As Byte(), ByVal maxLen As Integer) As Integer
            Return ID3V2GetEventTimingCodesDataW(arr, maxLen)
        End Function
        Public Function ID3V2GetEventTimingCodesTimestampFormat() As Short
            Return ID3V2GetEventTimingCodesTimestampFormatW()
        End Function
        Public Function ID3V2GetFrameCount(ByVal FrameID As ID3V2FRAMES) As Short
            Return ID3V2GetFrameCountW(FrameID)
        End Function
        Public Function ID3V2GetGeneralObjectData(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
            Return ID3V2GetGeneralObjectDataW(arr, maxLen, Index)
        End Function
        Public Function ID3V2GetGeneralObjectDescription(ByVal Index As Short) As String
            Return ID3V2GetGeneralObjectDescriptionW(Index)
        End Function
        Public Function ID3V2GetGeneralObjectFilename(ByVal Index As Short) As String
            Return ID3V2GetGeneralObjectFilenameW(Index)
        End Function
        Public Function ID3V2GetGeneralObjectMime(ByVal Index As Short) As String
            Return ID3V2GetGeneralObjectMimeW(Index)
        End Function
        Public Function ID3V2GetGenre() As String
            Return ID3V2GetGenreW()
        End Function
        Public Function ID3V2GetGroupIdentificationData(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
            Return ID3V2GetGroupIdentificationDataW(arr, maxLen, Index)
        End Function
        Public Function ID3V2GetGroupIdentificationSymbol(ByVal Index As Short) As Short
            Return ID3V2GetGroupIdentificationSymbolW(Index)
        End Function
        Public Function ID3V2GetGroupIdentificationURL(ByVal Index As Short) As String
            Return ID3V2GetGroupIdentificationURLW(Index)
        End Function
        Public Function ID3V2GetLinkedInformationAdditionalData(ByVal Index As Short) As String
            Return ID3V2GetLinkedInformationAdditionalDataW(Index)
        End Function
        Public Function ID3V2GetLinkedInformationFrameIdentifier(ByVal Index As Short) As Integer
            Return ID3V2GetLinkedInformationFrameIdentifierW(Index)
        End Function
        Public Function ID3V2GetLinkedInformationURL(ByVal Index As Short) As String
            Return ID3V2GetLinkedInformationURLW(Index)
        End Function
        Public Function ID3V2GetLyricDescription(ByVal Index As Short) As String
            Return ID3V2GetLyricDescriptionW(Index)
        End Function
        Public Function ID3V2GetLyricLanguage(ByVal Index As Short) As String
            Return ID3V2GetLyricLanguageW(Index)
        End Function
        Public Function ID3V2GetLyric(ByVal Index As Short) As String
            Return ID3V2GetLyricW(Index)
        End Function
        Public Function ID3V2GetMpegLocationLookupTableBytesDeviation() As Integer
            Return ID3V2GetMpegLocationLookupTableBytesDeviationW()
        End Function
        Public Function ID3V2GetMpegLocationLookupTableBytes() As Integer
            Return ID3V2GetMpegLocationLookupTableBytesW()
        End Function
        Public Function ID3V2GetMpegLocationLookupTableData(ByVal arr As Byte(), ByVal maxLen As Integer) As Integer
            Return ID3V2GetMpegLocationLookupTableDataW(arr, maxLen)
        End Function
        Public Function ID3V2GetMpegLocationLookupTableFrames() As Integer
            Return ID3V2GetMpegLocationLookupTableFramesW()
        End Function
        Public Function ID3V2GetMpegLocationLookupTableMillisecondsDeviation() As Integer
            Return ID3V2GetMpegLocationLookupTableMillisecondsDeviationW()
        End Function
        Public Function ID3V2GetMpegLocationLookupTableMilliseconds() As Integer
            Return ID3V2GetMpegLocationLookupTableMillisecondsW()
        End Function
        Public Function ID3V2GetMusicCdIdentifierData(ByVal arr As Byte(), ByVal maxLen As Integer) As Integer
            Return ID3V2GetMusicCdIdentifierDataW(arr, maxLen)
        End Function
        Public Function ID3V2GetOwnershipDate() As String
            Return ID3V2GetOwnershipDateW()
        End Function
        Public Function ID3V2GetOwnershipPrice() As String
            Return ID3V2GetOwnershipPriceW()
        End Function
        Public Function ID3V2GetOwnershipSeller() As String
            Return ID3V2GetOwnershipSellerW()
        End Function
        Public Function ID3V2GetPictureCount() As Short
            Return ID3V2GetFrameCount(ID3V2FRAMES.ID3F_APIC)
        End Function
        Public Function ID3V2GetPictureArray(ByVal Index As Short) As Byte()
            Dim maxLen As Integer = ID3V2GetPictureSize(Index)
            Dim dummy As Byte() = New Byte(maxLen) {}
            Dim ln As Integer = ID3V2GetPictureArrayW(dummy, maxLen, Index)
            If ln <= 0 OrElse ln >= maxLen Then
                ln = 0
            End If
            Return dummy
        End Function
        Public Function ID3V2GetPictureDescription(ByVal Index As Short) As String
            Return ID3V2GetPictureDescriptionW(Index)
        End Function
        Public Function ID3V2GetPictureFile(ByVal FileName As String, ByVal Index As Short) As Short
            Return ID3V2GetPictureFileW(FileName, Index)
        End Function
        Public Function ID3V2GetPictureMime(ByVal Index As Short) As String
            Return ID3V2GetPictureMimeW(Index)
        End Function
        Public Function ID3V2GetPictureSize(ByVal Index As Short) As Integer
            Return ID3V2GetPictureSizeW(Index)
        End Function
        Public Function ID3V2GetPictureType(ByVal Index As Short) As Short
            Return ID3V2GetPictureTypeW(Index)
        End Function
        Public Function ID3V2GetPictureTypeText(ByVal Index As Short) As String
            Return ID3V2GetPictureTypeTextW(Index)
        End Function
        Public Function ID3V2GetPlayCounter() As Integer
            Return ID3V2GetPlayCounterW()
        End Function
        Public Function ID3V2GetPopularimeterCounter(ByVal Index As Short) As Integer
            Return ID3V2GetPopularimeterCounterW(Index)
        End Function
        Public Function ID3V2GetPopularimeterEmail(ByVal Index As Short) As String
            Return ID3V2GetPopularimeterEmailW(Index)
        End Function
        Public Function ID3V2GetPopularimeterRating(ByVal Index As Short) As Short
            Return ID3V2GetPopularimeterRatingW(Index)
        End Function
        Public Function ID3V2GetPositionSynchronisationTimestampFormat() As Short
            Return ID3V2GetPositionSynchronisationTimestampFormatW()
        End Function
        Public Function ID3V2GetPositionSynchronisationValue() As Integer
            Return ID3V2GetPositionSynchronisationValueW()
        End Function
        Public Function ID3V2GetPossibleCHAPID() As String
            Return ID3V2GetPossibleCHAPIDW()
        End Function
        Public Function ID3V2GetPossibleTOCID() As String
            Return ID3V2GetPossibleTOCIDW()
        End Function
        Public Function ID3V2GetPrivateFrameData(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
            Return ID3V2GetPrivateFrameDataW(arr, maxLen, Index)
        End Function
        Public Function ID3V2GetPrivateFrameURL(ByVal Index As Short) As String
            Return ID3V2GetPrivateFrameURLW(Index)
        End Function
        Public Function ID3V2GetRelativeVolumeAdjustmentData(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
            Return ID3V2GetRelativeVolumeAdjustmentDataW(arr, maxLen, Index)
        End Function
        Public Function ID3V2GetRelativeVolumeAdjustmentIdentifier(ByVal Index As Short) As String
            Return ID3V2GetRelativeVolumeAdjustmentIdentifierW(Index)
        End Function
        Public Function ID3V2GetReverbBouncesLeft() As Short
            Return ID3V2GetReverbBouncesLeftW()
        End Function
        Public Function ID3V2GetReverbBouncesRight() As Short
            Return ID3V2GetReverbBouncesRightW()
        End Function
        Public Function ID3V2GetReverbFeedbackLeftToLeft() As Short
            Return ID3V2GetReverbFeedbackLeftToLeftW()
        End Function
        Public Function ID3V2GetReverbFeedbackLeftToRight() As Short
            Return ID3V2GetReverbFeedbackLeftToRightW()
        End Function
        Public Function ID3V2GetReverbFeedbackRightToLeft() As Short
            Return ID3V2GetReverbFeedbackRightToLeftW()
        End Function
        Public Function ID3V2GetReverbFeedbackRightToRight() As Short
            Return ID3V2GetReverbFeedbackRightToRightW()
        End Function
        Public Function ID3V2GetReverbLeft() As Short
            Return ID3V2GetReverbLeftW()
        End Function
        Public Function ID3V2GetReverbPremixLeftToRight() As Short
            Return ID3V2GetReverbPremixLeftToRightW()
        End Function
        Public Function ID3V2GetReverbPremixRightToLeft() As Short
            Return ID3V2GetReverbPremixRightToLeftW()
        End Function
        Public Function ID3V2GetReverbRight() As Short
            Return ID3V2GetReverbRightW()
        End Function
        Public Function ID3V2GetSignatureFrameData(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
            Return ID3V2GetSignatureFrameDataW(arr, maxLen, Index)
        End Function
        Public Function ID3V2GetSignatureFrameGroupSymbol(ByVal Index As Short) As Short
            Return ID3V2GetSignatureFrameGroupSymbolW(Index)
        End Function
        Public Function ID3V2GetSize() As Integer
            Return ID3V2GetSizeW()
        End Function
        Public Function ID3V2GetSubFrameDescription(ByVal ID As String, ByVal Index As Short) As String
            Return ID3V2GetSubFrameDescriptionW(ID, Index)
        End Function
        Public Function ID3V2GetSubFrameID(ByVal ID As String, ByVal Index As Short) As Integer
            Return ID3V2GetSubFrameIDW(ID, Index)
        End Function
        Public Function ID3V2GetSubFrameImageType(ByVal ID As String, ByVal Index As Short) As Short
            Return ID3V2GetSubFrameImageTypeW(ID, Index)
        End Function
        Public Function ID3V2GetSubFrameImage(ByVal ID As String, ByVal Index As Short) As Byte()
            Dim maxLen As Integer = 1000000
            Dim dummy As Byte() = New Byte(maxLen) {}
            Dim ln As Integer = ID3V2GetSubFrameImageW(dummy, maxLen, ID, Index)
            If ln <= 0 OrElse ln >= maxLen Then
                ln = 0
            End If
            Array.Resize(Of Byte)(dummy, ln)
            Return dummy
        End Function
        Public Function ID3V2GetSubFrameText(ByVal ID As String, ByVal Index As Short) As String
            Return ID3V2GetSubFrameTextW(ID, Index)
        End Function
        Public Function ID3V2GetSubFrameType(ByVal ID As String, ByVal Index As Short) As Short
            Return ID3V2GetSubFrameTypeW(ID, Index)
        End Function
        Public Function ID3V2GetSubFrames(ByVal ID As String) As Short
            Return ID3V2GetSubFramesW(ID)
        End Function
        Public Function ID3V2GetSyncLyricContentType(ByVal Index As Short) As Short
            Return ID3V2GetSyncLyricContentTypeW(Index)
        End Function
        Public Function ID3V2GetSyncLyricDescription(ByVal Index As Short) As String
            Return ID3V2GetSyncLyricDescriptionW(Index)
        End Function
        Public Function ID3V2GetSyncLyricLanguage(ByVal Index As Short) As String
            Return ID3V2GetSyncLyricLanguageW(Index)
        End Function
        Public Function ID3V2GetSyncLyricTimeFormat(ByVal Index As Short) As Short
            Return ID3V2GetSyncLyricTimeFormatW(Index)
        End Function
        Public Function ID3V2GetSyncLyric(ByVal Index As Short) As String
            Return ID3V2GetSyncLyricW(Index)
        End Function
        Public Function ID3V2GetSynchronizedTempoData(ByVal arr As Byte(), ByVal maxLen As Integer) As Integer
            Return ID3V2GetSynchronizedTempoDataW(arr, maxLen)
        End Function
        Public Function ID3V2GetSynchronizedTempoFormat() As Short
            Return ID3V2GetSynchronizedTempoFormatW()
        End Function
        Public Function ID3V2GetTOCRootID() As String
            Return ID3V2GetTOCRootIDW()
        End Function
        Public Function ID3V2GetTextFrame(ByVal FrameID As ID3V2FRAMES) As String
            Return ID3V2GetTextFrameW(FrameID)
        End Function
        Public Function ID3V2GetURLFrame(ByVal FrameID As ID3V2FRAMES) As String
            Return ID3V2GetURLFrameW(FrameID)
        End Function
        Public Function ID3V2GetUniqueFileIdentifierData(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal Index As Short) As Integer
            Return ID3V2GetUniqueFileIdentifierDataW(arr, maxLen, Index)
        End Function
        Public Function ID3V2GetUniqueFileIdentifierOwner(ByVal Index As Short) As String
            Return ID3V2GetUniqueFileIdentifierOwnerW(Index)
        End Function
        Public Function ID3V2GetUserFrameLanguage(ByVal Index As Short) As String
            Return ID3V2GetUserFrameLanguageW(Index)
        End Function
        Public Function ID3V2GetUserFrame(ByVal Index As Short) As String
            Return ID3V2GetUserFrameW(Index)
        End Function
        Public Function ID3V2GetUserTextDescription(ByVal Index As Short) As String
            Return ID3V2GetUserTextDescriptionW(Index)
        End Function
        Public Function ID3V2GetUserText(ByVal Index As Short) As String
            Return ID3V2GetUserTextW(Index)
        End Function
        Public Function ID3V2GetUserURLDescription(ByVal Index As Short) As String
            Return ID3V2GetUserURLDescriptionW(Index)
        End Function
        Public Function ID3V2GetUserURL(ByVal Index As Short) As String
            Return ID3V2GetUserURLW(Index)
        End Function
        Public Function ID3V2GetVersion() As String
            Return ID3V2GetVersionW()
        End Function
        Public Function ID3V2GetTOCIsOrdered(ByVal ID As String) As Boolean
            Return Convert.ToBoolean(ID3V2GetTOCIsOrderedW(ID))
        End Function
        Public Function ID3V2GetRecommendedBufferSizeFlag() As Short
            Return ID3V2GetRecommendedBufferSizeFlagW()
        End Function
        Public Function ID3V2RecommendedBufferSizeOffset() As Integer
            Return ID3V2GetRecommendedBufferSizeOffsetW()
        End Function
        Public Function ID3V2GetRecommendedBufferSizeValue() As Integer
            Return ID3V2GetRecommendedBufferSizeValueW()
        End Function
        Public Function ID3V2RemoveTagFromFile(ByVal FileName As String) As Short
            Return ID3V2RemoveTagFromFileW(FileName)
        End Function
        Public Function ID3V2RemoveTag() As Short
            Return ID3V2RemoveTagW()
        End Function
        Public Function ID3V2AddEventTimingCodes(ByVal arr As Byte(), ByVal maxLen As Integer, ByVal TimestampFormat As Byte) As Short
            Return ID3V2AddEventTimingCodesW(arr, maxLen, TimestampFormat)
        End Function
        Public Function ID3V2SaveChangesToFile(ByVal FileName As String) As Short
            Return ID3V2SaveChangesToFileW(FileName)
        End Function
        Public Function ID3V2SaveChanges() As Short
            Return ID3V2SaveChangesW()
        End Function
        Public Function ID3v2SetAddendumDescription(ByVal ID As String, ByVal newDescription As String) As Integer
            Return ID3V2SetAddendumDescriptionW(ID, newDescription)
        End Function
        Public Function ID3v2SetAddendumTitle(ByVal ID As String, ByVal newTitle As String) As Integer
            Return ID3V2SetAddendumTitleW(ID, newTitle)
        End Function
        Public Sub ID3V2SetChapterTimes(ByVal ID As String, ByVal startTime As Integer, ByVal endTime As Integer)
            ID3V2SetChapterTimesW(ID, startTime, endTime)
        End Sub
        Public Function ID3V2SetFormatAndEncoding(ByVal format As Short, ByVal encoding As Short) As Short
            Return ID3V2SetFormatAndEncodingW(format, encoding)
        End Function
        Public Sub ID3V2SetGenre(ByVal textString As String)
            ID3V2SetGenreW(textString)
        End Sub
        Public Function ID3V2SetImageSubFrame(ByVal arr As Byte(), ByVal ID As String, ByVal Description As String, ByVal pictureType As PictureType) As Short
            Return ID3V2SetImageSubFrameW(arr, arr.Length, ID, Description, pictureType)
        End Function
        Public Sub ID3V2SetPlayCounter(ByVal counter As Integer)
            ID3V2SetPlayCounterW(counter)
        End Sub
        Public Sub ID3V2AddSeekOffset(ByVal offset As Integer)
            ID3V2AddSeekOffsetW(offset)
        End Sub
        Public Sub ID3V2SetTextFrame(ByVal FrameID As ID3V2FRAMES, ByVal textString As String)
            ID3V2SetTextFrameW(FrameID, textString)
        End Sub
        Public Function ID3V2SetTextSubFrame(ByVal ID As String, ByVal FrameID As ID3V2FRAMES, ByVal textString As String, ByVal description As String) As Short
            Return ID3V2SetTextSubFrameW(ID, FrameID, textString, description)
        End Function
        Public Sub ID3V2SetURLFrame(ByVal FrameID As ID3V2FRAMES, ByVal textString As String)
            ID3V2SetURLFrameW(FrameID, textString)
        End Sub
        Public Function ID3V2SetURLSubFrame(ByVal ID As String, ByVal FrameID As ID3V2FRAMES, ByVal urlString As String, ByVal description As String) As Short
            Return ID3V2SetURLSubFrameW(ID, FrameID, urlString, description)
        End Function
        Public Function ID3V2SetTOCIsOrdered(ByVal ID As String, ByVal isOrdered As Boolean) As Short
            Return ID3V2SetTOCIsOrderedW(ID, Convert.ToInt16(isOrdered))
        End Function
        Public Function ID3V2ImportCueFile(ByVal FileName As String) As Short
            Return ID3V2ImportCueFileW(FileName)
        End Function
#End Region

#Region "LYRICS Methods"
        Public Function LYRICSExists() As Boolean
            Return Convert.ToBoolean(LYRICSExistsW())
        End Function
        Public Function LYRICSGetSize() As Integer
            Return LYRICSGetSizeW()
        End Function
        Public Function LYRICSGetStartPosition() As Integer
            Return LYRICSGetStartPositionW()
        End Function
        Public Function LYRICSGetVersion() As String
            Return LYRICSGetVersionW()
        End Function
        Public Function LYRICSRemoveTagFromFile(ByVal FileName As String) As Boolean
            Return Convert.ToBoolean(LYRICSRemoveTagFromFileW(FileName))
        End Function
        Public Function LYRICSRemoveTag() As Boolean
            Return Convert.ToBoolean(LYRICSRemoveTagW())
        End Function
        Public Function LYRICSSaveChangesToFile(ByVal FileName As String) As Boolean
            Return Convert.ToBoolean(LYRICSSaveChangesToFileW(FileName))
        End Function
        Public Function LYRICSSaveChanges() As Boolean
            Return Convert.ToBoolean(LYRICSSaveChangesW())
        End Function
#End Region

#Region "MONKEY Methods"
        Public Function MONKEYGetBits() As Short
            Return MONKEYGetBitsW()
        End Function
        Public Function MONKEYGetCompressionRatio() As Single
            Return MONKEYGetCompressionRatioW()
        End Function
        Public Function MONKEYGetCompression() As String
            Return MONKEYGetCompressionW()
        End Function
        Public Function MONKEYGetFrames() As Integer
            Return MONKEYGetFramesW()
        End Function
        Public Function MONKEYGetPeak() As Single
            Return MONKEYGetPeakW()
        End Function
        Public Function MONKEYGetSamplesPerFrame() As Integer
            Return MONKEYGetSamplesPerFrameW()
        End Function
        Public Function MONKEYGetSamples() As Integer
            Return MONKEYGetSamplesW()
        End Function
#End Region

#Region "MP4 Methods"
        Public Sub MP4DeleteEntries()
            MP4DeleteEntriesW()
        End Sub
        Public Sub MP4DeletePicture(ByVal Index As Short)
            MP4DeletePictureW(Index)
        End Sub
        Public Sub MP4DeletePictures()
            MP4DeletePicturesW()
        End Sub
        Public Function MP4GetAllFrameIDs() As String
            Return MP4GetAllFrameIDsW()
        End Function
        Public Function MP4GetiTuneFrame(ByVal FrameID As String) As String
            Return MP4GetiTuneFrameW(FrameID)
        End Function
        Public Function MP4GetTextFrame(ByVal FrameID As MP4FRAMES) As String
            Return MP4GetTextFrameW(FrameID)
        End Function
        Public Function MP4GetPictureArray(ByVal Index As Short) As Byte()
            Dim maxLen As Integer = MP4GetPictureSize(Index)
            Dim dummy As Byte() = New Byte(maxLen) {}
            Dim ln As Integer = MP4GetPictureArrayW(dummy, maxLen, Index)
            If ln <= 0 OrElse ln >= maxLen Then
                ln = 0
            End If
            Return dummy
        End Function
        Public Function MP4GetPictureCount() As Integer
            Return MP4GetPictureCountW()
        End Function
        Public Function MP4GetPictureFile(ByVal FileName As String, ByVal Index As Short) As Short
            Return MP4GetPictureFileW(FileName, Index)
        End Function
        Public Function MP4GetPictureMime(ByVal Index As Short) As String
            Return MP4GetPictureMimeW(Index)
        End Function
        Public Function MP4GetPictureSize(ByVal Index As Short) As Integer
            Return MP4GetPictureSizeW(Index)
        End Function
        Public Function MP4GetVersion() As String
            Return MP4GetVersionW()
        End Function
        Public Function MP4SaveChangesToFile(ByVal FileName As String) As Boolean
            Return Convert.ToBoolean(MP4SaveChangesToFileW(FileName))
        End Function
        Public Function MP4SaveChanges() As Boolean
            Return Convert.ToBoolean(MP4SaveChangesW())
        End Function
        Public Function MP4AddPictureArray(ByVal arr As Byte()) As Short
            Return MP4AddPictureArrayW(arr, arr.Length)
        End Function
        Public Function MP4AddPictureFile(ByVal FileName As String) As Short
            Return MP4AddPictureFileW(FileName)
        End Function
        Public Sub MP4SetiTuneFrame(ByVal FrameID As String, ByVal textString As String)
            MP4SetiTuneFrameW(FrameID, textString)
        End Sub
        Public Sub MP4SetTextFrame(ByVal FrameID As MP4FRAMES, ByVal textString As String)
            MP4SetTextFrameW(FrameID, textString)
        End Sub
#End Region

#Region "MPEG Methods"
        Public Function MPEGGetEmphasis() As String
            Return MPEGGetEmphasisW()
        End Function
        Public Function MPEGGetEncoder() As String
            Return MPEGGetEncoderW()
        End Function
        Public Function MPEGGetFramePosition() As Integer
            Return MPEGGetFramePositionW()
        End Function
        Public Function MPEGGetFrameSize() As Integer
            Return MPEGGetFrameSizeW()
        End Function
        Public Function MPEGGetFrames() As Integer
            Return MPEGGetFramesW()
        End Function
        Public Function MPEGGetLayer() As String
            Return MPEGGetLayerW()
        End Function
        Public Function MPEGGetVersion() As String
            Return MPEGGetVersionW()
        End Function
        Public Function MPEGGetIsCopyrighted() As Boolean
            Return Convert.ToBoolean(MPEGIsCopyrightedW())
        End Function
        Public Function MPEGIsOriginal() As Boolean
            Return Convert.ToBoolean(MPEGIsOriginalW())
        End Function
        Public Function MPEGIsPadding() As Boolean
            Return Convert.ToBoolean(MPEGIsPaddingW())
        End Function
        Public Function MPEGIsPrivate() As Boolean
            Return Convert.ToBoolean(MPEGIsPrivateW())
        End Function
        Public Function MPEGIsProtected() As Boolean
            Return Convert.ToBoolean(MPEGIsProtectedW())
        End Function
        Public Function MPEGIsVBR() As Boolean
            Return Convert.ToBoolean(MPEGIsVBRW())
        End Function
        Public Function MPEGSetCopyrighted(ByVal FileName As String, ByVal newValue As Short) As Boolean
            Return Convert.ToBoolean(MPEGSetCopyrightedW(FileName, newValue))
        End Function
        Public Function MPEGSetOriginal(ByVal FileName As String, ByVal newValue As Short) As Boolean
            Return Convert.ToBoolean(MPEGSetOriginalW(FileName, newValue))
        End Function
        Public Function MPEGSetPrivate(ByVal FileName As String, ByVal newValue As Short) As Boolean
            Return Convert.ToBoolean(MPEGSetPrivateW(FileName, newValue))
        End Function
#End Region

#Region "MPP Methods"
        Public Function MPPGetFrames() As Integer
            Return MPPGetFramesW()
        End Function
        Public Function MPPGetStreamVersion() As Short
            Return MPPGetStreamVersionW()
        End Function
#End Region

#Region "OGG Methods"
        Public Function OGGGetBitRateNominal() As Integer
            Return OGGGetBitRateNominalW()
        End Function
        Public Function OGGGetItemKeys() As String
            Return OGGGetItemKeysW()
        End Function
        Public Function OGGGetUserItem(ByVal ItemKey As String) As String
            Return OGGGetUserItemW(ItemKey)
        End Function
        Public Function OGGRemoveTagFromFile(ByVal FileName As String) As Short
            Return OGGRemoveTagFromFileW(FileName)
        End Function
        Public Function OGGRemoveTag() As Boolean
            Return Convert.ToBoolean(OGGRemoveTagW())
        End Function
        Public Function OGGSaveChangesToFile(ByVal FileName As String) As Boolean
            Return Convert.ToBoolean(OGGSaveChangesToFileW(FileName))
        End Function
        Public Function OGGSaveChanges() As Boolean
            Return Convert.ToBoolean(OGGSaveChangesW())
        End Function
        Public Sub OGGSetUserItem(ByVal ItemKey As String, ByVal textString As String)
            OGGSetUserItemW(ItemKey, textString)
        End Sub
#End Region

#Region "WAV Methods"
        Public Function WAVGetBitsPerSample() As Integer
            Return WAVGetBitsPerSampleW()
        End Function
        Public Function WAVGetBlockAlign() As Integer
            Return WAVGetBlockAlignW()
        End Function
        Public Function WAVGetBytesPerSecond() As Integer
            Return WAVGetBytesPerSecondW()
        End Function
        Public Function WAVGetChannels() As Integer
            Return WAVGetChannelsW()
        End Function
        Public Function WAVGetFormatID() As Short
            Return WAVGetFormatIDW()
        End Function
        Public Function WAVGetFormat() As String
            Return WAVGetFormatW()
        End Function
        Public Function WAVGetHeaderSize() As Integer
            Return WAVGetHeaderSizeW()
        End Function
        Public Function WAVGetInfoChunkIDs() As String
            Return WAVGetInfoChunkIDsW()
        End Function
        Public Function WAVGetTextFrame(ByVal FrameID As WAVFRAMES) As String
            Return WAVGetTextFrameW(FrameID)
        End Function
        Public Sub WAVSetTextFrame(ByVal FrameID As WAVFRAMES, ByVal textString As String)
            WAVSetTextFrameW(FrameID, textString)
        End Sub
        Public Function WAVGetCartChunkEntry(ByVal Index As WAVCARTENTRIES) As String
            Return WAVGetCartChunkEntryW(CShort(Index))
        End Function
        Public Sub WAVSetCartChunkEntry(ByVal Index As WAVCARTENTRIES, ByVal textString As String)
            WAVSetCartChunkEntryW(CShort(Index), textString)
        End Sub
        Public Function WAVSaveChanges() As Boolean
            Return Convert.ToBoolean(WAVSaveChangesW())
        End Function
        Public Function WAVSaveChangesToFile(ByVal FileName As String) As Boolean
            Return Convert.ToBoolean(WAVSaveChangesToFileW(FileName))
        End Function
#End Region

#Region "WMA Methods"
        Public Function WMADeletePicture(ByVal Index As Short) As Boolean
            Return Convert.ToBoolean(WMADeletePictureW(Index))
        End Function
        Public Sub WMADeletePictures()
            WMADeletePicturesW()
        End Sub
        Public Function WMAGetItemKeys() As String
            Return WMAGetItemKeysW()
        End Function
        Public Function WMAGetPictureArray(ByVal Index As Short) As Byte()
            Dim maxLen As Integer = WMAGetPictureSize(Index)
            Dim dummy As Byte() = New Byte(maxLen) {}
            Dim ln As Integer = WMAGetPictureArrayW(dummy, maxLen, Index)
            If ln <= 0 OrElse ln >= maxLen Then
                ln = 0
            End If
            Return dummy
        End Function
        Public Function WMAGetPictureDescription(ByVal Index As Short) As String
            Return WMAGetPictureDescriptionW(Index)
        End Function
        Public Function WMAGetPictureMime(ByVal Index As Short) As String
            Return WMAGetPictureMimeW(Index)
        End Function
        Public Function WMAGetPictureSize(ByVal Index As Short) As Integer
            Return WMAGetPictureSizeW(Index)
        End Function
        Public Function WMAGetPictureType(ByVal Index As Short) As Short
            Return WMAGetPictureTypeW(Index)
        End Function
        Public Function WMAGetPictureFile(ByVal FileName As String, ByVal Index As Short) As Short
            Return WMAGetPictureFileW(FileName, Index)
        End Function
        Public Function WMAGetPictureCount() As Short
            Return WMAGetPictureCountW()
        End Function
        Public Function WMAGetUserItem(ByVal ItemKey As String) As String
            Return WMAGetUserItemW(ItemKey)
        End Function
        Public Function WMAIsVBR() As Boolean
            Return Convert.ToBoolean(WMAIsVBRW())
        End Function
        Public Function WMASaveChangesToFile(ByVal FileName As String) As Boolean
            Return Convert.ToBoolean(WMASaveChangesToFileW(FileName))
        End Function
        Public Function WMASaveChanges() As Boolean
            Return Convert.ToBoolean(WMASaveChangesW())
        End Function
        Public Function WMAAddPictureArray(ByVal arr As Byte(), ByVal Description As String, ByVal PictureType As Short, ByVal Index As Short) As Short
            Return WMAAddPictureArrayW(arr, arr.Length, Description, PictureType, Index)
        End Function
        Public Function WMAAddPictureFile(ByVal FileName As String, ByVal Description As String, ByVal PictureType As Short, ByVal Index As Short) As Short
            Return WMAAddPictureFileW(FileName, Description, PictureType, Index)
        End Function
        Public Sub WMASetUserItem(ByVal ItemKey As String, ByVal textString As String)
            WMASetUserItemW(ItemKey, textString)
        End Sub
#End Region

    End Module
End Namespace
