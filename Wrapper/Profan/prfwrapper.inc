'Autor Dieter Zornow, included in AudioGenie3 with the permission of the author
'Läuft unverändert ab Xprofan 11
'Checked and adapted to AudioGenie3 3.3 (470 functions): functions that the DLL does not export any more are called by their new names,
'the functions of the versions 3.2 and 3.3 (composer, enhanced ID3v1 tag, LAME tag, WAV display text, cue files) are at the end
'of the procedures. Functions that return a Float ("Vorsicht Float") cannot be evaluated safely in every XProfan version.
'The DLL is initialized with initaudiogenie("AudioGenie3.dll"): it has to be registered and in the search path.
ErrorProc 'ab XProfan 11 benötigt, Routine wenn dll nicht gefunden wird
   Set("ErrorLevel", 1)
   Messagebox("Achtung eine wichtige Dll konnte nicht geladen werden.\nDas Programm ist nicht lauffähig","Schwerer Fehler",262144+16)
   end
EndProc
'Wav Felder Definition zum direkt ansprechen
DEF &WAV_IARL $4941524C
DEF &WAV_IART $49415254
DEF &WAV_ICMS $49434D53
DEF &WAV_ICMT $49434D54
DEF &WAV_ICOP $49434F50
DEF &WAV_ICRD $49435244
DEF &WAV_ICRP $49435250
DEF &WAV_IDIM $4944494D
DEF &WAV_IDPI $49445049
DEF &WAV_IENG $49454E47
DEF &WAV_IGNR $49474E52
DEF &WAV_IKEY $494B4559
DEF &WAV_ILGT $494C4754
DEF &WAV_IMED $494D4544
DEF &WAV_INAM $494E414D
DEF &WAV_IPLT $49504C54
DEF &WAV_IPRD $49505244
DEF &WAV_ISBJ $4953424A
DEF &WAV_ISFT $49534654
DEF &WAV_ISHP $49534850
DEF &WAV_ISRC $49535243
DEF &WAV_ISRF $49535246
DEF &WAV_ITCH $49544348
DEF &WAV_ITRK $4954524B
'MP4 Felder Definition zum direkt ansprechen
DEF &MP4_ALBUM $A9616C62 ' ©alb
DEF &MP4_ARTIST $A9617274 ' ©art
DEF &MP4_ARTISTb $A9415254 '©ART
DEF &MP4_AUTHOR $A9617574 '  ©aut
DEF &MP4_ALBUMARTIST $61415254 ' aART
Def   &MP4_BEATSPERMINUTE $746D706F 'tmpo
DEF &MP4_CATEGORY $63617467  ' catg
DEF  &MP4_CDSETNUMBER $2064736B 'dsk
DEF &MP4_COMMENT $A9636D74 ' ©cmt
DEF &MP4_COMPOSER $A9777274 ' ©wrt
DEF &MP4_COMPILATION $6370696C 'cpil
DEF &MP4_CREATEDATE $A9646179 ' ©day
DEF &MP4_COPYRIGHT $A9637079 ' ©cpy
'DEF &MP4_CREATEDATE $A9646179 ' ©day
DEF &MP4_DESCRIPTION $A9646573 ' ©des
DEF &MP4_INFORMATION $A9696E66 ' ©inf
DEF &MP4_DIRECTOR $A9646972 ' ©dir
DEF &MP4_DISCLAIMER $A9646973 ' ©dis
DEF &MP4_ENCODER $A9746F6F ' ©too
DEF &MP4_GROUPING $A9677270 ' ©grp
DEF &MP4_KEYWORD $6B657977 ' keyw
DEF &MP4_LYRICS $A96C7972 ' ©lyr
DEF &MP4_TITLE $A96E616D ' ©nam
DEF &MP4_NETURL $A975726C ' ©url
DEF &MP4_ORIGINALARTIST $A96F7065 ' ©ope
DEF &MP4_ORIGINALFORMAT $A9666D74 ' ©fmt
DEF &MP4_ORIGINALSOURCE $A9737263 ' ©src
DEF &MP4_PERFORMER $A9707266 ' ©prf
DEF &MP4_PRODUCER $A9707264 ' ©prd
DEF &MP4_WARNING $A977726E ' ©wrn
'Zusatz
Def &MP4_genre2 $A967656E  '©gen  OK wird gelesen und geschrieben
Def &MP4_Rating $72746E67  'rtng OK wird geschrieben und gelesen
Def &MP4_Stik $7374696B   'stik  Ok wird gelesen und geschrieben
Def &MP4_Podcast $70637374  'pcst 'kann nicht schreiben und lesen
Def &MP4_Podcasturl $7075726C  'purl  'wird geschrieben aber "purl" wird mit fremden Zeichen angezeigt, der Wert ist aber da
Def &MP4_EpisodeGlobalUniqueID $65676964  'egid OK wird geschrieben und gelesen
Def &MP4_TVNetworkName $74766E6E   'tvnn Wird nicht geschrieben der Framename taucht aber mit fremden Zeichen auf
Def &MP4_TVShowName $74767368 'tvsh OK wird geschrieben und gelesen
Def &MP4_TVEpisodeNumber $7476656E  'tven OK wird geschrieben und gelesen
Def &MP4_TVSeason $7476736E   'tvsn OK wird geschrieben und gelesen
Def &MP4_TVEpisode $74766573   'tves Wird nicht geschrieben der Framename wird mit fremden Zeichen geschrieben aber kein Wert
Def &MP4_PurchaseDate $70757264    'purd  OK wird geschrieben und gelesen
Def &MP4_GaplessPlayback $70676170    'pgap Der Framename wird richtig geschrieben aber kein Wert dazu eingetragen
Def &MP4_DiskNumber $6469736B 'disk Ok wird geschrieben und gelesen
Def &MP4_Copyrighta $63707274 'cprt OK kann schreiben und lesen
Def &MP4_Descriptiona $64657363 'desc Ok kann schreiben und lesen
'ID3V2 Felder Definition zum direkt ansprechen
DEF &ID3F_AENC $41454E43 'Audio encryption
DEF &ID3F_APIC $41504943 'Attached picture
DEF &ID3F_ASPI $41535049 'Audio seek point index
DEF &ID3F_CHAP $43484150 'ID3V2AddChapterW
DEF &ID3F_COMM $434F4D4D 'Comments
DEF &ID3F_COMR $434F4D52 'Commercial frame
DEF &ID3F_CTOC $43544F43 'ID3V2AddTableOfContentW
DEF &ID3F_ENCR $454E4352 'Encryption method registration
DEF &ID3F_EQU2 $45515532 'Equalization (2)
DEF &ID3F_EQUA $45515541 'Equalization
DEF &ID3F_ETCO $4554434F 'Event timing codes
DEF &ID3F_GEOB $47454F42 'General encapsulated object
DEF &ID3F_GRID $47524944 'Group identification registration
DEF &ID3F_IPLS $49504C53 'Involved people list
DEF &ID3F_LINK $4C494E4B 'Linked information
DEF &ID3F_MCDI $4D434449 'Music CD identifier
DEF &ID3F_MLLT $4D4C4C54 'MPEG location lookup table
DEF &ID3F_OWNE $4F574E45 'Ownership frame
DEF &ID3F_PCNT $50434E54 'IPlay counter
DEF &ID3F_POPM $504F504D 'Popularimeter ID3V2DeletePopularimeters
DEF &ID3F_POSS $504F5353 'Position synchronisation frame
DEF &ID3F_PRIV $50524956 'Private frame
DEF &ID3F_RBUF $52425546 'Recommended buffer size
DEF &ID3F_RVA2 $52564132 'Relative volume adjustment (2)
DEF &ID3F_RVAD $52564144 'Relative volume adjustment
DEF &ID3F_RVRB $52565242 'Reverb
DEF &ID3F_SEEK $5345454B 'Seek frame
DEF &ID3F_SIGN $5349474E 'Signature frame
DEF &ID3F_SYLT $53594C54 'Synchronized lyric/text ID3V2SyncLyrics
DEF &ID3F_SYTC $53595443 'Synchronised tempo codes
DEF &ID3F_TALB $54414C42 'Album/Movie/Show title
DEF &ID3F_TBPM $5442504D 'BPM (beats per minute)
DEF &ID3F_TCOM $54434F4D 'Composer
DEF &ID3F_TCON $54434F4E 'Content type
DEF &ID3F_TCOP $54434F50 'Copyright message
DEF &ID3F_TDAT $54444154 'Date
DEF &ID3F_TDEN $5444454E ' ID3V2EncodingTime
DEF &ID3F_TDLY $54444C59 'Playlist delay
DEF &ID3F_TDOR $54444F52 ' ID3V2OriginalReleaseTime
DEF &ID3F_TDRC $54445243 ' ID3V2RecordingTime
DEF &ID3F_TDRL $5444524C ' ID3V2ReleaseTime
DEF &ID3F_TDTG $54445447 ' ID3V2TaggingTime
DEF &ID3F_TENC $54454E43 'Encoded by
DEF &ID3F_TEXT $54455854 'Lyricist/Text writer
DEF &ID3F_TFLT $54464C54 'File type
DEF &ID3F_TIME $54494D45 'Time
DEF &ID3F_TIPL $5449504C ' ID3V2InvolvedPeopleList
DEF &ID3F_TIT1 $54495431 'Content group description  ID3V2Category
DEF &ID3F_TIT2 $54495432 'Title/songname/content description  ID3V2Title
DEF &ID3F_TIT3 $54495433 'Subtitle/Description refinement  ID3V2Subtitle
DEF &ID3F_TKEY $544B4559 'Initial key  ID3V2InitialKey
DEF &ID3F_TLAN $544C414E 'Language
DEF &ID3F_TLEN $544C454E 'Length  ID3V2Length
DEF &ID3F_TMCL $544D434C ' ID3V2MusicianCreditsList
DEF &ID3F_TMED $544D4544 'Media type
DEF &ID3F_TMOO $544D4F4F ' ID3V2Mood
DEF &ID3F_TOAL $544F414C 'Original album/movie/show title  ID3V2OriginalAlbum
DEF &ID3F_TOFN $544F464E 'Original filename
DEF &ID3F_TOLY $544F4C59 'Original lyricist(s)/text writer(s)
DEF &ID3F_TOPE $544F5045 'Original artist(s)/performer(s)
DEF &ID3F_TORY $544F5259 'Original release year
DEF &ID3F_TOWN $544F574E 'File owner/licensee  ID3V2FileOwner
DEF &ID3F_TPE1 $54504531 'Lead performer(s)/Soloist(s)  ID3V2Artist
DEF &ID3F_TPE2 $54504532 'Band/orchestra/accompaniment  ID3V2Orchestra
DEF &ID3F_TPE3 $54504533 'Conductor/performer refinement  ID3V2Conductor
DEF &ID3F_TPE4 $54504534 'Interpreted, remixed, or otherwise modified by  ID3V2Interpreted
DEF &ID3F_TPOS $54504F53 'Part of a set  ID3V2PartofSet
DEF &ID3F_TPRO $5450524F ' ID3V2ProducedNotice
DEF &ID3F_TPUB $54505542 'Publisher
DEF &ID3F_TRCK $5452434B 'Track number/Position in set
DEF &ID3F_TRDA $54524441 'Recording dates
DEF &ID3F_TRSN $5452534E 'Internet radio station name
DEF &ID3F_TRSO $5452534F 'Internet radio station owner
DEF &ID3F_TSIZ $5453495A 'Size  ID3V2Size
DEF &ID3F_TSO2 $54534F32 'itunes  ID3V2AlbumArtistSort
DEF &ID3F_TSOA $54534F41 'itunes   ID3V2AlbumSort
DEF &ID3F_TSOC $54534F43 'itunes  ID3V2ComposerSort
DEF &ID3F_TSOP $54534F50 'itunes  ID3V2ArtistSort
DEF &ID3F_TSOT $54534F54 'itunes  ID3V2TitleSort
DEF &ID3F_TSRC $54535243 'ISRC (international standard recording code)]
DEF &ID3F_TSSE $54535345 'Software/Hardware and settings used for encoding]  ID3V2EncodingSettings
DEF &ID3F_TSST $54535354 ' ID3V2SubtitleSet
DEF &ID3F_TXXX $54585858 'User defined text information frame ID3V2AddUserTextW
DEF &ID3F_TYER $54594552 'Year
DEF &ID3F_UFID $55464944 'Unique file identifier
DEF &ID3F_USER $55534552 'ID3V2UserFrameW (Terms of use)
DEF &ID3F_USLT $55534C54  'Unsychronized lyric/text transcription ID3V2Lyrics
DEF &ID3F_WCOM $57434F4D ' ID3V2CommercialURL
DEF &ID3F_WCOP $57434F50 ' ID3V2CopyrightURL
DEF &ID3F_WOAF $574F4146 ' ID3V2AudiofileURL,Urlframe
DEF &ID3F_WOAR $574F4152 ' ID3V2ArtistURL, Urlframe
DEF &ID3F_WOAS $574F4153 ' ID3V2AudiosourceURL
DEF &ID3F_WORS $574F5253 ' ID3V2RadiostationURL
DEF &ID3F_WPAY $57504159 ' ID3V2PaymentURL
DEF &ID3F_WPUB $57505542 ' ID3V2PublisherURL
DEF &ID3F_WXXX $57585858 'ID3V2AddUserURLW
Def &ID3F_PCST $50435354 'ITUNES PODCAST
DEF &ID3F_TCMP $54434D50 'ITUNES COMPILATION
DEF &ID3F_TDES $54444553 'ITUNES PODCAST DESC
DEF &ID3F_TGID $54474944 'ITUNES PODCAST ID
DEF &ID3F_WFED $57464544 'ITUNES PODCAST
DEF &ID3F_TCAT $54434154 'ITUNES PODCAST CATEGORY
DEF &ID3F_NCO $4E434F00 'Uknown binary data ?
DEF &ID3F_NCON $4E434F4E 'Uknown binary data ?
DEF &ID3F_XSOP $58534F50 'Musicbrainz Sortname
DEF &ID3F_XDOR $58444F52 'Release date
DEF &ID3F_MCD $4D434400 'Music CD identifier
DEF &ID3F_CM1 $434D3100 'Comment ?
DEF &ID3F_PRI $50524900
'für setConfigvalueW
DEF &CF_MPEGEXACTREAD 0
DEF &CF_ID3V2PADDINGSIZE 1
DEF &CF_ID3V2WRITEBLOCKSIZE 2
DEF &CF_DOEVENTSMILLIS 3
DEF &MAXTEXTBUFFER 4
DEF &CF_MAXTEXTBUFFER 4
DEF &CF_WMAPADDINGSIZE 5
DEF &CF_MP4PADDINGSIZE 6
DEF &CF_ANSICODEPAGE 7 'code page of ISO-8859-1 text, default 1252, 0 = code page of the system
DEF &CF_ID3V2LINKEDPICTURES 8 '1 = read pictures that an ID3v2 frame links to
DEF &CF_ID3V1MAXTEXTLENGTH 9 '30 to 90, longest title, artist and album of an ID3v1 tag (more than 30 characters: enhanced tag)

Proc GetTextframeConst 'Typ,Tagname übergeben erst ab XProfan 12 wegen explode() mit leerstring
parameters Type$,Frame$   'erstellt die obigen Konstanten aus dem Tagnamen und liest die Werte aus.
type$ = upper$(type$):var a! = 0
case len(frame$) < 3:return
declare id$[],id&[],result$
id$[] = explode(frame$,"")
whileloop 0,sizeof(id$[])-1
id&[&loop] = ord(id$[&loop])
endwhile
var d% = Get("Decimals")
set("Decimals",0)
case len(frame$) = 4:a! = (id&[0] * 2^24  +id&[1] * 2^16 + id&[2]*2^8 +id&[3])
select type$
caseof "MP4"
if frame$ = "dsk"
result$ = MP4GetCDNumber()
else
result$ = MP4GetTextFrame(a!)
endif
caseof "ID3V2"
result$ = ID3V2GetTextFrame(a!)
caseof "WAV"
result$ = WAVGetTextFrame(a!)
endselect
set("Decimals",d%)
return result$
endproc

Proc SetTextframeConst 'Typ,Tagname,Wert übergeben erst ab XProfan 12 wegen explode() mit leerstring
parameters Type$,Frame$,wert$ 'erstellt die obigen Konstanten und schreibt die Werte
type$ = upper$(type$):var a! = 0
case len(frame$) < 3:return
declare id$[],id&[]
id$[] = explode(frame$,"")
whileloop 0,sizeof(id$[])-1
id&[&loop] = ord(id$[&loop])
endwhile
var d% = Get("Decimals")
set("Decimals",0)
case len(frame$) = 4:a! = (id&[0] * 2^24  +id&[1] * 2^16 + id&[2]*2^8 +id&[3])
select type$
caseof "MP4"
wert$ = AnsiToWide(wert$)
if frame$ = "dsk"
MP4SetTextFrameW(&MP4_CDSETNUMBER,addr(wert$))
else
MP4SetTextFrameW(a!,addr(wert$))
endif
caseof "ID3V2"
ID3V2SetTextFrame(a!,wert$)
caseof "WAV"
WAVSetTextFrame(a!,wert$)
endselect
set("Decimals",d%)
endproc

