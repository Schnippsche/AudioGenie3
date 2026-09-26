{**************************************************************************************************}
{                                                                                                  }
{ AudioGenie3 DLL Wrapper for Delphi                                                               }
{                                                                                                  }
{ The contents of this file are subject to the Mozilla Public License Version 1.1 (the 'License'); }
{ you may not use this file except in compliance with the License. You may obtain a copy of the    }
{ License at http://www.mozilla.org/MPL/                                                           }
{                                                                                                  }
{ Software distributed under the License is distributed on an 'AS IS' basis, WITHOUT WARRANTY OF   }
{ ANY KIND, either express or implied. See the License for the specific language governing rights  }
{ and limitations under the License.                                                               }
{                                                                                                  }
{ The Original Code is AudioGenie3.pas.                                                            }
{ The Initial Developer of the Original Code is Ulrich Decker.                                     }
{                                                                                                  }
{ Contributors:                                                                                    }
{   Ulrich Decker, http://www.udse.de                                                              }
{   Stefan Toengi                                                                                  }
{                                                                                                  }
{**************************************************************************************************}

unit AudioGenie3;

interface

{$IFDEF VER130}
  {$DEFINE D5LOWER}
{$ENDIF}
{$IFDEF VER120}
  {$DEFINE D5LOWER}
{$ENDIF}
{$IFDEF VER100}
  {$DEFINE D5LOWER}
{$ENDIF}

const
  AG_TRUE = -1;

{$IFNDEF D5LOWER}
// As of Delphi6 we can use user-assigned values for enums.
type
  TExecution = (
    OK = -1,
    failed = 0
  );

  TAudioFormatID = (
    UNKNOWN = 0,
    MPEG = 1,
    WMA = 2,
    MONKEY = 3,
    FLAC = 4,
    WAV = 5,
    OGGVORBIS = 6,
    MPEGPLUS = 7,
    AAC = 8,
    MP4M4A = 9,
    TTA = 10,
    WAVPACK = 11
  );

  TPictureType = (
    OTHER = 0,
    FILE_ICON = 1,
    OTHER_FILE_ICON = 2,
    COVER_FRONT = 3,
    COVER_BACK = 4,
    LEAFLET_PAGE = 5,
    MEDIA_LABEL = 6,
    LEAD_ARTIST = 7,
    ARTIST_PERFORMER = 8,
    CONDUCTOR = 9,
    BAND_ORCHESTRA = 10,
    COMPOSER = 11,
    LYRICIST = 12,
    RECORDING_LOCATION = 13,
    DURING_RECORDING = 14,
    DURING_PERFORMANCE = 15,
    MOVIE_VIDEO_SCREEN_CAPTURE = 16,
    A_BRIGHT_COLOURED_FISH = 17,
    ILLUSTRATION = 18,
    BAND_ARTIST_LOGOTYPE = 19,
    PUBLISHER_STUDIO_LOGOTYPE = 20
  );

  TPictureLink = (
    AS_FILENAME = -1,
    AS_PICTURE = 0
  );

  TSyncLyricContentTypes = (
    LyricInvalid = -1,
    SONSTIGES = 0,
    SONGTEXTE = 1,
    TEXTUMSCHREIBUNGEN = 2,
    BEWEGUNGEN = 3,
    EREIGNISSE = 4,
    AKKORDE = 5,
    BELANGLOSES = 6,
    URLS_ZU_WEBSEITEN = 7,
    URLS_ZU_BILDERN = 8
  );

  TSyncLyricTimeFormat = (
    TimeInvalid = -1,
    MPEGFRAMES = 1,
    MILLISEKUNDEN = 2
  );

  TConfigValues = (
    CF_MPEGEXACTREAD = 0,
    CF_ID3V2PADDINGSIZE = 1,
    CF_ID3V2WRITEBLOCKSIZE = 2,
    CF_DOEVENTSMILLIS = 3
  );

  TID3FrameID = (
     ID3F_AENC = $41454E43,
     ID3F_APIC = $41504943,
     ID3F_ASPI = $41535049,
     ID3F_CHAP = $43484150,
     ID3F_COMM = $434F4D4D,
     ID3F_COMR = $434F4D52,
     ID3F_CTOC = $43544F43,
     ID3F_ENCR = $454E4352,
     ID3F_EQU2 = $45515532,
     ID3F_EQUA = $45515541,
     ID3F_ETCO = $4554434F,
     ID3F_GEOB = $47454F42,
     ID3F_GRID = $47524944,
     ID3F_IPLS = $49504C53,
     ID3F_LINK = $4C494E4B,
     ID3F_MCDI = $4D434449,
     ID3F_MLLT = $4D4C4C54,
     ID3F_OWNE = $4F574E45,
     ID3F_PCNT = $50434E54,
     ID3F_POPM = $504F504D,
     ID3F_POSS = $504F5353,
     ID3F_PRIV = $50524956,
     ID3F_RBUF = $52425546,
     ID3F_RVA2 = $52564132,
     ID3F_RVAD = $52564144,
     ID3F_RVRB = $52565242,
     ID3F_SEEK = $5345454B,
     ID3F_SIGN = $5349474E,
     ID3F_SYLT = $53594C54,
     ID3F_SYTC = $53595443,
     ID3F_TALB = $54414C42,
     ID3F_TBPM = $5442504D,
     ID3F_TCMP = $54434D50, // iTunes
     ID3F_TCOM = $54434F4D,
     ID3F_TCON = $54434F4E,
     ID3F_TCOP = $54434F50,
     ID3F_TDAT = $54444154,
     ID3F_TDEN = $5444454E,
     ID3F_TDLY = $54444C59,
     ID3F_TDOR = $54444F52,
     ID3F_TDRC = $54445243,
     ID3F_TDRL = $5444524C,
     ID3F_TDTG = $54445447,
     ID3F_TENC = $54454E43,
     ID3F_TEXT = $54455854,
     ID3F_TFLT = $54464C54,
     ID3F_TIME = $54494D45,
     ID3F_TIPL = $5449504C,
     ID3F_TIT1 = $54495431,
     ID3F_TIT2 = $54495432,
     ID3F_TIT3 = $54495433,
     ID3F_TKEY = $544B4559,
     ID3F_TLAN = $544C414E,
     ID3F_TLEN = $544C454E,
     ID3F_TMCL = $544D434C,
     ID3F_TMED = $544D4544,
     ID3F_TMOO = $544D4F4F,
     ID3F_TOAL = $544F414C,
     ID3F_TOFN = $544F464E,
     ID3F_TOLY = $544F4C59,
     ID3F_TOPE = $544F5045,
     ID3F_TORY = $544F5259,
     ID3F_TOWN = $544F574E,
     ID3F_TPE1 = $54504531,
     ID3F_TPE2 = $54504532,
     ID3F_TPE3 = $54504533,
     ID3F_TPE4 = $54504534,
     ID3F_TPOS = $54504F53,
     ID3F_TPRO = $5450524F,
     ID3F_TPUB = $54505542,
     ID3F_TRCK = $5452434B,
     ID3F_TRDA = $54524441,
     ID3F_TRSN = $5452534E,
     ID3F_TRSO = $5452534F,
     ID3F_TSIZ = $5453495A,
     ID3F_TSO2 = $54534F32, // iTunes
     ID3F_TSOA = $54534F41, // iTunes
     ID3F_TSOC = $54534F43, // iTunes
     ID3F_TSOP = $54534F50, // iTunes
     ID3F_TSOT = $54534F54, // iTunes
     ID3F_TSRC = $54535243,
     ID3F_TSSE = $54535345,
     ID3F_TSST = $54535354,
     ID3F_TXXX = $54585858,
     ID3F_TYER = $54594552,
     ID3F_UFID = $55464944,
     ID3F_USER = $55534552,
     ID3F_USLT = $55534C54,
     ID3F_WCOM = $57434F4D,
     ID3F_WCOP = $57434F50,
     ID3F_WOAF = $574F4146,
     ID3F_WOAR = $574F4152,
     ID3F_WOAS = $574F4153,
     ID3F_WORS = $574F5253,
     ID3F_WPAY = $57504159,
     ID3F_WPUB = $57505542,
     ID3F_WXXX = $57585858
  );

  TWavFrameID = (
     WAV_IARL = $4941524C,
     WAV_IART = $49415254,
     WAV_ICMS = $49434D53,
     WAV_ICMT = $49434D54,
     WAV_ICOP = $49434F50,
     WAV_ICRD = $49435244,
     WAV_ICRP = $49435250,
     WAV_IDIM = $4944494D,
     WAV_IDPI = $49445049,
     WAV_IENG = $49454E47,
     WAV_IGNR = $49474E52,
     WAV_IKEY = $494B4559,
     WAV_ILGT = $494C4754,
     WAV_IMED = $494D4544,
     WAV_INAM = $494E414D,
     WAV_IPLT = $49504C54,
     WAV_IPRD = $49505244,
     WAV_ISBJ = $4953424A,
     WAV_ISFT = $49534654,
     WAV_ISHP = $49534850,
     WAV_ISRC = $49535243,
     WAV_ISRF = $49535246,
     WAV_ITCH = $49544348,
     WAV_ITRK = $4954524B
  );

 TWavCartChunkID = (
     WAV_CART_VERSION = 0,
     WAV_CART_TITLE = 1,
     WAV_CART_ARTIST = 2,
     WAV_CART_CUTID = 3,
     WAV_CART_CLIENTID = 4,
     WAV_CART_CATEGORY = 5,
     WAV_CART_CLASSIFICATION = 6,
     WAV_CART_OUTCUE = 7,
     WAV_CART_STARTDATE = 8,
     WAV_CART_STARTTIME = 9,
     WAV_CART_ENDDATE = 10,
     WAV_CART_ENDTIME = 11,
     WAV_CART_PRODUCERAPPID = 12,
     WAV_CART_PRODUCERAPPVERSION = 13,
     WAV_CART_USERDEF = 14,
     WAV_CART_LEVELREFERENCE = 15,
     WAV_CART_POSTTIMER0 = 16,
     WAV_CART_POSTTIMER1 = 17,
     WAV_CART_POSTTIMER2 = 18,
     WAV_CART_POSTTIMER3 = 19,
     WAV_CART_POSTTIMER4 = 20,
     WAV_CART_POSTTIMER5 = 21,
     WAV_CART_POSTTIMER6 = 22,
     WAV_CART_POSTTIMER7 = 23,
     WAV_CART_RESERVED = 24,
     WAV_CART_URL = 25,
     WAV_CART_TAGTEXT = 26
  );
      
  TMP4FrameID = (
     MP4_ALBUM = Longint($A9616C62),           // ©alb
     MP4_ARTIST = Longint($A9415254),          // ©ART
     MP4_AUTHOR = Longint($A9617574),          // ©aut
     MP4_ALBUMARTIST = Longint($61415254),     // aART
     MP4_BEATSPERMINUTE = Longint($746D706F),  // tmpo
     MP4_CATEGORY = Longint($63617467),        // catg
     MP4_CDSETNUMBER = Longint($2064736B),     //  dsk
     MP4_COMMENT = Longint($A9636D74),         // ©cmt
     MP4_COMPOSER = Longint($A9777274),        // ©wrt
     MP4_COMPILATION = Longint($6370696C),     // cpil
     MP4_DAY = Longint($A9646179),             // ©day
     MP4_COPYRIGHT = Longint($A9637079),       // ©cpy
     MP4_CREATEDATE = Longint($A9646179),      // ©day
     MP4_DESCRIPTION = Longint($A9646573),     // ©des
     MP4_INFORMATION = Longint($A9696E66),     // ©inf
     MP4_DIRECTOR = Longint($A9646972),        // ©dir
     MP4_DISCLAIMER = Longint($A9646973),      // ©dis
     MP4_ENCODER = Longint($A9746F6F),         // ©too
     MP4_GROUPING = Longint($A9677270),        // ©grp
     MP4_KEYWORD = Longint($6B657977),         // keyw
     MP4_LYRICS = Longint($A96C7972),          // ©lyr
     MP4_TITLE = Longint($A96E616D),           // ©nam
     MP4_NETURL = Longint($A975726C),          // ©url
     MP4_ORIGINALARTIST = Longint($A96F7065),  // ©ope
     MP4_ORIGINALFORMAT = Longint($A9666D74),  // ©fmt
     MP4_ORIGINALSOURCE = Longint($A9737263),  // ©src
     MP4_PERFORMER = Longint($A9707266),       // ©prf
     MP4_PRODUCER = Longint($A9707264),        // ©prd
     MP4_WARNING = Longint($A977726E)          // ©wrn
  );

{$ELSE}
const
  // AudioGenie results
  AudioGenieTrue = -1;
  AudioGenieFalse = 0;

  // Audio formats
  AUDIOFORMAT_UNKNOWN = 0;
  AUDIOFORMAT_MPEG = 1;
  AUDIOFORMAT_WMA = 2;
  AUDIOFORMAT_MONKEY = 3;
  AUDIOFORMAT_FLAC = 4;
  AUDIOFORMAT_WAV = 5;
  AUDIOFORMAT_OGGVORBIS = 6;
  AUDIOFORMAT_MPEGPLUS = 7;
  AUDIOFORMAT_AAC = 8;
  AUDIOFORMAT_MP4M4A = 9;
  AUDIOFORMAT_TTA = 10;
  AUDIOFORMAT_WAVPACK = 11;

  // Picture types
  PICTURE_OTHER = 0;
  PICTURE_FILE_ICON = 1;
  PICTURE_OTHER_FILE_ICON = 2;
  PICTURE_COVER_FRONT = 3;
  PICTURE_COVER_BACK = 4;
  PICTURE_LEAFLET_PAGE = 5;
  PICTURE_MEDIA_LABEL = 6;
  PICTURE_LEAD_ARTIST = 7;
  PICTURE_ARTIST_PERFORMER = 8;
  PICTURE_CONDUCTOR = 9;
  PICTURE_BAND_ORCHESTRA = 10;
  PICTURE_COMPOSER = 11;
  PICTURE_LYRICIST = 12;
  PICTURE_RECORDING_LOCATION = 13;
  PICTURE_DURING_RECORDING = 14;
  PICTURE_DURING_PERFORMANCE = 15;
  PICTURE_MOVIE_VIDEO_SCREEN_CAPTURE = 16;
  PICTURE_A_BRIGHT_COLOURED_FISH = 17;
  PICTURE_ILLUSTRATION = 18;
  PICTURE_BAND_ARTIST_LOGOTYPE = 19;
  PICTURE_PUBLISHER_STUDIO_LOGOTYPE = 20;

  // Picutre link types
  LINK_AS_FILENAME = -1;
  LINK_AS_PICTURE = 0;

  // Lyric content types
  LYRICCONTENT_INVALID = -1;
  LYRICCONTENT_SONSTIGES = 0;
  LYRICCONTENT_SONGTEXTE = 1;
  LYRICCONTENT_TEXTUMSCHREIBUNGEN = 2;
  LYRICCONTENT_BEWEGUNGEN = 3;
  LYRICCONTENT_EREIGNISSE = 4;
  LYRICCONTENT_AKKORDE = 5;
  LYRICCONTENT_BELANGLOSES = 6;
  LYRICCONTENT_URLS_ZU_WEBSEITEN = 7;
  LYRICCONTENT_URLS_ZU_BILDERN = 8;

  // Lyric time formats
  LYRICTIMEFORMAT_INVALID = -1;
  LYRICTIMEFORMAT_MPEGFRAMES = 1;
  LYRICTIMEFORMAT_MILLISEKUNDEN = 2;

  // Config values
  CF_MPEGEXACTREAD = 0;
  CF_ID3V2PADDINGSIZE = 1;
  CF_ID3V2WRITEBLOCKSIZE = 2;
  CF_DOEVENTSMILLIS = 3;

  // ID3 frame IDs
  ID3F_AENC = $41454E43;
  ID3F_APIC = $41504943;
  ID3F_ASPI = $41535049;
  ID3F_CHAP = $43484150;
  ID3F_COMM = $434F4D4D;
  ID3F_COMR = $434F4D52;
  ID3F_CTOC = $43544F43;
  ID3F_ENCR = $454E4352;
  ID3F_EQU2 = $45515532;
  ID3F_EQUA = $45515541;
  ID3F_ETCO = $4554434F;
  ID3F_GEOB = $47454F42;
  ID3F_GRID = $47524944;
  ID3F_IPLS = $49504C53;
  ID3F_LINK = $4C494E4B;
  ID3F_MCDI = $4D434449;
  ID3F_MLLT = $4D4C4C54;
  ID3F_OWNE = $4F574E45;
  ID3F_PCNT = $50434E54;
  ID3F_POPM = $504F504D;
  ID3F_POSS = $504F5353;
  ID3F_PRIV = $50524956;
  ID3F_RBUF = $52425546;
  ID3F_RVA2 = $52564132;
  ID3F_RVAD = $52564144;
  ID3F_RVRB = $52565242;
  ID3F_SEEK = $5345454B;
  ID3F_SIGN = $5349474E;
  ID3F_SYLT = $53594C54;
  ID3F_SYTC = $53595443;
  ID3F_TALB = $54414C42;
  ID3F_TBPM = $5442504D;
  ID3F_TCMP = $54434D50; // iTunes
  ID3F_TCOM = $54434F4D;
  ID3F_TCON = $54434F4E;
  ID3F_TCOP = $54434F50;
  ID3F_TDAT = $54444154;
  ID3F_TDEN = $5444454E;
  ID3F_TDLY = $54444C59;
  ID3F_TDOR = $54444F52;
  ID3F_TDRC = $54445243;
  ID3F_TDRL = $5444524C;
  ID3F_TDTG = $54445447;
  ID3F_TENC = $54454E43;
  ID3F_TEXT = $54455854;
  ID3F_TFLT = $54464C54;
  ID3F_TIME = $54494D45;
  ID3F_TIPL = $5449504C;
  ID3F_TIT1 = $54495431;
  ID3F_TIT2 = $54495432;
  ID3F_TIT3 = $54495433;
  ID3F_TKEY = $544B4559;
  ID3F_TLAN = $544C414E;
  ID3F_TLEN = $544C454E;
  ID3F_TMCL = $544D434C;
  ID3F_TMED = $544D4544;
  ID3F_TMOO = $544D4F4F;
  ID3F_TOAL = $544F414C;
  ID3F_TOFN = $544F464E;
  ID3F_TOLY = $544F4C59;
  ID3F_TOPE = $544F5045;
  ID3F_TORY = $544F5259;
  ID3F_TOWN = $544F574E;
  ID3F_TPE1 = $54504531;
  ID3F_TPE2 = $54504532;
  ID3F_TPE3 = $54504533;
  ID3F_TPE4 = $54504534;
  ID3F_TPOS = $54504F53;
  ID3F_TPRO = $5450524F;
  ID3F_TPUB = $54505542;
  ID3F_TRCK = $5452434B;
  ID3F_TRDA = $54524441;
  ID3F_TRSN = $5452534E;
  ID3F_TRSO = $5452534F;
  ID3F_TSIZ = $5453495A;
  ID3F_TSO2 = $54534F32; // iTunes
  ID3F_TSOA = $54534F41; // iTunes
  ID3F_TSOC = $54534F43; // iTunes
  ID3F_TSOP = $54534F50; // iTunes
  ID3F_TSOT = $54534F54; // iTunes
  ID3F_TSRC = $54535243;
  ID3F_TSSE = $54535345;
  ID3F_TSST = $54535354;
  ID3F_TXXX = $54585858;
  ID3F_TYER = $54594552;
  ID3F_UFID = $55464944;
  ID3F_USER = $55534552;
  ID3F_USLT = $55534C54;
  ID3F_WCOM = $57434F4D;
  ID3F_WCOP = $57434F50;
  ID3F_WOAF = $574F4146;
  ID3F_WOAR = $574F4152;
  ID3F_WOAS = $574F4153;
  ID3F_WORS = $574F5253;
  ID3F_WPAY = $57504159;
  ID3F_WPUB = $57505542;
  ID3F_WXXX = $57585858;

  // WAV frame IDs
  WAV_IARL = $4941524C;
  WAV_IART = $49415254;
  WAV_ICMS = $49434D53;
  WAV_ICMT = $49434D54;
  WAV_ICOP = $49434F50;
  WAV_ICRD = $49435244;
  WAV_ICRP = $49435250;
  WAV_IDIM = $4944494D;
  WAV_IDPI = $49445049;
  WAV_IENG = $49454E47;
  WAV_IGNR = $49474E52;
  WAV_IKEY = $494B4559;
  WAV_ILGT = $494C4754;
  WAV_IMED = $494D4544;
  WAV_INAM = $494E414D;
  WAV_IPLT = $49504C54;
  WAV_IPRD = $49505244;
  WAV_ISBJ = $4953424A;
  WAV_ISFT = $49534654;
  WAV_ISHP = $49534850;
  WAV_ISRC = $49535243;
  WAV_ISRF = $49535246;
  WAV_ITCH = $49544348;
  WAV_ITRK = $4954524B;

  // Wav Cart Chunk IDs
  WAV_CART_VERSION = 0;
  WAV_CART_TITLE = 1;
  WAV_CART_ARTIST = 2;
  WAV_CART_CUTID = 3;
  WAV_CART_CLIENTID = 4;
  WAV_CART_CATEGORY = 5;
  WAV_CART_CLASSIFICATION = 6;
  WAV_CART_OUTCUE = 7;
  WAV_CART_STARTDATE = 8;
  WAV_CART_STARTTIME = 9;
  WAV_CART_ENDDATE = 10;
  WAV_CART_ENDTIME = 11;
  WAV_CART_PRODUCERAPPID = 12;
  WAV_CART_PRODUCERAPPVERSION = 13;
  WAV_CART_USERDEF = 14;
  WAV_CART_LEVELREFERENCE = 15;
  WAV_CART_POSTTIMER0 = 16;
  WAV_CART_POSTTIMER1 = 17;
  WAV_CART_POSTTIMER2 = 18;
  WAV_CART_POSTTIMER3 = 19;
  WAV_CART_POSTTIMER4 = 20;
  WAV_CART_POSTTIMER5 = 21;
  WAV_CART_POSTTIMER6 = 22;
  WAV_CART_POSTTIMER7 = 23;
  WAV_CART_RESERVED = 24;
  WAV_CART_URL = 25;
  WAV_CART_TAGTEXT = 26;


  // MP4 frame IDs
  MP4_ALBUM = $A9616C62;           // ©alb
  MP4_ARTIST = $A9617274;          // ©art
  MP4_AUTHOR = $A9617574;          // ©aut
  MP4_ALBUMARTIST = $61415254;     // aART
  MP4_BEATSPERMINUTE = $746D706F;  // tmpo
  MP4_CATEGORY = $63617467;        // catg
  MP4_CDSETNUMBER = $2064736B;     //  dsk
  MP4_COMMENT = $A9636D74;         // ©cmt
  MP4_COMPOSER = $A9777274;        // ©wrt
  MP4_COMPILATION = $6370696C;     // cpil
  MP4_DAY = $A9646179;             // ©day
  MP4_COPYRIGHT = $A9637079;       // ©cpy
  MP4_CREATEDATE = $A9646179;      // ©day
  MP4_DESCRIPTION = $A9646573;     // ©des
  MP4_INFORMATION = $A9696E66;     // ©inf
  MP4_DIRECTOR = $A9646972;        // ©dir
  MP4_DISCLAIMER = $A9646973;      // ©dis
  MP4_ENCODER = $A9746F6F;         // ©too
  MP4_GROUPING = $A9677270;        // ©grp
  MP4_KEYWORD = $6B657977;         // keyw
  MP4_LYRICS = $A96C7972;          // ©lyr
  MP4_TITLE = $A96E616D;           // ©nam
  MP4_NETURL = $A975726C;          // ©url
  MP4_ORIGINALARTIST = $A96F7065;  // ©ope
  MP4_ORIGINALFORMAT = $A9666D74;  // ©fmt
  MP4_ORIGINALSOURCE = $A9737263;  // ©src
  MP4_PERFORMER = $A9707266;       // ©prf
  MP4_PRODUCER = $A9707264;        // ©prd
  MP4_WARNING = $A977726E;         // ©wrn

type
  TAudioFormatID  = Integer;
  TPictureType = ShortInt;
  TPictureLink = ShortInt;
  TSyncLyricContentTypes = ShortInt;
  TSyncLyricTimeFormat = ShortInt;
  TConfigValues = LongInt;
  TID3FrameID = LongInt;
  TWavFrameID = LongInt;
  TWavCartChunkID = ShortInt;
  TMP4FrameID = LongInt;
{$ENDIF}

const
  // WMA Frame IDs
  WM_ALBUMARTIST = 'WM/AlbumArtist';
  WM_ALBUMTITLE = 'WM/AlbumTitle';
  WM_AUTHOR = 'WM/Author';
  WM_AUTHORURL = 'WM/AuthorURL';
  WM_BEATSPERMINUTE = 'WM/BeatsPerMinute';
  WM_CATEGORY = 'WM/Category';
  WM_COMPOSER = 'WM/Composer';
  WM_CONDUCTOR = 'WM/Conductor';
  WM_CONTENTDISTRIBUTOR = 'WM/ContentDistributor';
  WM_CONTENTGROUPDESCRIPTION = 'WM/ContentGroupDescription';
  WM_DESCRIPTION = 'WM/Description';
  WM_ENCODEDBY = 'WM/EncodedBy';
  WM_ENCODINGSETTINGS = 'WM/EncodingSettings';
  WM_ENCODINGTIME = 'WM/EncodingTime';
  WM_GENRE = 'WM/Genre';
  WM_INITIALKEY = 'WM/InitialKey';
  WM_ISRC = 'WM/ISRC';
  WM_LANGUAGE = 'WM/Language';
  WM_LYRICS = 'WM/Lyrics';
  WM_MOOD = 'WM/Mood';
  WM_PARENTALRATING = 'WM/ParentalRating';
  WM_PARTOFSET = 'WM/PartOfSet';
  WM_PERIOD = 'WM/Period';
  WM_PRODUCER = 'WM/Producer';
  WM_PROMOTIONURL = 'WM/PromotionURL';
  WM_PROVIDER = 'WM/Provider';
  WM_PROVIDERCOPYRIGHT = 'WM/ProviderCopyright';
  WM_PROVIDERRATING = 'WM/ProviderRating';
  WM_PUBLISHER = 'WM/Publisher';
  WM_TITLE = 'WM/Title';
  WM_TOOLNAME = 'WM/ToolName';
  WM_TOOLVERSION = 'WM/ToolVersion';
  WM_TRACK = 'WM/Track';
  WM_TRACKNUMBER = 'WM/TrackNumber';
  WM_USERWEBURL = 'WM/UserWebURL';
  WM_WRITER = 'WM/Writer';
  WM_YEAR = 'WM/Year';



type
  TAudioGenie3 = class(TObject)
  private
    function GetAudioGenieVersionW: WideString;
    function AACGetBitRateTypeW: WideString;
    function AACGetHeaderTypeW: WideString;
    function AACGetProfileW: WideString;
    function APEGetItemKeysW: WideString;
    function APEGetSizeW: LongInt;
    function APEGetVersionW: WideString;
    function APEGetAlbumW: WideString;
    function APEGetArtistW: WideString;
    function APEGetCommentW: WideString;
    function APEGetCopyrightW: WideString;
    function APEGetGenreW: WideString;
    function APEGetTitleW: WideString;
    function APEGetTrackW: WideString;
    function APEGetUserItemW(const key: WideString): WideString;
    function APEGetYearW: WideString;
    function AUDIOGetBitrateW: LongInt;
    function AUDIOGetChannelModeW: WideString;
    function AUDIOGetChannelsW: LongInt;
    function AUDIOGetDurationMillisW: LongInt;
    function AUDIOGetDurationW: Single;
    function AUDIOGetFileSizeW: LongInt;
    function AUDIOGetLastErrorNumberW: LongInt;
    function AUDIOGetLastErrorTextW: WideString;
    function AUDIOGetLastFileW: WideString;
    function AUDIOGetMD5ValueW: WideString;
    function AUDIOGetSampleRateW: LongInt;
    function AUDIOGetVersionW: WideString;
    function AUDIOGetAlbumW: WideString;
    function AUDIOGetArtistW: WideString;
    function AUDIOGetCommentW: WideString;
    function AUDIOGetGenreW: WideString;
    function AUDIOGetTitleW: WideString;
    function AUDIOGetTrackW: WideString;
    function AUDIOGetYearW: WideString;
    function AUDIOGetComposerW: WideString;
    function GetConfigValueW(const key: TConfigValues): LongInt;
    function FLACGetAlbumW: WideString;
    function FLACGetArrangerW: WideString;
    function FLACGetArtistW: WideString;
    function FLACGetAuthorW: WideString;
    function FLACGetBitsPerSampleW: LongInt;
    function FLACGetCompressionRatioW: Single;
    function FLACGetCommentW: WideString;
    function FLACGetComposerW: WideString;
    function FLACGetContactW: WideString;
    function FLACGetCopyrightW: WideString;
    function FLACGetDateW: WideString;
    function FLACGetDescriptionW: WideString;
    function FLACGetDiscNumberW: WideString;
    function FLACGetEnsembleW: WideString;
    function FLACGetGenreW: WideString;
    function FLACGetISRCW: WideString;
    function FLACGetItemKeysW: WideString;
    function FLACGetLicenseW: WideString;
    function FLACGetLocationW: WideString;
    function FLACGetLyricistW: WideString;
    function FLACGetMaxBlockSizeW: LongInt;
    function FLACGetMaxFrameSizeW: LongInt;
    function FLACGetMinBlockSizeW: LongInt;
    function FLACGetMinFrameSizeW: LongInt;
    function FLACGetOrganizationW: WideString;
    function FLACGetPerformerW: WideString;
    function FLACGetPublisherW: WideString;
    function FLACGetSamplesW: LongInt;
    function FLACGetTitleW: WideString;
    function FLACGetTrackW: WideString;
    function FLACGetUserItemW(const ItemKey: WideString): WideString;
    function FLACGetVersionW: WideString;
    function FLACGetVendorW: WideString;
    function ID3V1GetVersionW: WideString;
    function ID3V1GetGenresW: SmallInt;
    function ID3V1GetAlbumW: WideString;
    function ID3V1GetArtistW: WideString;
    function ID3V1GetCommentW: WideString;
    function ID3V1GetGenreIDW: SmallInt;
    function ID3V1GetGenreW: WideString; 
    function ID3V1GetTitleW: WideString; 
    function ID3V1GetTrackW: WideString; 
    function ID3V1GetYearW: WideString;
    function ID3V2GetGenreW: WideString; 
    function ID3V2GetPlayCounterW: LongInt; 
    function ID3V2GetTextFrameW(const FrameID: TID3FrameID): WideString; 
    function ID3V2GetURLFrameW(const FrameID: TID3FrameID): WideString;
    function LYRICSGetAlbumW: WideString;
    function LYRICSGetArtistW: WideString; 
    function LYRICSGetAuthorW: WideString;
    function LYRICSGetGenreW: WideString;
    function LYRICSGetImageLinkW: WideString; 
    function LYRICSGetIndicationW: WideString;
    function LYRICSGetInformationW: WideString;
    function LYRICSGetLyricsW: WideString;
    function LYRICSGetTitleW: WideString;
    function MONKEYGetBitsW: SmallInt;
    function MONKEYGetCompressionRatioW: Single;
    function MONKEYGetCompressionW: WideString;
    function MONKEYGetFramesW: LongInt;
    function MONKEYGetPeakW: Single;
    function MONKEYGetSamplesPerFrameW: LongInt;
    function MONKEYGetSamplesW: LongInt;
    function MP4GetGenreW: WideString;
    function MP4GetTextFrameW(const FrameID: TMP4FrameID): WideString;
    function MP4GetTrackW: WideString;
    function MPEGGetEmphasisW: WideString;
    function MPEGGetEncoderW: WideString;
    function MPEGGetFramePositionW: LongInt;
    function MPEGGetFrameSizeW: LongInt;
    function MPEGGetFramesW: LongInt;
    function MPEGGetLayerW: WideString;
    function MPEGGetVersionW: WideString;
    function MPPGetFramesW: LongInt;
    function MPPGetStreamVersionW: SmallInt;
    function OGGGetAlbumW: WideString; 
    function OGGGetArtistW: WideString; 
    function OGGGetCommentW: WideString; 
    function OGGGetContactW: WideString; 
    function OGGGetCopyrightW: WideString;
    function OGGGetDateW: WideString;
    function OGGGetDescriptionW: WideString;
    function OGGGetGenreW: WideString; 
    function OGGGetISRCW: WideString; 
    function OGGGetLicenseW: WideString;
    function OGGGetLocationW: WideString;
    function OGGGetOrganizationW: WideString;
    function OGGGetPerformerW: WideString; 
    function OGGGetTitleW: WideString;
    function OGGGetTrackW: WideString; 
    function OGGGetUserItemW(const ItemKey: WideString): WideString;
    function OGGGetVendorW: WideString;
    function OGGGetVersionW: WideString;
    function WAVGetCartChunkEntryW(const Index: TWavCartChunkID): WideString;
    function WAVGetTextFrameW(const FrameID: TWAVFrameID): WideString;
    function WAVGetBitsPerSampleW: LongInt; 
    function WAVGetBlockAlignW: LongInt;
    function WAVGetBytesPerSecondW: LongInt; 
    function WAVGetChannelsW: LongInt; 
    function WAVGetFormatIDW: SmallInt;
    function WAVGetFormatW: WideString; 
    function WAVGetHeaderSizeW: LongInt;
    function WAVGetInfoChunkIDsW: WideString;
    function WMAGetAlbumArtistW: WideString; 
    function WMAGetAlbumW: WideString; 
    function WMAGetArtistW: WideString;
    function WMAGetAuthorURLW: WideString;
    function WMAGetBeatsPerMinuteW: WideString;
    function WMAGetCategoryW: WideString; 
    function WMAGetCommentW: WideString;
    function WMAGetComposerW: WideString;
    function WMAGetConductorW: WideString; 
    function WMAGetContentGroupDescriptionW: WideString;
    function WMAGetCopyrightW: WideString; 
    function WMAGetDistributorW: WideString;
    function WMAGetEncodedByW: WideString;
    function WMAGetEncodingSettingsW: WideString;
    function WMAGetEncodingTimeW: WideString; 
    function WMAGetGenreW: WideString;
    function WMAGetInitialKeyW: WideString; 
    function WMAGetISRCW: WideString; 
    function WMAGetLanguageW: WideString;
    function WMAGetLyricsW: WideString;
    function WMAGetMoodW: WideString; 
    function WMAGetParentalRatingW: WideString; 
    function WMAGetPartOfSetW: WideString;
    function WMAGetPeriodW: WideString; 
    function WMAGetProducerW: WideString; 
    function WMAGetPromotionURLW: WideString; 
    function WMAGetProviderW: WideString; 
    function WMAGetPublisherW: WideString;
    function WMAGetRatingW: WideString; 
    function WMAGetTitleW: WideString; 
    function WMAGetToolNameW: WideString;
    function WMAGetToolVersionW: WideString;
    function WMAGetTrackNumberW: WideString;
    function WMAGetUserItemW(const ItemKey: WideString): WideString;
    function WMAGetUserWebURLW: WideString;
    function WMAGetWriterW: WideString;
    function WMAGetYearW: WideString;
    procedure APESetAlbumW(const textString: WideString);
    procedure APESetArtistW(const textString: WideString);
    procedure APESetCommentW(const textString: WideString);
    procedure APESetCopyrightW(const textString: WideString);
    procedure APESetGenreW(const textString: WideString);
    procedure APESetTitleW(const textString: WideString);
    procedure APESetTrackW(const textString: WideString);
    procedure APESetUserItemW(const key: WideString; const textString: WideString);
    procedure APESetYearW(const textString: WideString);
    procedure AUDIOSetAlbumW(const textString: WideString);
    procedure AUDIOSetArtistW(const textString: WideString);
    procedure AUDIOSetCommentW(const textString: WideString);
    procedure AUDIOSetGenreW(const textString: WideString);
    procedure AUDIOSetTitleW(const textString: WideString);
    procedure AUDIOSetTrackW(const textString: WideString);
    procedure AUDIOSetYearW(const textString: WideString);
    procedure AUDIOSetComposerW(const textString: WideString);
    procedure SetConfigValueW(const key: TConfigValues; const value: LongInt);
    procedure FLACSetAlbumW(const textString: WideString);
    procedure FLACSetArrangerW(const textString: WideString);
    procedure FLACSetArtistW(const textString: WideString);
    procedure FLACSetAuthorW(const textString: WideString);
    procedure FLACSetCommentW(const textString: WideString);
    procedure FLACSetComposerW(const textString: WideString);
    procedure FLACSetContactW(const textString: WideString);
    procedure FLACSetCopyrightW(const textString: WideString);
    procedure FLACSetDateW(const textString: WideString);
    procedure FLACSetDescriptionW(const textString: WideString);
    procedure FLACSetDiscNumberW(const textString: WideString);
    procedure FLACSetEnsembleW(const textString: WideString);
    procedure FLACSetGenreW(const textString: WideString);
    procedure FLACSetISRCW(const textString: WideString);
    procedure FLACSetLicenseW(const textString: WideString);
    procedure FLACSetLocationW(const textString: WideString);
    procedure FLACSetLyricistW(const textString: WideString);
    procedure FLACSetOrganizationW(const textString: WideString);
    procedure FLACSetPerformerW(const textString: WideString);
    procedure FLACSetPublisherW(const textString: WideString);
    procedure FLACSetTitleW(const textString: WideString);
    procedure FLACSetTrackW(const textString: WideString);
    procedure FLACSetUserItemW(const ItemKey: WideString; const textString: WideString);
    procedure FLACSetVendorW(const textString: WideString);
    procedure FLACSetVersionW(const textString: WideString);
    procedure ID3V1SetAlbumW(const textString: WideString);
    procedure ID3V1SetArtistW(const textString: WideString);
    procedure ID3V1SetCommentW(const textString: WideString);
    procedure ID3V1SetGenreIDW(const nNewValue: SmallInt);
    procedure ID3V1SetGenreW(const textString: WideString);
    procedure ID3V1SetTitleW(const textString: WideString);
    procedure ID3V1SetTrackW(const textString: WideString);
    procedure ID3V1SetYearW(const textString: WideString);
    procedure ID3V2SetGenreW(const textString: WideString);
    procedure ID3V2SetPlayCounterW(const Counter: LongInt);
    procedure ID3V2SetTextFrameW(const FrameID: TID3FrameID; const textString: WideString);
    procedure ID3V2SetURLFrameW(const FrameID: TID3FrameID; const textString: WideString);
    procedure LYRICSSetAlbumW(const textString: WideString);
    procedure LYRICSSetArtistW(const textString: WideString);
    procedure LYRICSSetAuthorW(const textString: WideString);
    procedure LYRICSSetGenreW(const textString: WideString);
    procedure LYRICSSetImageLinkW(const textString: WideString);
    procedure LYRICSSetIndicationW(const textString: WideString);
    procedure LYRICSSetInformationW(const textString: WideString);
    procedure LYRICSSetLyricsW(const textString: WideString);
    procedure LYRICSSetTitleW(const textString: WideString);
    procedure MP4SetGenreW(const textString: WideString);
    procedure MP4SetTextFrameW(const FrameID: TMP4FrameID; const newText: WideString);
    procedure MP4SetTrackW(const textString: WideString);
    procedure OGGSetAlbumW(const textString: WideString);
    procedure OGGSetArtistW(const textString: WideString);
    procedure OGGSetCommentW(const textString: WideString);
    procedure OGGSetContactW(const textString: WideString);
    procedure OGGSetCopyrightW(const textString: WideString);
    procedure OGGSetDateW(const textString: WideString);
    procedure OGGSetDescriptionW(const textString: WideString);
    procedure OGGSetGenreW(const textString: WideString);
    procedure OGGSetISRCW(const textString: WideString);
    procedure OGGSetLicenseW(const textString: WideString);
    procedure OGGSetLocationW(const textString: WideString);
    procedure OGGSetOrganizationW(const textString: WideString);
    procedure OGGSetPerformerW(const textString: WideString);
    procedure OGGSetTitleW(const textString: WideString);
    procedure OGGSetTrackW(const textString: WideString);
    procedure OGGSetUserItemW(const ItemKey: WideString; const textString: WideString);
    procedure OGGSetVendorW(const textString: WideString);
    procedure OGGSetVersionW(const textString: WideString);
    procedure WAVSetTextFrameW(const FrameID: TWAVFrameID; const textString: WideString);
    procedure WAVSetCartChunkEntryW(const Index: TWavCartChunkID; const textString: WideString);
    procedure WMASetAlbumArtistW(const textString: WideString); 
    procedure WMASetAlbumW(const textString: WideString);
    procedure WMASetArtistW(const textString: WideString);
    procedure WMASetAuthorURLW(const textString: WideString); 
    procedure WMASetBeatsPerMinuteW(const textString: WideString); 
    procedure WMASetCategoryW(const textString: WideString);
    procedure WMASetCommentW(const textString: WideString);
    procedure WMASetComposerW(const textString: WideString);
    procedure WMASetConductorW(const textString: WideString);
    procedure WMASetContentGroupDescriptionW(const textString: WideString); 
    procedure WMASetCopyrightW(const textString: WideString);
    procedure WMASetDistributorW(const textString: WideString);
    procedure WMASetEncodedByW(const textString: WideString);
    procedure WMASetEncodingSettingsW(const textString: WideString);
    procedure WMASetEncodingTimeW(const textString: WideString);
    procedure WMASetGenreW(const textString: WideString);
    procedure WMASetInitialKeyW(const textString: WideString);
    procedure WMASetISRCW(const textString: WideString);
    procedure WMASetLanguageW(const textString: WideString);
    procedure WMASetLyricsW(const textString: WideString);
    procedure WMASetMoodW(const textString: WideString);
    procedure WMASetParentalRatingW(const textString: WideString);
    procedure WMASetPartOfSetW(const textString: WideString);
    procedure WMASetPeriodW(const textString: WideString);
    procedure WMASetProducerW(const textString: WideString);
    procedure WMASetPromotionURLW(const textString: WideString);
    procedure WMASetProviderW(const textString: WideString);
    procedure WMASetPublisherW(const textString: WideString);
    procedure WMASetRatingW(const textString: WideString);
    procedure WMASetTitleW(const textString: WideString);
    procedure WMASetToolNameW(const textString: WideString);
    procedure WMASetToolVersionW(const textString: WideString);
    procedure WMASetTrackNumberW(const textString: WideString);
    procedure WMASetUserItemW(const ItemKey: WideString; const textString: WideString);
    procedure WMASetUserWebURLW(const textString: WideString);
    procedure WMASetWriterW(const textString: WideString);
    procedure WMASetYearW(const textString: WideString) ;
  public
    property AudioGenieVersionW: WideString read GetAudioGenieVersionW;

    // AAC
    property AACBitRateTypeW: WideString read AACGetBitRateTypeW;
    property AACHeaderTypeW: WideString read AACGetHeaderTypeW;
    property AACProfileW: WideString read AACGetProfileW;

    // APE
    property APEAlbumW: WideString read APEGetAlbumW write APESetAlbumW;
    property APEArtistW: WideString read APEGetArtistW write APESetArtistW;
    property APECommentW: WideString read APEGetCommentW write APESetCommentW;
    property APECopyrightW: WideString read APEGetCopyrightW write APESetCopyrightW;
    property APEGenreW: WideString read APEGetGenreW write APESetGenreW;
    property APEItemKeysW: WideString read APEGetItemKeysW;
    property APESizeW: LongInt read APEGetSizeW;
    property APETitleW: WideString read APEGetTitleW write APESetTitleW;
    property APETrackW: WideString read APEGetTrackW write APESetTrackW;
    property APEUserItemW[const key: WideString]: WideString read APEGetUserItemW write APESetUserItemW;
    property APEVersionW: WideString read APEGetVersionW;
    property APEYearW: WideString read APEGetYearW write APESetYearW;

    // Audio
    property AUDIOAlbumW: WideString read AUDIOGetAlbumW write AUDIOSetAlbumW;
    property AUDIOArtistW: WideString read AUDIOGetArtistW write AUDIOSetArtistW;
    property AUDIOBitrateW: LongInt read AUDIOGetBitrateW;
    property AUDIOChannelModeW: WideString read AUDIOGetChannelModeW;
    property AUDIOChannelsW: LongInt read AUDIOGetChannelsW;
    property AUDIOCommentW: WideString read AUDIOGetCommentW write AUDIOSetCommentW;
    property AUDIODurationMillisW: LongInt read AUDIOGetDurationMillisW;
    property AUDIODurationW: Single read AUDIOGetDurationW;
    property AUDIOFileSizeW: LongInt read AUDIOGetFileSizeW;
    property AUDIOGenreW: WideString read AUDIOGetGenreW write AUDIOSetGenreW;
    property AUDIOLastErrorNumberW: LongInt read AUDIOGetLastErrorNumberW;
    property AUDIOLastErrorTextW: WideString read AUDIOGetLastErrorTextW;
    property AUDIOLastFileW: WideString read AUDIOGetLastFileW;
    property AUDIOMD5ValueW: WideString read AUDIOGetMD5ValueW;
    property AUDIOSampleRateW: LongInt read AUDIOGetSampleRateW;
    property AUDIOTitleW: WideString read AUDIOGetTitleW write AUDIOSetTitleW;
    property AUDIOTrackW: WideString read AUDIOGetTrackW write AUDIOSetTrackW;
    property AUDIOVersionW: WideString read AUDIOGetVersionW;
    property AUDIOYearW: WideString read AUDIOGetYearW write AUDIOSetYearW;
    property AUDIOComposerW: WideString read AUDIOGetComposerW write AUDIOSetComposerW;

    // Config
    property ConfigValueW[const key: TConfigValues]: LongInt read GetConfigValueW write SetConfigValueW;

    // FLAC
    property FLACAlbumW: WideString read FLACGetAlbumW write FLACSetAlbumW;
    property FLACArrangerW: WideString read FLACGetArrangerW write FLACSetArrangerW;
    property FLACArtistW: WideString read FLACGetArtistW write FLACSetArtistW;
    property FLACAuthorW: WideString read FLACGetAuthorW write FLACSetAuthorW;
    property FLACBitsPerSampleW: LongInt read FLACGetBitsPerSampleW;
    property FLACCommentW: WideString read FLACGetCommentW write FLACSetCommentW;
    property FLACComposerW: WideString read FLACGetComposerW write FLACSetComposerW;
    property FLACCompressionRatioW: Single read FLACGetCompressionRatioW;
    property FLACContactW: WideString read FLACGetContactW write FLACSetContactW;
    property FLACCopyrightW: WideString read FLACGetCopyrightW write FLACSetCopyrightW;
    property FLACDateW: WideString read FLACGetDateW write FLACSetDateW;
    property FLACDescriptionW: WideString read FLACGetDescriptionW write FLACSetDescriptionW;
    property FLACDiscNumberW: WideString read FLACGetDiscNumberW write FLACSetDiscNumberW;
    property FLACEnsembleW: WideString read FLACGetEnsembleW write FLACSetEnsembleW;
    property FLACGenreW: WideString read FLACGetGenreW write FLACSetGenreW;
    property FLACISRCW: WideString read FLACGetISRCW write FLACSetISRCW;
    property FLACItemKeysW: WideString read FLACGetItemKeysW;
    property FLACLicenseW: WideString read FLACGetLicenseW write FLACSetLicenseW;
    property FLACLocationW: WideString read FLACGetLocationW write FLACSetLocationW;
    property FLACLyricistW: WideString read FLACGetLyricistW write FLACSetLyricistW;
    property FLACMaxBlockSizeW: LongInt read FLACGetMaxBlockSizeW;
    property FLACMaxFrameSizeW: LongInt read FLACGetMaxFrameSizeW;
    property FLACMinBlockSizeW: LongInt read FLACGetMinBlockSizeW;
    property FLACMinFrameSizeW: LongInt read FLACGetMinFrameSizeW;
    property FLACOrganizationW: WideString read FLACGetOrganizationW write FLACSetOrganizationW;
    property FLACPerformerW: WideString read FLACGetPerformerW write FLACSetPerformerW;
    property FLACPublisherW: WideString read FLACGetPublisherW write FLACSetPublisherW;
    property FLACSamplesW: LongInt read FLACGetSamplesW;
    property FLACTitleW: WideString read FLACGetTitleW write FLACSetTitleW;
    property FLACTrackW: WideString read FLACGetTrackW write FLACSetTrackW;
    property FLACUserItemW[const ItemKey: WideString]: WideString read FLACGetUserItemW write FLACSetUserItemW;
    property FLACVendorW: WideString read FLACGetVendorW write FLACSetVendorW;
    property FLACVersionW: WideString read FLACGetVersionW write FLACSetVersionW;

    // ID3V1
    property ID3V1AlbumW: WideString read ID3V1GetAlbumW write ID3V1SetAlbumW;
    property ID3V1ArtistW: WideString read ID3V1GetArtistW write ID3V1SetArtistW;
    property ID3V1CommentW: WideString read ID3V1GetCommentW write ID3V1SetCommentW;
    property ID3V1GenreIDW: SmallInt read ID3V1GetGenreIDW write ID3V1SetGenreIDW;
    property ID3V1GenreW: WideString read ID3V1GetGenreW write ID3V1SetGenreW;
    property ID3V1GenresW: SmallInt read ID3V1GetGenresW;
    property ID3V1TitleW: WideString read ID3V1GetTitleW write ID3V1SetTitleW;
    property ID3V1TrackW: WideString read ID3V1GetTrackW write ID3V1SetTrackW;
    property ID3V1VersionW: WideString read ID3V1GetVersionW;
    property ID3V1YearW: WideString read ID3V1GetYearW write ID3V1SetYearW;

    // ID3V2
    property ID3V2GenreW: WideString read ID3V2GetGenreW write ID3V2SetGenreW;
    property ID3V2PlayCounterW: LongInt read ID3V2GetPlayCounterW write ID3V2SetPlayCounterW;
    property ID3V2TextFrameW[const FrameID: TID3FrameID]: WideString read ID3V2GetTextFrameW write ID3V2SetTextFrameW;
    property ID3V2URLFrameW[const FrameID: TID3FrameID]: WideString read ID3V2GetURLFrameW write ID3V2SetURLFrameW;

    // LYRICS
    property LYRICSAlbumW: WideString read LYRICSGetAlbumW write LYRICSSetAlbumW;
    property LYRICSArtistW: WideString read LYRICSGetArtistW write LYRICSSetArtistW;
    property LYRICSAuthorW: WideString read LYRICSGetAuthorW write LYRICSSetAuthorW;
    property LYRICSGenreW: WideString read LYRICSGetGenreW write LYRICSSetGenreW;
    property LYRICSImageLinkW: WideString read LYRICSGetImageLinkW write LYRICSSetImageLinkW;
    property LYRICSIndicationW: WideString read LYRICSGetIndicationW write LYRICSSetIndicationW;
    property LYRICSInformationW: WideString read LYRICSGetInformationW write LYRICSSetInformationW;
    property LYRICSLyricsW: WideString read LYRICSGetLyricsW write LYRICSSetLyricsW;
    property LYRICSTitleW: WideString read LYRICSGetTitleW write LYRICSSetTitleW;

    // MONKEY
    property MONKEYBitsW: SmallInt read MONKEYGetBitsW;
    property MONKEYCompressionRatioW: Single read MONKEYGetCompressionRatioW;
    property MONKEYCompressionW: WideString read MONKEYGetCompressionW;
    property MONKEYFramesW: LongInt read MONKEYGetFramesW;
    property MONKEYPeakW: Single read MONKEYGetPeakW;
    property MONKEYSamplesPerFrameW: LongInt read MONKEYGetSamplesPerFrameW;
    property MONKEYSamplesW: LongInt read MONKEYGetSamplesW;

    // MP4
    property MP4GenreW: WideString read MP4GetGenreW write MP4SetGenreW;
    property MP4TextFrameW[const FrameID: TMP4FrameID]: WideString read MP4GetTextFrameW write MP4SetTextFrameW;
    property MP4TrackW: WideString read MP4GetTrackW write MP4SetTrackW;

    // MPEG
    property MPEGEmphasisW: WideString read MPEGGetEmphasisW;
    property MPEGEncoderW: WideString read MPEGGetEncoderW;
    property MPEGFramePositionW: LongInt read MPEGGetFramePositionW;
    property MPEGFrameSizeW: LongInt read MPEGGetFrameSizeW;
    property MPEGFramesW: LongInt read MPEGGetFramesW;
    property MPEGLayerW: WideString read MPEGGetLayerW;
    property MPEGVersionW: WideString read MPEGGetVersionW;

    // MPP
    property MPPFramesW: LongInt read MPPGetFramesW;
    property MPPStreamVersionW: SmallInt read MPPGetStreamVersionW;

    // OGG
    property OGGAlbumW: WideString read OGGGetAlbumW write OGGSetAlbumW;
    property OGGArtistW: WideString read OGGGetArtistW write OGGSetArtistW;
    property OGGCommentW: WideString read OGGGetCommentW write OGGSetCommentW;
    property OGGContactW: WideString read OGGGetContactW write OGGSetContactW;
    property OGGCopyrightW: WideString read OGGGetCopyrightW write OGGSetCopyrightW;
    property OGGDateW: WideString read OGGGetDateW write OGGSetDateW;
    property OGGDescriptionW: WideString read OGGGetDescriptionW write OGGSetDescriptionW;
    property OGGGenreW: WideString read OGGGetGenreW write OGGSetGenreW;
    property OGGISRCW: WideString read OGGGetISRCW write OGGSetISRCW;
    property OGGLicenseW: WideString read OGGGetLicenseW write OGGSetLicenseW;
    property OGGLocationW: WideString read OGGGetLocationW write OGGSetLocationW;
    property OGGOrganizationW: WideString read OGGGetOrganizationW write OGGSetOrganizationW;
    property OGGPerformerW: WideString read OGGGetPerformerW write OGGSetPerformerW;
    property OGGTitleW: WideString read OGGGetTitleW write OGGSetTitleW;
    property OGGTrackW: WideString read OGGGetTrackW write OGGSetTrackW;
    property OGGUserItemW[const ItemKey: WideString]: WideString read OGGGetUserItemW write OGGSetUserItemW;
    property OGGVendorW: WideString read OGGGetVendorW write OGGSetVendorW;
    property OGGVersionW: WideString read OGGGetVersionW write OGGSetVersionW;

    // WAV
    property WAVBitsPerSampleW: LongInt read WAVGetBitsPerSampleW;
    property WAVBlockAlignW: LongInt read WAVGetBlockAlignW;
    property WAVBytesPerSecondW: LongInt read WAVGetBytesPerSecondW;
    property WAVChannelsW: LongInt read WAVGetChannelsW;
    property WAVFormatIDW: SmallInt read WAVGetFormatIDW;
    property WAVFormatW: WideString read WAVGetFormatW;
    property WAVHeaderSizeW: LongInt read WAVGetHeaderSizeW;
    property WAVTextFrameW[const FrameID: TWAVFrameID]: WideString read WAVGetTextFrameW write WAVSetTextFrameW;
    property WAVCartChunkEntryW[const Index: TWavCartChunkID]: WideString read WAVGetCartChunkEntryW write WAVSetCartChunkEntryW;
    // WMA
    property WMAAlbumArtistW: WideString read WMAGetAlbumArtistW write WMASetAlbumArtistW;
    property WMAAlbumW: WideString read WMAGetAlbumW write WMASetAlbumW;
    property WMAArtistW: WideString read WMAGetArtistW write WMASetArtistW;
    property WMAAuthorURLW: WideString read WMAGetAuthorURLW write WMASetAuthorURLW;
    property WMABeatsPerMinuteW: WideString read WMAGetBeatsPerMinuteW write WMASetBeatsPerMinuteW;
    property WMACategoryW: WideString read WMAGetCategoryW write WMASetCategoryW;
    property WMACommentW: WideString read WMAGetCommentW write WMASetCommentW;
    property WMAComposerW: WideString read WMAGetComposerW write WMASetComposerW;
    property WMAConductorW: WideString read WMAGetConductorW write WMASetConductorW;
    property WMAContentGroupDescriptionW: WideString read WMAGetContentGroupDescriptionW write WMASetContentGroupDescriptionW;
    property WMACopyrightW: WideString read WMAGetCopyrightW write WMASetCopyrightW;
    property WMADistributorW: WideString read WMAGetDistributorW write WMASetDistributorW;
    property WMAEncodedByW: WideString read WMAGetEncodedByW write WMASetEncodedByW;
    property WMAEncodingSettingsW: WideString read WMAGetEncodingSettingsW write WMASetEncodingSettingsW;
    property WMAEncodingTimeW: WideString read WMAGetEncodingTimeW write WMASetEncodingTimeW;
    property WMAGenreW: WideString read WMAGetGenreW write WMASetGenreW;
    property WMAInitialKeyW: WideString read WMAGetInitialKeyW write WMASetInitialKeyW;
    property WMAISRCW: WideString read WMAGetISRCW write WMASetISRCW;
    property WMALanguageW: WideString read WMAGetLanguageW write WMASetLanguageW;
    property WMALyricsW: WideString read WMAGetLyricsW write WMASetLyricsW;
    property WMAMoodW: WideString read WMAGetMoodW write WMASetMoodW;
    property WMAParentalRatingW: WideString read WMAGetParentalRatingW write WMASetParentalRatingW;
    property WMAPartOfSetW: WideString read WMAGetPartOfSetW write WMASetPartOfSetW;
    property WMAPeriodW: WideString read WMAGetPeriodW write WMASetPeriodW;
    property WMAProducerW: WideString read WMAGetProducerW write WMASetProducerW;
    property WMAPromotionURLW: WideString read WMAGetPromotionURLW write WMASetPromotionURLW;
    property WMAProviderW: WideString read WMAGetProviderW write WMASetProviderW;
    property WMAPublisherW: WideString read WMAGetPublisherW write WMASetPublisherW;
    property WMARatingW: WideString read WMAGetRatingW write WMASetRatingW;
    property WMATitleW: WideString read WMAGetTitleW write WMASetTitleW;
    property WMAToolNameW: WideString read WMAGetToolNameW write WMASetToolNameW;
    property WMAToolVersionW: WideString read WMAGetToolVersionW write WMASetToolVersionW;
    property WMATrackNumberW: WideString read WMAGetTrackNumberW write WMASetTrackNumberW;
    property WMAUserItemW[const ItemKey: WideString]: WideString read WMAGetUserItemW write WMASetUserItemW;
    property WMAUserWebURLW: WideString read WMAGetUserWebURLW write WMASetUserWebURLW;
    property WMAWriterW: WideString read WMAGetWriterW write WMASetWriterW;
    property WMAYearW: WideString read WMAGetYearW write WMASetYearW;      

////////////////////////////////////////////////////////////////////

    // APE
    function APEExistsW: boolean;
    function APERemoveTagFromFileW(const FileName: WideString): Boolean;
    function APERemoveTagW: Boolean;
    function APESaveChangesToFileW(const FileName: WideString): Boolean;
    function APESaveChangesW: Boolean;

    // AUDIO
    function AUDIOAnalyzeFileW(const FileName: WideString): TAudioFormatID;
    function AUDIOIsValidFormatW: Boolean;
    function AUDIOSaveChangesToFileW(const FileName: WideString): Boolean;
    function AUDIOSaveChangesW: Boolean;

    // FLAC
    function FLACSaveChangesToFileW(const FileName: WideString): Boolean;
    function FLACSaveChangesW: Boolean;

    // ID3V1
    function ID3V1ExistsW: Boolean;
    function ID3V1RemoveTagFromFileW(const FileName: WideString): Boolean;
    function ID3V1RemoveTagW: Boolean;
    function ID3V1SaveChangesToFileW(const FileName: WideString): Boolean;
    function ID3V1SaveChangesW: Boolean;
    function ID3V1GetGenreItemW(const number: SmallInt): WideString;

    // ID3V2
    function ID3V2AddAudioEncryptionW(const arr: Pointer; const maxLen: LongInt; const URL: WideString; const PreviewStart: SmallInt; const PreviewLength: SmallInt): Boolean;
    function ID3V2AddAudioSeekPointW(const arr: Pointer; const maxLen: LongInt; const start: LongInt; const Length: LongInt; const numbers: SmallInt; const  BitsPerPoint: Byte): Boolean;
    function ID3V2AddChapterW(const ID: WideString; const Title: WideString; const Description: WideString; const startTime: LongInt; const endTime: LongInt): Boolean;
    function ID3V2AddChildElementW(const ParentTocID: WideString; const ChildID: WideString): Boolean;
    function ID3V2AddCommentW(const Language: WideString; const Description: WideString; const text: WideString): Boolean;
    function ID3V2AddCommercialFrameW(const arr: Pointer; const maxLen: LongInt; const Price: PWidechar; const validUntil: WideString; const contactUrl: WideString; const receivedAs: SmallInt; const Seller: WideString; const Description: WideString): Boolean;
    function ID3V2AddEncryptionW(const arr: Pointer; const maxLen: LongInt; const URL: WideString; const symbol: SmallInt): Boolean;
    function ID3V2GetAddendumDescriptionW(const ID: WideString): WideString;
    function ID3V2GetAddendumTitleW(const ID: WideString): WideString;
    function ID3V2GetAddendumTypeW(const ID: WideString): SmallInt;
    function ID3V2AddEqualisationW(const arr: Pointer; const maxLen: LongInt; const Interpolationmethod: Byte; const Identification: WideString): Boolean;
    function ID3V2AddEventTimingCodesW(const arr: Pointer; const maxLen: LongInt; const TimestampFormat: Byte): Boolean;
    function ID3V2AddGeneralObjectW(const arr: Pointer; const maxLen: LongInt; const Mime: WideString; const FileName: WideString; const Description: WideString): Boolean;
    function ID3V2AddGroupIdentificationW(const arr: Pointer; const Length: LongInt; const URL: WideString; const symbol: Byte): Boolean;
    function ID3V2AddLinkedInformationW(const FrameIdentifier: LongInt; const URL: WideString; const additionalData: WideString): Boolean;
    function ID3V2AddLyricW(const Language: WideString; const Description: WideString; const text: WideString): Boolean;
    function ID3V2AddMpegLocationLookupTableW(const arr: Pointer; const Length: LongInt; const Frames: LongInt; const Bytes: LongInt; const Milliseconds: LongInt; const BytesDeviation: Byte; const MillisecondsDeviation: Byte): Boolean;
    function ID3V2AddMusicCdIdentifierW(const arr: Pointer; const Length: LongInt): Boolean;
    function ID3V2AddOwnershipW(constPrice: WideString; const dateString: WideString; const Seller: WideString): Boolean;
    function ID3V2AddPictureArrayW(const arr: Pointer; const maxLen: LongInt; const Description: WideString; const PictureType: TPictureType): Boolean;
    function ID3V2AddPictureFileW(const FileName: WideString; const Description: WideString; const PictureType: TPictureType; const asLink: TPictureLink): Boolean;
    function ID3V2AddPopularimeterW(const Email: WideString; const Rating: SmallInt; const Counter: LongInt): Boolean;
    function ID3V2AddPositionSynchronisationW(const TimestampFormat: Byte; const Position: LongInt): Boolean;
    function ID3V2AddPrivateFrameW(const arr: Pointer; const Length: LongInt; const URL: WideString): Boolean;
    function ID3V2AddRecommendedBufferSizeW(const BufferSize: LongInt; const EmbeddedInfoFlag: Byte; const offset: LongInt): Boolean;
    function ID3V2AddRelativeVolumeAdjustmentW(const arr: Pointer; const Length: LongInt; const Identifier: WideString): Boolean;
    function ID3V2AddReverbW(const reverbLeft: SmallInt; const reverbRight: SmallInt; const bouncesLeft: Byte; const bouncesRight: Byte; const feedbackLeftToLeft: Byte; const feedbackLeftToRight: Byte; const feedbackRightToRight: Byte; const feedbackRightToLeft: Byte; const premixLeftToRight: Byte; const  premixRightToLeft: Byte): Boolean;
    function ID3V2AddSeekOffsetW(const offset: LongInt): Boolean;
    function ID3V2AddSignatureFrameW(const arr: Pointer; const Length: LongInt; const GroupSymbol: SmallInt): Boolean;
    function ID3V2AddSynchronizedTempoW(const arr: Pointer; const Length: LongInt; const format: SmallInt): Boolean;
    function ID3V2AddSyncLyricW(const Language: WideString; const Description: WideString; const text: WideString; const ContentType: SmallInt; const TimestampFormat: SmallInt): Boolean;
    function ID3V2AddTableOfContentW(const ID: WideString; const Title: WideString; const Description: WideString; const isOrdered: SmallInt): Boolean;
    function ID3V2AddUniqueFileIdentifierW(const arr: Pointer; const Length: LongInt; const Owner: WideString): Boolean;
    function ID3V2AddUserFrameW(const Language: WideString; const text: WideString): Boolean;
    function ID3V2AddUserTextW(const Description: WideString; const text: WideString): Boolean;
    function ID3V2AddUserURLW(const Description: WideString; const URL: WideString): Boolean;
    function ID3V2GetAllFrameIDsW: WideString;
    function ID3V2GetAudioEncryptionDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
    function ID3V2GetAudioEncryptionPreviewLengthW(const index: SmallInt): SmallInt;
    function ID3V2GetAudioEncryptionPreviewStartW(const index: SmallInt): SmallInt;
    function ID3V2GetAudioEncryptionURLW(const index: SmallInt): WideString;
    function ID3V2GetAudioSeekPointBitsPerIndexpointW: LongInt;
    function ID3V2GetAudioSeekPointDataW(const arr: Pointer; const maxLen: LongInt): LongInt;
    function ID3V2GetAudioSeekPointLengthW: LongInt;
    function ID3V2GetAudioSeekPointNumberW: LongInt;
    function ID3V2GetAudioSeekPointStartW: LongInt;
    function ID3V2GetChapterEndTimeW(const ID: WideString): LongInt;
    function ID3V2GetChapterStartTimeW(const ID: WideString): LongInt;
    function ID3V2GetChildElementIDW(const ID: WideString; const index: SmallInt): WideString;
    function ID3V2GetChildElementsW(const ID: WideString): SmallInt;
    function ID3V2GetCommentDescriptionW(const index: SmallInt): WideString;
    function ID3V2GetCommentLanguageW(const index: SmallInt): WideString;
    function ID3V2GetCommentW(const index: SmallInt): WideString;
    function ID3V2GetCommercialFrameContactURLW(const index: SmallInt): WideString;
    function ID3V2GetCommercialFrameDescriptionW(const index: SmallInt): WideString;
    function ID3V2GetCommercialFramePictureMimeW(const index: SmallInt): WideString;
    function ID3V2GetCommercialFramePictureW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
    function ID3V2GetCommercialFramePriceW(const index: SmallInt): WideString;
    function ID3V2GetCommercialFrameReceivedAsW(const index: SmallInt): SmallInt;
    function ID3V2GetCommercialFrameSellerNameW(const index: SmallInt): WideString;
    function ID3V2GetCommercialFrameValidUntilW(const index: SmallInt): WideString;
    function ID3V2DeleteAddendumW(const ID: WideString): Boolean;
    function ID3V2DeleteAllFramesW(const FrameID: TID3FrameID): Boolean;
    function ID3V2DeleteChildElementW(const ParentTocID: WideString; const ChildID:  WideString): Boolean;
    function ID3V2DeleteSelectedFrameW(const FrameID: TID3FrameID; const index: SmallInt): Boolean;
    function ID3V2DeleteSubFrameW(const ID: WideString; const FrameID: TID3FrameID): Boolean;
    function ID3V2GetEncodingW(const FrameID: TID3FrameID): SmallInt;
    function ID3V2GetEncryptionDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
    function ID3V2GetEncryptionSymbolW(const index: SmallInt): SmallInt;
    function ID3V2GetEncryptionURLW(const index: SmallInt): WideString;
    function ID3V2GetEqualisationAdjustmentBitsW(const index: SmallInt): SmallInt;
    function ID3V2GetEqualisationDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
    function ID3V2GetEqualisationIdentificationW(const index: SmallInt): WideString;
    function ID3V2GetEqualisationInterpolationW(const index: SmallInt): SmallInt;
    function ID3V2GetEventTimingCodesDataW(const arr: Pointer; const maxLen: LongInt): LongInt;
    function ID3V2GetEventTimingCodesTimestampFormatW: SmallInt;
    function ID3V2ExistsW: Boolean;
    function ID3V2GetFrameCountW(const FrameID: TID3FrameID): SmallInt;
    function ID3V2GetGeneralObjectDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
    function ID3V2GetGeneralObjectDescriptionW(const index: SmallInt): WideString;
    function ID3V2GetGeneralObjectFilenameW(const index: SmallInt): WideString;
    function ID3V2GetGeneralObjectMimeW(const index: SmallInt): WideString;
    function ID3V2GetGroupIdentificationDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
    function ID3V2GetGroupIdentificationSymbolW(const index: SmallInt): SmallInt;
    function ID3V2GetGroupIdentificationURLW(const index: SmallInt): WideString;
    function ID3V2ImportCueFileW(const FileName: WideString): Boolean;
    function ID3V2GetLinkedInformationAdditionalDataW(const index: SmallInt): WideString;
    function ID3V2GetLinkedInformationFrameIdentifierW(const index: SmallInt): LongInt;
    function ID3V2GetLinkedInformationURLW(const index: SmallInt): WideString;
    function ID3V2GetLyricDescriptionW(const index: SmallInt): WideString;
    function ID3V2GetLyricLanguageW(const index: SmallInt): WideString;
    function ID3V2GetLyricW(const index: SmallInt): WideString;
    function ID3V2GetMpegLocationLookupTableBytesDeviationW: LongInt;
    function ID3V2GetMpegLocationLookupTableBytesW: LongInt;
    function ID3V2GetMpegLocationLookupTableDataW(const arr: Pointer; const maxLen: LongInt): LongInt;
    function ID3V2GetMpegLocationLookupTableFramesW: LongInt;
    function ID3V2GetMpegLocationLookupTableMillisecondsDeviationW: LongInt;
    function ID3V2GetMpegLocationLookupTableMillisecondsW: LongInt;
    function ID3V2GetMusicCdIdentifierDataW(const arr: Pointer; const maxLen: LongInt): LongInt;
    function ID3V2GetOwnershipDateW: WideString;
    function ID3V2GetOwnershipPriceW: WideString;
    function ID3V2GetOwnershipSellerW: WideString;
    function ID3V2GetPictureArrayW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
    function ID3V2GetPictureDescriptionW(const index: SmallInt): WideString;
    function ID3V2GetPictureFileW(const FileName: WideString; const index: SmallInt): Boolean;
    function ID3V2GetPictureMimeW(const index: SmallInt): WideString;
    function ID3V2GetPictureTypeTextW(const index: SmallInt): WideString;
    function ID3V2GetPictureTypeW(const index: SmallInt): TPictureType;
    function ID3V2GetPopularimeterCounterW(const index: SmallInt): LongInt;
    function ID3V2GetPopularimeterEmailW(const index: SmallInt): WideString;
    function ID3V2GetPopularimeterRatingW(const index: SmallInt): SmallInt;
    function ID3V2GetPositionSynchronisationTimestampFormatW: SmallInt;
    function ID3V2GetPositionSynchronisationValueW: LongInt;
    function ID3V2GetPossibleCHAPIDW: WideString;
    function ID3V2GetPossibleTOCIDW: WideString;
    function ID3V2GetPrivateFrameDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
    function ID3V2GetPrivateFrameURLW(const index: SmallInt): WideString;
    function ID3V2GetRecommendedBufferSizeFlagW: SmallInt;
    function ID3V2GetRecommendedBufferSizeOffsetW: LongInt;
    function ID3V2GetRecommendedBufferSizeValueW: LongInt;
    function ID3V2GetRelativeVolumeAdjustmentDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
    function ID3V2GetRelativeVolumeAdjustmentIdentifierW(const index: SmallInt): WideString;
    function ID3V2RemoveTagFromFileW(const FileName: WideString): Boolean;
    function ID3V2RemoveTagW: Boolean;
    function ID3V2GetReverbBouncesLeftW: SmallInt;
    function ID3V2GetReverbBouncesRightW: SmallInt;
    function ID3V2GetReverbFeedbackLeftToLeftW: SmallInt;
    function ID3V2GetReverbFeedbackLeftToRightW: SmallInt;
    function ID3V2GetReverbFeedbackRightToLeftW: SmallInt;
    function ID3V2GetReverbFeedbackRightToRightW: SmallInt;
    function ID3V2GetReverbLeftW: SmallInt;
    function ID3V2GetReverbPremixLeftToRightW: SmallInt;
    function ID3V2GetReverbPremixRightToLeftW: SmallInt;
    function ID3V2GetReverbRightW: SmallInt;
    function ID3V2SaveChangesToFileW(const FileName: WideString): Boolean;
    function ID3V2SaveChangesW: Boolean;
    function ID3V2GetSeekOffsetW: LongInt;
    function ID3V2SetAddendumDescriptionW(const ID: WideString; const newDescription: WideString): Boolean;
    function ID3V2SetAddendumTitleW(const ID: WideString; const newTitle: PWidechar): Boolean;
    function ID3V2SetChapterTimesW(const ID: WideString; const startTime: Cardinal; const endTime: Cardinal): Boolean;
    function ID3V2SetFormatAndEncodingW(const format: SmallInt; const encoding: SmallInt): Boolean;
    function ID3V2SetImageSubFrameW(const arr: Pointer; const maxLen: LongInt; const ID: WideString; const Description: WideString; const PictureType: TPictureType): Boolean;
    function ID3V2SetTextSubFrameW(const ID: WideString; const FrameID: TID3FrameID; const textString: WideString; const Description: WideString): Boolean;
    function ID3V2SetTOCIsOrderedW(const ID: WideString; const status: SmallInt): Boolean;
    function ID3V2SetURLSubFrameW(const ID: WideString; const FrameID: TID3FrameID; const urlString: WideString; const Description: WideString): Boolean;
    function ID3V2GetSignatureFrameDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
    function ID3V2GetSignatureFrameGroupSymbolW(const index: SmallInt): SmallInt;
    function ID3V2GetSizeW: LongInt;
    function ID3V2GetSubFrameDescriptionW(const ID: WideString; const index: SmallInt): WideString;
    function ID3V2GetSubFrameIDW(const ID: WideString; const index: SmallInt): LongInt;
    function ID3V2GetSubFrameImageTypeW(const ID: WideString; const index: SmallInt): SmallInt;
    function ID3V2GetSubFrameImageW(const arr: Pointer; const maxLen: LongInt; const ID: WideString; const index: SmallInt): LongInt;
    function ID3V2GetSubFramesW(const ID: WideString): SmallInt;
    function ID3V2GetSubFrameTextW(const ID: WideString; const index: SmallInt): WideString;
    function ID3V2GetSubFrameTypeW(const ID: WideString; const index: SmallInt): SmallInt;
    function ID3V2GetSynchronizedTempoDataW(const arr: Pointer; const maxLen: LongInt): LongInt;
    function ID3V2GetSynchronizedTempoFormatW: SmallInt;
    function ID3V2GetSyncLyricContentTypeW(const index: SmallInt): TSyncLyricContentTypes;
    function ID3V2GetSyncLyricDescriptionW(const index: SmallInt): WideString;
    function ID3V2GetSyncLyricLanguageW(const index: SmallInt): WideString;
    function ID3V2GetSyncLyricTimeFormatW(const index: SmallInt): TSyncLyricTimeFormat;
    function ID3V2GetSyncLyricW(const index: SmallInt): WideString;
    function ID3V2GetTOCIsOrderedW(const ID: WideString): Boolean;
    function ID3V2GetTOCRootIDW: WideString;
    function ID3V2GetUniqueFileIdentifierDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
    function ID3V2GetUniqueFileIdentifierOwnerW(const index: SmallInt): WideString;
    function ID3V2GetUserFrameLanguageW(const index: SmallInt): WideString;
    function ID3V2GetUserFrameW(const index: SmallInt): WideString;
    function ID3V2GetUserTextDescriptionW(const index: SmallInt): WideString;
    function ID3V2GetUserTextW(const index: SmallInt): WideString;
    function ID3V2GetUserURLDescriptionW(const index: SmallInt): WideString;
    function ID3V2GetUserURLW(const index: SmallInt): WideString;
    function ID3V2GetVersionW: WideString;

    // LYRICS
    function LYRICSExistsW: Boolean;
    function LYRICSRemoveTagFromFileW(const FileName: WideString): Boolean;
    function LYRICSRemoveTagW: Boolean;
    function LYRICSSaveChangesToFileW(const FileName: WideString): Boolean;
    function LYRICSSaveChangesW: Boolean;
    function LYRICSGetSizeW: LongInt;
    function LYRICSGetStartPositionW: LongInt;
    function LYRICSGetVersionW: WideString;

    // MP4
    function MP4GetPictureArrayW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
    function MP4GetPictureCountW: LongInt;
    function MP4GetPictureFileW(const FileName: WideString; const index: SmallInt): Boolean;
    function MP4SaveChangesToFileW(const FileName: WideString): Boolean;
    function MP4SaveChangesW: Boolean;
    function MP4AddPictureArrayW(const arr: Pointer; const Length: LongInt): Boolean;
    function MP4AddPictureFileW(const FileName: WideString): Boolean;
    procedure MP4DeleteEntriesW;
    procedure MP4DeletePictureW(const index: SmallInt);
    procedure MP4DeletePicturesW;
    function MP4GetAllFrameIDsW: WideString;
    function MP4GetPictureMimeW(const index: SmallInt): WideString;
    function MP4GetVersionW: WideString;
    function MP4GetiTuneFrameW(const FrameID: WideString): WideString;
    procedure MP4SetiTuneFrameW(const FrameID: WideString; const textString: WideString);

    // WAV
    function WAVSaveChangesToFileW(const FileName: WideString): Boolean;
    function WAVSaveChangesW: Boolean;
    
    // MPEG
    function MPEGIsCopyrightedW: Boolean;
    function MPEGIsOriginalW: Boolean;
    function MPEGIsPaddingW: Boolean;
    function MPEGIsPrivateW: Boolean;
    function MPEGIsProtectedW: Boolean;
    function MPEGIsVBRW: Boolean;
    function MPEGSetCopyrightedW(const FileName: WideString; const newValue: SmallInt): Boolean;
    function MPEGSetOriginalW(const FileName: WideString; const newValue: SmallInt): Boolean;
    function MPEGSetPrivateW(const FileName: WideString; const newValue: SmallInt): Boolean;

    // OGG
    function OGGGetBitRateNominalW: LongInt;
    function OGGGetItemKeysW: WideString;
    function OGGRemoveTagFromFileW(const FileName: WideString): Boolean;
    function OGGRemoveTagW: Boolean;
    function OGGSaveChangesToFileW(const FileName: WideString): Boolean;
    function OGGSaveChangesW: Boolean;

    // WMA
    function WMADeletePictureW(const index: SmallInt): Boolean; 
    function WMAGetItemKeysW: WideString; 
    function WMAGetPictureArrayW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; 
    function WMAGetPictureDescriptionW(const index: SmallInt): WideString; 
    function WMAGetPictureFileW(const FileName: WideString; const index: SmallInt): Boolean; 
    function WMAGetPictureMimeW(const index: SmallInt): WideString; 
    function WMAGetPictureCountW: SmallInt; 
    function WMAGetPictureTypeW(const index: SmallInt): TPictureType; 
    function WMAIsVBRW: Boolean;
    function WMASaveChangesToFileW(const FileName: WideString): Boolean;
    function WMASaveChangesW: Boolean;
    function WMAAddPictureArrayW(const arr: Pointer; const maxLen: LongInt; const Description: WideString; const PictureType: TPictureType; const Index: SmallInt): Boolean;
    function WMAAddPictureFileW(const FileName: WideString; const Description: WideString; const PictureType: TPictureType; const Index: SmallInt): Boolean;
    function FLACAddPictureArrayW(const arr: Pointer; const dataLen: LongInt; const Description: WideString; const PictureType: TPictureType): Boolean;
    function FLACAddPictureFileW(const FileName: WideString; const Description: WideString; const PictureType: TPictureType; const asLink: Boolean): Boolean;
    function FLACDeletePictureW(const index: SmallInt): Boolean;
    procedure FLACDeletePicturesW;
    function FLACGetPictureArrayW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
    function FLACGetPictureCountW: SmallInt;
    function FLACGetPictureDescriptionW(const index: SmallInt): WideString;
    function FLACGetPictureFileW(const FileName: WideString; const index: SmallInt): Boolean;
    function FLACGetPictureMimeW(const index: SmallInt): WideString;
    function FLACGetPictureSizeW(const index: SmallInt): LongInt;
    function FLACGetPictureTypeTextW(const index: SmallInt): WideString;
    function FLACGetPictureTypeW(const index: SmallInt): TPictureType;
    function ID3V2GetPictureSizeW(const index: SmallInt): LongInt;
    function MP4GetPictureSizeW(const index: SmallInt): LongInt;
    function WMAGetPictureSizeW(const index: SmallInt): LongInt;
    function WAVGetDisplayTextW: WideString;
    procedure WAVSetDisplayTextW(const textString: WideString);

    // Other
    function GetMD5ValueFromFileW(const FileName: WideString): WideString;
    procedure SetLogFileW(const FileName: WideString);

    // constructor
    constructor create;
  end;


implementation

uses
  activex;

const
  AudioGenie3DLL = 'Audiogenie3.dll';

function AG3AACGetBitRateTypeW: PWideChar; stdcall; external AudioGenie3DLL name 'AACGetBitRateTypeW';
function AG3AACGetHeaderTypeW: PWideChar; stdcall; external AudioGenie3DLL name 'AACGetHeaderTypeW';
function AG3AACGetProfileW: PWideChar; stdcall; external AudioGenie3DLL name 'AACGetProfileW';
function AG3APEExistsW: SmallInt; stdcall; external AudioGenie3DLL name 'APEExistsW';
function AG3APEGetAlbumW: PWideChar; stdcall; external AudioGenie3DLL name 'APEGetAlbumW';
function AG3APEGetArtistW: PWideChar; stdcall; external AudioGenie3DLL name 'APEGetArtistW';
function AG3APEGetCommentW: PWideChar; stdcall; external AudioGenie3DLL name 'APEGetCommentW';
function AG3APEGetCopyrightW: PWideChar; stdcall; external AudioGenie3DLL name 'APEGetCopyrightW';
function AG3APEGetGenreW: PWideChar; stdcall; external AudioGenie3DLL name 'APEGetGenreW';
function AG3APEGetItemKeysW: PWideChar; stdcall; external AudioGenie3DLL name 'APEGetItemKeysW';
function AG3APEGetSizeW: LongInt; stdcall; external AudioGenie3DLL name 'APEGetSizeW';
function AG3APEGetTitleW: PWideChar; stdcall; external AudioGenie3DLL name 'APEGetTitleW';
function AG3APEGetTrackW: PWideChar; stdcall; external AudioGenie3DLL name 'APEGetTrackW';
function AG3APEGetUserItemW(const key: PWideChar): PWideChar; stdcall; external AudioGenie3DLL name 'APEGetUserItemW';
function AG3APEGetVersionW: PWideChar; stdcall; external AudioGenie3DLL name 'APEGetVersionW';
function AG3APEGetYearW: PWideChar; stdcall; external AudioGenie3DLL name 'APEGetYearW';
function AG3APERemoveTagFromFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'APERemoveTagFromFileW';
function AG3APERemoveTagW: SmallInt; stdcall; external AudioGenie3DLL name 'APERemoveTagW';
function AG3APESaveChangesToFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'APESaveChangesToFileW';
function AG3APESaveChangesW: SmallInt; stdcall; external AudioGenie3DLL name 'APESaveChangesW';
procedure AG3APESetAlbumW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'APESetAlbumW';
procedure AG3APESetArtistW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'APESetArtistW';
procedure AG3APESetCommentW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'APESetCommentW';
procedure AG3APESetCopyrightW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'APESetCopyrightW';
procedure AG3APESetGenreW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'APESetGenreW';
procedure AG3APESetTitleW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'APESetTitleW';
procedure AG3APESetTrackW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'APESetTrackW';
procedure AG3APESetUserItemW(const key: PWideChar; const textString: PWideChar); stdcall; external AudioGenie3DLL name 'APESetUserItemW';
procedure AG3APESetYearW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'APESetYearW';
function AG3AUDIOAnalyzeFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'AUDIOAnalyzeFileW';
function AG3AUDIOIsValidFormatW: SmallInt; stdcall; external AudioGenie3DLL name 'AUDIOIsValidFormatW';
function AG3AUDIOGetAlbumW: PWideChar; stdcall; external AudioGenie3DLL name 'AUDIOGetAlbumW';
function AG3AUDIOGetArtistW: PWideChar; stdcall; external AudioGenie3DLL name 'AUDIOGetArtistW';
function AG3AUDIOGetBitrateW: LongInt; stdcall; external AudioGenie3DLL name 'AUDIOGetBitrateW';
function AG3AUDIOGetChannelModeW: PWideChar; stdcall; external AudioGenie3DLL name 'AUDIOGetChannelModeW';
function AG3AUDIOGetChannelsW: LongInt; stdcall; external AudioGenie3DLL name 'AUDIOGetChannelsW';
function AG3AUDIOGetCommentW: PWideChar; stdcall; external AudioGenie3DLL name 'AUDIOGetCommentW';
function AG3AUDIOGetDurationMillisW: LongInt; stdcall; external AudioGenie3DLL name 'AUDIOGetDurationMillisW';
function AG3AUDIOGetDurationW: Single; stdcall; external AudioGenie3DLL name 'AUDIOGetDurationW';
function AG3AUDIOGetFileSizeW: LongInt; stdcall; external AudioGenie3DLL name 'AUDIOGetFileSizeW';
function AG3AUDIOGetGenreW: PWideChar; stdcall; external AudioGenie3DLL name 'AUDIOGetGenreW';
function AG3AUDIOGetLastErrorNumberW: LongInt; stdcall; external AudioGenie3DLL name 'AUDIOGetLastErrorNumberW';
function AG3AUDIOGetLastErrorTextW: PWideChar; stdcall; external AudioGenie3DLL name 'AUDIOGetLastErrorTextW';
function AG3AUDIOGetLastFileW: PWideChar; stdcall; external AudioGenie3DLL name 'AUDIOGetLastFileW';
function AG3AUDIOGetMD5ValueW: PWideChar; stdcall; external AudioGenie3DLL name 'AUDIOGetMD5ValueW';
function AG3AUDIOGetSampleRateW: LongInt; stdcall; external AudioGenie3DLL name 'AUDIOGetSampleRateW';
function AG3AUDIOGetTitleW: PWideChar; stdcall; external AudioGenie3DLL name 'AUDIOGetTitleW';
function AG3AUDIOGetTrackW: PWideChar; stdcall; external AudioGenie3DLL name 'AUDIOGetTrackW';
function AG3AUDIOGetVersionW: PWideChar; stdcall; external AudioGenie3DLL name 'AUDIOGetVersionW';
function AG3AUDIOGetYearW: PWideChar; stdcall; external AudioGenie3DLL name 'AUDIOGetYearW';
function AG3AUDIOSaveChangesToFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'AUDIOSaveChangesToFileW';
function AG3AUDIOSaveChangesW: SmallInt; stdcall; external AudioGenie3DLL name 'AUDIOSaveChangesW';
procedure AG3AUDIOSetAlbumW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'AUDIOSetAlbumW';
procedure AG3AUDIOSetArtistW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'AUDIOSetArtistW';
procedure AG3AUDIOSetCommentW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'AUDIOSetCommentW';
procedure AG3AUDIOSetGenreW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'AUDIOSetGenreW';
procedure AG3AUDIOSetTitleW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'AUDIOSetTitleW';
procedure AG3AUDIOSetTrackW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'AUDIOSetTrackW';
procedure AG3AUDIOSetYearW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'AUDIOSetYearW';
function AG3FLACGetBitsPerSampleW: LongInt; stdcall; external AudioGenie3DLL name 'FLACGetBitsPerSampleW';
function AG3FLACGetCompressionRatioW: Single; stdcall; external AudioGenie3DLL name 'FLACGetCompressionRatioW';
function AG3FLACGetItemKeysW: PWideChar; stdcall; external AudioGenie3DLL name 'FLACGetItemKeysW';
function AG3FLACGetMaxBlockSizeW: LongInt; stdcall; external AudioGenie3DLL name 'FLACGetMaxBlockSizeW';
function AG3FLACGetMaxFrameSizeW: LongInt; stdcall; external AudioGenie3DLL name 'FLACGetMaxFrameSizeW';
function AG3FLACGetMinBlockSizeW: LongInt; stdcall; external AudioGenie3DLL name 'FLACGetMinBlockSizeW';
function AG3FLACGetMinFrameSizeW: LongInt; stdcall; external AudioGenie3DLL name 'FLACGetMinFrameSizeW';
function AG3FLACGetSamplesW: LongInt; stdcall; external AudioGenie3DLL name 'FLACGetSamplesW';
function AG3FLACGetUserItemW(const ItemKey: PWideChar): PWideChar; stdcall; external AudioGenie3DLL name 'FLACGetUserItemW';
function AG3FLACGetVendorW: PWideChar; stdcall; external AudioGenie3DLL name 'FLACGetVendorW';
function AG3FLACSaveChangesToFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'FLACSaveChangesToFileW';
function AG3FLACSaveChangesW: SmallInt; stdcall; external AudioGenie3DLL name 'FLACSaveChangesW';
procedure AG3FLACSetUserItemW(const ItemKey: PWideChar; const textString: PWideChar); stdcall; external AudioGenie3DLL name 'FLACSetUserItemW';
procedure AG3FLACSetVendorW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'FLACSetVendorW';
function AG3GetAudioGenieVersionW: PWideChar; stdcall; external AudioGenie3DLL name 'GetAudioGenieVersionW';
function AG3GetConfigValueW(const key: LongInt): LongInt; stdcall; external AudioGenie3DLL name 'GetConfigValueW';
function AG3GetMD5ValueFromFileW(const FileName: PWideChar): PWideChar; stdcall; external AudioGenie3DLL name 'GetMD5ValueFromFileW';
function AG3ID3V1ExistsW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V1ExistsW';
function AG3ID3V1GetAlbumW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V1GetAlbumW';
function AG3ID3V1GetArtistW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V1GetArtistW';
function AG3ID3V1GetCommentW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V1GetCommentW';
function AG3ID3V1GetGenreIDW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V1GetGenreIDW';
function AG3ID3V1GetGenreItemW(const number: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V1GetGenreItemW';
function AG3ID3V1GetGenreW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V1GetGenreW';
function AG3ID3V1GetGenresW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V1GetGenresW';
function AG3ID3V1GetTitleW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V1GetTitleW';
function AG3ID3V1GetTrackW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V1GetTrackW';
function AG3ID3V1GetVersionW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V1GetVersionW';
function AG3ID3V1GetYearW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V1GetYearW';
function AG3ID3V1RemoveTagFromFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V1RemoveTagFromFileW';
function AG3ID3V1RemoveTagW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V1RemoveTagW';
function AG3ID3V1SaveChangesToFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V1SaveChangesToFileW';
function AG3ID3V1SaveChangesW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V1SaveChangesW';
procedure AG3ID3V1SetAlbumW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'ID3V1SetAlbumW';
procedure AG3ID3V1SetArtistW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'ID3V1SetArtistW';
procedure AG3ID3V1SetCommentW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'ID3V1SetCommentW';
procedure AG3ID3V1SetGenreIDW(const nNewValue: SmallInt); stdcall; external AudioGenie3DLL name 'ID3V1SetGenreIDW';
procedure AG3ID3V1SetGenreW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'ID3V1SetGenreW';
procedure AG3ID3V1SetTitleW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'ID3V1SetTitleW';
procedure AG3ID3V1SetTrackW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'ID3V1SetTrackW';
procedure AG3ID3V1SetYearW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'ID3V1SetYearW';
function AG3ID3V2AddAudioEncryptionW(const arr: Pointer; const maxLen: LongInt; const URL: PWideChar; const PreviewStart: SmallInt; const PreviewLength: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddAudioEncryptionW';
function AG3ID3V2AddAudioSeekPointW(const arr: Pointer; const maxLen: LongInt; const start: LongInt; const Length: LongInt; const numbers: SmallInt; const  BitsPerPoint: Byte): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddAudioSeekPointW';
function AG3ID3V2AddChapterW(const ID: PWideChar; const Title: PWideChar; const Description: PWideChar; const startTime: LongInt; const endTime: LongInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddChapterW';
function AG3ID3V2AddChildElementW(const ParentTocID: PWideChar; const ChildID: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddChildElementW';
function AG3ID3V2AddCommentW(const Language: PWideChar; const Description: PWideChar; const text: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddCommentW';
function AG3ID3V2AddCommercialFrameW(const arr: Pointer; const maxLen: LongInt; const Price: PWidechar; const validUntil: PWideChar; const contactUrl: PWideChar; const receivedAs: SmallInt; const Seller: PWideChar; const Description: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddCommercialFrameW';
function AG3ID3V2AddEncryptionW(const arr: Pointer; const maxLen: LongInt; const URL: PWideChar; const symbol: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddEncryptionW';
function AG3ID3V2AddEqualisationW(const arr: Pointer; const maxLen: LongInt; const Interpolationmethod: Byte; const Identification: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddEqualisationW';
function AG3ID3V2AddEventTimingCodesW(const arr: Pointer; const maxLen: LongInt; const TimestampFormat: Byte): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddEventTimingCodesW';
function AG3ID3V2AddGeneralObjectW(const arr: Pointer; const maxLen: LongInt; const Mime: PWideChar; const FileName: PWideChar; const Description: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddGeneralObjectW';
function AG3ID3V2AddGroupIdentificationW(const arr: Pointer; const Length: LongInt; const URL: PWideChar; const symbol: Byte): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddGroupIdentificationW';
function AG3ID3V2AddLinkedInformationW(const FrameIdentifier: LongInt; const URL: PWideChar; const additionalData: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddLinkedInformationW';
function AG3ID3V2AddLyricW(const Language: PWideChar; const Description: PWideChar; const text: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddLyricW';
function AG3ID3V2AddMpegLocationLookupTableW(const arr: Pointer; const Length: LongInt; const Frames: LongInt; const Bytes: LongInt; const Milliseconds: LongInt; const BytesDeviation: Byte; const MillisecondsDeviation: Byte): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddMpegLocationLookupTableW';
function AG3ID3V2AddMusicCdIdentifierW(const arr: Pointer; const Length: LongInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddMusicCdIdentifierW';
function AG3ID3V2AddOwnershipW(constPrice: PWideChar; const dateString: PWideChar; const Seller: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddOwnershipW';
function AG3ID3V2AddPictureArrayW(const arr: Pointer; const maxLen: LongInt; const Description: PWideChar; const PictureType: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddPictureArrayW';
function AG3ID3V2AddPictureFileW(const FileName: PWideChar; const Description: PWideChar; const PictureType: SmallInt; const asLink: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddPictureFileW';
function AG3ID3V2AddPopularimeterW(const Email: PWideChar; const Rating: SmallInt; const Counter: LongInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddPopularimeterW';
function AG3ID3V2AddPositionSynchronisationW(const TimestampFormat: Byte; const Position: LongInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddPositionSynchronisationW';
function AG3ID3V2AddPrivateFrameW(const arr: Pointer; const Length: LongInt; const URL: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddPrivateFrameW';
function AG3ID3V2AddRecommendedBufferSizeW(const BufferSize: LongInt; const EmbeddedInfoFlag: Byte; const offset: LongInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddRecommendedBufferSizeW';
function AG3ID3V2AddRelativeVolumeAdjustmentW(const arr: Pointer; const Length: LongInt; const Identifier: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddRelativeVolumeAdjustmentW';
function AG3ID3V2AddReverbW(const reverbLeft: SmallInt; const reverbRight: SmallInt; const bouncesLeft: Byte; const bouncesRight: Byte; const feedbackLeftToLeft: Byte; const feedbackLeftToRight: Byte; const feedbackRightToRight: Byte; const feedbackRightToLeft: Byte; const premixLeftToRight: Byte; const  premixRightToLeft: Byte): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddReverbW';
function AG3ID3V2AddSignatureFrameW(const arr: Pointer; const Length: LongInt; const GroupSymbol: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddSignatureFrameW';
function AG3ID3V2AddSyncLyricW(const Language: PWideChar; const Description: PWideChar; const text: PWideChar; const ContentType: SmallInt; const TimestampFormat: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddSyncLyricW';
function AG3ID3V2AddSynchronizedTempoW(const arr: Pointer; const Length: LongInt; const format: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddSynchronizedTempoW';
function AG3ID3V2AddTableOfContentW(const ID: PWideChar; const Title: PWideChar; const Description: PWideChar; const isOrdered: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddTableOfContentW';
function AG3ID3V2AddUniqueFileIdentifierW(const arr: Pointer; const Length: LongInt; const Owner: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddUniqueFileIdentifierW';
function AG3ID3V2AddUserFrameW(const Language: PWideChar; const text: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddUserFrameW';
function AG3ID3V2AddUserTextW(const Description: PWideChar; const text: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddUserTextW';
function AG3ID3V2AddUserURLW(const Description: PWideChar; const URL: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddUserURLW';
function AG3ID3V2DeleteAddendumW(const ID: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2DeleteAddendumW';
function AG3ID3V2DeleteAllFramesW(const FrameID: LongInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2DeleteAllFramesW';
function AG3ID3V2DeleteChildElementW(const ParentTocID: PWideChar; const ChildID:  PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2DeleteChildElementW';
function AG3ID3V2DeleteSelectedFrameW(const FrameID: LongInt; const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2DeleteSelectedFrameW';
function AG3ID3V2DeleteSubFrameW(const ID: PWideChar; const FrameID: LongInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2DeleteSubFrameW';
function AG3ID3V2ExistsW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2ExistsW';
function AG3ID3V2GetAddendumDescriptionW(const ID: PWideChar): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetAddendumDescriptionW';
function AG3ID3V2GetAddendumTitleW(const ID: PWideChar): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetAddendumTitleW';
function AG3ID3V2GetAddendumTypeW(const ID: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetAddendumTypeW';
function AG3ID3V2GetAllFrameIDsW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetAllFrameIDsW';
function AG3ID3V2GetAudioEncryptionDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetAudioEncryptionDataW';
function AG3ID3V2GetAudioEncryptionPreviewLengthW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetAudioEncryptionPreviewLengthW';
function AG3ID3V2GetAudioEncryptionPreviewStartW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetAudioEncryptionPreviewStartW';
function AG3ID3V2GetAudioEncryptionURLW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetAudioEncryptionURLW';
function AG3ID3V2GetAudioSeekPointBitsPerIndexpointW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetAudioSeekPointBitsPerIndexpointW';
function AG3ID3V2GetAudioSeekPointDataW(const arr: Pointer; const maxLen: LongInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetAudioSeekPointDataW';
function AG3ID3V2GetAudioSeekPointLengthW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetAudioSeekPointLengthW';
function AG3ID3V2GetAudioSeekPointNumberW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetAudioSeekPointNumberW';
function AG3ID3V2GetAudioSeekPointStartW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetAudioSeekPointStartW';
function AG3ID3V2GetChapterEndTimeW(const ID: PWideChar): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetChapterEndTimeW';
function AG3ID3V2GetChapterStartTimeW(const ID: PWideChar): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetChapterStartTimeW';
function AG3ID3V2GetChildElementIDW(const ID: PWideChar; const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetChildElementIDW';
function AG3ID3V2GetChildElementsW(const ID: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetChildElementsW';
function AG3ID3V2GetCommentDescriptionW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetCommentDescriptionW';
function AG3ID3V2GetCommentLanguageW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetCommentLanguageW';
function AG3ID3V2GetCommentW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetCommentW';
function AG3ID3V2GetCommercialFrameContactURLW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetCommercialFrameContactURLW';
function AG3ID3V2GetCommercialFrameDescriptionW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetCommercialFrameDescriptionW';
function AG3ID3V2GetCommercialFramePictureMimeW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetCommercialFramePictureMimeW';
function AG3ID3V2GetCommercialFramePictureW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetCommercialFramePictureW';
function AG3ID3V2GetCommercialFramePriceW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetCommercialFramePriceW';
function AG3ID3V2GetCommercialFrameReceivedAsW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetCommercialFrameReceivedAsW';
function AG3ID3V2GetCommercialFrameSellerNameW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetCommercialFrameSellerNameW';
function AG3ID3V2GetCommercialFrameValidUntilW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetCommercialFrameValidUntilW';
function AG3ID3V2GetEncodingW(const FrameID: LongInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetEncodingW';
function AG3ID3V2GetEncryptionDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetEncryptionDataW';
function AG3ID3V2GetEncryptionSymbolW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetEncryptionSymbolW';
function AG3ID3V2GetEncryptionURLW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetEncryptionURLW';
function AG3ID3V2GetEqualisationAdjustmentBitsW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetEqualisationAdjustmentBitsW';
function AG3ID3V2GetEqualisationDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetEqualisationDataW';
function AG3ID3V2GetEqualisationIdentificationW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetEqualisationIdentificationW';
function AG3ID3V2GetEqualisationInterpolationW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetEqualisationInterpolationW';
function AG3ID3V2GetEventTimingCodesDataW(const arr: Pointer; const maxLen: LongInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetEventTimingCodesDataW';
function AG3ID3V2GetEventTimingCodesTimestampFormatW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetEventTimingCodesTimestampFormatW';
function AG3ID3V2GetFrameCountW(const FrameID: LongInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetFrameCountW';
function AG3ID3V2GetGeneralObjectDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetGeneralObjectDataW';
function AG3ID3V2GetGeneralObjectDescriptionW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetGeneralObjectDescriptionW';
function AG3ID3V2GetGeneralObjectFilenameW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetGeneralObjectFilenameW';
function AG3ID3V2GetGeneralObjectMimeW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetGeneralObjectMimeW';
function AG3ID3V2GetGenreW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetGenreW';
function AG3ID3V2GetGroupIdentificationDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetGroupIdentificationDataW';
function AG3ID3V2GetGroupIdentificationSymbolW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetGroupIdentificationSymbolW';
function AG3ID3V2GetGroupIdentificationURLW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetGroupIdentificationURLW';
function AG3ID3V2GetLinkedInformationAdditionalDataW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetLinkedInformationAdditionalDataW';
function AG3ID3V2GetLinkedInformationFrameIdentifierW(const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetLinkedInformationFrameIdentifierW';
function AG3ID3V2GetLinkedInformationURLW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetLinkedInformationURLW';
function AG3ID3V2GetLyricDescriptionW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetLyricDescriptionW';
function AG3ID3V2GetLyricLanguageW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetLyricLanguageW';
function AG3ID3V2GetLyricW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetLyricW';
function AG3ID3V2GetMpegLocationLookupTableBytesDeviationW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetMpegLocationLookupTableBytesDeviationW';
function AG3ID3V2GetMpegLocationLookupTableBytesW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetMpegLocationLookupTableBytesW';
function AG3ID3V2GetMpegLocationLookupTableDataW(const arr: Pointer; const maxLen: LongInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetMpegLocationLookupTableDataW';
function AG3ID3V2GetMpegLocationLookupTableFramesW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetMpegLocationLookupTableFramesW';
function AG3ID3V2GetMpegLocationLookupTableMillisecondsDeviationW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetMpegLocationLookupTableMillisecondsDeviationW';
function AG3ID3V2GetMpegLocationLookupTableMillisecondsW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetMpegLocationLookupTableMillisecondsW';
function AG3ID3V2GetMusicCdIdentifierDataW(const arr: Pointer; const maxLen: LongInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetMusicCdIdentifierDataW';
function AG3ID3V2GetOwnershipDateW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetOwnershipDateW';
function AG3ID3V2GetOwnershipPriceW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetOwnershipPriceW';
function AG3ID3V2GetOwnershipSellerW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetOwnershipSellerW';
function AG3ID3V2GetPictureArrayW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetPictureArrayW';
function AG3ID3V2GetPictureDescriptionW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetPictureDescriptionW';
function AG3ID3V2GetPictureFileW(const FileName: PWideChar; const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetPictureFileW';
function AG3ID3V2GetPictureMimeW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetPictureMimeW';
function AG3ID3V2GetPictureTypeTextW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetPictureTypeTextW';
function AG3ID3V2GetPictureTypeW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetPictureTypeW';
function AG3ID3V2GetPlayCounterW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetPlayCounterW';
function AG3ID3V2GetPopularimeterCounterW(const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetPopularimeterCounterW';
function AG3ID3V2GetPopularimeterEmailW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetPopularimeterEmailW';
function AG3ID3V2GetPopularimeterRatingW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetPopularimeterRatingW';
function AG3ID3V2GetPositionSynchronisationTimestampFormatW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetPositionSynchronisationTimestampFormatW';
function AG3ID3V2GetPositionSynchronisationValueW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetPositionSynchronisationValueW';
function AG3ID3V2GetPossibleCHAPIDW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetPossibleCHAPIDW';
function AG3ID3V2GetPossibleTOCIDW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetPossibleTOCIDW';
function AG3ID3V2GetPrivateFrameDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetPrivateFrameDataW';
function AG3ID3V2GetPrivateFrameURLW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetPrivateFrameURLW';
function AG3ID3V2GetRecommendedBufferSizeFlagW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetRecommendedBufferSizeFlagW';
function AG3ID3V2GetRecommendedBufferSizeOffsetW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetRecommendedBufferSizeOffsetW';
function AG3ID3V2GetRecommendedBufferSizeValueW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetRecommendedBufferSizeValueW';
function AG3ID3V2GetRelativeVolumeAdjustmentDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetRelativeVolumeAdjustmentDataW';
function AG3ID3V2GetRelativeVolumeAdjustmentIdentifierW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetRelativeVolumeAdjustmentIdentifierW';
function AG3ID3V2GetReverbBouncesLeftW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetReverbBouncesLeftW';
function AG3ID3V2GetReverbBouncesRightW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetReverbBouncesRightW';
function AG3ID3V2GetReverbFeedbackLeftToLeftW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetReverbFeedbackLeftToLeftW';
function AG3ID3V2GetReverbFeedbackLeftToRightW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetReverbFeedbackLeftToRightW';
function AG3ID3V2GetReverbFeedbackRightToLeftW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetReverbFeedbackRightToLeftW';
function AG3ID3V2GetReverbFeedbackRightToRightW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetReverbFeedbackRightToRightW';
function AG3ID3V2GetReverbLeftW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetReverbLeftW';
function AG3ID3V2GetReverbPremixLeftToRightW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetReverbPremixLeftToRightW';
function AG3ID3V2GetReverbPremixRightToLeftW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetReverbPremixRightToLeftW';
function AG3ID3V2GetReverbRightW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetReverbRightW';
function AG3ID3V2GetSeekOffsetW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetSeekOffsetW';
function AG3ID3V2GetSignatureFrameDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetSignatureFrameDataW';
function AG3ID3V2GetSignatureFrameGroupSymbolW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetSignatureFrameGroupSymbolW';
function AG3ID3V2GetSizeW: LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetSizeW';
function AG3ID3V2GetSubFrameDescriptionW(const ID: PWideChar; const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetSubFrameDescriptionW';
function AG3ID3V2GetSubFrameIDW(const ID: PWideChar; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetSubFrameIDW';
function AG3ID3V2GetSubFrameImageTypeW(const ID: PWideChar; const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetSubFrameImageTypeW';
function AG3ID3V2GetSubFrameImageW(const arr: Pointer; const maxLen: LongInt; const ID: PWideChar; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetSubFrameImageW';
function AG3ID3V2GetSubFrameTextW(const ID: PWideChar; const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetSubFrameTextW';
function AG3ID3V2GetSubFrameTypeW(const ID: PWideChar; const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetSubFrameTypeW';
function AG3ID3V2GetSubFramesW(const ID: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetSubFramesW';
function AG3ID3V2GetSyncLyricContentTypeW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetSyncLyricContentTypeW';
function AG3ID3V2GetSyncLyricDescriptionW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetSyncLyricDescriptionW';
function AG3ID3V2GetSyncLyricLanguageW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetSyncLyricLanguageW';
function AG3ID3V2GetSyncLyricTimeFormatW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetSyncLyricTimeFormatW';
function AG3ID3V2GetSyncLyricW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetSyncLyricW';
function AG3ID3V2GetSynchronizedTempoDataW(const arr: Pointer; const maxLen: LongInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetSynchronizedTempoDataW';
function AG3ID3V2GetSynchronizedTempoFormatW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetSynchronizedTempoFormatW';
function AG3ID3V2GetTOCIsOrderedW(const ID: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2GetTOCIsOrderedW';
function AG3ID3V2GetTOCRootIDW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetTOCRootIDW';
function AG3ID3V2GetTextFrameW(const FrameID: LongInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetTextFrameW';
function AG3ID3V2GetURLFrameW(const FrameID: LongInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetURLFrameW';
function AG3ID3V2GetUniqueFileIdentifierDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetUniqueFileIdentifierDataW';
function AG3ID3V2GetUniqueFileIdentifierOwnerW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetUniqueFileIdentifierOwnerW';
function AG3ID3V2GetUserFrameLanguageW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetUserFrameLanguageW';
function AG3ID3V2GetUserFrameW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetUserFrameW';
function AG3ID3V2GetUserTextDescriptionW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetUserTextDescriptionW';
function AG3ID3V2GetUserTextW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetUserTextW';
function AG3ID3V2GetUserURLDescriptionW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetUserURLDescriptionW';
function AG3ID3V2GetUserURLW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetUserURLW';
function AG3ID3V2GetVersionW: PWideChar; stdcall; external AudioGenie3DLL name 'ID3V2GetVersionW';
function AG3ID3V2ImportCueFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2ImportCueFileW';
function AG3ID3V2RemoveTagFromFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2RemoveTagFromFileW';
function AG3ID3V2RemoveTagW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2RemoveTagW';
function AG3ID3V2SaveChangesToFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2SaveChangesToFileW';
function AG3ID3V2SaveChangesW: SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2SaveChangesW';
function AG3ID3V2SetAddendumDescriptionW(const ID: PWideChar; const newDescription: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2SetAddendumDescriptionW';
function AG3ID3V2SetAddendumTitleW(const ID: PWideChar; const newTitle: PWidechar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2SetAddendumTitleW';
function AG3ID3V2SetChapterTimesW(const ID: PWideChar; const startTime: Cardinal; const endTime: Cardinal): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2SetChapterTimesW';
function AG3ID3V2SetFormatAndEncodingW(const format: SmallInt; const encoding: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2SetFormatAndEncodingW';
procedure AG3ID3V2SetGenreW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'ID3V2SetGenreW';
function AG3ID3V2SetImageSubFrameW(const arr: Pointer; const maxLen: LongInt; const ID: PWideChar; const Description: PWideChar; const PictureType: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2SetImageSubFrameW';
procedure AG3ID3V2SetPlayCounterW(const Counter: LongInt); stdcall; external AudioGenie3DLL name 'ID3V2SetPlayCounterW';
function AG3ID3V2AddSeekOffsetW(const offset: LongInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2AddSeekOffsetW';
function AG3ID3V2SetTOCIsOrderedW(const ID: PWideChar; const status: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2SetTOCIsOrderedW';
procedure AG3ID3V2SetTextFrameW(const FrameID: LongInt; const textString: PWideChar); stdcall; external AudioGenie3DLL name 'ID3V2SetTextFrameW';
function AG3ID3V2SetTextSubFrameW(const ID: PWideChar; const FrameID: LongInt; const textString: PWideChar; const Description: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2SetTextSubFrameW';
procedure AG3ID3V2SetURLFrameW(const FrameID: LongInt; const textString: PWideChar); stdcall; external AudioGenie3DLL name 'ID3V2SetURLFrameW';
function AG3ID3V2SetURLSubFrameW(const ID: PWideChar; const FrameID: LongInt; const urlString: PWideChar; const Description: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'ID3V2SetURLSubFrameW';
function AG3LYRICSExistsW: SmallInt; stdcall; external AudioGenie3DLL name 'LYRICSExistsW';
function AG3LYRICSGetAlbumW: PWideChar; stdcall; external AudioGenie3DLL name 'LYRICSGetAlbumW';
function AG3LYRICSGetArtistW: PWideChar; stdcall; external AudioGenie3DLL name 'LYRICSGetArtistW';
function AG3LYRICSGetAuthorW: PWideChar; stdcall; external AudioGenie3DLL name 'LYRICSGetAuthorW';
function AG3LYRICSGetGenreW: PWideChar; stdcall; external AudioGenie3DLL name 'LYRICSGetGenreW';
function AG3LYRICSGetImageLinkW: PWideChar; stdcall; external AudioGenie3DLL name 'LYRICSGetImageLinkW';
function AG3LYRICSGetIndicationW: PWideChar; stdcall; external AudioGenie3DLL name 'LYRICSGetIndicationW';
function AG3LYRICSGetInformationW: PWideChar; stdcall; external AudioGenie3DLL name 'LYRICSGetInformationW';
function AG3LYRICSGetLyricsW: PWideChar; stdcall; external AudioGenie3DLL name 'LYRICSGetLyricsW';
function AG3LYRICSGetSizeW: LongInt; stdcall; external AudioGenie3DLL name 'LYRICSGetSizeW';
function AG3LYRICSGetStartPositionW: LongInt; stdcall; external AudioGenie3DLL name 'LYRICSGetStartPositionW';
function AG3LYRICSGetTitleW: PWideChar; stdcall; external AudioGenie3DLL name 'LYRICSGetTitleW';
function AG3LYRICSGetVersionW: PWideChar; stdcall; external AudioGenie3DLL name 'LYRICSGetVersionW';
function AG3LYRICSRemoveTagFromFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'LYRICSRemoveTagFromFileW';
function AG3LYRICSRemoveTagW: SmallInt; stdcall; external AudioGenie3DLL name 'LYRICSRemoveTagW';
function AG3LYRICSSaveChangesToFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'LYRICSSaveChangesToFileW';
function AG3LYRICSSaveChangesW: SmallInt; stdcall; external AudioGenie3DLL name 'LYRICSSaveChangesW';
procedure AG3LYRICSSetAlbumW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'LYRICSSetAlbumW';
procedure AG3LYRICSSetArtistW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'LYRICSSetArtistW';
procedure AG3LYRICSSetAuthorW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'LYRICSSetAuthorW';
procedure AG3LYRICSSetGenreW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'LYRICSSetGenreW';
procedure AG3LYRICSSetImageLinkW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'LYRICSSetImageLinkW';
procedure AG3LYRICSSetIndicationW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'LYRICSSetIndicationW';
procedure AG3LYRICSSetInformationW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'LYRICSSetInformationW';
procedure AG3LYRICSSetLyricsW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'LYRICSSetLyricsW';
procedure AG3LYRICSSetTitleW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'LYRICSSetTitleW';
function AG3MONKEYGetBitsW: SmallInt; stdcall; external AudioGenie3DLL name 'MONKEYGetBitsW';
function AG3MONKEYGetCompressionRatioW: Single; stdcall; external AudioGenie3DLL name 'MONKEYGetCompressionRatioW';
function AG3MONKEYGetCompressionW: PWideChar; stdcall; external AudioGenie3DLL name 'MONKEYGetCompressionW';
function AG3MONKEYGetFramesW: LongInt; stdcall; external AudioGenie3DLL name 'MONKEYGetFramesW';
function AG3MONKEYGetPeakW: Single; stdcall; external AudioGenie3DLL name 'MONKEYGetPeakW';
function AG3MONKEYGetSamplesPerFrameW: LongInt; stdcall; external AudioGenie3DLL name 'MONKEYGetSamplesPerFrameW';
function AG3MONKEYGetSamplesW: LongInt; stdcall; external AudioGenie3DLL name 'MONKEYGetSamplesW';
function AG3MP4GetTextFrameW(const FrameID: LongInt): PWideChar; stdcall; external AudioGenie3DLL name 'MP4GetTextFrameW';
function AG3MP4GetGenreW: PWideChar; stdcall; external AudioGenie3DLL name 'MP4GetGenreW';
function AG3MP4GetPictureArrayW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'MP4GetPictureArrayW';
function AG3MP4GetPictureCountW: LongInt; stdcall; external AudioGenie3DLL name 'MP4GetPictureCountW';
function AG3MP4GetPictureFileW(const FileName: PWideChar; const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'MP4GetPictureFileW';
function AG3MP4GetTrackW: PWideChar; stdcall; external AudioGenie3DLL name 'MP4GetTrackW';
function AG3MP4SaveChangesToFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'MP4SaveChangesToFileW';
function AG3MP4SaveChangesW: SmallInt; stdcall; external AudioGenie3DLL name 'MP4SaveChangesW';
procedure AG3MP4SetGenreW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'MP4SetGenreW';
procedure AG3MP4SetTextFrameW(const FrameID: LongInt; const newText: PWideChar); stdcall; external AudioGenie3DLL name 'MP4SetTextFrameW';
procedure AG3MP4SetTrackW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'MP4SetTrackW';
procedure AG3MP4DeleteEntriesW; stdcall; external AudioGenie3DLL name 'MP4DeleteEntriesW';
procedure AG3MP4DeletePictureW(const index: SmallInt); stdcall; external AudioGenie3DLL name 'MP4DeletePictureW';
procedure AG3MP4DeletePicturesW; stdcall; external AudioGenie3DLL name 'MP4DeletePicturesW';
function AG3MP4GetAllFrameIDsW: PWideChar; stdcall; external AudioGenie3DLL name 'MP4GetAllFrameIDsW';
function AG3MP4GetPictureMimeW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'MP4GetPictureMimeW';
function AG3MP4GetVersionW: PWideChar; stdcall; external AudioGenie3DLL name 'MP4GetVersionW';
function AG3MP4GetiTuneFrameW(const FrameID: PWideChar): PWideChar; stdcall; external AudioGenie3DLL name 'MP4GetiTuneFrameW';
procedure AG3MP4SetiTuneFrameW(const FrameID: PWideChar; const textString: WideString); stdcall; external AudioGenie3DLL name 'MP4SetiTuneFrameW';
function AG3MP4AddPictureArrayW(const arr: Pointer; const Length: LongInt): SmallInt; stdcall; external AudioGenie3DLL name 'MP4AddPictureArrayW';
function AG3MP4AddPictureFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'MP4AddPictureFileW';
function AG3MPEGGetEmphasisW: PWideChar; stdcall; external AudioGenie3DLL name 'MPEGGetEmphasisW';
function AG3MPEGGetEncoderW: PWideChar; stdcall; external AudioGenie3DLL name 'MPEGGetEncoderW';
function AG3MPEGGetFramePositionW: LongInt; stdcall; external AudioGenie3DLL name 'MPEGGetFramePositionW';
function AG3MPEGGetFrameSizeW: LongInt; stdcall; external AudioGenie3DLL name 'MPEGGetFrameSizeW';
function AG3MPEGGetFramesW: LongInt; stdcall; external AudioGenie3DLL name 'MPEGGetFramesW';
function AG3MPEGGetLayerW: PWideChar; stdcall; external AudioGenie3DLL name 'MPEGGetLayerW';
function AG3MPEGGetVersionW: PWideChar; stdcall; external AudioGenie3DLL name 'MPEGGetVersionW';
function AG3MPEGIsCopyrightedW: SmallInt; stdcall; external AudioGenie3DLL name 'MPEGIsCopyrightedW';
function AG3MPEGIsOriginalW: SmallInt; stdcall; external AudioGenie3DLL name 'MPEGIsOriginalW';
function AG3MPEGIsPaddingW: SmallInt; stdcall; external AudioGenie3DLL name 'MPEGIsPaddingW';
function AG3MPEGIsPrivateW: SmallInt; stdcall; external AudioGenie3DLL name 'MPEGIsPrivateW';
function AG3MPEGIsProtectedW: SmallInt; stdcall; external AudioGenie3DLL name 'MPEGIsProtectedW';
function AG3MPEGIsVBRW: SmallInt; stdcall; external AudioGenie3DLL name 'MPEGIsVBRW';
function AG3MPEGSetCopyrightedW(const FileName: PWideChar; const newValue: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'MPEGSetCopyrightedW';
function AG3MPEGSetOriginalW(const FileName: PWideChar; const newValue: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'MPEGSetOriginalW';
function AG3MPEGSetPrivateW(const FileName: PWideChar; const newValue: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'MPEGSetPrivateW';
function AG3MPPGetFramesW: LongInt; stdcall; external AudioGenie3DLL name 'MPPGetFramesW';
function AG3MPPGetStreamVersionW: SmallInt; stdcall; external AudioGenie3DLL name 'MPPGetStreamVersionW';
function AG3OGGGetAlbumW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetAlbumW';
function AG3OGGGetArtistW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetArtistW';
function AG3OGGGetBitRateNominalW: LongInt; stdcall; external AudioGenie3DLL name 'OGGGetBitRateNominalW';
function AG3OGGGetCommentW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetCommentW';
function AG3OGGGetCopyrightW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetCopyrightW';
function AG3OGGGetDateW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetDateW';
function AG3OGGGetDescriptionW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetDescriptionW';
function AG3OGGGetGenreW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetGenreW';
function AG3OGGGetISRCW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetISRCW';
function AG3OGGGetItemKeysW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetItemKeysW';
function AG3OGGGetLicenseW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetLicenseW';
function AG3OGGGetLocationW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetLocationW';
function AG3OGGGetOrganizationW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetOrganizationW';
function AG3OGGGetPerformerW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetPerformerW';
function AG3OGGGetTitleW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetTitleW';
function AG3OGGGetTrackW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetTrackW';
function AG3OGGGetUserItemW(const ItemKey: PWideChar): PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetUserItemW';
function AG3OGGGetVendorW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetVendorW';
function AG3OGGGetVersionW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetVersionW';
function AG3OGGRemoveTagFromFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'OGGRemoveTagFromFileW';
function AG3OGGRemoveTagW: SmallInt; stdcall; external AudioGenie3DLL name 'OGGRemoveTagW';
function AG3OGGSaveChangesToFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'OGGSaveChangesToFileW';
function AG3OGGSaveChangesW: SmallInt; stdcall; external AudioGenie3DLL name 'OGGSaveChangesW';
procedure AG3OGGSetAlbumW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetAlbumW';
procedure AG3OGGSetArtistW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetArtistW';
procedure AG3OGGSetCommentW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetCommentW';
procedure AG3OGGSetCopyrightW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetCopyrightW';
procedure AG3OGGSetDateW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetDateW';
procedure AG3OGGSetDescriptionW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetDescriptionW';
procedure AG3OGGSetGenreW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetGenreW';
procedure AG3OGGSetISRCW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetISRCW';
procedure AG3OGGSetLicenseW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetLicenseW';
procedure AG3OGGSetLocationW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetLocationW';
procedure AG3OGGSetOrganizationW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetOrganizationW';
procedure AG3OGGSetPerformerW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetPerformerW';
procedure AG3OGGSetTitleW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetTitleW';
procedure AG3OGGSetTrackW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetTrackW';
procedure AG3OGGSetUserItemW(const ItemKey: PWideChar; const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetUserItemW';
procedure AG3OGGSetVendorW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetVendorW';
procedure AG3OGGSetVersionW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetVersionW';
function AG3OGGGetContactW: PWideChar; stdcall; external AudioGenie3DLL name 'OGGGetContactW';
procedure AG3OGGSetContactW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'OGGSetContactW';
procedure AG3SetConfigValueW(const key: LongInt; const value: LongInt); stdcall; external AudioGenie3DLL name 'SetConfigValueW';
procedure AG3SetLogFileW(const FileName: PWideChar); stdcall; external AudioGenie3DLL name 'SetLogFileW';
function AG3WAVGetBitsPerSampleW: LongInt; stdcall; external AudioGenie3DLL name 'WAVGetBitsPerSampleW';
function AG3WAVGetBlockAlignW: LongInt; stdcall; external AudioGenie3DLL name 'WAVGetBlockAlignW';
function AG3WAVGetBytesPerSecondW: LongInt; stdcall; external AudioGenie3DLL name 'WAVGetBytesPerSecondW';
function AG3WAVGetChannelsW: LongInt; stdcall; external AudioGenie3DLL name 'WAVGetChannelsW';
function AG3WAVGetFormatIDW: SmallInt; stdcall; external AudioGenie3DLL name 'WAVGetFormatIDW';
function AG3WAVGetFormatW: PWideChar; stdcall; external AudioGenie3DLL name 'WAVGetFormatW';
function AG3WAVGetHeaderSizeW: LongInt; stdcall; external AudioGenie3DLL name 'WAVGetHeaderSizeW';
function AG3WAVGetCartChunkEntryW(const Index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'WAVGetCartChunkEntryW';
function AG3WAVGetTextFrameW(const FrameID: LongInt): PWideChar; stdcall; external AudioGenie3DLL name 'WAVGetTextFrameW';
function AG3WAVSaveChangesToFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'WAVSaveChangesToFileW';
function AG3WAVSaveChangesW: SmallInt; stdcall; external AudioGenie3DLL name 'WAVSaveChangesW';
procedure AG3WAVSetTextFrameW(const FrameID: LongInt; const textString: PWideChar); stdcall; external AudioGenie3DLL name 'WAVSetTextFrameW';
procedure AG3WAVSetCartChunkEntryW(const Index: SmallInt; const textString: PWideChar); stdcall; external AudioGenie3DLL name 'WAVSetCartChunkEntryW';
function AG3WAVGetInfoChunkIDsW: PWideChar; stdcall; external AudioGenie3DLL name 'WAVGetInfoChunkIDsW';
function AG3WMADeletePictureW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'WMADeletePictureW';
procedure AG3WMADeletePicturesW(); stdcall; external AudioGenie3DLL name 'WMADeletePicturesW';
function AG3WMAGetItemKeysW: PWideChar; stdcall; external AudioGenie3DLL name 'WMAGetItemKeysW';
function AG3WMAGetPictureArrayW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'WMAGetPictureArrayW';
function AG3WMAGetPictureDescriptionW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'WMAGetPictureDescriptionW';
function AG3WMAGetPictureFileW(const FileName: PWideChar; const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'WMAGetPictureFileW';
function AG3WMAGetPictureMimeW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'WMAGetPictureMimeW';
function AG3WMAGetPictureTypeW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'WMAGetPictureTypeW';
function AG3WMAGetPictureCountW: SmallInt; stdcall; external AudioGenie3DLL name 'WMAGetPictureCountW';
function AG3WMAGetUserItemW(const ItemKey: PWideChar): PWideChar; stdcall; external AudioGenie3DLL name 'WMAGetUserItemW';
function AG3WMAIsVBRW: SmallInt; stdcall; external AudioGenie3DLL name 'WMAIsVBRW';
function AG3WMASaveChangesToFileW(const FileName: PWideChar): SmallInt; stdcall; external AudioGenie3DLL name 'WMASaveChangesToFileW';
function AG3WMASaveChangesW: SmallInt; stdcall; external AudioGenie3DLL name 'WMASaveChangesW';
function AG3WMAAddPictureArrayW(const arr: Pointer; const maxLen: LongInt; const Description: PWideChar; const PictureType: SmallInt; const Index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'WMAAddPictureArrayW';
function AG3WMAAddPictureFileW(const FileName: PWideChar; const Description: PWideChar; const PictureType: SmallInt; const Index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'WMAAddPictureFileW';
function AG3FLACAddPictureArrayW(const arr: Pointer; const dataLen: LongInt; const Description: PWideChar; const PictureType: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'FLACAddPictureArrayW';
function AG3FLACAddPictureFileW(const FileName: PWideChar; const Description: PWideChar; const PictureType: SmallInt; const asLink: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'FLACAddPictureFileW';
function AG3FLACDeletePictureW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'FLACDeletePictureW';
procedure AG3FLACDeletePicturesW(); stdcall; external AudioGenie3DLL name 'FLACDeletePicturesW';
function AG3FLACGetPictureArrayW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'FLACGetPictureArrayW';
function AG3FLACGetPictureCountW: SmallInt; stdcall; external AudioGenie3DLL name 'FLACGetPictureCountW';
function AG3FLACGetPictureDescriptionW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'FLACGetPictureDescriptionW';
function AG3FLACGetPictureFileW(const FileName: PWideChar; const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'FLACGetPictureFileW';
function AG3FLACGetPictureMimeW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'FLACGetPictureMimeW';
function AG3FLACGetPictureSizeW(const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'FLACGetPictureSizeW';
function AG3FLACGetPictureTypeTextW(const index: SmallInt): PWideChar; stdcall; external AudioGenie3DLL name 'FLACGetPictureTypeTextW';
function AG3FLACGetPictureTypeW(const index: SmallInt): SmallInt; stdcall; external AudioGenie3DLL name 'FLACGetPictureTypeW';
function AG3ID3V2GetPictureSizeW(const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'ID3V2GetPictureSizeW';
function AG3MP4GetPictureSizeW(const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'MP4GetPictureSizeW';
function AG3WMAGetPictureSizeW(const index: SmallInt): LongInt; stdcall; external AudioGenie3DLL name 'WMAGetPictureSizeW';
function AG3WAVGetDisplayTextW: PWideChar; stdcall; external AudioGenie3DLL name 'WAVGetDisplayTextW';
procedure AG3WAVSetDisplayTextW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'WAVSetDisplayTextW';
function AG3AUDIOGetComposerW: PWideChar; stdcall; external AudioGenie3DLL name 'AUDIOGetComposerW';
procedure AG3AUDIOSetComposerW(const textString: PWideChar); stdcall; external AudioGenie3DLL name 'AUDIOSetComposerW';
procedure AG3WMASetUserItemW(const ItemKey: PWideChar; const textString: PWideChar); stdcall; external AudioGenie3DLL name 'WMASetUserItemW';


// AudioGenie provides strings as OLE-Strings.
// To to avoide memory-leaks, we have to call this function to release the string.
function ConvertString(const pc: PWideChar): WideString;
var
  len: cardinal;
begin
  try
    len := length(pc);
    SetLength(result, len);

    if len = 0 then
      exit;

    move(pc^, result[1], len * SizeOf(WideChar));
  finally
    SysFreeString(pc);
  end;
end;


function TAudioGenie3.AACGetBitRateTypeW: WideString; 
begin
  result := ConvertString(AG3AACGetBitRateTypeW());
end;

function TAudioGenie3.AACGetHeaderTypeW: WideString; 
begin
  result := ConvertString(AG3AACGetHeaderTypeW());
end;

function TAudioGenie3.AACGetProfileW: WideString; 
begin
  result := ConvertString(AG3AACGetProfileW());
end;

function TAudioGenie3.APEGetAlbumW: WideString; 
begin
  result := ConvertString(AG3APEGetAlbumW());
end;

function TAudioGenie3.APEGetArtistW: WideString; 
begin
  result := ConvertString(AG3APEGetArtistW());
end;

function TAudioGenie3.APEGetCommentW: WideString; 
begin
  result := ConvertString(AG3APEGetCommentW());
end;

function TAudioGenie3.APEGetCopyrightW: WideString; 
begin
  result := ConvertString(AG3APEGetCopyrightW());
end;

function TAudioGenie3.APEExistsW: Boolean;
begin
  result := AG3APEExistsW() = AG_TRUE;
end;

function TAudioGenie3.APEGetGenreW: WideString; 
begin
  result := ConvertString(AG3APEGetGenreW());
end;

function TAudioGenie3.APEGetItemKeysW: WideString; 
begin
  result := ConvertString(AG3APEGetItemKeysW());
end;

function TAudioGenie3.APERemoveTagFromFileW(const FileName: WideString): Boolean;
begin
  result := AG3APERemoveTagFromFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.APERemoveTagW: Boolean; 
begin
  result := AG3APERemoveTagW() = AG_TRUE;
end;

function TAudioGenie3.APESaveChangesToFileW(const FileName: WideString): Boolean;
begin
  result := AG3APESaveChangesToFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.APESaveChangesW: Boolean;
begin
  result := AG3APESaveChangesW() = AG_TRUE;
end;

function TAudioGenie3.APEGetSizeW: LongInt; 
begin
  result := AG3APEGetSizeW();
end;

function TAudioGenie3.APEGetTitleW: WideString; 
begin
  result := ConvertString(AG3APEGetTitleW());
end;

function TAudioGenie3.APEGetTrackW: WideString;
begin
  result := ConvertString(AG3APEGetTrackW());
end;

function TAudioGenie3.APEGetUserItemW(const key: WideString): WideString; 
begin
  result := ConvertString(AG3APEGetUserItemW(PWideChar(key)));
end;

function TAudioGenie3.APEGetVersionW: WideString; 
begin
  result := ConvertString(AG3APEGetVersionW());
end;

function TAudioGenie3.APEGetYearW: WideString; 
begin
  result := ConvertString(AG3APEGetYearW());
end;

function TAudioGenie3.AUDIOGetAlbumW: WideString; 
begin
  result := ConvertString(AG3AUDIOGetAlbumW());
end;

function TAudioGenie3.AUDIOAnalyzeFileW(const FileName: WideString): TAudioFormatID;
begin
  result := TAudioFormatID(AG3AUDIOAnalyzeFileW(PWideChar(FileName)));
end;

function TAudioGenie3.AUDIOGetArtistW: WideString; 
begin
  result := ConvertString(AG3AUDIOGetArtistW());
end;

function TAudioGenie3.AUDIOGetBitrateW: LongInt; 
begin
  result := AG3AUDIOGetBitrateW();
end;

function TAudioGenie3.AUDIOGetChannelModeW: WideString; 
begin
  result := ConvertString(AG3AUDIOGetChannelModeW());
end;

function TAudioGenie3.AUDIOGetChannelsW: LongInt; 
begin
  result := AG3AUDIOGetChannelsW();
end;

function TAudioGenie3.AUDIOGetCommentW: WideString; 
begin
  result := ConvertString(AG3AUDIOGetCommentW());
end;

function TAudioGenie3.AUDIOGetDurationMillisW: LongInt; 
begin
  result := AG3AUDIOGetDurationMillisW();
end;

function TAudioGenie3.AUDIOGetDurationW: Single; 
begin
  result := AG3AUDIOGetDurationW();
end;

function TAudioGenie3.AUDIOGetFileSizeW: LongInt; 
begin
  result := AG3AUDIOGetFileSizeW();
end;

function TAudioGenie3.AUDIOIsValidFormatW: Boolean;
begin
  result := AG3AUDIOIsValidFormatW() = AG_TRUE;
end;

function TAudioGenie3.GetAudioGenieVersionW: WideString;
begin
  result := ConvertString(AG3GetAudioGenieVersionW());
end;

function TAudioGenie3.AUDIOGetGenreW: WideString; 
begin
  result := ConvertString(AG3AUDIOGetGenreW());
end;

function TAudioGenie3.AUDIOGetLastErrorNumberW: LongInt; 
begin
  result := AG3AUDIOGetLastErrorNumberW();
end;

function TAudioGenie3.AUDIOGetLastErrorTextW: WideString; 
begin
  result := ConvertString(AG3AUDIOGetLastErrorTextW());
end;

function TAudioGenie3.AUDIOGetLastFileW: WideString; 
begin
  result := ConvertString(AG3AUDIOGetLastFileW());
end;

function TAudioGenie3.AUDIOGetMD5ValueW: WideString; 
begin
  result := ConvertString(AG3AUDIOGetMD5ValueW());
end;

function TAudioGenie3.AUDIOGetSampleRateW: LongInt; 
begin
  result := AG3AUDIOGetSampleRateW();
end;

function TAudioGenie3.AUDIOSaveChangesToFileW(const FileName: WideString): Boolean;
begin
  result := AG3AUDIOSaveChangesToFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.AUDIOSaveChangesW: Boolean;
begin
  result := AG3AUDIOSaveChangesW() = AG_TRUE;
end;

function TAudioGenie3.AUDIOGetTitleW: WideString; 
begin
  result := ConvertString(AG3AUDIOGetTitleW());
end;

function TAudioGenie3.AUDIOGetTrackW: WideString; 
begin
  result := ConvertString(AG3AUDIOGetTrackW());
end;

function TAudioGenie3.AUDIOGetVersionW: WideString; 
begin
  result := ConvertString(AG3AUDIOGetVersionW());
end;

function TAudioGenie3.AUDIOGetYearW: WideString; 
begin
  result := ConvertString(AG3AUDIOGetYearW());
end;

function TAudioGenie3.AUDIOGetComposerW: WideString;
begin
  result := ConvertString(AG3AUDIOGetComposerW());
end;

procedure TAudioGenie3.AUDIOSetComposerW(const textString: WideString);
begin
  AG3AUDIOSetComposerW(PWideChar(textString));
end;

function TAudioGenie3.GetConfigValueW(const key: TConfigValues): LongInt;
begin
  result := AG3GetConfigValueW(Longint(key));
end;

function TAudioGenie3.FLACGetAlbumW: WideString; 
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('ALBUM'))));
end;

function TAudioGenie3.FLACGetArrangerW: WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('ARRANGER'))));
end;

function TAudioGenie3.FLACGetArtistW: WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('ARTIST'))));
end;

function TAudioGenie3.FLACGetAuthorW: WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('AUTHOR'))));
end;

function TAudioGenie3.FLACGetBitsPerSampleW: LongInt;
begin
  result := AG3FLACGetBitsPerSampleW();
end;

function TAudioGenie3.FLACGetCommentW: WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('COMMENT'))));
end;

function TAudioGenie3.FLACGetComposerW: WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('COMPOSER'))));
end;

function TAudioGenie3.FLACGetCompressionRatioW: Single; 
begin
  result := AG3FLACGetCompressionRatioW();
end;

function TAudioGenie3.FLACGetContactW: WideString; 
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('CONTACT'))));
end;

function TAudioGenie3.FLACGetCopyrightW: WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('COPYRIGHT'))));
end;

function TAudioGenie3.FLACGetDateW: WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('DATE'))));
end;

function TAudioGenie3.FLACGetDescriptionW: WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('DESCRIPTION'))));
end;

function TAudioGenie3.FLACGetDiscNumberW: WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('DISCNUMBER'))));
end;

function TAudioGenie3.FLACGetEnsembleW: WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('ENSEMBLE'))));
end;

function TAudioGenie3.FLACGetGenreW: WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('GENRE'))));
end;

function TAudioGenie3.FLACGetISRCW: WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('ISRC'))));
end;

function TAudioGenie3.FLACGetItemKeysW: WideString; 
begin
  result := ConvertString(AG3FLACGetItemKeysW());
end;

function TAudioGenie3.FLACGetLicenseW: WideString; 
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('LICENSE'))));
end;

function TAudioGenie3.FLACGetLocationW: WideString; 
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('LOCATION'))));
end;

function TAudioGenie3.FLACGetLyricistW: WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('LYRICIST'))));
end;

function TAudioGenie3.FLACGetMaxBlockSizeW: LongInt; 
begin
  result := AG3FLACGetMaxBlockSizeW();
end;

function TAudioGenie3.FLACGetMaxFrameSizeW: LongInt; 
begin
  result := AG3FLACGetMaxFrameSizeW();
end;

function TAudioGenie3.FLACGetMinBlockSizeW: LongInt; 
begin
  result := AG3FLACGetMinBlockSizeW();
end;

function TAudioGenie3.FLACGetMinFrameSizeW: LongInt; 
begin
  result := AG3FLACGetMinFrameSizeW();
end;

function TAudioGenie3.FLACGetOrganizationW: WideString; 
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('ORGANIZATION'))));
end;

function TAudioGenie3.FLACGetPerformerW: WideString;
begin
 result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('PERFORMER'))));
end;

function TAudioGenie3.FLACGetPublisherW: WideString;
begin
 result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('PUBLISHER'))));
end;

function TAudioGenie3.FLACGetSamplesW: LongInt; 
begin
  result := AG3FLACGetSamplesW();
end;

function TAudioGenie3.FLACSaveChangesToFileW(const FileName: WideString): Boolean;
begin
  result := AG3FLACSaveChangesToFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.FLACSaveChangesW: Boolean; 
begin
  result := AG3FLACSaveChangesW() = AG_TRUE;
end;

function TAudioGenie3.FLACGetTitleW: WideString; 
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('TITLE'))));
end;

function TAudioGenie3.FLACGetTrackW: WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('TRACKNUMBER'))));
end;

function TAudioGenie3.FLACGetUserItemW(const ItemKey: WideString): WideString;
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(ItemKey)));
end;

function TAudioGenie3.FLACGetVendorW: WideString; 
begin
  result := ConvertString(AG3FLACGetVendorW());
end;

function TAudioGenie3.FLACGetVersionW: WideString; 
begin
  result := ConvertString(AG3FLACGetUserItemW(PWideChar(WideString('VERSION'))));
end;

function TAudioGenie3.ID3V1GetAlbumW: WideString; 
begin
  result := ConvertString(AG3ID3V1GetAlbumW());
end;

function TAudioGenie3.ID3V1GetArtistW: WideString; 
begin
  result := ConvertString(AG3ID3V1GetArtistW());
end;

function TAudioGenie3.ID3V1GetCommentW: WideString; 
begin
  result := ConvertString(AG3ID3V1GetCommentW());
end;

function TAudioGenie3.ID3V1ExistsW: Boolean;
begin
  result := AG3ID3V1ExistsW() = AG_TRUE;
end;

function TAudioGenie3.ID3V1GetGenreIDW: SmallInt; 
begin
  result := AG3ID3V1GetGenreIDW();
end;

function TAudioGenie3.ID3V1GetGenreItemW(const number: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V1GetGenreItemW(number));
end;

function TAudioGenie3.ID3V1GetGenresW: SmallInt; 
begin
  result := AG3ID3V1GetGenresW();
end;

function TAudioGenie3.ID3V1GetGenreW: WideString; 
begin
  result := ConvertString(AG3ID3V1GetGenreW());
end;

function TAudioGenie3.ID3V1RemoveTagFromFileW(const FileName: WideString): Boolean;
begin
  result := AG3ID3V1RemoveTagFromFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.ID3V1RemoveTagW: Boolean;
begin
  result := AG3ID3V1RemoveTagW() = AG_TRUE;
end;

function TAudioGenie3.ID3V1SaveChangesToFileW(const FileName: WideString): Boolean;
begin
  result := AG3ID3V1SaveChangesToFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.ID3V1SaveChangesW: Boolean;
begin
  result := AG3ID3V1SaveChangesW() = AG_TRUE;
end;

function TAudioGenie3.ID3V1GetTitleW: WideString; 
begin
  result := ConvertString(AG3ID3V1GetTitleW());
end;

function TAudioGenie3.ID3V1GetTrackW: WideString; 
begin
  result := ConvertString(AG3ID3V1GetTrackW());
end;

function TAudioGenie3.ID3V1GetVersionW: WideString; 
begin
  result := ConvertString(AG3ID3V1GetVersionW());
end;

function TAudioGenie3.ID3V1GetYearW: WideString; 
begin
  result := ConvertString(AG3ID3V1GetYearW());
end;

function TAudioGenie3.ID3V2AddAudioEncryptionW(const arr: Pointer; const maxLen: LongInt; const URL: WideString; const PreviewStart: SmallInt; const PreviewLength: SmallInt): Boolean;
begin
  result := AG3ID3V2AddAudioEncryptionW(PWideChar(arr), maxLen, PWideChar(URL), PreviewStart, PreviewLength) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddAudioSeekPointW(const arr: Pointer; const maxLen: LongInt; const start: LongInt; const Length: LongInt; const numbers: SmallInt; const  BitsPerPoint: Byte): Boolean;
begin
  result := AG3ID3V2AddAudioSeekPointW(PWideChar(arr), maxLen, start, Length, numbers,  BitsPerPoint) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddChapterW(const ID: WideString; const Title: WideString; const Description: WideString; const startTime: LongInt; const endTime: LongInt): Boolean;
begin
  result := AG3ID3V2AddChapterW(PWideChar(ID), PWideChar(Title), PWideChar(Description), startTime, endTime) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddChildElementW(const ParentTocID: WideString; const ChildID: WideString): Boolean;
begin
  result := AG3ID3V2AddChildElementW(PWideChar(ParentTocID), PWideChar(ChildID)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddCommentW(const Language: WideString; const Description: WideString; const text: WideString): Boolean;
begin
  result := AG3ID3V2AddCommentW(PWideChar(Language), PWideChar(Description), PWideChar(text)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddCommercialFrameW(const arr: Pointer; const maxLen: LongInt; const Price: PWidechar; const validUntil: WideString; const contactUrl: WideString; const receivedAs: SmallInt; const Seller: WideString; const Description: WideString): Boolean;
begin
  result := AG3ID3V2AddCommercialFrameW(PWideChar(arr), maxLen, PWideChar(Price), PWideChar(validUntil), PWideChar(contactUrl), receivedAs, PWideChar(Seller), PWideChar(Description)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddEncryptionW(const arr: Pointer; const maxLen: LongInt; const URL: WideString; const symbol: SmallInt): Boolean;
begin
  result := AG3ID3V2AddEncryptionW(PWideChar(arr), maxLen, PWideChar(URL), symbol) = AG_TRUE;
end;

function TAudioGenie3.ID3V2GetAddendumDescriptionW(const ID: WideString): WideString;
begin
  result := ConvertString(AG3ID3V2GetAddendumDescriptionW(PWideChar(ID)));
end;

function TAudioGenie3.ID3V2GetAddendumTitleW(const ID: WideString): WideString; 
begin
  result := ConvertString(AG3ID3V2GetAddendumTitleW(PWideChar(ID)));
end;

function TAudioGenie3.ID3V2GetAddendumTypeW(const ID: WideString): SmallInt; 
begin
  result := AG3ID3V2GetAddendumTypeW(PWideChar(ID));
end;

function TAudioGenie3.ID3V2AddEqualisationW(const arr: Pointer; const maxLen: LongInt; const Interpolationmethod: Byte; const Identification: WideString): Boolean;
begin
  result := AG3ID3V2AddEqualisationW(PWideChar(arr), maxLen, Interpolationmethod, PWideChar(Identification)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddEventTimingCodesW(const arr: Pointer; const maxLen: LongInt; const TimestampFormat: Byte): Boolean;
begin
  result := AG3ID3V2AddEventTimingCodesW(PWideChar(arr), maxLen, TimestampFormat) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddGeneralObjectW(const arr: Pointer; const maxLen: LongInt; const Mime: WideString; const FileName: WideString; const Description: WideString): Boolean;
begin
  result := AG3ID3V2AddGeneralObjectW(PWideChar(arr), maxLen, PWideChar(Mime), PWideChar(FileName), PWideChar(Description)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddGroupIdentificationW(const arr: Pointer; const Length: LongInt; const URL: WideString; const symbol: Byte): Boolean;
begin
  result := AG3ID3V2AddGroupIdentificationW(PWideChar(arr), Length, PWideChar(URL), symbol) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddLinkedInformationW(const FrameIdentifier: LongInt; const URL: WideString; const additionalData: WideString): Boolean;
begin
  result := AG3ID3V2AddLinkedInformationW(FrameIdentifier, PWideChar(URL), PWideChar(additionalData)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddLyricW(const Language: WideString; const Description: WideString; const text: WideString): Boolean;
begin
  result := AG3ID3V2AddLyricW(PWideChar(Language), PWideChar(Description), PWideChar(text)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddMpegLocationLookupTableW(const arr: Pointer; const Length: LongInt; const Frames: LongInt; const Bytes: LongInt; const Milliseconds: LongInt; const BytesDeviation: Byte; const MillisecondsDeviation: Byte): Boolean;
begin
  result := AG3ID3V2AddMpegLocationLookupTableW(PWideChar(arr), Length, Frames, Bytes, Milliseconds, BytesDeviation, MillisecondsDeviation) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddMusicCdIdentifierW(const arr: Pointer; const Length: LongInt): Boolean;
begin
  result := AG3ID3V2AddMusicCdIdentifierW(PWideChar(arr), Length) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddOwnershipW(constPrice: WideString; const dateString: WideString; const Seller: WideString): Boolean;
begin
  result := AG3ID3V2AddOwnershipW(PWideChar(constPrice), PWideChar(dateString), PWideChar(Seller)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddPictureArrayW(const arr: Pointer; const maxLen: LongInt; const Description: WideString; const PictureType: TPictureType): Boolean;
begin
  result := AG3ID3V2AddPictureArrayW(PWideChar(arr), maxLen, PWideChar(Description), Smallint(PictureType)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddPictureFileW(const FileName: WideString; const Description: WideString; const PictureType: TPictureType; const asLink: TPictureLink): Boolean;
begin
  result := AG3ID3V2AddPictureFileW(PWideChar(FileName), PWideChar(Description), Smallint(PictureType), SmallInt(asLink)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddPopularimeterW(const Email: WideString; const Rating: SmallInt; const Counter: LongInt): Boolean;
begin
  result := AG3ID3V2AddPopularimeterW(PWideChar(Email), Rating, Counter) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddPositionSynchronisationW(const TimestampFormat: Byte; const Position: LongInt): Boolean;
begin
  result := AG3ID3V2AddPositionSynchronisationW(TimestampFormat, Position) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddPrivateFrameW(const arr: Pointer; const Length: LongInt; const URL: WideString): Boolean;
begin
  result := AG3ID3V2AddPrivateFrameW(PWideChar(arr), Length, PWideChar(URL)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddRecommendedBufferSizeW(const BufferSize: LongInt; const EmbeddedInfoFlag: Byte; const offset: LongInt): Boolean;
begin
  result := AG3ID3V2AddRecommendedBufferSizeW(BufferSize, EmbeddedInfoFlag, offset) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddRelativeVolumeAdjustmentW(const arr: Pointer; const Length: LongInt; const Identifier: WideString): Boolean;
begin
  result := AG3ID3V2AddRelativeVolumeAdjustmentW(PWideChar(arr), Length, PWideChar(Identifier)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddReverbW(const reverbLeft: SmallInt; const reverbRight: SmallInt; const bouncesLeft: Byte; const bouncesRight: Byte; const feedbackLeftToLeft: Byte; const feedbackLeftToRight: Byte; const feedbackRightToRight: Byte; const feedbackRightToLeft: Byte; const premixLeftToRight: Byte; const  premixRightToLeft: Byte): Boolean;
begin
  result := AG3ID3V2AddReverbW(reverbLeft, reverbRight, bouncesLeft, bouncesRight, feedbackLeftToLeft, feedbackLeftToRight, feedbackRightToRight, feedbackRightToLeft, premixLeftToRight,  premixRightToLeft) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddSeekOffsetW(const offset: LongInt): Boolean;
begin
  result := AG3ID3V2AddSeekOffsetW(offset) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddSignatureFrameW(const arr: Pointer; const Length: LongInt; const GroupSymbol: SmallInt): Boolean;
begin
  result := AG3ID3V2AddSignatureFrameW(PWideChar(arr), Length, GroupSymbol) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddSynchronizedTempoW(const arr: Pointer; const Length: LongInt; const format: SmallInt): Boolean;
begin
  result := AG3ID3V2AddSynchronizedTempoW(PWideChar(arr), Length, format) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddSyncLyricW(const Language: WideString; const Description: WideString; const text: WideString; const ContentType: SmallInt; const TimestampFormat: SmallInt): Boolean;
begin
  result := AG3ID3V2AddSyncLyricW(PWideChar(Language), PWideChar(Description), PWideChar(text), ContentType, TimestampFormat) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddTableOfContentW(const ID: WideString; const Title: WideString; const Description: WideString; const isOrdered: SmallInt): Boolean;
begin
  result := AG3ID3V2AddTableOfContentW(PWideChar(ID), PWideChar(Title), PWideChar(Description), isOrdered) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddUniqueFileIdentifierW(const arr: Pointer; const Length: LongInt; const Owner: WideString): Boolean;
begin
  result := AG3ID3V2AddUniqueFileIdentifierW(PWideChar(arr), Length, PWideChar(Owner)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddUserFrameW(const Language: WideString; const text: WideString): Boolean;
begin
  result := AG3ID3V2AddUserFrameW(PWideChar(Language), PWideChar(text)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddUserTextW(const Description: WideString; const text: WideString): Boolean;
begin
  result := AG3ID3V2AddUserTextW(PWideChar(Description), PWideChar(text)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2AddUserURLW(const Description: WideString; const URL: WideString): Boolean;
begin
  result := AG3ID3V2AddUserURLW(PWideChar(Description), PWideChar(URL)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2GetAllFrameIDsW: WideString; 
begin
  result := ConvertString(AG3ID3V2GetAllFrameIDsW());
end;

function TAudioGenie3.ID3V2GetAudioEncryptionDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetAudioEncryptionDataW(PWideChar(arr), maxLen, index);
end;

function TAudioGenie3.ID3V2GetAudioEncryptionPreviewLengthW(const index: SmallInt): SmallInt; 
begin
  result := AG3ID3V2GetAudioEncryptionPreviewLengthW(index);
end;

function TAudioGenie3.ID3V2GetAudioEncryptionPreviewStartW(const index: SmallInt): SmallInt; 
begin
  result := AG3ID3V2GetAudioEncryptionPreviewStartW(index);
end;

function TAudioGenie3.ID3V2GetAudioEncryptionURLW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetAudioEncryptionURLW(index));
end;

function TAudioGenie3.ID3V2GetAudioSeekPointBitsPerIndexpointW: LongInt; 
begin
  result := AG3ID3V2GetAudioSeekPointBitsPerIndexpointW();
end;

function TAudioGenie3.ID3V2GetAudioSeekPointDataW(const arr: Pointer; const maxLen: LongInt): LongInt; 
begin
  result := AG3ID3V2GetAudioSeekPointDataW(PWideChar(arr), maxLen);
end;

function TAudioGenie3.ID3V2GetAudioSeekPointLengthW: LongInt; 
begin
  result := AG3ID3V2GetAudioSeekPointLengthW();
end;

function TAudioGenie3.ID3V2GetAudioSeekPointNumberW: LongInt; 
begin
  result := AG3ID3V2GetAudioSeekPointNumberW();
end;

function TAudioGenie3.ID3V2GetAudioSeekPointStartW: LongInt; 
begin
  result := AG3ID3V2GetAudioSeekPointStartW();
end;

function TAudioGenie3.ID3V2GetChapterEndTimeW(const ID: WideString): LongInt; 
begin
  result := AG3ID3V2GetChapterEndTimeW(PWideChar(ID));
end;

function TAudioGenie3.ID3V2GetChapterStartTimeW(const ID: WideString): LongInt; 
begin
  result := AG3ID3V2GetChapterStartTimeW(PWideChar(ID));
end;

function TAudioGenie3.ID3V2GetChildElementIDW(const ID: WideString; const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetChildElementIDW(PWideChar(ID), index));
end;

function TAudioGenie3.ID3V2GetChildElementsW(const ID: WideString): SmallInt; 
begin
  result := AG3ID3V2GetChildElementsW(PWideChar(ID));
end;

function TAudioGenie3.ID3V2GetCommentDescriptionW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetCommentDescriptionW(index));
end;

function TAudioGenie3.ID3V2GetCommentLanguageW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetCommentLanguageW(index));
end;

function TAudioGenie3.ID3V2GetCommentW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetCommentW(index));
end;

function TAudioGenie3.ID3V2GetCommercialFrameContactURLW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetCommercialFrameContactURLW(index));
end;

function TAudioGenie3.ID3V2GetCommercialFrameDescriptionW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetCommercialFrameDescriptionW(index));
end;

function TAudioGenie3.ID3V2GetCommercialFramePictureMimeW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetCommercialFramePictureMimeW(index));
end;

function TAudioGenie3.ID3V2GetCommercialFramePictureW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetCommercialFramePictureW(PWideChar(arr), maxLen, index);
end;

function TAudioGenie3.ID3V2GetCommercialFramePriceW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetCommercialFramePriceW(index));
end;

function TAudioGenie3.ID3V2GetCommercialFrameReceivedAsW(const index: SmallInt): SmallInt; 
begin
  result := AG3ID3V2GetCommercialFrameReceivedAsW(index);
end;

function TAudioGenie3.ID3V2GetCommercialFrameSellerNameW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetCommercialFrameSellerNameW(index));
end;

function TAudioGenie3.ID3V2GetCommercialFrameValidUntilW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetCommercialFrameValidUntilW(index));
end;

function TAudioGenie3.ID3V2DeleteAddendumW(const ID: WideString): Boolean;
begin
  result := AG3ID3V2DeleteAddendumW(PWideChar(ID)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2DeleteAllFramesW(const FrameID: TID3FrameID): Boolean;
begin
  result := AG3ID3V2DeleteAllFramesW(Longint(FrameID)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2DeleteChildElementW(const ParentTocID: WideString; const ChildID:  WideString): Boolean; 
begin
  result := AG3ID3V2DeleteChildElementW(PWideChar(ParentTocID), PWideChar(ChildID)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2DeleteSelectedFrameW(const FrameID: TID3FrameID; const index: SmallInt): Boolean;
begin
  result := AG3ID3V2DeleteSelectedFrameW(Longint(FrameID), index) = AG_TRUE;
end;

function TAudioGenie3.ID3V2DeleteSubFrameW(const ID: WideString; const FrameID: TID3FrameID): Boolean;
begin
  result := AG3ID3V2DeleteSubFrameW(PWideChar(ID), Longint(FrameID)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2GetEncodingW(const FrameID: TID3FrameID): SmallInt; 
begin
  result := AG3ID3V2GetEncodingW(Longint(FrameID));
end;

function TAudioGenie3.ID3V2GetEncryptionDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetEncryptionDataW(PWideChar(arr), maxLen, index);
end;

function TAudioGenie3.ID3V2GetEncryptionSymbolW(const index: SmallInt): SmallInt; 
begin
  result := AG3ID3V2GetEncryptionSymbolW(index);
end;

function TAudioGenie3.ID3V2GetEncryptionURLW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetEncryptionURLW(index));
end;

function TAudioGenie3.ID3V2GetEqualisationAdjustmentBitsW(const index: SmallInt): SmallInt; 
begin
  result := AG3ID3V2GetEqualisationAdjustmentBitsW(index);
end;

function TAudioGenie3.ID3V2GetEqualisationDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetEqualisationDataW(PWideChar(arr), maxLen, index);
end;

function TAudioGenie3.ID3V2GetEqualisationIdentificationW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetEqualisationIdentificationW(index));
end;

function TAudioGenie3.ID3V2GetEqualisationInterpolationW(const index: SmallInt): SmallInt; 
begin
  result := AG3ID3V2GetEqualisationInterpolationW(index);
end;

function TAudioGenie3.ID3V2GetEventTimingCodesDataW(const arr: Pointer; const maxLen: LongInt): LongInt; 
begin
  result := AG3ID3V2GetEventTimingCodesDataW(PWideChar(arr), maxLen);
end;

function TAudioGenie3.ID3V2GetEventTimingCodesTimestampFormatW: SmallInt; 
begin
  result := AG3ID3V2GetEventTimingCodesTimestampFormatW();
end;

function TAudioGenie3.ID3V2ExistsW: Boolean;
begin
  result := AG3ID3V2ExistsW() = AG_TRUE;
end;

function TAudioGenie3.ID3V2GetFrameCountW(const FrameID: TID3FrameID): SmallInt; 
begin
  result := AG3ID3V2GetFrameCountW(Longint(FrameID));
end;

function TAudioGenie3.ID3V2GetGeneralObjectDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetGeneralObjectDataW(PWideChar(arr), maxLen, index);
end;

function TAudioGenie3.ID3V2GetGeneralObjectDescriptionW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetGeneralObjectDescriptionW(index));
end;

function TAudioGenie3.ID3V2GetGeneralObjectFilenameW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetGeneralObjectFilenameW(index));
end;

function TAudioGenie3.ID3V2GetGeneralObjectMimeW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetGeneralObjectMimeW(index));
end;

function TAudioGenie3.ID3V2GetGenreW: WideString; 
begin
  result := ConvertString(AG3ID3V2GetGenreW());
end;

function TAudioGenie3.ID3V2GetGroupIdentificationDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetGroupIdentificationDataW(PWideChar(arr), maxLen, index);
end;

function TAudioGenie3.ID3V2GetGroupIdentificationSymbolW(const index: SmallInt): SmallInt; 
begin
  result := AG3ID3V2GetGroupIdentificationSymbolW(index);
end;

function TAudioGenie3.ID3V2GetGroupIdentificationURLW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetGroupIdentificationURLW(index));
end;

function TAudioGenie3.ID3V2ImportCueFileW(const FileName: WideString): Boolean;
begin
  result := AG3ID3V2ImportCueFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2GetLinkedInformationAdditionalDataW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetLinkedInformationAdditionalDataW(index));
end;

function TAudioGenie3.ID3V2GetLinkedInformationFrameIdentifierW(const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetLinkedInformationFrameIdentifierW(index);
end;

function TAudioGenie3.ID3V2GetLinkedInformationURLW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetLinkedInformationURLW(index));
end;

function TAudioGenie3.ID3V2GetLyricDescriptionW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetLyricDescriptionW(index));
end;

function TAudioGenie3.ID3V2GetLyricLanguageW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetLyricLanguageW(index));
end;

function TAudioGenie3.ID3V2GetLyricW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetLyricW(index));
end;

function TAudioGenie3.ID3V2GetMpegLocationLookupTableBytesDeviationW: LongInt; 
begin
  result := AG3ID3V2GetMpegLocationLookupTableBytesDeviationW();
end;

function TAudioGenie3.ID3V2GetMpegLocationLookupTableBytesW: LongInt; 
begin
  result := AG3ID3V2GetMpegLocationLookupTableBytesW();
end;

function TAudioGenie3.ID3V2GetMpegLocationLookupTableDataW(const arr: Pointer; const maxLen: LongInt): LongInt; 
begin
  result := AG3ID3V2GetMpegLocationLookupTableDataW(PWideChar(arr), maxLen);
end;

function TAudioGenie3.ID3V2GetMpegLocationLookupTableFramesW: LongInt; 
begin
  result := AG3ID3V2GetMpegLocationLookupTableFramesW();
end;

function TAudioGenie3.ID3V2GetMpegLocationLookupTableMillisecondsDeviationW: LongInt; 
begin
  result := AG3ID3V2GetMpegLocationLookupTableMillisecondsDeviationW();
end;

function TAudioGenie3.ID3V2GetMpegLocationLookupTableMillisecondsW: LongInt; 
begin
  result := AG3ID3V2GetMpegLocationLookupTableMillisecondsW();
end;

function TAudioGenie3.ID3V2GetMusicCdIdentifierDataW(const arr: Pointer; const maxLen: LongInt): LongInt; 
begin
  result := AG3ID3V2GetMusicCdIdentifierDataW(PWideChar(arr), maxLen);
end;

function TAudioGenie3.ID3V2GetOwnershipDateW: WideString; 
begin
  result := ConvertString(AG3ID3V2GetOwnershipDateW());
end;

function TAudioGenie3.ID3V2GetOwnershipPriceW: WideString; 
begin
  result := ConvertString(AG3ID3V2GetOwnershipPriceW());
end;

function TAudioGenie3.ID3V2GetOwnershipSellerW: WideString; 
begin
  result := ConvertString(AG3ID3V2GetOwnershipSellerW());
end;

function TAudioGenie3.ID3V2GetPictureArrayW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetPictureArrayW(PWideChar(arr), maxLen, index);
end;

function TAudioGenie3.ID3V2GetPictureDescriptionW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetPictureDescriptionW(index));
end;

function TAudioGenie3.ID3V2GetPictureFileW(const FileName: WideString; const index: SmallInt): Boolean;
begin
  result := AG3ID3V2GetPictureFileW(PWideChar(FileName), index) = AG_TRUE;
end;

function TAudioGenie3.ID3V2GetPictureMimeW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetPictureMimeW(index));
end;

function TAudioGenie3.ID3V2GetPictureTypeTextW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetPictureTypeTextW(index));
end;

function TAudioGenie3.ID3V2GetPictureTypeW(const index: SmallInt): TPictureType;
begin
  result := TPictureType(AG3ID3V2GetPictureTypeW(index));
end;

function TAudioGenie3.ID3V2GetPlayCounterW: LongInt; 
begin
  result := AG3ID3V2GetPlayCounterW();
end;

function TAudioGenie3.ID3V2GetPopularimeterCounterW(const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetPopularimeterCounterW(index);
end;

function TAudioGenie3.ID3V2GetPopularimeterEmailW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetPopularimeterEmailW(index));
end;

function TAudioGenie3.ID3V2GetPopularimeterRatingW(const index: SmallInt): SmallInt; 
begin
  result := AG3ID3V2GetPopularimeterRatingW(index);
end;

function TAudioGenie3.ID3V2GetPositionSynchronisationTimestampFormatW: SmallInt; 
begin
  result := AG3ID3V2GetPositionSynchronisationTimestampFormatW();
end;

function TAudioGenie3.ID3V2GetPositionSynchronisationValueW: LongInt; 
begin
  result := AG3ID3V2GetPositionSynchronisationValueW();
end;

function TAudioGenie3.ID3V2GetPossibleCHAPIDW: WideString; 
begin
  result := ConvertString(AG3ID3V2GetPossibleCHAPIDW());
end;

function TAudioGenie3.ID3V2GetPossibleTOCIDW: WideString; 
begin
  result := ConvertString(AG3ID3V2GetPossibleTOCIDW());
end;

function TAudioGenie3.ID3V2GetPrivateFrameDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetPrivateFrameDataW(PWideChar(arr), maxLen, index);
end;

function TAudioGenie3.ID3V2GetPrivateFrameURLW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetPrivateFrameURLW(index));
end;

function TAudioGenie3.ID3V2GetRecommendedBufferSizeFlagW: SmallInt; 
begin
  result := AG3ID3V2GetRecommendedBufferSizeFlagW();
end;

function TAudioGenie3.ID3V2GetRecommendedBufferSizeOffsetW: LongInt; 
begin
  result := AG3ID3V2GetRecommendedBufferSizeOffsetW();
end;

function TAudioGenie3.ID3V2GetRecommendedBufferSizeValueW: LongInt; 
begin
  result := AG3ID3V2GetRecommendedBufferSizeValueW();
end;

function TAudioGenie3.ID3V2GetRelativeVolumeAdjustmentDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetRelativeVolumeAdjustmentDataW(PWideChar(arr), maxLen, index);
end;

function TAudioGenie3.ID3V2GetRelativeVolumeAdjustmentIdentifierW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetRelativeVolumeAdjustmentIdentifierW(index));
end;

function TAudioGenie3.ID3V2RemoveTagFromFileW(const FileName: WideString): Boolean;
begin
  result := AG3ID3V2RemoveTagFromFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2RemoveTagW: Boolean;
begin
  result := AG3ID3V2RemoveTagW() = AG_TRUE;
end;

function TAudioGenie3.ID3V2GetReverbBouncesLeftW: SmallInt; 
begin
  result := AG3ID3V2GetReverbBouncesLeftW();
end;

function TAudioGenie3.ID3V2GetReverbBouncesRightW: SmallInt; 
begin
  result := AG3ID3V2GetReverbBouncesRightW();
end;

function TAudioGenie3.ID3V2GetReverbFeedbackLeftToLeftW: SmallInt; 
begin
  result := AG3ID3V2GetReverbFeedbackLeftToLeftW();
end;

function TAudioGenie3.ID3V2GetReverbFeedbackLeftToRightW: SmallInt; 
begin
  result := AG3ID3V2GetReverbFeedbackLeftToRightW();
end;

function TAudioGenie3.ID3V2GetReverbFeedbackRightToLeftW: SmallInt; 
begin
  result := AG3ID3V2GetReverbFeedbackRightToLeftW();
end;

function TAudioGenie3.ID3V2GetReverbFeedbackRightToRightW: SmallInt; 
begin
  result := AG3ID3V2GetReverbFeedbackRightToRightW();
end;

function TAudioGenie3.ID3V2GetReverbLeftW: SmallInt; 
begin
  result := AG3ID3V2GetReverbLeftW();
end;

function TAudioGenie3.ID3V2GetReverbPremixLeftToRightW: SmallInt; 
begin
  result := AG3ID3V2GetReverbPremixLeftToRightW();
end;

function TAudioGenie3.ID3V2GetReverbPremixRightToLeftW: SmallInt; 
begin
  result := AG3ID3V2GetReverbPremixRightToLeftW();
end;

function TAudioGenie3.ID3V2GetReverbRightW: SmallInt; 
begin
  result := AG3ID3V2GetReverbRightW();
end;

function TAudioGenie3.ID3V2SaveChangesToFileW(const FileName: WideString): Boolean;
begin
  result := AG3ID3V2SaveChangesToFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2SaveChangesW: Boolean;
begin
  result := AG3ID3V2SaveChangesW() = AG_TRUE;
end;

function TAudioGenie3.ID3V2GetSeekOffsetW: LongInt; 
begin
  result := AG3ID3V2GetSeekOffsetW();
end;

function TAudioGenie3.ID3V2SetAddendumDescriptionW(const ID: WideString; const newDescription: WideString): Boolean;
begin
  result := AG3ID3V2SetAddendumDescriptionW(PWideChar(ID), PWideChar(newDescription)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2SetAddendumTitleW(const ID: WideString; const newTitle: PWidechar): Boolean;
begin
  result := AG3ID3V2SetAddendumTitleW(PWideChar(ID), PWideChar(newTitle)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2SetChapterTimesW(const ID: WideString; const startTime: Cardinal; const endTime: Cardinal): Boolean;
begin
  result := AG3ID3V2SetChapterTimesW(PWideChar(ID), startTime, endTime) = AG_TRUE;
end;

function TAudioGenie3.ID3V2SetFormatAndEncodingW(const format: SmallInt; const encoding: SmallInt): Boolean;
begin
  result := AG3ID3V2SetFormatAndEncodingW(format, encoding) = AG_TRUE;
end;

function TAudioGenie3.ID3V2SetImageSubFrameW(const arr: Pointer; const maxLen: LongInt; const ID: WideString; const Description: WideString; const PictureType: TPictureType): Boolean;
begin
  result := AG3ID3V2SetImageSubFrameW(PWideChar(arr), maxLen, PWideChar(ID), PWideChar(Description), Smallint(PictureType)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2SetTextSubFrameW(const ID: WideString; const FrameID: TID3FrameID; const textString: WideString; const Description: WideString): Boolean;
begin
  result := AG3ID3V2SetTextSubFrameW(PWideChar(ID), Longint(FrameID), PWideChar(textString), PWideChar(Description)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2SetTOCIsOrderedW(const ID: WideString; const status: SmallInt): Boolean;
begin
  result := AG3ID3V2SetTOCIsOrderedW(PWideChar(ID), status) = AG_TRUE;
end;

function TAudioGenie3.ID3V2SetURLSubFrameW(const ID: WideString; const FrameID: TID3FrameID; const urlString: WideString; const Description: WideString): Boolean;
begin
  result := AG3ID3V2SetURLSubFrameW(PWideChar(ID), Longint(FrameID), PWideChar(urlString), PWideChar(Description)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2GetSignatureFrameDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetSignatureFrameDataW(PWideChar(arr), maxLen, index);
end;

function TAudioGenie3.ID3V2GetSignatureFrameGroupSymbolW(const index: SmallInt): SmallInt; 
begin
  result := AG3ID3V2GetSignatureFrameGroupSymbolW(index);
end;

function TAudioGenie3.ID3V2GetSizeW: LongInt; 
begin
  result := AG3ID3V2GetSizeW();
end;

function TAudioGenie3.ID3V2GetSubFrameDescriptionW(const ID: WideString; const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetSubFrameDescriptionW(PWideChar(ID), index));
end;

function TAudioGenie3.ID3V2GetSubFrameIDW(const ID: WideString; const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetSubFrameIDW(PWideChar(ID), index);
end;

function TAudioGenie3.ID3V2GetSubFrameImageTypeW(const ID: WideString; const index: SmallInt): SmallInt; 
begin
  result := AG3ID3V2GetSubFrameImageTypeW(PWideChar(ID), index);
end;

function TAudioGenie3.ID3V2GetSubFrameImageW(const arr: Pointer; const maxLen: LongInt; const ID: WideString; const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetSubFrameImageW(PWideChar(arr), maxLen, PWideChar(ID), index);
end;

function TAudioGenie3.ID3V2GetSubFramesW(const ID: WideString): SmallInt; 
begin
  result := AG3ID3V2GetSubFramesW(PWideChar(ID));
end;

function TAudioGenie3.ID3V2GetSubFrameTextW(const ID: WideString; const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetSubFrameTextW(PWideChar(ID), index));
end;

function TAudioGenie3.ID3V2GetSubFrameTypeW(const ID: WideString; const index: SmallInt): SmallInt; 
begin
  result := AG3ID3V2GetSubFrameTypeW(PWideChar(ID), index);
end;

function TAudioGenie3.ID3V2GetSynchronizedTempoDataW(const arr: Pointer; const maxLen: LongInt): LongInt; 
begin
  result := AG3ID3V2GetSynchronizedTempoDataW(PWideChar(arr), maxLen);
end;

function TAudioGenie3.ID3V2GetSynchronizedTempoFormatW: SmallInt; 
begin
  result := AG3ID3V2GetSynchronizedTempoFormatW();
end;

function TAudioGenie3.ID3V2GetSyncLyricContentTypeW(const index: SmallInt): TSyncLyricContentTypes;
begin
  result := TSyncLyricContentTypes(AG3ID3V2GetSyncLyricContentTypeW(index));
end;

function TAudioGenie3.ID3V2GetSyncLyricDescriptionW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetSyncLyricDescriptionW(index));
end;

function TAudioGenie3.ID3V2GetSyncLyricLanguageW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetSyncLyricLanguageW(index));
end;

function TAudioGenie3.ID3V2GetSyncLyricTimeFormatW(const index: SmallInt): TSyncLyricTimeFormat;
begin
  result := TSyncLyricTimeFormat(AG3ID3V2GetSyncLyricTimeFormatW(index));
end;

function TAudioGenie3.ID3V2GetSyncLyricW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetSyncLyricW(index));
end;

function TAudioGenie3.ID3V2GetTextFrameW(const FrameID: TID3FrameID): WideString; 
begin
  result := ConvertString(AG3ID3V2GetTextFrameW(Longint(FrameID)));
end;

function TAudioGenie3.ID3V2GetTOCIsOrderedW(const ID: WideString): Boolean;
begin
  result := AG3ID3V2GetTOCIsOrderedW(PWideChar(ID)) = AG_TRUE;
end;

function TAudioGenie3.ID3V2GetTOCRootIDW: WideString; 
begin
  result := ConvertString(AG3ID3V2GetTOCRootIDW());
end;

function TAudioGenie3.ID3V2GetUniqueFileIdentifierDataW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; 
begin
  result := AG3ID3V2GetUniqueFileIdentifierDataW(PWideChar(arr), maxLen, index);
end;

function TAudioGenie3.ID3V2GetUniqueFileIdentifierOwnerW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetUniqueFileIdentifierOwnerW(index));
end;

function TAudioGenie3.ID3V2GetURLFrameW(const FrameID: TID3FrameID): WideString; 
begin
  result := ConvertString(AG3ID3V2GetURLFrameW(Longint(FrameID)));
end;

function TAudioGenie3.ID3V2GetUserFrameLanguageW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetUserFrameLanguageW(index));
end;

function TAudioGenie3.ID3V2GetUserFrameW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetUserFrameW(index));
end;

function TAudioGenie3.ID3V2GetUserTextDescriptionW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetUserTextDescriptionW(index));
end;

function TAudioGenie3.ID3V2GetUserTextW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetUserTextW(index));
end;

function TAudioGenie3.ID3V2GetUserURLDescriptionW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetUserURLDescriptionW(index));
end;

function TAudioGenie3.ID3V2GetUserURLW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3ID3V2GetUserURLW(index));
end;

function TAudioGenie3.ID3V2GetVersionW: WideString; 
begin
  result := ConvertString(AG3ID3V2GetVersionW());
end;

function TAudioGenie3.LYRICSGetAlbumW: WideString; 
begin
  result := ConvertString(AG3LYRICSGetAlbumW());
end;

function TAudioGenie3.LYRICSGetArtistW: WideString; 
begin
  result := ConvertString(AG3LYRICSGetArtistW());
end;

function TAudioGenie3.LYRICSGetAuthorW: WideString; 
begin
  result := ConvertString(AG3LYRICSGetAuthorW());
end;

function TAudioGenie3.LYRICSExistsW: Boolean;
begin
  result := AG3LYRICSExistsW() = AG_TRUE;
end;

function TAudioGenie3.LYRICSGetGenreW: WideString; 
begin
  result := ConvertString(AG3LYRICSGetGenreW());
end;

function TAudioGenie3.LYRICSGetImageLinkW: WideString; 
begin
  result := ConvertString(AG3LYRICSGetImageLinkW());
end;

function TAudioGenie3.LYRICSGetIndicationW: WideString; 
begin
  result := ConvertString(AG3LYRICSGetIndicationW());
end;

function TAudioGenie3.LYRICSGetInformationW: WideString; 
begin
  result := ConvertString(AG3LYRICSGetInformationW());
end;

function TAudioGenie3.LYRICSGetLyricsW: WideString; 
begin
  result := ConvertString(AG3LYRICSGetLyricsW());
end;

function TAudioGenie3.LYRICSRemoveTagFromFileW(const FileName: WideString): Boolean;
begin
  result := AG3LYRICSRemoveTagFromFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.LYRICSRemoveTagW: Boolean;
begin
  result := AG3LYRICSRemoveTagW() = AG_TRUE;
end;

function TAudioGenie3.LYRICSSaveChangesToFileW(const FileName: WideString): Boolean;
begin
  result := AG3LYRICSSaveChangesToFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.LYRICSSaveChangesW: Boolean;
begin
  result := AG3LYRICSSaveChangesW() = AG_TRUE;
end;

function TAudioGenie3.LYRICSGetSizeW: LongInt; 
begin
  result := AG3LYRICSGetSizeW();
end;

function TAudioGenie3.LYRICSGetStartPositionW: LongInt; 
begin
  result := AG3LYRICSGetStartPositionW();
end;

function TAudioGenie3.LYRICSGetTitleW: WideString; 
begin
  result := ConvertString(AG3LYRICSGetTitleW());
end;

function TAudioGenie3.LYRICSGetVersionW: WideString; 
begin
  result := ConvertString(AG3LYRICSGetVersionW());
end;

function TAudioGenie3.GetMD5ValueFromFileW(const FileName: WideString): WideString; 
begin
  result := ConvertString(AG3GetMD5ValueFromFileW(PWideChar(FileName)));
end;

function TAudioGenie3.MONKEYGetBitsW: SmallInt; 
begin
  result := AG3MONKEYGetBitsW();
end;

function TAudioGenie3.MONKEYGetCompressionRatioW: Single; 
begin
  result := AG3MONKEYGetCompressionRatioW();
end;

function TAudioGenie3.MONKEYGetCompressionW: WideString; 
begin
  result := ConvertString(AG3MONKEYGetCompressionW());
end;

function TAudioGenie3.MONKEYGetFramesW: LongInt; 
begin
  result := AG3MONKEYGetFramesW();
end;

function TAudioGenie3.MONKEYGetPeakW: Single; 
begin
  result := AG3MONKEYGetPeakW();
end;

function TAudioGenie3.MONKEYGetSamplesPerFrameW: LongInt; 
begin
  result := AG3MONKEYGetSamplesPerFrameW();
end;

function TAudioGenie3.MONKEYGetSamplesW: LongInt; 
begin
  result := AG3MONKEYGetSamplesW();
end;

function TAudioGenie3.MP4GetAllFrameIDsW: WideString;
begin
  result := ConvertString(AG3MP4GetAllFrameIDsW);
end;

function TAudioGenie3.MP4GetGenreW: WideString;
begin
  result := ConvertString(AG3MP4GetGenreW());
end;

function TAudioGenie3.MP4GetiTuneFrameW(const FrameID: WideString): WideString;
begin
  result := ConvertString(AG3MP4GetiTuneFrameW(PWideChar(FrameID)));
end;

function TAudioGenie3.MP4GetPictureArrayW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
begin
  result := AG3MP4GetPictureArrayW(PWideChar(arr), maxLen, index);
end;

function TAudioGenie3.MP4GetPictureCountW: LongInt;
begin
  result := AG3MP4GetPictureCountW();
end;

function TAudioGenie3.MP4GetPictureFileW(const FileName: WideString; const index: SmallInt): Boolean;
begin
  result := AG3MP4GetPictureFileW(PWideChar(FileName), index) = AG_TRUE;
end;

function TAudioGenie3.MP4GetPictureMimeW(const index: SmallInt): WideString;
begin
  result := ConvertString(AG3MP4GetPictureMimeW(index));
end;

function TAudioGenie3.MP4SaveChangesToFileW(const FileName: WideString): Boolean;
begin
  result := AG3MP4SaveChangesToFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.MP4SaveChangesW: Boolean;
begin
  result := AG3MP4SaveChangesW() = AG_TRUE;
end;

function TAudioGenie3.MP4AddPictureArrayW(const arr: Pointer; const Length: LongInt): Boolean;
begin
  result := AG3MP4AddPictureArrayW(PWideChar(arr), Length) = AG_TRUE;
end;

function TAudioGenie3.MP4AddPictureFileW(const FileName: WideString): Boolean;
begin
  result := AG3MP4AddPictureFileW(PWideChar(FileName)) = AG_TRUE;
end;

procedure TAudioGenie3.MP4DeleteEntriesW;
begin
  AG3MP4DeleteEntriesW;
end;

procedure TAudioGenie3.MP4DeletePicturesW;
begin
  AG3MP4DeletePicturesW
end;

procedure TAudioGenie3.MP4DeletePictureW(const index: SmallInt);
begin
  AG3MP4DeletePictureW(index);
end;

function TAudioGenie3.MP4GetTextFrameW(const FrameID: TMP4FrameID): WideString;
begin
  result := ConvertString(AG3MP4GetTextFrameW(Longint(FrameID)));
end;

function TAudioGenie3.MP4GetTrackW: WideString;
begin
  result := ConvertString(AG3MP4GetTrackW());
end;

function TAudioGenie3.MP4GetVersionW: WideString;
begin
  result := ConvertString(AG3MP4GetVersionW);
end;

function TAudioGenie3.MPEGGetEmphasisW: WideString;
begin
  result := ConvertString(AG3MPEGGetEmphasisW());
end;

function TAudioGenie3.MPEGGetEncoderW: WideString;
begin
  result := ConvertString(AG3MPEGGetEncoderW());
end;

function TAudioGenie3.MPEGGetFramePositionW: LongInt;
begin
  result := AG3MPEGGetFramePositionW();
end;

function TAudioGenie3.MPEGGetFrameSizeW: LongInt;
begin
  result := AG3MPEGGetFrameSizeW();
end;

function TAudioGenie3.MPEGGetFramesW: LongInt; 
begin
  result := AG3MPEGGetFramesW();
end;

function TAudioGenie3.MPEGIsCopyrightedW: Boolean;
begin
  result := AG3MPEGIsCopyrightedW() = AG_TRUE;
end;

function TAudioGenie3.MPEGIsOriginalW: Boolean;
begin
  result := AG3MPEGIsOriginalW() = AG_TRUE;
end;

function TAudioGenie3.MPEGIsPaddingW: Boolean;
begin
  result := AG3MPEGIsPaddingW() = AG_TRUE;
end;

function TAudioGenie3.MPEGIsPrivateW: Boolean;
begin
  result := AG3MPEGIsPrivateW() = AG_TRUE;
end;

function TAudioGenie3.MPEGIsProtectedW: Boolean;
begin
  result := AG3MPEGIsProtectedW() = AG_TRUE;
end;

function TAudioGenie3.MPEGIsVBRW: Boolean;
begin
  result := AG3MPEGIsVBRW() = AG_TRUE;
end;

function TAudioGenie3.MPEGGetLayerW: WideString; 
begin
  result := ConvertString(AG3MPEGGetLayerW());
end;

function TAudioGenie3.MPEGSetCopyrightedW(const FileName: WideString; const newValue: SmallInt): Boolean;
begin
  result := AG3MPEGSetCopyrightedW(PWideChar(FileName), newValue) = AG_TRUE;
end;

function TAudioGenie3.MPEGSetOriginalW(const FileName: WideString; const newValue: SmallInt): Boolean;
begin
  result := AG3MPEGSetOriginalW(PWideChar(FileName), newValue) = AG_TRUE;
end;

function TAudioGenie3.MPEGSetPrivateW(const FileName: WideString; const newValue: SmallInt): Boolean;
begin
  result := AG3MPEGSetPrivateW(PWideChar(FileName), newValue) = AG_TRUE;
end;

function TAudioGenie3.MPEGGetVersionW: WideString; 
begin
  result := ConvertString(AG3MPEGGetVersionW());
end;

function TAudioGenie3.MPPGetFramesW: LongInt; 
begin
  result := AG3MPPGetFramesW();
end;

function TAudioGenie3.MPPGetStreamVersionW: SmallInt; 
begin
  result := AG3MPPGetStreamVersionW();
end;

function TAudioGenie3.OGGGetAlbumW: WideString; 
begin
  result := ConvertString(AG3OGGGetAlbumW());
end;

function TAudioGenie3.OGGGetArtistW: WideString; 
begin
  result := ConvertString(AG3OGGGetArtistW());
end;

function TAudioGenie3.OGGGetBitRateNominalW: LongInt; 
begin
  result := AG3OGGGetBitRateNominalW();
end;

function TAudioGenie3.OGGGetCommentW: WideString; 
begin
  result := ConvertString(AG3OGGGetCommentW());
end;

function TAudioGenie3.OGGGetContactW: WideString; 
begin
  result := ConvertString(AG3OGGGetContactW());
end;

function TAudioGenie3.OGGGetCopyrightW: WideString; 
begin
  result := ConvertString(AG3OGGGetCopyrightW());
end;

function TAudioGenie3.OGGGetDateW: WideString; 
begin
  result := ConvertString(AG3OGGGetDateW());
end;

function TAudioGenie3.OGGGetDescriptionW: WideString; 
begin
  result := ConvertString(AG3OGGGetDescriptionW());
end;

function TAudioGenie3.OGGGetGenreW: WideString; 
begin
  result := ConvertString(AG3OGGGetGenreW());
end;

function TAudioGenie3.OGGGetISRCW: WideString; 
begin
  result := ConvertString(AG3OGGGetISRCW());
end;

function TAudioGenie3.OGGGetItemKeysW: WideString; 
begin
  result := ConvertString(AG3OGGGetItemKeysW());
end;

function TAudioGenie3.OGGGetLicenseW: WideString; 
begin
  result := ConvertString(AG3OGGGetLicenseW());
end;

function TAudioGenie3.OGGGetLocationW: WideString; 
begin
  result := ConvertString(AG3OGGGetLocationW());
end;

function TAudioGenie3.OGGGetOrganizationW: WideString; 
begin
  result := ConvertString(AG3OGGGetOrganizationW());
end;

function TAudioGenie3.OGGGetPerformerW: WideString; 
begin
  result := ConvertString(AG3OGGGetPerformerW());
end;

function TAudioGenie3.OGGRemoveTagFromFileW(const FileName: WideString): Boolean;
begin
  result := AG3OGGRemoveTagFromFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.OGGRemoveTagW: Boolean;
begin
  result := AG3OGGRemoveTagW() = AG_TRUE;
end;

function TAudioGenie3.OGGSaveChangesToFileW(const FileName: WideString): Boolean;
begin
  result := AG3OGGSaveChangesToFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.OGGSaveChangesW: Boolean;
begin
  result := AG3OGGSaveChangesW() = AG_TRUE;
end;

function TAudioGenie3.OGGGetTitleW: WideString; 
begin
  result := ConvertString(AG3OGGGetTitleW());
end;

function TAudioGenie3.OGGGetTrackW: WideString; 
begin
  result := ConvertString(AG3OGGGetTrackW());
end;

function TAudioGenie3.OGGGetUserItemW(const ItemKey: WideString): WideString; 
begin
  result := ConvertString(AG3OGGGetUserItemW(PWideChar(ItemKey)));
end;

function TAudioGenie3.OGGGetVendorW: WideString; 
begin
  result := ConvertString(AG3OGGGetVendorW());
end;

function TAudioGenie3.OGGGetVersionW: WideString; 
begin
  result := ConvertString(AG3OGGGetVersionW());
end;

function TAudioGenie3.WAVGetBitsPerSampleW: LongInt; 
begin
  result := AG3WAVGetBitsPerSampleW();
end;

function TAudioGenie3.WAVGetBlockAlignW: LongInt; 
begin
  result := AG3WAVGetBlockAlignW();
end;

function TAudioGenie3.WAVGetBytesPerSecondW: LongInt; 
begin
  result := AG3WAVGetBytesPerSecondW();
end;

function TAudioGenie3.WAVGetChannelsW: LongInt; 
begin
  result := AG3WAVGetChannelsW();
end;

function TAudioGenie3.WAVGetFormatIDW: SmallInt; 
begin
  result := AG3WAVGetFormatIDW();
end;

function TAudioGenie3.WAVGetFormatW: WideString; 
begin
  result := ConvertString(AG3WAVGetFormatW());
end;

function TAudioGenie3.WAVGetHeaderSizeW: LongInt; 
begin
  result := AG3WAVGetHeaderSizeW();
end;

function TAudioGenie3.WAVGetInfoChunkIDsW: WideString;
begin
  result := ConvertString(AG3WAVGetInfoChunkIDsW);
end;

function TAudioGenie3.WAVGetTextFrameW(const FrameID: TWAVFrameID): WideString;
begin
  result := ConvertString(AG3WAVGetTextFrameW(Longint(FrameID)));
end;

function TAudioGenie3.WAVGetCartChunkEntryW(const Index: TWavCartChunkID): WideString; 
begin
  result := ConvertString(AG3WAVGetCartChunkEntryW(Smallint(Index)));
end;

function TAudioGenie3.WAVSaveChangesToFileW(const FileName: WideString): Boolean;
begin
  result := AG3WAVSaveChangesToFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.WAVSaveChangesW: Boolean;
begin
  result := AG3WAVSaveChangesW() = AG_TRUE;
end;

function TAudioGenie3.WMAGetAlbumArtistW: WideString; 
begin
  result := WMAGetUserItemW(WM_ALBUMARTIST);
end;

function TAudioGenie3.WMAGetAlbumW: WideString; 
begin
  result := WMAGetUserItemW(WM_ALBUMTITLE);
end;

function TAudioGenie3.WMAGetArtistW: WideString; 
begin
  result := WMAGetUserItemW(WM_AUTHOR);
end;

function TAudioGenie3.WMAGetAuthorURLW: WideString; 
begin
  result := WMAGetUserItemW(WM_AUTHORURL);
end;

function TAudioGenie3.WMAGetBeatsPerMinuteW: WideString; 
begin
  result := WMAGetUserItemW(WM_BEATSPERMINUTE);
end;

function TAudioGenie3.WMAGetCategoryW: WideString; 
begin
  result := WMAGetUserItemW(WM_CATEGORY);
end;

function TAudioGenie3.WMAGetCommentW: WideString; 
begin
  result := WMAGetUserItemW(WM_DESCRIPTION);
end;

function TAudioGenie3.WMAGetComposerW: WideString; 
begin
  result := WMAGetUserItemW(WM_COMPOSER);
end;

function TAudioGenie3.WMAGetConductorW: WideString; 
begin
  result := WMAGetUserItemW(WM_CONDUCTOR);
end;

function TAudioGenie3.WMAGetContentGroupDescriptionW: WideString; 
begin
  result := WMAGetUserItemW(WM_CONTENTGROUPDESCRIPTION);
end;

function TAudioGenie3.WMAGetCopyrightW: WideString; 
begin
  result := WMAGetUserItemW(WM_PROVIDERCOPYRIGHT);
end;

function TAudioGenie3.WMADeletePictureW(const index: SmallInt): Boolean;
begin
  result := AG3WMADeletePictureW(index) = AG_TRUE;
end;

function TAudioGenie3.WMAGetDistributorW: WideString; 
begin
  result := WMAGetUserItemW(WM_CONTENTDISTRIBUTOR);
end;

function TAudioGenie3.WMAGetEncodedByW: WideString; 
begin
  result := WMAGetUserItemW(WM_ENCODEDBY);
end;

function TAudioGenie3.WMAGetEncodingSettingsW: WideString; 
begin
  result := WMAGetUserItemW(WM_ENCODINGSETTINGS);
end;

function TAudioGenie3.WMAGetEncodingTimeW: WideString; 
begin
  result := WMAGetUserItemW(WM_ENCODINGTIME);
end;

function TAudioGenie3.WMAGetGenreW: WideString; 
begin
  result := WMAGetUserItemW(WM_GENRE);
end;

function TAudioGenie3.WMAGetInitialKeyW: WideString; 
begin
  result := WMAGetUserItemW(WM_INITIALKEY);
end;

function TAudioGenie3.WMAGetISRCW: WideString; 
begin
  result := WMAGetUserItemW(WM_ISRC);
end;

function TAudioGenie3.WMAGetItemKeysW: WideString; 
begin
  result := ConvertString(AG3WMAGetItemKeysW());
end;

function TAudioGenie3.WMAGetLanguageW: WideString; 
begin
  result := WMAGetUserItemW(WM_LANGUAGE);
end;

function TAudioGenie3.WMAGetLyricsW: WideString; 
begin
  result := WMAGetUserItemW(WM_LYRICS);
end;

function TAudioGenie3.WMAGetMoodW: WideString; 
begin
  result := WMAGetUserItemW(WM_MOOD);
end;

function TAudioGenie3.WMAGetParentalRatingW: WideString; 
begin
  result := WMAGetUserItemW(WM_PARENTALRATING);
end;

function TAudioGenie3.WMAGetPartOfSetW: WideString; 
begin
  result := WMAGetUserItemW(WM_PARTOFSET);
end;

function TAudioGenie3.WMAGetPeriodW: WideString;
begin
  result := WMAGetUserItemW(WM_PERIOD);
end;

function TAudioGenie3.WMAGetPictureArrayW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt; 
begin
  result := AG3WMAGetPictureArrayW(PWideChar(arr), maxLen, index);
end;

function TAudioGenie3.WMAGetPictureDescriptionW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3WMAGetPictureDescriptionW(index));
end;

function TAudioGenie3.WMAGetPictureFileW(const FileName: WideString; const index: SmallInt): Boolean;
begin
  result := AG3WMAGetPictureFileW(PWideChar(FileName), index) = AG_TRUE;
end;

function TAudioGenie3.WMAGetPictureMimeW(const index: SmallInt): WideString; 
begin
  result := ConvertString(AG3WMAGetPictureMimeW(index));
end;

function TAudioGenie3.WMAGetPictureCountW: SmallInt; 
begin
  result := AG3WMAGetPictureCountW();
end;

function TAudioGenie3.WMAGetPictureTypeW(const index: SmallInt): TPictureType;
begin
  result := TPictureType(AG3WMAGetPictureTypeW(index));
end;

function TAudioGenie3.WMAGetProducerW: WideString; 
begin
  result := WMAGetUserItemW(WM_PRODUCER);
end;

function TAudioGenie3.WMAGetPromotionURLW: WideString; 
begin
  result := WMAGetUserItemW(WM_PROMOTIONURL);
end;

function TAudioGenie3.WMAGetProviderW: WideString; 
begin
  result := WMAGetUserItemW(WM_PROVIDER);
end;

function TAudioGenie3.WMAGetPublisherW: WideString; 
begin
  result := WMAGetUserItemW(WM_PUBLISHER);
end;

function TAudioGenie3.WMAGetRatingW: WideString; 
begin
  result := WMAGetUserItemW(WM_PROVIDERRATING);
end;

function TAudioGenie3.WMAIsVBRW: Boolean;
begin
  result := AG3WMAIsVBRW() = AG_TRUE;
end;

function TAudioGenie3.WMASaveChangesToFileW(const FileName: WideString): Boolean;
begin
  result := AG3WMASaveChangesToFileW(PWideChar(FileName)) = AG_TRUE;
end;

function TAudioGenie3.WMASaveChangesW: Boolean;
begin
  result := AG3WMASaveChangesW() = AG_TRUE;
end;

function TAudioGenie3.WMAAddPictureArrayW(const arr: Pointer; const maxLen: LongInt; const Description: WideString; const PictureType: TPictureType; const Index: SmallInt): Boolean;
begin
  result := AG3WMAAddPictureArrayW(PWideChar(arr), maxLen, PWideChar(Description), Smallint(PictureType), Index) = AG_TRUE;
end;

function TAudioGenie3.WMAAddPictureFileW(const FileName: WideString; const Description: WideString; const PictureType: TPictureType; const Index: SmallInt): Boolean;
begin
  result := AG3WMAAddPictureFileW(PWideChar(FileName), PWideChar(Description), Smallint(PictureType), index) = AG_TRUE;
end;

function TAudioGenie3.FLACAddPictureArrayW(const arr: Pointer; const dataLen: LongInt; const Description: WideString; const PictureType: TPictureType): Boolean;
begin
  result := AG3FLACAddPictureArrayW(arr, dataLen, PWideChar(Description), Smallint(PictureType)) = AG_TRUE;
end;

function TAudioGenie3.FLACAddPictureFileW(const FileName: WideString; const Description: WideString; const PictureType: TPictureType; const asLink: Boolean): Boolean;
begin
  result := AG3FLACAddPictureFileW(PWideChar(FileName), PWideChar(Description), Smallint(PictureType), Smallint(Ord(asLink))) = AG_TRUE;
end;

function TAudioGenie3.FLACDeletePictureW(const index: SmallInt): Boolean;
begin
  result := AG3FLACDeletePictureW(index) = AG_TRUE;
end;

procedure TAudioGenie3.FLACDeletePicturesW;
begin
  AG3FLACDeletePicturesW();
end;

function TAudioGenie3.FLACGetPictureArrayW(const arr: Pointer; const maxLen: LongInt; const index: SmallInt): LongInt;
begin
  result := AG3FLACGetPictureArrayW(arr, maxLen, index);
end;

function TAudioGenie3.FLACGetPictureCountW: SmallInt;
begin
  result := AG3FLACGetPictureCountW();
end;

function TAudioGenie3.FLACGetPictureDescriptionW(const index: SmallInt): WideString;
begin
  result := ConvertString(AG3FLACGetPictureDescriptionW(index));
end;

function TAudioGenie3.FLACGetPictureFileW(const FileName: WideString; const index: SmallInt): Boolean;
begin
  result := AG3FLACGetPictureFileW(PWideChar(FileName), index) = AG_TRUE;
end;

function TAudioGenie3.FLACGetPictureMimeW(const index: SmallInt): WideString;
begin
  result := ConvertString(AG3FLACGetPictureMimeW(index));
end;

function TAudioGenie3.FLACGetPictureSizeW(const index: SmallInt): LongInt;
begin
  result := AG3FLACGetPictureSizeW(index);
end;

function TAudioGenie3.FLACGetPictureTypeTextW(const index: SmallInt): WideString;
begin
  result := ConvertString(AG3FLACGetPictureTypeTextW(index));
end;

function TAudioGenie3.FLACGetPictureTypeW(const index: SmallInt): TPictureType;
begin
  result := TPictureType(AG3FLACGetPictureTypeW(index));
end;

function TAudioGenie3.ID3V2GetPictureSizeW(const index: SmallInt): LongInt;
begin
  result := AG3ID3V2GetPictureSizeW(index);
end;

function TAudioGenie3.MP4GetPictureSizeW(const index: SmallInt): LongInt;
begin
  result := AG3MP4GetPictureSizeW(index);
end;

function TAudioGenie3.WMAGetPictureSizeW(const index: SmallInt): LongInt;
begin
  result := AG3WMAGetPictureSizeW(index);
end;

function TAudioGenie3.WAVGetDisplayTextW: WideString;
begin
  result := ConvertString(AG3WAVGetDisplayTextW());
end;

procedure TAudioGenie3.WAVSetDisplayTextW(const textString: WideString);
begin
  AG3WAVSetDisplayTextW(PWideChar(textString));
end;

function TAudioGenie3.WMAGetTitleW: WideString;
begin
  result := WMAGetUserItemW(WM_TITLE);
end;

function TAudioGenie3.WMAGetToolNameW: WideString; 
begin
  result := WMAGetUserItemW(WM_TOOLNAME);
end;

function TAudioGenie3.WMAGetToolVersionW: WideString; 
begin
  result := WMAGetUserItemW(WM_TOOLVERSION);
end;

function TAudioGenie3.WMAGetTrackNumberW: WideString; 
begin
  result := WMAGetUserItemW(WM_TRACKNUMBER);
end;

function TAudioGenie3.WMAGetUserItemW(const ItemKey: WideString): WideString; 
begin
  result := ConvertString(AG3WMAGetUserItemW(PWideChar(ItemKey)));
end;

function TAudioGenie3.WMAGetUserWebURLW: WideString; 
begin
  result := WMAGetUserItemW(WM_USERWEBURL);
end;

function TAudioGenie3.WMAGetWriterW: WideString; 
begin
  result := WMAGetUserItemW(WM_WRITER);
end;

function TAudioGenie3.WMAGetYearW: WideString;
begin
  result := WMAGetUserItemW(WM_YEAR);
end;

procedure TAudioGenie3.APESetAlbumW(const textString: WideString);
begin
  AG3APESetAlbumW(PWideChar(textString));
end;

procedure TAudioGenie3.APESetArtistW(const textString: WideString);
begin
  AG3APESetArtistW(PWideChar(textString));
end;

procedure TAudioGenie3.APESetCommentW(const textString: WideString);
begin
  AG3APESetCommentW(PWideChar(textString));
end;

procedure TAudioGenie3.APESetCopyrightW(const textString: WideString);
begin
  AG3APESetCopyrightW(PWideChar(textString));
end;

procedure TAudioGenie3.APESetGenreW(const textString: WideString);
begin
  AG3APESetGenreW(PWideChar(textString));
end;

procedure TAudioGenie3.APESetTitleW(const textString: WideString);
begin
  AG3APESetTitleW(PWideChar(textString));
end;

procedure TAudioGenie3.APESetTrackW(const textString: WideString);
begin
  AG3APESetTrackW(PWideChar(textString));
end;

procedure TAudioGenie3.APESetUserItemW(const key: WideString; const textString: WideString);
begin
  AG3APESetUserItemW(PWideChar(key), PWideChar(textString));
end;

procedure TAudioGenie3.APESetYearW(const textString: WideString);
begin
  AG3APESetYearW(PWideChar(textString));
end;

procedure TAudioGenie3.AUDIOSetAlbumW(const textString: WideString);
begin
  AG3AUDIOSetAlbumW(PWideChar(textString));
end;

procedure TAudioGenie3.AUDIOSetArtistW(const textString: WideString);
begin
  AG3AUDIOSetArtistW(PWideChar(textString));
end;

procedure TAudioGenie3.AUDIOSetCommentW(const textString: WideString);
begin
  AG3AUDIOSetCommentW(PWideChar(textString));
end;

procedure TAudioGenie3.AUDIOSetGenreW(const textString: WideString);
begin
  AG3AUDIOSetGenreW(PWideChar(textString));
end;

procedure TAudioGenie3.AUDIOSetTitleW(const textString: WideString);
begin
  AG3AUDIOSetTitleW(PWideChar(textString));
end;

procedure TAudioGenie3.AUDIOSetTrackW(const textString: WideString);
begin
  AG3AUDIOSetTrackW(PWideChar(textString));
end;

procedure TAudioGenie3.AUDIOSetYearW(const textString: WideString);
begin
  AG3AUDIOSetYearW(PWideChar(textString));
end;

procedure TAudioGenie3.SetConfigValueW(const key: TConfigValues; const value: LongInt);
begin
  AG3SetConfigValueW(Longint(key), value);
end;

procedure TAudioGenie3.FLACSetAlbumW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('ALBUM')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetArrangerW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('ARRANGER')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetArtistW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('ARTIST')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetAuthorW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('AUTHOR')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetCommentW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('COMMENT')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetComposerW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('COMPOSER')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetContactW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('CONTACT')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetCopyrightW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('COPYRIGHT')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetDateW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('DATE')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetDescriptionW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('DESCRIPTION')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetDiscNumberW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('DISCNUMBER')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetEnsembleW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('ENSEMBLE')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetGenreW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('GENRE')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetISRCW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('ISRC')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetLicenseW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('LICENSE')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetLocationW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('LOCATION')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetLyricistW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('LYRICIST')), PWideChar(textString));
end;
procedure TAudioGenie3.FLACSetOrganizationW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('ORGANIZATION')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetPerformerW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('PERFORMER')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetPublisherW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('PUBLISHER')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetTitleW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('TITLE')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetTrackW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('TRACKNUMBER')), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetUserItemW(const ItemKey: WideString; const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(ItemKey), PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetVendorW(const textString: WideString);
begin
  AG3FLACSetVendorW(PWideChar(textString));
end;

procedure TAudioGenie3.FLACSetVersionW(const textString: WideString);
begin
  AG3FLACSetUserItemW(PWideChar(WideString('VERSION')), PWideChar(textString));
end;

procedure TAudioGenie3.ID3V1SetAlbumW(const textString: WideString);
begin
  AG3ID3V1SetAlbumW(PWideChar(textString));
end;

procedure TAudioGenie3.ID3V1SetArtistW(const textString: WideString);
begin
  AG3ID3V1SetArtistW(PWideChar(textString));
end;

procedure TAudioGenie3.ID3V1SetCommentW(const textString: WideString);
begin
  AG3ID3V1SetCommentW(PWideChar(textString));
end;

procedure TAudioGenie3.ID3V1SetGenreIDW(const nNewValue: SmallInt);
begin
  AG3ID3V1SetGenreIDW(nNewValue);
end;

procedure TAudioGenie3.ID3V1SetGenreW(const textString: WideString);
begin
  AG3ID3V1SetGenreW(PWideChar(textString));
end;

procedure TAudioGenie3.ID3V1SetTitleW(const textString: WideString);
begin
  AG3ID3V1SetTitleW(PWideChar(textString));
end;

procedure TAudioGenie3.ID3V1SetTrackW(const textString: WideString);
begin
  AG3ID3V1SetTrackW(PWideChar(textString));
end;

procedure TAudioGenie3.ID3V1SetYearW(const textString: WideString);
begin
  AG3ID3V1SetYearW(PWideChar(textString));
end;

procedure TAudioGenie3.ID3V2SetGenreW(const textString: WideString);
begin
  AG3ID3V2SetGenreW(PWideChar(textString));
end;

procedure TAudioGenie3.ID3V2SetPlayCounterW(const Counter: LongInt);
begin
  AG3ID3V2SetPlayCounterW(Counter);
end;

procedure TAudioGenie3.ID3V2SetTextFrameW(const FrameID: TID3FrameID; const textString: WideString);
begin
  AG3ID3V2SetTextFrameW(Longint(FrameID), PWideChar(textString));
end;

procedure TAudioGenie3.ID3V2SetURLFrameW(const FrameID: TID3FrameID; const textString: WideString);
begin
  AG3ID3V2SetURLFrameW(Longint(FrameID), PWideChar(textString));
end;

procedure TAudioGenie3.LYRICSSetAlbumW(const textString: WideString);
begin
  AG3LYRICSSetAlbumW(PWideChar(textString));
end;

procedure TAudioGenie3.LYRICSSetArtistW(const textString: WideString);
begin
  AG3LYRICSSetArtistW(PWideChar(textString));
end;

procedure TAudioGenie3.LYRICSSetAuthorW(const textString: WideString);
begin
  AG3LYRICSSetAuthorW(PWideChar(textString));
end;

procedure TAudioGenie3.LYRICSSetGenreW(const textString: WideString);
begin
  AG3LYRICSSetGenreW(PWideChar(textString));
end;

procedure TAudioGenie3.LYRICSSetImageLinkW(const textString: WideString);
begin
  AG3LYRICSSetImageLinkW(PWideChar(textString));
end;

procedure TAudioGenie3.LYRICSSetIndicationW(const textString: WideString);
begin
  AG3LYRICSSetIndicationW(PWideChar(textString));
end;

procedure TAudioGenie3.LYRICSSetInformationW(const textString: WideString);
begin
  AG3LYRICSSetInformationW(PWideChar(textString));
end;

procedure TAudioGenie3.LYRICSSetLyricsW(const textString: WideString);
begin
  AG3LYRICSSetLyricsW(PWideChar(textString));
end;

procedure TAudioGenie3.LYRICSSetTitleW(const textString: WideString);
begin
  AG3LYRICSSetTitleW(PWideChar(textString));
end;

procedure TAudioGenie3.SetLogFileW(const FileName: WideString);
begin
  AG3SetLogFileW(PWideChar(FileName));
end;

procedure TAudioGenie3.MP4SetGenreW(const textString: WideString);
begin
  AG3MP4SetGenreW(PWideChar(textString));
end;

procedure TAudioGenie3.MP4SetiTuneFrameW(const FrameID, textString: WideString);
begin
  AG3MP4SetiTuneFrameW(PWideChar(FrameID), PWideChar(textString));
end;

procedure TAudioGenie3.MP4SetTextFrameW(const FrameID: TMP4FrameID; const newText: WideString);
begin
  AG3MP4SetTextFrameW(Longint(FrameID), PWideChar(newText));
end;

procedure TAudioGenie3.MP4SetTrackW(const textString: WideString);
begin
  AG3MP4SetTrackW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetAlbumW(const textString: WideString);
begin
  AG3OGGSetAlbumW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetArtistW(const textString: WideString);
begin
  AG3OGGSetArtistW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetCommentW(const textString: WideString);
begin
  AG3OGGSetCommentW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetContactW(const textString: WideString);
begin
  AG3OGGSetContactW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetCopyrightW(const textString: WideString);
begin
  AG3OGGSetCopyrightW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetDateW(const textString: WideString);
begin
  AG3OGGSetDateW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetDescriptionW(const textString: WideString);
begin
  AG3OGGSetDescriptionW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetGenreW(const textString: WideString);
begin
  AG3OGGSetGenreW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetISRCW(const textString: WideString);
begin
  AG3OGGSetISRCW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetLicenseW(const textString: WideString);
begin
  AG3OGGSetLicenseW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetLocationW(const textString: WideString);
begin
  AG3OGGSetLocationW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetOrganizationW(const textString: WideString);
begin
  AG3OGGSetOrganizationW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetPerformerW(const textString: WideString);
begin
  AG3OGGSetPerformerW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetTitleW(const textString: WideString);
begin
  AG3OGGSetTitleW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetTrackW(const textString: WideString);
begin
  AG3OGGSetTrackW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetUserItemW(const ItemKey: WideString; const textString: WideString);
begin
  AG3OGGSetUserItemW(PWideChar(ItemKey), PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetVendorW(const textString: WideString);
begin
  AG3OGGSetVendorW(PWideChar(textString));
end;

procedure TAudioGenie3.OGGSetVersionW(const textString: WideString);
begin
  AG3OGGSetVersionW(PWideChar(textString));
end;

procedure TAudioGenie3.WAVSetTextFrameW(const FrameID: TWAVFrameID; const textString: WideString);
begin
  AG3WAVSetTextFrameW(Longint(FrameID), PWideChar(textString));
end;

procedure TAudioGenie3.WAVSetCartChunkEntryW(const Index: TWavCartChunkID; const textString: WideString);
begin
  AG3WAVSetCartChunkEntryW(Smallint(Index), PWideChar(textString));
end;

procedure TAudioGenie3.WMASetAlbumArtistW(const textString: WideString);
begin
  WMASetUserItemW(WM_ALBUMARTIST, textString);
end;

procedure TAudioGenie3.WMASetAlbumW(const textString: WideString);
begin
  WMASetUserItemW(WM_ALBUMTITLE, textString);
end;

procedure TAudioGenie3.WMASetArtistW(const textString: WideString);
begin
  WMASetUserItemW(WM_AUTHOR, textString);
end;

procedure TAudioGenie3.WMASetAuthorURLW(const textString: WideString);
begin
  WMASetUserItemW(WM_AUTHORURL, textString);
end;

procedure TAudioGenie3.WMASetBeatsPerMinuteW(const textString: WideString);
begin
  WMASetUserItemW(WM_BEATSPERMINUTE, textString);
end;

procedure TAudioGenie3.WMASetCategoryW(const textString: WideString);
begin
  WMASetUserItemW(WM_CATEGORY, textString);
end;

procedure TAudioGenie3.WMASetCommentW(const textString: WideString);
begin
  WMASetUserItemW(WM_DESCRIPTION, textString);
end;

procedure TAudioGenie3.WMASetComposerW(const textString: WideString);
begin
  WMASetUserItemW(WM_COMPOSER, textString);
end;

procedure TAudioGenie3.WMASetConductorW(const textString: WideString);
begin
  WMASetUserItemW(WM_CONDUCTOR, textString);
end;

procedure TAudioGenie3.WMASetContentGroupDescriptionW(const textString: WideString);
begin
  WMASetUserItemW(WM_CONTENTGROUPDESCRIPTION, textString);
end;

procedure TAudioGenie3.WMASetCopyrightW(const textString: WideString);
begin
  WMASetUserItemW(WM_PROVIDERCOPYRIGHT, textString);
end;

procedure TAudioGenie3.WMASetDistributorW(const textString: WideString);
begin
  WMASetUserItemW(WM_CONTENTDISTRIBUTOR, textString);
end;

procedure TAudioGenie3.WMASetEncodedByW(const textString: WideString);
begin
  WMASetUserItemW(WM_ENCODEDBY, textString);
end;

procedure TAudioGenie3.WMASetEncodingSettingsW(const textString: WideString);
begin
  WMASetUserItemW(WM_ENCODINGSETTINGS, textString);
end;

procedure TAudioGenie3.WMASetEncodingTimeW(const textString: WideString);
begin
  WMASetUserItemW(WM_ENCODINGTIME, textString);
end;

procedure TAudioGenie3.WMASetGenreW(const textString: WideString);
begin
  WMASetUserItemW(WM_GENRE, textString);
end;

procedure TAudioGenie3.WMASetInitialKeyW(const textString: WideString);
begin
  WMASetUserItemW(WM_INITIALKEY, textString);
end;

procedure TAudioGenie3.WMASetISRCW(const textString: WideString);
begin
  WMASetUserItemW(WM_ISRC, textString);
end;

procedure TAudioGenie3.WMASetLanguageW(const textString: WideString);
begin
  WMASetUserItemW(WM_LANGUAGE, textString);
end;

procedure TAudioGenie3.WMASetLyricsW(const textString: WideString);
begin
  WMASetUserItemW(WM_LYRICS, textString);
end;

procedure TAudioGenie3.WMASetMoodW(const textString: WideString);
begin
  WMASetUserItemW(WM_MOOD, textString);
end;

procedure TAudioGenie3.WMASetParentalRatingW(const textString: WideString);
begin
  WMASetUserItemW(WM_PARENTALRATING, textString);
end;

procedure TAudioGenie3.WMASetPartOfSetW(const textString: WideString);
begin
  WMASetUserItemW(WM_PARTOFSET, textString);
end;

procedure TAudioGenie3.WMASetPeriodW(const textString: WideString);
begin
  WMASetUserItemW(WM_PERIOD, textString);
end;

procedure TAudioGenie3.WMASetProducerW(const textString: WideString);
begin
  WMASetUserItemW(WM_PRODUCER, textString);
end;

procedure TAudioGenie3.WMASetPromotionURLW(const textString: WideString);
begin
  WMASetUserItemW(WM_PROMOTIONURL, textString);
end;

procedure TAudioGenie3.WMASetProviderW(const textString: WideString);
begin
  WMASetUserItemW(WM_PROVIDER, textString);
end;

procedure TAudioGenie3.WMASetPublisherW(const textString: WideString);
begin
  WMASetUserItemW(WM_PUBLISHER, textString);
end;

procedure TAudioGenie3.WMASetRatingW(const textString: WideString);
begin
  WMASetUserItemW(WM_PROVIDERRATING, textString);
end;

procedure TAudioGenie3.WMASetTitleW(const textString: WideString);
begin
  WMASetUserItemW(WM_TITLE, textString);
end;

procedure TAudioGenie3.WMASetToolNameW(const textString: WideString);
begin
  WMASetUserItemW(WM_TOOLNAME, textString);
end;

procedure TAudioGenie3.WMASetToolVersionW(const textString: WideString);
begin
  WMASetUserItemW(WM_TOOLVERSION, textString);
end;

procedure TAudioGenie3.WMASetTrackNumberW(const textString: WideString);
begin
  WMASetUserItemW(WM_TRACKNUMBER, textString);
end;

procedure TAudioGenie3.WMASetUserItemW(const ItemKey: WideString; const textString: WideString);
begin
  AG3WMASetUserItemW(PWideChar(ItemKey), PWideChar(textString));
end;

procedure TAudioGenie3.WMASetUserWebURLW(const textString: WideString);
begin
  WMASetUserItemW(WM_USERWEBURL, textString);
end;

procedure TAudioGenie3.WMASetWriterW(const textString: WideString);
begin
  WMASetUserItemW(WM_WRITER, textString);
end;

procedure TAudioGenie3.WMASetYearW(const textString: WideString) ;
begin
  WMASetUserItemW(WM_YEAR, textString);
end;


constructor TAudioGenie3.create;
begin
  inherited create;
end;

end.
