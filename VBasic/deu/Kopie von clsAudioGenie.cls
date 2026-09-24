VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsAudioGenie"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
' *******************************************************
' ******* AUDIOGENIE DLL WRAPPER FOR VISUAL BASIC *******
' *******************************************************
' *   this class is a Wrapper for the audiogenie2.dll   *
' *    written by Stefan Toengi, www.audiogenie.net     *
' *  Visit my forum under http://forum.audiogenie.net   *
' * To use this class, copy this file in your project   *
' * and the audiogenie2.dll in the %Windows%\system32   *
' * folder(32bit) or %Windows%\SysWOW64 folder(64bit).  *
' * Now you can use this class like the example         *
'  Dim Genie As clsAudioGenie
'  Set Genie = New clsAudioGenie
'  Genie.AUDIOAnalyzeFile(file$)
' *******************************************************
' *******  Copyright 2007, 2008 by Stefan Toengi  *******
' *******************************************************
Option Explicit

' Universal
Private Declare Sub DisablePopupKeyW Lib "AudioGenie3.dll" Alias "DisablePopupKey" (ByVal keyValue As Long)
Private Declare Function GetAudioGenieVersionW Lib "AudioGenie3.dll" Alias "GetAudioGenieVersion" () As String
Private Declare Function GetMD5ValueFromFileW Lib "AudioGenie3.dll" Alias "GetMD5ValueFromFile" (ByVal FileName As Long) As String
' AAC Methods
Private Declare Function AACGetBitRateTypeW Lib "AudioGenie3.dll" Alias "AACGetBitRateType" () As String
Private Declare Function AACGetHeaderTypeW Lib "AudioGenie3.dll" Alias "AACGetHeaderType" () As String
Private Declare Function AACGetProfileW Lib "AudioGenie3.dll" Alias "AACGetProfile" () As String
' APE Methods
Private Declare Function APEExistsW Lib "AudioGenie3.dll" Alias "APEExists" () As Integer
Private Declare Function APEGetItemKeysW Lib "AudioGenie3.dll" Alias "APEGetItemKeys" () As String
Private Declare Function APEGetSizeW Lib "AudioGenie3.dll" Alias "APEGetSize" () As Long
Private Declare Function APEGetVersionW Lib "AudioGenie3.dll" Alias "APEGetVersion" () As String
Private Declare Function APERemoveTagFromFileW Lib "AudioGenie3.dll" Alias "APERemoveTagFromFile" (ByVal FileName As Long) As Integer
Private Declare Function APERemoveTagW Lib "AudioGenie3.dll" Alias "APERemoveTag" () As Integer
Private Declare Function APESaveChangesToFileW Lib "AudioGenie3.dll" Alias "APESaveChangesToFile" (ByVal FileName As Long) As Integer
Private Declare Function APESaveChangesW Lib "AudioGenie3.dll" Alias "APESaveChanges" () As Integer
' APE Properties
Private Declare Function GetAPEAlbumW Lib "AudioGenie3.dll" Alias "GetAPEAlbum" () As String
Private Declare Function GetAPEArtistW Lib "AudioGenie3.dll" Alias "GetAPEArtist" () As String
Private Declare Function GetAPECommentW Lib "AudioGenie3.dll" Alias "GetAPEComment" () As String
Private Declare Function GetAPECopyrightW Lib "AudioGenie3.dll" Alias "GetAPECopyright" () As String
Private Declare Function GetAPEGenreW Lib "AudioGenie3.dll" Alias "GetAPEGenre" () As String
Private Declare Function GetAPETitleW Lib "AudioGenie3.dll" Alias "GetAPETitle" () As String
Private Declare Function GetAPETrackW Lib "AudioGenie3.dll" Alias "GetAPETrack" () As String
Private Declare Function GetAPEUserItemW Lib "AudioGenie3.dll" Alias "GetAPEUserItem" (ByVal Key As Long) As String
Private Declare Function GetAPEYearW Lib "AudioGenie3.dll" Alias "GetAPEYear" () As String
Private Declare Sub SetAPEAlbumW Lib "AudioGenie3.dll" Alias "SetAPEAlbum" (ByVal NewValue As Long)
Private Declare Sub SetAPEArtistW Lib "AudioGenie3.dll" Alias "SetAPEArtist" (ByVal NewValue As Long)
Private Declare Sub SetAPECommentW Lib "AudioGenie3.dll" Alias "SetAPEComment" (ByVal NewValue As Long)
Private Declare Sub SetAPECopyrightW Lib "AudioGenie3.dll" Alias "SetAPECopyright" (ByVal NewValue As Long)
Private Declare Sub SetAPEGenreW Lib "AudioGenie3.dll" Alias "SetAPEGenre" (ByVal NewValue As Long)
Private Declare Sub SetAPETitleW Lib "AudioGenie3.dll" Alias "SetAPETitle" (ByVal NewValue As Long)
Private Declare Sub SetAPETrackW Lib "AudioGenie3.dll" Alias "SetAPETrack" (ByVal NewValue As Long)
Private Declare Sub SetAPEUserItemW Lib "AudioGenie3.dll" Alias "SetAPEUserItem" (ByVal Key As Long, ByVal NewValue As Long)
Private Declare Sub SetAPEYearW Lib "AudioGenie3.dll" Alias "SetAPEYear" (ByVal NewValue As Long)
' AUDIO Methods
Private Declare Function AUDIOAnalyzeFileW Lib "AudioGenie3.dll" Alias "AUDIOAnalyzeFile" (ByVal FileName As Long) As Integer
Private Declare Function AUDIOFormatIsValidW Lib "AudioGenie3.dll" Alias "AUDIOFormatIsValid" () As Integer
Private Declare Function AUDIOGetBitrateW Lib "AudioGenie3.dll" Alias "AUDIOGetBitrate" () As Long
Private Declare Function AUDIOGetChannelModeW Lib "AudioGenie3.dll" Alias "AUDIOGetChannelMode" () As String
Private Declare Function AUDIOGetChannelsW Lib "AudioGenie3.dll" Alias "AUDIOGetChannels" () As Long
Private Declare Function AUDIOGetDurationW Lib "AudioGenie3.dll" Alias "AUDIOGetDuration" () As Single
Private Declare Function AUDIOGetDurationMillisW Lib "AudioGenie3.dll" Alias "AUDIOGetDurationMillis" () As Long
Private Declare Function AUDIOGetFileSizeW Lib "AudioGenie3.dll" Alias "AUDIOGetFileSize" () As Long
Private Declare Function AUDIOGetLastErrorNumberW Lib "AudioGenie3.dll" Alias "AUDIOGetLastErrorNumber" () As Long
Private Declare Function AUDIOGetLastErrorTextW Lib "AudioGenie3.dll" Alias "AUDIOGetLastErrorText" () As String
Private Declare Function AUDIOGetLastFileW Lib "AudioGenie3.dll" Alias "AUDIOGetLastFile" () As String
Private Declare Function AUDIOGetSampleRateW Lib "AudioGenie3.dll" Alias "AUDIOGetSampleRate" () As Long
Private Declare Function AUDIOGetVersionW Lib "AudioGenie3.dll" Alias "AUDIOGetVersion" () As String
Private Declare Function AUDIOSaveChangesToFileW Lib "AudioGenie3.dll" Alias "AUDIOSaveChangesToFile" (ByVal FileName As Long) As Integer
Private Declare Function AUDIOSaveChangesW Lib "AudioGenie3.dll" Alias "AUDIOSaveChanges" () As Integer
Private Declare Function AUDIOGetMD5ValueW Lib "AudioGenie3.dll" Alias "AUDIOGetMD5Value" () As String
' AUDIO Properties
Private Declare Function GetAUDIOAlbumW Lib "AudioGenie3.dll" Alias "GetAUDIOAlbum" () As String
Private Declare Function GetAUDIOArtistW Lib "AudioGenie3.dll" Alias "GetAUDIOArtist" () As String
Private Declare Function GetAUDIOCommentW Lib "AudioGenie3.dll" Alias "GetAUDIOComment" () As String
Private Declare Function GetAUDIOGenreW Lib "AudioGenie3.dll" Alias "GetAUDIOGenre" () As String
Private Declare Function GetAUDIOTitleW Lib "AudioGenie3.dll" Alias "GetAUDIOTitle" () As String
Private Declare Function GetAUDIOTrackW Lib "AudioGenie3.dll" Alias "GetAUDIOTrack" () As String
Private Declare Function GetAUDIOYearW Lib "AudioGenie3.dll" Alias "GetAUDIOYear" () As String
Private Declare Sub SetAUDIOAlbumW Lib "AudioGenie3.dll" Alias "SetAUDIOAlbum" (ByVal NewValue As Long)
Private Declare Sub SetAUDIOArtistW Lib "AudioGenie3.dll" Alias "SetAUDIOArtist" (ByVal NewValue As Long)
Private Declare Sub SetAUDIOCommentW Lib "AudioGenie3.dll" Alias "SetAUDIOComment" (ByVal NewValue As Long)
Private Declare Sub SetAUDIOGenreW Lib "AudioGenie3.dll" Alias "SetAUDIOGenre" (ByVal NewValue As Long)
Private Declare Sub SetAUDIOTitleW Lib "AudioGenie3.dll" Alias "SetAUDIOTitle" (ByVal NewValue As Long)
Private Declare Sub SetAUDIOTrackW Lib "AudioGenie3.dll" Alias "SetAUDIOTrack" (ByVal NewValue As Long)
Private Declare Sub SetAUDIOYearW Lib "AudioGenie3.dll" Alias "SetAUDIOYear" (ByVal NewValue As Long)
' FLAC Methods
Private Declare Function FLACGetBitsPerSampleW Lib "AudioGenie3.dll" Alias "FLACGetBitsPerSample" () As Long
Private Declare Function FLACGetCompressionRatioW Lib "AudioGenie3.dll" Alias "FLACGetCompressionRatio" () As Single
Private Declare Function FLACGetItemKeysW Lib "AudioGenie3.dll" Alias "FLACGetItemKeys" () As String
Private Declare Function FLACGetMaxBlockSizeW Lib "AudioGenie3.dll" Alias "FLACGetMaxBlockSize" () As Long
Private Declare Function FLACGetMaxFrameSizeW Lib "AudioGenie3.dll" Alias "FLACGetMaxFrameSize" () As Long
Private Declare Function FLACGetMinBlockSizeW Lib "AudioGenie3.dll" Alias "FLACGetMinBlockSize" () As Long
Private Declare Function FLACGetMinFrameSizeW Lib "AudioGenie3.dll" Alias "FLACGetMinFrameSize" () As Long
Private Declare Function FLACGetSamplesW Lib "AudioGenie3.dll" Alias "FLACGetSamples" () As Long
Private Declare Function FLACSaveChangesToFileW Lib "AudioGenie3.dll" Alias "FLACSaveChangesToFile" (ByVal FileName As Long) As Integer
Private Declare Function FLACSaveChangesW Lib "AudioGenie3.dll" Alias "FLACSaveChanges" () As Integer
' FLAC Properties
Private Declare Function GetFLACAlbumW Lib "AudioGenie3.dll" Alias "GetFLACAlbum" () As String
Private Declare Function GetFLACArtistW Lib "AudioGenie3.dll" Alias "GetFLACArtist" () As String
Private Declare Function GetFLACCommentW Lib "AudioGenie3.dll" Alias "GetFLACComment" () As String
Private Declare Function GetFLACContactW Lib "AudioGenie3.dll" Alias "GetFLACContact" () As String
Private Declare Function GetFLACCopyrightW Lib "AudioGenie3.dll" Alias "GetFLACCopyright" () As String
Private Declare Function GetFLACDateW Lib "AudioGenie3.dll" Alias "GetFLACDate" () As String
Private Declare Function GetFLACDescriptionW Lib "AudioGenie3.dll" Alias "GetFLACDescription" () As String
Private Declare Function GetFLACGenreW Lib "AudioGenie3.dll" Alias "GetFLACGenre" () As String
Private Declare Function GetFLACISRCW Lib "AudioGenie3.dll" Alias "GetFLACISRC" () As String
Private Declare Function GetFLACLicenseW Lib "AudioGenie3.dll" Alias "GetFLACLicense" () As String
Private Declare Function GetFLACLocationW Lib "AudioGenie3.dll" Alias "GetFLACLocation" () As String
Private Declare Function GetFLACOrganizationW Lib "AudioGenie3.dll" Alias "GetFLACOrganization" () As String
Private Declare Function GetFLACPerformerW Lib "AudioGenie3.dll" Alias "GetFLACPerformer" () As String
Private Declare Function GetFLACTitleW Lib "AudioGenie3.dll" Alias "GetFLACTitle" () As String
Private Declare Function GetFLACTrackW Lib "AudioGenie3.dll" Alias "GetFLACTrack" () As String
Private Declare Function GetFLACUserItemW Lib "AudioGenie3.dll" Alias "GetFLACUserItem" (ByVal ItemKey As Long) As String
Private Declare Function GetFLACVendorW Lib "AudioGenie3.dll" Alias "GetFLACVendor" () As String
Private Declare Function GetFLACVersionW Lib "AudioGenie3.dll" Alias "GetFLACVersion" () As String
Private Declare Sub SetFLACAlbumW Lib "AudioGenie3.dll" Alias "SetFLACAlbum" (ByVal NewValue As Long)
Private Declare Sub SetFLACArtistW Lib "AudioGenie3.dll" Alias "SetFLACArtist" (ByVal NewValue As Long)
Private Declare Sub SetFLACCommentW Lib "AudioGenie3.dll" Alias "SetFLACComment" (ByVal NewValue As Long)
Private Declare Sub SetFLACContactW Lib "AudioGenie3.dll" Alias "SetFLACContact" (ByVal NewValue As Long)
Private Declare Sub SetFLACCopyrightW Lib "AudioGenie3.dll" Alias "SetFLACCopyright" (ByVal NewValue As Long)
Private Declare Sub SetFLACDateW Lib "AudioGenie3.dll" Alias "SetFLACDate" (ByVal NewValue As Long)
Private Declare Sub SetFLACDescriptionW Lib "AudioGenie3.dll" Alias "SetFLACDescription" (ByVal NewValue As Long)
Private Declare Sub SetFLACGenreW Lib "AudioGenie3.dll" Alias "SetFLACGenre" (ByVal NewValue As Long)
Private Declare Sub SetFLACISRCW Lib "AudioGenie3.dll" Alias "SetFLACISRC" (ByVal NewValue As Long)
Private Declare Sub SetFLACLicenseW Lib "AudioGenie3.dll" Alias "SetFLACLicense" (ByVal NewValue As Long)
Private Declare Sub SetFLACLocationW Lib "AudioGenie3.dll" Alias "SetFLACLocation" (ByVal NewValue As Long)
Private Declare Sub SetFLACOrganizationW Lib "AudioGenie3.dll" Alias "SetFLACOrganization" (ByVal NewValue As Long)
Private Declare Sub SetFLACPerformerW Lib "AudioGenie3.dll" Alias "SetFLACPerformer" (ByVal NewValue As Long)
Private Declare Sub SetFLACTitleW Lib "AudioGenie3.dll" Alias "SetFLACTitle" (ByVal NewValue As Long)
Private Declare Sub SetFLACTrackW Lib "AudioGenie3.dll" Alias "SetFLACTrack" (ByVal NewValue As Long)
Private Declare Sub SetFLACUserItemW Lib "AudioGenie3.dll" Alias "SetFLACUserItem" (ByVal ItemKey As Long, ByVal NewValue As Long)
Private Declare Sub SetFLACVendorW Lib "AudioGenie3.dll" Alias "SetFLACVendor" (ByVal NewValue As Long)
Private Declare Sub SetFLACVersionW Lib "AudioGenie3.dll" Alias "SetFLACVersion" (ByVal NewValue As Long)
' ID3v1 Methods
Private Declare Function ID3V1ExistsW Lib "AudioGenie3.dll" Alias "ID3V1Exists" () As Integer
Private Declare Function ID3V1GetGenreItemW Lib "AudioGenie3.dll" Alias "ID3V1GetGenreItem" (ByVal number As Integer) As String
Private Declare Function ID3V1GetGenresW Lib "AudioGenie3.dll" Alias "ID3V1GetGenres" () As Integer
Private Declare Function ID3V1GetVersionW Lib "AudioGenie3.dll" Alias "ID3V1GetVersion" () As String
Private Declare Function ID3V1RemoveTagFromFileW Lib "AudioGenie3.dll" Alias "ID3V1RemoveTagFromFile" (ByVal FileName As Long) As Integer
Private Declare Function ID3V1RemoveTagW Lib "AudioGenie3.dll" Alias "ID3V1RemoveTag" () As Integer
Private Declare Function ID3V1SaveChangesToFileW Lib "AudioGenie3.dll" Alias "ID3V1SaveChangesToFile" (ByVal FileName As Long) As Integer
Private Declare Function ID3V1SaveChangesW Lib "AudioGenie3.dll" Alias "ID3V1SaveChanges" () As Integer
' ID3v1 Properties
Private Declare Function GetID3V1AlbumW Lib "AudioGenie3.dll" Alias "GetID3V1Album" () As String
Private Declare Function GetID3V1ArtistW Lib "AudioGenie3.dll" Alias "GetID3V1Artist" () As String
Private Declare Function GetID3V1CommentW Lib "AudioGenie3.dll" Alias "GetID3V1Comment" () As String
Private Declare Function GetID3V1GenreW Lib "AudioGenie3.dll" Alias "GetID3V1Genre" () As String
Private Declare Function GetID3V1GenreIDW Lib "AudioGenie3.dll" Alias "GetID3V1GenreID" () As Integer
Private Declare Function GetID3V1TitleW Lib "AudioGenie3.dll" Alias "GetID3V1Title" () As String
Private Declare Function GetID3V1TrackW Lib "AudioGenie3.dll" Alias "GetID3V1Track" () As String
Private Declare Function GetID3V1YearW Lib "AudioGenie3.dll" Alias "GetID3V1Year" () As String
Private Declare Sub SetID3V1AlbumW Lib "AudioGenie3.dll" Alias "SetID3V1Album" (ByVal NewValue As Long)
Private Declare Sub SetID3V1ArtistW Lib "AudioGenie3.dll" Alias "SetID3V1Artist" (ByVal NewValue As Long)
Private Declare Sub SetID3V1CommentW Lib "AudioGenie3.dll" Alias "SetID3V1Comment" (ByVal NewValue As Long)
Private Declare Sub SetID3V1GenreW Lib "AudioGenie3.dll" Alias "SetID3V1Genre" (ByVal NewValue As Long)
Private Declare Sub SetID3V1GenreIDW Lib "AudioGenie3.dll" Alias "SetID3V1GenreID" (ByVal NewValue As Integer)
Private Declare Sub SetID3V1TitleW Lib "AudioGenie3.dll" Alias "SetID3V1Title" (ByVal NewValue As Long)
Private Declare Sub SetID3V1TrackW Lib "AudioGenie3.dll" Alias "SetID3V1Track" (ByVal NewValue As Long)
Private Declare Sub SetID3V1YearW Lib "AudioGenie3.dll" Alias "SetID3V1Year" (ByVal NewValue As Long)
' ID3v2 Methods
Private Declare Function ID3V2AddCommentW Lib "AudioGenie3.dll" Alias "ID3V2AddComment" (ByVal Language As Long, ByVal Description As Long, ByVal Text As Long) As Integer
Private Declare Function ID3V2AddLyricW Lib "AudioGenie3.dll" Alias "ID3V2AddLyric" (ByVal Language As Long, ByVal Description As Long, ByVal Text As Long) As Integer
Private Declare Function ID3V2CopyPictureArrayW Lib "AudioGenie3.dll" Alias "ID3V2CopyPictureArray" (ByVal firstPos As Long, ByVal maxLength As Long, ByVal Index As Integer) As Long
Private Declare Function ID3V2AddPictureArrayW Lib "AudioGenie3.dll" Alias "ID3V2AddPictureArray" (ByVal firstPos As Long, ByVal length As Long, ByVal Description As Long, ByVal PictureType As Integer) As Integer
Private Declare Function ID3V2AddPictureFileW Lib "AudioGenie3.dll" Alias "ID3V2AddPictureFile" (ByVal Language As Long, ByVal Description As Long, ByVal Text As Long) As Integer
Private Declare Function ID3V2AddPopularimeterW Lib "AudioGenie3.dll" Alias "ID3V2AddPopularimeter" (ByVal Email As Long, ByVal Rating As Integer, ByVal Counter As Long) As Integer
Private Declare Function ID3V2AddSyncLyricW Lib "AudioGenie3.dll" Alias "ID3V2AddSyncLyric" (ByVal Language As Long, ByVal Description As Long, ByVal Text As Long, ByVal ContentType As Integer, ByVal TimeStampFormat As Integer) As Integer
Private Declare Function ID3V2AddUserFrameW Lib "AudioGenie3.dll" Alias "ID3V2AddUserFrame" (ByVal Language As Long, ByVal Text As Long) As Integer
Private Declare Function ID3V2AddUserTextW Lib "AudioGenie3.dll" Alias "ID3V2AddUserText" (ByVal Description As Long, ByVal Text As Long) As Integer
Private Declare Function ID3V2AddUserURLW Lib "AudioGenie3.dll" Alias "ID3V2AddUserURL" (ByVal Description As Long, ByVal URL As Long) As Integer
Private Declare Function ID3V2DeleteAllFramesW Lib "AudioGenie3.dll" Alias "ID3V2DeleteAllFrames" (ByVal ID As Long) As Integer
Private Declare Function ID3V2DeleteSelectedFrameW Lib "AudioGenie3.dll" Alias "ID3V2DeleteSelectedFrame" (ByVal ID As Long, ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetFrameCountW Lib "AudioGenie3.dll" Alias "ID3V2GetFrameCount" (ByVal ID As Long) As Integer
Private Declare Function ID3V2ExistsW Lib "AudioGenie3.dll" Alias "ID3V2Exists" () As Integer
Private Declare Function ID3V2GetCommentW Lib "AudioGenie3.dll" Alias "ID3V2GetComment" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetCommentDescriptionW Lib "AudioGenie3.dll" Alias "ID3V2GetCommentDescription" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetCommentLanguageW Lib "AudioGenie3.dll" Alias "ID3V2GetCommentLanguage" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetLyricW Lib "AudioGenie3.dll" Alias "ID3V2GetLyric" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetLyricDescriptionW Lib "AudioGenie3.dll" Alias "ID3V2GetLyricDescription" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetLyricLanguageW Lib "AudioGenie3.dll" Alias "ID3V2GetLyricLanguage" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetPictureW Lib "AudioGenie3.dll" Alias "ID3V2GetPicture" (ByVal Index As Integer, ByVal FileName As Long) As Integer
Private Declare Function ID3V2GetPictureArrayW Lib "AudioGenie3.dll" Alias "ID3V2GetPictureArray" (ByVal Index As Integer, arr() As Byte) As Integer
Private Declare Function ID3V2GetPictureDescriptionW Lib "AudioGenie3.dll" Alias "ID3V2GetPictureDescription" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetPictureMimeW Lib "AudioGenie3.dll" Alias "ID3V2GetPictureMime" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetPictureTypeW Lib "AudioGenie3.dll" Alias "ID3V2GetPictureType" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetPopularimeterCounterW Lib "AudioGenie3.dll" Alias "ID3V2GetPopularimeterCounter" (ByVal Index As Integer) As Long
Private Declare Function ID3V2GetPopularimeterEmailW Lib "AudioGenie3.dll" Alias "ID3V2GetPopularimeterEmail" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetPopularimeterRatingW Lib "AudioGenie3.dll" Alias "ID3V2GetPopularimeterRating" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetSizeW Lib "AudioGenie3.dll" Alias "ID3V2GetSize" () As Long
Private Declare Function ID3V2GetSyncLyricW Lib "AudioGenie3.dll" Alias "ID3V2GetSyncLyric" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetSyncLyricContentTypeW Lib "AudioGenie3.dll" Alias "ID3V2GetSyncLyricContentType" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetSyncLyricDescriptionW Lib "AudioGenie3.dll" Alias "ID3V2GetSyncLyricDescription" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetSyncLyricLanguageW Lib "AudioGenie3.dll" Alias "ID3V2GetSyncLyricLanguage" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetSyncLyricTimeFormatW Lib "AudioGenie3.dll" Alias "ID3V2GetSyncLyricTimeFormat" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetUserFrameW Lib "AudioGenie3.dll" Alias "ID3V2GetUserFrame" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetUserFrameLanguageW Lib "AudioGenie3.dll" Alias "ID3V2GetUserFrameLanguage" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetUserTextW Lib "AudioGenie3.dll" Alias "ID3V2GetUserText" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetUserTextDescriptionW Lib "AudioGenie3.dll" Alias "ID3V2GetUserTextDescription" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetUserURLW Lib "AudioGenie3.dll" Alias "ID3V2GetUserURL" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetUserURLDescriptionW Lib "AudioGenie3.dll" Alias "ID3V2GetUserURLDescription" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetVersionW Lib "AudioGenie3.dll" Alias "ID3V2GetVersion" () As String
Private Declare Function ID3V2RemoveTagFromFileW Lib "AudioGenie3.dll" Alias "ID3V2RemoveTagFromFile" (ByVal FileName As Long) As Integer
Private Declare Function ID3V2RemoveTagW Lib "AudioGenie3.dll" Alias "ID3V2RemoveTag" () As Integer
Private Declare Function ID3V2SaveChangesToFileW Lib "AudioGenie3.dll" Alias "ID3V2SaveChangesToFile" (ByVal FileName As Long) As Integer
Private Declare Function ID3V2SaveChangesW Lib "AudioGenie3.dll" Alias "ID3V2SaveChanges" () As Integer
Private Declare Function ID3V2SetCommentW Lib "AudioGenie3.dll" Alias "ID3V2SetComment" (ByVal Index As Integer, ByVal Language As Long, ByVal Description As Long, ByVal Text As Long) As Integer
Private Declare Function ID3V2SetFormatAndEncodingW Lib "AudioGenie3.dll" Alias "ID3V2SetFormatAndEncoding" (ByVal format As Integer, ByVal encoding As Integer) As Integer
Private Declare Function ID3V2SetLyricW Lib "AudioGenie3.dll" Alias "ID3V2SetLyric" (ByVal Index As Integer, ByVal Language As Long, ByVal Description As Long, ByVal Text As Long) As Integer
Private Declare Function ID3V2SetPictureW Lib "AudioGenie3.dll" Alias "ID3V2SetPicture" (ByVal FileName As Long, ByVal Description As Long, ByVal Index As Integer, ByVal PictureType As Integer, ByVal asLink As Integer) As Ausfuehrung
Private Declare Function ID3V2SetPictureArrayW Lib "AudioGenie3.dll" Alias "ID3V2SetPictureArray" (arr() As Byte, ByVal Description As Long, ByVal Index As Integer, ByVal PictureType As Integer) As Integer
Private Declare Function ID3V2SetPopularimeterW Lib "AudioGenie3.dll" Alias "ID3V2SetPopularimeter" (ByVal Index As Integer, ByVal Email As Long, ByVal Rating As Integer, ByVal Counter As Long) As Integer
Private Declare Function ID3V2SetSyncLyricW Lib "AudioGenie3.dll" Alias "ID3V2SetSyncLyric" (ByVal Index As Integer, ByVal Language As Long, ByVal Description As Long, ByVal Text As Long, ByVal ContentType As Integer, ByVal TimeStampFormat As Integer) As Integer
Private Declare Function ID3V2SetUserFrameW Lib "AudioGenie3.dll" Alias "ID3V2SetUserFrame" (ByVal Index As Integer, ByVal Language As Long, ByVal Text As Long) As Integer
Private Declare Function ID3V2SetUserTextW Lib "AudioGenie3.dll" Alias "ID3V2SetUserText" (ByVal Index As Integer, ByVal Description As Long, ByVal Text As Long) As Integer
Private Declare Function ID3V2SetUserURLW Lib "AudioGenie3.dll" Alias "ID3V2SetUserURL" (ByVal Index As Integer, ByVal Description As Long, ByVal Text As Long) As Integer
Private Declare Sub ID3V2SetTextFormatToUnicodeW Lib "AudioGenie3.dll" Alias "ID3V2SetTextFormatToUnicode" ()
Private Declare Sub ID3V2SetTextFormatToAnsiW Lib "AudioGenie3.dll" Alias "ID3V2SetTextFormatToAnsi" ()
' ID3v2 Properties
Private Declare Function GetID3V2TextFrameW Lib "AudioGenie3.dll" Alias "GetID3V2TextFrame" (ByVal FrameID As Long) As String
Private Declare Function GetID3V2URLFrameW Lib "AudioGenie3.dll" Alias "GetID3V2URLFrame" (ByVal FrameID As Long) As String
Private Declare Function GetID3V2PlayCounterW Lib "AudioGenie3.dll" Alias "GetID3V2PlayCounter" () As Long
Private Declare Function GetID3V2GenreW Lib "AudioGenie3.dll" Alias "GetID3V2Genre" () As String
Private Declare Sub SetID3V2TextFrameW Lib "AudioGenie3.dll" Alias "SetID3V2TextFrame" (ByVal FrameID As Long, ByVal NewValue As Long)
Private Declare Sub SetID3V2URLFrameW Lib "AudioGenie3.dll" Alias "SetID3V2URLFrame" (ByVal FrameID As Long, ByVal NewValue As Long)
Private Declare Sub SetID3V2GenreW Lib "AudioGenie3.dll" Alias "SetID3V2Genre" (ByVal NewValue As Long)
Private Declare Sub SetID3V2PlayCounterW Lib "AudioGenie3.dll" Alias "SetID3V2PlayCounter" (ByVal nNewValue As Long)
' Lyrics Methods
Private Declare Function LYRICSExistsW Lib "AudioGenie3.dll" Alias "LYRICSExists" () As Integer
Private Declare Function LYRICSGetSizeW Lib "AudioGenie3.dll" Alias "LYRICSGetSize" () As Long
Private Declare Function LYRICSGetStartPositionW Lib "AudioGenie3.dll" Alias "LYRICSGetStartPosition" () As Long
Private Declare Function LYRICSGetVersionW Lib "AudioGenie3.dll" Alias "LYRICSGetVersion" () As String
Private Declare Function LYRICSRemoveTagFromFileW Lib "AudioGenie3.dll" Alias "LYRICSRemoveTagFromFile" (ByVal FileName As Long) As Integer
Private Declare Function LYRICSRemoveTagW Lib "AudioGenie3.dll" Alias "LYRICSRemoveTag" () As Integer
Private Declare Function LYRICSSaveChangesToFileW Lib "AudioGenie3.dll" Alias "LYRICSSaveChangesToFile" (ByVal FileName As Long) As Integer
Private Declare Function LYRICSSaveChangesW Lib "AudioGenie3.dll" Alias "LYRICSSaveChanges" () As Integer
' Lyrics Properties
Private Declare Function GetLYRICSAlbumW Lib "AudioGenie3.dll" Alias "GetLYRICSAlbum" () As String
Private Declare Function GetLYRICSArtistW Lib "AudioGenie3.dll" Alias "GetLYRICSArtist" () As String
Private Declare Function GetLYRICSAuthorW Lib "AudioGenie3.dll" Alias "GetLYRICSAuthor" () As String
Private Declare Function GetLYRICSGenreW Lib "AudioGenie3.dll" Alias "GetLYRICSGenre" () As String
Private Declare Function GetLYRICSImageLinkW Lib "AudioGenie3.dll" Alias "GetLYRICSImageLink" () As String
Private Declare Function GetLYRICSIndicationW Lib "AudioGenie3.dll" Alias "GetLYRICSIndication" () As String
Private Declare Function GetLYRICSInformationW Lib "AudioGenie3.dll" Alias "GetLYRICSInformation" () As String
Private Declare Function GetLYRICSLyricsW Lib "AudioGenie3.dll" Alias "GetLYRICSLyrics" () As String
Private Declare Function GetLYRICSTitleW Lib "AudioGenie3.dll" Alias "GetLYRICSTitle" () As String
Private Declare Sub SetLYRICSAlbumW Lib "AudioGenie3.dll" Alias "SetLYRICSAlbum" (ByVal NewValue As Long)
Private Declare Sub SetLYRICSArtistW Lib "AudioGenie3.dll" Alias "SetLYRICSArtist" (ByVal NewValue As Long)
Private Declare Sub SetLYRICSAuthorW Lib "AudioGenie3.dll" Alias "SetLYRICSAuthor" (ByVal NewValue As Long)
Private Declare Sub SetLYRICSGenreW Lib "AudioGenie3.dll" Alias "SetLYRICSGenre" (ByVal NewValue As Long)
Private Declare Sub SetLYRICSImageLinkW Lib "AudioGenie3.dll" Alias "SetLYRICSImageLink" (ByVal NewValue As Long)
Private Declare Sub SetLYRICSIndicationW Lib "AudioGenie3.dll" Alias "SetLYRICSIndication" (ByVal NewValue As Long)
Private Declare Sub SetLYRICSInformationW Lib "AudioGenie3.dll" Alias "SetLYRICSInformation" (ByVal NewValue As Long)
Private Declare Sub SetLYRICSLyricsW Lib "AudioGenie3.dll" Alias "SetLYRICSLyrics" (ByVal NewValue As Long)
Private Declare Sub SetLYRICSTitleW Lib "AudioGenie3.dll" Alias "SetLYRICSTitle" (ByVal NewValue As Long)
' Monkey Methods
Private Declare Function MONKEYGetBitsW Lib "AudioGenie3.dll" Alias "MONKEYGetBits" () As Integer
Private Declare Function MONKEYGetCompressionW Lib "AudioGenie3.dll" Alias "MONKEYGetCompression" () As String
Private Declare Function MONKEYGetCompressionRatioW Lib "AudioGenie3.dll" Alias "MONKEYGetCompressionRatio" () As Single
Private Declare Function MONKEYGetFramesW Lib "AudioGenie3.dll" Alias "MONKEYGetFrames" () As Long
Private Declare Function MONKEYGetPeakW Lib "AudioGenie3.dll" Alias "MONKEYGetPeak" () As Single
Private Declare Function MONKEYGetSamplesW Lib "AudioGenie3.dll" Alias "MONKEYGetSamples" () As Long
Private Declare Function MONKEYGetSamplesPerFrameW Lib "AudioGenie3.dll" Alias "MONKEYGetSamplesPerFrame" () As Long
' MP4 Methods
Private Declare Function MP4GetPictureCountW Lib "AudioGenie3.dll" Alias "MP4GetPictureCount" () As Long
Private Declare Function MP4LoadPictureW Lib "AudioGenie3.dll" Alias "MP4LoadPicture" (ByVal FileName As Long, ByVal Index As Integer) As Integer
Private Declare Function MP4GetPictureArrayW Lib "AudioGenie3.dll" Alias "MP4GetPictureArray" (ByVal Index As Integer, arr() As Byte) As Integer
' MP4 Properties
Private Declare Function GetMP4AlbumW Lib "AudioGenie3.dll" Alias "GetMP4Album" () As String
Private Declare Function GetMP4ArtistW Lib "AudioGenie3.dll" Alias "GetMP4Artist" () As String
Private Declare Function GetMP4AuthorW Lib "AudioGenie3.dll" Alias "GetMP4Author" () As String
Private Declare Function GetMP4BeatsPerMinuteW Lib "AudioGenie3.dll" Alias "GetMP4BeatsPerMinute" () As String
Private Declare Function GetMP4CDSetNumberW Lib "AudioGenie3.dll" Alias "GetMP4CDSetNumber" () As String
Private Declare Function GetMP4CommentW Lib "AudioGenie3.dll" Alias "GetMP4Comment" () As String
Private Declare Function GetMP4CompilationW Lib "AudioGenie3.dll" Alias "GetMP4Compilation" () As String
Private Declare Function GetMP4ComposerW Lib "AudioGenie3.dll" Alias "GetMP4Composer" () As String
Private Declare Function GetMP4CopyrightW Lib "AudioGenie3.dll" Alias "GetMP4Copyright" () As String
Private Declare Function GetMP4DateW Lib "AudioGenie3.dll" Alias "GetMP4Date" () As String
Private Declare Function GetMP4DescriptionW Lib "AudioGenie3.dll" Alias "GetMP4Description" () As String
Private Declare Function GetMP4DirectorW Lib "AudioGenie3.dll" Alias "GetMP4Director" () As String
Private Declare Function GetMP4DisclaimerW Lib "AudioGenie3.dll" Alias "GetMP4Disclaimer" () As String
Private Declare Function GetMP4EncoderW Lib "AudioGenie3.dll" Alias "GetMP4Encoder" () As String
Private Declare Function GetMP4GenreW Lib "AudioGenie3.dll" Alias "GetMP4Genre" () As String
Private Declare Function GetMP4NetUrlW Lib "AudioGenie3.dll" Alias "GetMP4NetUrl" () As String
Private Declare Function GetMP4OriginalArtistW Lib "AudioGenie3.dll" Alias "GetMP4OriginalArtist" () As String
Private Declare Function GetMP4OriginalFormatW Lib "AudioGenie3.dll" Alias "GetMP4OriginalFormat" () As String
Private Declare Function GetMP4OriginalSourceW Lib "AudioGenie3.dll" Alias "GetMP4OriginalSource" () As String
Private Declare Function GetMP4PerformerW Lib "AudioGenie3.dll" Alias "GetMP4Performer" () As String
Private Declare Function GetMP4ProducerW Lib "AudioGenie3.dll" Alias "GetMP4Producer" () As String
Private Declare Function GetMP4TitleW Lib "AudioGenie3.dll" Alias "GetMP4Title" () As String
Private Declare Function GetMP4TrackW Lib "AudioGenie3.dll" Alias "GetMP4Track" () As String
Private Declare Function GetMP4WarningW Lib "AudioGenie3.dll" Alias "GetMP4Warning" () As String
Private Declare Sub SetMP4AlbumW Lib "AudioGenie3.dll" Alias "SetMP4Album" (ByVal NewValue As Long)
Private Declare Sub SetMP4ArtistW Lib "AudioGenie3.dll" Alias "SetMP4Artist" (ByVal NewValue As Long)
Private Declare Sub SetMP4AuthorW Lib "AudioGenie3.dll" Alias "SetMP4Author" (ByVal NewValue As Long)
Private Declare Sub SetMP4BeatsPerMinuteW Lib "AudioGenie3.dll" Alias "SetMP4BeatsPerMinute" (ByVal NewValue As Long)
Private Declare Sub SetMP4CDSetNumberW Lib "AudioGenie3.dll" Alias "SetMP4CDSetNumber" (ByVal NewValue As Long)
Private Declare Sub SetMP4CommentW Lib "AudioGenie3.dll" Alias "SetMP4Comment" (ByVal NewValue As Long)
Private Declare Sub SetMP4CompilationW Lib "AudioGenie3.dll" Alias "SetMP4Compilation" (ByVal NewValue As Long)
Private Declare Sub SetMP4ComposerW Lib "AudioGenie3.dll" Alias "SetMP4Composer" (ByVal NewValue As Long)
Private Declare Sub SetMP4CopyrightW Lib "AudioGenie3.dll" Alias "SetMP4Copyright" (ByVal NewValue As Long)
Private Declare Sub SetMP4DateW Lib "AudioGenie3.dll" Alias "SetMP4Date" (ByVal NewValue As Long)
Private Declare Sub SetMP4DescriptionW Lib "AudioGenie3.dll" Alias "SetMP4Description" (ByVal NewValue As Long)
Private Declare Sub SetMP4DirectorW Lib "AudioGenie3.dll" Alias "SetMP4Director" (ByVal NewValue As Long)
Private Declare Sub SetMP4DisclaimerW Lib "AudioGenie3.dll" Alias "SetMP4Disclaimer" (ByVal NewValue As Long)
Private Declare Sub SetMP4EncoderW Lib "AudioGenie3.dll" Alias "SetMP4Encoder" (ByVal NewValue As Long)
Private Declare Sub SetMP4GenreW Lib "AudioGenie3.dll" Alias "SetMP4Genre" (ByVal NewValue As Long)
Private Declare Sub SetMP4NetUrlW Lib "AudioGenie3.dll" Alias "SetMP4NetUrl" (ByVal NewValue As Long)
Private Declare Sub SetMP4OriginalArtistW Lib "AudioGenie3.dll" Alias "SetMP4OriginalArtist" (ByVal NewValue As Long)
Private Declare Sub SetMP4OriginalFormatW Lib "AudioGenie3.dll" Alias "SetMP4OriginalFormat" (ByVal NewValue As Long)
Private Declare Sub SetMP4OriginalSourceW Lib "AudioGenie3.dll" Alias "SetMP4OriginalSource" (ByVal NewValue As Long)
Private Declare Sub SetMP4PerformerW Lib "AudioGenie3.dll" Alias "SetMP4Performer" (ByVal NewValue As Long)
Private Declare Sub SetMP4ProducerW Lib "AudioGenie3.dll" Alias "SetMP4Producer" (ByVal NewValue As Long)
Private Declare Sub SetMP4TitleW Lib "AudioGenie3.dll" Alias "SetMP4Title" (ByVal NewValue As Long)
Private Declare Sub SetMP4TrackW Lib "AudioGenie3.dll" Alias "SetMP4Track" (ByVal NewValue As Long)
Private Declare Sub SetMP4WarningW Lib "AudioGenie3.dll" Alias "SetMP4Warning" (ByVal NewValue As Long)
' MPEG Methods
Private Declare Function GetMPEGAnalyzeAllFramesW Lib "AudioGenie3.dll" Alias "GetMPEGAnalyzeAllFrames" () As Integer
Private Declare Sub SetMPEGAnalyzeAllFramesW Lib "AudioGenie3.dll" Alias "SetMPEGAnalyzeAllFrames" (ByVal NewValue As Integer)
' MPEG Properties
Private Declare Function MPEGGetEmphasisW Lib "AudioGenie3.dll" Alias "MPEGGetEmphasis" () As String
Private Declare Function MPEGGetEncoderW Lib "AudioGenie3.dll" Alias "MPEGGetEncoder" () As String
Private Declare Function MPEGGetFramePositionW Lib "AudioGenie3.dll" Alias "MPEGGetFramePosition" () As Long
Private Declare Function MPEGGetFramesW Lib "AudioGenie3.dll" Alias "MPEGGetFrames" () As Long
Private Declare Function MPEGGetFrameSizeW Lib "AudioGenie3.dll" Alias "MPEGGetFrameSize" () As Long
Private Declare Function MPEGGetLayerW Lib "AudioGenie3.dll" Alias "MPEGGetLayer" () As String
Private Declare Function MPEGGetVersionW Lib "AudioGenie3.dll" Alias "MPEGGetVersion" () As String
Private Declare Function MPEGIsCopyrightedW Lib "AudioGenie3.dll" Alias "MPEGIsCopyrighted" () As Integer
Private Declare Function MPEGIsOriginalW Lib "AudioGenie3.dll" Alias "MPEGIsOriginal" () As Integer
Private Declare Function MPEGIsPaddingW Lib "AudioGenie3.dll" Alias "MPEGIsPadding" () As Integer
Private Declare Function MPEGIsPrivateW Lib "AudioGenie3.dll" Alias "MPEGIsPrivate" () As Integer
Private Declare Function MPEGIsProtectedW Lib "AudioGenie3.dll" Alias "MPEGIsProtected" () As Integer
Private Declare Function MPEGIsVBRW Lib "AudioGenie3.dll" Alias "MPEGIsVBR" () As Integer
Private Declare Function MPEGModifyCopyrightBitW Lib "AudioGenie3.dll" Alias "MPEGModifyCopyrightBit" (ByVal FileName As Long, ByVal NewValue As Integer) As Integer
Private Declare Function MPEGModifyOriginalBitW Lib "AudioGenie3.dll" Alias "MPEGModifyOriginalBit" (ByVal FileName As Long, ByVal NewValue As Integer) As Integer
Private Declare Function MPEGModifyPrivateBitW Lib "AudioGenie3.dll" Alias "MPEGModifyPrivateBit" (ByVal FileName As Long, ByVal NewValue As Integer) As Integer
Private Declare Function MPPGetFramesW Lib "AudioGenie3.dll" Alias "MPPGetFrames" () As Long
Private Declare Function MPPGetStreamVersionW Lib "AudioGenie3.dll" Alias "MPPGetStreamVersion" () As Integer
' OGG Methods
Private Declare Function OGGGetBitRateNominalW Lib "AudioGenie3.dll" Alias "OGGGetBitRateNominal" () As Long
Private Declare Function OGGGetItemKeysW Lib "AudioGenie3.dll" Alias "OGGGetItemKeys" () As String
Private Declare Function OGGRemoveTagFromFileW Lib "AudioGenie3.dll" Alias "OGGRemoveTagFromFile" (ByVal FileName As Long) As Integer
Private Declare Function OGGRemoveTagW Lib "AudioGenie3.dll" Alias "OGGRemoveTag" () As Integer
Private Declare Function OGGSaveChangesToFileW Lib "AudioGenie3.dll" Alias "OGGSaveChangesToFile" (ByVal FileName As Long) As Integer
Private Declare Function OGGSaveChangesW Lib "AudioGenie3.dll" Alias "OGGSaveChanges" () As Integer
' OGG Properties
Private Declare Function GetOGGAlbumW Lib "AudioGenie3.dll" Alias "GetOGGAlbum" () As String
Private Declare Function GetOGGArtistW Lib "AudioGenie3.dll" Alias "GetOGGArtist" () As String
Private Declare Function GetOGGCommentW Lib "AudioGenie3.dll" Alias "GetOGGComment" () As String
Private Declare Function GetOggContactW Lib "AudioGenie3.dll" Alias "GetOggContact" () As String
Private Declare Function GetOGGCopyrightW Lib "AudioGenie3.dll" Alias "GetOGGCopyright" () As String
Private Declare Function GetOGGDateW Lib "AudioGenie3.dll" Alias "GetOGGDate" () As String
Private Declare Function GetOGGDescriptionW Lib "AudioGenie3.dll" Alias "GetOGGDescription" () As String
Private Declare Function GetOGGGenreW Lib "AudioGenie3.dll" Alias "GetOGGGenre" () As String
Private Declare Function GetOGGISRCW Lib "AudioGenie3.dll" Alias "GetOGGISRC" () As String
Private Declare Function GetOGGLicenseW Lib "AudioGenie3.dll" Alias "GetOGGLicense" () As String
Private Declare Function GetOGGLocationW Lib "AudioGenie3.dll" Alias "GetOGGLocation" () As String
Private Declare Function GetOGGOrganizationW Lib "AudioGenie3.dll" Alias "GetOGGOrganization" () As String
Private Declare Function GetOGGPerformerW Lib "AudioGenie3.dll" Alias "GetOGGPerformer" () As String
Private Declare Function GetOGGTitleW Lib "AudioGenie3.dll" Alias "GetOGGTitle" () As String
Private Declare Function GetOGGTrackW Lib "AudioGenie3.dll" Alias "GetOGGTrack" () As String
Private Declare Function GetOGGUserItemW Lib "AudioGenie3.dll" Alias "GetOGGUserItem" (ByVal ItemKey As Long) As String
Private Declare Function GetOGGVendorW Lib "AudioGenie3.dll" Alias "GetOGGVendor" () As String
Private Declare Function GetOGGVersionW Lib "AudioGenie3.dll" Alias "GetOGGVersion" () As String
Private Declare Sub SetOGGAlbumW Lib "AudioGenie3.dll" Alias "SetOGGAlbum" (ByVal NewValue As Long)
Private Declare Sub SetOGGArtistW Lib "AudioGenie3.dll" Alias "SetOGGArtist" (ByVal NewValue As Long)
Private Declare Sub SetOGGCommentW Lib "AudioGenie3.dll" Alias "SetOGGComment" (ByVal NewValue As Long)
Private Declare Sub SetOggContactW Lib "AudioGenie3.dll" Alias "SetOggContact" (ByVal NewValue As Long)
Private Declare Sub SetOGGCopyrightW Lib "AudioGenie3.dll" Alias "SetOGGCopyright" (ByVal NewValue As Long)
Private Declare Sub SetOGGDateW Lib "AudioGenie3.dll" Alias "SetOGGDate" (ByVal NewValue As Long)
Private Declare Sub SetOGGDescriptionW Lib "AudioGenie3.dll" Alias "SetOGGDescription" (ByVal NewValue As Long)
Private Declare Sub SetOGGGenreW Lib "AudioGenie3.dll" Alias "SetOGGGenre" (ByVal NewValue As Long)
Private Declare Sub SetOGGISRCW Lib "AudioGenie3.dll" Alias "SetOGGISRC" (ByVal NewValue As Long)
Private Declare Sub SetOGGLicenseW Lib "AudioGenie3.dll" Alias "SetOGGLicense" (ByVal NewValue As Long)
Private Declare Sub SetOGGLocationW Lib "AudioGenie3.dll" Alias "SetOGGLocation" (ByVal NewValue As Long)
Private Declare Sub SetOGGOrganizationW Lib "AudioGenie3.dll" Alias "SetOGGOrganization" (ByVal NewValue As Long)
Private Declare Sub SetOGGPerformerW Lib "AudioGenie3.dll" Alias "SetOGGPerformer" (ByVal NewValue As Long)
Private Declare Sub SetOGGTitleW Lib "AudioGenie3.dll" Alias "SetOGGTitle" (ByVal NewValue As Long)
Private Declare Sub SetOGGTrackW Lib "AudioGenie3.dll" Alias "SetOGGTrack" (ByVal NewValue As Long)
Private Declare Sub SetOGGUserItemW Lib "AudioGenie3.dll" Alias "SetOGGUserItem" (ByVal ItemKey As Long, ByVal NewValue As Long)
Private Declare Sub SetOGGVendorW Lib "AudioGenie3.dll" Alias "SetOGGVendor" (ByVal NewValue As Long)
Private Declare Sub SetOGGVersionW Lib "AudioGenie3.dll" Alias "SetOGGVersion" (ByVal NewValue As Long)
' WAV Methods
Private Declare Function WAVGetBitsPerSampleW Lib "AudioGenie3.dll" Alias "WAVGetBitsPerSample" () As Long
Private Declare Function WAVGetBlockAlignW Lib "AudioGenie3.dll" Alias "WAVGetBlockAlign" () As Long
Private Declare Function WAVGetBytesPerSecondW Lib "AudioGenie3.dll" Alias "WAVGetBytesPerSecond" () As Long
Private Declare Function WAVGetChannelsW Lib "AudioGenie3.dll" Alias "WAVGetChannels" () As Long
Private Declare Function WAVGetFormatW Lib "AudioGenie3.dll" Alias "WAVGetFormat" () As String
Private Declare Function WAVGetFormatIDW Lib "AudioGenie3.dll" Alias "WAVGetFormatID" () As Integer
Private Declare Function WAVGetHeaderSizeW Lib "AudioGenie3.dll" Alias "WAVGetHeaderSize" () As Long
' WAV Properties
Private Declare Function GetWAVArchivalLocationW Lib "AudioGenie3.dll" Alias "GetWAVArchivalLocation" () As String
Private Declare Function GetWAVArtistW Lib "AudioGenie3.dll" Alias "GetWAVArtist" () As String
Private Declare Function GetWAVComissionedW Lib "AudioGenie3.dll" Alias "GetWAVComissioned" () As String
Private Declare Function GetWAVCommentW Lib "AudioGenie3.dll" Alias "GetWAVComment" () As String
Private Declare Function GetWAVCopyrightW Lib "AudioGenie3.dll" Alias "GetWAVCopyright" () As String
Private Declare Function GetWAVCreationDateW Lib "AudioGenie3.dll" Alias "GetWAVCreationDate" () As String
Private Declare Function GetWAVCroppedW Lib "AudioGenie3.dll" Alias "GetWAVCropped" () As String
Private Declare Function GetWAVDimensionW Lib "AudioGenie3.dll" Alias "GetWAVDimension" () As String
Private Declare Function GetWAVDotsPerInchW Lib "AudioGenie3.dll" Alias "GetWAVDotsPerInch" () As String
Private Declare Function GetWAVEngineerW Lib "AudioGenie3.dll" Alias "GetWAVEngineer" () As String
Private Declare Function GetWAVGenreW Lib "AudioGenie3.dll" Alias "GetWAVGenre" () As String
Private Declare Function GetWAVKeywordsW Lib "AudioGenie3.dll" Alias "GetWAVKeywords" () As String
Private Declare Function GetWAVLightnessW Lib "AudioGenie3.dll" Alias "GetWAVLightness" () As String
Private Declare Function GetWAVMediumW Lib "AudioGenie3.dll" Alias "GetWAVMedium" () As String
Private Declare Function GetWAVNameW Lib "AudioGenie3.dll" Alias "GetWAVName" () As String
Private Declare Function GetWAVPaletteSettingW Lib "AudioGenie3.dll" Alias "GetWAVPaletteSetting" () As String
Private Declare Function GetWAVProductW Lib "AudioGenie3.dll" Alias "GetWAVProduct" () As String
Private Declare Function GetWAVSharpnessW Lib "AudioGenie3.dll" Alias "GetWAVSharpness" () As String
Private Declare Function GetWAVSoftwareW Lib "AudioGenie3.dll" Alias "GetWAVSoftware" () As String
Private Declare Function GetWAVSourceW Lib "AudioGenie3.dll" Alias "GetWAVSource" () As String
Private Declare Function GetWAVSourceFormW Lib "AudioGenie3.dll" Alias "GetWAVSourceForm" () As String
Private Declare Function GetWAVSubjectW Lib "AudioGenie3.dll" Alias "GetWAVSubject" () As String
Private Declare Function GetWAVTechnicianW Lib "AudioGenie3.dll" Alias "GetWAVTechnician" () As String
Private Declare Function GetWAVTrackW Lib "AudioGenie3.dll" Alias "GetWAVTrack" () As String
Private Declare Sub SetWAVArchivalLocationW Lib "AudioGenie3.dll" Alias "SetWAVArchivalLocation" (ByVal NewValue As Long)
Private Declare Sub SetWAVArtistW Lib "AudioGenie3.dll" Alias "SetWAVArtist" (ByVal NewValue As Long)
Private Declare Sub SetWAVComissionedW Lib "AudioGenie3.dll" Alias "SetWAVComissioned" (ByVal NewValue As Long)
Private Declare Sub SetWAVCommentW Lib "AudioGenie3.dll" Alias "SetWAVComment" (ByVal NewValue As Long)
Private Declare Sub SetWAVCopyrightW Lib "AudioGenie3.dll" Alias "SetWAVCopyright" (ByVal NewValue As Long)
Private Declare Sub SetWAVCreationDateW Lib "AudioGenie3.dll" Alias "SetWAVCreationDate" (ByVal NewValue As Long)
Private Declare Sub SetWAVCroppedW Lib "AudioGenie3.dll" Alias "SetWAVCropped" (ByVal NewValue As Long)
Private Declare Sub SetWAVDimensionW Lib "AudioGenie3.dll" Alias "SetWAVDimension" (ByVal NewValue As Long)
Private Declare Sub SetWAVDotsPerInchW Lib "AudioGenie3.dll" Alias "SetWAVDotsPerInch" (ByVal NewValue As Long)
Private Declare Sub SetWAVEngineerW Lib "AudioGenie3.dll" Alias "SetWAVEngineer" (ByVal NewValue As Long)
Private Declare Sub SetWAVGenreW Lib "AudioGenie3.dll" Alias "SetWAVGenre" (ByVal NewValue As Long)
Private Declare Sub SetWAVKeywordsW Lib "AudioGenie3.dll" Alias "SetWAVKeywords" (ByVal NewValue As Long)
Private Declare Sub SetWAVLightnessW Lib "AudioGenie3.dll" Alias "SetWAVLightness" (ByVal NewValue As Long)
Private Declare Sub SetWAVMediumW Lib "AudioGenie3.dll" Alias "SetWAVMedium" (ByVal NewValue As Long)
Private Declare Sub SetWAVNameW Lib "AudioGenie3.dll" Alias "SetWAVName" (ByVal NewValue As Long)
Private Declare Sub SetWAVPaletteSettingW Lib "AudioGenie3.dll" Alias "SetWAVPaletteSetting" (ByVal NewValue As Long)
Private Declare Sub SetWAVProductW Lib "AudioGenie3.dll" Alias "SetWAVProduct" (ByVal NewValue As Long)
Private Declare Sub SetWAVSharpnessW Lib "AudioGenie3.dll" Alias "SetWAVSharpness" (ByVal NewValue As Long)
Private Declare Sub SetWAVSoftwareW Lib "AudioGenie3.dll" Alias "SetWAVSoftware" (ByVal NewValue As Long)
Private Declare Sub SetWAVSourceW Lib "AudioGenie3.dll" Alias "SetWAVSource" (ByVal NewValue As Long)
Private Declare Sub SetWAVSourceFormW Lib "AudioGenie3.dll" Alias "SetWAVSourceForm" (ByVal NewValue As Long)
Private Declare Sub SetWAVSubjectW Lib "AudioGenie3.dll" Alias "SetWAVSubject" (ByVal NewValue As Long)
Private Declare Sub SetWAVTechnicianW Lib "AudioGenie3.dll" Alias "SetWAVTechnician" (ByVal NewValue As Long)
Private Declare Sub SetWAVTrackW Lib "AudioGenie3.dll" Alias "SetWAVTrack" (ByVal NewValue As Long)
' WMA Methods
Private Declare Function WMADeletePictureW Lib "AudioGenie3.dll" Alias "WMADeletePicture" (ByVal Index As Integer) As Integer
Private Declare Sub WMADeletePicturesW Lib "AudioGenie3.dll" Alias "WMADeletePictures" ()
Private Declare Function WMAGetItemKeysW Lib "AudioGenie3.dll" Alias "WMAGetItemKeys" () As String
Private Declare Function WMAGetPictureW Lib "AudioGenie3.dll" Alias "WMAGetPicture" (ByVal Index As Integer, ByVal PictureFileName As Long) As Integer
Private Declare Function WMAGetPictureArrayW Lib "AudioGenie3.dll" Alias "WMAGetPictureArray" (ByVal Index As Integer, arr() As Byte) As Integer
Private Declare Function WMAGetPictureDescriptionW Lib "AudioGenie3.dll" Alias "WMAGetPictureDescription" (ByVal Index As Integer) As String
Private Declare Function WMAGetPictureMimeW Lib "AudioGenie3.dll" Alias "WMAGetPictureMime" (ByVal Index As Integer) As String
Private Declare Function WMAGetPicturesW Lib "AudioGenie3.dll" Alias "WMAGetPictures" () As Integer
Private Declare Function WMAGetPictureTypeW Lib "AudioGenie3.dll" Alias "WMAGetPictureType" (ByVal Index As Integer) As Integer
Private Declare Function WMASaveChangesToFileW Lib "AudioGenie3.dll" Alias "WMASaveChangesToFile" (ByVal FileName As Long) As Integer
Private Declare Function WMASaveChangesW Lib "AudioGenie3.dll" Alias "WMASaveChanges" () As Integer
Private Declare Function WMASetPictureW Lib "AudioGenie3.dll" Alias "WMASetPicture" (ByVal PictureFileName As Long, ByVal Description As Long, ByVal Index As Integer, ByVal PictureType As Integer) As Integer
Private Declare Function WMASetPictureArrayW Lib "AudioGenie3.dll" Alias "WMASetPictureArray" (arr() As Byte, ByVal Description As Long, ByVal Index As Integer, ByVal PictureType As Integer) As Integer
' WMA Properties
Private Declare Function GetWMAAlbumW Lib "AudioGenie3.dll" Alias "GetWMAAlbum" () As String
Private Declare Function GetWMAAlbumArtistW Lib "AudioGenie3.dll" Alias "GetWMAAlbumArtist" () As String
Private Declare Function GetWMAArtistW Lib "AudioGenie3.dll" Alias "GetWMAArtist" () As String
Private Declare Function GetWMAAuthorURLW Lib "AudioGenie3.dll" Alias "GetWMAAuthorURL" () As String
Private Declare Function GetWMABeatsPerMinuteW Lib "AudioGenie3.dll" Alias "GetWMABeatsPerMinute" () As String
Private Declare Function GetWMACategoryW Lib "AudioGenie3.dll" Alias "GetWMACategory" () As String
Private Declare Function GetWMACommentW Lib "AudioGenie3.dll" Alias "GetWMAComment" () As String
Private Declare Function GetWMAComposerW Lib "AudioGenie3.dll" Alias "GetWMAComposer" () As String
Private Declare Function GetWMAConductorW Lib "AudioGenie3.dll" Alias "GetWMAConductor" () As String
Private Declare Function GetWMAContentGroupDescriptionW Lib "AudioGenie3.dll" Alias "GetWMAContentGroupDescription" () As String
Private Declare Function GetWMACopyrightW Lib "AudioGenie3.dll" Alias "GetWMACopyright" () As String
Private Declare Function GetWMADistributorW Lib "AudioGenie3.dll" Alias "GetWMADistributor" () As String
Private Declare Function GetWMAEncodedByW Lib "AudioGenie3.dll" Alias "GetWMAEncodedBy" () As String
Private Declare Function GetWMAEncodingSettingsW Lib "AudioGenie3.dll" Alias "GetWMAEncodingSettings" () As String
Private Declare Function GetWMAEncodingTimeW Lib "AudioGenie3.dll" Alias "GetWMAEncodingTime" () As String
Private Declare Function GetWMAGenreW Lib "AudioGenie3.dll" Alias "GetWMAGenre" () As String
Private Declare Function GetWMAInitialKeyW Lib "AudioGenie3.dll" Alias "GetWMAInitialKey" () As String
Private Declare Function GetWMAISRCW Lib "AudioGenie3.dll" Alias "GetWMAISRC" () As String
Private Declare Function GetWMALanguageW Lib "AudioGenie3.dll" Alias "GetWMALanguage" () As String
Private Declare Function GetWMALyricsW Lib "AudioGenie3.dll" Alias "GetWMALyrics" () As String
Private Declare Function GetWMAMoodW Lib "AudioGenie3.dll" Alias "GetWMAMood" () As String
Private Declare Function GetWMAParentalRatingW Lib "AudioGenie3.dll" Alias "GetWMAParentalRating" () As String
Private Declare Function GetWMAPartOfSetW Lib "AudioGenie3.dll" Alias "GetWMAPartOfSet" () As String
Private Declare Function GetWMAPeriodW Lib "AudioGenie3.dll" Alias "GetWMAPeriod" () As String
Private Declare Function GetWMAProducerW Lib "AudioGenie3.dll" Alias "GetWMAProducer" () As String
Private Declare Function GetWMAPromotionURLW Lib "AudioGenie3.dll" Alias "GetWMAPromotionURL" () As String
Private Declare Function GetWMAProviderW Lib "AudioGenie3.dll" Alias "GetWMAProvider" () As String
Private Declare Function GetWMAPublisherW Lib "AudioGenie3.dll" Alias "GetWMAPublisher" () As String
Private Declare Function GetWMARatingW Lib "AudioGenie3.dll" Alias "GetWMARating" () As String
Private Declare Function GetWMATitleW Lib "AudioGenie3.dll" Alias "GetWMATitle" () As String
Private Declare Function GetWMAToolNameW Lib "AudioGenie3.dll" Alias "GetWMAToolName" () As String
Private Declare Function GetWMAToolVersionW Lib "AudioGenie3.dll" Alias "GetWMAToolVersion" () As String
Private Declare Function GetWMATrackNumberW Lib "AudioGenie3.dll" Alias "GetWMATrackNumber" () As String
Private Declare Function GetWMAUserItemW Lib "AudioGenie3.dll" Alias "GetWMAUserItem" (ByVal ItemKey As Long) As String
Private Declare Function GetWMAUserWebURLW Lib "AudioGenie3.dll" Alias "GetWMAUserWebURL" () As String
Private Declare Function GetWMAWriterW Lib "AudioGenie3.dll" Alias "GetWMAWriter" () As String
Private Declare Function GetWMAYearW Lib "AudioGenie3.dll" Alias "GetWMAYear" () As String
Private Declare Sub SetWMAAlbumW Lib "AudioGenie3.dll" Alias "SetWMAAlbum" (ByVal NewValue As Long)
Private Declare Sub SetWMAAlbumArtistW Lib "AudioGenie3.dll" Alias "SetWMAAlbumArtist" (ByVal NewValue As Long)
Private Declare Sub SetWMAArtistW Lib "AudioGenie3.dll" Alias "SetWMAArtist" (ByVal NewValue As Long)
Private Declare Sub SetWMAAuthorURLW Lib "AudioGenie3.dll" Alias "SetWMAAuthorURL" (ByVal NewValue As Long)
Private Declare Sub SetWMABeatsPerMinuteW Lib "AudioGenie3.dll" Alias "SetWMABeatsPerMinute" (ByVal NewValue As Long)
Private Declare Sub SetWMACategoryW Lib "AudioGenie3.dll" Alias "SetWMACategory" (ByVal NewValue As Long)
Private Declare Sub SetWMACommentW Lib "AudioGenie3.dll" Alias "SetWMAComment" (ByVal NewValue As Long)
Private Declare Sub SetWMAComposerW Lib "AudioGenie3.dll" Alias "SetWMAComposer" (ByVal NewValue As Long)
Private Declare Sub SetWMAConductorW Lib "AudioGenie3.dll" Alias "SetWMAConductor" (ByVal NewValue As Long)
Private Declare Sub SetWMAContentGroupDescriptionW Lib "AudioGenie3.dll" Alias "SetWMAContentGroupDescription" (ByVal NewValue As Long)
Private Declare Sub SetWMACopyrightW Lib "AudioGenie3.dll" Alias "SetWMACopyright" (ByVal NewValue As Long)
Private Declare Sub SetWMADistributorW Lib "AudioGenie3.dll" Alias "SetWMADistributor" (ByVal NewValue As Long)
Private Declare Sub SetWMAEncodedByW Lib "AudioGenie3.dll" Alias "SetWMAEncodedBy" (ByVal NewValue As Long)
Private Declare Sub SetWMAEncodingSettingsW Lib "AudioGenie3.dll" Alias "SetWMAEncodingSettings" (ByVal NewValue As Long)
Private Declare Sub SetWMAEncodingTimeW Lib "AudioGenie3.dll" Alias "SetWMAEncodingTime" (ByVal NewValue As Long)
Private Declare Sub SetWMAGenreW Lib "AudioGenie3.dll" Alias "SetWMAGenre" (ByVal NewValue As Long)
Private Declare Sub SetWMAInitialKeyW Lib "AudioGenie3.dll" Alias "SetWMAInitialKey" (ByVal NewValue As Long)
Private Declare Sub SetWMAISRCW Lib "AudioGenie3.dll" Alias "SetWMAISRC" (ByVal NewValue As Long)
Private Declare Sub SetWMALanguageW Lib "AudioGenie3.dll" Alias "SetWMALanguage" (ByVal NewValue As Long)
Private Declare Sub SetWMALyricsW Lib "AudioGenie3.dll" Alias "SetWMALyrics" (ByVal NewValue As Long)
Private Declare Sub SetWMAMoodW Lib "AudioGenie3.dll" Alias "SetWMAMood" (ByVal NewValue As Long)
Private Declare Sub SetWMAParentalRatingW Lib "AudioGenie3.dll" Alias "SetWMAParentalRating" (ByVal NewValue As Long)
Private Declare Sub SetWMAPartOfSetW Lib "AudioGenie3.dll" Alias "SetWMAPartOfSet" (ByVal NewValue As Long)
Private Declare Sub SetWMAPeriodW Lib "AudioGenie3.dll" Alias "SetWMAPeriod" (ByVal NewValue As Long)
Private Declare Sub SetWMAProducerW Lib "AudioGenie3.dll" Alias "SetWMAProducer" (ByVal NewValue As Long)
Private Declare Sub SetWMAPromotionURLW Lib "AudioGenie3.dll" Alias "SetWMAPromotionURL" (ByVal NewValue As Long)
Private Declare Sub SetWMAProviderW Lib "AudioGenie3.dll" Alias "SetWMAProvider" (ByVal NewValue As Long)
Private Declare Sub SetWMAPublisherW Lib "AudioGenie3.dll" Alias "SetWMAPublisher" (ByVal NewValue As Long)
Private Declare Sub SetWMARatingW Lib "AudioGenie3.dll" Alias "SetWMARating" (ByVal NewValue As Long)
Private Declare Sub SetWMATitleW Lib "AudioGenie3.dll" Alias "SetWMATitle" (ByVal NewValue As Long)
Private Declare Sub SetWMAToolNameW Lib "AudioGenie3.dll" Alias "SetWMAToolName" (ByVal NewValue As Long)
Private Declare Sub SetWMAToolVersionW Lib "AudioGenie3.dll" Alias "SetWMAToolVersion" (ByVal NewValue As Long)
Private Declare Sub SetWMATrackNumberW Lib "AudioGenie3.dll" Alias "SetWMATrackNumber" (ByVal NewValue As Long)
Private Declare Sub SetWMAUserItemW Lib "AudioGenie3.dll" Alias "SetWMAUserItem" (ByVal ItemKey As Long, ByVal NewValue As Long)
Private Declare Sub SetWMAUserWebURLW Lib "AudioGenie3.dll" Alias "SetWMAUserWebURL" (ByVal NewValue As Long)
Private Declare Sub SetWMAWriterW Lib "AudioGenie3.dll" Alias "SetWMAWriter" (ByVal NewValue As Long)
Private Declare Sub SetWMAYearW Lib "AudioGenie3.dll" Alias "SetWMAYear" (ByVal NewValue As Long)

Public Enum Ausfuehrung
  FEHLERFREI = -1
  FEHLERHAFT = 0
End Enum
  
Public Enum AudioFormatID
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
End Enum

Public Enum PictureType
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

Public Enum PictureLink
  AS_FILENAME = -1
  AS_PICTURE = 0
End Enum
  
Public Enum SyncLyricContentTypes
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

Public Enum SyncLyricTimeFormat
  UNGUELTIG = -1
  MPEGFRAMES = 1
  MILLISEKUNDEN = 2
End Enum

Public Enum FRAMES
  F_APIC = 1095780675
  F_ASPI = 1095979081
  F_COMM = 1129270605
  F_COMR = 1129270610
  F_ENCR = 1162756946
  F_EQU2 = 1162958130
  F_EQUA = 1162958145
  F_ETCO = 1163150159
  F_GEOB = 1195724610
  F_GRID = 1196575044
  F_IPLS = 1229999187
  F_LINK = 1279872587
  F_MCDI = 1296254025
  F_MLLT = 1296845908
  F_OWNE = 1331121733
  F_PCNT = 1346588244
  F_POPM = 1347375181
  F_POSS = 1347375955
  F_PRIV = 1347570006
  F_RBUF = 1380078918
  F_RVA2 = 1381384498
  F_RVAD = 1381384516
  F_RVRB = 1381388866
  F_SEEK = 1397048651
  F_SIGN = 1397311310
  F_SYLT = 1398361172
  F_SYTC = 1398363203
  F_TALB = &H54414C42
  F_TBPM = &H5442504D
  F_TCM = &H54434D20  ' Only id3v2.2
  F_TCOM = &H54434F4D
  F_TCON = &H54434F4E
  F_TCOP = &H54434F50
  F_TCR = &H54435220  ' Only id3v2.2
  F_TDAT = &H54444154
  F_TDEN = &H5444454E
  F_TDLY = &H54444C59
  F_TDOR = &H54444F52
  F_TDRC = &H54445243
  F_TDRL = &H5444524C
  F_TDTG = &H54445447
  F_TENC = &H54454E43
  F_TEXT = &H54455854
  F_TFLT = &H54464C54
  F_TIME = &H54494D45
  F_TIPL = &H5449504C
  F_TIT1 = &H54495431
  F_TIT2 = &H54495432
  F_TIT3 = &H54495433
  F_TKEY = &H544B4559
  F_TLAN = &H544C414E
  F_TLEN = &H544C454E
  F_TMCL = &H544D434C
  F_TMED = &H544D4544
  F_TMOO = &H544D4F4F
  F_TOAL = &H544F414C
  F_TOFN = &H544F464E
  F_TOLY = &H544F4C59
  F_TOPE = &H544F5045
  F_TORY = &H544F5259
  F_TOWN = &H544F574E
  F_TPE1 = &H54504531
  F_TPE2 = &H54504532
  F_TPE3 = &H54504533
  F_TPE4 = &H54504534
  F_TPOS = &H54504F53
  F_TPRO = &H5450524F
  F_TPUB = &H54505542
  F_TRCK = &H5452434B
  F_TRDA = &H54524441
  F_TRSN = &H5452534E
  F_TRSO = &H5452534F
  F_TSIZ = &H5453495A
  F_TSOA = &H54534F41
  F_TSOP = &H54534F50
  F_TSOT = &H54534F54
  F_TSRC = &H54535243
  F_TSSE = &H54535345
  F_TSST = &H54535354
  F_TXXX = &H54585858
  F_TYER = &H54594552
  F_UFID = &H55464944
  F_USER = &H55534552
  F_USLT = &H55534C54
  F_WCOM = &H57434F4D
  F_WCOP = &H57434F50
  F_WOAF = &H574F4146
  F_WOAR = &H574F4152
  F_WOAS = &H574F4153
  F_WORS = &H574F5253
  F_WPAY = &H57504159
  F_WXXX = &H57585858
  F_WPUB = &H57505542
End Enum

Public Property Get APEAlbum() As String
  APEAlbum = StrConv(GetAPEAlbumW(), vbFromUnicode)
End Property

Public Property Let APEAlbum(ByVal NewValue As String)
  SetAPEAlbumW StrPtr(NewValue)
End Property

Public Property Get APEArtist() As String
  APEArtist = StrConv(GetAPEArtistW(), vbFromUnicode)
End Property

Public Property Let APEArtist(ByVal NewValue As String)
  SetAPEArtistW StrPtr(NewValue)
End Property

Public Property Get APEComment() As String
  APEComment = StrConv(GetAPECommentW(), vbFromUnicode)
End Property

Public Property Let APEComment(ByVal NewValue As String)
  SetAPECommentW StrPtr(NewValue)
End Property

Public Property Get APECopyright() As String
  APECopyright = StrConv(GetAPECopyrightW(), vbFromUnicode)
End Property

Public Property Let APECopyright(ByVal NewValue As String)
  SetAPECopyrightW StrPtr(NewValue)
End Property

Public Property Get APEGenre() As String
  APEGenre = StrConv(GetAPEGenreW(), vbFromUnicode)
End Property

Public Property Let APEGenre(ByVal NewValue As String)
  SetAPEGenreW StrPtr(NewValue)
End Property

Public Property Get APETitle() As String
  APETitle = StrConv(GetAPETitleW(), vbFromUnicode)
End Property

Public Property Let APETitle(ByVal NewValue As String)
  SetAPETitleW StrPtr(NewValue)
End Property

Public Property Get APETrack() As String
  APETrack = StrConv(GetAPETrackW(), vbFromUnicode)
End Property

Public Property Let APETrack(ByVal NewValue As String)
  SetAPETrackW StrPtr(NewValue)
End Property

Public Property Get APEUserItem(ByVal Key As String) As String
  APEUserItem = StrConv(GetAPEUserItemW(StrPtr(Key)), vbFromUnicode)
End Property

Public Property Let APEUserItem(ByVal Key As String, ByVal NewValue As String)
  SetAPEUserItemW StrPtr(Key), StrPtr(NewValue)
End Property

Public Property Get APEYear() As String
  APEYear = StrConv(GetAPEYearW(), vbFromUnicode)
End Property

Public Property Let APEYear(ByVal NewValue As String)
  SetAPEYearW StrPtr(NewValue)
End Property

Public Function AUDIOAnalyzeFile(FileName As String) As AudioFormatID
  AUDIOAnalyzeFile = AUDIOAnalyzeFileW(StrPtr(FileName))
End Function

Public Function AUDIOGetMD5Value() As String
  AUDIOGetMD5Value = StrConv(AUDIOGetMD5ValueW, vbFromUnicode)
End Function

Public Property Get AUDIOAlbum() As String
  AUDIOAlbum = StrConv(GetAUDIOAlbumW(), vbFromUnicode)
End Property

Public Property Let AUDIOAlbum(ByVal NewValue As String)
  SetAUDIOAlbumW StrPtr(NewValue)
End Property

Public Property Get AUDIOArtist() As String
   AUDIOArtist = StrConv(GetAUDIOArtistW(), vbFromUnicode)
End Property

Public Property Let AUDIOArtist(ByVal NewValue As String)
  SetAUDIOArtistW StrPtr(NewValue)
End Property

Public Property Get AUDIOComment() As String
  AUDIOComment = StrConv(GetAUDIOCommentW(), vbFromUnicode)
End Property

Public Property Let AUDIOComment(ByVal NewValue As String)
  SetAUDIOCommentW StrPtr(NewValue)
End Property

Public Property Get AUDIOGenre() As String
  AUDIOGenre = StrConv(GetAUDIOGenreW(), vbFromUnicode)
End Property

Public Property Let AUDIOGenre(ByVal NewValue As String)
  SetAUDIOGenreW StrPtr(NewValue)
End Property

Public Property Get AUDIOTitle() As String
  AUDIOTitle = StrConv(GetAUDIOTitleW(), vbFromUnicode)
End Property

Public Property Let AUDIOTitle(ByVal NewValue As String)
  SetAUDIOTitleW StrPtr(NewValue)
End Property

Public Property Get AUDIOTrack() As String
  AUDIOTrack = StrConv(GetAUDIOTrackW(), vbFromUnicode)
End Property

Public Property Let AUDIOTrack(ByVal NewValue As String)
  SetAUDIOTrackW StrPtr(NewValue)
End Property

Public Property Get AUDIOYear() As String
  AUDIOYear = StrConv(GetAUDIOYearW(), vbFromUnicode)
End Property

Public Property Let AUDIOYear(ByVal NewValue As String)
  SetAUDIOYearW StrPtr(NewValue)
End Property

Public Property Get FLACAlbum() As String
  FLACAlbum = StrConv(GetFLACAlbumW(), vbFromUnicode)
End Property

Public Property Let FLACAlbum(ByVal NewValue As String)
  SetFLACAlbumW StrPtr(NewValue)
End Property

Public Property Get FLACArtist() As String
  FLACArtist = StrConv(GetFLACArtistW(), vbFromUnicode)
End Property

Public Property Let FLACArtist(ByVal NewValue As String)
  SetFLACArtistW StrPtr(NewValue)
End Property

Public Property Get FLACComment() As String
  FLACComment = StrConv(GetFLACCommentW(), vbFromUnicode)
End Property

Public Property Let FLACComment(ByVal NewValue As String)
  SetFLACCommentW StrPtr(NewValue)
End Property

Public Property Get FLACContact() As String
  FLACContact = StrConv(GetFLACContactW(), vbFromUnicode)
End Property

Public Property Let FLACContact(ByVal NewValue As String)
  SetFLACContactW StrPtr(NewValue)
End Property

Public Property Get FLACCopyright() As String
  FLACCopyright = StrConv(GetFLACCopyrightW(), vbFromUnicode)
End Property

Public Property Let FLACCopyright(ByVal NewValue As String)
  SetFLACCopyrightW StrPtr(NewValue)
End Property

Public Property Get FLACDate() As String
  FLACDate = StrConv(GetFLACDateW(), vbFromUnicode)
End Property

Public Property Let FLACDate(ByVal NewValue As String)
  SetFLACDateW StrPtr(NewValue)
End Property

Public Property Get FLACDescription() As String
  FLACDescription = StrConv(GetFLACDescriptionW(), vbFromUnicode)
End Property

Public Property Let FLACDescription(ByVal NewValue As String)
  SetFLACDescriptionW StrPtr(NewValue)
End Property

Public Property Get FLACGenre() As String
  FLACGenre = StrConv(GetFLACGenreW(), vbFromUnicode)
End Property

Public Property Let FLACGenre(ByVal NewValue As String)
  SetFLACGenreW StrPtr(NewValue)
End Property

Public Property Get FLACISRC() As String
  FLACISRC = StrConv(GetFLACISRCW(), vbFromUnicode)
End Property

Public Property Let FLACISRC(ByVal NewValue As String)
  SetFLACISRCW StrPtr(NewValue)
End Property

Public Property Get FLACLicense() As String
  FLACLicense = StrConv(GetFLACLicenseW(), vbFromUnicode)clyrics
End Property

Public Property Let FLACLicense(ByVal NewValue As String)
  SetFLACLicenseW StrPtr(NewValue)
End Property

Public Property Get FLACLocation() As String
  FLACLocation = StrConv(GetFLACLocationW(), vbFromUnicode)
End Property

Public Property Let FLACLocation(ByVal NewValue As String)
  SetFLACLocationW StrPtr(NewValue)
End Property

Public Property Get FLACOrganization() As String
  FLACOrganization = StrConv(GetFLACOrganizationW(), vbFromUnicode)
End Property

Public Property Let FLACOrganization(ByVal NewValue As String)
  SetFLACOrganizationW StrPtr(NewValue)
End Property

Public Property Get FLACPerformer() As String
  FLACPerformer = StrConv(GetFLACPerformerW(), vbFromUnicode)
End Property

Public Property Let FLACPerformer(ByVal NewValue As String)
  SetFLACPerformerW StrPtr(NewValue)
End Property

Public Property Get FLACTitle() As String
  FLACTitle = StrConv(GetFLACTitleW(), vbFromUnicode)
End Property

Public Property Let FLACTitle(ByVal NewValue As String)
  SetFLACTitleW StrPtr(NewValue)
End Property

Public Property Get FLACTrack() As String
  FLACTrack = StrConv(GetFLACTrackW(), vbFromUnicode)
End Property

Public Property Let FLACTrack(ByVal NewValue As String)
  SetFLACTrackW StrPtr(NewValue)
End Property

Public Property Get FLACUserItem(ByVal Key As String) As String
  FLACUserItem = StrConv(GetFLACUserItemW(StrPtr(Key)), vbFromUnicode)
End Property

Public Property Let FLACUserItem(ByVal Key As String, ByVal NewValue As String)
  SetFLACUserItemW StrPtr(Key), StrPtr(NewValue)
End Property

Public Property Get FLACVendor() As String
  FLACVendor = StrConv(GetFLACVendorW(), vbFromUnicode)
End Property

Public Property Let FLACVendor(ByVal NewValue As String)
  SetFLACVendorW StrPtr(NewValue)
End Property

Public Property Get FLACVersion() As String
  FLACVersion = StrConv(GetFLACVersionW(), vbFromUnicode)
End Property

Public Property Let FLACVersion(ByVal NewValue As String)
  SetFLACVersionW StrPtr(NewValue)
End Property

Public Property Get ID3V1Album() As String
  ID3V1Album = StrConv(GetID3V1AlbumW(), vbFromUnicode)
End Property

Public Property Let ID3V1Album(ByVal NewValue As String)
  SetID3V1AlbumW StrPtr(NewValue)
End Property

Public Property Get ID3V1Artist() As String
  ID3V1Artist = StrConv(GetID3V1ArtistW(), vbFromUnicode)
End Property

Public Property Let ID3V1Artist(ByVal NewValue As String)
  SetID3V1ArtistW StrPtr(NewValue)
End Property

Public Property Get ID3V1Comment() As String
  ID3V1Comment = StrConv(GetID3V1CommentW(), vbFromUnicode)
End Property

Public Property Let ID3V1Comment(ByVal NewValue As String)
  SetID3V1CommentW StrPtr(NewValue)
End Property

Public Property Get ID3V1Genre() As String
  ID3V1Genre = StrConv(GetID3V1GenreW(), vbFromUnicode)
End Property

Public Property Let ID3V1Genre(ByVal NewValue As String)
  SetID3V1GenreW StrPtr(NewValue)
End Property

Public Property Get ID3V1GenreID() As Integer
  ID3V1GenreID = GetID3V1GenreIDW()
End Property

Public Property Let ID3V1GenreID(ByVal NewValue As Integer)
  SetID3V1GenreIDW NewValue
End Property

Public Property Get ID3V1Title() As String
  ID3V1Title = StrConv(GetID3V1TitleW(), vbFromUnicode)
End Property

Public Property Let ID3V1Title(ByVal NewValue As String)
  SetID3V1TitleW StrPtr(NewValue)
End Property

Public Property Get ID3V1Track() As String
  ID3V1Track = StrConv(GetID3V1TrackW(), vbFromUnicode)
End Property

Public Property Let ID3V1Track(ByVal NewValue As String)
  SetID3V1TrackW StrPtr(NewValue)
End Property

Public Property Get ID3V1Year() As String
  ID3V1Year = StrConv(GetID3V1YearW(), vbFromUnicode)
End Property

Public Property Let ID3V1Year(ByVal NewValue As String)
  SetID3V1YearW StrPtr(NewValue)
End Property

Public Property Get ID3V2Album() As String ' TALB
  ID3V2Album = StrConv(GetID3V2TextFrameW(F_TALB), vbFromUnicode)
End Property

Public Property Let ID3V2Album(ByVal NewValue As String) ' TALB
  SetID3V2TextFrameW F_TALB, StrPtr(NewValue)
End Property

Public Property Get ID3V2Artist() As String ' TPE1
  ID3V2Artist = StrConv(GetID3V2TextFrameW(F_TPE1), vbFromUnicode)
End Property

Public Property Let ID3V2Artist(ByVal NewValue As String)
  SetID3V2TextFrameW F_TPE1, StrPtr(NewValue)
End Property

Public Property Get ID3V2ArtistURL() As String ' WOAR
  ID3V2ArtistURL = StrConv(GetID3V2URLFrameW(F_WOAR), vbFromUnicode)
End Property

Public Property Let ID3V2ArtistURL(ByVal NewValue As String)
  SetID3V2URLFrameW F_WOAR, StrPtr(NewValue)
End Property

Public Property Get ID3V2AudiofileURL() As String ' WOAF
  ID3V2AudiofileURL = StrConv(GetID3V2URLFrameW(F_WOAF), vbFromUnicode)
End Property

Public Property Let ID3V2AudiofileURL(ByVal NewValue As String)
  SetID3V2URLFrameW F_WOAF, StrPtr(NewValue)
End Property

Public Property Get ID3V2AudiosourceURL() As String ' WOAS
  ID3V2AudiosourceURL = StrConv(GetID3V2URLFrameW(F_WOAS), vbFromUnicode)
End Property

Public Property Let ID3V2AudiosourceURL(ByVal NewValue As String)
  SetID3V2URLFrameW F_WOAS, StrPtr(NewValue)
End Property

Public Property Get ID3V2BeatsPerMinute() As String  ' TBPM
  ID3V2BeatsPerMinute = StrConv(GetID3V2TextFrameW(F_TBPM), vbFromUnicode)
End Property

Public Property Let ID3V2BeatsPerMinute(ByVal NewValue As String)
  SetID3V2TextFrameW F_TBPM, StrPtr(NewValue)
End Property

Public Property Get ID3V2Category() As String ' TIT1
  ID3V2Category = StrConv(GetID3V2TextFrameW(F_TIT1), vbFromUnicode)
End Property

Public Property Let ID3V2Category(ByVal NewValue As String)
  SetID3V2TextFrameW F_TIT1, StrPtr(NewValue)
End Property

Public Property Get ID3V2CommercialURL() As String ' WCOM
  ID3V2CommercialURL = StrConv(GetID3V2URLFrameW(F_WCOM), vbFromUnicode)
End Property

Public Property Let ID3V2CommercialURL(ByVal NewValue As String) ' WCOM
  SetID3V2URLFrameW F_WCOM, StrPtr(NewValue)
End Property

Public Property Get ID3V2Composer() As String ' TCOM
  ID3V2Composer = StrConv(GetID3V2TextFrameW(F_TCOM), vbFromUnicode)
End Property

Public Property Let ID3V2Composer(ByVal NewValue As String)
  SetID3V2TextFrameW F_TCOM, StrPtr(NewValue)
End Property

Public Property Get ID3V2Conductor() As String ' TPE3
  ID3V2Conductor = StrConv(GetID3V2TextFrameW(F_TPE3), vbFromUnicode)
End Property

Public Property Let ID3V2Conductor(ByVal NewValue As String) ' TPE3
  SetID3V2TextFrameW F_TPE3, StrPtr(NewValue)
End Property

Public Property Get ID3V2Copyright() As String ' TCOP
  ID3V2Copyright = StrConv(GetID3V2TextFrameW(F_TCOP), vbFromUnicode)
End Property

Public Property Let ID3V2Copyright(ByVal NewValue As String)
  SetID3V2TextFrameW F_TCOP, StrPtr(NewValue)
End Property

Public Property Get ID3V2CopyrightURL() As String ' WCOP
  ID3V2CopyrightURL = StrConv(GetID3V2URLFrameW(F_WCOP), vbFromUnicode)
End Property

Public Property Let ID3V2CopyrightURL(ByVal NewValue As String)
  SetID3V2URLFrameW F_WCOP, StrPtr(NewValue)
End Property

Public Property Get ID3V2Date() As String ' TDAT
  ID3V2Date = StrConv(GetID3V2TextFrameW(F_TDAT), vbFromUnicode)
End Property

Public Property Let ID3V2Date(ByVal NewValue As String)
 SetID3V2TextFrameW F_TDAT, StrPtr(NewValue)
End Property

Public Property Get ID3V2Encoder() As String ' TENC
  ID3V2Encoder = StrConv(GetID3V2TextFrameW(F_TENC), vbFromUnicode)
End Property

Public Property Let ID3V2Encoder(ByVal NewValue As String)
  SetID3V2TextFrameW F_TENC, StrPtr(NewValue)
End Property

Public Property Get ID3V2EncodingSettings() As String ' TSSE
  ID3V2EncodingSettings = StrConv(GetID3V2TextFrameW(F_TSSE), vbFromUnicode)
End Property

Public Property Let ID3V2EncodingSettings(ByVal NewValue As String)
  SetID3V2TextFrameW F_TSSE, StrPtr(NewValue)
End Property

Public Property Get ID3V2FileOwner() As String ' TOWN
  ID3V2FileOwner = StrConv(GetID3V2TextFrameW(F_TOWN), vbFromUnicode)
End Property

Public Property Let ID3V2FileOwner(ByVal NewValue As String)
  SetID3V2TextFrameW F_TOWN, StrPtr(NewValue)
End Property

Public Property Get ID3V2FileType() As String ' TFLT
  ID3V2FileType = StrConv(GetID3V2TextFrameW(F_TFLT), vbFromUnicode)
End Property

Public Property Let ID3V2FileType(ByVal NewValue As String)
  SetID3V2TextFrameW F_TFLT, StrPtr(NewValue)
End Property

Public Property Get ID3V2Genre() As String ' TCON
  ID3V2Genre = StrConv(GetID3V2GenreW, vbFromUnicode)
End Property

Public Property Let ID3V2Genre(ByVal NewValue As String)
  SetID3V2GenreW StrPtr(NewValue)
End Property

Public Property Get ID3V2InetRadioName() As String ' TRSN
  ID3V2InetRadioName = StrConv(GetID3V2TextFrameW(F_TRSN), vbFromUnicode)
End Property

Public Property Let ID3V2InetRadioName(ByVal NewValue As String)
  SetID3V2TextFrameW F_TRSN, StrPtr(NewValue)
End Property

Public Property Get ID3V2InetRadioOwner() As String ' TRSO
  ID3V2InetRadioOwner = StrConv(GetID3V2TextFrameW(F_TRSO), vbFromUnicode)
End Property

Public Property Let ID3V2InetRadioOwner(ByVal NewValue As String)
  SetID3V2TextFrameW F_TRSO, StrPtr(NewValue)
End Property

Public Property Get ID3V2InitialKey() As String ' TKEY
  ID3V2InitialKey = StrConv(GetID3V2TextFrameW(F_TKEY), vbFromUnicode)
End Property

Public Property Let ID3V2InitialKey(ByVal NewValue As String)
  SetID3V2TextFrameW F_TKEY, StrPtr(NewValue)
End Property

Public Property Get ID3V2Interpreted() As String ' TPE4
  ID3V2Interpreted = StrConv(GetID3V2TextFrameW(F_TPE4), vbFromUnicode)
End Property

Public Property Let ID3V2Interpreted(ByVal NewValue As String)
  SetID3V2TextFrameW F_TPE4, StrPtr(NewValue)
End Property

Public Property Get ID3V2ISRC() As String ' TSRC
  ID3V2ISRC = StrConv(GetID3V2TextFrameW(F_TSRC), vbFromUnicode)
End Property

Public Property Let ID3V2ISRC(ByVal NewValue As String)
  SetID3V2TextFrameW F_TSRC, StrPtr(NewValue)
End Property

Public Property Get ID3V2Language() As String ' TLAN
  ID3V2Language = StrConv(GetID3V2TextFrameW(F_TLAN), vbFromUnicode)
End Property

Public Property Let ID3V2Language(ByVal NewValue As String)
  SetID3V2TextFrameW F_TLAN, StrPtr(NewValue)
End Property

Public Property Get ID3V2Length() As String ' TLEN
  ID3V2Length = StrConv(GetID3V2TextFrameW(F_TLEN), vbFromUnicode)
End Property

Public Property Let ID3V2Length(ByVal NewValue As String)
  SetID3V2TextFrameW F_TLEN, StrPtr(NewValue)
End Property

Public Property Get ID3V2MediaType() As String ' TMED
  ID3V2MediaType = StrConv(GetID3V2TextFrameW(F_TMED), vbFromUnicode)
End Property

Public Property Let ID3V2MediaType(ByVal NewValue As String)
  SetID3V2TextFrameW F_TMED, StrPtr(NewValue)
End Property

Public Property Get ID3V2Orchestra() As String ' TPE2
  ID3V2Orchestra = StrConv(GetID3V2TextFrameW(F_TPE2), vbFromUnicode)
End Property

Public Property Let ID3V2Orchestra(ByVal NewValue As String)
  SetID3V2TextFrameW F_TPE2, StrPtr(NewValue)
End Property

Public Property Get ID3V2OriginalAlbum() As String ' TOAL
  ID3V2OriginalAlbum = StrConv(GetID3V2TextFrameW(F_TOAL), vbFromUnicode)
End Property

Public Property Let ID3V2OriginalAlbum(ByVal NewValue As String)
  SetID3V2TextFrameW F_TOAL, StrPtr(NewValue)
End Property

Public Property Get ID3V2OriginalArtist() As String ' TOPE
  ID3V2OriginalArtist = StrConv(GetID3V2TextFrameW(F_TOPE), vbFromUnicode)
End Property

Public Property Let ID3V2OriginalArtist(ByVal NewValue As String)
  SetID3V2TextFrameW F_TOPE, StrPtr(NewValue)
End Property

Public Property Get ID3V2OriginalFilename() As String ' TOFN
  ID3V2OriginalFilename = StrConv(GetID3V2TextFrameW(F_TOFN), vbFromUnicode)
End Property

Public Property Let ID3V2OriginalFilename(ByVal NewValue As String)
  SetID3V2TextFrameW F_TOFN, StrPtr(NewValue)
End Property

Public Property Get ID3V2OriginalReleaseYear() As String ' TORY
  ID3V2OriginalReleaseYear = StrConv(GetID3V2TextFrameW(F_TORY), vbFromUnicode)
End Property

Public Property Let ID3V2OriginalReleaseYear(ByVal NewValue As String)
  SetID3V2TextFrameW F_TORY, StrPtr(NewValue)
End Property

Public Property Get ID3V2OriginalTextwriter() As String ' TOLY
  ID3V2OriginalTextwriter = StrConv(GetID3V2TextFrameW(F_TOLY), vbFromUnicode)
End Property

Public Property Let ID3V2OriginalTextwriter(ByVal NewValue As String)
  SetID3V2TextFrameW F_TOLY, StrPtr(NewValue)
End Property

Public Property Get ID3V2PartofSet() As String ' TPOS
  ID3V2PartofSet = StrConv(GetID3V2TextFrameW(F_TPOS), vbFromUnicode)
End Property

Public Property Let ID3V2PartofSet(ByVal NewValue As String)
  SetID3V2TextFrameW F_TPOS, StrPtr(NewValue)
End Property

Public Property Get ID3V2PaymentURL() As String ' WPAY
  ID3V2PaymentURL = StrConv(GetID3V2URLFrameW(F_WPAY), vbFromUnicode)
End Property

Public Property Let ID3V2PaymentURL(ByVal NewValue As String)
  SetID3V2URLFrameW F_WPAY, StrPtr(NewValue)
End Property

Public Property Get ID3V2PlayCounter() As Long ' PCNT
  ID3V2PlayCounter = GetID3V2PlayCounterW()
End Property

Public Property Let ID3V2PlayCounter(ByVal NewValue As Long)
  SetID3V2PlayCounterW NewValue
End Property

Public Property Get ID3V2PlaylistDelay() As String ' TDLY
  ID3V2PlaylistDelay = StrConv(GetID3V2TextFrameW(F_TDLY), vbFromUnicode)
End Property

Public Property Let ID3V2PlaylistDelay(ByVal NewValue As String)
  SetID3V2TextFrameW F_TDLY, StrPtr(NewValue)
End Property

Public Property Get ID3V2Publisher() As String ' TPUB
  ID3V2Publisher = StrConv(GetID3V2TextFrameW(F_TPUB), vbFromUnicode)
End Property

Public Property Let ID3V2Publisher(ByVal NewValue As String)
  SetID3V2TextFrameW F_TPUB, StrPtr(NewValue)
End Property

Public Property Get ID3V2PublisherURL() As String ' WPUB
  ID3V2PublisherURL = StrConv(GetID3V2URLFrameW(F_WPUB), vbFromUnicode)
End Property

Public Property Let ID3V2PublisherURL(ByVal NewValue As String)
  SetID3V2URLFrameW F_WPUB, StrPtr(NewValue)
End Property

Public Property Get ID3V2RadiostationURL() As String ' WORS
  ID3V2RadiostationURL = StrConv(GetID3V2URLFrameW(F_WORS), vbFromUnicode)
End Property

Public Property Let ID3V2RadiostationURL(ByVal NewValue As String)
  SetID3V2URLFrameW F_WORS, StrPtr(NewValue)
End Property

Public Property Get ID3V2RecordingDate() As String ' TRDA
  ID3V2RecordingDate = StrConv(GetID3V2TextFrameW(F_TRDA), vbFromUnicode)
End Property

Public Property Let ID3V2RecordingDate(ByVal NewValue As String)
  SetID3V2TextFrameW F_TRDA, StrPtr(NewValue)
End Property

Public Property Get ID3V2RecordingTime() As String ' TDRC
  ID3V2RecordingTime = StrConv(GetID3V2TextFrameW(F_TDRC), vbFromUnicode)
End Property

Public Property Let ID3V2RecordingTime(ByVal NewValue As String) ' TDRC
  SetID3V2TextFrameW F_TDRC, StrPtr(NewValue)
End Property

Public Property Get ID3V2Subtitle() As String ' TIT3
  ID3V2Subtitle = StrConv(GetID3V2TextFrameW(F_TIT3), vbFromUnicode)
End Property

Public Property Let ID3V2Subtitle(ByVal NewValue As String)
  SetID3V2TextFrameW F_TIT3, StrPtr(NewValue)
End Property

Public Property Get ID3V2TextWriter() As String ' TEXT
  ID3V2TextWriter = StrConv(GetID3V2TextFrameW(F_TEXT), vbFromUnicode)
End Property

Public Property Let ID3V2TextWriter(ByVal NewValue As String)
  SetID3V2TextFrameW F_TEXT, StrPtr(NewValue)
End Property

Public Property Get ID3V2Time() As String ' TIME
  ID3V2Time = StrConv(GetID3V2TextFrameW(F_TIME), vbFromUnicode)
End Property

Public Property Let ID3V2Time(ByVal NewValue As String)
  SetID3V2TextFrameW F_TIME, StrPtr(NewValue)
End Property

Public Property Get ID3V2Title() As String ' TIT2
  ID3V2Title = StrConv(GetID3V2TextFrameW(F_TIT2), vbFromUnicode)
End Property

Public Property Let ID3V2Title(ByVal NewValue As String)
  SetID3V2TextFrameW F_TIT2, StrPtr(NewValue)
End Property

Public Property Get ID3V2Track() As String ' TRCK
  ID3V2Track = StrConv(GetID3V2TextFrameW(F_TRCK), vbFromUnicode)
End Property

Public Property Let ID3V2Track(ByVal NewValue As String)
  SetID3V2TextFrameW F_TRCK, StrPtr(NewValue)
End Property

Public Property Get ID3V2Year() As String ' TYER
  ID3V2Year = StrConv(GetID3V2TextFrameW(F_TYER), vbFromUnicode)
End Property

Public Property Let ID3V2Year(ByVal NewValue As String)
  SetID3V2TextFrameW F_TYER, StrPtr(NewValue)
End Property

Public Property Get LYRICSAlbum() As String
  LYRICSAlbum = StrConv(GetLYRICSAlbumW(), vbFromUnicode)
End Property

Public Property Let LYRICSAlbum(ByVal NewValue As String)
  SetLYRICSAlbumW StrPtr(NewValue)
End Property

Public Property Get LYRICSArtist() As String
  LYRICSArtist = StrConv(GetLYRICSArtistW(), vbFromUnicode)
End Property

Public Property Let LYRICSArtist(ByVal NewValue As String)
  SetLYRICSArtistW StrPtr(NewValue)
End Property

Public Property Get LYRICSAuthor() As String
  LYRICSAuthor = StrConv(GetLYRICSAuthorW(), vbFromUnicode)
End Property

Public Property Let LYRICSAuthor(ByVal NewValue As String)
  SetLYRICSAuthorW StrPtr(NewValue)
End Property

Public Property Get LYRICSGenre() As String
  LYRICSGenre = StrConv(GetLYRICSGenreW(), vbFromUnicode)
End Property

Public Property Let LYRICSGenre(ByVal NewValue As String)
  SetLYRICSGenreW StrPtr(NewValue)
End Property

Public Property Get LYRICSImageLink() As String
  LYRICSImageLink = StrConv(GetLYRICSImageLinkW(), vbFromUnicode)
End Property

Public Property Let LYRICSImageLink(ByVal NewValue As String)
  SetLYRICSImageLinkW StrPtr(NewValue)
End Property

Public Property Get LYRICSIndication() As String
  LYRICSIndication = StrConv(GetLYRICSIndicationW(), vbFromUnicode)
End Property

Public Property Let LYRICSIndication(ByVal NewValue As String)
  SetLYRICSIndicationW StrPtr(NewValue)
End Property

Public Property Get LYRICSInformation() As String
  LYRICSInformation = StrConv(GetLYRICSInformationW(), vbFromUnicode)
End Property

Public Property Let LYRICSInformation(ByVal NewValue As String)
  SetLYRICSInformationW StrPtr(NewValue)
End Property

Public Property Get LYRICSLyrics() As String
  LYRICSLyrics = StrConv(GetLYRICSLyricsW(), vbFromUnicode)
End Property

Public Property Let LYRICSLyrics(ByVal NewValue As String)
  SetLYRICSLyricsW StrPtr(NewValue)
End Property

Public Property Get LYRICSTitle() As String
  LYRICSTitle = StrConv(GetLYRICSTitleW(), vbFromUnicode)
End Property

Public Property Let LYRICSTitle(ByVal NewValue As String)
  SetLYRICSTitleW StrPtr(NewValue)
End Property

Public Property Get MP4Album() As String
  MP4Album = StrConv(GetMP4AlbumW(), vbFromUnicode)
End Property

Public Property Let MP4Album(ByVal NewValue As String)
  SetMP4AlbumW StrPtr(NewValue)
End Property

Public Property Get MP4Artist() As String
  MP4Artist = StrConv(GetMP4ArtistW(), vbFromUnicode)
End Property

Public Property Let MP4Artist(ByVal NewValue As String)
  SetMP4ArtistW StrPtr(NewValue)
End Property

Public Property Get MP4Author() As String
  MP4Author = StrConv(GetMP4AuthorW(), vbFromUnicode)
End Property

Public Property Let MP4Author(ByVal NewValue As String)
  SetMP4AuthorW StrPtr(NewValue)
End Property

Public Property Get MP4BeatsPerMinute() As String
  MP4BeatsPerMinute = StrConv(GetMP4BeatsPerMinuteW(), vbFromUnicode)
End Property

Public Property Let MP4BeatsPerMinute(ByVal NewValue As String)
  SetMP4BeatsPerMinuteW StrPtr(NewValue)
End Property

Public Property Get MP4CDSetNumber() As String
  MP4CDSetNumber = StrConv(GetMP4CDSetNumberW(), vbFromUnicode)
End Property

Public Property Let MP4CDSetNumber(ByVal NewValue As String)
  SetMP4CDSetNumberW StrPtr(NewValue)
End Property

Public Property Get MP4Comment() As String
  MP4Comment = StrConv(GetMP4CommentW(), vbFromUnicode)
End Property

Public Property Let MP4Comment(ByVal NewValue As String)
  SetMP4CommentW StrPtr(NewValue)
End Property

Public Property Get MP4Compilation() As String
  MP4Compilation = StrConv(GetMP4CompilationW(), vbFromUnicode)
End Property

Public Property Let MP4Compilation(ByVal NewValue As String)
  SetMP4CompilationW StrPtr(NewValue)
End Property

Public Property Get MP4Composer() As String
  MP4Composer = StrConv(GetMP4ComposerW(), vbFromUnicode)
End Property

Public Property Let MP4Composer(ByVal NewValue As String)
  SetMP4ComposerW StrPtr(NewValue)
End Property

Public Property Get MP4Copyright() As String
  MP4Copyright = StrConv(GetMP4CopyrightW(), vbFromUnicode)
End Property

Public Property Let MP4Copyright(ByVal NewValue As String)
  SetMP4CopyrightW StrPtr(NewValue)
End Property

Public Property Get MP4Date() As String
  MP4Date = StrConv(GetMP4DateW(), vbFromUnicode)
End Property

Public Property Let MP4Date(ByVal NewValue As String)
  SetMP4DateW StrPtr(NewValue)
End Property

Public Property Get MP4Description() As String
  MP4Description = StrConv(GetMP4DescriptionW(), vbFromUnicode)
End Property

Public Property Let MP4Description(ByVal NewValue As String)
  SetMP4DescriptionW StrPtr(NewValue)
End Property

Public Property Get MP4Director() As String
  MP4Director = StrConv(GetMP4DirectorW(), vbFromUnicode)
End Property

Public Property Let MP4Director(ByVal NewValue As String)
  SetMP4DirectorW StrPtr(NewValue)
End Property

Public Property Get MP4Disclaimer() As String
  MP4Disclaimer = StrConv(GetMP4DisclaimerW(), vbFromUnicode)
End Property

Public Property Let MP4Disclaimer(ByVal NewValue As String)
  SetMP4DisclaimerW StrPtr(NewValue)
End Property

Public Property Get MP4Encoder() As String
  MP4Encoder = StrConv(GetMP4EncoderW(), vbFromUnicode)
End Property

Public Property Let MP4Encoder(ByVal NewValue As String)
  SetMP4EncoderW StrPtr(NewValue)
End Property

Public Property Get MP4Genre() As String
  MP4Genre = StrConv(GetMP4GenreW(), vbFromUnicode)
End Property

Public Property Let MP4Genre(ByVal NewValue As String)
  SetMP4GenreW StrPtr(NewValue)
End Property

Public Property Get MP4NetUrl() As String
  MP4NetUrl = StrConv(GetMP4NetUrlW(), vbFromUnicode)
End Property

Public Property Let MP4NetUrl(ByVal NewValue As String)
  SetMP4NetUrlW StrPtr(NewValue)
End Property

Public Property Get MP4OriginalArtist() As String
  MP4OriginalArtist = StrConv(GetMP4OriginalArtistW(), vbFromUnicode)
End Property

Public Property Let MP4OriginalArtist(ByVal NewValue As String)
  SetMP4OriginalArtistW StrPtr(NewValue)
End Property

Public Property Get MP4OriginalFormat() As String
  MP4OriginalFormat = StrConv(GetMP4OriginalFormatW(), vbFromUnicode)
End Property

Public Property Let MP4OriginalFormat(ByVal NewValue As String)
  SetMP4OriginalFormatW StrPtr(NewValue)
End Property

Public Property Get MP4OriginalSource() As String
  MP4OriginalSource = StrConv(GetMP4OriginalSourceW(), vbFromUnicode)
End Property

Public Property Let MP4OriginalSource(ByVal NewValue As String)
  SetMP4OriginalSourceW StrPtr(NewValue)
End Property

Public Property Get MP4Performer() As String
  MP4Performer = StrConv(GetMP4PerformerW(), vbFromUnicode)
End Property

Public Property Let MP4Performer(ByVal NewValue As String)
  SetMP4PerformerW StrPtr(NewValue)
End Property

Public Property Get MP4Producer() As String
  MP4Producer = StrConv(GetMP4ProducerW(), vbFromUnicode)
End Property

Public Property Let MP4Producer(ByVal NewValue As String)
  SetMP4ProducerW StrPtr(NewValue)
End Property

Public Property Get MP4Title() As String
  MP4Title = StrConv(GetMP4TitleW(), vbFromUnicode)
End Property

Public Property Let MP4Title(ByVal NewValue As String)
  SetMP4TitleW StrPtr(NewValue)
End Property

Public Property Get MP4Track() As String
  MP4Track = StrConv(GetMP4TrackW(), vbFromUnicode)
End Property

Public Property Let MP4Track(ByVal NewValue As String)
  SetMP4TrackW StrPtr(NewValue)
End Property

Public Property Get MP4Warning() As String
  MP4Warning = StrConv(GetMP4WarningW(), vbFromUnicode)
End Property

Public Property Let MP4Warning(ByVal NewValue As String)
  SetMP4WarningW StrPtr(NewValue)
End Property

Public Property Get MPEGAnalyzeAllFrames() As Boolean
  MPEGAnalyzeAllFrames = (GetMPEGAnalyzeAllFramesW() <> 0)
End Property

Public Property Let MPEGAnalyzeAllFrames(ByVal NewValue As Boolean)
  ' SetMPEGAnalyzeAllFramesW NewValue
End Property

Public Property Get OGGAlbum() As String
  OGGAlbum = StrConv(GetOGGAlbumW(), vbFromUnicode)
End Property

Public Property Let OGGAlbum(ByVal NewValue As String)
  SetOGGAlbumW StrPtr(NewValue)
End Property

Public Property Get OGGArtist() As String
  OGGArtist = StrConv(GetOGGArtistW(), vbFromUnicode)
End Property

Public Property Let OGGArtist(ByVal NewValue As String)
  SetOGGArtistW StrPtr(NewValue)
End Property

Public Property Get OGGComment() As String
  OGGComment = StrConv(GetOGGCommentW(), vbFromUnicode)
End Property

Public Property Let OGGComment(ByVal NewValue As String)
  SetOGGCommentW StrPtr(NewValue)
End Property

Public Property Get OggContact() As String
  OggContact = StrConv(GetOggContactW(), vbFromUnicode)
End Property

Public Property Let OggContact(ByVal NewValue As String)
  SetOggContactW StrPtr(NewValue)
End Property

Public Property Get OGGCopyright() As String
  OGGCopyright = StrConv(GetOGGCopyrightW(), vbFromUnicode)
End Property

Public Property Let OGGCopyright(ByVal NewValue As String)
  SetOGGCopyrightW StrPtr(NewValue)
End Property

Public Property Get OGGDate() As String
  OGGDate = StrConv(GetOGGDateW(), vbFromUnicode)
End Property

Public Property Let OGGDate(ByVal NewValue As String)
  SetOGGDateW StrPtr(NewValue)
End Property

Public Property Get OGGDescription() As String
  OGGDescription = StrConv(GetOGGDescriptionW(), vbFromUnicode)
End Property

Public Property Let OGGDescription(ByVal NewValue As String)
  SetOGGDescriptionW StrPtr(NewValue)
End Property

Public Property Get OGGGenre() As String
  OGGGenre = StrConv(GetOGGGenreW(), vbFromUnicode)
End Property

Public Property Let OGGGenre(ByVal NewValue As String)
  SetOGGGenreW StrPtr(NewValue)
End Property

Public Property Get OGGISRC() As String
  OGGISRC = StrConv(GetOGGISRCW(), vbFromUnicode)
End Property

Public Property Let OGGISRC(ByVal NewValue As String)
  SetOGGISRCW StrPtr(NewValue)
End Property

Public Property Get OGGLicense() As String
  OGGLicense = StrConv(GetOGGLicenseW(), vbFromUnicode)
End Property

Public Property Let OGGLicense(ByVal NewValue As String)
  SetOGGLicenseW StrPtr(NewValue)
End Property

Public Property Get OGGLocation() As String
  OGGLocation = StrConv(GetOGGLocationW(), vbFromUnicode)
End Property

Public Property Let OGGLocation(ByVal NewValue As String)
  SetOGGLocationW StrPtr(NewValue)
End Property

Public Property Get OGGOrganization() As String
  OGGOrganization = StrConv(GetOGGOrganizationW(), vbFromUnicode)
End Property

Public Property Let OGGOrganization(ByVal NewValue As String)
  SetOGGOrganizationW StrPtr(NewValue)
End Property

Public Property Get OGGPerformer() As String
  OGGPerformer = StrConv(GetOGGPerformerW(), vbFromUnicode)
End Property

Public Property Let OGGPerformer(ByVal NewValue As String)
  SetOGGPerformerW StrPtr(NewValue)
End Property

Public Property Get OGGTitle() As String
  OGGTitle = StrConv(GetOGGTitleW(), vbFromUnicode)
End Property

Public Property Let OGGTitle(ByVal NewValue As String)
  SetOGGTitleW StrPtr(NewValue)
End Property

Public Property Get OGGTrack() As String
  OGGTrack = StrConv(GetOGGTrackW(), vbFromUnicode)
End Property

Public Property Let OGGTrack(ByVal NewValue As String)
  SetOGGTrackW StrPtr(NewValue)
End Property

Public Property Get OGGUserItem(ByVal Key As String) As String
  OGGUserItem = StrConv(GetOGGUserItemW(StrPtr(Key)), vbFromUnicode)
End Property

Public Property Let OGGUserItem(ByVal Key As String, ByVal NewValue As String)
  SetOGGUserItemW StrPtr(Key), StrPtr(NewValue)
End Property

Public Property Get OGGVendor() As String
  OGGVendor = StrConv(GetOGGVendorW(), vbFromUnicode)
End Property

Public Property Let OGGVendor(ByVal NewValue As String)
  SetOGGVendorW StrPtr(NewValue)
End Property

Public Property Get OGGVersion() As String
  OGGVersion = StrConv(GetOGGVersionW(), vbFromUnicode)
End Property

Public Property Let OGGVersion(ByVal NewValue As String)
  SetOGGVersionW StrPtr(NewValue)
End Property

Public Property Get WAVArchivalLocation() As String
  WAVArchivalLocation = StrConv(GetWAVArchivalLocationW(), vbFromUnicode)
End Property

Public Property Let WAVArchivalLocation(ByVal NewValue As String)
  SetWAVArchivalLocationW StrPtr(NewValue)
End Property

Public Property Get WAVArtist() As String
  WAVArtist = StrConv(GetWAVArtistW(), vbFromUnicode)
End Property

Public Property Let WAVArtist(ByVal NewValue As String)
  SetWAVArtistW StrPtr(NewValue)
End Property

Public Property Get WAVComissioned() As String
  WAVComissioned = StrConv(GetWAVComissionedW(), vbFromUnicode)
End Property

Public Property Let WAVComissioned(ByVal NewValue As String)
  SetWAVComissionedW StrPtr(NewValue)
End Property

Public Property Get WAVComment() As String
  WAVComment = StrConv(GetWAVCommentW(), vbFromUnicode)
End Property

Public Property Let WAVComment(ByVal NewValue As String)
  SetWAVCommentW StrPtr(NewValue)
End Property

Public Property Get WAVCopyright() As String
  WAVCopyright = StrConv(GetWAVCopyrightW(), vbFromUnicode)
End Property

Public Property Let WAVCopyright(ByVal NewValue As String)
  SetWAVCopyrightW StrPtr(NewValue)
End Property

Public Property Get WAVCreationDate() As String
  WAVCreationDate = StrConv(GetWAVCreationDateW(), vbFromUnicode)
End Property

Public Property Let WAVCreationDate(ByVal NewValue As String)
  SetWAVCreationDateW StrPtr(NewValue)
End Property

Public Property Get WAVCropped() As String
  WAVCropped = StrConv(GetWAVCroppedW(), vbFromUnicode)
End Property

Public Property Let WAVCropped(ByVal NewValue As String)
  SetWAVCroppedW StrPtr(NewValue)
End Property

Public Property Get WAVDimension() As String
  WAVDimension = StrConv(GetWAVDimensionW(), vbFromUnicode)
End Property

Public Property Let WAVDimension(ByVal NewValue As String)
  SetWAVDimensionW StrPtr(NewValue)
End Property

Public Property Get WAVDotsPerInch() As String
  WAVDotsPerInch = StrConv(GetWAVDotsPerInchW(), vbFromUnicode)
End Property

Public Property Let WAVDotsPerInch(ByVal NewValue As String)
  SetWAVDotsPerInchW StrPtr(NewValue)
End Property

Public Property Get WAVEngineer() As String
  WAVEngineer = StrConv(GetWAVEngineerW(), vbFromUnicode)
End Property

Public Property Let WAVEngineer(ByVal NewValue As String)
  SetWAVEngineerW StrPtr(NewValue)
End Property

Public Property Get WAVGenre() As String
  WAVGenre = StrConv(GetWAVGenreW(), vbFromUnicode)
End Property

Public Property Let WAVGenre(ByVal NewValue As String)
  SetWAVGenreW StrPtr(NewValue)
End Property

Public Property Get WAVKeywords() As String
  WAVKeywords = StrConv(GetWAVKeywordsW(), vbFromUnicode)
End Property

Public Property Let WAVKeywords(ByVal NewValue As String)
  SetWAVKeywordsW StrPtr(NewValue)
End Property

Public Property Get WAVLightness() As String
  WAVLightness = StrConv(GetWAVLightnessW(), vbFromUnicode)
End Property

Public Property Let WAVLightness(ByVal NewValue As String)
  SetWAVLightnessW StrPtr(NewValue)
End Property

Public Property Get WAVMedium() As String
  WAVMedium = StrConv(GetWAVMediumW(), vbFromUnicode)
End Property

Public Property Let WAVMedium(ByVal NewValue As String)
  SetWAVMediumW StrPtr(NewValue)
End Property

Public Property Get WAVName() As String
  WAVName = StrConv(GetWAVNameW(), vbFromUnicode)
End Property

Public Property Let WAVName(ByVal NewValue As String)
  SetWAVNameW StrPtr(NewValue)
End Property

Public Property Get WAVPaletteSetting() As String
  WAVPaletteSetting = StrConv(GetWAVPaletteSettingW(), vbFromUnicode)
End Property

Public Property Let WAVPaletteSetting(ByVal NewValue As String)
  SetWAVPaletteSettingW StrPtr(NewValue)
End Property

Public Property Get WAVProduct() As String
  WAVProduct = StrConv(GetWAVProductW(), vbFromUnicode)
End Property

Public Property Let WAVProduct(ByVal NewValue As String)
  SetWAVProductW StrPtr(NewValue)
End Property

Public Property Get WAVSharpness() As String
  WAVSharpness = StrConv(GetWAVSharpnessW(), vbFromUnicode)
End Property

Public Property Let WAVSharpness(ByVal NewValue As String)
  SetWAVSharpnessW StrPtr(NewValue)
End Property

Public Property Get WAVSoftware() As String
  WAVSoftware = StrConv(GetWAVSoftwareW(), vbFromUnicode)
End Property

Public Property Let WAVSoftware(ByVal NewValue As String)
  SetWAVSoftwareW StrPtr(NewValue)
End Property

Public Property Get WAVSource() As String
  WAVSource = StrConv(GetWAVSourceW(), vbFromUnicode)
End Property

Public Property Let WAVSource(ByVal NewValue As String)
  SetWAVSourceW StrPtr(NewValue)
End Property

Public Property Get WAVSourceForm() As String
  WAVSourceForm = StrConv(GetWAVSourceFormW(), vbFromUnicode)
End Property

Public Property Let WAVSourceForm(ByVal NewValue As String)
  SetWAVSourceFormW StrPtr(NewValue)
End Property

Public Property Get WAVSubject() As String
  WAVSubject = StrConv(GetWAVSubjectW(), vbFromUnicode)
End Property

Public Property Let WAVSubject(ByVal NewValue As String)
  SetWAVSubjectW StrPtr(NewValue)
End Property

Public Property Get WAVTechnician() As String
  WAVTechnician = StrConv(GetWAVTechnicianW(), vbFromUnicode)
End Property

Public Property Let WAVTechnician(ByVal NewValue As String)
  SetWAVTechnicianW StrPtr(NewValue)
End Property

Public Property Get WAVTrack() As String
  WAVTrack = StrConv(GetWAVTrackW(), vbFromUnicode)
End Property

Public Property Let WAVTrack(ByVal NewValue As String)
  SetWAVTrackW StrPtr(NewValue)
End Property

Public Property Get WMAAlbum() As String
  WMAAlbum = StrConv(GetWMAAlbumW(), vbFromUnicode)
End Property

Public Property Let WMAAlbum(ByVal NewValue As String)
  SetWMAAlbumW StrPtr(NewValue)
End Property

Public Property Get WMAAlbumArtist() As String
  WMAAlbumArtist = StrConv(GetWMAAlbumArtistW(), vbFromUnicode)
End Property

Public Property Let WMAAlbumArtist(ByVal NewValue As String)
  SetWMAAlbumArtistW StrPtr(NewValue)
End Property

Public Property Get WMAArtist() As String
  WMAArtist = StrConv(GetWMAArtistW(), vbFromUnicode)
End Property

Public Property Let WMAArtist(ByVal NewValue As String)
  SetWMAArtistW StrPtr(NewValue)
End Property

Public Property Get WMAAuthorURL() As String
  WMAAuthorURL = StrConv(GetWMAAuthorURLW(), vbFromUnicode)
End Property

Public Property Let WMAAuthorURL(ByVal NewValue As String)
  SetWMAAuthorURLW StrPtr(NewValue)
End Property

Public Property Get WMABeatsPerMinute() As String
  WMABeatsPerMinute = StrConv(GetWMABeatsPerMinuteW(), vbFromUnicode)
End Property

Public Property Let WMABeatsPerMinute(ByVal NewValue As String)
  SetWMABeatsPerMinuteW StrPtr(NewValue)
End Property

Public Property Get WMACategory() As String
  WMACategory = StrConv(GetWMACategoryW(), vbFromUnicode)
End Property

Public Property Let WMACategory(ByVal NewValue As String)
  SetWMACategoryW StrPtr(NewValue)
End Property

Public Property Get WMAComment() As String
  WMAComment = StrConv(GetWMACommentW(), vbFromUnicode)
End Property

Public Property Let WMAComment(ByVal NewValue As String)
  SetWMACommentW StrPtr(NewValue)
End Property

Public Property Get WMAComposer() As String
  WMAComposer = StrConv(GetWMAComposerW(), vbFromUnicode)
End Property

Public Property Let WMAComposer(ByVal NewValue As String)
  SetWMAComposerW StrPtr(NewValue)
End Property

Public Property Get WMAConductor() As String
  WMAConductor = StrConv(GetWMAConductorW(), vbFromUnicode)
End Property

Public Property Let WMAConductor(ByVal NewValue As String)
  SetWMAConductorW StrPtr(NewValue)
End Property

Public Property Get WMAContentGroupDescription() As String
  WMAContentGroupDescription = StrConv(GetWMAContentGroupDescriptionW(), vbFromUnicode)
End Property

Public Property Let WMAContentGroupDescription(ByVal NewValue As String)
  SetWMAContentGroupDescriptionW StrPtr(NewValue)
End Property

Public Property Get WMACopyright() As String
  WMACopyright = StrConv(GetWMACopyrightW(), vbFromUnicode)
End Property

Public Property Let WMACopyright(ByVal NewValue As String)
  SetWMACopyrightW StrPtr(NewValue)
End Property

Public Property Get WMADistributor() As String
  WMADistributor = StrConv(GetWMADistributorW(), vbFromUnicode)
End Property

Public Property Let WMADistributor(ByVal NewValue As String)
  SetWMADistributorW StrPtr(NewValue)
End Property

Public Property Get WMAEncodedBy() As String
  WMAEncodedBy = StrConv(GetWMAEncodedByW(), vbFromUnicode)
End Property

Public Property Let WMAEncodedBy(ByVal NewValue As String)
  SetWMAEncodedByW StrPtr(NewValue)
End Property

Public Property Get WMAEncodingSettings() As String
  WMAEncodingSettings = StrConv(GetWMAEncodingSettingsW(), vbFromUnicode)
End Property

Public Property Let WMAEncodingSettings(ByVal NewValue As String)
  SetWMAEncodingSettingsW StrPtr(NewValue)
End Property

Public Property Get WMAEncodingTime() As String
  WMAEncodingTime = StrConv(GetWMAEncodingTimeW(), vbFromUnicode)
End Property

Public Property Let WMAEncodingTime(ByVal NewValue As String)
  SetWMAEncodingTimeW StrPtr(NewValue)
End Property

Public Property Get WMAGenre() As String
  WMAGenre = StrConv(GetWMAGenreW(), vbFromUnicode)
End Property

Public Property Let WMAGenre(ByVal NewValue As String)
  SetWMAGenreW StrPtr(NewValue)
End Property

Public Property Get WMAInitialKey() As String
  WMAInitialKey = StrConv(GetWMAInitialKeyW(), vbFromUnicode)
End Property

Public Property Let WMAInitialKey(ByVal NewValue As String)
  SetWMAInitialKeyW StrPtr(NewValue)
End Property

Public Property Get WMAISRC() As String
  WMAISRC = StrConv(GetWMAISRCW(), vbFromUnicode)
End Property

Public Property Let WMAISRC(ByVal NewValue As String)
  SetWMAISRCW StrPtr(NewValue)
End Property

Public Property Get WMALanguage() As String
  WMALanguage = StrConv(GetWMALanguageW(), vbFromUnicode)
End Property

Public Property Let WMALanguage(ByVal NewValue As String)
  SetWMALanguageW StrPtr(NewValue)
End Property

Public Property Get WMALyrics() As String
  WMALyrics = StrConv(GetWMALyricsW(), vbFromUnicode)
End Property

Public Property Let WMALyrics(ByVal NewValue As String)
  SetWMALyricsW StrPtr(NewValue)
End Property

Public Property Get WMAMood() As String
  WMAMood = StrConv(GetWMAMoodW(), vbFromUnicode)
End Property

Public Property Let WMAMood(ByVal NewValue As String)
  SetWMAMoodW StrPtr(NewValue)
End Property

Public Property Get WMAParentalRating() As String
  WMAParentalRating = StrConv(GetWMAParentalRatingW(), vbFromUnicode)
End Property

Public Property Let WMAParentalRating(ByVal NewValue As String)
  SetWMAParentalRatingW StrPtr(NewValue)
End Property

Public Property Get WMAPartOfSet() As String
  WMAPartOfSet = StrConv(GetWMAPartOfSetW(), vbFromUnicode)
End Property

Public Property Let WMAPartOfSet(ByVal NewValue As String)
  SetWMAPartOfSetW StrPtr(NewValue)
End Property

Public Property Get WMAPeriod() As String
  WMAPeriod = StrConv(GetWMAPeriodW(), vbFromUnicode)
End Property

Public Property Let WMAPeriod(ByVal NewValue As String)
  SetWMAPeriodW StrPtr(NewValue)
End Property

Public Property Get WMAProducer() As String
  WMAProducer = StrConv(GetWMAProducerW(), vbFromUnicode)
End Property

Public Property Let WMAProducer(ByVal NewValue As String)
  SetWMAProducerW StrPtr(NewValue)
End Property

Public Property Get WMAPromotionURL() As String
  WMAPromotionURL = StrConv(GetWMAPromotionURLW(), vbFromUnicode)
End Property

Public Property Let WMAPromotionURL(ByVal NewValue As String)
  SetWMAPromotionURLW StrPtr(NewValue)
End Property

Public Property Get WMAProvider() As String
  WMAProvider = StrConv(GetWMAProviderW(), vbFromUnicode)
End Property

Public Property Let WMAProvider(ByVal NewValue As String)
  SetWMAProviderW StrPtr(NewValue)
End Property

Public Property Get WMAPublisher() As String
  WMAPublisher = StrConv(GetWMAPublisherW(), vbFromUnicode)
End Property

Public Property Let WMAPublisher(ByVal NewValue As String)
  SetWMAPublisherW StrPtr(NewValue)
End Property

Public Property Get WMARating() As String
  WMARating = StrConv(GetWMARatingW(), vbFromUnicode)
End Property

Public Property Let WMARating(ByVal NewValue As String)
  SetWMARatingW StrPtr(NewValue)
End Property

Public Property Get WMATitle() As String
  WMATitle = StrConv(GetWMATitleW(), vbFromUnicode)
End Property

Public Property Let WMATitle(ByVal NewValue As String)
  SetWMATitleW StrPtr(NewValue)
End Property

Public Property Get WMAToolName() As String
  WMAToolName = StrConv(GetWMAToolNameW(), vbFromUnicode)
End Property

Public Property Let WMAToolName(ByVal NewValue As String)
  SetWMAToolNameW StrPtr(NewValue)
End Property

Public Property Get WMAToolVersion() As String
  WMAToolVersion = StrConv(GetWMAToolVersionW(), vbFromUnicode)
End Property

Public Property Let WMAToolVersion(ByVal NewValue As String)
  SetWMAToolVersionW StrPtr(NewValue)
End Property

Public Property Get WMATrackNumber() As String
  WMATrackNumber = StrConv(GetWMATrackNumberW(), vbFromUnicode)
End Property

Public Property Let WMATrackNumber(ByVal NewValue As String)
  SetWMATrackNumberW StrPtr(NewValue)
End Property

Public Property Get WMAUserItem(ByVal Key As String) As String
  WMAUserItem = StrConv(GetWMAUserItemW(StrPtr(Key)), vbFromUnicode)
End Property

Public Property Let WMAUserItem(ByVal Key As String, ByVal NewValue As String)
  SetWMAUserItemW StrPtr(Key), StrPtr(NewValue)
End Property

Public Property Get WMAUserWebURL() As String
  WMAUserWebURL = StrConv(GetWMAUserWebURLW(), vbFromUnicode)
End Property

Public Property Let WMAUserWebURL(ByVal NewValue As String)
  SetWMAUserWebURLW StrPtr(NewValue)
End Property

Public Property Get WMAWriter() As String
  WMAWriter = StrConv(GetWMAWriterW(), vbFromUnicode)
End Property

Public Property Let WMAWriter(ByVal NewValue As String)
  SetWMAWriterW StrPtr(NewValue)
End Property

Public Property Get WMAYear() As String
  WMAYear = StrConv(GetWMAYearW(), vbFromUnicode)
End Property

Public Property Let WMAYear(ByVal NewValue As String)
  SetWMAYearW StrPtr(NewValue)
End Property

Public Function AACGetBitRateType() As String
  AACGetBitRateType = StrConv(AACGetBitRateTypeW(), vbFromUnicode)
End Function

Public Function AACGetHeaderType() As String
  AACGetHeaderType = StrConv(AACGetHeaderTypeW(), vbFromUnicode)
End Function

Public Function AACGetProfile() As String
  AACGetProfile = StrConv(AACGetProfileW, vbFromUnicode)
End Function

Public Function APEGetItemKeys() As String
  APEGetItemKeys = StrConv(APEGetItemKeysW(), vbFromUnicode)
End Function

Public Function APEGetVersion() As String
  APEGetVersion = StrConv(APEGetVersionW(), vbFromUnicode)
End Function

Public Function APERemoveTagFromFile(ByVal FileName As String) As Ausfuehrung
  APERemoveTagFromFile = APERemoveTagFromFileW(StrPtr(FileName))
End Function

Public Function APERemoveTag() As Ausfuehrung
  APERemoveTag = APERemoveTagW()
End Function

Public Function APESaveChangesToFile(ByVal FileName As String) As Ausfuehrung
  APESaveChangesToFile = APESaveChangesToFileW(StrPtr(FileName))
End Function

Public Function APESaveChanges() As Ausfuehrung
  APESaveChanges = APESaveChangesW()
End Function

Public Function AUDIOGetChannelMode() As String
  AUDIOGetChannelMode = StrConv(AUDIOGetChannelModeW(), vbFromUnicode)
End Function

Public Function AUDIOGetLastErrorText() As String
  AUDIOGetLastErrorText = StrConv(AUDIOGetLastErrorTextW(), vbFromUnicode)
End Function

Public Function AUDIOGetVersion() As String
  AUDIOGetVersion = StrConv(AUDIOGetVersionW(), vbFromUnicode)
End Function

Public Function AUDIOSaveChangesToFile(ByVal FileName As String) As Ausfuehrung
  AUDIOSaveChangesToFile = AUDIOSaveChangesToFileW(StrPtr(FileName))
End Function

Public Function AUDIOSaveChanges() As Ausfuehrung
  AUDIOSaveChanges = AUDIOSaveChangesW()
End Function

Public Function FLACGetItemKeys() As String
  FLACGetItemKeys = StrConv(FLACGetItemKeysW(), vbFromUnicode)
End Function

Public Function FLACSaveChangesToFile(ByVal FileName As String) As Ausfuehrung
  FLACSaveChangesToFile = FLACSaveChangesToFileW(StrPtr(FileName))
End Function

Public Function FLACSaveChanges() As Ausfuehrung
  FLACSaveChanges = FLACSaveChangesW()
End Function

Public Function GetAudioGenieVersion() As String
  GetAudioGenieVersion = StrConv(GetAudioGenieVersionW(), vbFromUnicode)
End Function

Public Function ID3V1GetGenreItem(ByVal number As Integer) As String
  ID3V1GetGenreItem = StrConv(ID3V1GetGenreItemW(number), vbFromUnicode)
End Function

Public Function ID3V1GetVersion() As String
  ID3V1GetVersion = StrConv(ID3V1GetVersionW(), vbFromUnicode)
End Function

Public Function ID3V1RemoveTagFromFile(ByVal FileName As String) As Ausfuehrung
  ID3V1RemoveTagFromFile = ID3V1RemoveTagFromFileW(StrPtr(FileName))
End Function

Public Function ID3V1RemoveTag() As Ausfuehrung
  ID3V1RemoveTag = ID3V1RemoveTagW()
End Function

Public Function ID3V1SaveChangesToFile(ByVal FileName As String) As Ausfuehrung
  ID3V1SaveChangesToFile = ID3V1SaveChangesToFileW(StrPtr(FileName))
End Function

Public Function ID3V1SaveChanges() As Ausfuehrung
  ID3V1SaveChanges = ID3V1SaveChangesW()
End Function

Public Function ID3V2AddComment(ByVal Language As String, ByVal Description As String, ByVal Text As String) As Integer
  ID3V2AddComment = ID3V2AddCommentW(StrPtr(Language), StrPtr(Description), StrPtr(Text))
End Function

Public Function ID3V2AddLyric(ByVal Language As String, ByVal Description As Long, ByVal Text As String) As Integer
  ID3V2AddLyric = ID3V2AddLyricW(StrPtr(Language), StrPtr(Description), StrPtr(Text))
End Function

Public Function ID3V2AddPopularimeter(ByVal Email As String, ByVal Rating As Integer, ByVal Counter As Long) As Integer
  ID3V2AddPopularimeter = ID3V2AddPopularimeterW(StrPtr(Email), Rating, Counter)
End Function

Public Function ID3V2AddSyncLyric(ByVal Language As String, ByVal Description As String, ByVal Text As String, ByVal ContentType As Integer, ByVal TimeStampFormat As Integer) As Integer
  ID3V2AddSyncLyric = ID3V2AddSyncLyricW(StrPtr(Language), StrPtr(Description), StrPtr(Text), ContentType, TimeStampFormat)
End Function

Public Function ID3V2AddUserFrame(ByVal Language As String, ByVal Text As String) As Integer
  ID3V2AddUserFrame = ID3V2AddUserFrameW(StrPtr(Language), StrPtr(Text))
End Function

Public Function ID3V2AddUserText(ByVal Description As String, ByVal Text As String) As Integer
  ID3V2AddUserText = ID3V2AddUserTextW(StrPtr(Description), StrPtr(Text))
End Function

Public Function ID3V2AddUserURL(ByVal Description As String, ByVal URL As String) As Integer
  ID3V2AddUserURL = ID3V2AddUserURLW(StrPtr(Description), StrPtr(URL))
End Function

Public Function ID3V2GetComment(ByVal Index As Integer) As String
  ID3V2GetComment = StrConv(ID3V2GetCommentW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetCommentDescription(ByVal Index As Integer) As String
 ID3V2GetCommentDescription = StrConv(ID3V2GetCommentDescriptionW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetCommentLanguage(ByVal Index As Integer) As String
  ID3V2GetCommentLanguage = StrConv(ID3V2GetCommentLanguageW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetLyric(ByVal Index As Integer) As String
  ID3V2GetLyric = StrConv(ID3V2GetLyricW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetLyricDescription(ByVal Index As Integer) As String
  ID3V2GetLyricDescription = StrConv(ID3V2GetLyricDescriptionW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetLyricLanguage(ByVal Index As Integer) As String
  ID3V2GetLyricLanguage = StrConv(ID3V2GetLyricLanguageW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetPicture(ByVal Index As Integer, ByVal FileName As String) As Ausfuehrung
  ID3V2GetPicture = ID3V2GetPictureW(Index, StrPtr(FileName))
End Function

Public Function ID3V2GetPictureArray(ByVal Index As Integer) As Byte()
  Dim tmpByteArray() As Byte
  ReDim tmpByteArray(0) As Byte
  Call ID3V2GetPictureArrayW(Index, tmpByteArray)
  ID3V2GetPictureArray = tmpByteArray
End Function

Public Function ID3V2CopyPictureArray(Index As Integer) As Byte()
  Dim Result As Long
  Dim tmpByteArray() As Byte
  ReDim tmpByteArray(10000000) As Byte
  Result = ID3V2CopyPictureArrayW(VarPtr(tmpByteArray(0)), 10000000, Index)
  If Result > 0 Then
    ReDim Preserve tmpByteArray(Result) As Byte
  Else
    ReDim tmpByteArray(0) As Byte
  End If
  ID3V2CopyPictureArray = tmpByteArray
End Function

Public Function ID3V2GetPictureDescription(ByVal Index As Integer) As String
  ID3V2GetPictureDescription = StrConv(ID3V2GetPictureDescriptionW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetPictureMime(ByVal Index As Integer) As String
  ID3V2GetPictureMime = StrConv(ID3V2GetPictureMimeW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetPopularimeterEmail(ByVal Index As Integer) As String
  ID3V2GetPopularimeterEmail = StrConv(ID3V2GetPopularimeterEmailW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetSyncLyric(ByVal Index As Integer) As String
  ID3V2GetSyncLyric = StrConv(ID3V2GetSyncLyricW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetSyncLyricDescription(ByVal Index As Integer) As String
  ID3V2GetSyncLyricDescription = StrConv(ID3V2GetSyncLyricDescriptionW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetSyncLyricLanguage(ByVal Index As Integer) As String
  ID3V2GetSyncLyricLanguage = StrConv(ID3V2GetSyncLyricLanguageW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetUserFrames() As Integer
  ID3V2GetUserFrames = ID3V2GetFrameCountW(F_USER)
End Function

Public Function ID3V2GetUserFrame(ByVal Index As Integer) As String
  ID3V2GetUserFrame = StrConv(ID3V2GetUserFrameW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetUserFrameLanguage(ByVal Index As Integer) As String
  ID3V2GetUserFrameLanguage = StrConv(ID3V2GetUserFrameLanguageW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetUserText(ByVal Index As Integer) As String
  ID3V2GetUserText = StrConv(ID3V2GetUserTextW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetUserTextDescription(ByVal Index As Integer) As String
  ID3V2GetUserTextDescription = StrConv(ID3V2GetUserTextDescriptionW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetUserURL(ByVal Index As Integer) As String
  ID3V2GetUserURL = StrConv(ID3V2GetUserURLW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetUserURLDescription(ByVal Index As Integer) As String
  ID3V2GetUserURLDescription = StrConv(ID3V2GetUserURLDescriptionW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetVersion() As String
  ID3V2GetVersion = StrConv(ID3V2GetVersionW(), vbFromUnicode)
End Function

Public Function ID3V2RemoveTagFromFile(ByVal FileName As String) As Ausfuehrung
  ID3V2RemoveTagFromFile = ID3V2RemoveTagFromFileW(StrPtr(FileName))
End Function

Public Function ID3V2RemoveTag() As Ausfuehrung
  ID3V2RemoveTag = ID3V2RemoveTagW()
End Function

Public Function ID3V2SaveChangesToFile(ByVal FileName As String) As Ausfuehrung
  ID3V2SaveChangesToFile = ID3V2SaveChangesToFileW(StrPtr(FileName))
End Function

Public Function ID3V2SaveChanges() As Ausfuehrung
  ID3V2SaveChanges = ID3V2SaveChangesW()
End Function

Public Function ID3V2SetFormatAndEncoding(ByVal format As Integer, ByVal encoding As Integer) As Ausfuehrung
  ID3V2SetFormatAndEncoding = ID3V2SetFormatAndEncodingW(format, encoding)
End Function

Public Function ID3V2SetComment(ByVal Index As Integer, ByVal Language As String, ByVal Description As String, ByVal Text As String) As Ausfuehrung
  ID3V2SetComment = ID3V2SetCommentW(Index, StrPtr(Language), StrPtr(Description), StrPtr(Text))
End Function

Public Function ID3V2SetLyric(ByVal Index As Integer, ByVal Language As String, ByVal Description As String, ByVal Text As String) As Ausfuehrung
  ID3V2SetLyric = ID3V2SetLyricW(Index, StrPtr(Language), StrPtr(Description), StrPtr(Text))
End Function

Public Function ID3V2SetPicture(ByVal FileName As String, ByVal Description As String, ByVal Index As Integer, ByVal PictureType As PictureType, ByVal asLink As PictureLink) As Ausfuehrung
  ID3V2SetPicture = ID3V2SetPictureW(StrPtr(FileName), StrPtr(Description), Index, PictureType, asLink)
End Function

Public Function ID3V2SetPictureArray(byteArray() As Byte, Index As Integer, Description As String, picType As PictureType) As Ausfuehrung
  ID3V2SetPictureArray = ID3V2SetPictureArrayW(byteArray, StrPtr(Description), Index, picType)
End Function

Public Function ID3V2AddPictureArray(byteArray() As Byte, Description As String, picType As PictureType) As Ausfuehrung
  ID3V2AddPictureArray = ID3V2AddPictureArrayW(VarPtr(byteArray(0)), UBound(byteArray), StrPtr(Description), picType)
End Function

Public Function ID3V2SetPopularimeter(ByVal Index As Integer, ByVal Email As String, ByVal Rating As Integer, ByVal Counter As Long) As Ausfuehrung
  ID3V2SetPopularimeter = ID3V2SetPopularimeterW(Index, StrPtr(Email), Rating, Counter)
End Function

Public Function ID3V2SetSyncLyric(ByVal Index As Integer, ByVal Language As String, ByVal Description As String, ByVal Text As String, ByVal ContentType As SyncLyricContentTypes, ByVal TimeStampFormat As SyncLyricTimeFormat) As Ausfuehrung
  ID3V2SetSyncLyric = ID3V2SetSyncLyricW(Index, StrPtr(Language), StrPtr(Description), StrPtr(Text), ContentType, TimeStampFormat)
End Function

Public Function ID3V2SetUserFrame(ByVal Index As Integer, ByVal Language As String, ByVal Text As String) As Ausfuehrung
  ID3V2SetUserFrame = ID3V2SetUserFrameW(Index, StrPtr(Language), StrPtr(Text))
End Function

Public Function ID3V2SetUserText(ByVal Index As Integer, ByVal Description As String, ByVal Text As String) As Ausfuehrung
  ID3V2SetUserText = ID3V2SetUserTextW(Index, StrPtr(Description), StrPtr(Text))
End Function

Public Function ID3V2SetUserURL(ByVal Index As Integer, ByVal Description As String, ByVal Text As String) As Ausfuehrung
  ID3V2SetUserURL = ID3V2SetUserURLW(Index, StrPtr(Description), StrPtr(Text))
End Function

Public Sub ID3V2SetTextFormatToAnsi()
    Call ID3V2SetTextFormatToAnsiW
End Sub

Public Sub ID3V2SetTextFormatToUnicode()
    Call ID3V2SetTextFormatToUnicodeW
End Sub

Public Function LYRICSGetVersion() As String
  LYRICSGetVersion = StrConv(LYRICSGetVersionW(), vbFromUnicode)
End Function

Public Function LYRICSRemoveTagFromFile(ByVal FileName As String) As Ausfuehrung
  LYRICSRemoveTagFromFile = LYRICSRemoveTagFromFileW(StrPtr(FileName))
End Function

Public Function LYRICSRemoveTag() As Ausfuehrung
  LYRICSRemoveTag = LYRICSRemoveTagW()
End Function

Public Function LYRICSSaveChangesToFile(ByVal FileName As String) As Ausfuehrung
  LYRICSSaveChangesToFile = LYRICSSaveChangesToFileW(StrPtr(FileName))
End Function

Public Function LYRICSSaveChanges() As Ausfuehrung
  LYRICSSaveChanges = LYRICSSaveChangesW()
End Function

Public Function MONKEYGetCompression() As String
  MONKEYGetCompression = StrConv(MONKEYGetCompressionW(), vbFromUnicode)
End Function

Public Function MP4LoadPicture(ByVal FileName As String, ByVal Index As Integer) As Ausfuehrung
  MP4LoadPicture = MP4LoadPictureW(StrPtr(FileName), Index)
End Function

Public Function MP4GetPictureArray(ByVal Index As Integer) As Byte()
  Dim tmpByteArray() As Byte
  ReDim tmpByteArray(0) As Byte
  Call MP4GetPictureArrayW(Index, tmpByteArray)
  MP4GetPictureArray = tmpByteArray
End Function

Public Function MPEGGetEmphasis() As String
  MPEGGetEmphasis = StrConv(MPEGGetEmphasisW(), vbFromUnicode)
End Function

Public Function MPEGGetEncoder() As String
  MPEGGetEncoder = StrConv(MPEGGetEncoder, vbFromUnicode)
End Function

Public Function MPEGGetLayer() As String
  MPEGGetLayer = StrConv(MPEGGetLayerW(), vbFromUnicode)
End Function

Public Function MPEGGetVersion() As String
  MPEGGetVersion = StrConv(MPEGGetVersionW(), vbFromUnicode)
End Function

Public Function MPEGModifyCopyrightBit(ByVal FileName As String, ByVal NewValue As Integer) As Ausfuehrung
  MPEGModifyCopyrightBit = MPEGModifyCopyrightBitW(StrPtr(FileName), NewValue)
End Function

Public Function MPEGModifyOriginalBit(ByVal FileName As String, ByVal NewValue As Integer) As Ausfuehrung
  MPEGModifyOriginalBit = MPEGModifyOriginalBitW(StrPtr(FileName), NewValue)
End Function

Public Function MPEGModifyPrivateBit(ByVal FileName As String, ByVal NewValue As Integer) As Ausfuehrung
  MPEGModifyPrivateBit = MPEGModifyPrivateBitW(StrPtr(FileName), NewValue)
End Function

Public Function OGGGetItemKeys() As String
  OGGGetItemKeys = StrConv(OGGGetItemKeysW(), vbFromUnicode)
End Function

Public Function OGGRemoveTagFromFile(ByVal FileName As String) As Ausfuehrung
  OGGRemoveTagFromFile = OGGRemoveTagFromFileW(StrPtr(FileName))
End Function

Public Function OGGRemoveTag() As Ausfuehrung
  OGGRemoveTag = OGGRemoveTagW()
End Function

Public Function OGGSaveChangesToFile(ByVal FileName As String) As Ausfuehrung
  OGGSaveChangesToFile = OGGSaveChangesToFileW(StrPtr(FileName))
End Function

Public Function OGGSaveChanges() As Ausfuehrung
  OGGSaveChanges = OGGSaveChangesW()
End Function

Public Function WAVGetFormat() As String
  WAVGetFormat = StrConv(WAVGetFormatW(), vbFromUnicode)
End Function

Public Function WMAGetItemKeys() As String
  WMAGetItemKeys = StrConv(WMAGetItemKeysW(), vbFromUnicode)
End Function

Public Function WMAGetPicture(ByVal Index As Integer, ByVal PictureFileName As String) As Ausfuehrung
  WMAGetPicture = WMAGetPictureW(Index, StrPtr(PictureFileName))
End Function

Public Function WMAGetPictureArray(ByVal Index As Integer) As Byte()
  Dim tmpByteArray() As Byte
  ReDim tmpByteArray(0) As Byte
  Call WMAGetPictureArrayW(Index, tmpByteArray)
  WMAGetPictureArray = tmpByteArray
End Function

Public Function WMAGetPictureDescription(ByVal Index As Integer) As String
  WMAGetPictureDescription = StrConv(WMAGetPictureDescriptionW(Index), vbFromUnicode)
End Function

Public Function WMAGetPictureMime(ByVal Index As Integer) As String
  WMAGetPictureMime = StrConv(WMAGetPictureMimeW(Index), vbFromUnicode)
End Function

Public Function WMASaveChangesToFile(ByVal FileName As String) As Ausfuehrung
  WMASaveChangesToFile = WMASaveChangesToFileW(StrPtr(FileName))
End Function

Public Function WMASaveChanges() As Ausfuehrung
  WMASaveChanges = WMASaveChangesW()
End Function

Public Function WMASetPicture(ByVal PictureFileName As String, ByVal Description As String, ByVal Index As Integer, ByVal PictureType As PictureType) As Ausfuehrung
  WMASetPicture = WMASetPictureW(StrPtr(PictureFileName), StrPtr(Description), Index, PictureType)
End Function

Public Function WMASetPictureArray(byteArray() As Byte, Index As Integer, Description As String, picType As PictureType) As Ausfuehrung
  WMASetPictureArray = WMASetPictureArrayW(byteArray, StrPtr(Description), Index, picType)
End Function

Public Function APEExists() As Boolean
  APEExists = (APEExistsW() <> 0)
End Function

Public Function APEGetSize() As Long
  APEGetSize = APEGetSizeW()
End Function

Public Function AUDIOFormatIsValid() As Boolean
  AUDIOFormatIsValid = (AUDIOFormatIsValidW <> 0)
End Function

Public Function AUDIOGetBitrate() As Long
  AUDIOGetBitrate = AUDIOGetBitrateW()
End Function

Public Function AUDIOGetChannels() As Long
  AUDIOGetChannels = AUDIOGetChannelsW()
End Function

Public Function AUDIOGetDuration() As Single
  AUDIOGetDuration = AUDIOGetDurationW()
End Function

Public Function AUDIOGetDurationMillis() As Long
  AUDIOGetDurationMillis = AUDIOGetDurationMillisW()
End Function

Public Function AUDIOGetFileSize() As Long
  AUDIOGetFileSize = AUDIOGetFileSizeW()
End Function

Public Function AUDIOGetLastErrorNumber() As Long
  AUDIOGetLastErrorNumber = AUDIOGetLastErrorNumberW()
End Function

Public Function AUDIOGetLastFile() As String
  AUDIOGetLastFile = StrConv(AUDIOGetLastFileW(), vbFromUnicode)
End Function

Public Function AUDIOGetSampleRate() As Long
  AUDIOGetSampleRate = AUDIOGetSampleRateW()
End Function

Public Sub DisablePopupKey(keyValue As String)
  Call DisablePopupKeyW(StrPtr(keyValue))
End Sub

Public Function GetMD5ValueFromFile(FileName As String) As String
  GetMD5ValueFromFile = StrConv(GetMD5ValueFromFileW(StrPtr(FileName)), vbFromUnicode)
End Function

Public Function FLACGetBitsPerSample() As Long
  FLACGetBitsPerSample = FLACGetBitsPerSampleW()
End Function

Public Function FLACGetCompressionRatio() As Single
  FLACGetCompressionRatio = FLACGetCompressionRatioW()
End Function

Public Function FLACGetMaxBlockSize() As Long
  FLACGetMaxBlockSize = FLACGetMaxBlockSizeW()
End Function

Public Function FLACGetMaxFrameSize() As Long
  FLACGetMaxFrameSize = FLACGetMaxFrameSizeW()
End Function

Public Function FLACGetMinBlockSize() As Long
  FLACGetMinBlockSize = FLACGetMinBlockSizeW()
End Function

Public Function FLACGetMinFrameSize() As Long
  FLACGetMinFrameSize = FLACGetMinFrameSizeW()
End Function

Public Function FLACGetSamples() As Long
  FLACGetSamples = FLACGetSamplesW()
End Function

Public Function ID3V1Exists() As Boolean
  ID3V1Exists = (ID3V1ExistsW() <> 0)
End Function

Public Function ID3V1GetGenres() As Integer
  ID3V1GetGenres = ID3V1GetGenresW()
End Function

Public Sub ID3V2DeleteComments()
  Call ID3V2DeleteAllFramesW(F_COMM)
End Sub

Public Sub ID3V2DeleteLyrics()
  Call ID3V2DeleteAllFramesW(F_USLT)
End Sub

Public Function ID3V2DeletePicture(Index As Integer) As Ausfuehrung
  ID3V2DeletePicture = ID3V2DeleteSelectedFrameW(F_APIC, Index)
End Function

Public Sub ID3V2DeletePictures()
  Call ID3V2DeleteAllFramesW(F_APIC)
End Sub

Public Sub ID3V2DeletePopularimeters()
  Call ID3V2DeleteAllFramesW(F_POPM)
End Sub

Public Function ID3V2DeleteSelectedFrame(ID As Long, Index As Integer) As Integer
  ID3V2DeleteSelectedFrame = ID3V2DeleteSelectedFrameW(ID, Index)
End Function

Public Sub ID3V2DeleteSyncLyrics()
  Call ID3V2DeleteAllFramesW(F_SYLT)
End Sub

Public Sub ID3V2DeleteUserFrames()
  Call ID3V2DeleteAllFramesW(F_USER)
End Sub

Public Sub ID3V2DeleteUserTexts()
  Call ID3V2DeleteAllFramesW(F_TXXX)
End Sub

Public Sub ID3V2DeleteUserURLs()
  Call ID3V2DeleteAllFramesW(F_WXXX)
End Sub

Public Function ID3V2Exists() As Boolean
  ID3V2Exists = (ID3V2ExistsW() <> 0)
End Function

Public Function ID3V2GetComments() As Integer
  ID3V2GetComments = ID3V2GetFrameCountW(F_COMM)
End Function

Public Function ID3V2GetLyrics() As Integer
  ID3V2GetLyrics = ID3V2GetFrameCountW(F_USLT)
End Function

Public Function ID3V2GetPictures() As Integer
  ID3V2GetPictures = ID3V2GetFrameCountW(F_APIC)
End Function

Public Function ID3V2GetPictureType(Index As Integer) As PictureType
  ID3V2GetPictureType = ID3V2GetPictureTypeW(Index)
End Function

Public Function ID3V2GetPopularimeterCounter(Index As Integer) As Long
  ID3V2GetPopularimeterCounter = ID3V2GetPopularimeterCounterW(Index)
End Function

Public Function ID3V2GetPopularimeterRating(Index As Integer) As Integer
  ID3V2GetPopularimeterRating = ID3V2GetPopularimeterRatingW(Index)
End Function

Public Function ID3V2GetPopularimeters() As Integer
  ID3V2GetPopularimeters = ID3V2GetFrameCountW(F_POPM)
End Function

Public Function ID3V2GetSize() As Long
  ID3V2GetSize = ID3V2GetSizeW()
End Function

Public Function ID3V2GetSyncLyricContentType(Index As Integer) As SyncLyricContentTypes
  ID3V2GetSyncLyricContentType = ID3V2GetSyncLyricContentTypeW(Index)
End Function

Public Function ID3V2GetSyncLyrics() As Integer
  ID3V2GetSyncLyrics = ID3V2GetFrameCountW(F_SYLT)
End Function

Public Function ID3V2GetSyncLyricTimeFormat(Index As Integer) As SyncLyricTimeFormat
  ID3V2GetSyncLyricTimeFormat = ID3V2GetSyncLyricTimeFormatW(Index)
End Function

Public Function ID3V2GetUserTexts() As Integer
  ID3V2GetUserTexts = ID3V2GetFrameCountW(F_TXXX)
End Function

Public Function ID3V2GetUserURLs() As Integer
  ID3V2GetUserURLs = ID3V2GetFrameCountW(F_WXXX)
End Function

Public Function LYRICSExists() As Boolean
  LYRICSExists = (LYRICSExistsW() <> 0)
End Function

Public Function LYRICSGetSize() As Long
  LYRICSGetSize = LYRICSGetSizeW()
End Function

Public Function LYRICSGetStartPosition() As Long
  LYRICSGetStartPosition = LYRICSGetStartPositionW()
End Function

Public Function MONKEYGetBits() As Integer
  MONKEYGetBits = MONKEYGetBitsW()
End Function

Public Function MONKEYGetCompressionRatio() As Single
  MONKEYGetCompressionRatio = MONKEYGetCompressionRatioW()
End Function

Public Function MONKEYGetFrames() As Long
  MONKEYGetFrames = MONKEYGetFramesW()
End Function

Public Function MONKEYGetPeak() As Single
  MONKEYGetPeak = MONKEYGetPeakW()
End Function

Public Function MONKEYGetSamples() As Long
  MONKEYGetSamples = MONKEYGetSamplesW()
End Function

Public Function MONKEYGetSamplesPerFrame() As Long
  MONKEYGetSamplesPerFrame = MONKEYGetSamplesPerFrameW()
End Function

Public Function MP4GetPictureCount() As Long
  MP4GetPictureCount = MP4GetPictureCountW()
End Function

Public Function MPEGGetFramePosition() As Long
  MPEGGetFramePosition = MPEGGetFramePositionW()
End Function

Public Function MPEGGetFrames() As Long
  MPEGGetFrames = MPEGGetFramesW()
End Function

Public Function MPEGGetFrameSize() As Long
  MPEGGetFrameSize = MPEGGetFrameSizeW()
End Function

Public Function MPEGIsCopyrighted() As Boolean
  MPEGIsCopyrighted = (MPEGIsCopyrightedW() <> 0)
End Function

Public Function MPEGIsOriginal() As Boolean
  MPEGIsOriginal = (MPEGIsOriginalW() <> 0)
End Function

Public Function MPEGIsPadding() As Boolean
  MPEGIsPadding = (MPEGIsPaddingW() <> 0)
End Function

Public Function MPEGIsPrivate() As Boolean
  MPEGIsPrivate = (MPEGIsPrivateW() <> 0)
End Function

Public Function MPEGIsProtected() As Boolean
  MPEGIsProtected = (MPEGIsProtectedW() <> 0)
End Function

Public Function MPEGIsVBR() As Boolean
  MPEGIsVBR = (MPEGIsVBRW() <> 0)
End Function

Public Function MPPGetFrames() As Long
  MPPGetFrames = MPPGetFramesW()
End Function

Public Function MPPGetStreamVersion() As Integer
  MPPGetStreamVersion = MPPGetStreamVersionW()
End Function

Public Function OGGGetBitRateNominal() As Long
  OGGGetBitRateNominal = OGGGetBitRateNominalW()
End Function

Public Function WAVGetBitsPerSample() As Long
  WAVGetBitsPerSample = WAVGetBitsPerSampleW()
End Function

Public Function WAVGetBlockAlign() As Long
  WAVGetBlockAlign = WAVGetBlockAlignW()
End Function

Public Function WAVGetBytesPerSecond() As Long
  WAVGetBytesPerSecond = WAVGetBytesPerSecondW()
End Function

Public Function WAVGetChannels() As Long
  WAVGetChannels = WAVGetChannelsW()
End Function

Public Function WAVGetFormatID() As Integer
  WAVGetFormatID = WAVGetFormatIDW()
End Function

Public Function WAVGetHeaderSize() As Long
  WAVGetHeaderSize = WAVGetHeaderSizeW()
End Function

Public Function WMADeletePicture(Index As Integer) As Ausfuehrung
  WMADeletePicture = WMADeletePictureW(Index)
End Function

Public Sub WMADeletePictures()
  Call WMADeletePicturesW
End Sub

Public Function WMAGetPictures() As Integer
  WMAGetPictures = WMAGetPicturesW()
End Function

Public Function WMAGetPictureType(Index As Integer) As PictureType
  WMAGetPictureType = WMAGetPictureTypeW(Index)
End Function