Proc MilliToMin
var ms& = AUDIOGetDurationMillisW()
Declare mem#,h$,m$,s$,ms&
dim mem#,50
var ch% = External("Shlwapi","StrFromTimeIntervalA",mem#,50,ms&,6)
if ch%
var t$ = Trim$(string$(mem#,0))
dispose mem#
t$ = Upper$(t$)
t$ = Translate$(t$,"STD.","HR")
t$ = Translate$(t$,"MIN.","MIN")
t$ = Translate$(t$,"SEK.","SEC")
if instr("HR",t$)
h$ = trim$(substr$(t$,1,"HR"))
t$ = Del$(t$,1,Len(h$)+3)
else
h$ = "00"
endif
if instr("MIN",t$)
m$ = Trim$(substr$(t$,1,"MIN"))
t$ = Del$(t$,1,Len(m$)+5)
else
m$ = "00"
endif
if instr("SEC",t$)
s$ = trim$(substr$(t$,1,"SEC"))
else
s$ = "00"
endif
case len(h$) = 1:h$ = "0"+h$
case len(m$) = 1:m$ = "0"+m$
case len(s$) = 1:s$ = "0"+s$
else
h$ = "00":m$ = "00":s$ = "00"
endif
Return h$+":"+m$+":"+s$
endproc

Proc AnsiToWide
   Parameters Ansi$
   case ansi$ = "":return ""
   Declare Wide$
   WhileLoop 1, Len(Ansi$)
      Wide$ = Wide$ + Substr$(Ansi$, &loop) + "\z"
   EndWhile
   case wide$ <> "":wide$ = wide$ +"\z"
   Return Wide$
EndProc

Proc WideToAnsi
   Parameters buf&
   case buf& = 0:return ""
   Declare I%, Ansi$, WChar$
   I% = 0
   WChar$ = Char$(buf&, I%, 2)
   While WChar$ <> "\z\z"
      Ansi$ = Ansi$ + Char$(buf&, I%, 1)
      Inc I%, 2
      WChar$ = Char$(buf&, I%, 2)
   EndWhile
  ansi$ = Translate$(ansi$,chr$(255),"")
  ansi$ = Translate$(ansi$,chr$(254),"")
   Return Ansi$
EndProc

proc initaudiogenie
parameters dll$
Set("ErrorLevel", -1) 'Profan >= 10
Set("OnError", 1) ' Profan >= 10
Declare temp&
temp& = ImportDLL(dll$, "")
Set("ErrorLevel",0) 'Profan >= 10
Set("OnError", 0)
return temp&
endproc

proc deinitaudiogenie
parameters temp&
Freedll temp&
endproc

proc BildType
parameters hdl&
addstring(hdl&,"Other")
addstring(hdl&,"32*32 pixels file icon")
addstring(hdl&,"Other file icon")
addstring(hdl&,"Cover front")
addstring(hdl&,"Cover back")
addstring(hdl&,"Leaflet page")
addstring(hdl&,"Media ( label side of CD )")
addstring(hdl&,"Lead artist/solist")
addstring(hdl&,"Artist/Performer")
addstring(hdl&,"Conductor")
addstring(hdl&,"Band/Orchestra")
addstring(hdl&,"Composer 11")
addstring(hdl&,"Lyricist/text writer")
addstring(hdl&,"Recording Location")
addstring(hdl&,"During recording")
addstring(hdl&,"During performance")
addstring(hdl&,"Movie/video screen capture")
addstring(hdl&,"A bright coloured fish")
addstring(hdl&,"Illustration")
addstring(hdl&,"Band/artist logotype")
addstring(hdl&,"Publisher/Studio logotype")
addstring(hdl&,"undefined")
endproc

Proc Pictypenr 'select ab Version 10 sonst If/endif
Parameters type$
type$ = upper$(type$)
select type$
caseof Upper$("Other")
Return 0
caseof Upper$("32*32 pixels file icon")
Return 1
caseof Upper$("Other file icon")
Return 2
caseof  Upper$("Cover front")
Return 3
caseof Upper$("Cover back")
Return 4
caseof Upper$("Leaflet page")
Return 5
Caseof Upper$("Media ( label side of CD )")
Return 6
caseof Upper$("Lead artist/solist")
Return 7
caseof Upper$("Artist/Performer")
Return 8
caseof Upper$("Conductor")
Return 9
caseof Upper$("Band/Orchestra")
Return 10
caseof Upper$("Composer")
Return 11
Caseof Upper$("Lyricist/text writer")
Return 12
caseof Upper$("Recording Location")
Return 13
caseof Upper$("During recording")
Return 14
caseof Upper$("During performance")
Return 15
caseof Upper$("Movie/video screen capture")
Return 16
caseof Upper$("A bright coloured fish")
Return 17
caseof Upper$("Illustration")
Return 18
caseof Upper$("Band/artist logotype")
Return 19
caseof Upper$("Publisher/Studio logotype")
Return 20
otherwise
Return 0
endselect
endproc

Proc Pictype 'select ab Version 10 sonst If/endif
Parameters type&
select type&
caseof 0
Return "Other"
caseof 1
Return "32*32 pixels file icon"
caseof 2
Return "Other file icon"
caseof 3
Return "Cover front"
caseof 4
Return "Cover back"
caseof 5
Return "Leaflet page"
Caseof 6
Return "Media ( label side of CD )"
caseof 7
Return "Lead artist/solist"
caseof 8
Return "Artist/Performer"
caseof 9
Return "Conductor"
caseof 10
Return "Band/Orchestra"
caseof 11
Return "Composer"
Caseof 12
Return "Lyricist/text writer"
caseof 13
Return "Recording Location"
caseof 14
Return "During recording"
caseof 15
Return "During performance"
caseof 16
Return "Movie/video screen capture"
caseof 17
Return "A bright coloured fish"
caseof 18
Return "Illustration"
caseof 19
Return "Band/artist logotype"
caseof 20
Return "Publisher/Studio logotype"
otherwise
Return "undefined"
endselect
endproc

Proc comercialrecieved
parameters type%
select type%
caseof 0
return "Other"
caseof 1
return "Standard CD Album with other songs"
caseof 2
return "Compressed audio on CD"
caseof 3
return "File over the Internet"
caseof 4
return "Stream over the Internet"
caseof 5
return "As note sheets"
caseof 6
return "As note sheets in a book with other sheets"
caseof 7
return "Music on other media"
caseof 8
return "Non-musical merchandise"
Otherwise
Return "Fehler"
endselect
endproc

proc SyncLyricContentType
parameters wert&
select wert&
caseof -1
return "ungültiger Eintrag"
caseof 0
return "Sonstiges"
caseof 1
return "Songtexte"
caseof 2
return "Textumschreibungen"
caseof 3
return "Bewegungen"
caseof 4
return "Ereignisse"
caseof 5
return "Akkorde"
caseof 6
return "Belangloses"
caseof 7
return "URLs zu Webseiten"
caseof 8
return "URLs zu Bildern"
otherwise
Return "nicht erkannt"
endselect
endproc
'-----------------Audio------------------------
Proc Audiofileanalyze
Parameters file$
var temp$ = file$
file$ = Ansitowide(file$)
Return Loword(AUDIOAnalyzeFileW(Addr(File$)))
endproc

proc  AUDIOIsValidFormat '-1 gültig 0 ungültig
return  AUDIOIsValidFormatW()
endproc

Proc AudioGetfilesize
return AUDIOGetFileSizeW()
endproc

Proc AudioGetversion
var tmp& = AUDIOGetVersionW()
Return WideToAnsi(tmp&)
endproc

Proc AudioGetTitle
Var tmp& = AUDIOGetTitleW()
Return WideToAnsi(tmp&)
endproc

Proc AudioGetArtist
var tmp& = AUDIOGetArtistW()
Return WideToAnsi(tmp&)
endproc

Proc AudioGetAlbum
var tmp& = AUDIOGetAlbumW()
Return WideToAnsi(tmp&)
endproc

Proc AudioGetYear
Var tmp& = AUDIOGetYearW()
Return WideToAnsi(tmp&)
endproc

Proc AudioGetTrack
Var tmp& = AUDIOGetTrackW()
Return WideToAnsi(tmp&)
endproc

Proc AudioGetGenre
Var tmp& =  AUDIOGetGenreW()
Return WideToAnsi(tmp&)
endproc

Proc AudioGetComment
var tmp& = AUDIOGetCommentW()
Return WideToAnsi(tmp&)
endproc

Proc AudioGetBitrate
Return AUDIOGetBitrateW()
endproc

Proc AudioGetSampleRate
Return AUDIOGetSampleRateW()
endproc

Proc AudioGetChannelMode
var tmp& = AUDIOGetChannelModeW()
Return WideToAnsi(tmp&)
endproc

Proc AudioGetChannels
return AUDIOGetChannelsW()
endproc

Proc AudioGetLastfile
var tmp& = AUDIOGetLastFileW()
Return WideToAnsi(tmp&)
endproc

Proc AudioGetMD5Value
Var tmp& = AUDIOGetMD5ValueW()
Return WideToAnsi(tmp&)
endproc

Proc AudioGetLastErrorNumber
Return AUDIOGetLastErrorNumberW()
endproc

Proc AudioGetLastErrorText
var tmp& = AUDIOGetLastErrorTextW()
Return WideToAnsi(tmp&)
endproc

Proc AudioGetDuration
var tmp& = AUDIOGetDurationMillisW()
tmp& = Tmp&/1000
return tmp&
endproc

Proc AudioSetAlbum
parameters wert$
wert$ = AnsiToWide(wert$)
AUDIOSetAlbumW(addr(wert$))
endproc

Proc AudioSetArtist
parameters wert$
wert$ = AnsiToWide(wert$)
AUDIOSetArtistW(addr(wert$))
endproc

Proc AudioSetComment
parameters wert$
wert$ = AnsiToWide(wert$)
AUDIOSetCommentW(addr(wert$))
endproc

Proc AudioSetGenre
parameters wert$
wert$ = AnsiToWide(wert$)
AUDIOSetGenreW(addr(wert$))
endproc

Proc AudioSetTitle
parameters wert$
wert$ = AnsiToWide(wert$)
AUDIOSetTitleW(addr(wert$))
endproc

Proc AudioSetTrack
parameters wert$
wert$ = AnsiToWide(wert$)
AUDIOSetTrackW(addr(wert$))
endproc

Proc AudioSetYear
parameters wert$
wert$ = AnsiToWide(wert$)
AUDIOSetYearW(addr(wert$))
endproc

Proc AudioSaveChanges
var tmp& = AUDIOSaveChangesW()
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler beim Schreiben mit Audio-Funktion (AudioSaveChanges)"
'case tmp& = 0:Messagebox("Beim Schreiben ist ein Fehler aufgetreten",error$,262144+48)
endproc

Proc AudioSaveChangesToFile
Parameters wert$
wert$ = AnsiToWide(wert$)
var tmp& = AUDIOSaveChangesToFileW(addr(wert$))
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler beim Schreiben mit Audio-Funktion (AudioSaveChangesToFile)"
'case tmp& = 0:Messagebox("Beim Schreiben des Tags ist ein Fehler aufgetreten",error$,262144+48)
endproc

'----------------------ID3V1----------------------------
Proc ID3V1Exists
var tmp% = ID3V1ExistsW()
if tmp% = -1
return "Ja"
else
return "Nein"
endif
endproc

proc ID3V1GetVersion
var tmp& = ID3V1GetVersionW()
Return WideToAnsi(tmp&)
endproc

Proc ID3V1GetGenreItem
parameters number&
var tmp& = ID3V1GetGenreItemW(number&)
Return WideToAnsi(tmp&)
endproc

Proc ID3V1GetGenres
return ID3V1GetGenresW()
endproc

Proc ID3V1GetTitle
var tmp& = ID3V1GetTitleW()
Return WideToAnsi(tmp&)
endproc

Proc ID3V1GetArtist
var tmp& = ID3V1GetArtistW()
Return WideToAnsi(tmp&)
endproc

Proc ID3V1GetAlbum
var tmp& = ID3V1GetAlbumW()
Return WideToAnsi(tmp&)
endproc

Proc ID3V1GetYear
Var tmp& = ID3V1GetYearW()
Return WideToAnsi(tmp&)
endproc

Proc ID3V1GetGenre
Var tmp& = ID3V1GetGenreW()
Return WideToAnsi(tmp&)
endproc

Proc ID3V1GetComment
var tmp& = ID3V1GetCommentW()
Return WideToAnsi(tmp&)
endproc

Proc ID3V1GetTrack
var tmp& = ID3V1GetTrackW()
Return WideToAnsi(tmp&)
endproc

Proc ID3V1GetGenreID
Return ID3V1GetGenreIDW()
endproc

Proc ID3V1RemoveTag
var tmp& = ID3V1RemoveTagW()
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler beim Tag entfernen. (ID3V1RemoveTag)"
'case tmp& = 0:Messagebox(error$,"Beim Entfernen des Tags ist ein Fehler aufgetreten",262144+48)
endproc

Proc ID3V1RemoveTagFromFile
parameters wert$
wert$ = AnsiToWide(wert$)
Var tmp& = ID3V1RemoveTagFromFileW(addr(wert$))
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler beim Tag entfernen. (ID3V1RemoveTagFromFile)"
'case tmp& = 0:Messagebox(error$,"Beim Entfernen des Tags ist ein Fehler aufgetreten",262144+48)
endproc

Proc ID3V1SaveChanges
Var tmp& = ID3V1SaveChangesW()
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler beim Tag schreiben. (ID3V1SaveChanges)"
'case tmp& = 0:Messagebox(error$,"Fehler beim Schreiben",262144+48)
endproc

Proc ID3V1SaveChangesToFile
Parameters wert$
wert$ = AnsiToWide(wert$)
var tmp& = ID3V1SaveChangesToFileW(addr(wert$))
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler beim Tag schreiben. (ID3V1SaveChangesToFile)"
'case tmp& = 0:Messagebox(error$,"Fehler beim Schreiben",262144+48)
endproc

Proc ID3V1SetAlbum
Parameters wert$
wert$ = AnsiToWide(wert$)
ID3V1SetAlbumW(addr(wert$))
endproc

Proc ID3V1SetArtist
Parameters wert$
wert$ = AnsiToWide(wert$)
ID3V1SetArtistW(addr(wert$))
endproc

Proc ID3V1SetComment
Parameters wert$
wert$ = AnsiToWide(wert$)
ID3V1SetCommentW(addr(wert$))
endproc

Proc ID3V1SetGenre
Parameters wert$
wert$ = AnsiToWide(wert$)
ID3V1SetGenreW(addr(wert$))
endproc

Proc ID3V1SetGenreID
Parameters wert&
ID3V1SetGenreIDW(wert&)
endproc

Proc ID3V1SetTitle
Parameters wert$
wert$ = AnsiToWide(wert$)
ID3V1SetTitleW(addr(wert$))
endproc

Proc ID3V1SetTrack
Parameters wert$
wert$ = AnsiToWide(wert$)
ID3V1SetTrackW(addr(wert$))
endproc

Proc ID3V1SetYear
Parameters wert$
wert$ = AnsiToWide(wert$)
ID3V1SetYearW(addr(wert$))
endproc
'--------------------------ID3V2----------------------------
Proc ID3V2Exists
Var tmp% = ID3V2ExistsW()
if tmp% = -1
Return "Ja"
else
Return "Nein"
endif
endproc

proc ID3V2GetVersion
var tmp& = ID3V2GetVersionW()
Return WideToAnsi(tmp&)
endproc

Proc ID3V2GetSize
Return ID3V2GetSizeW()
endproc

Proc ID3V2GetAllFrameIDs
var tmp& = ID3V2GetAllFrameIDsW()
var liste$ = WideToAnsi(tmp&)
'Xprofan < 11 hier return liste$
Declare list$[]
list$[] = explode(liste$,",")
if sizeof(list$[]) > 0
whileloop 0,sizeof(list$[]) -1
list$[&loop] = Trim$(list$[&loop])
endwhile
endif
return list$[]
endproc

proc ID3V2SetTextFormatToAnsi
ID3V2SetFormatAndEncodingW(0,0)
endproc

proc ID3V2SetTextFormatToUnicode
ID3V2SetFormatAndEncodingW(0,1)
endproc

Proc ID3V2GetTextFrame
parameters id&
var tmp& = ID3V2GetTextFrameW(id&)
Return WideToAnsi(tmp&)
endproc

Proc ID3V2GetSubFrameText
parameters pID$,sID&
pID$ = AnsiToWide(pID$)
var tmp& = ID3V2GetSubFrameTextW(addr(pID$),sID&)
return WidetoAnsi(tmp&)
endproc

Proc ID3V2GetURLFrame
Parameters id&
var tmp& =  ID3V2GetURLFrameW(id&)
Return WideToAnsi(tmp&)
endproc

Proc ID3V2GetSubFrames 'Anzahl Unterframes  CTOC, CHAP usw
parameters id$
id$ = AnsiToWide(id$)
Return ID3V2GetSubFramesW(addr(id$))
'Int16 die Anzahl der Subframes oder -1 bei Fehler
endproc

Proc ID3V2SetTextFrame
Parameters id&,text$
text$ = AnsitoWide(text$)
ID3V2SetTextFrameW(id&,addr(text$))
endproc

proc ID3V2SetTextSubFrame
parameters pID$,sID&,text$,desc$
pID$ = AnsiToWide(pID$)
text$ = AnsitoWide(text$)
desc$ = AnsitoWide(desc$)
ID3V2SetTextSubFrameW(addr(pID$),sID&,addr(text$),addr(desc$))
endproc

Proc ID3V2GetSubFrameDescription
parameters id$,index%
id$ = AnsiToWide(id$)
var tmp& = ID3V2GetSubFrameDescriptionW(addr(id$),index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2GetSubFrameID
Parameters id$,index%
id$ = AnsiToWide(id$)
return ID3V2GetSubFrameIDW(addr(id$),index%)
'Int32 die Frame ID oder 0 falls übergeordneter Frame nicht gefunden
endproc

Proc ID3V2GetSubFrameImageType
parameters id$,index%
id$ = AnsiToWide(id$)
var tmp& = ID3V2GetSubFrameImageTypeW(addr(id$),index%)
return Pictype(tmp&) 'String Pictype
endproc

Proc ID3V2GetSubFrameImage
parameters array#,size&,id$,index%
id$ = AnsiToWide(id$)
Return ID3V2GetSubFrameImageW(array#,size&,addr(id$),index%)
'Int32 Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

Proc ID3V2GetSubFrameType
parameters id$,index%
id$ = AnsiToWide(id$)
Return ID3V2GetSubFrameTypeW(addr(id$),index%)
'Int16 1=normaler Textframe T000, 2=Benutzerdefinierter Textframe TXXX, 3=normaler URL Frame W000,
'4=Benutzerdefinierter URL Frame WXXX, 5=Bild APIC, -1=ID nicht gefunden
endproc

Proc ID3V2AddChildElement
Parameters ParentTocID$,ChildID$
ParentTocID$ = AnsiToWide(ParentTocID$)
ChildID$ = AnsiToWide(ChildID$)
Return ID3V2AddChildElementW(addr(ParentTocID$),addr(ChildID$))
'Int16 normalerweise -1, bei Fehler 0
endproc

Proc ID3V2DeleteAddendum 'löscht ein CTOC oder CHAP Element und alle Unterverweise
Parameters id$
id$ = AnsiToWide(id$)
Return ID3V2DeleteAddendumW(addr(id$))
'Int16 normalerweise -1, 0 falls ID nicht gefunden
endproc

Proc ID3V2DeleteAllFrames
Parameters id&
Return ID3V2DeleteAllFramesW(id&)
'Int16 normalerweise -1, 0 falls ID nicht gefunden
endproc

Proc ID3V2DeleteChildElement
Parameters ParentTocID$,ChildID$
ParentTocID$ = AnsiToWide(ParentTocID$)
ChildID$ = AnsiToWide(ChildID$)
Return ID3V2DeleteChildElementW(addr(ParentTocID$),addr(ChildID$))
'Int16 normalerweise -1, 0 falls Element nicht gefunden
endproc

Proc ID3V2DeleteSelectedFrame
Parameters id&,index%
Return ID3V2DeleteSelectedFrameW(id&,index%)
'Int16 normalerweise -1, 0 falls Frame nicht gefunden
endproc

proc ID3V2DeleteSubFrame
Parameters id$,frameid&
id$ = AnsiToWide(id$)
Return ID3V2DeleteSubFrameW(addr(id$),frameid&)
'Int16 normalerweise -1, bei Fehler 0
endproc

proc ID3V2SetURLFrame
Parameters id&,text$
text$ = AnsitoWide(text$)
ID3V2SetURLFrameW(id&,addr(text$))
endproc

Proc ID3V2SetURLSubFrame
Parameters pid$,sid&,url$,desc$
Pid$ = AnsiToWide(pid$)
url$ = AnsiToWide(url$)
desc$ = AnsiToWide(desc$)
ID3V2SetURLSubFrameW(addr(pid$),sid&,addr(url$),addr(desc$))
endproc

Proc ID3V2SetFormatAndEncoding '0=bestehendes Format, 1=id3v2.2, 2=id3v2.3, 3=id3v2.4
Parameters format&,enc& '0= ISO-8859-1, 1=UTF-16 mit BOM, 2=UTF-16 ohne BOM, 3=UTF-8
var tmp& = ID3V2SetFormatAndEncodingW(format&,enc&)
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler bei Format (ID3V2SetFormatAndEncoding)"
'case tmp& = 0:Messagebox(error$,"Die Umstellung war nicht erfolgreich",262144+48)
endproc

proc ID3V2GetFrameCount 'Gibt Anzahl Frames zurück
parameters id&
Return ID3V2GetFrameCountW(id&)
endproc

proc ID3V2GetTitle
Return ID3V2GetTextFrame(&ID3F_TIT2)
endproc

Proc ID3V2GetArtist
Return ID3V2GetTextFrame(&ID3F_TPE1)
endproc

Proc ID3V2GetAlbum
Return ID3V2GetTextFrame(&ID3F_TALB)
endproc

Proc ID3V2GetDate
Return ID3V2GetTextFrame(&ID3F_TDAT)
endproc

Proc ID3V2GetGenre
Var tmp& = ID3V2GetGenreW()
Return WideToAnsi(tmp&)
endproc

Proc ID3V2GetTrack
Return ID3V2GetTextFrame(&ID3F_TRCK)
endproc

Proc ID3V2GetYear
Return ID3V2GetTextFrame(&ID3F_TYER)
endproc

Proc ID3V2SetUserUrl
Parameters url$
ID3V2SetURLFrame
endproc

Proc ID3V2AddAudioEncryption
parameters Array#,len&,url$,previewstart&,previewLength&
url$ = AnsiToWide(url$)
return ID3V2AddAudioEncryptionW(array#,len&,addr(url$),previewstart&,previewLength&)
'-1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc ID3V2GetAudioEncryptionData
Parameters array#,len&,index%
return ID3V2GetAudioEncryptionDataW(array#,len&,index%)
'Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

Proc ID3V2GetAudioEncryptionPreviewLength
parameters index%
return ID3V2GetAudioEncryptionPreviewLengthW(index%)
endproc

Proc ID3V2GetAudioEncryptionPreviewStart
parameters index%
Return ID3V2GetAudioEncryptionPreviewStartW(index%)
endproc

Proc ID3V2GetAudioEncryptionURL
parameters index%
var tmp& = ID3V2GetAudioEncryptionURLW(index%)
return widetoansi(tmp&)
endproc

Proc ID3V2AddPictureArray
parameters array#,len&,desc$,ptyp&
desc$ = AnsiToWide(desc$)
return ID3V2AddPictureArrayW(array#,len&,addr(desc$),ptyp&)
'-1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetPictureSize
parameters index%
return ID3V2GetPictureSizeW(index%)
endproc

Proc ID3V2AddPictureFile
parameters file$,desc$,ptyp%,aslink% '-1 es wird nur der Dateiname, 0 = das Bild selbst
file$ = AnsiToWide(file$)
desc$ = AnsiToWide(desc$)
var tmp& = ID3V2AddPictureFileW(addr(file$),addr(desc$),ptyp%,aslink%)
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler Bild einfügen (ID3V2AddPictureFile)"
'case tmp& = 0:Messagebox("Das Bild konnte nicht eingefügt werden",error$,262144+64)
endproc

Proc ID3V2GetPictureArray
parameters array#,len&,index%
return ID3V2GetPictureArrayW(array#,len&,index%)
endproc

proc ID3V2GetPictureDescription
parameters index%
var tmp& = ID3V2GetPictureDescriptionW(index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2GetPictureFile 'bild in datei
parameters file$,index%
file$ = AnsiToWide(file$)
Var tmp% =  ID3V2GetPictureFileW(addr(file$),index%)
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler beim Bild einlesen ( ID3V2GetPictureFile)"
'case tmp% = 0:Messagebox(error$,"Es ist ein Fehler aufgetreten",262144+64)
return tmp%
endproc

Proc ID3V2GetPictureMime
parameters index%
Var tmp& = ID3V2GetPictureMimeW(index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2GetPictureTypeText
parameters index%
var tmp& = ID3V2GetPictureTypeTextW(index%)
Return WideToAnsi(tmp&)
endproc

Proc ID3V2GetPictureType
Parameters index%
Return ID3V2GetPictureTypeW(index%)
endproc

Proc ID3V2GetPicturecount 'Anzahl Pictures
return ID3V2GetFrameCountW(&ID3F_APIC)
endproc

proc ID3V2DeletePictures
return ID3V2DeleteAllFrames(&ID3F_APIC)
endproc

proc ID3V2DeletePicture
parameters index%
return ID3V2DeleteSelectedFrame(&ID3F_APIC,index%)
endproc

Proc ID3V2AddAudioSeekPoint
parameters array#,len&,start&,length&,numbers&,BitsPerPoint%
Return ID3V2AddAudioSeekPointW(array#,len&,start&,length&,numbers&,BitsPerPoint%)
'-1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc ID3V2GetAudioSeekPointBitsPerIndexpoint
return ID3V2GetAudioSeekPointBitsPerIndexpointW()
endproc

Proc ID3V2GetAudioSeekPointData
parameters array#,len&
return ID3V2GetAudioSeekPointDataW(array#,len&)
'Int32 Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

proc ID3V2GetAudioSeekPointLength
Return ID3V2GetAudioSeekPointLengthW()
endproc

Proc ID3V2GetAudioSeekPointNumber
Return ID3V2GetAudioSeekPointNumberW()
endproc

Proc ID3V2GetAudioSeekPointStart
return ID3V2GetAudioSeekPointStartW()
endproc

Proc ID3V2AddChapter
Parameters id$,title$,desc$,startTime&,endTime&
id$ = AnsiToWide(id$)
title$ = AnsiToWide(title$)
desc$ = AnsiToWide(desc$)
return ID3V2AddChapterW(addr(id$),addr(title$),addr(desc$),startTime&,endTime&)
'-1 falls Element ersetzt wurde, ansonsten 0
endproc

proc ID3V2GetChapterEndTime
Parameters id$
Id$ = AnsiToWide(id$)
return ID3V2GetChapterEndTimeW(addr(id$))
endproc

Proc ID3V2GetChapterStartTime
parameters id$
Id$ = AnsiToWide(id$)
return ID3V2GetChapterStartTimeW(addr(id$))
endproc

Proc ID3V2GetPossibleCHAPID
Var tmp& = ID3V2GetPossibleCHAPIDW()
return WideToAnsi(tmp&)
endproc

Proc ID3V2SetChapterTimes
parameters ID$,StartTime&,endTime&
id$ = AnsiToWide(id$)
Return ID3V2SetChapterTimesW(addr(id$),StartTime&,endTime&)
'-1, 0 falls ID nicht gefunden oder Element kein CHAP Element ist
endproc

proc ID3V2AddComment
Parameters lang$,desc$,text$ 'lang$ = 3 Byte z.B. ger, eng
lang$ = AnsiToWide(lang$)
desc$ = AnsiToWide(desc$)
text$ = AnsiToWide(text$)
return  loword(ID3V2AddCommentW(addr(lang$),addr(desc$),addr(text$)))
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc ID3V2GetCommentDescription
parameters index%
var tmp& = ID3V2GetCommentDescriptionW(index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2GetCommentLanguage
parameters index%
var tmp& = ID3V2GetCommentLanguageW(index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2GetComment
Parameters index%
var tmp& = ID3V2GetCommentW(index%)
Return WideToAnsi(tmp&)
endproc

Proc ID3V2GetComments 'Anzahl Comments
Return ID3V2GetFrameCountW(&ID3F_COMM)
endproc

Proc ID3V2AddCommercialFrame
parameters array#,len&,price$,validUntil$,contactUrl$,receivedAs%,seller$,desc$
price$ = AnsiToWide(price$)
validUntil$ = AnsiToWide(validUntil$)
contactUrl$ = AnsiToWide(contactUrl$)
seller$ = AnsiToWide(seller$)
desc$ = AnsiToWide(desc$)
return ID3V2AddCommercialFrameW(array#,len&,addr(price$),addr(validUntil$),addr(contactUrl$),receivedAs%,addr(seller$),addr(desc$))
'-1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc ID3V2GetCommercialFrameContactURL
Parameters index%
var tmp& = ID3V2GetCommercialFrameContactURLW(index%)
return WideToAnsi(tmp&)
endproc

proc ID3V2GetCommercialFrames 'Anzahl Frames
return ID3V2GetFrameCountW(&ID3F_COMR) 'Im Wrapper vergessen
endproc

proc ID3V2GetCommercialFrameDescription
parameters index%
var tmp& = ID3V2GetCommercialFrameDescriptionW(index%)
return wideToAnsi(tmp&)
endproc

Proc ID3V2GetCommercialFramePictureMime
Parameters index%
var tmp& = ID3V2GetCommercialFramePictureMimeW(index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2GetCommercialFramePicture
parameters array#,len&,index%
Return ID3V2GetCommercialFramePictureW(array#,len&,index%)
'Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

Proc ID3V2GetCommercialFramePrice
parameters index%
var tmp& = ID3V2GetCommercialFramePriceW(index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2GetCommercialFrameReceivedAs
parameters index%
var tmp% = ID3V2GetCommercialFrameReceivedAsW(index%)
return comercialrecieved(tmp%)
endproc

Proc ID3V2GetCommercialFrameSellerName
parameters index%
var tmp& = ID3V2GetCommercialFrameSellerNameW(index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2GetCommercialFrameValidUntil
parameters index%
var tmp& = ID3V2GetCommercialFrameValidUntilW(index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2AddTableOfContent '-1 wenn die Untereinträge sortiert sind oder 0 falls unsortiert
parameters id$,title$,desc$,sort%
Id$ = AnsiToWide(id$)
title$ = AnsiToWide(title$)
desc$ = AnsiToWide(desc$)
Return ID3V2AddTableOfContentW(addr(id$),addr(title$),addr(desc$),sort%)
'-1 falls Element ersetzt wurde, ansonsten 0
endproc

Proc ID3V2GetPossibleTOCID
var tmp& = ID3V2GetPossibleTOCIDW()
Return WideToAnsi(tmp&)
endproc

Proc ID3V2GetTOCIsOrdered
parameters id$
Id$ = AnsiToWide(id$)
return ID3V2GetTOCIsOrderedW(addr(id$))
'0=nicht sortiert, -1=sortiert
endproc

proc ID3V2GetTOCRootID
var tmp& = ID3V2GetTOCRootIDW()
return WideToAnsi(tmp&)
endproc

Proc ID3V2SetTOCIsOrdered '0=nicht sortiert -1=sortiert
parameters id$,sort%
id$ = AnsiToWide(id$)
return ID3V2SetTOCIsOrderedW(addr(id$),sort%)
'normalerweise -1, 0 falls ID nicht gefunden
endproc

Proc ID3V2AddEncryption
Parameters array#,len&,url$,symbol%
url$ = AnsiToWide(url$)
Return ID3V2AddEncryptionW(array#,len&,addr(url$),symbol%)
'-1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc ID3V2GetEncryptionData
parameters array#,len&,index%
return ID3V2GetEncryptionDataW(array#,len&,index%)
'Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

Proc ID3V2GetEncryptionFrames 'Anzahl Frames
Return ID3V2GetFrameCountW(&ID3F_AENC)
endproc

Proc ID3V2GetEncryptionSymbol
parameters index%
return ID3V2GetEncryptionSymbolW(index%)
endproc

proc ID3V2GetEncryptionURL
parameters index%
var tmp& = ID3V2GetEncryptionURLW(index%)
return WidetoAnsi(tmp&)
endproc

proc ID3V2AddEqualisation
parameters array#,len&,ip%,Ident$ 'ip% = 0=Band, 1=Linear
ident$ = AnsiToWide(ident$)
return ID3V2AddEqualisationW(array#,len&,ip%,addr(ident$))
'-1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc ID3V2GetEqualisationAdjustmentBits
parameters index%
return ID3V2GetEqualisationAdjustmentBitsW(index%)
'Wert oder -1 falls Fehler
endproc

Proc ID3V2GetEqualisationData
parameters array#,len&,index%
return ID3V2GetEqualisationDataW(array#,len&,index%)
'Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

proc ID3V2GetEqualisationFrames 'Anzahl Frames
return ID3V2GetFrameCountW(&ID3F_EQUA) 'Welches EQUA or 2
endproc

Proc ID3V2GetEqualisationIdentification
Parameters index%
Var tmp& = ID3V2GetEqualisationIdentificationW(index%)
Return WideToAnsi(tmp&)
endproc

Proc ID3V2GetEqualisationInterpolation
Parameters index%
Return ID3V2GetEqualisationInterpolationW(index%)
'Int16 Wert oder -1 falls Fehler, 0=Band (no interpolation) 1=Linear
endproc

Proc ID3V2AddEventTimingCodes
Parameters array#,len&,timeformat& '1=als Frames 2=als Millisekunden
Return ID3V2AddEventTimingCodesW(array#,len&,timeformat&)
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc ID3V2GetEventTimingCodesData
parameters array#,len&
Return ID3V2GetEventTimingCodesDataW(array#,len&)
'Int32 Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

Proc ID3V2GetEventTimingCodesTimestampFormat
Return ID3V2GetEventTimingCodesTimestampFormatW()
'Int16 Wert oder -1 falls Fehler, 1=als Frames 2=als Millisekunden
endproc

Proc ID3V2AddGeneralObject 'Was ist Filename
Parameters Array#,len&,mime$,file$,desc$
mime$ = AnsiToWide(mime$)
file$ = AnsiToWide(file$)
desc$ = AnsiToWide(desc$)
Return ID3V2AddGeneralObjectW(array#,len&,addr(mime$),addr(file$),addr(desc$))
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetGeneralObjectData
Parameters array#,len&,index%
Return ID3V2GetGeneralObjectDataW(array#,len&,index%)
'Int32 Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

Proc ID3V2GetGeneralObjects 'Anzahl Frames
return ID3V2GetFrameCountW(&ID3F_GEOB)
endproc

Proc ID3V2GetGeneralObjectDescription
Parameters index%
Var tmp& = ID3V2GetGeneralObjectDescriptionW(index%)
Return WideToAnsi(tmp&)
endproc

Proc ID3V2GetGeneralObjectFilename
parameters index%
var tmp& = ID3V2GetGeneralObjectFilenameW(index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2GetGeneralObjectMime
Parameters index%
Var tmp& = ID3V2GetGeneralObjectMimeW(index%)
Return WideToAnsi(tmp&)
endproc

Proc ID3V2AddGroupIdentification
Parameters array#,len&,url$,symbol%
url$ = AnsiToWide(url$)
Return ID3V2AddGroupIdentificationW(array#,len&,addr(url$),symbol%)
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc ID3V2GetGroupIdentificationData
Parameters array#,len&,index%
Return ID3V2GetGroupIdentificationDataW(array#,len&,index%)
'Int32 Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

proc ID3V2GetGroupIdentifications 'Anzahl Frames
Return ID3V2GetFrameCountW(&ID3F_GRID)
endproc

Proc ID3V2GetGroupIdentificationSymbol
parameters index%
return ID3V2GetGroupIdentificationSymbolW(index%)
'Int16 Wert oder -1 falls Fehler
endproc

Proc ID3V2GetGroupIdentificationURL
Parameters index%
Var tmp& = ID3V2GetGroupIdentificationURLW(index%)
Return WideToAnsi(tmp&)
endproc

Proc ID3V2GetAddendumDescription
Parameters id$
id$ = AnsiToWide(id$)
var tmp& = ID3V2GetAddendumDescriptionW(addr(id$))
return WideToAnsi(tmp&)
endproc

Proc ID3V2GetAddendumTitle
Parameters id$
id$ = AnsiToWide(id$)
var tmp& = ID3V2GetAddendumTitleW(addr(id$))
return WideToAnsi(tmp&)
endproc

Proc ID3V2GetAddendumType
parameters id$
id$ = AnsiToWide(id$)
Return ID3V2GetAddendumTypeW(addr(id$))
'0= Element ist ein CHAP Element, 1=Element ist ein CTOC Element, -1=ID nicht gefunden
endproc

Proc ID3V2GetChildElementID
parameters id$,index%
id$ = AnsiToWide(id$)
var tmp& = ID3V2GetChildElementIDW(addr(id$),index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2GetChildElements 'Anzahl
parameters id$
id$ = AnsiToWide(id$)
return ID3V2GetChildElementsW(addr(id$))
endproc

proc ID3V2SetAddendumDescription
parameters id$,desc$
id$ = AnsiToWide(id$)
desc$ = AnsiToWide(desc$)
return ID3V2SetAddendumDescriptionW(addr(id$),addr(desc$))
'Int16 normalerweise -1, 0 falls ID nicht gefunden
endproc

proc ID3V2SetAddendumTitle
parameters id$,title$
id$ = AnsiToWide(id$)
title$ = AnsiToWide(title$)
return ID3V2SetAddendumTitleW(addr(id$),addr(title$))
'Int16 normalerweise -1, 0 falls ID nicht gefunden
endproc

Proc ID3V2SetImageSubFrame
parameters array#,size&,id$,desc$,type%
id$ = AnsiToWide(id$)
desc$ = AnsiToWide(desc$)
Return ID3V2SetImageSubFrameW(array#,size&,addr(Id$),addr(desc$),type%)
'Int16 normalerweise -1, bei Fehler 0
endproc

Proc ID3V2AddUserURL
parameters desc$,Url$
desc$ = AnsiToWide(desc$)
url$ = AnsiToWide(url$)
return loword(ID3V2AddUserURLW(addr(desc$),addr(url$)))
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc ID3V2GetUserURLDescription
parameters index%
var tmp& = ID3V2GetUserURLDescriptionW(index%)
return WideToAnsi(tmp&)
endproc

proc ID3V2GetUserURL 'hier
parameters index%
var tmp& = ID3V2GetUserURLW(index%)
return WideToAnsi(tmp&)
endproc

proc ID3V2AddLinkedInformation
parameters id&,url$,adata$
url$ = AnsiToWide(url$)
adata$ = AnsiToWide(adata$)
return ID3V2AddLinkedInformationW(id&,addr(url$),addr(adata$))
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetLinkedInformationAdditionalData
parameters index%
var tmp& = ID3V2GetLinkedInformationAdditionalDataW(index%)
return WideToAnsi(tmp&)
endproc

proc ID3V2GetLinkedInformationFrameIdentifier
parameters index%
return ID3V2GetLinkedInformationFrameIdentifierW(index%)
'Int32 Wert oder -1 falls Fehler
endproc

proc ID3V2GetLinkedInformationURL
parameters index%
var tmp& = ID3V2GetLinkedInformationURLW(index%)
return WideToAnsi(tmp&)
endproc

proc ID3V2GetLinkedInformationFrames 'Anzahl
return ID3V2GetFrameCountW(&ID3F_LINK)
endproc

proc ID3V2AddMusicCdIdentifier
parameters Array#,size&
Return ID3V2AddMusicCdIdentifierW(array#,size&)
'nt16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetMusicCdIdentifierData
parameters array#,size&
return ID3V2GetMusicCdIdentifierDataW(array#,size&)
'Int32 Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

proc ID3V2AddMpegLocationLookupTable
parameters array#,size&,frames&,bytes&,millisec&,bytesd%,millisecd%
return ID3V2AddMpegLocationLookupTableW(array#,size&,frames&,bytes&,millisec&,bytesd%,millisecd%)
'Int16-1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetMpegLocationLookupTableBytesDeviation
return ID3V2GetMpegLocationLookupTableBytesDeviationW()
'Int32 Wert oder -1 falls Fehler
endproc

proc ID3V2GetMpegLocationLookupTableBytes
return ID3V2GetMpegLocationLookupTableBytesW()
'Int32 Wert oder -1 falls Fehler
endproc

proc ID3V2GetMpegLocationLookupTableData
parameters array#,size&
return ID3V2GetMpegLocationLookupTableDataW(array#,size&)
'Int32 Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

Proc ID3V2GetMpegLocationLookupTableFrames
return ID3V2GetMpegLocationLookupTableFramesW()
'Int32 Wert oder -1 falls Fehler
endproc

proc ID3V2GetMpegLocationLookupTableMillisecondsDeviation
return ID3V2GetMpegLocationLookupTableMillisecondsDeviationW()
'Int32 Wert oder -1 falls Fehler
endproc

proc ID3V2GetMpegLocationLookupTableMilliseconds
return ID3V2GetMpegLocationLookupTableMillisecondsW()
'Int32 Wert oder -1 falls Fehler
endproc

Proc ID3V2AddOwnership
parameters price$,vdate$,seller$
price$ = AnsiToWide(price$)
vdate$ = AnsiToWide(vdate$)
seller$ = AnsiToWide(seller$)
return ID3V2AddOwnershipW(addr(price$),addr(vdate$),addr(seller$))
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetOwnershipDate
var tmp& = ID3V2GetOwnershipDateW()
return WideToAnsi(tmp&)
endproc

proc ID3V2GetOwnershipPrice
var tmp& = ID3V2GetOwnershipPriceW()

return WideToAnsi(tmp&)
endproc

proc ID3V2GetOwnershipSeller
var tmp& = ID3V2GetOwnershipSellerW()
return WideToAnsi(tmp&)
endproc

proc ID3V2AddPopularimeter 'wenn email = "", wird gelöscht, 1-255, abspielconter
parameters email$,rating%,counter&
email$ = ansiToWide(email$)
Return ID3V2AddPopularimeterW(addr(email$),rating%,counter&)
'nt16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc ID3V2GetPopularimeterCounter
parameters index%
return ID3V2GetPopularimeterCounterW(index%)
'Int32 counter or -1 falls Eintrag nicht existiert
endproc

Proc ID3V2GetPopularimeterEmail
Parameters index%
var tmp& = ID3V2GetPopularimeterEmailW(Index%)
Return WideToAnsi(tmp&)
endproc

Proc ID3V2GetPopularimeterRating
Parameters index%
Return ID3V2GetPopularimeterRatingW(Index%)
'Int16 Bewertung von 1 (schlecht) bis 255 (beste) oder 0 falls Eintrag nicht existiert
endproc

Proc ID3V2AddPositionSynchronisation
Parameters stamp%,pos&  '1=mpeg frames als Einheit 2=millisekunden als Einheit
return ID3V2AddPositionSynchronisationW(stamp%,pos&)
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc ID3V2GetPositionSynchronisationTimestampFormat
return ID3V2GetPositionSynchronisationTimestampFormatW()
'Int16 Wert oder -1 falls Fehler
endproc

Proc ID3V2GetPositionSynchronisationValue
return ID3V2GetPositionSynchronisationValueW()
'Int32 Wert oder -1 falls Fehler
endproc

Proc ID3V2AddPrivateFrame
Parameters array#,size&,url$
url$ = AnsiToWide(url$)
return ID3V2AddPrivateFrameW(array#,size&,addr(url$))
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetPrivateFrameData
parameters array#,size&,index%
return ID3V2GetPrivateFrameDataW(array#,size&,index%)
'Int32 Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

Proc ID3V2GetPrivateFrameURL
parameters index%
var tmp& = ID3V2GetPrivateFrameURLW(index%)
return WideToAnsi(tmp&)
endproc

proc ID3V2AddRecommendedBufferSize
parameters buffersize&,EmbeddedInfoFlag%,offset&
return ID3V2AddRecommendedBufferSizeW(buffersize&,EmbeddedInfoFlag%,offset&)
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetRecommendedBufferSizeFlag
return ID3V2GetRecommendedBufferSizeFlagW()
'Int16 0=false 1=true, bei Fehler -1
endproc

Proc ID3V2GetRecommendedBufferSizeOffset
return ID3V2GetRecommendedBufferSizeOffsetW()
'Int32 Wert Buffersize oder -1 falls Fehler
endproc

proc ID3V2GetRecommendedBufferSizeValue
return ID3V2GetRecommendedBufferSizeValueW()
'Int32 Grösse des Puffers oder -1 bei Fehler
endproc

Proc ID3V2GetRelativeVolumeAdjustmentData
parameters array#,size&,index%
return ID3V2GetRelativeVolumeAdjustmentDataW(array#,size&,index%)
'Int32 Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

Proc ID3V2AddRelativeVolumeAdjustmentData 'old name of ID3V2GetRelativeVolumeAdjustmentData (the function reads the data)
parameters array#,size&,index%
return ID3V2GetRelativeVolumeAdjustmentDataW(array#,size&,index%)
'Int32 Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

proc ID3V2AddRelativeVolumeAdjustment
parameters array#,size&,ident$
ident$ = AnsiToWide(ident$)
Return ID3V2AddRelativeVolumeAdjustmentW(array#,size&,addr(ident$))
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetRelativeVolumeAdjustmentIdentifier
parameters index%
var tmp& = ID3V2GetRelativeVolumeAdjustmentIdentifierW(index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2AddReverb
parameters reverbLeft%,reverbRight%,bouncesLeft%,bouncesRight%,feedbackLeftToLeft%,feedbackLeftToRight%,feedbackRightToRight%,feedbackRightToLeft%,premixLeftToRight%,premixRightToLeft%
return ID3V2AddReverbW(reverbLeft%,reverbRight%,bouncesLeft%,bouncesRight%,feedbackLeftToLeft%,feedbackLeftToRight%,feedbackRightToRight%,feedbackRightToLeft%,premixLeftToRight%,premixRightToLeft%)
'nt16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetReverbBouncesLeft
return ID3V2GetReverbBouncesLeftW()
'Int16 reverb bounces links oder -1 bei Fehler
endproc

proc ID3V2GetReverbBouncesRight
return ID3V2GetReverbBouncesRightW()
'Int16 reverb bounces rechts oder -1 bei Fehler
endproc

proc ID3V2GetReverbFeedbackLeftToLeft
return ID3V2GetReverbFeedbackLeftToLeftW()
'Int16 reverb feedback links zu links oder -1 bei Fehler
endproc

proc ID3V2GetReverbFeedbackLeftToRight
return ID3V2GetReverbFeedbackLeftToRightW()
'Int16 reverb feedback links zu rechts oder -1 bei Fehler
endproc

proc ID3V2GetReverbFeedbackRightToLeft
return ID3V2GetReverbFeedbackRightToLeftW()
'Int16 reverb feedback rechts zu links oder -1 bei Fehler
endproc

proc ID3V2GetReverbFeedbackRightToRight
return ID3V2GetReverbFeedbackRightToRightW()
'Int16 reverb feedback rechts zu rechts oder -1 bei Fehler
endproc

proc ID3V2GetReverbLeft
return ID3V2GetReverbLeftW()
'Int16 reverb links in ms oder -1 bei Fehler
endproc

proc ID3V2GetReverbPremixLeftToRight
return ID3V2GetReverbPremixLeftToRightW()
'Int16 premix links zu rechts oder -1 bei Fehler
endproc

proc ID3V2GetReverbPremixRightToLeft
return ID3V2GetReverbPremixRightToLeftW()
'Int16 premix rechts zu links oder -1 bei Fehler
endproc

proc ID3V2GetReverbRight
return ID3V2GetReverbRightW()
'Int16 reverb rechts in ms oder -1 bei Fehler
endproc

proc ID3V2AddSeekOffset
parameters offset&
return ID3V2AddSeekOffsetW(offset&)
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetSeekOffset
return ID3V2GetSeekOffsetW()
'Int32 Wert oder -1 bei Fehler
endproc

proc ID3V2AddSignatureFrame
parameters array#,size&,symb%
return ID3V2AddSignatureFrameW(array#,size&,symb%)
'nt16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetSignatureFrameData
parameters array#,size&,index%
return ID3V2GetSignatureFrameDataW(array#,size&,index%)
'Int32 Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

proc ID3V2GetSignatureFrameGroupSymbol
parameters index%
return ID3V2GetSignatureFrameGroupSymbolW(index%)
'Int16 Wert oder -1 bei Fehler
endproc

proc ID3V2AddSyncLyric
parameters lng$,desc$,text$,type%,tstf%
lng$ = AnsiToWide(lng$)
desc$ = AnsiToWide(desc$)
text$ = AnsiToWide(text$)
return ID3V2AddSyncLyricW(addr(lng$),addr(desc$),addr(text$),type%,tstf%)
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetSyncLyricContentType
parameters index%
var tmp& = ID3V2GetSyncLyricContentTypeW(index%)
return SyncLyricContentType(tmp&)
endproc

proc ID3V2GetSyncLyricDescription
parameters index%
var tmp& = ID3V2GetSyncLyricDescriptionW(index%)
return WideToAnsi(tmp&)
endproc

proc ID3V2GetSyncLyricLanguage
parameters index%
var tmp& = ID3V2GetSyncLyricLanguageW(index%)
return WideToAnsi(tmp&)
endproc

proc ID3V2GetSyncLyricTimeFormat
parameters index%
return ID3V2GetSyncLyricTimeFormatW(index%)
'Int16, -1 = ungültiger Eintrag, 1 = Zeitangaben in mpeg frames, 2 = Zeitangaben in millisekunden
endproc

proc ID3V2GetSyncLyric
parameters index%
var tmp& = ID3V2GetSyncLyricW(index%)
return WideToAnsi(tmp&)
endproc

proc ID3V2AddSynchronizedTempo
parameters array#,size&,format%
return ID3V2AddSynchronizedTempoW(array#,size&,format%)
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetSynchronizedTempoData
parameters array#,size&
return ID3V2GetSynchronizedTempoDataW(array#,size&)
'Int32 Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

proc ID3V2GetSynchronizedTempoFormat
return ID3V2GetSynchronizedTempoFormatW()
'Int16 1=in frames 2=in Millisekunden -1=Fehler
endproc

proc ID3V2AddUserText
parameters desc$,text$
desc$ = AnsiToWide(desc$)
text$ = AnsiToWide(text$)
return loword(ID3V2AddUserTextW(addr(desc$),addr(text$)))
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetUserTextDescription
parameters index%
var tmp& = ID3V2GetUserTextDescriptionW(index%)
return WideToAnsi(tmp&)
endproc

proc ID3V2GetUserText
parameters index%
var tmp& = ID3V2GetUserTextW(index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2AddUniqueFileIdentifier
parameters array#,size&,owner$
owner$ = AnsiToWide(owner$)
return ID3V2AddUniqueFileIdentifierW(array#,size&,addr(owner$))
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc ID3V2GetUniqueFileIdentifierData
parameters array#,size&,index%
return ID3V2GetUniqueFileIdentifierDataW(array#,size&,index%)
'Int32 Grösse des tatsächlichen Arrays in Bytes oder -1 bei Fehler
endproc

proc ID3V2GetUniqueFileIdentifierOwner
parameters index%
var tmp& = ID3V2GetUniqueFileIdentifierOwnerW(index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2AddUserFrame
parameters lng$,text$
lng$ = AnsiToWide(lng$)
text$ = AnsiToWide(text$)
return loword(ID3V2AddUserFrameW(addr(lng$),addr(text$)))
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

proc ID3V2GetUserFrameLanguage
parameters index%
var tmp& = ID3V2GetUserFrameLanguageW(index%)
return WideToAnsi(tmp&)
endproc

proc ID3V2GetUserFrame
parameters index%
var tmp& =  ID3V2GetUserFrameW(index%)
return WideToAnsi(tmp&)
endproc

Proc ID3V2GetArtistUrl
return ID3V2GetURLFrame(&ID3F_WOAR)
endproc

Proc ID3V2GetAudiofileURL
return ID3V2GetURLFrame(&ID3F_WOAF)

endproc

Proc ID3V2GetAudiosourceURL
Return ID3V2GetURLFrame(&ID3F_WOAS)
endproc

Proc ID3V2GetCommercialURL
return ID3V2GetURLFrame(&ID3F_WCOM)

endproc

Proc ID3V2GetCopyrightURL
return ID3V2GetURLFrame(&ID3F_WCOP)
endproc

Proc ID3V2GetRadiostationsurl
return ID3V2GetURLFrame(&ID3F_WORS)
endproc

Proc ID3V2GetBeatsPerMinute
Return ID3V2GetTextFrame(&ID3F_TBPM)
endproc

 Proc ID3V2GetCategory
Return ID3V2GetTextFrame(&ID3F_TIT1)
endproc

Proc ID3V2GetComposer
Return ID3V2GetTextFrame(&ID3F_TCOM)
endproc

Proc ID3V2GetConductor
Return ID3V2GetTextFrame(&ID3F_TPE3)
endproc

Proc ID3V2GetCopyright
Return ID3V2GetTextFrame(&ID3F_TCOP)
endproc

Proc ID3V2GetEncoder
Return ID3V2GetTextFrame(&ID3F_TENC)
endproc

Proc ID3V2GetEncodingSettings
return ID3V2GetTextFrame(&ID3F_TSSE)
endproc

Proc ID3V2GetOwnership
Return ID3V2GetTextFrame(&ID3F_OWNE)
endproc

Proc ID3V2GetFileType
return ID3V2GetTextFrame(&ID3F_TFLT)
endproc

Proc ID3V2GetISRC
return ID3V2GetTextFrame(&ID3F_TSRC)
endproc

Proc ID3V2GetInetRadioName
return ID3V2GetTextFrame(&ID3F_TRSN)
endproc

Proc ID3V2GetLyric
Parameters index%
var tmp& = ID3V2GetLyricW(Index%)
Return WideToAnsi(tmp&)
endproc

Proc ID3V2GetLyricDescription
Parameters index%
var tmp& = ID3V2GetLyricDescriptionW(Index%)
Return WideToAnsi(tmp&)
endproc

Proc ID3V2GetLyricLanguage
Parameters index%
var tmp& = ID3V2GetLyricLanguageW(Index%)
Return WideToAnsi(tmp&)
endproc

Proc ID3V2GetFileOwner
Return ID3V2GetTextFrame(&ID3F_TOWN)
endproc

Proc ID3V2GetInetRadioOwner
return ID3V2GetTextFrame(&ID3F_TRSO)
endproc

Proc ID3V2GetInitialKey
return ID3V2GetTextFrame(&ID3F_TKEY)
endproc

Proc ID3V2GetInterpreted
return ID3V2GetTextFrame(&ID3F_TPE4)
endproc

Proc ID3V2GetLanguage
return ID3V2GetTextFrame(&ID3F_TLAN)
endproc

Proc ID3V2GetLength
return ID3V2GetTextFrame(&ID3F_TLEN)
endproc

Proc ID3V2GetMediaType
return ID3V2GetTextFrame(&ID3F_TMED)
endproc

Proc ID3V2GetOrchestra
return ID3V2GetTextFrame(&ID3F_TPE2)
endproc

Proc ID3V2GetOriginalAlbum
return ID3V2GetTextFrame(&ID3F_TOAL)
endproc

Proc ID3V2GetOriginalArtist
return ID3V2GetTextFrame(&ID3F_TOPE)
endproc

Proc ID3V2GetOriginalFilename
return ID3V2GetTextFrame(&ID3F_TOFN)
endproc

Proc ID3V2GetOriginalReleaseYear
return ID3V2GetTextFrame(&ID3F_TORY)
endproc

Proc ID3V2GetOriginalTextwriter
return ID3V2GetTextFrame(&ID3F_TOLY)
endproc

Proc ID3V2GetPartofSet
return ID3V2GetTextFrame(&ID3F_TPOS)
endproc

Proc ID3V2GetPaymentURL
return ID3V2GetURLFrame(&ID3F_WPAY)
endproc

Proc ID3V2GetPlayCounter
Return ID3V2GetPlayCounterW()
endproc

Proc ID3V2GetPlaylistDelay
return ID3V2GetTextFrame(&ID3F_TDLY)
endproc

Proc ID3V2GetPublisher
return ID3V2GetTextFrame(&ID3F_TPUB)
endproc

Proc ID3V2GetPublisherURL
return ID3V2GetURLFrame(&ID3F_WPUB)
endproc

Proc ID3V2GetRadiostationURL
return ID3V2GetURLFrame(&ID3F_WORS)
endproc

Proc ID3V2GetRecordingDate
return ID3V2GetTextFrame(&ID3F_TRDA)
endproc

Proc ID3V2GetRecordingTime
return ID3V2GetTextFrame(&ID3F_TDRC)
endproc

Proc ID3V2GetSubtitle
return ID3V2GetTextFrame(&ID3F_TIT3)
endproc

Proc ID3V2GetTextWriter
return ID3V2GetTextFrame(&ID3F_TEXT)
endproc

Proc ID3V2GetTime
return ID3V2GetTextFrame(&ID3F_TIME)
endproc

Proc ID3V2SetAlbum
Parameters wert$
ID3V2SetTextFrame(&ID3F_TALB,wert$)
endproc

Proc ID3V2SetArtist
Parameters wert$
ID3V2SetTextFrame(&ID3F_TPE1,wert$)
endproc

Proc ID3V2SetArtistURL
Parameters wert$
ID3V2SetURLFrame(&ID3F_WOAR,wert$)
endproc

Proc ID3V2SetAudiofileURL
Parameters wert$
ID3V2SetURLFrame(&ID3F_WOAF,wert$)
endproc

Proc ID3V2SetAudiosourceURL
Parameters wert$
ID3V2SetURLFrame(&ID3F_WOAS,wert$)
endproc

Proc ID3V2SetBeatsPerMinute
Parameters wert$
ID3V2SetTextFrame(&ID3F_TBPM,wert$)
endproc

Proc ID3V2SetCategory
Parameters wert$
ID3V2SetTextFrame(&ID3F_TIT1,wert$)
endproc

Proc ID3V2SetCommercialURL
Parameters wert$
ID3V2SetURLFrame(&ID3F_WCOM,wert$)
endproc

Proc ID3V2SetComposer
Parameters wert$
ID3V2SetTextFrame(&ID3F_TCOM,wert$)
endproc

Proc ID3V2SetConductor
Parameters wert$
ID3V2SetTextFrame(&ID3F_TPE3,wert$)
endproc

Proc ID3V2SetCopyright
Parameters wert$
ID3V2SetTextFrame(&ID3F_TCOP,wert$)
endproc

Proc ID3V2SetCopyrightURL
Parameters wert$
ID3V2SetURLFrame(&ID3F_WCOP,wert$)
endproc

Proc ID3V2SetDate
Parameters wert$
ID3V2SetTextFrame(&ID3F_TDAT,wert$)
endproc

Proc ID3V2SetEncoder
Parameters wert$
ID3V2SetTextFrame(&ID3F_TENC,wert$)
endproc

Proc ID3V2SetEncodingSettings
Parameters wert$
ID3V2SetTextFrame(&ID3F_TSSE,wert$)
endproc

Proc ID3V2SetFileOwner
Parameters wert$
ID3V2SetTextFrame(&ID3F_TOWN,wert$)
endproc

Proc ID3V2SetFileType
Parameters wert$
ID3V2SetTextFrame(&ID3F_TFLT,wert$)
endproc

Proc ID3V2SetGenre
Parameters wert$
wert$ = AnsiToWide(wert$)
ID3V2SetGenreW(addr(wert$))
endproc

Proc ID3V2SetISRC
Parameters wert$
ID3V2SetTextFrame(&ID3F_TSRC,wert$)
endproc

Proc ID3V2SetInetRadioName
Parameters wert$
ID3V2SetTextFrame(&ID3F_TRSN,wert$)
endproc

Proc ID3V2SetInetRadioOwner
Parameters wert$
ID3V2SetTextFrame(&ID3F_TRSO,wert$)
endproc

Proc ID3V2SetInitialKey
Parameters wert$
ID3V2SetTextFrame(&ID3F_TKEY,wert$)
endproc

Proc ID3V2SetInterpreted
Parameters wert$
ID3V2SetTextFrame(&ID3F_TPE4,wert$)
endproc

Proc ID3V2SetLanguage
Parameters wert$
ID3V2SetTextFrame(&ID3F_TLAN,wert$)
endproc

Proc ID3V2SetLength
Parameters wert$
ID3V2SetTextFrame(&ID3F_TLEN,wert$)
endproc

Proc ID3V2SetMediaType
Parameters wert$
ID3V2SetTextFrame(&ID3F_TMED,wert$)
endproc

Proc ID3V2SetOrchestra
Parameters wert$
ID3V2SetTextFrame(&ID3F_TPE2,wert$)
endproc

Proc ID3V2SetOriginalAlbum
Parameters wert$
ID3V2SetTextFrame(&ID3F_TOAL,wert$)
endproc

Proc ID3V2SetOriginalArtist
Parameters wert$
ID3V2SetTextFrame(&ID3F_TOPE,wert$)
endproc

Proc ID3V2SetOriginalFilename
Parameters wert$
ID3V2SetTextFrame(&ID3F_TOFN,wert$)
endproc

Proc ID3V2SetOriginalReleaseYear
Parameters wert$
ID3V2SetTextFrame(&ID3F_TORY,wert$)
endproc

Proc ID3V2SetOriginalTextwriter
Parameters wert$
ID3V2SetTextFrame(&ID3F_TOLY,wert$)
endproc

Proc ID3V2SetPartofSet
Parameters wert$
ID3V2SetTextFrame(&ID3F_TPOS,wert$)
endproc

Proc ID3V2SetPaymentURL
Parameters wert$
ID3V2SetURLFrame(&ID3F_WPAY,wert$)
endproc

Proc ID3V2SetPlayCounter
Parameters wert&
ID3V2SetPlayCounterW(wert&)
endproc

Proc ID3V2SetPlaylistDelay
Parameters wert$
ID3V2SetTextFrame(&ID3F_TDLY,wert$)
endproc

Proc ID3V2SetPublisher
Parameters wert$
ID3V2SetTextFrame(&ID3F_TPUB,wert$)
endproc

Proc ID3V2SetPublisherURL
Parameters wert$
ID3V2SetURLFrame(&ID3F_WPUB,wert$)
endproc

Proc ID3V2SetRadiostationURL
Parameters wert$
ID3V2SetURLFrame(&ID3F_WORS,wert$)
endproc

Proc ID3V2SetRecordingDate
Parameters wert$
ID3V2SetTextFrame(&ID3F_TRDA,wert$)
endproc

Proc ID3V2SetRecordingTime
Parameters wert$
ID3V2SetTextFrame(&ID3F_TDRC,wert$)
endproc

Proc ID3V2SetSubtitle
Parameters wert$
ID3V2SetTextFrame(&ID3F_TSST,wert$)
endproc

Proc ID3V2SetTextWriter
Parameters wert$
ID3V2SetTextFrame(&ID3F_TEXT,wert$)
endproc

Proc ID3V2SetTime
Parameters wert$
ID3V2SetTextFrame(&ID3F_TIME,wert$)
endproc

Proc ID3V2SetTitle
Parameters wert$
ID3V2SetTextFrame(&ID3F_TIT2,wert$)
endproc

Proc ID3V2SetTrack
Parameters wert$
ID3V2SetTextFrame(&ID3F_TRCK,wert$)
endproc

Proc ID3V2SetYear
Parameters wert$
ID3V2SetTextFrame(&ID3F_TYER,wert$)
endproc

proc ID3V2SetMood
parameters wert$
ID3V2SetTextFrame(&ID3F_TMOO,wert$)
endproc

proc ID3V2GetMood
return D3V2GetTextFrame(&ID3F_TMOO)
endproc

Proc ID3V2AddLyric
Parameters lang$,desc$,text$
lang$ = AnsiToWide(lang$)
desc$ = AnsiToWide(desc$)
text$ = AnsiToWide(text$)
return ID3V2AddLyricW(addr(lang$),addr(desc$),addr(text$))
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc ID3V2GetLyrics 'Anzahl
Return ID3V2GetFrameCount(&ID3F_USLT)
endproc

Proc ID3V2GetSyncLyrics 'Anzahl
Return ID3V2GetFrameCount(&ID3F_SYLT)
endproc

Proc ID3V2GetPopularimeters 'Anzahl
Return ID3V2GetFrameCount(&ID3F_POPM)
endproc

Proc ID3V2GetUserFrames  'Anzahl
Return ID3V2GetFrameCount(&ID3F_USER)
endproc

Proc ID3V2GetUserTexts 'Anzahl
Return ID3V2GetFrameCount(&ID3F_TXXX)
endproc

Proc ID3V2GetUserUrls 'Anzahl
Return ID3V2GetFrameCount(&ID3F_WXXX)
endproc

proc ID3V2GetRelativeVolumeAdjustmentIdentifiers 'Anzahl
return ID3V2GetFrameCount(&ID3F_RVAD)
endproc

proc ID3V2Reverbs 'Anzahl
return ID3V2GetFrameCount(&ID3F_RVRB)
endproc

proc ID3V2SignatureFrames 'Anzahl
return ID3V2GetFrameCount(&ID3F_SIGN)
endproc

proc ID3V2UniqueFileIdentifiers 'Anzahl
return ID3V2GetFrameCount(&ID3F_UFID)
endproc

proc ID3V2DeleteComment
parameters index%
return ID3V2DeleteSelectedFrame(&ID3F_COMM,index%)
'Int16 normalerweise -1, 0 falls Frame nicht gefunden
endproc

Proc ID3V2DeleteComments
ID3V2DeleteAllFrames(&ID3F_COMM)
endproc

Proc ID3V2DeleteLyrics 'Alle Löschen
ID3V2DeleteAllFrames(&ID3F_USLT)
endproc

Proc ID3V2DeleteLyric 'Einen Löschen
parameters index%
ID3V2DeleteSelectedFrames(&ID3F_USLT,index%)
endproc

proc ID3V2DeletePopularimeters
ID3V2DeleteAllFrames(&ID3F_POPM)
endproc

Proc ID3V2DeleteSyncLyrics
ID3V2DeleteAllFrames(&ID3F_SYLT)
endproc

Proc ID3V2DeleteUserFrames
ID3V2DeleteAllFrames(&ID3F_USER)
endproc

Proc ID3V2DeleteUserFrame
parameters index%
return ID3V2DeleteSelectedFrame(&ID3F_USER,index%)
'Int16 normalerweise -1, 0 falls Frame nicht gefunden
endproc

Proc ID3V2DeleteUserTexts
ID3V2DeleteAllFrames(&ID3F_TXXX)
endproc

Proc ID3V2DeleteUserText
parameters index%
return ID3V2DeleteSelectedFrame(&ID3F_TXXX,index%)
'Int16 normalerweise -1, 0 falls Frame nicht gefunden
endproc

Proc ID3V2DeleteUserUrls
ID3V2DeleteAllFrames(&ID3F_WXXX)
Endproc

Proc ID3V2DeleteUserUrl
parameters index%
return ID3V2DeleteSelectedFrame(&ID3F_WXXX,index%)
'Int16 normalerweise -1, 0 falls Frame nicht gefunden
endproc

proc ID3V2AddLyric 'was ID3V2AddLyricW, the name of the DLL function
parameters lang$,desc$,text$
lang$ = AnsiToWide(lang$)
desc$ = AnsiToWide(desc$)
text$ = AnsiToWide(text$)
return ID3V2AddLyricW(addr(lang$),addr(desc$),addr(text$))
'Int16 -1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc ID3V2RemoveTag
var tmp& = ID3V2RemoveTagW()
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler beim Tag entfernen (ID3V2RemoveTag)"
'case tmp& = 0:Messagebox("Beim Entfernen des Tags ist ein Fehler aufgetreten",error$,262144+48)
endproc

Proc ID3V2RemoveTagFromFile
parameters wert$
wert$ = AnsiToWide(wert$)
Var tmp& = ID3V2RemoveTagFromFileW(addr(wert$))
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler beim Tag entfernen (ID3V2RemoveTagFromFile)"
'case tmp& = 0:Messagebox("Beim Entfernen des Tags ist ein Fehler aufgetreten",error$,262144+48)
endproc

Proc ID3V2SaveChanges
Var tmp& = ID3V2SaveChangesW()
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler beim Tag schreiben (ID3V2SaveChanges)"
'case tmp& = 0:Messagebox("Beim Schreiben des Tags ist ein Fehler aufgetreten",error$,262144+48)
endproc

Proc ID3V2SaveChangesToFile
Parameters wert$
wert$ = AnsiToWide(wert$)
var tmp& = ID3V2SaveChangesToFileW(addr(wert$))
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler beim Tag schreiben (ID3V2SaveChangesToFile)"
'case tmp& = 0:Messagebox("Beim Schreiben des Tags ist ein Fehler aufgetreten",error$,262144+48)
endproc
'-------------------OGG-----------------------------------
Proc OGGGetAlbum
Var tmp& = OGGGetAlbumW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetArtist
var tmp& = OGGGetArtistW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetComment
Var tmp& = OGGGetCommentW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetCopyright
Var tmp& = OGGGetCopyrightW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetDate
Var tmp& = OGGGetDateW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetDescription
Var tmp& = OGGGetDescriptionW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetGenre
Var tmp& = OGGGetGenreW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetISRC
Var tmp& = OGGGetISRCW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetLicense
var tmp& = OGGGetLicenseW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetLocation
var tmp& = OGGGetLocationW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetOrganization
var tmp& = OGGGetOrganizationW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetPerformer
Var tmp& = OGGGetPerformerW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetTitle
Var tmp& = OGGGetTitleW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetTrack
Var tmp& = OGGGetTrackW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetUserItem
Parameters wert$
wert$ = AnsiToWide(wert$)
Var tmp& = OGGGetUserItemW(addr(wert$))
Return WideToAnsi(tmp&)
endproc

Proc OGGGetVendor
Var tmp& = OGGGetVendorW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetVersion
Var tmp& = OGGGetVersionW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetContact
Var tmp& = OGGGetContactW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetItemKeys
Var tmp& = OGGGetItemKeysW()
Return WideToAnsi(tmp&)
endproc

Proc OGGGetBitRateNominal
Return OGGGetBitRateNominalW()
endproc

Proc OGGRemoveTag
Var tmp& = OGGRemoveTagW()
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Beim Entfernen des Tags ist ein Fehler aufgetreten",error$,262144+48)
endproc

Proc OGGRemoveTagFromFile
Parameters wert$
wert$ = AnsiToWide(wert$)
var tmp& = OGGRemoveTagFromFileW(addr(wert$))
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Beim Entfernen des Tags ist ein Fehler aufgetreten",error$,262144+48)
endproc

Proc OGGSaveChanges
Var tmp& = OGGSaveChangesW()
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Beim Schreiben ist ein Fehler aufgetreten",error$,262144+48)
endproc

Proc OGGSaveChangesToFile
Parameters wert$
wert$ = AnsiToWide(wert$)
var tmp& = OGGSaveChangesToFileW(addr(wert$))
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Beim Schreiben ist ein Fehler aufgetreten",error$,262144+48)
endproc

Proc OGGSetAlbum
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetAlbumW(addr(wert$))
endproc

Proc OGGSetArtist
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetArtistW(addr(wert$))
endproc

Proc OGGSetComment
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetCommentW(addr(wert$))
Endproc

Proc OGGSetCopyright
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetCopyrightW(addr(wert$))
endproc

Proc OGGSetDate
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetDateW(addr(wert$))
endproc

Proc OGGSetDescription
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetDescriptionW(addr(wert$))
endproc

Proc OGGSetGenre
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetGenreW(addr(wert$))
endproc

Proc OGGSetISRC
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetISRCW(addr(wert$))
endproc

Proc OGGSetLicense
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetLicenseW(addr(wert$))
endproc

Proc OGGSetLocation
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetLocationW(addr(wert$))
endproc

Proc OGGSetOrganization
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetOrganizationW(addr(wert$))
endproc

Proc OGGSetPerformer
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetPerformerW(addr(wert$))
endproc

Proc OGGSetTitle
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetTitleW(addr(wert$))
endproc

Proc OGGSetTrack
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetTrackW(addr(wert$))
Endproc

Proc OGGSetUserItem
Parameters key$,wert$
key$ = AnsiToWide(key$)
wert$ = AnsiToWide(wert$)
OGGSetUserItemW(addr(key$),addr(wert$))
endproc

Proc OGGSetVendor
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetVendorW(addr(wert$))
endproc

Proc OGGSetVersion
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetVersionW(addr(wert$))
endproc

Proc OGGSetContact
Parameters wert$
wert$ = AnsiToWide(wert$)
OGGSetContactW(addr(wert$))
endproc

'------------WMA-------------------
Proc WMASetUserItem
Parameters key$,wert$
key$ = AnsiToWide(key$)
wert$ = AnsiToWide(wert$)
WMASetUserItemW(addr(key$),addr(wert$))
endproc

Proc WMAGetUserItem
Parameters key$
key$ = AnsiToWide(key$)
Var tmp& = WMAGetUserItemW(addr(key$))
Return WideToAnsi(tmp&)
endproc

proc WMAIsVBR 'variable Bitrate
var tmp% = WMAIsVBRW()
if Tmp% = -1
return "Ja"
else
return "Nein"
endif
endproc

Proc WMAGetAlbum
Return WMAGetUserItem("WM/ALBUM")
endproc

Proc WMAGetAlbumTitle 'ist alt Album
Return WMAGetUserItem("WM/ALBUMTITLE")
endproc

Proc WMAGetAlbumArtist
Return WMAGetUserItem("WM/ALBUMARTIST")
endproc

Proc WMAGetArtist
return WMAGetUserItem("WM/ARTIST")
endproc

Proc WMAGetAuthor 'ist alt Artist
return WMAGetUserItem("WM/AUTHOR")
endproc

Proc WMAGetAuthorURL
return WMAGetUserItem("WM/AUTHORURL")
endproc

Proc WMAGetBeatsPerMinute
return WMAGetUserItem("WM/BEATSPERMINUTE")
Endproc

Proc WMAGetCategory
return WMAGetUserItem("WM/CATEGORY")
endproc

Proc WMAGetComment
Return WMAGetUserItem("WM/COMMENT")
endproc

Proc WMAGetDescription 'ist alt comment
Return WMAGetUserItem("WM/Description")
endproc

Proc WMAGetComposer
return WMAGetUserItem("WM/COMPOSER")
endproc

Proc WMAGetconductor
Return WMAGetUserItem("WM/CONDUCTOR")
Endproc

Proc WMAGetContentGroupDescription
Return WMAGetUserItem("WM/CONTENTGROUPDESCRIPTION")
endproc

Proc WMAGetCopyright
return WMAGetUserItem("WM/COPYRIGHT")
endproc

Proc WMAGetProviderCopyright 'ist alt copyright
return WMAGetUserItem("WM/PROVIDERCOPYRIGHT")
endproc


Proc WMAGetDistributor
return WMAGetUserItem("WM/DISTRIBUTOR")
endproc

Proc WMAGetContentDistributor 'ist alt distributer
return WMAGetUserItem("WM/CONTENTDISTRIBUTOR")
endproc

Proc WMAGetEncodedBy
return WMAGetUserItem("WM/ENCODEDBY")
endproc

Proc WMAGetEncodingSettings
return WMAGetUserItem("WM/ENCODINGSETTINGS")
endproc

Proc WMAGetEncodingTime
return WMAGetUserItem("WM/ENCODINGTIME")
endproc

Proc WMAGetGenre
Return WMAGetUserItem("WM/GENRE")
endproc

Proc WMAGetISRC
return WMAGetUserItem("WM/ISRC")
endproc

Proc WMAGetInitialKey
return WMAGetUserItem("WM/INITIALKEY")
endproc

Proc WMAGetLanguage
return WMAGetUserItem("WM/LANGUAGE")
endproc

proc WMAGetLyrics
return WMAGetUserItem("WM/LYRICS")
endproc

Proc WMAGetMood
return WMAGetUserItem("WM/MOOD")
endproc

Proc WMAGetParentalRating
return WMAGetUserItem("WM/PARENTALRATING")
endproc

Proc WMAGetPartOfSet
return WMAGetUserItem("WM/PARTOFSET")
endproc

Proc WMAGetPeriod
return WMAGetUserItem("WM/PERIOD")
endproc

Proc WMAGetProducer
return WMAGetUserItem("WM/PRODUCER")
endproc

Proc WMAGetPromotionURL
return WMAGetUserItem("WM/PROMOTIONURL")
endproc

Proc WMAGetProvider
return WMAGetUserItem("WM/PROVIDER")
endproc

Proc WMAGetPublisher
return WMAGetUserItem("WM/PUBLISHER")
endproc

Proc WMAGetRating
return WMAGetUserItem("WM/RATING")
endproc

Proc WMAGetProviderRating 'ist alt rating
return WMAGetUserItem("WM/PROVIDERRATING")
endproc

Proc WMAGetTitle
return WMAGetUserItem("WM/TITLE")
endproc

Proc WMAGetToolName
return WMAGetUserItem("WM/TOOLNAME")
endproc

Proc WMAGetToolVersion
return WMAGetUserItem("WM/TOOLVERSION")
endproc

Proc WMAGetTrackNumber
return WMAGetUserItem("WM/TRACKNUMBER")
endproc

Proc WMAGetTrack
return WMAGetUserItem("WM/TRACK")
endproc

Proc WMAGetUserWebURL
return WMAGetUserItem("WM/USERWEBURL")
endproc

Proc WMAGetWriter
return WMAGetUserItem("WM/WRITER")
endproc

Proc WMAGetYear
return WMAGetUserItem("WM/YEAR")
endproc
'get methoden fertig
Proc WMASetAlbum
Parameters wert$
WMASetUserItem("WM/ALBUM",wert$)
endproc

Proc WMASetAlbumTitle
Parameters wert$
WMASetUserItem("WM/ALBUMTITLE",wert$)
endproc

Proc WMASetAlbumArtist
Parameters wert$
WMASetUserItem("WM/ALBUMARTIST",wert$)
endproc

Proc WMASetArtist
Parameters wert$
WMASetUserItem("WM/ARTIST",wert$)
endproc

Proc WMASetAuthor 'ist alt Artist
Parameters wert$
WMASetUserItem("WM/AUTHOR",wert$)
endproc

Proc WMASetAuthorURL
Parameters wert$
WMASetUserItem("WM/AUTHORURL",wert$)
endproc

Proc WMASetBeatsPerMinute
Parameters wert$
WMASetUserItem("WM/BEATSPERMINUTE",wert$)
endproc

Proc WMASetCategory
Parameters wert$
WMASetUserItem("WM/CATEGORY",wert$)
endproc

Proc WMASetComment
Parameters wert$
WMASetUserItem("WM/COMMENT",wert$)
endproc

Proc WMASetDescription ' ist alt Comment
Parameters wert$
WMASetUserItem("WM/Description",wert$)
endproc

Proc WMASetComposer
Parameters wert$
WMASetUserItem("WM/COMPOSER",wert$)
endproc

Proc WMASetConductor
Parameters wert$
WMASetUserItem("WM/CONDUCTOR",wert$)
endproc

Proc WMASetContentGroupDescription
Parameters wert$
WMASetUserItem("WM/CONTENTGROUPDESCRIPTION",wert$)
endproc

Proc WMASetCopyright
Parameters wert$
WMASetUserItem("WM/COPYRIGHT",wert$)
endproc

Proc WMASetProviderCopyright 'ist alt Copyrigt
Parameters wert$
WMASetUserItem("WM/PROVIDERCOPYRIGHT",wert$)
endproc

Proc WMASetDistributor
Parameters wert$
WMASetUserItem("WM/DISTRIBUTER",wert$)
endproc

Proc WMASetContentDistributor ' ist alt Distributer
Parameters wert$
WMASetUserItem("WM/CONTENTDISTRIBUTER",wert$)
endproc

Proc WMASetEncodedBy
Parameters wert$
WMASetUserItem("WM/ENCODEDBY",wert$)
endproc

Proc WMASetEncodingSettings
Parameters wert$
WMASetUserItem("WM/ENCODINGSETTINGS",wert$)
endproc

Proc WMASetEncodingTime
Parameters wert$
WMASetUserItem("WM/ENCODINGTIME",wert$)
endproc

Proc WMASetGenre
Parameters wert$
WMASetUserItem("WM/GENRE",wert$)
endproc

Proc WMASetISRC
Parameters wert$
WMASetUserItem("WM/ISRC",wert$)
endproc

Proc WMASetInitialKey
Parameters wert$
WMASetUserItem("WM/INITIALKEY",wert$)
endproc

Proc WMASetLanguage
Parameters wert$
WMASetUserItem("WM/LANGUAGE",wert$)
endproc

Proc WMASetLyrics
Parameters wert$
WMASetUserItem("WM/LYRICS",wert$)
endproc

Proc WMASetMood
Parameters wert$
WMASetUserItem("WM/MOOD",wert$)
endproc

Proc WMASetParentalRating
Parameters wert$
WMASetUserItem("WM/PARENTALRATING",wert$)
endproc

Proc WMASetPartOfSet
Parameters wert$
WMASetUserItem("WM/PARTOFSET",wert$)
endproc

Proc WMASetPeriod
Parameters wert$
WMASetUserItem("WM/PERIOD",wert$)
endproc

Proc WMASetProducer
Parameters wert$
WMASetUserItem("WM/PRODUCER",wert$)
endproc

Proc WMASetPromotionURL
Parameters wert$
WMASetUserItem("WM/PROMOTIONURL",wert$)
endproc

Proc WMASetProvider
Parameters wert$
WMASetUserItem("WM/PROVIDER",wert$)
endproc

Proc WMASetPublisher
Parameters wert$
WMASetUserItem("WM/PUBLISHER",wert$)
endproc

Proc WMASetRating
Parameters wert$
WMASetUserItem("WM/RATING",wert$)
endproc

Proc WMASetProviderRating ' ist alt Rating
Parameters wert$
WMASetUserItem("WM/PROVIDERRATING",wert$)
endproc

Proc WMASetTitle
Parameters wert$
WMASetUserItem("WM/TITLE",wert$)
endproc

Proc WMASetToolName
Parameters wert$
WMASetUserItem("WM/TOOLNAME",wert$)
endproc

Proc WMASetToolVersion
Parameters wert$
WMASetUserItem("WM/TOOLVERSION",wert$)
endproc

Proc WMASetTrackNumber
Parameters wert$
WMASetUserItem("WM/TRACKNUMBER",wert$)
endproc

Proc WMASetTrack
Parameters wert$
WMASetUserItem("WM/TRACK",wert$)
endproc

Proc WMASetUserWebURL
Parameters wert$
WMASetUserItem("WM/USERWEBURL",wert$)
endproc

Proc WMASetWriter
Parameters wert$
WMASetUserItem("WM/WRITER",wert$)
endproc

Proc WMASetYear
Parameters wert$
WMASetUserItem("WM/YEAR",wert$)
endproc

 Proc WMAGetPictureCount
 Return WMAGetPictureCountW()
 endproc

 proc WMAGetPictureSize
 parameters index&
 return WMAGetPictureSizeW(index&)
 endproc

Proc WMADeletePicture 'Löscht Bild mit Index aus Frame
Parameters index%
var tmp& = WMADeletePictureW(index%)
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Löschen des Bildes misslungen.",error$, 262144 + 64)
endproc

Proc WMADeletePictures 'Löscht alle Bilder
WMADeletePicturesW()
endproc

proc WMAGetPictureArray ' Bild aus Frame in Speicher, Speicherbereich muss außerhalb gedimt und freigegeben werden
parameters mem#,size&,index%   'bereich, bildindex
Return WMAGetPictureArrayW(mem#,size&,index%) 'Returnwert Größe des beschriebenen Bereiches
endproc

Proc WMAAddPictureArray 'Bild aus Speicher in Frame. Bild muss außerhalb in Speicher gesetzt werden
parameters mem#,size&,com$,type&,index%  'bereich# mit bilddaten, bildindex
com$ = AnsiToWide(com$)
var tmp% = WMAAddPictureArrayW(mem#,size&,addr(com$),type&,index%)
dispose mem#
return tmp%
endproc

Proc WMAGetPictureFile 'speichert Bild in Datei
Parameters file$,index%
file$ = AnsiToWide(file$)
var tmp% = loword(WMAGetPictureFileW(addr(file$),index%))
var error$ = AUDIOGetLastErrorText()
case Error$ = "":error$ = "Fehler beim Speichern"
'case tmp% = 0:Messagebox(error$,"Bild konnte nicht gespeichert werden.", 262144 + 64)
return tmp%
endproc

Proc WMAAddPictureFile 'externes Bild in Frame
Parameters file$,comment$,type&,index% 'Type& siehe oben pictype
file$ = AnsiToWide(file$)
comment$ = AnsiToWide(comment$)
var tmp% = Loword(WMAAddPictureFileW(addr(file$),addr(comment$),type&,index%))
var error$ = AUDIOGetLastErrorText()
'case tmp% = 0:Messagebox(error$,"Bild konnte nicht eingefügt werden.", 262144 + 64)
return tmp%
endproc

Proc WMAGetPictureType
Parameters index%
return WMAGetPictureTypeW(index%)
endproc

Proc WMAGetPictureDescription
Parameters index%
var tmp& = WMAGetPictureDescriptionW(index%)
return WideToAnsi(tmp&)
endproc

Proc WMAGetPictureMime
Parameters index%
var tmp& = WMAGetPictureMimeW(index%)
Return widetoAnsi(tmp&)
endproc

Proc WMAGetItemKeys
var tmp& = WMAGetItemKeysW()
return WideToAnsi(tmp&)
endproc

Proc WMASaveChanges
var tmp& = WMASaveChangesW()
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox(error$,"Der Tag konnte nicht gespeichert werden.", 262144 + 64)
endproc

Proc WMASaveChangesToFile
Parameters file$
file$ = AnsiToWide(file$)
var tmp& = WMASaveChangesToFileW(addr(file$))
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox(error$,"Der Tag konnte nicht gespeichert werden.", 262144 + 64)
endproc

'--------------MPEG----------------------
Proc MPEGGetAnalyzeAlFrames
Return GetConfigValueW(&CF_MPEGEXACTREAD)
endproc

Proc MPEGSetAnalyzeAllFrames '1 = an, 0 = aus
parameters flag&
SetConfigValueW(&CF_MPEGEXACTREAD,flag&)
endproc

 Proc MPEGModifyCopyrightBit
Parameters file$,flag%
file$ = AnsiToWide(file$)
Var tmp& = MPEGSetCopyrightedW(addr(file$),flag%)
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Der Eintrag konnte nicht geändert werden",error$,262144+48)
endproc

Proc MPEGModifyOriginalBit
Parameters file$,flag%
file$ = AnsiToWide(file$)
Var tmp& = MPEGSetOriginalW(addr(file$),flag%)
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Der Eintrag konnte nicht geändert werden",error$,262144+48)
endproc

Proc MPEGModifyPrivateBit
Parameters file$,flag%
file$ = AnsiToWide(file$)
Var tmp& = MPEGSetPrivateW(addr(file$),flag%)
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Der Eintrag konnte nicht geändert werden",error$,262144+48)
endproc

Proc MPEGGetFramePosition
Return MPEGGetFramePositionW() 'int
endproc

Proc MPEGGetFrameSize
Return MPEGGetFrameSizeW() 'int
endproc

Proc MPEGGetFrames
Return MPEGGetFramesW() 'int
endproc

Proc MPEGGetEmphasis
Var tmp& = MPEGGetEmphasisW()
Return WideToAnsi(tmp&)
endproc

Proc MPEGGetEncoder
Var tmp& = MPEGGetEncoderW()
Return WideToAnsi(tmp&)
endproc

Proc MPEGGetLayer
Var tmp& = MPEGGetLayerW()
Return WideToAnsi(tmp&)
endproc

Proc MPEGGetVersion
Var tmp& = MPEGGetVersionW()
Return WideToAnsi(tmp&)
endproc

Proc MPEGIsCopyrighted
var tmp& = MPEGIsCopyrightedW()
if tmp& = -1
return "Ja"
else
return "Nein"
endif
endproc

proc MPEGSetCopyrighted '1 setzen, 0 löschen
parameters file$,set%
file$ = AnsiToWide(file$)
return MPEGSetCopyrightedW(addr(file$),set%)
endproc

Proc MPEGSetOriginal
parameters file$,set%
file$ = AnsiToWide(file$)
return MPEGSetOriginalW(addr(file$),set%)
endproc

Proc MPEGSetPrivate
parameters file$,set%
file$ = AnsiToWide(file$)
return MPEGSetPrivateW(addr(file$),set%)
endproc

Proc MPEGIsOriginal
Var tmp& = MPEGIsOriginalW()
if tmp& = -1
return "Ja"
else
return "Nein"
endif
endproc

Proc MPEGIsPadding
var tmp& = MPEGIsPaddingW()
if tmp& = -1
return "Ja"
else
return "Nein"
endif
endproc

Proc MPEGIsPrivate
Var tmp& = MPEGIsPrivateW()
if tmp& = -1
return "Ja"
else
return "Nein"
endif
endproc

Proc MPEGIsProtected
var tmp& = MPEGIsProtectedW()
if tmp& = -1
return "Ja"
else
return "Nein"
endif
endproc

Proc MPEGIsVBR
Var tmp% = MPEGIsVBRW()
if tmp% = -1
return "Ja"
else
return "Nein"
endif
endproc
'------------------------Lyrics------------------------
Proc LYRICSExcists
var tmp& = LYRICSExistsW()
if tmp& =1
return "Ja"
else
return "Nein"
endif
endproc

Proc LYRICSGetSize
Return LYRICSGetSizeW()
endproc

Proc LYRICSGetStartPosition
Return LYRICSGetStartPositionW()
endproc

Proc LYRICSGetVersion
Var tmp& = LYRICSGetVersionW()
Return WideToAnsi(tmp&)
endproc

Proc LyricsGetAlbum
Var tmp& = LYRICSGetAlbumW()
Return WideToAnsi(tmp&)
endproc

Proc LyricsGetArtist
Var tmp& = LYRICSGetArtistW()
Return WideToAnsi(tmp&)
endproc

Proc LyricsGetAuthor
Var tmp& = LYRICSGetAuthorW()
Return WideToAnsi(tmp&)
endproc

Proc LyricsGetGenre
Var tmp& = LYRICSGetGenreW()
Return WideToAnsi(tmp&)
endproc

Proc LyricsGetImageLink
Var tmp& = LYRICSGetImageLinkW()
Return WideToAnsi(tmp&)
endproc

Proc LyricsGetIndication
Var tmp& = LYRICSGetIndicationW()
Return WideToAnsi(tmp&)
endproc

Proc LyricsGetInformation
Var tmp& = LYRICSGetInformationW()
Return WideToAnsi(tmp&)
endproc

Proc LyricsGetLyrics
Var tmp& = LYRICSGetLyricsW()
Return WideToAnsi(tmp&)
endproc

Proc LyricsGetTitle
Var tmp& = LYRICSGetTitleW()
Return WideToAnsi(tmp&)
endproc

Proc LYRICSSetAlbum
Parameters wert$
wert$ = AnsiToWide(wert$)
LYRICSSetAlbumW(addr(wert$))
endproc

Proc LYRICSSetArtist
Parameters wert$
wert$ = AnsiToWide(wert$)
LYRICSSetArtistW(addr(wert$))
endproc

Proc LYRICSSetAuthor
Parameters wert$
wert$ = AnsiToWide(wert$)
LYRICSSetAuthorW(addr(wert$))
endproc

Proc LYRICSSetGenre
Parameters wert$
wert$ = AnsiToWide(wert$)
LYRICSSetGenreW(addr(wert$))
endproc

Proc LYRICSSetImageLink
Parameters wert$
wert$ = AnsiToWide(wert$)
LYRICSSetImageLinkW(addr(wert$))
endproc

Proc LYRICSSetIndication
Parameters wert$
wert$ = AnsiToWide(wert$)
LYRICSSetIndicationW(addr(wert$))
endproc

Proc LYRICSSetInformation
Parameters wert$
wert$ = AnsiToWide(wert$)
LYRICSSetInformationW(addr(wert$))
endproc

Proc LYRICSSetLyrics
Parameters wert$
wert$ = AnsiToWide(wert$)
LYRICSSetLyricsW(addr(wert$))
endproc

Proc LYRICSSetTitle
Parameters wert$
wert$ = AnsiToWide(wert$)
LYRICSSetTitleW(addr(wert$))
endproc

Proc LYRICSRemoveTag
var tmp& = LYRICSRemoveTagW()
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Der Tag konnte nicht entfernt werden",error$,262144+48)
endproc

Proc LYRICSRemoveTagFromFile
parameters file$
file$ = AnsiToWide(file$)
var tmp& = LYRICSRemoveTagFromFileW(addr(file$))
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Der Tag konnte nicht entfernt werden",error$,262144+48)
endproc

Proc LYRICSSaveChanges
var tmp& = LYRICSSaveChangesW()
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Der Tag konnte nicht gespeichert werden",error$,262144+48)
endproc

Proc LyricsSaveChangesToFile
parameters file$
file$ = AnsiToWide(file$)
var tmp& = LYRICSSaveChangesToFileW(addr(file$))
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Der Tag konnte nicht gespeichert werden",error$,262144+48)
endproc

'----------------------------APE--------------------
Proc APEExists
var tmp% = APEExistsW()
if tmp% = -1
return "Ja"
else
return "Nein"
endif
endproc

Proc APEGetSize
return APEGetSizeW()
endproc

Proc APEGetVersion
var tmp& = APEGetVersionW()
return wideToAnsi(tmp&)
endproc

Proc APEGetItemkeys
var tmp& = APEGetItemKeysW()
return wideToAnsi(tmp&)
endproc

Proc APERemoveTag
Var tmp& = APERemoveTagW()
Var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Der Tag konnte nicht gelöscht werden "+str$(tmp&),error$,262144 + 64)
endproc

Proc APERemoveTagFromFile
Parameters file$
file$ = ansiToWide(file$)
var tmp& = APERemoveTagFromFileW(addr(file$))
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox(error$,"Der Tag konnte nicht gelöscht werden",262144 + 64)
endproc

Proc APESaveChanges
var tmp& = APESaveChangesW()
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox(error$,"Der Tag konnte nicht gespeichert werden",262144 + 64)
endproc

proc APESaveChangesToFile
Parameters file$
file$ = ansiToWide(file$)
var tmp& = APESaveChangesToFileW(addr(file$))
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox(error$,"Der Tag konnte nicht gespeichert werden",262144 + 64)
endproc

Proc APEGetAlbum
Var tmp& = APEGetAlbumW()
return wideToAnsi(tmp&)
endproc

Proc APEGetArtist
var tmp& = APEGetArtistW()
return wideToAnsi(tmp&)
endproc

Proc APEGetComment
var tmp& = APEGetCommentW()
return wideToAnsi(tmp&)
endproc

Proc APEGetCopyright
var tmp& = APEGetCopyrightW()
return wideToAnsi(tmp&)
endproc

Proc APEGetGenre
var tmp& = APEGetGenreW()
return wideToAnsi(tmp&)
endproc

Proc APEGetTitle
Var tmp& = APEGetTitleW()
return wideToAnsi(tmp&)
endproc

Proc APEGetTrack
var tmp& = APEGetTrackW()
return wideToAnsi(tmp&)
endproc

Proc APEGetUserItem
parameters key$
key$ = ansiToWide(key$)
var tmp& = APEGetUserItemW(addr(key$))
return WideToAnsi(tmp&)
endproc

Proc APEGetYear
var tmp& = APEGetYearW()
return wideToAnsi(tmp&)
endproc

Proc APESetAlbum
parameters wert$
wert$ = ansiToWide(wert$)
APESetAlbumW(addr(wert$))
endproc

proc APESetArtist
parameters wert$
wert$ = ansiToWide(wert$)
APESetArtistW(addr(wert$))
endproc

proc APESetComment
parameters wert$
wert$ = ansiToWide(wert$)
APESetCommentW(addr(wert$))
endproc

proc APESetCopyright
parameters wert$
wert$ = ansiToWide(wert$)
APESetCopyrightW(addr(wert$))
endproc

proc APESetGenre
parameters wert$
wert$ = ansiToWide(wert$)
APESetGenreW(addr(wert$))
endproc

proc APESetTitle
parameters wert$
wert$ = ansiToWide(wert$)
APESetTitleW(addr(wert$))
endproc

proc APESetTrack
parameters wert$
wert$ = ansiToWide(wert$)
APESetTrackW(addr(wert$))
endproc

proc APESetUserItem
parameters key$,wert$
key$ = ansiToWide(key$)
wert$ = ansiToWide(wert$)
APESetUserItemW(addr(key$),addr(wert$))
endproc

proc APESetYear
parameters wert$
wert$ = ansiToWide(wert$)
APESetYearW(addr(wert$))
endproc
'------------------------AAC---------------------------------------
Proc AACGetBitRateType
var tmp& = AACGetBitRateTypeW()
return wideToAnsi(tmp&)
endproc

proc AACGetHeaderType
var tmp& = AACGetHeaderTypeW()
return wideToAnsi(tmp&)
endproc

Proc AACGetProfile
var tmp& = AACGetProfileW()
return wideToAnsi(tmp&)
endproc
'------------------------------Monkey------------------------------
proc MonkeyGetBits
return MONKEYGetBitsW()
endproc

proc MonkeyGetCompression
var tmp& = MONKEYGetCompressionW()
return WideToAnsi(tmp&)
endproc

proc MonkeyGetCompressionRatio 'gibt Programmabsturz
return MONKEYGetCompressionRatioW()
endproc

proc MONKEYGetFrames
return MONKEYGetFramesW()
endproc

Proc MONKEYGetPeak 'gibt Programmabsturz
return MONKEYGetPeakW()
endproc

proc MonkeyGetsamples
return MONKEYGetSamplesW()
endproc

proc MonkeyGetSamplesPerFrame
return MONKEYGetSamplesPerFrameW()
endproc

'------------------------------------MPP------------------------
Proc MPPGetFrames
Return MPPGetFramesW()
endproc

Proc MPPGetStreamVersion
Return MPPGetStreamVersionW()
endproc

'----------------------------WAV------------------------------
proc WAVSaveChangesToFile
parameters file$
file$ = AnsitoWide(file$)
var tmp% = WAVSaveChangesToFileW(addr(file$))
if tmp% <> -1
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Tags konnten nicht geschrieben werden"
'Messagebox(error$,"Fehler beim Schreiben",262144+48)
endif
endproc

Proc WAVSaveChanges
var tmp% = WAVSaveChangesW()
if tmp% <> -1
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Tags konnten nicht geschrieben werden"
'Messagebox(error$,"Fehler beim Schreiben",262144+48)
endif
endproc

Proc WAVGetCartChunkEntry '0-26
parameters index%
var tmp& = WAVGetCartChunkEntryW(index%)
return WideToAnsi(tmp&)
endproc

proc WAVGetInfoChunkIDs 'Kommagetrennte Liste
var tmp& = WAVGetInfoChunkIDsW()
return WideToAnsi(tmp&)
endproc

proc WAVSetCartChunkEntry
parameters index%,text$
text$ = AnsiToWide(text$)
WAVSetCartChunkEntryW(index%,addr(text$))
endproc

proc WAVGetBitsPerSample
Return WAVGetBitsPerSampleW()
endproc

Proc WAVGetBlockalign
Return WAVGetBlockAlignW()
endproc

Proc WAVGetBytesPerSecond
return WAVGetBytesPerSecondW()
endproc

Proc WAVGetChannels
Return WAVGetChannelsW()
endproc

proc WAVGetFormat
Var tmp& = WAVGetFormatW()
Return WideToAnsi(tmp&)
endproc

proc WAVGetFormatID
return WAVGetFormatIDW()
endproc

proc WAVGetHeaderSize
return WAVGetHeaderSizeW()
endproc

Proc WAVGetTextFrame
Parameters id&
var tmp& = WAVGetTextFrameW(id&)
return WidetoAnsi(tmp&)
endproc

proc WAVSetTextFrame
parameters id&,text$
text$ = AnsitoWide(text$)
WAVSetTextFrameW(id&,addr(text$))
endproc
'----------------------------------------------------------
proc WavGetArchivalLocation 'Aufbewahrungsort
return WAVGetTextFrame(&WAV_IARL)
endproc

proc WAVGetArtist
return WAVGetTextFrame(&WAV_IART)
endproc

Proc WAVGetComissioned 'Auftraggeber
return WAVGetTextFrame(&WAV_ICMS)
endproc

Proc WAVGetComment
return WAVGetTextFrame(&WAV_ICMT)
endproc

Proc WAVGetCopyright
return WAVGetTextFrame(&WAV_ICOP)
endproc

Proc WAVGetCreationDate
return WAVGetTextFrame(&WAV_ICRD)
endproc

Proc WAVGetCropped 'Bildbeschneidung
return WAVGetTextFrame(&WAV_ICRP)
endproc

Proc WAVGetDimension
return WAVGetTextFrame(&WAV_IDIM)
endproc

Proc WAVDotsPerInch
return WAVGetTextFrame(&WAV_IDPI)
endproc

Proc WAVGetEngineer 'Entwickler
return WAVGetTextFrame(&WAV_IENG)
endproc

Proc WAVGetGenre
return WAVGetTextFrame(&WAV_IGNR)
endproc

Proc WAVGetKeywords 'Stichwörter
return WAVGetTextFrame(&WAV_IKEY)
endproc

Proc WAVGetLightness 'Beleuchtung
return WAVGetTextFrame(&WAV_ILGT)
endproc

Proc WAVGetMedium 'Originalmedium
return WAVGetTextFrame(&WAV_IMED)
endproc

Proc WAVGetName   'Titel
return WAVGetTextFrame(&WAV_INAM)
endproc

Proc WAVGetPaletteSetting 'Anzahl Farben
return WAVGetTextFrame(&WAV_IPLT)
endproc

Proc WAVGetProduct 'Originaltitel
return WAVGetTextFrame(&WAV_IPRD)
endproc

Proc WAVGetSharpness 'Schärfe
return WAVGetTextFrame(&WAV_ISHP)
endproc

Proc WAVGetSoftware
return WAVGetTextFrame(&WAV_ISFT)
endproc

Proc WAVGetSource 'Original
return WAVGetTextFrame(&WAV_ISRC)
endproc

Proc WAVGetSourceForm 'Originalformat
return WAVGetTextFrame(&WAV_ISRF)
endproc

Proc WAVGetSubject 'Sparte
return WAVGetTextFrame(&WAV_ISBJ)
endproc

Proc WAVGetTechnician
return WAVGetTextFrame(&WAV_ITCH)
endproc

proc WAVGetTrack
return WAVGetTextFrame(&WAV_ITRK)
endproc
'------------------------schreiben-----------------
Proc WAVSetArchivalLocation
Parameters wert$
WAVSetTextFrame(&WAV_IARL,wert$)
endproc

Proc WAVSetArtist
Parameters wert$
WAVSetTextFrame(&WAV_IART,wert$)
endproc

Proc WAVSetComissioned
Parameters wert$
WAVSetTextFrame(&WAV_ICMS,wert$)
endproc

Proc WAVSetComment
Parameters wert$
WAVSetTextFrame(&WAV_ICMT,wert$)
endproc

Proc WAVSetCopyright
Parameters wert$
WAVSetTextFrame(&WAV_ICOP,wert$)
endproc

Proc WAVSetCreationDate
Parameters wert$
WAVSetTextFrame(&WAV_ICRD,wert$)
endproc

Proc WAVSetCropped
Parameters wert$
WAVSetTextFrame(&WAV_ICRP,wert$)
endproc

Proc WAVSetDimension
Parameters wert$
WAVSetTextFrame(&WAV_IDIM,wert$)
endproc

Proc WAVSetDotsPerInch
Parameters wert$
WAVSetTextFrame(&WAV_IDPI,wert$)
endproc

Proc WAVSetEngineer
Parameters wert$
WAVSetTextFrame(&WAV_IENG,wert$)
endproc

Proc WAVSetGenre
Parameters wert$
WAVSetTextFrame(&WAV_IGNR,wert$)
endproc

Proc WAVSetKeywords
Parameters wert$
WAVSetTextFrame(&WAV_IKEY,wert$)
endproc

Proc WAVSetLightness
Parameters wert$
WAVSetTextFrame(&WAV_ILGT,wert$)
endproc

Proc WAVSetMedium
Parameters wert$
WAVSetTextFrame(&WAV_IMED,wert$)
endproc

Proc WAVSetName
Parameters wert$
WAVSetTextFrame(&WAV_INAM,wert$)
endproc

Proc WAVSetPaletteSetting
Parameters wert$
WAVSetTextFrame(&WAV_IPLT,wert$)
endproc

Proc WAVSetProduct
Parameters wert$
WAVSetTextFrame(&WAV_IPRD,wert$)
endproc

Proc WAVSetSharpness
Parameters wert$
WAVSetTextFrame(&WAV_ISHP,wert$)
endproc

Proc WAVSetSoftware
Parameters wert$
WAVSetTextFrame(&WAV_ISFT,wert$)
endproc

Proc WAVSetSource
Parameters wert$
WAVSetTextFrame(&WAV_ISRC,wert$)
endproc

Proc WAVSetSourceForm
Parameters wert$
WAVSetTextFrame(&WAV_ISRF,wert$)
endproc

Proc WAVSetSubject
Parameters wert$
WAVSetTextFrame(&WAV_ISBJ,wert$)
endproc

Proc WAVSetTechnician
Parameters wert$
WAVSetTextFrame(&WAV_ITCH,wert$)
endproc

Proc WAVSetTrack
Parameters wert$
WAVSetTextFrame(&WAV_ITRK,wert$)
endproc

'--------------------------------FLAC--------------------------------------------
Proc FlacExists
parameters file$
var size! = filesize(file$)
declare file#,offset&
dim file#,size!
blockread(file$,file#,0,size!)
var such$ = char$(file#,0,size!)
dispose file#
offset& = instr("fLaC",such$)
if offset&
dec offset&
return "Ja|"+str$(offset&)
else
return "Nein"
endif
endproc

Proc FlacGetBitsPerSample
Return FLACGetBitsPerSampleW()
endproc

proc FLACGetCompressionRatio 'Vorsicht Float
return FLACGetCompressionRatioW()
endproc

Proc FLACGetItemKeys
var tmp& = FLACGetItemKeysW()
return WideToAnsi(tmp&)
endproc

proc FLACGetMaxBlockSize
Return FLACGetMaxBlockSizeW()
endproc

Proc FLACGetMaxFrameSize
return FLACGetMaxFrameSizeW()
endproc

Proc FLACGetMinBlockSize
return FLACGetMinBlockSizeW()
endproc

Proc FLACGetMinFrameSize
return FLACGetMinFrameSizeW()
endproc

Proc FLACGetSamples
return FLACGetSamplesW()
endproc

proc FLACSaveChanges
var tmp& = FLACSaveChangesW()
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Der Tag konnte nicht gespeichert werden",error$,262144 + 64)
endproc

Proc FLACSaveChangesToFile
parameters file$
file$ = AnsiToWide(file$)
var tmp& = FLACSaveChangesToFileW(addr(file$))
var error$ = AUDIOGetLastErrorText()
'case tmp& = 0:Messagebox("Der Tag konnte nicht gespeichert werden",error$,262144 + 64)
endproc

proc FLACGetUserItem
parameters key$
key$ = AnsiToWide(key$)
var tmp& = FLACGetUserItemW(addr(key$))
return WideToAnsi(tmp&)
endproc

proc FLACGetAlbum
return FLACGetUserItem("ALBUM")
endproc

proc FLACGetArtist
return FLACGetUserItem("ARTIST")
endproc

proc FLACGetComment
return FLACGetUserItem("COMMENT")
endproc

Proc FLACGetContact
return FLACGetUserItem("CONTACT")
endproc

Proc FLACGetCopyright
return FLACGetUserItem("COPYRIGHT")
endproc

Proc FLACGetDate
return FLACGetUserItem("DATE")
endproc

Proc FLACGetDescription
return FLACGetUserItem("DESCRIPTION")
endproc

Proc FLACGetGenre
return FLACGetUserItem("GENRE")
endproc

Proc FLACGetISRC
return FLACGetUserItem("ISRC")
endproc

Proc FLACGetLicense
return FLACGetUserItem("LICENSE")
endproc

Proc FLACGetLocation
return FLACGetUserItem("LOCATION")
endproc

proc FLACGetOrganization
return FLACGetUserItem("ORGANIZATION")
endproc

proc FLACGetPerformer
return FLACGetUserItem("PERFORMER")
endproc

Proc FLACGetTitle
return FLACGetUserItem("TITLE")
endproc

proc FLACGetTrack
return FLACGetUserItem("TRACK")
endproc

proc FLACGetVendor
var tmp& = FLACGetVendorW()
return WideToAnsi(tmp&)
endproc

proc FLACGetVersion
return FLACGetUserItem("VERSION")
endproc

proc FLACSetUserItem
parameters key$,wert$
key$ = AnsiToWide(key$)
wert$ = AnsiToWide(wert$)
FLACSetUserItemW(addr(key$),addr(wert$))
endproc

proc FLACSetAlbum
parameters wert$
FLACSetUserItem("ALBUM",wert$)
endproc

proc FLACSetArtist
parameters wert$
FLACSetUserItem("ARTIST",wert$)
endproc

proc FLACSetComment
parameters wert$
FLACSetUserItem("COMMENT",wert$)
endproc

Proc FLACSetContact
parameters wert$
FLACSetUserItem("CONTACT",wert$)
endproc

Proc FLACSetCopyright
parameters wert$
FLACSetUserItem("COPYRIGHT",wert$)
endproc

Proc FLACSetDate
parameters wert$
FLACSetUserItem("DATE",wert$)
endproc

Proc FLACSetDescription
parameters wert$
FLACSetUserItem("DESCRIPTION",wert$)
endproc

Proc FLAcSetGenre
parameters wert$
FLACSetUserItem("GENRE",wert$)
endproc

Proc FLACSetISRC
parameters wert$
FLACSetUserItem("ISRC",wert$)
endproc

Proc FLACSetLicense
parameters wert$
FLACSetUserItem("LICENSE",wert$)
endproc

Proc FLACSetLocation
parameters wert$
FLACSetUserItem("LOCATION",wert$)
endproc

proc FLACSetOrganization
parameters wert$
FLACSetUserItem("ORGANIZATION",wert$)
endproc

proc FLACSetPerformer
parameters wert$
FLACSetUserItem("PERFORMER",wert$)
endproc

Proc FLACSetTitle
parameters wert$
FLACSetUserItem("TITLE",wert$)
endproc

proc FLACSetTrack
parameters wert$
FLACSetUserItem("TRACK",wert$)
endproc

proc FLACSetVendor
parameters wert$
wert$ = AnsiToWide(wert$)
FLACSetVendorW(addr(wert$))
endproc

proc FLACSetVersion
parameters wert$
FLACSetUserItem("VERSION",wert$)
endproc

Proc FLACAddPictureArray
parameters array#,len&,desc$,ptyp&
desc$ = AnsiToWide(desc$)
return FLACAddPictureArrayW(array#,len&,addr(desc$),ptyp&)
'-1 falls frame ersetzt wurde, 0 falls neuer Eintrag erfolgte
endproc

Proc FLACAddPictureFile
parameters file$,desc$,ptyp%,aslink% '-1 es wird nur der Dateiname, 0 = das Bild selbst
file$ = AnsiToWide(file$)
desc$ = AnsiToWide(desc$)
var tmp& = FLACAddPictureFileW(addr(file$),addr(desc$),ptyp%,aslink%)
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler Bild einfügen (ID3V2AddPictureFile)"
'case tmp& = 0:Messagebox("Das Bild konnte nicht eingefügt werden",error$,262144+64)
endproc

proc FLACDeletePicture
parameters index%
return FLACDeletePictureW(index%)
endproc

proc FLACDeletePictures
FLACDeletePicturesW()
endproc

Proc FLACGetPictureArray
parameters array#,len&,index%
return FLACGetPictureArrayW(array#,len&,index%)
endproc

proc FLACGetPictureCount
return loword(FLACGetPictureCountW())
endproc

proc FLACGetPictureDescription
parameters index%
var tmp& = FLACGetPictureDescriptionW(index%)
return WideToAnsi(tmp&)
endproc

Proc FLACGetPictureFile 'bild in datei
parameters file$,index%
file$ = AnsiToWide(file$)
Var tmp% =  FLACGetPictureFileW(addr(file$),index%)
var error$ = AUDIOGetLastErrorText()
case error$ = "":error$ = "Fehler beim Bild einlesen ( FLACGetPictureFile)"
'case tmp% = 0:Messagebox(error$,"Es ist ein Fehler aufgetreten",262144+64)
return tmp%
endproc

Proc FLACGetPictureMime
parameters index%
Var tmp& = FLACGetPictureMimeW(index%)
return WideToAnsi(tmp&)
endproc

proc FLACGetPictureSize
parameters index%
return FLACGetPictureSizeW(index%) 'int32
endproc

Proc FLACGetPictureTypeText
parameters index%
var tmp& = FLACGetPictureTypeTextW(index%)
Return WideToAnsi(tmp&)
endproc

Proc FLACGetPictureType
Parameters index%
Return FLACGetPictureTypeW(index%) 'int16
endproc

'______________________MP4__________________
Proc MP4GetVersion
var tmp& = MP4GetVersionW()
return WideToAnsi(tmp&)
endproc

proc MP4SaveChanges
var er% = MP4SaveChangesW()
If er% <> -1
'Messagebox(AUDIOGetLastErrorText(),"Konnte den Tag nicht schreiben",64)
endif
endproc

Proc MP4SaveChangesToFile
parameters file$
file$ = AnsiToWide(file$)
var er% = MP4SaveChangesToFileW(addr(file$))
If er% <> -1
'Messagebox(AUDIOGetLastErrorText(),"Konnte den Tag nicht schreiben",64)
endif
endproc

proc MP4SetiTuneFrame
parameters frameID$,text$
frameID$ = AnsiToWide(frameID$)
text$ = AnsiToWide(text$)
MP4SetiTuneFrameW(addr(frameID$),addr(text$))
endproc

proc MP4GetiTuneFrame
parameters frameID$
frameID$ = AnsiToWide(frameID$)
var tmp& = MP4GetiTuneFrameW(addr(frameID$))
return WideToAnsi(Tmp&)
endproc

Proc MP4DeleteEntries 'löscht alle benutzerdefinierten Einträge wie Titel, Bilder etc.
MP4DeleteEntriesW()
endproc

proc MP4GetAllFrameIDs 'kommagetrennte Liste
var tmp& = MP4GetAllFrameIDsW()
return WideToAnsi(tmp&)
endproc

proc MP4GetTextFrame
parameters id&
var tmp& = MP4GetTextFrameW(id&)
return WideToAnsi(tmp&)
endproc

Proc MP4SetTextFrame
parameters id!,wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(id!,addr(wert$))
endproc

Proc MP4getAlbum
var tmp& = MP4GetTextFrameW(&MP4_ALBUM)
Return WideToAnsi(tmp&)
endproc

Proc MP4getAlbumArtist
var tmp& = MP4GetTextFrameW(&MP4_ALBUMARTIST)
Return WideToAnsi(tmp&)
endproc

Proc MP4getCategory
var tmp& = MP4GetTextFrameW(&MP4_CATEGORY)
Return WideToAnsi(tmp&)
endproc

Proc MP4getDate
var tmp& = MP4GetTextFrameW(&MP4_CREATEDATE)
Return WideToAnsi(tmp&)
endproc

Proc MP4getInformation
var tmp& = MP4GetTextFrameW(&MP4_INFORMATION)
Return WideToAnsi(tmp&)
endproc

Proc MP4getGrouping
var tmp& = MP4GetTextFrameW(&MP4_GROUPING)
Return WideToAnsi(tmp&)
endproc

Proc MP4getLyrics
var tmp& = MP4GetTextFrameW(&MP4_LYRICS)
Return WideToAnsi(tmp&)
endproc

Proc MP4getKeyWord
var tmp& = MP4GetTextFrameW(&MP4_KEYWORD)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetArtist
var tmp& = MP4GetTextFrameW(&MP4_ARTIST)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetArtistb
var tmp& = MP4GetTextFrameW(&MP4_ARTISTb)
Return WideToAnsi(tmp&)
endproc


Proc MP4GetAuthor
var tmp& = MP4GetTextFrameW(&MP4_AUTHOR)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetBeatsPerMinute
var tmp& = MP4GetTextFrameW(&MP4_BEATSPERMINUTE)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetCDNumber
var tmp& = MP4GetTextFrameW(&MP4_CDSETNUMBER)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetComment
var tmp& = MP4GetTextFrameW(&MP4_COMMENT)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetCompilation
Var tmp& = MP4GetTextFrameW(&MP4_COMPILATION)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetComposer
var tmp& = MP4GetTextFrameW(&MP4_COMPOSER)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetCopyright
var tmp& = MP4GetTextFrameW(&MP4_COPYRIGHT)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetDescription
Var tmp& = MP4GetTextFrameW(&MP4_DESCRIPTION)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetDirector
var tmp& = MP4GetTextFrameW(&MP4_DIRECTOR)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetDisclaimer
var tmp& = MP4GetTextFrameW(&MP4_DISCLAIMER)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetEncoder
var tmp& = MP4GetTextFrameW(&MP4_ENCODER)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetGenre
var tmp& = MP4GetGenreW()
Return WideToAnsi(tmp&)
endproc

Proc MP4GetNetUrl
var tmp& = MP4GetTextFrameW(&MP4_NETURL)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetOriginalArtist
var tmp& = MP4GetTextFrameW(&MP4_ORIGINALARTIST)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetOriginalFormat
var tmp& = MP4GetTextFrameW(&MP4_ORIGINALFORMAT)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetoriginalSource
var tmp& = MP4GetTextFrameW(&MP4_ORIGINALSOURCE)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetProducer
var tmp& = MP4GetTextFrameW(&MP4_PRODUCER)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetTitle
var tmp& = MP4GetTextFrameW(&MP4_TITLE)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetTrack
var tmp& = MP4GetTrackW()
Return WideToAnsi(tmp&)
endproc

Proc MP4GetWarning
var tmp& = MP4GetTextFrameW(&MP4_WARNING)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetPerformer
var tmp& = MP4GetTextFrameW(&MP4_PERFORMER)
Return WideToAnsi(tmp&)
endproc

Proc MP4GetPictureCount 'Anzahl Bilder
return MP4GetPictureCountW()
endproc

proc MP4GetPictureSize
parameters index%
return MP4GetPictureSizeW(index%)
endproc

proc MP4GetPictureMime
Parameters index%
var tmp& = MP4GetPictureMimeW(index%)
return WideToAnsi(tmp&)
endproc

proc MP4GetPictureArray ' Bild aus Frame in Speicher, Speicherbereich muss außerhalb gedimt und freigegeben werden
parameters mem#,size&,ind%  'bereich, bildindex
Return MP4GetPictureArrayW(mem#,size&,ind%) 'Returnwert Größe des beschriebenen Bereiches
endproc

Proc MP4GetPictureFile 'überträgt Bild in Datei
parameters file$,ind%
file$ = AnsiToWide(file$)
var tmp% = MP4GetPictureFileW(addr(file$),ind%)
var error$ = AudioGetLastErrorText()
'case tmp% = 0:Messagebox(error$,"Das Bild konnte nicht gespeichert werden",262144+64)
return tmp%
endproc

Proc MP4AddPictureArray 'Bild aus Speicher in Frame. Bild muss außerhalb in Speicher getsetzt werden
parameters mem#,size&
Return MP4AddPictureArrayW(mem#,size&)
endproc

Proc MP4AddPictureFile 'Bild von Datei in Tag
parameters file$
file$ = AnsiToWide(file$)
var tmp% = MP4AddPictureFileW(addr(file$))
var error$ = AudioGetLastErrorText()
'case tmp% = 0:Messagebox(error$,"Das Bild konnte nicht eingefügt werden",262144+64)
clear file$
return tmp%
endproc

proc  MP4DeletePicture
parameters index%
var tmp% =  MP4DeletePictureW(index%)
if tmp% <> -1
var error$ = AUDIOGetLastErrorText()
'Messagebox(error$,"Beim Löschen ist ein Fehler aufgetreten",262144+48)
endif
endproc

Proc  MP4DeletePictures
 MP4DeletePicturesW()
endproc

proc MP4SetAlbum
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_ALBUM,addr(wert$))
endproc

proc MP4SetArtist
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_ARTIST,addr(wert$))
endproc

proc MP4SetArtistb
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_ARTISTb,addr(wert$))
endproc

proc MP4SetAlbumArtist
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_ALBUMARTIST,addr(wert$))
endproc

proc MP4SetCategory
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_CATEGORY,addr(wert$))
endproc

proc MP4SetDate
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_CREATEDATE,addr(wert$))
endproc

proc MP4SetInformation
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_INFORMATION,addr(wert$))
endproc

proc MP4SetGrouping
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_GROUPING,addr(wert$))
endproc

proc MP4SetLyrics
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_LYRICS,addr(wert$))
endproc

Proc MP4SetAuthor
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_AUTHOR,addr(wert$))
endproc

Proc MP4SetBeatsPerMinute
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_BEATSPERMINUTE,addr(wert$))
endproc

Proc MP4SetCDNumber
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_CDSETNUMBER,addr(wert$))
endproc

Proc MP4SetComment
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_COMMENT,addr(wert$))
endproc

Proc MP4SetCompilation
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_COMPILATION,addr(wert$))
endproc

Proc MP4SetComposer
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_COMPOSER,addr(wert$))
endproc

Proc MP4SetCopyright
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_COPYRIGHT,addr(wert$))
endproc

Proc MP4SetDescription
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_DESCRIPTION,addr(wert$))
endproc

Proc MP4SetDirector
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_DIRECTOR,addr(wert$))
endproc

Proc MP4SetDisclaimer
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_DISCLAIMER,addr(wert$))
endproc

Proc MP4SetEncoder
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_ENCODER,addr(wert$))
endproc

Proc MP4SetGenre
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetGenreW(addr(wert$))
endproc

Proc MP4SetNetUrl
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_NETURL,addr(wert$))
endproc

Proc MP4SetOriginalArtist
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_ORIGINALARTIST,addr(wert$))
endproc

Proc MP4SetOriginalFormat
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_ORIGINALFORMAT,addr(wert$))
endproc

Proc MP4SetOriginalSource
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_ORIGINALSOURCE,addr(wert$))
endproc

Proc MP4SetPerformer
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_PERFORMER,addr(wert$))
endproc

Proc MP4SetProducer
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_PRODUCER,addr(wert$))
endproc

Proc MP4SetTitle
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_TITLE,addr(wert$))
endproc

Proc MP4SetTrack
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTrackW(addr(wert$))
endproc

Proc MP4SetWarning
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_WARNING,addr(wert$))
endproc

proc MP4SetKeyWord
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_KEYWORD,addr(wert$))
endproc
'Zusatz
proc MP4getDisknumber
var tmp& = MP4GetTextFrameW(&MP4_DiskNumber)
Return WideToAnsi(tmp&)
endproc

Proc MP4getCopyright2
var tmp& = MP4GetTextFrameW(&MP4_Copyrighta)
Return WideToAnsi(tmp&)
endproc

Proc Mp4getDescrition2
var tmp& = MP4GetTextFrameW(&MP4_Descriptiona)
Return WideToAnsi(tmp&)
endproc

Proc MP4getGenre2
var tmp& = MP4GetTextFrameW(&MP4_genre2)
Return WideToAnsi(tmp&)
endproc

Proc MP4getRating
var tmp& = MP4GetTextFrameW(&MP4_Rating)
Return WideToAnsi(tmp&)
endproc

Proc MP4getStick
var tmp& = MP4GetTextFrameW(&MP4_Stik)
Return WideToAnsi(tmp&)
endproc

Proc MP4getPodcast
var tmp& = MP4GetTextFrameW(&MP4_Podcast)
Return WideToAnsi(tmp&)
endproc

Proc MP4getPodcastUrl
var tmp& = MP4GetTextFrameW(&MP4_Podcasturl)
Return WideToAnsi(tmp&)
endproc

Proc MP4getEpisodeGlobalUniqueID
var tmp& = MP4GetTextFrameW(&MP4_EpisodeGlobalUniqueID)
Return WideToAnsi(tmp&)
endproc

Proc MP4getTVShowName
var tmp& = MP4GetTextFrameW(&MP4_TVShowName)
Return WideToAnsi(tmp&)
endproc

Proc MP4getTVNetworkName
var tmp& = MP4GetTextFrameW(&MP4_TVNetworkName)
Return WideToAnsi(tmp&)
endproc

Proc MP4getTVEpisodeNumber
var tmp& = MP4GetTextFrameW(&MP4_TVEpisodeNumber)
Return WideToAnsi(tmp&)
endproc

Proc MP4getTVSeason
var tmp& = MP4GetTextFrameW(&MP4_TVSeason)
Return WideToAnsi(tmp&)
endproc

Proc MP4getTVEpisode
var tmp& = MP4GetTextFrameW(&MP4_TVEpisode)
Return WideToAnsi(tmp&)
endproc

Proc MP4getPurchaseDate
var tmp& = MP4GetTextFrameW(&MP4_PurchaseDate)
Return WideToAnsi(tmp&)
endproc

Proc MP4getGaplessPlayback
var tmp& = MP4GetTextFrameW(&MP4_GaplessPlayback)
Return WideToAnsi(tmp&)
endproc
'------------------------------------------------------------
proc MP4SetDisknumber
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_DiskNumber,addr(wert$))
endproc

Proc MP4setDescrition2
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_Descriptiona,addr(wert$))
endproc

Proc MP4SetCopyright2
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_Copyrighta,addr(wert$))
endproc

Proc MP4Setgenre2
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_genre2,addr(wert$))
endproc

Proc MP4SetRating
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_Rating,addr(wert$))
endproc

Proc MP4SetStick
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_Stik,addr(wert$))
endproc

Proc MP4SetPodcast
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_Podcast,addr(wert$))
endproc

Proc MP4SetPodcastUrl
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_Podcasturl,addr(wert$))
endproc

Proc MP4SetEpisodeGlobalUniqueID
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_EpisodeGlobalUniqueID,addr(wert$))
endproc

Proc MP4SetTVShowName
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_TVShowName,addr(wert$))
endproc

Proc MP4SetTVNetworkName
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_TVNetworkName,addr(wert$))
endproc

Proc MP4SetTVEpisodeNumber
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_TVEpisodeNumber,addr(wert$))
endproc

Proc MP4SetTVSeason
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_TVSeason,addr(wert$))
endproc

Proc MP4SetTVEpisode
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_TVEpisode,addr(wert$))
endproc

Proc MP4SetPurchaseDate
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_PurchaseDate,addr(wert$))
endproc

Proc MP4SetGaplessPlayback
parameters wert$
wert$ = AnsiToWide(wert$)
MP4SetTextFrameW(&MP4_GaplessPlayback,addr(wert$))
endproc
'-----------------------------------Gruppe Allgemein-------------------
proc DisablePopup 'the DLL shows no popup any more (SetPopupKeyW does not exist): nothing to do, the procedure is kept for old programs
parameters code$
endproc

proc GetAudioGenieVersion
var tmp& = GetAudioGenieVersionW()
return wideToAnsi(tmp&)
endproc

Proc MD5GetValueFromFile
Parameters file$
file$ = AnsiToWide(file$)
Var tmp& = GetMD5ValueFromFileW(addr(file$))
Return wideToAnsi(tmp&)
endproc

Proc SetLogFile
parameters file$
file$ = AnsitoWide(file$)
SetLogFileW(addr(file$))
endproc

Proc SetConfigValue '0=MPEGEXACTREAD 1=ID3V2PADDINGSIZE 2=ID3V2WRITEBLOCKSIZE 3=DOEVENTSMILLIS
parameters setkey&,wert&
SetConfigValueW(setkey&,wert&)
endproc

Proc GetConfigValue
parameters key&
return GetConfigValueW(key&)
endproc

'-----------------------------------Functions added for AudioGenie3 3.2 and 3.3-------------------
Proc AudioGetComposer
var tmp& = AUDIOGetComposerW()
Return WideToAnsi(tmp&)
endproc

Proc AudioSetComposer
parameters wert$
wert$ = AnsiToWide(wert$)
AUDIOSetComposerW(addr(wert$))
endproc

Proc AudioGetDurationFloat 'Vorsicht Float, duration in seconds (AudioGetDuration returns whole seconds)
return AUDIOGetDurationW()
endproc

'--- ID3v1: enhanced tag (TAG+), the values are kept when the tag is written
Proc ID3V1GetSpeed '0 = not set, 1 = slow, 2 = medium, 3 = fast, 4 = hardcore
return loword(ID3V1GetSpeedW())
endproc

Proc ID3V1SetSpeed
parameters speed%
ID3V1SetSpeedW(speed%)
endproc

Proc ID3V1GetEnhancedGenre
var tmp& = ID3V1GetEnhancedGenreW()
Return WideToAnsi(tmp&)
endproc

Proc ID3V1SetEnhancedGenre
parameters wert$
wert$ = AnsiToWide(wert$)
ID3V1SetEnhancedGenreW(addr(wert$))
endproc

Proc ID3V1GetStartTime
var tmp& = ID3V1GetStartTimeW()
Return WideToAnsi(tmp&)
endproc

Proc ID3V1SetStartTime
parameters wert$
wert$ = AnsiToWide(wert$)
ID3V1SetStartTimeW(addr(wert$))
endproc

Proc ID3V1GetEndTime
var tmp& = ID3V1GetEndTimeW()
Return WideToAnsi(tmp&)
endproc

Proc ID3V1SetEndTime
parameters wert$
wert$ = AnsiToWide(wert$)
ID3V1SetEndTimeW(addr(wert$))
endproc

'--- ID3v2, cue files, WAV
Proc ID3V2GetEncoding 'text encoding of a frame: -1 = frame not found, 0 = ISO-8859-1, 1 = UTF-16 with BOM, 2 = UTF-16BE, 3 = UTF-8
parameters id&
return ID3V2GetEncodingW(id&)
endproc

Proc ID3V2ImportCueFile 'creates the chapter frames of the file from a cue file
parameters file$
file$ = AnsiToWide(file$)
return ID3V2ImportCueFileW(addr(file$))
endproc

Proc WAVGetDisplayText
var tmp& = WAVGetDisplayTextW()
Return WideToAnsi(tmp&)
endproc

Proc WAVSetDisplayText
parameters wert$
wert$ = AnsiToWide(wert$)
WAVSetDisplayTextW(addr(wert$))
endproc

'--- MPEG: LAME tag (the extension of the Xing/Info header)
Proc MPEGHasLameTag
Var tmp% = MPEGHasLameTagW()
if tmp% = -1
return "Ja"
else
return "Nein"
endif
endproc

Proc MPEGGetLameVersion
var tmp& = MPEGGetLameVersionW()
Return WideToAnsi(tmp&)
endproc

Proc MPEGGetLameRevision
return loword(MPEGGetLameRevisionW())
endproc

Proc MPEGGetLameVBRMethod '0 = unknown, 1 = CBR, 2 = ABR, 3 = VBR old, 4 = VBR mtrh, 5 = VBR mt, 8 = CBR 2 pass, 9 = ABR 2 pass
return loword(MPEGGetLameVBRMethodW())
endproc

Proc MPEGGetLameLowpass 'Hz, 0 = unknown
return MPEGGetLameLowpassW()
endproc

Proc MPEGGetLameBitrate 'kbit/s of the ABR or minimal bit rate, 0 = unknown
return MPEGGetLameBitrateW()
endproc

Proc MPEGGetEncoderDelay 'samples added at the start (gapless playback)
return MPEGGetEncoderDelayW()
endproc

Proc MPEGGetEncoderPadding 'samples added at the end (gapless playback)
return MPEGGetEncoderPaddingW()
endproc

Proc MPEGGetLamePeakSignal 'Vorsicht Float, 1.0 = maximal amplitude, 0 = unknown
return MPEGGetLamePeakSignalW()
endproc

Proc MPEGGetLameRadioGain 'Vorsicht Float, dB, 0 = not set
return MPEGGetLameRadioGainW()
endproc

Proc MPEGGetLameAudiophileGain 'Vorsicht Float, dB, 0 = not set
return MPEGGetLameAudiophileGainW()
endproc

Proc MPEGGetLameMp3Gain 'steps of 1.5 dB, -127 to 127
var tmp% = loword(MPEGGetLameMp3GainW())
case tmp% > 32767:tmp% = tmp% - 65536
return tmp%
endproc

Proc MPEGGetLamePreset '0 = unknown or none
return loword(MPEGGetLamePresetW())
endproc

Proc MPEGGetLameMusicLength 'bytes at the time of the encoding, 0 = unknown
return MPEGGetLameMusicLengthW()
endproc

Proc MPEGIsLameTagCrcValid
Var tmp% = MPEGIsLameTagCrcValidW()
if tmp% = -1
return "Ja"
else
return "Nein"
endif
endproc

Proc MPEGIsLameMusicCrcValid 'reads the audio data of the last file, takes time for a large file
Var tmp% = MPEGIsLameMusicCrcValidW()
if tmp% = -1
return "Ja"
else
return "Nein"
endif
endproc

Proc LoadPictoMem 'Für alle Arrays zu füllen
parameters file$
Declare mem#
Dim mem#,filesize(file$)
Blockread(file$,mem#,0,sizeof(mem#))
return mem#
endproc

Proc SavePicfromMem
Parameters file$,mem#,size&
blockwrite file$,mem#,0,size&
Dispose mem#
endproc
