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
' *******  Copyright 2007-2010 by Stefan Toengi   *******
' *******************************************************
Option Explicit
Private Declare Function AACGetBitRateTypeW Lib "AudioGenie3.dll" () As String
Private Declare Function AACGetHeaderTypeW Lib "AudioGenie3.dll" () As String
Private Declare Function AACGetProfileW Lib "AudioGenie3.dll" () As String
Private Declare Function APEExistsW Lib "AudioGenie3.dll" () As Integer
Private Declare Function APEGetAlbumW Lib "AudioGenie3.dll" () As String
Private Declare Function APEGetArtistW Lib "AudioGenie3.dll" () As String
Private Declare Function APEGetCommentW Lib "AudioGenie3.dll" () As String
Private Declare Function APEGetCopyrightW Lib "AudioGenie3.dll" () As String
Private Declare Function APEGetGenreW Lib "AudioGenie3.dll" () As String
Private Declare Function APEGetItemKeysW Lib "AudioGenie3.dll" () As String
Private Declare Function APEGetSizeW Lib "AudioGenie3.dll" () As Long
Private Declare Function APEGetTitleW Lib "AudioGenie3.dll" () As String
Private Declare Function APEGetTrackW Lib "AudioGenie3.dll" () As String
Private Declare Function APEGetUserItemW Lib "AudioGenie3.dll" (ByVal key As Long) As String
Private Declare Function APEGetVersionW Lib "AudioGenie3.dll" () As String
Private Declare Function APEGetYearW Lib "AudioGenie3.dll" () As String
Private Declare Function APERemoveTagFromFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function APERemoveTagW Lib "AudioGenie3.dll" () As Integer
Private Declare Function APESaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function APESaveChangesW Lib "AudioGenie3.dll" () As Integer
Private Declare Sub APESetAlbumW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub APESetArtistW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub APESetCommentW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub APESetCopyrightW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub APESetGenreW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub APESetTitleW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub APESetTrackW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub APESetUserItemW Lib "AudioGenie3.dll" (ByVal key As Long, ByVal textString As Long)
Private Declare Sub APESetYearW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Function AUDIOAnalyzeFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function AUDIOIsValidFormatW Lib "AudioGenie3.dll" () As Integer
Private Declare Function AUDIOGetAlbumW Lib "AudioGenie3.dll" () As String
Private Declare Function AUDIOGetArtistW Lib "AudioGenie3.dll" () As String
Private Declare Function AUDIOGetBitrateW Lib "AudioGenie3.dll" () As Long
Private Declare Function AUDIOGetChannelModeW Lib "AudioGenie3.dll" () As String
Private Declare Function AUDIOGetChannelsW Lib "AudioGenie3.dll" () As Long
Private Declare Function AUDIOGetCommentW Lib "AudioGenie3.dll" () As String
Private Declare Function AUDIOGetComposerW Lib "AudioGenie3.dll" () As String
Private Declare Function AUDIOGetDurationMillisW Lib "AudioGenie3.dll" () As Long
Private Declare Function AUDIOGetDurationW Lib "AudioGenie3.dll" () As Single
Private Declare Function AUDIOGetFileSizeW Lib "AudioGenie3.dll" () As Long
Private Declare Function AUDIOGetGenreW Lib "AudioGenie3.dll" () As String
Private Declare Function AUDIOGetLastErrorNumberW Lib "AudioGenie3.dll" () As Long
Private Declare Function AUDIOGetLastErrorTextW Lib "AudioGenie3.dll" () As String
Private Declare Function AUDIOGetLastFileW Lib "AudioGenie3.dll" () As String
Private Declare Function AUDIOGetMD5ValueW Lib "AudioGenie3.dll" () As String
Private Declare Function AUDIOGetSampleRateW Lib "AudioGenie3.dll" () As Long
Private Declare Function AUDIOGetTitleW Lib "AudioGenie3.dll" () As String
Private Declare Function AUDIOGetTrackW Lib "AudioGenie3.dll" () As String
Private Declare Function AUDIOGetVersionW Lib "AudioGenie3.dll" () As String
Private Declare Function AUDIOGetYearW Lib "AudioGenie3.dll" () As String
Private Declare Function AUDIOSaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function AUDIOSaveChangesW Lib "AudioGenie3.dll" () As Integer
Private Declare Sub AUDIOSetAlbumW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub AUDIOSetArtistW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub AUDIOSetCommentW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub AUDIOSetComposerW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub AUDIOSetGenreW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub AUDIOSetTitleW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub AUDIOSetTrackW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub AUDIOSetYearW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Function FLACGetBitsPerSampleW Lib "AudioGenie3.dll" () As Long
Private Declare Function FLACGetCompressionRatioW Lib "AudioGenie3.dll" () As Single
Private Declare Function FLACGetItemKeysW Lib "AudioGenie3.dll" () As String
Private Declare Function FLACGetMaxBlockSizeW Lib "AudioGenie3.dll" () As Long
Private Declare Function FLACGetMaxFrameSizeW Lib "AudioGenie3.dll" () As Long
Private Declare Function FLACGetMinBlockSizeW Lib "AudioGenie3.dll" () As Long
Private Declare Function FLACGetMinFrameSizeW Lib "AudioGenie3.dll" () As Long
Private Declare Function FLACGetSamplesW Lib "AudioGenie3.dll" () As Long
Private Declare Function FLACGetUserItemW Lib "AudioGenie3.dll" (ByVal ItemKey As Long) As String
Private Declare Function FLACGetVendorW Lib "AudioGenie3.dll" () As String
Private Declare Function FLACSaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function FLACSaveChangesW Lib "AudioGenie3.dll" () As Integer
Private Declare Sub FLACSetUserItemW Lib "AudioGenie3.dll" (ByVal ItemKey As Long, ByVal textString As Long)
Private Declare Sub FLACSetVendorW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Function GetAudioGenieVersionW Lib "AudioGenie3.dll" () As String
Private Declare Function GetConfigValueW Lib "AudioGenie3.dll" (ByVal key As Long) As Long
Private Declare Function GetMD5ValueFromFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As String
Private Declare Function ID3V1ExistsW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V1GetAlbumW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V1GetArtistW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V1GetCommentW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V1GetGenreIDW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V1GetGenreItemW Lib "AudioGenie3.dll" (ByVal number As Integer) As String
Private Declare Function ID3V1GetGenreW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V1GetGenresW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V1GetTitleW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V1GetTrackW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V1GetVersionW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V1GetYearW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V1RemoveTagFromFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function ID3V1RemoveTagW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V1SaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function ID3V1SaveChangesW Lib "AudioGenie3.dll" () As Integer
Private Declare Sub ID3V1SetAlbumW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub ID3V1SetArtistW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub ID3V1SetCommentW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub ID3V1SetGenreIDW Lib "AudioGenie3.dll" (ByVal nNewValue As Integer)
Private Declare Sub ID3V1SetGenreW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub ID3V1SetTitleW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub ID3V1SetTrackW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub ID3V1SetYearW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Function ID3V2AddAudioEncryptionW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal URL As Long, ByVal PreviewStart As Integer, ByVal PreviewLength As Integer) As Integer
Private Declare Function ID3V2AddAudioSeekPointW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal start As Long, ByVal Length As Long, ByVal numbers As Integer, ByVal BitsPerPoint As Byte) As Integer
Private Declare Function ID3V2AddChapterW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal Title As Long, ByVal Description As Long, ByVal startTime As Long, ByVal endTime As Long) As Integer
Private Declare Function ID3V2AddChildElementW Lib "AudioGenie3.dll" (ByVal ParentTocID As Long, ByVal ChildID As Long) As Integer
Private Declare Function ID3V2AddCommentW Lib "AudioGenie3.dll" (ByVal Language As Long, ByVal Description As Long, ByVal text As Long) As Integer
Private Declare Function ID3V2AddCommercialFrameW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Price As Long, ByVal validUntil As Long, ByVal contactUrl As Long, ByVal receivedAs As Integer, ByVal Seller As Long, ByVal Description As Long) As Integer
Private Declare Function ID3V2AddEncryptionW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal URL As Long, ByVal symbol As Integer) As Integer
Private Declare Function ID3V2AddEqualisationW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Interpolationmethod As Byte, ByVal Identification As Long) As Integer
Private Declare Function ID3V2AddEventTimingCodesW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal TimestampFormat As Byte) As Integer
Private Declare Function ID3V2AddGeneralObjectW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Mime As Long, ByVal FileName As Long, ByVal Description As Long) As Integer
Private Declare Function ID3V2AddGroupIdentificationW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal Length As Long, ByVal URL As Long, ByVal symbol As Byte) As Integer
Private Declare Function ID3V2AddLinkedInformationW Lib "AudioGenie3.dll" (ByVal FrameIdentifier As Long, ByVal URL As Long, ByVal additionalData As Long) As Integer
Private Declare Function ID3V2AddLyricW Lib "AudioGenie3.dll" (ByVal Language As Long, ByVal Description As Long, ByVal text As Long) As Integer
Private Declare Function ID3V2AddMpegLocationLookupTableW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal Length As Long, ByVal Frames As Long, ByVal Bytes As Long, ByVal Milliseconds As Long, ByVal BytesDeviation As Byte, ByVal MillisecondsDeviation As Byte) As Integer
Private Declare Function ID3V2AddMusicCdIdentifierW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal Length As Long) As Integer
Private Declare Function ID3V2AddOwnershipW Lib "AudioGenie3.dll" (ByVal Price As Long, ByVal dateString As Long, ByVal Seller As Long) As Integer
Private Declare Function ID3V2AddPictureArrayW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Description As Long, ByVal PictureType As Integer) As Integer
Private Declare Function ID3V2AddPictureFileW Lib "AudioGenie3.dll" (ByVal FileName As Long, ByVal Description As Long, ByVal PictureType As Integer, ByVal asLink As Integer) As Integer
Private Declare Function ID3V2AddPopularimeterW Lib "AudioGenie3.dll" (ByVal Email As Long, ByVal Rating As Integer, ByVal Counter As Long) As Integer
Private Declare Function ID3V2AddPositionSynchronisationW Lib "AudioGenie3.dll" (ByVal TimestampFormat As Byte, ByVal Position As Long) As Integer
Private Declare Function ID3V2AddPrivateFrameW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal Length As Long, ByVal URL As Long) As Integer
Private Declare Function ID3V2AddRecommendedBufferSizeW Lib "AudioGenie3.dll" (ByVal BufferSize As Long, ByVal EmbeddedInfoFlag As Byte, ByVal offset As Long) As Integer
Private Declare Function ID3V2AddRelativeVolumeAdjustmentW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal Length As Long, ByVal Identifier As Long) As Integer
Private Declare Function ID3V2AddReverbW Lib "AudioGenie3.dll" (ByVal reverbLeft As Integer, ByVal reverbRight As Integer, ByVal bouncesLeft As Byte, ByVal bouncesRight As Byte, ByVal feedbackLeftToLeft As Byte, ByVal feedbackLeftToRight As Byte, ByVal feedbackRightToRight As Byte, ByVal feedbackRightToLeft As Byte, ByVal premixLeftToRight As Byte, ByVal premixRightToLeft As Byte) As Integer
Private Declare Function ID3V2AddSignatureFrameW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal Length As Long, ByVal GroupSymbol As Integer) As Integer
Private Declare Function ID3V2AddSyncLyricW Lib "AudioGenie3.dll" (ByVal Language As Long, ByVal Description As Long, ByVal text As Long, ByVal ContentType As Integer, ByVal TimestampFormat As Integer) As Integer
Private Declare Function ID3V2AddSynchronizedTempoW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal Length As Long, ByVal format As Integer) As Integer
Private Declare Function ID3V2AddTableOfContentW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal Title As Long, ByVal Description As Long, ByVal isOrdered As Integer) As Integer
Private Declare Function ID3V2AddUniqueFileIdentifierW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal Length As Long, ByVal Owner As Long) As Integer
Private Declare Function ID3V2AddUserFrameW Lib "AudioGenie3.dll" (ByVal Language As Long, ByVal text As Long) As Integer
Private Declare Function ID3V2AddUserTextW Lib "AudioGenie3.dll" (ByVal Description As Long, ByVal text As Long) As Integer
Private Declare Function ID3V2AddUserURLW Lib "AudioGenie3.dll" (ByVal Description As Long, ByVal URL As Long) As Integer
Private Declare Function ID3V2DeleteAddendumW Lib "AudioGenie3.dll" (ByVal ID As Long) As Integer
Private Declare Function ID3V2DeleteAllFramesW Lib "AudioGenie3.dll" (ByVal frameID As Long) As Integer
Private Declare Function ID3V2DeleteChildElementW Lib "AudioGenie3.dll" (ByVal ParentTocID As Long, ByVal ChildID As Long) As Integer
Private Declare Function ID3V2DeleteSelectedFrameW Lib "AudioGenie3.dll" (ByVal frameID As Long, ByVal Index As Integer) As Integer
Private Declare Function ID3V2DeleteSubFrameW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal frameID As Long) As Integer
Private Declare Function ID3V2ExistsW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetAddendumDescriptionW Lib "AudioGenie3.dll" (ByVal ID As Long) As String
Private Declare Function ID3V2GetAddendumTitleW Lib "AudioGenie3.dll" (ByVal ID As Long) As String
Private Declare Function ID3V2GetAddendumTypeW Lib "AudioGenie3.dll" (ByVal ID As Long) As Integer
Private Declare Function ID3V2GetAllFrameIDsW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V2GetAudioEncryptionDataW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Index As Integer) As Long
Private Declare Function ID3V2GetAudioEncryptionPreviewLengthW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetAudioEncryptionPreviewStartW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetAudioEncryptionURLW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetAudioSeekPointBitsPerIndexpointW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetAudioSeekPointDataW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long) As Long
Private Declare Function ID3V2GetAudioSeekPointLengthW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetAudioSeekPointNumberW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetAudioSeekPointStartW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetChapterEndTimeW Lib "AudioGenie3.dll" (ByVal ID As Long) As Long
Private Declare Function ID3V2GetChapterStartTimeW Lib "AudioGenie3.dll" (ByVal ID As Long) As Long
Private Declare Function ID3V2GetChildElementIDW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal Index As Integer) As String
Private Declare Function ID3V2GetChildElementsW Lib "AudioGenie3.dll" (ByVal ID As Long) As Integer
Private Declare Function ID3V2GetCommentDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetCommentLanguageW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetCommentW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetCommercialFrameContactURLW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetCommercialFrameDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetCommercialFramePictureMimeW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetCommercialFramePictureW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Index As Integer) As Long
Private Declare Function ID3V2GetCommercialFramePriceW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetCommercialFrameReceivedAsW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetCommercialFrameSellerNameW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetCommercialFrameValidUntilW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetEncodingW Lib "AudioGenie3.dll" (ByVal frameID As Long) As Integer
Private Declare Function ID3V2GetEncryptionDataW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Index As Integer) As Long
Private Declare Function ID3V2GetEncryptionSymbolW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetEncryptionURLW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetEqualisationAdjustmentBitsW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetEqualisationDataW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Index As Integer) As Long
Private Declare Function ID3V2GetEqualisationIdentificationW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetEqualisationInterpolationW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetEventTimingCodesDataW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long) As Long
Private Declare Function ID3V2GetEventTimingCodesTimestampFormatW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetFrameCountW Lib "AudioGenie3.dll" (ByVal frameID As Long) As Integer
Private Declare Function ID3V2GetGeneralObjectDataW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Index As Integer) As Long
Private Declare Function ID3V2GetGeneralObjectDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetGeneralObjectFilenameW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetGeneralObjectMimeW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetGenreW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V2GetGroupIdentificationDataW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Index As Integer) As Long
Private Declare Function ID3V2GetGroupIdentificationSymbolW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetGroupIdentificationURLW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetLinkedInformationAdditionalDataW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetLinkedInformationFrameIdentifierW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Long
Private Declare Function ID3V2GetLinkedInformationURLW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetLyricDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetLyricLanguageW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetLyricW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetMpegLocationLookupTableBytesDeviationW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetMpegLocationLookupTableBytesW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetMpegLocationLookupTableDataW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long) As Long
Private Declare Function ID3V2GetMpegLocationLookupTableFramesW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetMpegLocationLookupTableMillisecondsDeviationW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetMpegLocationLookupTableMillisecondsW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetMusicCdIdentifierDataW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long) As Long
Private Declare Function ID3V2GetOwnershipDateW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V2GetOwnershipPriceW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V2GetOwnershipSellerW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V2GetPictureArrayW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Index As Integer) As Long
Private Declare Function ID3V2GetPictureDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetPictureFileW Lib "AudioGenie3.dll" (ByVal FileName As Long, ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetPictureMimeW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetPictureTypeTextW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetPictureTypeW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetPlayCounterW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetPopularimeterCounterW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Long
Private Declare Function ID3V2GetPopularimeterEmailW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetPopularimeterRatingW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetPositionSynchronisationTimestampFormatW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetPositionSynchronisationValueW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetPossibleCHAPIDW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V2GetPossibleTOCIDW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V2GetPrivateFrameDataW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Index As Integer) As Long
Private Declare Function ID3V2GetPrivateFrameURLW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetRecommendedBufferSizeFlagW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetRecommendedBufferSizeOffsetW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetRecommendedBufferSizeValueW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetRelativeVolumeAdjustmentDataW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Index As Integer) As Long
Private Declare Function ID3V2GetRelativeVolumeAdjustmentIdentifierW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetReverbBouncesLeftW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetReverbBouncesRightW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetReverbFeedbackLeftToLeftW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetReverbFeedbackLeftToRightW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetReverbFeedbackRightToLeftW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetReverbFeedbackRightToRightW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetReverbLeftW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetReverbPremixLeftToRightW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetReverbPremixRightToLeftW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetReverbRightW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetSeekOffsetW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetSignatureFrameDataW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Index As Integer) As Long
Private Declare Function ID3V2GetSignatureFrameGroupSymbolW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetSizeW Lib "AudioGenie3.dll" () As Long
Private Declare Function ID3V2GetSubFrameDescriptionW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal Index As Integer) As String
Private Declare Function ID3V2GetSubFrameIDW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal Index As Integer) As Long
Private Declare Function ID3V2GetSubFrameImageTypeW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetSubFrameImageW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal ID As Long, ByVal Index As Integer) As Long
Private Declare Function ID3V2GetSubFrameTextW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal Index As Integer) As String
Private Declare Function ID3V2GetSubFrameTypeW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetSubFramesW Lib "AudioGenie3.dll" (ByVal ID As Long) As Integer
Private Declare Function ID3V2GetSyncLyricContentTypeW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetSyncLyricDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetSyncLyricLanguageW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetSyncLyricTimeFormatW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetSyncLyricW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetSynchronizedTempoDataW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long) As Long
Private Declare Function ID3V2GetSynchronizedTempoFormatW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2GetTOCIsOrderedW Lib "AudioGenie3.dll" (ByVal ID As Long) As Integer
Private Declare Function ID3V2GetTOCRootIDW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V2GetTextFrameW Lib "AudioGenie3.dll" (ByVal frameID As Long) As String
Private Declare Function ID3V2GetURLFrameW Lib "AudioGenie3.dll" (ByVal frameID As Long) As String
Private Declare Function ID3V2GetUniqueFileIdentifierDataW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Index As Integer) As Long
Private Declare Function ID3V2GetUniqueFileIdentifierOwnerW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetUserFrameLanguageW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetUserFrameW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetUserTextDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetUserTextW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetUserURLDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetUserURLW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function ID3V2GetVersionW Lib "AudioGenie3.dll" () As String
Private Declare Function ID3V2ImportCueFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function ID3V2RemoveTagFromFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function ID3V2RemoveTagW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2SaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function ID3V2SaveChangesW Lib "AudioGenie3.dll" () As Integer
Private Declare Function ID3V2SetAddendumDescriptionW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal newDescription As Long) As Integer
Private Declare Function ID3V2SetAddendumTitleW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal newTitle As Long) As Integer
Private Declare Function ID3V2SetChapterTimesW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal startTime As Long, ByVal endTime As Long) As Integer
Private Declare Function ID3V2SetFormatAndEncodingW Lib "AudioGenie3.dll" (ByVal format As Integer, ByVal encoding As Integer) As Integer
Private Declare Sub ID3V2SetGenreW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Function ID3V2SetImageSubFrameW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal ID As Long, ByVal Description As Long, ByVal PictureType As Integer) As Integer
Private Declare Sub ID3V2SetPlayCounterW Lib "AudioGenie3.dll" (ByVal Counter As Long)
Private Declare Function ID3V2AddSeekOffsetW Lib "AudioGenie3.dll" (ByVal offset As Long) As Integer
Private Declare Function ID3V2SetTOCIsOrderedW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal status As Integer) As Integer
Private Declare Sub ID3V2SetTextFrameW Lib "AudioGenie3.dll" (ByVal frameID As Long, ByVal textString As Long)
Private Declare Function ID3V2SetTextSubFrameW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal frameID As Long, ByVal textString As Long, ByVal Description As Long) As Integer
Private Declare Sub ID3V2SetURLFrameW Lib "AudioGenie3.dll" (ByVal frameID As Long, ByVal textString As Long)
Private Declare Function ID3V2SetURLSubFrameW Lib "AudioGenie3.dll" (ByVal ID As Long, ByVal frameID As Long, ByVal urlString As Long, ByVal Description As Long) As Integer
Private Declare Function LYRICSExistsW Lib "AudioGenie3.dll" () As Integer
Private Declare Function LYRICSGetAlbumW Lib "AudioGenie3.dll" () As String
Private Declare Function LYRICSGetArtistW Lib "AudioGenie3.dll" () As String
Private Declare Function LYRICSGetAuthorW Lib "AudioGenie3.dll" () As String
Private Declare Function LYRICSGetGenreW Lib "AudioGenie3.dll" () As String
Private Declare Function LYRICSGetImageLinkW Lib "AudioGenie3.dll" () As String
Private Declare Function LYRICSGetIndicationW Lib "AudioGenie3.dll" () As String
Private Declare Function LYRICSGetInformationW Lib "AudioGenie3.dll" () As String
Private Declare Function LYRICSGetLyricsW Lib "AudioGenie3.dll" () As String
Private Declare Function LYRICSGetSizeW Lib "AudioGenie3.dll" () As Long
Private Declare Function LYRICSGetStartPositionW Lib "AudioGenie3.dll" () As Long
Private Declare Function LYRICSGetTitleW Lib "AudioGenie3.dll" () As String
Private Declare Function LYRICSGetVersionW Lib "AudioGenie3.dll" () As String
Private Declare Function LYRICSRemoveTagFromFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function LYRICSRemoveTagW Lib "AudioGenie3.dll" () As Integer
Private Declare Function LYRICSSaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function LYRICSSaveChangesW Lib "AudioGenie3.dll" () As Integer
Private Declare Sub LYRICSSetAlbumW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub LYRICSSetArtistW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub LYRICSSetAuthorW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub LYRICSSetGenreW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub LYRICSSetImageLinkW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub LYRICSSetIndicationW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub LYRICSSetInformationW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub LYRICSSetLyricsW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub LYRICSSetTitleW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Function MONKEYGetBitsW Lib "AudioGenie3.dll" () As Integer
Private Declare Function MONKEYGetCompressionRatioW Lib "AudioGenie3.dll" () As Single
Private Declare Function MONKEYGetCompressionW Lib "AudioGenie3.dll" () As String
Private Declare Function MONKEYGetFramesW Lib "AudioGenie3.dll" () As Long
Private Declare Function MONKEYGetPeakW Lib "AudioGenie3.dll" () As Single
Private Declare Function MONKEYGetSamplesPerFrameW Lib "AudioGenie3.dll" () As Long
Private Declare Function MONKEYGetSamplesW Lib "AudioGenie3.dll" () As Long
Private Declare Function MP4GetiTuneFrameW Lib "AudioGenie3.dll" (ByVal frameID As Long) As String
Private Declare Function MP4GetTextFrameW Lib "AudioGenie3.dll" (ByVal frameID As Long) As String
Private Declare Function MP4GetAllFrameIDsW Lib "AudioGenie3.dll" () As String
Private Declare Function MP4GetGenreW Lib "AudioGenie3.dll" () As String
Private Declare Function MP4GetPictureArrayW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Index As Integer) As Long
Private Declare Function MP4GetPictureMimeW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function MP4GetPictureCountW Lib "AudioGenie3.dll" () As Long
Private Declare Function MP4GetPictureFileW Lib "AudioGenie3.dll" (ByVal FileName As Long, ByVal Index As Integer) As Integer
Private Declare Function MP4GetTrackW Lib "AudioGenie3.dll" () As String
Private Declare Function MP4GetVersionW Lib "AudioGenie3.dll" () As String
Private Declare Function MP4SaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function MP4SaveChangesW Lib "AudioGenie3.dll" () As Integer
Private Declare Sub MP4DeleteEntriesW Lib "AudioGenie3.dll" ()
Private Declare Sub MP4SetGenreW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub MP4SetiTuneFrameW Lib "AudioGenie3.dll" (ByVal frameID As Long, ByVal newText As Long)
Private Declare Sub MP4SetTextFrameW Lib "AudioGenie3.dll" (ByVal frameID As Long, ByVal newText As Long)
Private Declare Sub MP4SetTrackW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Function MP4AddPictureArrayW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal Length As Long) As Integer
Private Declare Function MP4AddPictureFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Sub MP4DeletePicturesW Lib "AudioGenie3.dll" ()
Private Declare Function MP4DeletePictureW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function MPEGGetEmphasisW Lib "AudioGenie3.dll" () As String
Private Declare Function MPEGGetEncoderW Lib "AudioGenie3.dll" () As String
Private Declare Function MPEGGetFramePositionW Lib "AudioGenie3.dll" () As Long
Private Declare Function MPEGGetFrameSizeW Lib "AudioGenie3.dll" () As Long
Private Declare Function MPEGGetFramesW Lib "AudioGenie3.dll" () As Long
Private Declare Function MPEGGetLayerW Lib "AudioGenie3.dll" () As String
Private Declare Function MPEGGetVersionW Lib "AudioGenie3.dll" () As String
Private Declare Function MPEGIsCopyrightedW Lib "AudioGenie3.dll" () As Integer
Private Declare Function MPEGIsOriginalW Lib "AudioGenie3.dll" () As Integer
Private Declare Function MPEGIsPaddingW Lib "AudioGenie3.dll" () As Integer
Private Declare Function MPEGIsPrivateW Lib "AudioGenie3.dll" () As Integer
Private Declare Function MPEGIsProtectedW Lib "AudioGenie3.dll" () As Integer
Private Declare Function MPEGIsVBRW Lib "AudioGenie3.dll" () As Integer
Private Declare Function MPEGSetCopyrightedW Lib "AudioGenie3.dll" (ByVal FileName As Long, ByVal newValue As Integer) As Integer
Private Declare Function MPEGSetOriginalW Lib "AudioGenie3.dll" (ByVal FileName As Long, ByVal newValue As Integer) As Integer
Private Declare Function MPEGSetPrivateW Lib "AudioGenie3.dll" (ByVal FileName As Long, ByVal newValue As Integer) As Integer
Private Declare Function MPPGetFramesW Lib "AudioGenie3.dll" () As Long
Private Declare Function MPPGetStreamVersionW Lib "AudioGenie3.dll" () As Integer
Private Declare Function OGGGetAlbumW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetArtistW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetBitRateNominalW Lib "AudioGenie3.dll" () As Long
Private Declare Function OGGGetCommentW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetCopyrightW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetDateW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetDescriptionW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetGenreW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetISRCW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetItemKeysW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetLicenseW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetLocationW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetOrganizationW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetPerformerW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetTitleW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetTrackW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetUserItemW Lib "AudioGenie3.dll" (ByVal ItemKey As Long) As String
Private Declare Function OGGGetVendorW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGGetVersionW Lib "AudioGenie3.dll" () As String
Private Declare Function OGGRemoveTagFromFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function OGGRemoveTagW Lib "AudioGenie3.dll" () As Integer
Private Declare Function OGGSaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function OGGSaveChangesW Lib "AudioGenie3.dll" () As Integer
Private Declare Sub OGGSetAlbumW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetArtistW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetCommentW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetCopyrightW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetDateW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetDescriptionW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetGenreW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetISRCW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetLicenseW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetLocationW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetOrganizationW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetPerformerW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetTitleW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetTrackW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetUserItemW Lib "AudioGenie3.dll" (ByVal ItemKey As Long, ByVal textString As Long)
Private Declare Sub OGGSetVendorW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub OGGSetVersionW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Function OGGGetContactW Lib "AudioGenie3.dll" () As String
Private Declare Sub OGGSetContactW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Sub SetConfigValueW Lib "AudioGenie3.dll" (ByVal key As Long, ByVal value As Long)
Private Declare Sub SetLogFileW Lib "AudioGenie3.dll" (ByVal FileName As Long)
Private Declare Function WAVGetBitsPerSampleW Lib "AudioGenie3.dll" () As Long
Private Declare Function WAVGetBlockAlignW Lib "AudioGenie3.dll" () As Long
Private Declare Function WAVGetBytesPerSecondW Lib "AudioGenie3.dll" () As Long
Private Declare Function WAVGetCartChunkEntryW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function WAVGetChannelsW Lib "AudioGenie3.dll" () As Long
Private Declare Function WAVGetFormatIDW Lib "AudioGenie3.dll" () As Integer
Private Declare Function WAVGetFormatW Lib "AudioGenie3.dll" () As String
Private Declare Function WAVGetHeaderSizeW Lib "AudioGenie3.dll" () As Long
Private Declare Function WAVGetInfoChunkIDsW Lib "AudioGenie3.dll" () As String
Private Declare Function WAVGetTextFrameW Lib "AudioGenie3.dll" (ByVal frameID As Long) As String
Private Declare Function WAVSaveChangesW Lib "AudioGenie3.dll" () As Integer
Private Declare Function WAVSaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Sub WAVSetCartChunkEntryW Lib "AudioGenie3.dll" (ByVal Index As Integer, ByVal textString As Long)
Private Declare Sub WAVSetTextFrameW Lib "AudioGenie3.dll" (ByVal frameID As Long, ByVal textString As Long)
Private Declare Function WMADeletePictureW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Sub WMADeletePicturesW Lib "AudioGenie3.dll" ()
Private Declare Function WMAGetItemKeysW Lib "AudioGenie3.dll" () As String
Private Declare Function WMAGetPictureArrayW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Index As Integer) As Long
Private Declare Function WMAGetPictureDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function WMAGetPictureFileW Lib "AudioGenie3.dll" (ByVal FileName As Long, ByVal Index As Integer) As Integer
Private Declare Function WMAGetPictureMimeW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function WMAGetPictureTypeW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function WMAGetPictureCountW Lib "AudioGenie3.dll" () As Integer
Private Declare Function FLACAddPictureArrayW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal Length As Long, ByVal Description As Long, ByVal PictureType As Integer) As Integer
Private Declare Function FLACAddPictureFileW Lib "AudioGenie3.dll" (ByVal FileName As Long, ByVal Description As Long, ByVal PictureType As Integer, ByVal asLink As Integer) As Integer
Private Declare Function FLACDeletePictureW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Sub FLACDeletePicturesW Lib "AudioGenie3.dll" ()
Private Declare Function FLACGetPictureArrayW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Index As Integer) As Long
Private Declare Function FLACGetPictureCountW Lib "AudioGenie3.dll" () As Integer
Private Declare Function FLACGetPictureDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function FLACGetPictureFileW Lib "AudioGenie3.dll" (ByVal FileName As Long, ByVal Index As Integer) As Integer
Private Declare Function FLACGetPictureMimeW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function FLACGetPictureSizeW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Long
Private Declare Function FLACGetPictureTypeTextW Lib "AudioGenie3.dll" (ByVal Index As Integer) As String
Private Declare Function FLACGetPictureTypeW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Integer
Private Declare Function ID3V2GetPictureSizeW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Long
Private Declare Function MP4GetPictureSizeW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Long
Private Declare Function WMAGetPictureSizeW Lib "AudioGenie3.dll" (ByVal Index As Integer) As Long
Private Declare Function WAVGetDisplayTextW Lib "AudioGenie3.dll" () As String
Private Declare Sub WAVSetDisplayTextW Lib "AudioGenie3.dll" (ByVal textString As Long)
Private Declare Function WMAGetUserItemW Lib "AudioGenie3.dll" (ByVal ItemKey As Long) As String
Private Declare Function WMAIsVBRW Lib "AudioGenie3.dll" () As Integer
Private Declare Function WMASaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Long) As Integer
Private Declare Function WMASaveChangesW Lib "AudioGenie3.dll" () As Integer
Private Declare Function WMAAddPictureArrayW Lib "AudioGenie3.dll" (ByVal arr As Long, ByVal maxLen As Long, ByVal Description As Long, ByVal PictureType As Integer, ByVal Index As Integer) As Integer
Private Declare Function WMAAddPictureFileW Lib "AudioGenie3.dll" (ByVal FileName As Long, ByVal Description As Long, ByVal PictureType As Integer, ByVal Index As Integer) As Integer
Private Declare Sub WMASetUserItemW Lib "AudioGenie3.dll" (ByVal ItemKey As Long, ByVal textString As Long)

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
  TTA = 10
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

Public Enum CONFIG_VALUES
  CF_MPEGEXACTREAD = 0
  CF_ID3V2PADDINGSIZE = 1
  CF_ID3V2WRITEBLOCKSIZE = 2
  CF_DOEVENTSMILLIS = 3
End Enum

Public Enum ID3V2FRAMES
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
   ID3F_TCMP = &H54434D50 'ITUNES
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

Public Enum WAVFRAMES
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
   WAV_ISBJ = &H4953424A ' Subject
   WAV_ISFT = &H49534654 ' Software
   WAV_ISHP = &H49534850 ' Sharpness
   WAV_ISRC = &H49535243 ' Source
   WAV_ISRF = &H49535246 ' SourceForm
   WAV_ITCH = &H49544348 ' Technician
   WAV_ITRK = &H4954524B ' Track
End Enum

Public Enum WAVCARTENTRIES
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

Public Enum MP4FRAMES
   MP4_ALBUM = &HA9616C62           ' ©alb
   MP4_ARTIST = &HA9415254          ' ©ART
   MP4_AUTHOR = &HA9617574          ' ©aut
   MP4_ALBUMARTIST = &H61415254     ' aART
   MP4_BEATSPERMINUTE = &H746D706F  ' tmpo
   MP4_CATEGORY = &H63617467        ' catg
   MP4_CDSETNUMBER = &H6469736B     ' disk
   MP4_COMMENT = &HA9636D74         ' ©cmt
   MP4_COMPOSER = &HA9777274        ' ©wrt
   MP4_COMPILATION = &H6370696C     ' cpil
   MP4_DAY = &HA9646179             ' ©day
   MP4_COPYRIGHT = &HA9637079       ' ©cpy
   MP4_CREATEDATE = &HA9646179      ' ©day
   MP4_DESCRIPTION = &HA9646573     ' ©des
   MP4_INFORMATION = &HA9696E66     ' ©inf
   MP4_DIRECTOR = &HA9646972        ' ©dir
   MP4_DISCLAIMER = &HA9646973      ' ©dis
   MP4_ENCODER = &HA9746F6F         ' ©too
   MP4_GROUPING = &HA9677270        ' ©grp
   MP4_KEYWORD = &H6B657977         ' keyw
   MP4_LYRICS = &HA96C7972          ' ©lyr
   MP4_TITLE = &HA96E616D           ' ©nam
   MP4_NETURL = &HA975726C          ' ©url
   MP4_ORIGINALARTIST = &HA96F7065  ' ©ope
   MP4_ORIGINALFORMAT = &HA9666D74  ' ©fmt
   MP4_ORIGINALSOURCE = &HA9737263  ' ©src
   MP4_PERFORMER = &HA9707266       ' ©prf
   MP4_PRODUCER = &HA9707264        ' ©prd
   MP4_WARNING = &HA977726E         ' ©wrn
End Enum

Const WM_ALBUMARTIST = "WM/AlbumArtist"
Const WM_ALBUMTITLE = "WM/AlbumTitle"
Const WM_AUTHOR = "WM/Author"
Const WM_AUTHORURL = "WM/AuthorURL"
Const WM_BEATSPERMINUTE = "WM/BeatsPerMinute"
Const WM_CATEGORY = "WM/Category"
Const WM_COMPOSER = "WM/Composer"
Const WM_CONDUCTOR = "WM/Conductor"
Const WM_CONTENTDISTRIBUTOR = "WM/ContentDistributor"
Const WM_CONTENTGROUPDESCRIPTION = "WM/ContentGroupDescription"
Const WM_DESCRIPTION = "WM/Description"
Const WM_ENCODEDBY = "WM/EncodedBy"
Const WM_ENCODINGSETTINGS = "WM/EncodingSettings"
Const WM_ENCODINGTIME = "WM/EncodingTime"
Const WM_GENRE = "WM/Genre"
Const WM_INITIALKEY = "WM/InitialKey"
Const WM_ISRC = "WM/ISRC"
Const WM_LANGUAGE = "WM/Language"
Const WM_LYRICS = "WM/Lyrics"
Const WM_MOOD = "WM/Mood"
Const WM_PARENTALRATING = "WM/ParentalRating"
Const WM_PARTOFSET = "WM/PartOfSet"
Const WM_PERIOD = "WM/Period"
Const WM_PRODUCER = "WM/Producer"
Const WM_PROMOTIONURL = "WM/PromotionURL"
Const WM_PROVIDER = "WM/Provider"
Const WM_PROVIDERCOPYRIGHT = "WM/ProviderCopyright"
Const WM_PROVIDERRATING = "WM/ProviderRating"
Const WM_PUBLISHER = "WM/Publisher"
Const WM_TITLE = "WM/Title"
Const WM_TOOLNAME = "WM/ToolName"
Const WM_TOOLVERSION = "WM/ToolVersion"
Const WM_TRACK = "WM/Track"
Const WM_TRACKNUMBER = "WM/TrackNumber"
Const WM_USERWEBURL = "WM/UserWebURL"
Const WM_WRITER = "WM/Writer"
Const WM_YEAR = "WM/Year"


' *****************************************************
'                     Properties
' *****************************************************
Public Property Get APEAlbum() As String
  APEAlbum = StrConv(APEGetAlbumW(), vbFromUnicode)
End Property
Public Property Let APEAlbum(ByVal textString As String)
  Call APESetAlbumW(StrPtr(textString))
End Property

Public Property Get APEArtist() As String
  APEArtist = StrConv(APEGetArtistW(), vbFromUnicode)
End Property
Public Property Let APEArtist(ByVal textString As String)
  Call APESetArtistW(StrPtr(textString))
End Property

Public Property Get APEComment() As String
  APEComment = StrConv(APEGetCommentW(), vbFromUnicode)
End Property
Public Property Let APEComment(ByVal textString As String)
  Call APESetCommentW(StrPtr(textString))
End Property

Public Property Get APECopyright() As String
  APECopyright = StrConv(APEGetCopyrightW(), vbFromUnicode)
End Property
Public Property Let APECopyright(ByVal textString As String)
  Call APESetCopyrightW(StrPtr(textString))
End Property

Public Property Get APEGenre() As String
  APEGenre = StrConv(APEGetGenreW(), vbFromUnicode)
End Property
Public Property Let APEGenre(ByVal textString As String)
  Call APESetGenreW(StrPtr(textString))
End Property

Public Property Get APETitle() As String
  APETitle = StrConv(APEGetTitleW(), vbFromUnicode)
End Property
Public Property Let APETitle(ByVal textString As String)
  Call APESetTitleW(StrPtr(textString))
End Property

Public Property Get APETrack() As String
  APETrack = StrConv(APEGetTrackW(), vbFromUnicode)
End Property
Public Property Let APETrack(ByVal textString As String)
  Call APESetTrackW(StrPtr(textString))
End Property

Public Property Get APEUserItem(ByVal key As String) As String
  APEUserItem = StrConv(APEGetUserItemW(StrPtr(key)), vbFromUnicode)
End Property
Public Property Let APEUserItem(ByVal key As String, ByVal textString As String)
  Call APESetUserItemW(StrPtr(key), StrPtr(textString))
End Property

Public Property Get APEYear() As String
  APEYear = StrConv(APEGetYearW(), vbFromUnicode)
End Property
Public Property Let APEYear(ByVal textString As String)
  Call APESetYearW(StrPtr(textString))
End Property

Public Property Get AUDIOAlbum() As String
  AUDIOAlbum = StrConv(AUDIOGetAlbumW(), vbFromUnicode)
End Property
Public Property Let AUDIOAlbum(ByVal textString As String)
  Call AUDIOSetAlbumW(StrPtr(textString))
End Property

Public Property Get AUDIOArtist() As String
  AUDIOArtist = StrConv(AUDIOGetArtistW(), vbFromUnicode)
End Property
Public Property Let AUDIOArtist(ByVal textString As String)
  Call AUDIOSetArtistW(StrPtr(textString))
End Property

Public Property Get AUDIOComment() As String
  AUDIOComment = StrConv(AUDIOGetCommentW(), vbFromUnicode)
End Property
Public Property Let AUDIOComment(ByVal textString As String)
  Call AUDIOSetCommentW(StrPtr(textString))
End Property

Public Property Get AUDIOComposer() As String
  AUDIOComposer = StrConv(AUDIOGetComposerW(), vbFromUnicode)
End Property
Public Property Let AUDIOComposer(ByVal textString As String)
  Call AUDIOSetComposerW(StrPtr(textString))
End Property

Public Property Get AUDIOGenre() As String
  AUDIOGenre = StrConv(AUDIOGetGenreW(), vbFromUnicode)
End Property
Public Property Let AUDIOGenre(ByVal textString As String)
  Call AUDIOSetGenreW(StrPtr(textString))
End Property

Public Property Get AUDIOTitle() As String
  AUDIOTitle = StrConv(AUDIOGetTitleW(), vbFromUnicode)
End Property
Public Property Let AUDIOTitle(ByVal textString As String)
  Call AUDIOSetTitleW(StrPtr(textString))
End Property

Public Property Get AUDIOTrack() As String
  AUDIOTrack = StrConv(AUDIOGetTrackW(), vbFromUnicode)
End Property
Public Property Let AUDIOTrack(ByVal textString As String)
  Call AUDIOSetTrackW(StrPtr(textString))
End Property

Public Property Get AUDIOYear() As String
  AUDIOYear = StrConv(AUDIOGetYearW(), vbFromUnicode)
End Property
Public Property Let AUDIOYear(ByVal textString As String)
  Call AUDIOSetYearW(StrPtr(textString))
End Property

Public Property Get ConfigValue(ByVal key As CONFIG_VALUES) As Long
  ConfigValue = GetConfigValueW(key)
End Property
Public Property Let ConfigValue(ByVal key As CONFIG_VALUES, ByVal value As Long)
  Call SetConfigValueW(key, value)
End Property

Public Property Get FLACAlbum() As String
  FLACAlbum = StrConv(FLACGetUserItemW(StrPtr("ALBUM")), vbFromUnicode)
End Property
Public Property Let FLACAlbum(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("ALBUM"), StrPtr(textString))
End Property

Public Property Get FLACArranger() As String
  FLACArranger = StrConv(FLACGetUserItemW(StrPtr("ARRANGER")), vbFromUnicode)
End Property
Public Property Let FLACArranger(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("ARRANGER"), StrPtr(textString))
End Property

Public Property Get FLACArtist() As String
  FLACArtist = StrConv(FLACGetUserItemW(StrPtr("ARTIST")), vbFromUnicode)
End Property
Public Property Let FLACArtist(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("ARTIST"), StrPtr(textString))
End Property

Public Property Get FLACAuthor() As String
  FLACAuthor = StrConv(FLACGetUserItemW(StrPtr("AUTHOR")), vbFromUnicode)
End Property
Public Property Let FLACAuthor(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("AUTHOR"), StrPtr(textString))
End Property

Public Property Get FLACComment() As String
  FLACComment = StrConv(FLACGetUserItemW(StrPtr("COMMENT")), vbFromUnicode)
End Property
Public Property Let FLACComment(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("COMMENT"), StrPtr(textString))
End Property

Public Property Get FLACComposer() As String
  FLACComposer = StrConv(FLACGetUserItemW(StrPtr("COMPOSER")), vbFromUnicode)
End Property
Public Property Let FLACComposer(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("COMPOSER"), StrPtr(textString))
End Property

Public Property Get FLACContact() As String
  FLACContact = StrConv(FLACGetUserItemW(StrPtr("CONTACT")), vbFromUnicode)
End Property
Public Property Let FLACContact(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("CONTACT"), StrPtr(textString))
End Property

Public Property Get FLACCopyright() As String
  FLACCopyright = StrConv(FLACGetUserItemW(StrPtr("COPYRIGHT")), vbFromUnicode)
End Property
Public Property Let FLACCopyright(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("COPYRIGHT"), StrPtr(textString))
End Property

Public Property Get FLACDate() As String
  FLACDate = StrConv(FLACGetUserItemW(StrPtr("DATE")), vbFromUnicode)
End Property
Public Property Let FLACDate(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("DATE"), StrPtr(textString))
End Property

Public Property Get FLACDescription() As String
  FLACDescription = StrConv(FLACGetUserItemW(StrPtr("DESCRIPTION")), vbFromUnicode)
End Property
Public Property Let FLACDescription(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("DESCRIPTION"), StrPtr(textString))
End Property

Public Property Get FLACDiscNumber() As String
  FLACDiscNumber = StrConv(FLACGetUserItemW(StrPtr("DISCNUMBER")), vbFromUnicode)
End Property
Public Property Let FLACDiscNumber(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("DISCNUMBER"), StrPtr(textString))
End Property

Public Property Get FLACEnsemble() As String
  FLACEnsemble = StrConv(FLACGetUserItemW(StrPtr("ENSEMBLE")), vbFromUnicode)
End Property
Public Property Let FLACEnsemble(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("ENSEMBLE"), StrPtr(textString))
End Property

Public Property Get FLACGenre() As String
  FLACGenre = StrConv(FLACGetUserItemW(StrPtr("GENRE")), vbFromUnicode)
End Property
Public Property Let FLACGenre(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("GENRE"), StrPtr(textString))
End Property

Public Property Get FLACISRC() As String
  FLACISRC = StrConv(FLACGetUserItemW(StrPtr("ISRC")), vbFromUnicode)
End Property
Public Property Let FLACISRC(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("ISRC"), StrPtr(textString))
End Property

Public Property Get FLACLicense() As String
  FLACLicense = StrConv(FLACGetUserItemW(StrPtr("LICENSE")), vbFromUnicode)
End Property
Public Property Let FLACLicense(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("LICENSE"), StrPtr(textString))
End Property

Public Property Get FLACLocation() As String
  FLACLocation = StrConv(FLACGetUserItemW(StrPtr("LOCATION")), vbFromUnicode)
End Property
Public Property Let FLACLocation(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("LOCATION"), StrPtr(textString))
End Property

Public Property Get FLACLyricist() As String
  FLACLyricist = StrConv(FLACGetUserItemW(StrPtr("LYRICIST")), vbFromUnicode)
End Property
Public Property Let FLACLyricist(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("LYRICIST"), StrPtr(textString))
End Property

Public Property Get FLACOrganization() As String
  FLACOrganization = StrConv(FLACGetUserItemW(StrPtr("ORGANIZATION")), vbFromUnicode)
End Property
Public Property Let FLACOrganization(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("ORGANIZATION"), StrPtr(textString))
End Property

Public Property Get FLACPerformer() As String
  FLACPerformer = StrConv(FLACGetUserItemW(StrPtr("PERFORMER")), vbFromUnicode)
End Property
Public Property Let FLACPerformer(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("PERFORMER"), StrPtr(textString))
End Property

Public Property Get FLACPublisher() As String
  FLACPublisher = StrConv(FLACGetUserItemW(StrPtr("PUBLISHER")), vbFromUnicode)
End Property
Public Property Let FLACPublisher(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("PUBLISHER"), StrPtr(textString))
End Property

Public Property Get FLACTitle() As String
  FLACTitle = StrConv(FLACGetUserItemW(StrPtr("TITLE")), vbFromUnicode)
End Property
Public Property Let FLACTitle(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("TITLE"), StrPtr(textString))
End Property

Public Property Get FLACTrack() As String
  FLACTrack = StrConv(FLACGetUserItemW(StrPtr("TRACKNUMBER")), vbFromUnicode)
End Property
Public Property Let FLACTrack(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("TRACKNUMBER"), StrPtr(textString))
End Property

Public Property Get FLACUserItem(ByVal ItemKey As String) As String
  FLACUserItem = StrConv(FLACGetUserItemW(StrPtr(ItemKey)), vbFromUnicode)
End Property
Public Property Let FLACUserItem(ByVal ItemKey As String, ByVal textString As String)
  Call FLACSetUserItemW(StrPtr(ItemKey), StrPtr(textString))
End Property

Public Property Get FLACVendor() As String
  FLACVendor = StrConv(FLACGetVendorW(), vbFromUnicode)
End Property
Public Property Let FLACVendor(ByVal textString As String)
  Call FLACSetVendorW(StrPtr(textString))
End Property

Public Property Get FLACVersion() As String
  FLACVersion = StrConv(FLACGetUserItemW(StrPtr("VERSION")), vbFromUnicode)
End Property
Public Property Let FLACVersion(ByVal textString As String)
  Call FLACSetUserItemW(StrPtr("VERSION"), StrPtr(textString))
End Property

Public Property Get ID3V1Album() As String
  ID3V1Album = StrConv(ID3V1GetAlbumW(), vbFromUnicode)
End Property
Public Property Let ID3V1Album(ByVal textString As String)
  Call ID3V1SetAlbumW(StrPtr(textString))
End Property

Public Property Get ID3V1Artist() As String
  ID3V1Artist = StrConv(ID3V1GetArtistW(), vbFromUnicode)
End Property
Public Property Let ID3V1Artist(ByVal textString As String)
  Call ID3V1SetArtistW(StrPtr(textString))
End Property

Public Property Get ID3V1Comment() As String
  ID3V1Comment = StrConv(ID3V1GetCommentW(), vbFromUnicode)
End Property
Public Property Let ID3V1Comment(ByVal textString As String)
  Call ID3V1SetCommentW(StrPtr(textString))
End Property

Public Property Get ID3V1GenreID() As Integer
  ID3V1GenreID = ID3V1GetGenreIDW()
End Property
Public Property Let ID3V1GenreID(ByVal nNewValue As Integer)
  Call ID3V1SetGenreIDW(nNewValue)
End Property

Public Property Get ID3V1Genre() As String
  ID3V1Genre = StrConv(ID3V1GetGenreW(), vbFromUnicode)
End Property
Public Property Let ID3V1Genre(ByVal textString As String)
  Call ID3V1SetGenreW(StrPtr(textString))
End Property

Public Property Get ID3V1Title() As String
  ID3V1Title = StrConv(ID3V1GetTitleW(), vbFromUnicode)
End Property
Public Property Let ID3V1Title(ByVal textString As String)
  Call ID3V1SetTitleW(StrPtr(textString))
End Property

Public Property Get ID3V1Track() As String
  ID3V1Track = StrConv(ID3V1GetTrackW(), vbFromUnicode)
End Property
Public Property Let ID3V1Track(ByVal textString As String)
  Call ID3V1SetTrackW(StrPtr(textString))
End Property

Public Property Get ID3V1Year() As String
  ID3V1Year = StrConv(ID3V1GetYearW(), vbFromUnicode)
End Property
Public Property Let ID3V1Year(ByVal textString As String)
  Call ID3V1SetYearW(StrPtr(textString))
End Property

Public Property Get ID3V2Album() As String ' TALB
  ID3V2Album = ID3V2TextFrame(ID3F_TALB)
End Property
Public Property Let ID3V2Album(ByVal newValue As String) ' TALB
   ID3V2TextFrame(ID3F_TALB) = newValue
End Property

Public Property Get ID3V2AlbumArtistSort() As String ' TSO2 ITunes
  ID3V2AlbumArtistSort = ID3V2TextFrame(ID3F_TSO2)
End Property
Public Property Let ID3V2AlbumArtistSort(ByVal newValue As String) ' TSO2
  ID3V2TextFrame(ID3F_TSO2) = newValue
End Property

Public Property Get ID3V2AlbumSort() As String ' TSOA ITunes
  ID3V2AlbumSort = ID3V2TextFrame(ID3F_TSOA)
End Property
Public Property Let ID3V2AlbumSort(ByVal newValue As String) ' TSOA
  ID3V2TextFrame(ID3F_TSOA) = newValue
End Property

Public Property Get ID3V2Artist() As String ' TPE1
  ID3V2Artist = ID3V2TextFrame(ID3F_TPE1)
End Property
Public Property Let ID3V2Artist(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TPE1) = newValue
End Property

Public Property Get ID3V2ArtistSort() As String ' TSOP itunes
  ID3V2ArtistSort = ID3V2TextFrame(ID3F_TSOP)
End Property
Public Property Let ID3V2ArtistSort(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TSOP) = newValue
End Property

Public Property Get ID3V2ArtistURL() As String ' WOAR
  ID3V2ArtistURL = ID3V2URLFrame(ID3F_WOAR)
End Property
Public Property Let ID3V2ArtistURL(ByVal newValue As String)
  ID3V2URLFrame(ID3F_WOAR) = newValue
End Property

Public Property Get ID3V2AudiofileURL() As String ' WOAF
  ID3V2AudiofileURL = ID3V2URLFrame(ID3F_WOAF)
End Property
Public Property Let ID3V2AudiofileURL(ByVal newValue As String)
  ID3V2URLFrame(ID3F_WOAF) = newValue
End Property

Public Property Get ID3V2AudiosourceURL() As String ' WOAS
  ID3V2AudiosourceURL = ID3V2URLFrame(ID3F_WOAS)
End Property
Public Property Let ID3V2AudiosourceURL(ByVal newValue As String)
   ID3V2URLFrame(ID3F_WOAS) = newValue
End Property

Public Property Get ID3V2BeatsPerMinute() As String  ' TBPM
  ID3V2BeatsPerMinute = ID3V2TextFrame(ID3F_TBPM)
End Property
Public Property Let ID3V2BeatsPerMinute(ByVal newValue As String)
   ID3V2TextFrame(ID3F_TBPM) = newValue
End Property

Public Property Get ID3V2Category() As String ' TIT1
  ID3V2Category = ID3V2TextFrame(ID3F_TIT1)
End Property
Public Property Let ID3V2Category(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TIT1) = newValue
End Property

Public Property Get ID3V2CommercialURL() As String ' WCOM
  ID3V2CommercialURL = ID3V2URLFrame(ID3F_WCOM)
End Property
Public Property Let ID3V2CommercialURL(ByVal newValue As String) ' WCOM
  ID3V2URLFrame(ID3F_WCOM) = newValue
End Property

Public Property Get ID3V2Composer() As String ' TCOM
  ID3V2Composer = ID3V2TextFrame(ID3F_TCOM)
End Property
Public Property Let ID3V2Composer(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TCOM) = newValue
End Property

Public Property Get ID3V2ComposerSort() As String ' TSOC ITunes
  ID3V2ComposerSort = ID3V2TextFrame(ID3F_TSOC)
End Property
Public Property Let ID3V2ComposerSort(ByVal newValue As String) ' TSOC
  ID3V2TextFrame(ID3F_TSOC) = newValue
End Property

Public Property Get ID3V2Conductor() As String ' TPE3
  ID3V2Conductor = ID3V2TextFrame(ID3F_TPE3)
End Property
Public Property Let ID3V2Conductor(ByVal newValue As String) ' TPE3
  ID3V2TextFrame(ID3F_TPE3) = newValue
End Property

Public Property Get ID3V2ContentType() As String ' TCON
  ID3V2Conductor = ID3V2TextFrame(ID3F_TCON)
End Property
Public Property Let ID3V2ContentType(ByVal newValue As String) ' TCON
  ID3V2TextFrame(ID3F_TCON) = newValue
End Property

Public Property Get ID3V2Copyright() As String ' TCOP
  ID3V2Copyright = ID3V2TextFrame(ID3F_TCOP)
End Property
Public Property Let ID3V2Copyright(ByVal newValue As String)
 ID3V2TextFrame(ID3F_TCOP) = newValue
End Property

Public Property Get ID3V2CopyrightURL() As String ' WCOP
  ID3V2CopyrightURL = ID3V2URLFrame(ID3F_WCOP)
End Property
Public Property Let ID3V2CopyrightURL(ByVal newValue As String)
  ID3V2URLFrame(ID3F_WCOP) = newValue
End Property

Public Property Get ID3V2Date() As String ' TDAT
  ID3V2Date = ID3V2TextFrame(ID3F_TDAT)
End Property
Public Property Let ID3V2Date(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TDAT) = newValue
End Property

Public Property Get ID3V2Encoder() As String ' TENC
  ID3V2Encoder = ID3V2TextFrame(ID3F_TENC)
End Property
Public Property Let ID3V2Encoder(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TENC) = newValue
End Property

Public Property Get ID3V2EncodingSettings() As String ' TSSE
  ID3V2EncodingSettings = ID3V2TextFrame(ID3F_TSSE)
End Property
Public Property Let ID3V2EncodingSettings(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TSSE) = newValue
End Property

Public Property Get ID3V2EncodingTime() As String ' TDEN
  ID3V2EncodingTime = ID3V2TextFrame(ID3F_TDEN)
End Property
Public Property Let ID3V2EncodingTime(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TDEN) = newValue
End Property

Public Property Get ID3V2FileOwner() As String ' TOWN
  ID3V2FileOwner = ID3V2TextFrame(ID3F_TOWN)
End Property
Public Property Let ID3V2FileOwner(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TOWN) = newValue
End Property

Public Property Get ID3V2FileType() As String ' TFLT
  ID3V2FileType = ID3V2TextFrame(ID3F_TFLT)
End Property
Public Property Let ID3V2FileType(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TFLT) = newValue
End Property

Public Property Get ID3V2Genre() As String
  ID3V2Genre = StrConv(ID3V2GetGenreW(), vbFromUnicode)
End Property
Public Property Let ID3V2Genre(ByVal textString As String)
  Call ID3V2SetGenreW(StrPtr(textString))
End Property

Public Property Get ID3V2InetRadioName() As String ' TRSN
  ID3V2InetRadioName = ID3V2TextFrame(ID3F_TRSN)
End Property
Public Property Let ID3V2InetRadioName(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TRSN) = newValue
End Property

Public Property Get ID3V2InetRadioOwner() As String ' TRSO
  ID3V2InetRadioOwner = ID3V2TextFrame(ID3F_TRSO)
End Property
Public Property Let ID3V2InetRadioOwner(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TRSO) = newValue
End Property

Public Property Get ID3V2InitialKey() As String ' TKEY
  ID3V2InitialKey = ID3V2TextFrame(ID3F_TKEY)
End Property
Public Property Let ID3V2InitialKey(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TKEY) = newValue
End Property

Public Property Get ID3V2Interpreted() As String ' TPE4
  ID3V2Interpreted = ID3V2TextFrame(ID3F_TPE4)
End Property
Public Property Let ID3V2Interpreted(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TPE4) = newValue
End Property

Public Property Get ID3V2InvolvedPeopleList() As String ' TIPL && IPLS
  ID3V2InvolvedPeopleList = ID3V2TextFrame(ID3F_TIPL)
End Property
Public Property Let ID3V2InvolvedPeopleList(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TIPL) = newValue
End Property

Public Property Get ID3V2ISRC() As String ' TSRC
  ID3V2ISRC = ID3V2TextFrame(ID3F_TSRC)
End Property
Public Property Let ID3V2ISRC(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TSRC) = newValue
End Property

Public Property Get ID3V2Language() As String ' TLAN
  ID3V2Language = ID3V2TextFrame(ID3F_TLAN)
End Property
Public Property Let ID3V2Language(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TLAN) = newValue
End Property

Public Property Get ID3V2Length() As String ' TLEN
  ID3V2Length = ID3V2TextFrame(ID3F_TLEN)
End Property
Public Property Let ID3V2Length(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TLEN) = newValue
End Property

Public Property Get ID3V2MediaType() As String ' TMED
  ID3V2MediaType = ID3V2TextFrame(ID3F_TMED)
End Property
Public Property Let ID3V2MediaType(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TMED) = newValue
End Property

Public Property Get ID3V2Mood() As String ' TMOO
  ID3V2Mood = ID3V2TextFrame(ID3F_TMOO)
End Property
Public Property Let ID3V2Mood(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TMOO) = newValue
End Property

Public Property Get ID3V2MusicianCreditsList() As String ' TMCL
  ID3V2MusicianCreditsList = ID3V2TextFrame(ID3F_TMCL)
End Property
Public Property Let ID3V2MusicianCreditsList(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TMCL) = newValue
End Property

Public Property Get ID3V2Orchestra() As String ' TPE2
  ID3V2Orchestra = ID3V2TextFrame(ID3F_TPE2)
End Property
Public Property Let ID3V2Orchestra(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TPE2) = newValue
End Property

Public Property Get ID3V2OriginalAlbum() As String ' TOAL
  ID3V2OriginalAlbum = ID3V2TextFrame(ID3F_TOAL)
End Property
Public Property Let ID3V2OriginalAlbum(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TOAL) = newValue
End Property

Public Property Get ID3V2OriginalArtist() As String ' TOPE
  ID3V2OriginalArtist = ID3V2TextFrame(ID3F_TOPE)
End Property
Public Property Let ID3V2OriginalArtist(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TOPE) = newValue
End Property

Public Property Get ID3V2OriginalFilename() As String ' TOFN
  ID3V2OriginalFilename = ID3V2TextFrame(ID3F_TOFN)
End Property
Public Property Let ID3V2OriginalFilename(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TOFN) = newValue
End Property

Public Property Get ID3V2OriginalReleaseYear() As String ' TORY
  ID3V2OriginalReleaseYear = ID3V2TextFrame(ID3F_TORY)
End Property
Public Property Let ID3V2OriginalReleaseYear(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TORY) = newValue
End Property

Public Property Get ID3V2OriginalReleaseTime() As String ' TDOR
  ID3V2OriginalReleaseTime = ID3V2TextFrame(ID3F_TDOR)
End Property
Public Property Let ID3V2OriginalReleaseTime(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TDOR) = newValue
End Property

Public Property Get ID3V2OriginalTextwriter() As String ' TOLY
  ID3V2OriginalTextwriter = ID3V2TextFrame(ID3F_TOLY)
End Property
Public Property Let ID3V2OriginalTextwriter(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TOLY) = newValue
End Property

Public Property Get ID3V2PartofSet() As String ' TPOS
  ID3V2PartofSet = ID3V2TextFrame(ID3F_TPOS)
End Property
Public Property Let ID3V2PartofSet(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TPOS) = newValue
End Property

Public Property Get ID3V2PaymentURL() As String ' WPAY
  ID3V2PaymentURL = ID3V2URLFrame(ID3F_WPAY)
End Property
Public Property Let ID3V2PaymentURL(ByVal newValue As String)
 ID3V2URLFrame(ID3F_WPAY) = newValue
End Property

Public Property Get ID3V2PlayCounter() As Long
  ID3V2PlayCounter = ID3V2GetPlayCounterW()
End Property
Public Property Let ID3V2PlayCounter(ByVal Counter As Long)
  Call ID3V2SetPlayCounterW(Counter)
End Property

Public Property Get ID3V2PlaylistDelay() As String ' TDLY
  ID3V2PlaylistDelay = ID3V2TextFrame(ID3F_TDLY)
End Property
Public Property Let ID3V2PlaylistDelay(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TDLY) = newValue
End Property

Public Property Get ID3V2ProducedNotice() As String ' TPRO
  ID3V2ProducedNotice = ID3V2TextFrame(ID3F_TPRO)
End Property
Public Property Let ID3V2ProducedNotice(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TPRO) = newValue
End Property

Public Property Get ID3V2Publisher() As String ' TPUB
  ID3V2Publisher = ID3V2TextFrame(ID3F_TPUB)
End Property
Public Property Let ID3V2Publisher(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TPUB) = newValue
End Property

Public Property Get ID3V2PublisherURL() As String ' WPUB
  ID3V2PublisherURL = ID3V2URLFrame(ID3F_WPUB)
End Property
Public Property Let ID3V2PublisherURL(ByVal newValue As String)
  ID3V2URLFrame(ID3F_WPUB) = newValue
End Property

Public Property Get ID3V2RadiostationURL() As String ' WORS
  ID3V2RadiostationURL = ID3V2URLFrame(ID3F_WORS)
End Property
Public Property Let ID3V2RadiostationURL(ByVal newValue As String)
  ID3V2URLFrame(ID3F_WORS) = newValue
End Property

Public Property Get ID3V2RecordingDate() As String ' TRDA
  ID3V2RecordingDate = ID3V2TextFrame(ID3F_TRDA)
End Property
Public Property Let ID3V2RecordingDate(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TRDA) = newValue
End Property

Public Property Get ID3V2RecordingTime() As String ' TDRC
  ID3V2RecordingTime = ID3V2TextFrame(ID3F_TDRC)
End Property
Public Property Let ID3V2RecordingTime(ByVal newValue As String) ' TDRC
  ID3V2TextFrame(ID3F_TDRC) = newValue
End Property

Public Property Get ID3V2ReleaseTime() As String ' TDRL
  ID3V2ReleaseTime = ID3V2TextFrame(ID3F_TDRL)
End Property
Public Property Let ID3V2ReleaseTime(ByVal newValue As String)
 ID3V2TextFrame(ID3F_TDRL) = newValue
End Property

Public Property Get ID3V2Size() As String ' TSIZ
  ID3V2Size = ID3V2TextFrame(ID3F_TSIZ)
End Property
Public Property Let ID3V2Size(ByVal newValue As String)
 ID3V2TextFrame(ID3F_TSIZ) = newValue
End Property

Public Property Get ID3V2Subtitle() As String ' TIT3
  ID3V2Subtitle = ID3V2TextFrame(ID3F_TIT3)
End Property
Public Property Let ID3V2Subtitle(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TIT3) = newValue
End Property

Public Property Get ID3V2SubtitleSet() As String ' TSST
  ID3V2SubtitleSet = ID3V2TextFrame(ID3F_TSST)
End Property
Public Property Let ID3V2SubtitleSet(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TSST) = newValue
End Property

Public Property Get ID3V2TaggingTime() As String ' TDTG
  ID3V2TaggingTime = ID3V2TextFrame(ID3F_TDTG)
End Property
Public Property Let ID3V2TaggingTime(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TDTG) = newValue
End Property

Public Property Get ID3V2TextWriter() As String ' TEXT
  ID3V2TextWriter = ID3V2TextFrame(ID3F_TEXT)
End Property
Public Property Let ID3V2TextWriter(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TEXT) = newValue
End Property

Public Property Get ID3V2Time() As String ' TIME
  ID3V2Time = ID3V2TextFrame(ID3F_TIME)
End Property
Public Property Let ID3V2Time(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TIME) = newValue
End Property

Public Property Get ID3V2Title() As String ' TIT2
  ID3V2Title = ID3V2TextFrame(ID3F_TIT2)
End Property
Public Property Let ID3V2Title(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TIT2) = newValue
End Property

Public Property Get ID3V2TitleSort() As String ' TSOT itunes
  ID3V2TitleSort = ID3V2TextFrame(ID3F_TSOT)
End Property
Public Property Let ID3V2TitleSort(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TSOT) = newValue
End Property
Public Property Get ID3V2Track() As String ' TRCK
  ID3V2Track = ID3V2TextFrame(ID3F_TRCK)
End Property
Public Property Let ID3V2Track(ByVal newValue As String)
  ID3V2TextFrame(ID3F_TRCK) = newValue
End Property

Public Property Get ID3V2TextFrame(ByVal frameID As ID3V2FRAMES) As String
  ID3V2TextFrame = StrConv(ID3V2GetTextFrameW(frameID), vbFromUnicode)
End Property
Public Property Let ID3V2TextFrame(ByVal frameID As ID3V2FRAMES, ByVal textString As String)
  Call ID3V2SetTextFrameW(frameID, StrPtr(textString))
End Property

Public Property Get ID3V2Year() As String ' TYER
  ID3V2Year = StrConv(ID3V2GetTextFrameW(ID3F_TYER), vbFromUnicode)
End Property
Public Property Let ID3V2Year(ByVal newValue As String)
  Call ID3V2SetTextFrameW(ID3F_TYER, StrPtr(newValue))
End Property

Public Property Get ID3V2URLFrame(ByVal frameID As Long) As String
  ID3V2URLFrame = StrConv(ID3V2GetURLFrameW(frameID), vbFromUnicode)
End Property
Public Property Let ID3V2URLFrame(ByVal frameID As Long, ByVal textString As String)
  Call ID3V2SetURLFrameW(frameID, StrPtr(textString))
End Property

Public Property Get LYRICSAlbum() As String
  LYRICSAlbum = StrConv(LYRICSGetAlbumW(), vbFromUnicode)
End Property
Public Property Let LYRICSAlbum(ByVal textString As String)
  Call LYRICSSetAlbumW(StrPtr(textString))
End Property

Public Property Get LYRICSArtist() As String
  LYRICSArtist = StrConv(LYRICSGetArtistW(), vbFromUnicode)
End Property
Public Property Let LYRICSArtist(ByVal textString As String)
  Call LYRICSSetArtistW(StrPtr(textString))
End Property

Public Property Get LYRICSAuthor() As String
  LYRICSAuthor = StrConv(LYRICSGetAuthorW(), vbFromUnicode)
End Property
Public Property Let LYRICSAuthor(ByVal textString As String)
  Call LYRICSSetAuthorW(StrPtr(textString))
End Property

Public Property Get LYRICSGenre() As String
  LYRICSGenre = StrConv(LYRICSGetGenreW(), vbFromUnicode)
End Property
Public Property Let LYRICSGenre(ByVal textString As String)
  Call LYRICSSetGenreW(StrPtr(textString))
End Property

Public Property Get LYRICSImageLink() As String
  LYRICSImageLink = StrConv(LYRICSGetImageLinkW(), vbFromUnicode)
End Property
Public Property Let LYRICSImageLink(ByVal textString As String)
  Call LYRICSSetImageLinkW(StrPtr(textString))
End Property

Public Property Get LYRICSIndication() As String
  LYRICSIndication = StrConv(LYRICSGetIndicationW(), vbFromUnicode)
End Property
Public Property Let LYRICSIndication(ByVal textString As String)
  Call LYRICSSetIndicationW(StrPtr(textString))
End Property

Public Property Get LYRICSInformation() As String
  LYRICSInformation = StrConv(LYRICSGetInformationW(), vbFromUnicode)
End Property
Public Property Let LYRICSInformation(ByVal textString As String)
  Call LYRICSSetInformationW(StrPtr(textString))
End Property

Public Property Get LYRICSLyrics() As String
  LYRICSLyrics = StrConv(LYRICSGetLyricsW(), vbFromUnicode)
End Property
Public Property Let LYRICSLyrics(ByVal textString As String)
  Call LYRICSSetLyricsW(StrPtr(textString))
End Property

Public Property Get LYRICSTitle() As String
  LYRICSTitle = StrConv(LYRICSGetTitleW(), vbFromUnicode)
End Property
Public Property Let LYRICSTitle(ByVal textString As String)
  Call LYRICSSetTitleW(StrPtr(textString))
End Property

Public Property Get MP4Album() As String
  MP4Album = MP4GetTextFrame(MP4_ALBUM)
End Property
Public Property Let MP4Album(ByVal textString As String)
  Call MP4SetTextFrame(MP4_ALBUM, textString)
End Property

Public Property Get MP4Artist() As String
  MP4Artist = MP4GetTextFrame(MP4_ARTIST)
End Property
Public Property Let MP4Artist(ByVal textString As String)
  Call MP4SetTextFrame(MP4_ARTIST, textString)
End Property

Public Property Get MP4Author() As String
  MP4Author = MP4GetTextFrame(MP4_AUTHOR)
End Property
Public Property Let MP4Author(ByVal textString As String)
  Call MP4SetTextFrame(MP4_AUTHOR, textString)
End Property

Public Property Get MP4BeatsPerMinute() As String
  MP4BeatsPerMinute = MP4GetTextFrame(MP4_BEATSPERMINUTE)
End Property
Public Property Let MP4BeatsPerMinute(ByVal textString As String)
  Call MP4SetTextFrame(MP4_BEATSPERMINUTE, textString)
End Property

Public Property Get MP4CDSetNumber() As String
  MP4CDSetNumber = MP4GetTextFrame(MP4_CDSETNUMBER)
End Property
Public Property Let MP4CDSetNumber(ByVal textString As String)
  Call MP4SetTextFrame(MP4_CDSETNUMBER, textString)
End Property

Public Property Get MP4Comment() As String
  MP4Comment = MP4GetTextFrame(MP4_COMMENT)
End Property
Public Property Let MP4Comment(ByVal textString As String)
  Call MP4SetTextFrame(MP4_COMMENT, textString)
End Property

Public Property Get MP4Compilation() As String
  MP4Compilation = MP4GetTextFrame(MP4_COMPILATION)
End Property
Public Property Let MP4Compilation(ByVal textString As String)
  Call MP4SetTextFrame(MP4_COMPILATION, textString)
End Property

Public Property Get MP4Composer() As String
  MP4Composer = MP4GetTextFrame(MP4_COMPOSER)
End Property
Public Property Let MP4Composer(ByVal textString As String)
  Call MP4SetTextFrame(MP4_COMPOSER, textString)
End Property

Public Property Get MP4Copyright() As String
  MP4Copyright = MP4GetTextFrame(MP4_COPYRIGHT)
End Property
Public Property Let MP4Copyright(ByVal textString As String)
  Call MP4SetTextFrame(MP4_COPYRIGHT, textString)
End Property

Public Property Get MP4Date() As String
  MP4Copyright = MP4GetTextFrame(MP4_CREATEDATE)
End Property
Public Property Let MP4Date(ByVal textString As String)
  Call MP4SetTextFrame(MP4_CREATEDATE, textString)
End Property

Public Property Get MP4Description() As String
  MP4Description = MP4GetTextFrame(MP4_DESCRIPTION)
End Property
Public Property Let MP4Description(ByVal textString As String)
  Call MP4SetTextFrame(MP4_DESCRIPTION, textString)
End Property

Public Property Get MP4Director() As String
  MP4Director = MP4GetTextFrame(MP4_DIRECTOR)
End Property
Public Property Let MP4Director(ByVal textString As String)
  Call MP4SetTextFrame(MP4_DIRECTOR, textString)
End Property

Public Property Get MP4Disclaimer() As String
  MP4Disclaimer = MP4GetTextFrame(MP4_DISCLAIMER)
End Property
Public Property Let MP4Disclaimer(ByVal textString As String)
  Call MP4SetTextFrame(MP4_DISCLAIMER, textString)
End Property

Public Property Get MP4Encoder() As String
  MP4Encoder = MP4GetTextFrame(MP4_ENCODER)
End Property
Public Property Let MP4Encoder(ByVal textString As String)
  Call MP4SetTextFrame(MP4_ENCODER, textString)
End Property

Public Property Get MP4Genre() As String
  MP4Genre = StrConv(MP4GetGenreW(), vbFromUnicode)
End Property
Public Property Let MP4Genre(ByVal textString As String)
  Call MP4SetGenreW(StrPtr(textString))
End Property

Public Property Get MP4NetUrl() As String
  MP4NetUrl = MP4GetTextFrame(MP4_NETURL)
End Property
Public Property Let MP4NetUrl(ByVal textString As String)
  Call MP4SetTextFrame(MP4_NETURL, textString)
End Property

Public Property Get MP4OriginalArtist() As String
  MP4OriginalArtist = MP4GetTextFrame(MP4_ORIGINALARTIST)
End Property
Public Property Let MP4OriginalArtist(ByVal textString As String)
  Call MP4SetTextFrame(MP4_ORIGINALARTIST, textString)
End Property

Public Property Get MP4OriginalFormat() As String
  MP4OriginalFormat = MP4GetTextFrame(MP4_ORIGINALFORMAT)
End Property
Public Property Let MP4OriginalFormat(ByVal textString As String)
  Call MP4SetTextFrame(MP4_ORIGINALFORMAT, textString)
End Property

Public Property Get MP4OriginalSource() As String
  MP4OriginalSource = MP4GetTextFrame(MP4_ORIGINALSOURCE)
End Property
Public Property Let MP4OriginalSource(ByVal textString As String)
  Call MP4SetTextFrame(MP4_ORIGINALSOURCE, textString)
End Property

Public Property Get MP4Performer() As String
  MP4Performer = MP4GetTextFrame(MP4_PERFORMER)
End Property
Public Property Let MP4Performer(ByVal textString As String)
  Call MP4SetTextFrame(MP4_PERFORMER, textString)
End Property

Public Property Get MP4Producer() As String
  MP4Producer = MP4GetTextFrame(MP4_PRODUCER)
End Property
Public Property Let MP4Producer(ByVal textString As String)
  Call MP4SetTextFrame(MP4_PRODUCER, textString)
End Property

Public Property Get MP4Title() As String
  MP4Title = MP4GetTextFrame(MP4_TITLE)
End Property
Public Property Let MP4Title(ByVal textString As String)
  Call MP4SetTextFrame(MP4_TITLE, textString)
End Property

Public Property Get MP4Track() As String
  MP4Track = StrConv(MP4GetTrackW(), vbFromUnicode)
End Property
Public Property Let MP4Track(ByVal textString As String)
  Call MP4SetTrackW(StrPtr(textString))
End Property

Public Property Get MP4Warning() As String
  MP4Warning = MP4GetTextFrame(MP4_WARNING)
End Property
Public Property Let MP4Warning(ByVal textString As String)
  Call MP4SetTextFrame(MP4_WARNING, textString)
End Property

' ---------------------------------------------------------------------------------------
Public Property Get OGGAlbum() As String
  OGGAlbum = StrConv(OGGGetAlbumW(), vbFromUnicode)
End Property
Public Property Let OGGAlbum(ByVal textString As String)
  Call OGGSetAlbumW(StrPtr(textString))
End Property

Public Property Get OGGArtist() As String
  OGGArtist = StrConv(OGGGetArtistW(), vbFromUnicode)
End Property
Public Property Let OGGArtist(ByVal textString As String)
  Call OGGSetArtistW(StrPtr(textString))
End Property

Public Property Get OGGComment() As String
  OGGComment = StrConv(OGGGetCommentW(), vbFromUnicode)
End Property
Public Property Let OGGComment(ByVal textString As String)
  Call OGGSetCommentW(StrPtr(textString))
End Property

Public Property Get OGGContact() As String
  OGGContact = StrConv(OGGGetContactW(), vbFromUnicode)
End Property
Public Property Let OGGContact(ByVal textString As String)
  Call OGGSetContactW(StrPtr(textString))
End Property

Public Property Get OGGCopyright() As String
  OGGCopyright = StrConv(OGGGetCopyrightW(), vbFromUnicode)
End Property
Public Property Let OGGCopyright(ByVal textString As String)
  Call OGGSetCopyrightW(StrPtr(textString))
End Property

Public Property Get OGGDate() As String
  OGGDate = StrConv(OGGGetDateW(), vbFromUnicode)
End Property
Public Property Let OGGDate(ByVal textString As String)
  Call OGGSetDateW(StrPtr(textString))
End Property

Public Property Get OGGDescription() As String
  OGGDescription = StrConv(OGGGetDescriptionW(), vbFromUnicode)
End Property
Public Property Let OGGDescription(ByVal textString As String)
  Call OGGSetDescriptionW(StrPtr(textString))
End Property

Public Property Get OGGGenre() As String
  OGGGenre = StrConv(OGGGetGenreW(), vbFromUnicode)
End Property
Public Property Let OGGGenre(ByVal textString As String)
  Call OGGSetGenreW(StrPtr(textString))
End Property

Public Property Get OGGISRC() As String
  OGGISRC = StrConv(OGGGetISRCW(), vbFromUnicode)
End Property
Public Property Let OGGISRC(ByVal textString As String)
  Call OGGSetISRCW(StrPtr(textString))
End Property

Public Property Get OGGLicense() As String
  OGGLicense = StrConv(OGGGetLicenseW(), vbFromUnicode)
End Property
Public Property Let OGGLicense(ByVal textString As String)
  Call OGGSetLicenseW(StrPtr(textString))
End Property

Public Property Get OGGLocation() As String
  OGGLocation = StrConv(OGGGetLocationW(), vbFromUnicode)
End Property
Public Property Let OGGLocation(ByVal textString As String)
  Call OGGSetLocationW(StrPtr(textString))
End Property

Public Property Get OGGOrganization() As String
  OGGOrganization = StrConv(OGGGetOrganizationW(), vbFromUnicode)
End Property
Public Property Let OGGOrganization(ByVal textString As String)
  Call OGGSetOrganizationW(StrPtr(textString))
End Property

Public Property Get OGGPerformer() As String
  OGGPerformer = StrConv(OGGGetPerformerW(), vbFromUnicode)
End Property
Public Property Let OGGPerformer(ByVal textString As String)
  Call OGGSetPerformerW(StrPtr(textString))
End Property

Public Property Get OGGTitle() As String
  OGGTitle = StrConv(OGGGetTitleW(), vbFromUnicode)
End Property
Public Property Let OGGTitle(ByVal textString As String)
  Call OGGSetTitleW(StrPtr(textString))
End Property

Public Property Get OGGTrack() As String
  OGGTrack = StrConv(OGGGetTrackW(), vbFromUnicode)
End Property
Public Property Let OGGTrack(ByVal textString As String)
  Call OGGSetTrackW(StrPtr(textString))
End Property

Public Property Get OGGUserItem(ByVal ItemKey As String) As String
  OGGUserItem = StrConv(OGGGetUserItemW(StrPtr(ItemKey)), vbFromUnicode)
End Property
Public Property Let OGGUserItem(ByVal ItemKey As String, ByVal textString As String)
  Call OGGSetUserItemW(StrPtr(ItemKey), StrPtr(textString))
End Property

Public Property Get OGGVendor() As String
  OGGVendor = StrConv(OGGGetVendorW(), vbFromUnicode)
End Property
Public Property Let OGGVendor(ByVal textString As String)
  Call OGGSetVendorW(StrPtr(textString))
End Property

Public Property Get OGGVersion() As String
  OGGVersion = StrConv(OGGGetVersionW(), vbFromUnicode)
End Property
Public Property Let OGGVersion(ByVal textString As String)
  Call OGGSetVersionW(StrPtr(textString))
End Property

Public Property Get WAVArchivalLocation() As String
  WAVArchivalLocation = WAVTextFrame(WAV_IARL)
End Property
Public Property Let WAVArchivalLocation(ByVal textString As String)
  WAVTextFrame(WAV_IARL) = textString
End Property

Public Property Get WAVArtist() As String
  WAVArtist = WAVTextFrame(WAV_IART)
End Property
Public Property Let WAVArtist(ByVal textString As String)
  WAVTextFrame(WAV_IART) = textString
End Property

Public Property Get WAVComissioned() As String
  WAVComissioned = WAVTextFrame(WAV_ICMS)
End Property
Public Property Let WAVComissioned(ByVal textString As String)
  WAVTextFrame(WAV_ICMS) = textString
End Property

Public Property Get WAVComment() As String
  WAVComment = WAVTextFrame(WAV_ICMT)
End Property
Public Property Let WAVComment(ByVal textString As String)
  WAVTextFrame(WAV_ICMT) = textString
End Property

Public Property Get WAVCopyright() As String
  WAVCopyright = WAVTextFrame(WAV_ICOP)
End Property
Public Property Let WAVCopyright(ByVal textString As String)
  WAVTextFrame(WAV_ICOP) = textString
End Property

Public Property Get WAVCreationDate() As String
  WAVCreationDate = WAVTextFrame(WAV_ICRD)
End Property
Public Property Let WAVCreationDate(ByVal textString As String)
  WAVTextFrame(WAV_ICRD) = textString
End Property

Public Property Get WAVCropped() As String
  WAVCropped = WAVTextFrame(WAV_ICRP)
End Property
Public Property Let WAVCropped(ByVal textString As String)
  WAVTextFrame(WAV_ICRP) = textString
End Property

Public Property Get WAVDimension() As String
  WAVDimension = WAVTextFrame(WAV_IDIM)
End Property
Public Property Let WAVDimension(ByVal textString As String)
  WAVTextFrame(WAV_IDIM) = textString
End Property

Public Property Get WAVDotsPerInch() As String
  WAVDotsPerInch = WAVTextFrame(WAV_IDPI)
End Property
Public Property Let WAVDotsPerInch(ByVal textString As String)
  WAVTextFrame(WAV_IDPI) = textString
End Property

Public Property Get WAVEngineer() As String
  WAVEngineer = WAVTextFrame(WAV_IENG)
End Property
Public Property Let WAVEngineer(ByVal textString As String)
  WAVTextFrame(WAV_IENG) = textString
End Property

Public Property Get WAVGenre() As String
  WAVGenre = WAVTextFrame(WAV_IGNR)
End Property
Public Property Let WAVGenre(ByVal textString As String)
  WAVTextFrame(WAV_IGNR) = textString
End Property

Public Property Get WAVKeywords() As String
  WAVKeywords = WAVTextFrame(WAV_IKEY)
End Property
Public Property Let WAVKeywords(ByVal textString As String)
  WAVTextFrame(WAV_IKEY) = textString
End Property

Public Property Get WAVLightness() As String
  WAVLightness = WAVTextFrame(WAV_ILGT)
End Property
Public Property Let WAVLightness(ByVal textString As String)
  WAVTextFrame(WAV_ILGT) = textString
End Property

Public Property Get WAVMedium() As String
  WAVMedium = WAVTextFrame(WAV_IMED)
End Property
Public Property Let WAVMedium(ByVal textString As String)
  WAVTextFrame(WAV_IMED) = textString
End Property

Public Property Get WAVName() As String
  WAVName = WAVTextFrame(WAV_INAM)
End Property
Public Property Let WAVName(ByVal textString As String)
  WAVTextFrame(WAV_INAM) = textString
End Property

Public Property Get WAVPaletteSetting() As String
  WAVPaletteSetting = WAVTextFrame(WAV_IPLT)
End Property
Public Property Let WAVPaletteSetting(ByVal textString As String)
  WAVTextFrame(WAV_IPLT) = textString
End Property

Public Property Get WAVProduct() As String
  WAVProduct = WAVTextFrame(WAV_IPRD)
End Property
Public Property Let WAVProduct(ByVal textString As String)
  WAVTextFrame(WAV_IPRD) = textString
End Property

Public Property Get WAVSharpness() As String
  WAVSharpness = WAVTextFrame(WAV_ISHP)
End Property
Public Property Let WAVSharpness(ByVal textString As String)
  WAVTextFrame(WAV_ISHP) = textString
End Property

Public Property Get WAVSoftware() As String
  WAVSoftware = WAVTextFrame(WAV_ISFT)
End Property
Public Property Let WAVSoftware(ByVal textString As String)
  WAVTextFrame(WAV_ISFT) = textString
End Property

Public Property Get WAVSourceForm() As String
  WAVSourceForm = WAVTextFrame(WAV_ISRF)
End Property
Public Property Let WAVSourceForm(ByVal textString As String)
  WAVTextFrame(WAV_ISRF) = textString
End Property

Public Property Get WAVSource() As String
  WAVSource = WAVTextFrame(WAV_ISRC)
End Property
Public Property Let WAVSource(ByVal textString As String)
  WAVTextFrame(WAV_ISRC) = textString
End Property

Public Property Get WAVSubject() As String
  WAVSubject = WAVTextFrame(WAV_ISBJ)
End Property
Public Property Let WAVSubject(ByVal textString As String)
  WAVTextFrame(WAV_ISBJ) = textString
End Property

Public Property Get WAVTechnician() As String
  WAVTechnician = WAVTextFrame(WAV_ITCH)
End Property
Public Property Let WAVTechnician(ByVal textString As String)
 WAVTextFrame(WAV_ITCH) = textString
End Property

Public Property Get WAVTextFrame(ByVal frameID As WAVFRAMES) As String
  WAVTextFrame = StrConv(WAVGetTextFrameW(frameID), vbFromUnicode)
End Property
Public Property Let WAVTextFrame(ByVal frameID As WAVFRAMES, ByVal textString As String)
  Call WAVSetTextFrameW(frameID, StrPtr(textString))
End Property

Public Property Get WAVCartChunkEntry(ByVal Index As WAVCARTENTRIES) As String
  WAVCartChunkEntry = StrConv(WAVGetCartChunkEntryW(Index), vbFromUnicode)
End Property
Public Property Let WAVCartChunkEntry(ByVal Index As WAVCARTENTRIES, ByVal textString As String)
  Call WAVSetCartChunkEntryW(Index, StrPtr(textString))
End Property

Public Property Get WAVTrack() As String
  WAVTrack = WAVTextFrame(WAV_ITRK)
End Property
Public Property Let WAVTrack(ByVal textString As String)
  WAVTextFrame(WAV_ITRK) = textString
End Property

Public Property Get WAVCartVersion() As String
  WAVCartVersion = WAVCartChunkEntry(WAV_CART_VERSION)
End Property
Public Property Let WAVCartVersion(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_VERSION) = textString
End Property

Public Property Get WAVCartTitle() As String
  WAVCartTitle = WAVCartChunkEntry(WAV_CART_TITLE)
End Property
Public Property Let WAVCartTitle(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_TITLE) = textString
End Property

Public Property Get WAVCartArtist() As String
  WAVCartArtist = WAVCartChunkEntry(WAV_CART_ARTIST)
End Property
Public Property Let WAVCartArtist(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_ARTIST) = textString
End Property

Public Property Get WAVCartCutID() As String
  WAVCartCutID = WAVCartChunkEntry(WAV_CART_CUTID)
End Property
Public Property Let WAVCartCutID(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_CUTID) = textString
End Property

Public Property Get WAVCartClientID() As String
  WAVCartClientID = WAVCartChunkEntry(WAV_CART_CLIENTID)
End Property
Public Property Let WAVCartClientID(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_CLIENTID) = textString
End Property

Public Property Get WAVCartCategory() As String
  WAVCartCategory = WAVCartChunkEntry(WAV_CART_CATEGORY)
End Property
Public Property Let WAVCartCategory(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_CATEGORY) = textString
End Property

Public Property Get WAVCartClassification() As String
  WAVCartClassification = WAVCartChunkEntry(WAV_CART_CLASSIFICATION)
End Property
Public Property Let WAVCartClassification(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_CLASSIFICATION) = textString
End Property

Public Property Get WAVCartOutCue() As String
  WAVCartOutCue = WAVCartChunkEntry(WAV_CART_OUTCUE)
End Property
Public Property Let WAVCartOutCue(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_OUTCUE) = textString
End Property

Public Property Get WAVCartStartDate() As String
  WAVCartStartDate = WAVCartChunkEntry(WAV_CART_STARTDATE)
End Property
Public Property Let WAVCartStartDate(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_STARTDATE) = textString
End Property

Public Property Get WAVCartStartTime() As String
  WAVCartStartTime = WAVCartChunkEntry(WAV_CART_STARTTIME)
End Property
Public Property Let WAVCartStartTime(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_STARTTIME) = textString
End Property

Public Property Get WAVCartEndDate() As String
  WAVCartEndDate = WAVCartChunkEntry(WAV_CART_ENDDATE)
End Property
Public Property Let WAVCartEndDate(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_ENDDATE) = textString
End Property

Public Property Get WAVCartEndTime() As String
  WAVCartEndTime = WAVCartChunkEntry(WAV_CART_ENDTIME)
End Property
Public Property Let WAVCartEndTime(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_ENDTIME) = textString
End Property

Public Property Get WAVCartProducerAppID() As String
  WAVCartProducerAppID = WAVCartChunkEntry(WAV_CART_PRODUCERAPPID)
End Property
Public Property Let WAVCartProducerAppID(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_PRODUCERAPPID) = textString
End Property

Public Property Get WAVCartProducerAppVersion() As String
  WAVCartProducerAppVersion = WAVCartChunkEntry(WAV_CART_PRODUCERAPPVERSION)
End Property
Public Property Let WAVCartProducerAppVersion(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_PRODUCERAPPVERSION) = textString
End Property

Public Property Get WAVCartUserDef() As String
  WAVCartUserDef = WAVCartChunkEntry(WAV_CART_USERDEF)
End Property
Public Property Let WAVCartUserDef(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_USERDEF) = textString
End Property

Public Property Get WAVCartLevelReference() As String
  WAVCartLevelReference = WAVCartChunkEntry(WAV_CART_LEVELREFERENCE)
End Property
Public Property Let WAVCartLevelReference(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_LEVELREFERENCE) = textString
End Property

Public Property Get WAVCartPostTimer0() As String
  WAVCartPostTimer0 = WAVCartChunkEntry(WAV_CART_POSTTIMER0)
End Property
Public Property Let WAVCartPostTimer0(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_POSTTIMER0) = textString
End Property

Public Property Get WAVCartPostTimer1() As String
  WAVCartPostTimer1 = WAVCartChunkEntry(WAV_CART_POSTTIMER1)
End Property
Public Property Let WAVCartPostTimer1(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_POSTTIMER1) = textString
End Property

Public Property Get WAVCartPostTimer2() As String
  WAVCartPostTimer2 = WAVCartChunkEntry(WAV_CART_POSTTIMER2)
End Property
Public Property Let WAVCartPostTimer2(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_POSTTIMER2) = textString
End Property

Public Property Get WAVCartPostTimer3() As String
  WAVCartPostTimer3 = WAVCartChunkEntry(WAV_CART_POSTTIMER3)
End Property
Public Property Let WAVCartPostTimer3(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_POSTTIMER3) = textString
End Property

Public Property Get WAVCartPostTimer4() As String
  WAVCartPostTimer4 = WAVCartChunkEntry(WAV_CART_POSTTIMER4)
End Property
Public Property Let WAVCartPostTimer4(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_POSTTIMER4) = textString
End Property

Public Property Get WAVCartPostTimer5() As String
  WAVCartPostTimer5 = WAVCartChunkEntry(WAV_CART_POSTTIMER5)
End Property
Public Property Let WAVCartPostTimer5(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_POSTTIMER5) = textString
End Property

Public Property Get WAVCartPostTimer6() As String
  WAVCartPostTimer6 = WAVCartChunkEntry(WAV_CART_POSTTIMER6)
End Property
Public Property Let WAVCartPostTimer6(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_POSTTIMER6) = textString
End Property

Public Property Get WAVCartPostTimer7() As String
  WAVCartPostTimer7 = WAVCartChunkEntry(WAV_CART_POSTTIMER7)
End Property
Public Property Let WAVCartPostTimer7(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_POSTTIMER7) = textString
End Property

Public Property Get WAVCartReserved() As String
  WAVCartReserved = WAVCartChunkEntry(WAV_CART_RESERVED)
End Property
Public Property Let WAVCartReserved(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_RESERVED) = textString
End Property

Public Property Get WAVCartURL() As String
  WAVCartURL = WAVCartChunkEntry(WAV_CART_URL)
End Property
Public Property Let WAVCartURL(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_URL) = textString
End Property

Public Property Get WAVCartTagText() As String
  WAVCartTagText = WAVCartChunkEntry(WAV_CART_TAGTEXT)
End Property
Public Property Let WAVCartTagText(ByVal textString As String)
  WAVCartChunkEntry(WAV_CART_TAGTEXT) = textString
End Property

Public Property Get WMAAlbumArtist() As String
  WMAAlbumArtist = StrConv(WMAGetUserItemW(StrPtr(WM_ALBUMARTIST)), vbFromUnicode)
End Property
Public Property Let WMAAlbumArtist(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_ALBUMARTIST), StrPtr(textString))
End Property

Public Property Get WMAAlbum() As String
  WMAAlbum = StrConv(WMAGetUserItemW(StrPtr(WM_ALBUMTITLE)), vbFromUnicode)
End Property
Public Property Let WMAAlbum(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_ALBUMTITLE), StrPtr(textString))
End Property

Public Property Get WMAArtist() As String
  WMAArtist = StrConv(WMAGetUserItemW(StrPtr(WM_AUTHOR)), vbFromUnicode)
End Property
Public Property Let WMAArtist(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_AUTHOR), StrPtr(textString))
End Property

Public Property Get WMAAuthorURL() As String
  WMAAuthorURL = StrConv(WMAGetUserItemW(StrPtr(WM_AUTHORURL)), vbFromUnicode)
End Property
Public Property Let WMAAuthorURL(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_AUTHORURL), StrPtr(textString))
End Property

Public Property Get WMABeatsPerMinute() As String
  WMABeatsPerMinute = StrConv(WMAGetUserItemW(StrPtr(WM_BEATSPERMINUTE)), vbFromUnicode)
End Property
Public Property Let WMABeatsPerMinute(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_BEATSPERMINUTE), StrPtr(textString))
End Property

Public Property Get WMACategory() As String
  WMACategory = StrConv(WMAGetUserItemW(StrPtr(WM_CATEGORY)), vbFromUnicode)
End Property
Public Property Let WMACategory(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_CATEGORY), StrPtr(textString))
End Property

Public Property Get WMAComment() As String
  WMAComment = StrConv(WMAGetUserItemW(StrPtr(WM_DESCRIPTION)), vbFromUnicode)
End Property
Public Property Let WMAComment(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_DESCRIPTION), StrPtr(textString))
End Property

Public Property Get WMAComposer() As String
  WMAComposer = StrConv(WMAGetUserItemW(StrPtr(WM_COMPOSER)), vbFromUnicode)
End Property
Public Property Let WMAComposer(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_COMPOSER), StrPtr(textString))
End Property

Public Property Get WMAConductor() As String
  WMAConductor = StrConv(WMAGetUserItemW(StrPtr(WM_CONDUCTOR)), vbFromUnicode)
End Property
Public Property Let WMAConductor(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_CONDUCTOR), StrPtr(textString))
End Property

Public Property Get WMAContentGroupDescription() As String
  WMAContentGroupDescription = StrConv(WMAGetUserItemW(StrPtr(WM_CONTENTGROUPDESCRIPTION)), vbFromUnicode)
End Property
Public Property Let WMAContentGroupDescription(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_CONTENTGROUPDESCRIPTION), StrPtr(textString))
End Property

Public Property Get WMACopyright() As String
  WMACopyright = StrConv(WMAGetUserItemW(StrPtr(WM_PROVIDERCOPYRIGHT)), vbFromUnicode)
End Property
Public Property Let WMACopyright(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_PROVIDERCOPYRIGHT), StrPtr(textString))
End Property

Public Property Get WMADistributor() As String
  WMADistributor = StrConv(WMAGetUserItemW(StrPtr(WM_CONTENTDISTRIBUTOR)), vbFromUnicode)
End Property
Public Property Let WMADistributor(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_CONTENTDISTRIBUTOR), StrPtr(textString))
End Property

Public Property Get WMAEncodedBy() As String
  WMAEncodedBy = StrConv(WMAGetUserItemW(StrPtr(WM_ENCODEDBY)), vbFromUnicode)
End Property
Public Property Let WMAEncodedBy(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_ENCODEDBY), StrPtr(textString))
End Property

Public Property Get WMAEncodingSettings() As String
  WMAEncodingSettings = StrConv(WMAGetUserItemW(StrPtr(WM_ENCODINGSETTINGS)), vbFromUnicode)
End Property
Public Property Let WMAEncodingtings(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_ENCODINGSETTINGS), StrPtr(textString))
End Property

Public Property Get WMAEncodingTime() As String
  WMAEncodingTime = StrConv(WMAGetUserItemW(StrPtr(WM_ENCODINGTIME)), vbFromUnicode)
End Property
Public Property Let WMAEncodingTime(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_ENCODINGTIME), StrPtr(textString))
End Property

Public Property Get WMAGenre() As String
  WMAGenre = StrConv(WMAGetUserItemW(StrPtr(WM_GENRE)), vbFromUnicode)
End Property
Public Property Let WMAGenre(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_GENRE), StrPtr(textString))
End Property

Public Property Get WMAISRC() As String
  WMAISRC = StrConv(WMAGetUserItemW(StrPtr(WM_ISRC)), vbFromUnicode)
End Property
Public Property Let WMAISRC(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_ISRC), StrPtr(textString))
End Property

Public Property Get WMAInitialKey() As String
  WMAInitialKey = StrConv(WMAGetUserItemW(StrPtr(WM_INITIALKEY)), vbFromUnicode)
End Property
Public Property Let WMAInitialKey(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_INITIALKEY), StrPtr(textString))
End Property

Public Property Get WMALanguage() As String
  WMALanguage = StrConv(WMAGetUserItemW(StrPtr(WM_LANGUAGE)), vbFromUnicode)
End Property
Public Property Let WMALanguage(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_LANGUAGE), StrPtr(textString))
End Property

Public Property Get WMALyrics() As String
  WMALyrics = StrConv(WMAGetUserItemW(StrPtr(WM_LYRICS)), vbFromUnicode)
End Property
Public Property Let WMALyrics(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_LYRICS), StrPtr(textString))
End Property

Public Property Get WMAMood() As String
  WMAMood = StrConv(WMAGetUserItemW(StrPtr(WM_MOOD)), vbFromUnicode)
End Property
Public Property Let WMAMood(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_MOOD), StrPtr(textString))
End Property

Public Property Get WMAParentalRating() As String
  WMAParentalRating = StrConv(WMAGetUserItemW(StrPtr(WM_PARENTALRATING)), vbFromUnicode)
End Property
Public Property Let WMAParentalRating(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_PARENTALRATING), StrPtr(textString))
End Property

Public Property Get WMAPartOfSet() As String
  WMAPartOfSet = StrConv(WMAGetUserItemW(StrPtr(WM_PARTOFSET)), vbFromUnicode)
End Property
Public Property Let WMAPartOf(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_PARTOFSET), StrPtr(textString))
End Property

Public Property Get WMAPeriod() As String
  WMAPeriod = StrConv(WMAGetUserItemW(StrPtr(WM_PERIOD)), vbFromUnicode)
End Property
Public Property Let WMAPeriod(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_PERIOD), StrPtr(textString))
End Property

Public Property Get WMAProducer() As String
  WMAProducer = StrConv(WMAGetUserItemW(StrPtr(WM_PRODUCER)), vbFromUnicode)
End Property
Public Property Let WMAProducer(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_PRODUCER), StrPtr(textString))
End Property

Public Property Get WMAPromotionURL() As String
  WMAPromotionURL = StrConv(WMAGetUserItemW(StrPtr(WM_PROMOTIONURL)), vbFromUnicode)
End Property
Public Property Let WMAPromotionURL(ByVal textString As String)
 Call WMASetUserItemW(StrPtr(WM_PROMOTIONURL), StrPtr(textString))
End Property

Public Property Get WMAProvider() As String
  WMAProvider = StrConv(WMAGetUserItemW(StrPtr(WM_PROVIDER)), vbFromUnicode)
End Property
Public Property Let WMAProvider(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_PROVIDER), StrPtr(textString))
End Property

Public Property Get WMAPublisher() As String
  WMAPublisher = StrConv(WMAGetUserItemW(StrPtr(WM_PUBLISHER)), vbFromUnicode)
End Property
Public Property Let WMAPublisher(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_PUBLISHER), StrPtr(textString))
End Property

Public Property Get WMAProviderRating() As String
  WMAProviderRating = StrConv(WMAGetUserItemW(StrPtr(WM_PROVIDERRATING)), vbFromUnicode)
End Property
Public Property Let WMAProviderRating(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_PROVIDERRATING), StrPtr(textString))
End Property

Public Property Get WMATitle() As String
  WMATitle = StrConv(WMAGetUserItemW(StrPtr(WM_TITLE)), vbFromUnicode)
End Property
Public Property Let WMATitle(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_TITLE), StrPtr(textString))
End Property

Public Property Get WMAToolName() As String
  WMAToolName = StrConv(WMAGetUserItemW(StrPtr(WM_TOOLNAME)), vbFromUnicode)
End Property
Public Property Let WMAToolName(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_TOOLNAME), StrPtr(textString))
End Property

Public Property Get WMAToolVersion() As String
  WMAToolVersion = StrConv(WMAGetUserItemW(StrPtr(WM_TOOLVERSION)), vbFromUnicode)
End Property
Public Property Let WMAToolVersion(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_TOOLVERSION), StrPtr(textString))
End Property

Public Property Get WMATrackNumber() As String
  WMATrackNumber = StrConv(WMAGetUserItemW(StrPtr(WM_TRACKNUMBER)), vbFromUnicode)
End Property
Public Property Let WMATrackNumber(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_TRACKNUMBER), StrPtr(textString))
End Property

Public Property Get WMAUserItem(ByVal ItemKey As String) As String
  WMAUserItem = StrConv(WMAGetUserItemW(StrPtr(ItemKey)), vbFromUnicode)
End Property
Public Property Let WMAUserItem(ByVal ItemKey As String, ByVal textString As String)
  Call WMASetUserItemW(StrPtr(ItemKey), StrPtr(textString))
End Property

Public Property Get WMAUserWebURL() As String
  WMAUserWebURL = StrConv(WMAGetUserItemW(StrPtr(WM_USERWEBURL)), vbFromUnicode)
End Property
Public Property Let WMAUserWebURL(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_USERWEBURL), StrPtr(textString))
End Property

Public Property Get WMAWriter() As String
  WMAWriter = StrConv(WMAGetUserItemW(StrPtr(WM_WRITER)), vbFromUnicode)
End Property
Public Property Let WMAWriter(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_WRITER), StrPtr(textString))
End Property

Public Property Get WMAYear() As String
  WMAYear = StrConv(WMAGetUserItemW(StrPtr(WM_YEAR)), vbFromUnicode)
End Property
Public Property Let WMAYear(ByVal textString As String)
  Call WMASetUserItemW(StrPtr(WM_YEAR), StrPtr(textString))
End Property

' *****************************************************
'                     Functions
' *****************************************************
Public Function AACGetBitRateType() As String
  AACGetBitRateType = StrConv(AACGetBitRateTypeW(), vbFromUnicode)
End Function

Public Function AACGetHeaderType() As String
  AACGetHeaderType = StrConv(AACGetHeaderTypeW(), vbFromUnicode)
End Function

Public Function AACGetProfile() As String
  AACGetProfile = StrConv(AACGetProfileW(), vbFromUnicode)
End Function

Public Function APEExists() As Integer
  APEExists = APEExistsW()
End Function

Public Function APEGetItemKeys() As String
  APEGetItemKeys = StrConv(APEGetItemKeysW(), vbFromUnicode)
End Function

Public Function APEGetSize() As Long
  APEGetSize = APEGetSizeW()
End Function

Public Function APEGetVersion() As String
  APEGetVersion = StrConv(APEGetVersionW(), vbFromUnicode)
End Function

Public Function APERemoveTagFromFile(ByVal FileName As String) As Integer
  APERemoveTagFromFile = APERemoveTagFromFileW(StrPtr(FileName))
End Function

Public Function APERemoveTag() As Integer
  APERemoveTag = APERemoveTagW()
End Function

Public Function APESaveChangesToFile(ByVal FileName As String) As Integer
  APESaveChangesToFile = APESaveChangesToFileW(StrPtr(FileName))
End Function

Public Function APESaveChanges() As Integer
  APESaveChanges = APESaveChangesW()
End Function

Public Function AUDIOAnalyzeFile(ByVal FileName As String) As Integer
  AUDIOAnalyzeFile = AUDIOAnalyzeFileW(StrPtr(FileName))
End Function

Public Function AUDIOIsValidFormat() As Integer
  AUDIOIsValidFormat = AUDIOIsValidFormatW()
End Function

Public Function AUDIOGetBitrate() As Long
  AUDIOGetBitrate = AUDIOGetBitrateW()
End Function

Public Function AUDIOGetChannelMode() As String
  AUDIOGetChannelMode = StrConv(AUDIOGetChannelModeW(), vbFromUnicode)
End Function

Public Function AUDIOGetChannels() As Long
  AUDIOGetChannels = AUDIOGetChannelsW()
End Function

Public Function AUDIOGetDurationMillis() As Long
  AUDIOGetDurationMillis = AUDIOGetDurationMillisW()
End Function

Public Function AUDIOGetDuration() As Single
  AUDIOGetDuration = AUDIOGetDurationW()
End Function

Public Function AUDIOGetFileSize() As Long
  AUDIOGetFileSize = AUDIOGetFileSizeW()
End Function

Public Function AUDIOGetLastErrorNumber() As Long
  AUDIOGetLastErrorNumber = AUDIOGetLastErrorNumberW()
End Function

Public Function AUDIOGetLastErrorText() As String
  AUDIOGetLastErrorText = StrConv(AUDIOGetLastErrorTextW(), vbFromUnicode)
End Function

Public Function AUDIOGetLastFile() As String
  AUDIOGetLastFile = StrConv(AUDIOGetLastFileW(), vbFromUnicode)
End Function

Public Function AUDIOGetMD5Value() As String
  AUDIOGetMD5Value = StrConv(AUDIOGetMD5ValueW(), vbFromUnicode)
End Function

Public Function AUDIOGetSampleRate() As Long
  AUDIOGetSampleRate = AUDIOGetSampleRateW()
End Function

Public Function AUDIOGetVersion() As String
  AUDIOGetVersion = StrConv(AUDIOGetVersionW(), vbFromUnicode)
End Function

Public Function AUDIOSaveChangesToFile(ByVal FileName As String) As Integer
  AUDIOSaveChangesToFile = AUDIOSaveChangesToFileW(StrPtr(FileName))
End Function

Public Function AUDIOSaveChanges() As Integer
  AUDIOSaveChanges = AUDIOSaveChangesW()
End Function

Public Function GetAudioGenieVersion() As String
  GetAudioGenieVersion = StrConv(GetAudioGenieVersionW(), vbFromUnicode)
End Function

Public Function FLACGetBitsPerSample() As Long
  FLACGetBitsPerSample = FLACGetBitsPerSampleW()
End Function

Public Function FLACGetCompressionRatio() As Single
  FLACGetCompressionRatio = FLACGetCompressionRatioW()
End Function

Public Function FLACGetItemKeys() As String
  FLACGetItemKeys = StrConv(FLACGetItemKeysW(), vbFromUnicode)
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

Public Function FLACSaveChangesToFile(ByVal FileName As String) As Integer
  FLACSaveChangesToFile = FLACSaveChangesToFileW(StrPtr(FileName))
End Function

Public Function FLACSaveChanges() As Integer
  FLACSaveChanges = FLACSaveChangesW()
End Function

Public Function GetMD5ValueFromFile(ByVal FileName As String) As String
  GetMD5ValueFromFile = StrConv(GetMD5ValueFromFileW(StrPtr(FileName)), vbFromUnicode)
End Function

Public Function ID3V1Exists() As Integer
  ID3V1Exists = ID3V1ExistsW()
End Function

Public Function ID3V1GetGenreItem(ByVal number As Integer) As String
  ID3V1GetGenreItem = StrConv(ID3V1GetGenreItemW(number), vbFromUnicode)
End Function

Public Function ID3V1GetGenres() As Integer
  ID3V1GetGenres = ID3V1GetGenresW()
End Function

Public Function ID3V1GetVersion() As String
  ID3V1GetVersion = StrConv(ID3V1GetVersionW(), vbFromUnicode)
End Function

Public Function ID3V1RemoveTagFromFile(ByVal FileName As String) As Integer
  ID3V1RemoveTagFromFile = ID3V1RemoveTagFromFileW(StrPtr(FileName))
End Function

Public Function ID3V1RemoveTag() As Integer
  ID3V1RemoveTag = ID3V1RemoveTagW()
End Function

Public Function ID3V1SaveChangesToFile(ByVal FileName As String) As Integer
  ID3V1SaveChangesToFile = ID3V1SaveChangesToFileW(StrPtr(FileName))
End Function

Public Function ID3V1SaveChanges() As Integer
  ID3V1SaveChanges = ID3V1SaveChangesW()
End Function

Public Function ID3V2AddChapter(ByVal ID As String, ByVal Title As String, ByVal Description As String, ByVal startTime As Long, ByVal endTime As Long) As Integer
  ID3V2AddChapter = ID3V2AddChapterW(StrPtr(ID), StrPtr(Title), StrPtr(Description), startTime, endTime)
End Function
Public Function ID3V2AddChildElement(ByVal TocID As String, ByVal ChildID As String) As Integer
  ID3V2AddChildElement = ID3V2AddChildElementW(StrPtr(TocID), StrPtr(ChildID))
End Function
Public Function ID3V2AddTableOfContent(ByVal ID As String, ByVal Title As String, ByVal Description As String, ByVal isOrdered As Integer) As Integer
  ID3V2AddTableOfContent = ID3V2AddTableOfContentW(StrPtr(ID), StrPtr(Title), StrPtr(Description), isOrdered)
End Function
Public Function ID3V2DeleteAddendum(ByVal ID As String) As Integer
  ID3V2DeleteAddendum = ID3V2DeleteAddendumW(StrPtr(ID))
End Function
Public Function ID3V2DeleteChildElement(ByVal TocID As String, ByVal ChildID As String) As Integer
  ID3V2DeleteChildElement = ID3V2DeleteChildElementW(StrPtr(TocID), StrPtr(ChildID))
End Function
Public Function ID3V2DeleteSubFrame(ByVal ID As String, ByVal Index As Integer) As Integer
  ID3V2DeleteSubFrame = ID3V2DeleteSubFrameW(StrPtr(ID), Index)
End Function
Public Function ID3V2GetAddendumTitle(ByVal ID As String) As String
  ID3V2GetAddendumTitle = StrConv(ID3V2GetAddendumTitleW(StrPtr(ID)), vbFromUnicode)
End Function
Public Function ID3V2GetAddendumDescription(ByVal ID As String) As String
  ID3V2GetAddendumDescription = StrConv(ID3V2GetAddendumDescriptionW(StrPtr(ID)), vbFromUnicode)
End Function
Public Function ID3V2GetAddendumType(ByVal ID As String) As Integer
  ID3V2GetAddendumType = ID3V2GetAddendumTypeW(StrPtr(ID))
End Function
Public Function ID3V2GetAllFrameIDs() As String
  ID3V2GetAllFrameIDs = StrConv(ID3V2GetAllFrameIDsW(), vbFromUnicode)
End Function
Public Function ID3V2GetChildElements(ByVal ID As String) As Integer
  ID3V2GetChildElements = ID3V2GetChildElementsW(StrPtr(ID))
End Function
Public Function ID3V2GetChildElementID(ByVal ID As String, ByVal Index As Integer) As String
  ID3V2GetChildElementID = StrConv(ID3V2GetChildElementIDW(StrPtr(ID), Index), vbFromUnicode)
End Function
Public Function ID3V2GetSubFrames(ByVal ID As String) As Integer
  ID3V2GetSubFrames = ID3V2GetSubFramesW(StrPtr(ID))
End Function
Public Function ID3V2GetSubFrameID(ByVal ID As String, ByVal Index As Integer) As Long
  ID3V2GetSubFrameID = ID3V2GetSubFrameIDW(StrPtr(ID), Index)
End Function
Public Function ID3V2GetSubFrameImageType(ByVal ID As String, ByVal Index As Integer) As Integer
  ID3V2GetSubFrameImageType = ID3V2GetSubFrameImageTypeW(StrPtr(ID), Index)
End Function
Public Function ID3V2GetSubFrameImage(arr() As Byte, ByVal maxLen As Long, ID As String, Index As Integer) As Long
  ID3V2GetSubFrameImage = ID3V2GetSubFrameImageW(VarPtr(arr(0)), maxLen, StrPtr(ID), Index)
End Function
Public Function ID3V2GetSubFrameText(ByVal ID As String, ByVal Index As Integer) As String
  ID3V2GetSubFrameText = StrConv(ID3V2GetSubFrameTextW(StrPtr(ID), Index), vbFromUnicode)
End Function
Public Function ID3V2GetSubFrameDescription(ByVal ID As String, ByVal Index As Integer) As String
  ID3V2GetSubFrameDescription = StrConv(ID3V2GetSubFrameDescriptionW(StrPtr(ID), Index), vbFromUnicode)
End Function
Public Function ID3V2GetSubFrameType(ByVal ID As String, ByVal Index As Integer) As Integer
  ID3V2GetSubFrameType = ID3V2GetSubFrameTypeW(StrPtr(ID), Index)
End Function
Public Function ID3V2GetTOCRootID() As String
  ID3V2GetTOCRootID = StrConv(ID3V2GetTOCRootIDW(), vbFromUnicode)
End Function
Public Function ID3V2SetAddendumTitle(ByVal ID As String, ByVal newTitle As String) As Integer
  ID3V2SetAddendumTitle = ID3V2SetAddendumTitleW(StrPtr(ID), StrPtr(newTitle))
End Function
Public Function ID3V2SetAddendumDescription(ByVal ID As String, ByVal newDescription As String) As Integer
  ID3V2SetAddendumDescription = ID3V2SetAddendumDescriptionW(StrPtr(ID), StrPtr(newDescription))
End Function
Public Function ID3V2SetImageSubFrame(arr() As Byte, ByVal maxLen As Long, ByVal ParentID As String, ByVal Description As String, ByVal PictureType As Integer) As Integer
  ID3V2SetImageSubFrame = ID3V2SetImageSubFrameW(VarPtr(arr(0)), maxLen, StrPtr(ParentID), StrPtr(Description), PictureType)
End Function
Public Function ID3V2SetTextSubFrame(ByVal ParentID As String, ByVal frameID As ID3V2FRAMES, ByVal text As String, ByVal Description As String) As Integer
  ID3V2SetTextSubFrame = ID3V2SetTextSubFrameW(StrPtr(ParentID), frameID, StrPtr(text), StrPtr(Description))
End Function
Public Function ID3V2SetURLSubFrame(ByVal ParentID As String, ByVal frameID As ID3V2FRAMES, ByVal text As String, ByVal Description As String) As Integer
  ID3V2SetURLSubFrame = ID3V2SetURLSubFrameW(StrPtr(ParentID), frameID, StrPtr(text), StrPtr(Description))
End Function
Public Function ID3V2AddAudioEncryption(arr() As Byte, ByVal maxLen As Long, ByVal URL As String, ByVal PreviewStart As Integer, ByVal PreviewLength As Integer) As Integer
  ID3V2AddAudioEncryption = ID3V2AddAudioEncryptionW(VarPtr(arr(0)), maxLen, StrPtr(URL), PreviewStart, PreviewLength)
End Function

Public Function ID3V2AddSeekOffset(ByVal Counter As Long) As Integer ' SEEK
  ID3V2AddSeekOffset = ID3V2AddSeekOffsetW(Counter)
End Function

Public Function ID3V2AddAudioSeekPoint(arr() As Byte, ByVal maxLen As Long, ByVal start As Long, ByVal Length As Long, ByVal numbers As Integer, ByVal BitsPerPoint As Integer) As Integer
  ID3V2AddAudioSeekPoint = ID3V2AddAudioSeekPointW(VarPtr(arr(0)), maxLen, start, Length, numbers, BitsPerPoint)
End Function

Public Function ID3V2AddComment(ByVal Language As String, ByVal Description As String, ByVal text As String) As Integer
  ID3V2AddComment = ID3V2AddCommentW(StrPtr(Language), StrPtr(Description), StrPtr(text))
End Function

Public Function ID3V2AddCommercialFrame(arr() As Byte, ByVal maxLen As Long, ByVal Price As String, ByVal validUntil As String, ByVal contactUrl As String, ByVal receivedAs As Integer, ByVal Seller As String, ByVal Description As String) As Integer
  ID3V2AddCommercialFrame = ID3V2AddCommercialFrameW(VarPtr(arr(0)), maxLen, StrPtr(Price), StrPtr(validUntil), StrPtr(contactUrl), receivedAs, StrPtr(Seller), StrPtr(Description))
End Function

Public Function ID3V2AddEncryption(arr() As Byte, ByVal maxLen As Long, ByVal URL As String, ByVal symbol As Integer) As Integer
  ID3V2AddEncryption = ID3V2AddEncryptionW(VarPtr(arr(0)), maxLen, StrPtr(URL), symbol)
End Function

Public Function ID3V2AddEqualisation(arr() As Byte, ByVal maxLen As Long, ByVal Interpolationmethod As Integer, ByVal Identification As String) As Integer
  ID3V2AddEqualisation = ID3V2AddEqualisationW(VarPtr(arr(0)), maxLen, Interpolationmethod, StrPtr(Identification))
End Function

Public Function ID3V2AddGeneralObject(arr() As Byte, ByVal maxLen As Long, ByVal Mime As String, ByVal FileName As String, ByVal Description As String) As Integer
  ID3V2AddGeneralObject = ID3V2AddGeneralObjectW(VarPtr(arr(0)), maxLen, StrPtr(Mime), StrPtr(FileName), StrPtr(Description))
End Function

Public Function ID3V2AddGroupIdentification(arr() As Byte, ByVal Length As Long, ByVal URL As String, ByVal symbol As Integer) As Integer
  ID3V2AddGroupIdentification = ID3V2AddGroupIdentificationW(VarPtr(arr(0)), Length, StrPtr(URL), symbol)
End Function

Public Function ID3V2AddLinkedInformation(ByVal FrameIdentifier As Long, ByVal URL As String, ByVal additionalData As String) As Integer
  ID3V2AddLinkedInformation = ID3V2AddLinkedInformationW(FrameIdentifier, StrPtr(URL), StrPtr(additionalData))
End Function

Public Function ID3V2AddLyric(ByVal Language As String, ByVal Description As String, ByVal text As String) As Integer
  ID3V2AddLyric = ID3V2AddLyricW(StrPtr(Language), StrPtr(Description), StrPtr(text))
End Function

Public Function ID3V2AddMpegLocationLookupTable(arr() As Byte, ByVal Length As Long, ByVal Frames As Long, ByVal Bytes As Long, ByVal Milliseconds As Long, ByVal BytesDeviation As Integer, ByVal MillisecondsDeviation As Integer) As Integer
  ID3V2AddMpegLocationLookupTable = ID3V2AddMpegLocationLookupTableW(VarPtr(arr(0)), Length, Frames, Bytes, Milliseconds, BytesDeviation, MillisecondsDeviation)
End Function

Public Function ID3V2AddMusicCdIdentifier(arr() As Byte, ByVal Length As Long) As Integer
  ID3V2AddMusicCdIdentifier = ID3V2AddMusicCdIdentifierW(VarPtr(arr(0)), Length)
End Function

Public Function ID3V2AddOwnership(ByVal Price As String, ByVal dateString As String, ByVal Seller As String) As Integer
  ID3V2AddOwnership = ID3V2AddOwnershipW(StrPtr(Price), StrPtr(dateString), StrPtr(Seller))
End Function

Public Function ID3V2AddPictureArray(arr() As Byte, ByVal maxLen As Long, ByVal Description As String, ByVal PictureType As Integer) As Integer
  ID3V2AddPictureArray = ID3V2AddPictureArrayW(VarPtr(arr(0)), maxLen, StrPtr(Description), PictureType)
End Function

Public Function ID3V2AddPictureFile(ByVal FileName As String, ByVal Description As String, ByVal PictureType As Integer, ByVal asLink As Integer) As Integer
  ID3V2AddPictureFile = ID3V2AddPictureFileW(StrPtr(FileName), StrPtr(Description), PictureType, asLink)
End Function

Public Function ID3V2AddPopularimeter(ByVal Email As String, ByVal Rating As Integer, ByVal Counter As Long) As Integer
  ID3V2AddPopularimeter = ID3V2AddPopularimeterW(StrPtr(Email), Rating, Counter)
End Function

Public Function ID3V2AddPositionSynchronisation(ByVal TimestampFormat As Integer, ByVal Position As Long) As Integer
  ID3V2AddPositionSynchronisation = ID3V2AddPositionSynchronisationW(TimestampFormat, Position)
End Function

Public Function ID3V2AddPrivateFrame(arr() As Byte, ByVal Length As Long, ByVal URL As String) As Integer
  ID3V2AddPrivateFrame = ID3V2AddPrivateFrameW(VarPtr(arr(0)), Length, StrPtr(URL))
End Function

Public Function ID3V2AddRecommendedBufferSize(ByVal BufferSize As Long, ByVal EmbeddedInfoFlag As Integer, ByVal offset As Long) As Integer
  ID3V2AddRecommendedBufferSize = ID3V2AddRecommendedBufferSizeW(BufferSize, EmbeddedInfoFlag, offset)
End Function

Public Function ID3V2AddRelativeVolumeAdjustment(arr() As Byte, ByVal Length As Long, ByVal Identifier As String) As Integer
  ID3V2AddRelativeVolumeAdjustment = ID3V2AddRelativeVolumeAdjustmentW(VarPtr(arr(0)), Length, StrPtr(Identifier))
End Function

Public Function ID3V2AddReverb(ByVal reverbLeft As Integer, ByVal reverbRight As Integer, ByVal bouncesLeft As Integer, ByVal bouncesRight As Integer, ByVal feedbackLeftToLeft As Integer, ByVal feedbackLeftToRight As Integer, ByVal feedbackRightToRight As Integer, ByVal feedbackRightToLeft As Integer, ByVal premixLeftToRight As Integer, ByVal premixRightToLeft As Integer) As Integer
  ID3V2AddReverb = ID3V2AddReverbW(reverbLeft, reverbRight, bouncesLeft, bouncesRight, feedbackLeftToLeft, feedbackLeftToRight, feedbackRightToRight, feedbackRightToLeft, premixLeftToRight, premixRightToLeft)
End Function

Public Function ID3V2AddSignatureFrame(arr() As Byte, ByVal Length As Long, ByVal GroupSymbol As Integer) As Integer
  ID3V2AddSignatureFrame = ID3V2AddSignatureFrameW(VarPtr(arr(0)), Length, GroupSymbol)
End Function

Public Function ID3V2AddSyncLyric(ByVal Language As String, ByVal Description As String, ByVal text As String, ByVal ContentType As Integer, ByVal TimestampFormat As Integer) As Integer
  ID3V2AddSyncLyric = ID3V2AddSyncLyricW(StrPtr(Language), StrPtr(Description), StrPtr(text), ContentType, TimestampFormat)
End Function

Public Function ID3V2AddSynchronizedTempo(arr() As Byte, ByVal Length As Long, ByVal format As Integer) As Integer
  ID3V2AddSynchronizedTempo = ID3V2AddSynchronizedTempoW(VarPtr(arr(0)), Length, format)
End Function

Public Function ID3V2AddUniqueFileIdentifier(arr() As Byte, ByVal Length As Long, ByVal Owner As String) As Integer
  ID3V2AddUniqueFileIdentifier = ID3V2AddUniqueFileIdentifierW(VarPtr(arr(0)), Length, StrPtr(Owner))
End Function

Public Function ID3V2AddUserFrame(ByVal Language As String, ByVal text As String) As Integer
  ID3V2AddUserFrame = ID3V2AddUserFrameW(StrPtr(Language), StrPtr(text))
End Function

Public Function ID3V2AddUserText(ByVal Description As String, ByVal text As String) As Integer
  ID3V2AddUserText = ID3V2AddUserTextW(StrPtr(Description), StrPtr(text))
End Function

Public Function ID3V2AddUserURL(ByVal Description As String, ByVal URL As String) As Integer
  ID3V2AddUserURL = ID3V2AddUserURLW(StrPtr(Description), StrPtr(URL))
End Function

Public Function ID3V2DeleteAllFrames(ByVal ID As Long) As Integer
  ID3V2DeleteAllFrames = ID3V2DeleteAllFramesW(ID)
End Function

Public Sub ID3V2DeleteComments()
  Call ID3V2DeleteAllFramesW(ID3F_COMM)
End Sub

Public Sub ID3V2DeletePopularimeters()
  Call ID3V2DeleteAllFramesW(ID3F_POPM)
End Sub

Public Sub ID3V2DeleteLyrics()
  Call ID3V2DeleteAllFramesW(ID3F_USLT)
End Sub

Public Function ID3V2DeletePicture(Index As Integer) As Integer
  ID3V2DeletePicture = ID3V2DeleteSelectedFrameW(ID3F_APIC, Index)
End Function

Public Sub ID3V2DeletePictures() ' APIC
  Call ID3V2DeleteAllFramesW(ID3F_APIC)
End Sub

Public Function ID3V2DeleteSelectedFrame(ByVal ID As Long, ByVal Index As Integer) As Integer
  ID3V2DeleteSelectedFrame = ID3V2DeleteSelectedFrameW(ID, Index)
End Function

Public Sub ID3V2DeleteSyncLyrics() ' SYLT
  Call ID3V2DeleteAllFramesW(ID3F_SYLT)
End Sub

Public Sub ID3V2DeleteUserFrames() ' USER
  Call ID3V2DeleteAllFramesW(ID3F_USER)
End Sub

Public Sub ID3V2DeleteUserTexts()
  Call ID3V2DeleteAllFramesW(ID3F_TXXX)
End Sub

Public Sub ID3V2DeleteUserURLs()
  Call ID3V2DeleteAllFramesW(ID3F_WXXX)
End Sub

Public Function ID3V2Exists() As Integer
  ID3V2Exists = ID3V2ExistsW()
End Function

Public Function ID3V2GetAudioEncryptionData(arr() As Byte, ByVal maxLen As Long, ByVal Index As Integer) As Long
  ID3V2GetAudioEncryptionData = ID3V2GetAudioEncryptionDataW(VarPtr(arr(0)), maxLen, Index)
End Function

Public Function ID3V2GetAudioEncryptionPreviewLength(ByVal Index As Integer) As Integer
  ID3V2GetAudioEncryptionPreviewLength = ID3V2GetAudioEncryptionPreviewLengthW(Index)
End Function

Public Function ID3V2GetAudioEncryptionPreviewStart(ByVal Index As Integer) As Integer
  ID3V2GetAudioEncryptionPreviewStart = ID3V2GetAudioEncryptionPreviewStartW(Index)
End Function

Public Function ID3V2GetAudioEncryptionURL(ByVal Index As Integer) As String
  ID3V2GetAudioEncryptionURL = StrConv(ID3V2GetAudioEncryptionURLW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetAudioSeekPointBitsPerIndexpoint() As Long
  ID3V2GetAudioSeekPointBitsPerIndexpoint = ID3V2GetAudioSeekPointBitsPerIndexpointW()
End Function

Public Function ID3V2GetAudioSeekPointData(arr() As Byte, ByVal maxLen As Long) As Long
  ID3V2GetAudioSeekPointData = ID3V2GetAudioSeekPointDataW(VarPtr(arr(0)), maxLen)
End Function

Public Function ID3V2GetAudioSeekPointLength() As Long
  ID3V2GetAudioSeekPointLength = ID3V2GetAudioSeekPointLengthW()
End Function

Public Function ID3V2GetAudioSeekPointNumber() As Long
  ID3V2GetAudioSeekPointNumber = ID3V2GetAudioSeekPointNumberW()
End Function

Public Function ID3V2GetAudioSeekPointStart() As Long
  ID3V2GetAudioSeekPointStart = ID3V2GetAudioSeekPointStartW()
End Function

Public Function ID3V2GetChapterStartTime(ByVal Index As String) As Long
  ID3V2GetChapterStartTime = ID3V2GetChapterStartTimeW(StrPtr(Index))
End Function

Public Function ID3V2GetChapterEndTime(ByVal Index As String) As Long
  ID3V2GetChapterEndTime = ID3V2GetChapterEndTimeW(StrPtr(Index))
End Function

Public Function ID3V2GetComments() As Integer
  ID3V2GetComments = ID3V2GetFrameCountW(ID3F_COMM)
End Function

Public Function ID3V2GetCommentDescription(ByVal Index As Integer) As String
  ID3V2GetCommentDescription = StrConv(ID3V2GetCommentDescriptionW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetCommentLanguage(ByVal Index As Integer) As String
  ID3V2GetCommentLanguage = StrConv(ID3V2GetCommentLanguageW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetComment(ByVal Index As Integer) As String
  ID3V2GetComment = StrConv(ID3V2GetCommentW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetCommercialFrameContactURL(ByVal Index As Integer) As String
  ID3V2GetCommercialFrameContactURL = StrConv(ID3V2GetCommercialFrameContactURLW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetCommercialFrameDescription(ByVal Index As Integer) As String
  ID3V2GetCommercialFrameDescription = StrConv(ID3V2GetCommercialFrameDescriptionW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetCommercialFramePictureMime(ByVal Index As Integer) As String
  ID3V2GetCommercialFramePictureMime = StrConv(ID3V2GetCommercialFramePictureMimeW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetCommercialFramePicture(arr() As Byte, ByVal maxLen As Long, ByVal Index As Integer) As Long
  ID3V2GetCommercialFramePicture = ID3V2GetCommercialFramePictureW(VarPtr(arr(0)), maxLen, Index)
End Function

Public Function ID3V2GetCommercialFramePrice(ByVal Index As Integer) As String
  ID3V2GetCommercialFramePrice = StrConv(ID3V2GetCommercialFramePriceW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetCommercialFrameReceivedAs(ByVal Index As Integer) As Integer
  ID3V2GetCommercialFrameReceivedAs = ID3V2GetCommercialFrameReceivedAsW(Index)
End Function

Public Function ID3V2GetCommercialFrameSellerName(ByVal Index As Integer) As String
  ID3V2GetCommercialFrameSellerName = StrConv(ID3V2GetCommercialFrameSellerNameW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetCommercialFrameValidUntil(ByVal Index As Integer) As String
  ID3V2GetCommercialFrameValidUntil = StrConv(ID3V2GetCommercialFrameValidUntilW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetEncoding(ByVal ID As Long) As Integer
  ID3V2GetEncoding = ID3V2GetEncodingW(ID)
End Function

Public Function ID3V2GetEncryptionData(arr() As Byte, ByVal maxLen As Long, ByVal Index As Integer) As Long
  ID3V2GetEncryptionData = ID3V2GetEncryptionDataW(VarPtr(arr(0)), maxLen, Index)
End Function

Public Function ID3V2GetEncryptionSymbol(ByVal Index As Integer) As Integer
  ID3V2GetEncryptionSymbol = ID3V2GetEncryptionSymbolW(Index)
End Function

Public Function ID3V2GetEncryptionURL(ByVal Index As Integer) As String
  ID3V2GetEncryptionURL = StrConv(ID3V2GetEncryptionURLW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetEqualisationAdjustmentBits(ByVal Index As Integer) As Integer
  ID3V2GetEqualisationAdjustmentBits = ID3V2GetEqualisationAdjustmentBitsW(Index)
End Function

Public Function ID3V2GetEqualisationData(arr() As Byte, ByVal maxLen As Long, ByVal Index As Integer) As Long
  ID3V2GetEqualisationData = ID3V2GetEqualisationDataW(VarPtr(arr(0)), maxLen, Index)
End Function

Public Function ID3V2GetEqualisationIdentification(ByVal Index As Integer) As String
  ID3V2GetEqualisationIdentification = StrConv(ID3V2GetEqualisationIdentificationW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetEqualisationInterpolation(ByVal Index As Integer) As Integer
  ID3V2GetEqualisationInterpolation = ID3V2GetEqualisationInterpolationW(Index)
End Function

Public Function ID3V2GetEventTimingCodesData(arr() As Byte, ByVal maxLen As Long) As Long
  ID3V2GetEventTimingCodesData = ID3V2GetEventTimingCodesDataW(VarPtr(arr(0)), maxLen)
End Function

Public Function ID3V2GetEventTimingCodesTimestampFormat() As Integer
  ID3V2GetEventTimingCodesTimestampFormat = ID3V2GetEventTimingCodesTimestampFormatW()
End Function

Public Function ID3V2GetFrameCount(ByVal ID As Long) As Integer
  ID3V2GetFrameCount = ID3V2GetFrameCountW(ID)
End Function

Public Function ID3V2GetGeneralObjectData(arr() As Byte, ByVal maxLen As Long, ByVal Index As Integer) As Long
  ID3V2GetGeneralObjectData = ID3V2GetGeneralObjectDataW(VarPtr(arr(0)), maxLen, Index)
End Function

Public Function ID3V2GetGeneralObjectDescription(ByVal Index As Integer) As String
  ID3V2GetGeneralObjectDescription = StrConv(ID3V2GetGeneralObjectDescriptionW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetGeneralObjectFilename(ByVal Index As Integer) As String
  ID3V2GetGeneralObjectFilename = StrConv(ID3V2GetGeneralObjectFilenameW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetGeneralObjectMime(ByVal Index As Integer) As String
  ID3V2GetGeneralObjectMime = StrConv(ID3V2GetGeneralObjectMimeW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetGroupIdentificationData(arr() As Byte, ByVal maxLen As Long, ByVal Index As Integer) As Long
  ID3V2GetGroupIdentificationData = ID3V2GetGroupIdentificationDataW(VarPtr(arr(0)), maxLen, Index)
End Function

Public Function ID3V2GetGroupIdentificationSymbol(ByVal Index As Integer) As Integer
  ID3V2GetGroupIdentificationSymbol = ID3V2GetGroupIdentificationSymbolW(Index)
End Function

Public Function ID3V2GetGroupIdentificationURL(ByVal Index As Integer) As String
  ID3V2GetGroupIdentificationURL = StrConv(ID3V2GetGroupIdentificationURLW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetLinkedInformationAdditionalData(ByVal Index As Integer) As String
  ID3V2GetLinkedInformationAdditionalData = StrConv(ID3V2GetLinkedInformationAdditionalDataW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetLinkedInformationFrameIdentifier(ByVal Index As Integer) As Long
  ID3V2GetLinkedInformationFrameIdentifier = ID3V2GetLinkedInformationFrameIdentifierW(Index)
End Function

Public Function ID3V2GetLinkedInformationURL(ByVal Index As Integer) As String
  ID3V2GetLinkedInformationURL = StrConv(ID3V2GetLinkedInformationURLW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetLyrics() As Integer
  ID3V2GetLyrics = ID3V2GetFrameCountW(ID3F_USLT)
End Function

Public Function ID3V2GetLyricDescription(ByVal Index As Integer) As String
  ID3V2GetLyricDescription = StrConv(ID3V2GetLyricDescriptionW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetLyricLanguage(ByVal Index As Integer) As String
  ID3V2GetLyricLanguage = StrConv(ID3V2GetLyricLanguageW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetLyric(ByVal Index As Integer) As String
  ID3V2GetLyric = StrConv(ID3V2GetLyricW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetMpegLocationLookupTableBytesDeviation() As Long
  ID3V2GetMpegLocationLookupTableBytesDeviation = ID3V2GetMpegLocationLookupTableBytesDeviationW()
End Function

Public Function ID3V2GetMpegLocationLookupTableBytes() As Long
  ID3V2GetMpegLocationLookupTableBytes = ID3V2GetMpegLocationLookupTableBytesW()
End Function

Public Function ID3V2GetMpegLocationLookupTableData(arr() As Byte, ByVal maxLen As Long) As Long
  ID3V2GetMpegLocationLookupTableData = ID3V2GetMpegLocationLookupTableDataW(VarPtr(arr(0)), maxLen)
End Function

Public Function ID3V2GetMpegLocationLookupTableFrames() As Long
  ID3V2GetMpegLocationLookupTableFrames = ID3V2GetMpegLocationLookupTableFramesW()
End Function

Public Function ID3V2GetMpegLocationLookupTableMillisecondsDeviation() As Long
  ID3V2GetMpegLocationLookupTableMillisecondsDeviation = ID3V2GetMpegLocationLookupTableMillisecondsDeviationW()
End Function

Public Function ID3V2GetMpegLocationLookupTableMilliseconds() As Long
  ID3V2GetMpegLocationLookupTableMilliseconds = ID3V2GetMpegLocationLookupTableMillisecondsW()
End Function

Public Function ID3V2GetMusicCdIdentifierData(arr() As Byte, ByVal maxLen As Long) As Long
  ID3V2GetMusicCdIdentifierData = ID3V2GetMusicCdIdentifierDataW(VarPtr(arr(0)), maxLen)
End Function

Public Function ID3V2GetOwnershipDate() As String
  ID3V2GetOwnershipDate = StrConv(ID3V2GetOwnershipDateW(), vbFromUnicode)
End Function

Public Function ID3V2GetOwnershipPrice() As String
  ID3V2GetOwnershipPrice = StrConv(ID3V2GetOwnershipPriceW(), vbFromUnicode)
End Function

Public Function ID3V2GetOwnershipSeller() As String
  ID3V2GetOwnershipSeller = StrConv(ID3V2GetOwnershipSellerW(), vbFromUnicode)
End Function

Public Function ID3V2GetPictureCount() As Integer
  ID3V2GetPictureCount = ID3V2GetFrameCountW(ID3F_APIC)
End Function

Public Function ID3V2GetPictureArray(ByVal Index As Integer) As Byte()
  Dim maxLen As Long, ln As Long
  maxLen = 5000000
  Dim dummy() As Byte
  ReDim dummy(maxLen)
  ln = ID3V2GetPictureArrayW(VarPtr(dummy(0)), maxLen, Index)
  If ln = 0 Then
    Debug.Print "Fehler beim Laden des Bildes:" & AUDIOGetLastErrorText()
  End If
  ReDim Preserve dummy(ln)
  ID3V2GetPictureArray = dummy
End Function

Public Function ID3V2GetPictureDescription(ByVal Index As Integer) As String
  ID3V2GetPictureDescription = StrConv(ID3V2GetPictureDescriptionW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetPictureFile(ByVal FileName As String, ByVal Index As Integer) As Integer
  ID3V2GetPictureFile = ID3V2GetPictureFileW(StrPtr(FileName), Index)
End Function

Public Function ID3V2GetPictureMime(ByVal Index As Integer) As String
  ID3V2GetPictureMime = StrConv(ID3V2GetPictureMimeW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetPictureType(ByVal Index As Integer) As Integer
  ID3V2GetPictureType = ID3V2GetPictureTypeW(Index)
End Function

Public Function ID3V2GetPictureTypeText(ByVal Index As Integer) As String
  ID3V2GetPictureTypeText = StrConv(ID3V2GetPictureTypeTextW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetPopularimeterCounter(ByVal Index As Integer) As Long
  ID3V2GetPopularimeterCounter = ID3V2GetPopularimeterCounterW(Index)
End Function

Public Function ID3V2GetPopularimeterEmail(ByVal Index As Integer) As String
  ID3V2GetPopularimeterEmail = StrConv(ID3V2GetPopularimeterEmailW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetPopularimeterRating(ByVal Index As Integer) As Integer
  ID3V2GetPopularimeterRating = ID3V2GetPopularimeterRatingW(Index)
End Function

Public Function ID3V2GetPositionSynchronisationTimestampFormat() As Integer
  ID3V2GetPositionSynchronisationTimestampFormat = ID3V2GetPositionSynchronisationTimestampFormatW()
End Function

Public Function ID3V2GetPositionSynchronisationValue() As Long
  ID3V2GetPositionSynchronisationValue = ID3V2GetPositionSynchronisationValueW()
End Function

Public Function ID3V2GetPossibleCHAPID() As String
  ID3V2GetPossibleCHAPID = StrConv(ID3V2GetPossibleCHAPIDW(), vbFromUnicode)
End Function

Public Function ID3V2GetPossibleTOCID() As String
  ID3V2GetPossibleTOCID = StrConv(ID3V2GetPossibleTOCIDW(), vbFromUnicode)
End Function

Public Function ID3V2GetPrivateFrameData(arr() As Byte, ByVal maxLen As Long, ByVal Index As Integer) As Long
  ID3V2GetPrivateFrameData = ID3V2GetPrivateFrameDataW(VarPtr(arr(0)), maxLen, Index)
End Function

Public Function ID3V2GetPrivateFrameURL(ByVal Index As Integer) As String
  ID3V2GetPrivateFrameURL = StrConv(ID3V2GetPrivateFrameURLW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetRelativeVolumeAdjustmentData(arr() As Byte, ByVal maxLen As Long, ByVal Index As Integer) As Long
  ID3V2GetRelativeVolumeAdjustmentData = ID3V2GetRelativeVolumeAdjustmentDataW(VarPtr(arr(0)), maxLen, Index)
End Function

Public Function ID3V2GetRelativeVolumeAdjustmentIdentifier(ByVal Index As Integer) As String
  ID3V2GetRelativeVolumeAdjustmentIdentifier = StrConv(ID3V2GetRelativeVolumeAdjustmentIdentifierW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetReverbBouncesLeft() As Integer
  ID3V2GetReverbBouncesLeft = ID3V2GetReverbBouncesLeftW()
End Function

Public Function ID3V2GetReverbBouncesRight() As Integer
  ID3V2GetReverbBouncesRight = ID3V2GetReverbBouncesRightW()
End Function

Public Function ID3V2GetReverbFeedbackLeftToLeft() As Integer
  ID3V2GetReverbFeedbackLeftToLeft = ID3V2GetReverbFeedbackLeftToLeftW()
End Function

Public Function ID3V2GetReverbFeedbackLeftToRight() As Integer
  ID3V2GetReverbFeedbackLeftToRight = ID3V2GetReverbFeedbackLeftToRightW()
End Function

Public Function ID3V2GetReverbFeedbackRightToLeft() As Integer
  ID3V2GetReverbFeedbackRightToLeft = ID3V2GetReverbFeedbackRightToLeftW()
End Function

Public Function ID3V2GetReverbFeedbackRightToRight() As Integer
  ID3V2GetReverbFeedbackRightToRight = ID3V2GetReverbFeedbackRightToRightW()
End Function

Public Function ID3V2GetReverbLeft() As Integer
  ID3V2GetReverbLeft = ID3V2GetReverbLeftW()
End Function

Public Function ID3V2GetReverbPremixLeftToRight() As Integer
  ID3V2GetReverbPremixLeftToRight = ID3V2GetReverbPremixLeftToRightW()
End Function

Public Function ID3V2GetReverbPremixRightToLeft() As Integer
  ID3V2GetReverbPremixRightToLeft = ID3V2GetReverbPremixRightToLeftW()
End Function

Public Function ID3V2GetReverbRight() As Integer
  ID3V2GetReverbRight = ID3V2GetReverbRightW()
End Function

Public Function ID3V2GetSeekOffset() As Long ' SEEK
  ID3V2GetSeekOffset = ID3V2GetSeekOffsetW()
End Function

Public Function ID3V2GetSignatureFrameData(arr() As Byte, ByVal maxLen As Long, ByVal Index As Integer) As Long
  ID3V2GetSignatureFrameData = ID3V2GetSignatureFrameDataW(VarPtr(arr(0)), maxLen, Index)
End Function

Public Function ID3V2GetSignatureFrameGroupSymbol(ByVal Index As Integer) As Integer
  ID3V2GetSignatureFrameGroupSymbol = ID3V2GetSignatureFrameGroupSymbolW(Index)
End Function

Public Function ID3V2GetSize() As Long
  ID3V2GetSize = ID3V2GetSizeW()
End Function

Public Function ID3V2GetSyncLyrics() As Integer
  ID3V2GetSyncLyrics = ID3V2GetFrameCountW(ID3F_SYLT)
End Function

Public Function ID3V2GetSyncLyricContentType(ByVal Index As Integer) As Integer
  ID3V2GetSyncLyricContentType = ID3V2GetSyncLyricContentTypeW(Index)
End Function

Public Function ID3V2GetSyncLyricDescription(ByVal Index As Integer) As String
  ID3V2GetSyncLyricDescription = StrConv(ID3V2GetSyncLyricDescriptionW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetSyncLyricLanguage(ByVal Index As Integer) As String
  ID3V2GetSyncLyricLanguage = StrConv(ID3V2GetSyncLyricLanguageW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetSyncLyricTimeFormat(ByVal Index As Integer) As Integer
  ID3V2GetSyncLyricTimeFormat = ID3V2GetSyncLyricTimeFormatW(Index)
End Function

Public Function ID3V2GetSyncLyric(ByVal Index As Integer) As String
  ID3V2GetSyncLyric = StrConv(ID3V2GetSyncLyricW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetSynchronizedTempoData(arr() As Byte, ByVal maxLen As Long) As Long
  ID3V2GetSynchronizedTempoData = ID3V2GetSynchronizedTempoDataW(VarPtr(arr(0)), maxLen)
End Function

Public Function ID3V2GetSynchronizedTempoFormat() As Integer
  ID3V2GetSynchronizedTempoFormat = ID3V2GetSynchronizedTempoFormatW()
End Function
Public Function ID3V2GetTOCIsOrdered(ByVal ID As String) As Boolean
  ID3V2GetTOCIsOrdered = ID3V2GetTOCIsOrderedW(StrPtr(ID))
End Function
Public Function ID3V2GetUniqueFileIdentifierData(arr() As Byte, ByVal maxLen As Long, ByVal Index As Integer) As Long
  ID3V2GetUniqueFileIdentifierData = ID3V2GetUniqueFileIdentifierDataW(VarPtr(arr(0)), maxLen, Index)
End Function

Public Function ID3V2GetUniqueFileIdentifierOwner(ByVal Index As Integer) As String
  ID3V2GetUniqueFileIdentifierOwner = StrConv(ID3V2GetUniqueFileIdentifierOwnerW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetUserFrames() As Integer
  ID3V2GetUserFrames = ID3V2GetFrameCountW(ID3F_USER)
End Function

Public Function ID3V2GetUserFrameLanguage(ByVal Index As Integer) As String
  ID3V2GetUserFrameLanguage = StrConv(ID3V2GetUserFrameLanguageW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetUserFrame(ByVal Index As Integer) As String
  ID3V2GetUserFrame = StrConv(ID3V2GetUserFrameW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetUserTexts() As Integer
  ID3V2GetUserTexts = ID3V2GetFrameCountW(ID3F_TXXX)
End Function

Public Function ID3V2GetUserTextDescription(ByVal Index As Integer) As String
  ID3V2GetUserTextDescription = StrConv(ID3V2GetUserTextDescriptionW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetUserText(ByVal Index As Integer) As String
  ID3V2GetUserText = StrConv(ID3V2GetUserTextW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetUserURLDescription(ByVal Index As Integer) As String
  ID3V2GetUserURLDescription = StrConv(ID3V2GetUserURLDescriptionW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetUserURLs() As Integer
  ID3V2GetUserURLs = ID3V2GetFrameCountW(ID3F_WXXX)
End Function

Public Function ID3V2GetUserURL(ByVal Index As Integer) As String
  ID3V2GetUserURL = StrConv(ID3V2GetUserURLW(Index), vbFromUnicode)
End Function

Public Function ID3V2GetVersion() As String
  ID3V2GetVersion = StrConv(ID3V2GetVersionW(), vbFromUnicode)
End Function

Public Function ID3V2GetRecommendedBufferSizeFlag() As Integer
  ID3V2GetRecommendedBufferSizeFlag = ID3V2GetRecommendedBufferSizeFlagW()
End Function

Public Function ID3V2GetRecommendedBufferSizeOffset() As Long
  ID3V2GetRecommendedBufferSizeOffset = ID3V2GetRecommendedBufferSizeOffsetW()
End Function

Public Function ID3V2GetRecommendedBufferSizeValue() As Long
  ID3V2GetRecommendedBufferSizeValue = ID3V2GetRecommendedBufferSizeValueW()
End Function

Public Function ID3V2ImportCueFile(ByVal FileName As String) As Integer
  ID3V2ImportCueFile = ID3V2ImportCueFileW(StrPtr(FileName))
End Function

Public Function ID3V2RemoveTagFromFile(ByVal FileName As String) As Integer
  ID3V2RemoveTagFromFile = ID3V2RemoveTagFromFileW(StrPtr(FileName))
End Function

Public Function ID3V2RemoveTag() As Integer
  ID3V2RemoveTag = ID3V2RemoveTagW()
End Function

Public Function ID3V2AddEventTimingCodes(arr() As Byte, ByVal maxLen As Long, ByVal TimestampFormat As Integer) As Integer
  ID3V2AddEventTimingCodes = ID3V2AddEventTimingCodesW(VarPtr(arr(0)), maxLen, TimestampFormat)
End Function

Public Function ID3V2SaveChangesToFile(ByVal FileName As String) As Integer
  ID3V2SaveChangesToFile = ID3V2SaveChangesToFileW(StrPtr(FileName))
End Function

Public Function ID3V2SaveChanges() As Integer
  ID3V2SaveChanges = ID3V2SaveChangesW()
End Function

Public Function ID3V2SetChapterTimes(ByVal ID As String, ByVal startTime As Long, endTime As Long) As Integer
  ID3V2SetChapterTimes = ID3V2SetChapterTimesW(StrPtr(ID), startTime, endTime)
End Function

Public Function ID3V2SetFormatAndEncoding(ByVal format As Integer, ByVal encoding As Integer) As Integer
  ID3V2SetFormatAndEncoding = ID3V2SetFormatAndEncodingW(format, encoding)
End Function

Public Function ID3V2SetTOCIsOrdered(ByVal ID As String, ByVal status As Integer) As Integer
  ID3V2SetTOCIsOrdered = ID3V2SetTOCIsOrderedW(StrPtr(ID), status)
End Function

Public Function LYRICSExists() As Integer
  LYRICSExists = LYRICSExistsW()
End Function

Public Function LYRICSGetSize() As Long
  LYRICSGetSize = LYRICSGetSizeW()
End Function

Public Function LYRICSGetStartPosition() As Long
  LYRICSGetStartPosition = LYRICSGetStartPositionW()
End Function

Public Function LYRICSGetVersion() As String
  LYRICSGetVersion = StrConv(LYRICSGetVersionW(), vbFromUnicode)
End Function

Public Function LYRICSRemoveTagFromFile(ByVal FileName As String) As Integer
  LYRICSRemoveTagFromFile = LYRICSRemoveTagFromFileW(StrPtr(FileName))
End Function

Public Function LYRICSRemoveTag() As Integer
  LYRICSRemoveTag = LYRICSRemoveTagW()
End Function

Public Function LYRICSSaveChangesToFile(ByVal FileName As String) As Integer
  LYRICSSaveChangesToFile = LYRICSSaveChangesToFileW(StrPtr(FileName))
End Function

Public Function LYRICSSaveChanges() As Integer
  LYRICSSaveChanges = LYRICSSaveChangesW()
End Function

Public Function MONKEYGetBits() As Integer
  MONKEYGetBits = MONKEYGetBitsW()
End Function

Public Function MONKEYGetCompressionRatio() As Single
  MONKEYGetCompressionRatio = MONKEYGetCompressionRatioW()
End Function

Public Function MONKEYGetCompression() As String
  MONKEYGetCompression = StrConv(MONKEYGetCompressionW(), vbFromUnicode)
End Function

Public Function MONKEYGetFrames() As Long
  MONKEYGetFrames = MONKEYGetFramesW()
End Function

Public Function MONKEYGetPeak() As Single
  MONKEYGetPeak = MONKEYGetPeakW()
End Function

Public Function MONKEYGetSamplesPerFrame() As Long
  MONKEYGetSamplesPerFrame = MONKEYGetSamplesPerFrameW()
End Function

Public Function MONKEYGetSamples() As Long
  MONKEYGetSamples = MONKEYGetSamplesW()
End Function

Public Sub MP4DeletePictures()
  Call MP4DeletePicturesW
End Sub

Public Function MP4DeletePicture(ByVal Index As Integer) As Integer
  MP4DeletePicture = MP4DeletePictureW(Index)
End Function

Public Function MP4GetPictureArray(ByVal Index As Integer) As Byte()
  Dim maxLen As Long, ln As Long
  maxLen = 5000000
  Dim dummy() As Byte
  ReDim dummy(maxLen)
  ln = MP4GetPictureArrayW(VarPtr(dummy(0)), maxLen, Index)
  If ln = 0 Then
    Debug.Print "Fehler beim Laden des Bildes:" & AUDIOGetLastErrorText()
  End If
  ReDim Preserve dummy(ln)
  MP4GetPictureArray = dummy
End Function

Public Function MP4GetAllFrameIDs() As String
  MP4GetAllFrameIDs = StrConv(MP4GetAllFrameIDsW(), vbFromUnicode)
End Function

Public Function MP4GetPictureCount() As Long
  MP4GetPictureCount = MP4GetPictureCountW()
End Function

Public Function MP4GetPictureFile(ByVal FileName As String, ByVal Index As Integer) As Integer
  MP4GetPictureFile = MP4GetPictureFileW(StrPtr(FileName), Index)
End Function

Public Function MP4AddPictureArray(arr() As Byte, ByVal maxLen As Long) As Integer
  MP4AddPictureArray = MP4AddPictureArrayW(VarPtr(arr(0)), maxLen)
End Function

Public Function MP4AddPictureFile(ByVal FileName As String) As Integer
  MP4AddPictureFile = MP4AddPictureFileW(StrPtr(FileName))
End Function

Public Sub MP4DeleteEntries()
  MP4DeleteEntriesW
End Sub

Public Function MP4GetPictureMime(ByVal Index As Integer) As String
  MP4GetPictureMime = StrConv(MP4GetPictureMimeW(Index), vbFromUnicode)
End Function

Public Function MP4GetVersion() As String
  MP4GetVersion = StrConv(MP4GetVersionW(), vbFromUnicode)
End Function

Public Function MP4SaveChangesToFile(ByVal FileName As String) As Integer
  MP4SaveChangesToFile = MP4SaveChangesToFileW(StrPtr(FileName))
End Function

Public Function MP4SaveChanges() As Integer
  MP4SaveChanges = MP4SaveChangesW()
End Function

Public Sub MP4SetTextFrame(ByVal frameID As Long, ByVal textString As String)
  Call MP4SetTextFrameW(frameID, StrPtr(textString))
End Sub

Public Function MP4GetTextFrame(ByVal frameID As Long) As String
  MP4GetTextFrame = StrConv(MP4GetTextFrameW(frameID), vbFromUnicode)
End Function

Public Sub MP4SetiTuneFrame(ByVal frameID As String, ByVal textString As String)
  Call MP4SetiTuneFrameW(StrPtr(frameID), StrPtr(textString))
End Sub

Public Function MP4GetiTuneFrame(ByVal frameID As String) As String
  MP4GetiTuneFrame = StrConv(MP4GetiTuneFrameW(StrPtr(frameID)), vbFromUnicode)
End Function

Public Function MPEGGetEmphasis() As String
  MPEGGetEmphasis = StrConv(MPEGGetEmphasisW(), vbFromUnicode)
End Function

Public Function MPEGGetEncoder() As String
  MPEGGetEncoder = StrConv(MPEGGetEncoderW(), vbFromUnicode)
End Function

Public Function MPEGGetFramePosition() As Long
  MPEGGetFramePosition = MPEGGetFramePositionW()
End Function

Public Function MPEGGetFrameSize() As Long
  MPEGGetFrameSize = MPEGGetFrameSizeW()
End Function

Public Function MPEGGetFrames() As Long
  MPEGGetFrames = MPEGGetFramesW()
End Function

Public Function MPEGGetLayer() As String
  MPEGGetLayer = StrConv(MPEGGetLayerW(), vbFromUnicode)
End Function

Public Function MPEGGetVersion() As String
  MPEGGetVersion = StrConv(MPEGGetVersionW(), vbFromUnicode)
End Function

Public Function MPEGIsCopyrighted() As Integer
  MPEGIsCopyrighted = MPEGIsCopyrightedW()
End Function

Public Function MPEGIsOriginal() As Integer
  MPEGIsOriginal = MPEGIsOriginalW()
End Function

Public Function MPEGIsPadding() As Integer
  MPEGIsPadding = MPEGIsPaddingW()
End Function

Public Function MPEGIsPrivate() As Integer
  MPEGIsPrivate = MPEGIsPrivateW()
End Function

Public Function MPEGIsProtected() As Integer
  MPEGIsProtected = MPEGIsProtectedW()
End Function

Public Function MPEGIsVBR() As Integer
  MPEGIsVBR = MPEGIsVBRW()
End Function

Public Function MPEGSetCopyrighted(ByVal FileName As String, ByVal newValue As Integer) As Integer
  MPEGSetCopyrighted = MPEGSetCopyrightedW(StrPtr(FileName), newValue)
End Function

Public Function MPEGSetOriginal(ByVal FileName As String, ByVal newValue As Integer) As Integer
  MPEGSetOriginal = MPEGSetOriginalW(StrPtr(FileName), newValue)
End Function

Public Function MPEGSetPrivate(ByVal FileName As String, ByVal newValue As Integer) As Integer
  MPEGSetPrivate = MPEGSetPrivateW(StrPtr(FileName), newValue)
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

Public Function OGGGetItemKeys() As String
  OGGGetItemKeys = StrConv(OGGGetItemKeysW(), vbFromUnicode)
End Function

Public Function OGGRemoveTagFromFile(ByVal FileName As String) As Integer
  OGGRemoveTagFromFile = OGGRemoveTagFromFileW(StrPtr(FileName))
End Function

Public Function OGGRemoveTag() As Integer
  OGGRemoveTag = OGGRemoveTagW()
End Function

Public Function OGGSaveChangesToFile(ByVal FileName As String) As Integer
  OGGSaveChangesToFile = OGGSaveChangesToFileW(StrPtr(FileName))
End Function

Public Function OGGSaveChanges() As Integer
  OGGSaveChanges = OGGSaveChangesW()
End Function

Public Sub SetLogFile(ByVal FileName As String)
  Call SetLogFileW(StrPtr(FileName))
End Sub

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

Public Function WAVGetFormat() As String
  WAVGetFormat = StrConv(WAVGetFormatW(), vbFromUnicode)
End Function

Public Function WAVGetHeaderSize() As Long
  WAVGetHeaderSize = WAVGetHeaderSizeW()
End Function

Public Function WAVSaveChanges() As Integer
  WAVSaveChanges = WAVSaveChangesW()
End Function

Public Function WAVSaveChangesToFile(ByVal FileName As String) As Integer
  WAVSaveChangesToFile = WAVSaveChangesToFileW(StrPtr(FileName))
End Function

Public Function WAVGetInfoChunkIDs() As String
  WAVGetInfoChunkIDs = StrConv(WAVGetInfoChunkIDsW(), vbFromUnicode)
End Function

Public Function WMADeletePicture(ByVal Index As Integer) As Integer
  WMADeletePicture = WMADeletePictureW(Index)
End Function

Public Sub WMADeletePictures()
  Call WMADeletePicturesW
End Sub

Public Function WMAGetItemKeys() As String
  WMAGetItemKeys = StrConv(WMAGetItemKeysW(), vbFromUnicode)
End Function

Public Function WMAGetPictureArray(ByVal Index As Integer) As Byte()
  Dim maxLen As Long, ln As Long
  maxLen = 5000000
  Dim dummy() As Byte
  ReDim dummy(maxLen)
  ln = WMAGetPictureArrayW(VarPtr(dummy(0)), maxLen, Index)
  If ln = 0 Then
    Debug.Print "Fehler beim Laden des Bildes:" & AUDIOGetLastErrorText()
  End If
  ReDim Preserve dummy(ln)
  WMAGetPictureArray = dummy
End Function

Public Function WMAGetPictureDescription(ByVal Index As Integer) As String
  WMAGetPictureDescription = StrConv(WMAGetPictureDescriptionW(Index), vbFromUnicode)
End Function

Public Function WMAGetPictureMime(ByVal Index As Integer) As String
  WMAGetPictureMime = StrConv(WMAGetPictureMimeW(Index), vbFromUnicode)
End Function

Public Function WMAGetPictureType(ByVal Index As Integer) As Integer
  WMAGetPictureType = WMAGetPictureTypeW(Index)
End Function

Public Function WMAGetPictureFile(ByVal FileName As String, ByVal Index As Integer) As Integer
  WMAGetPictureFile = WMAGetPictureFileW(StrPtr(FileName), Index)
End Function

Public Function WMAGetPictureCount() As Integer
  WMAGetPictureCount = WMAGetPictureCountW()
End Function
' ---- Funktionen der aktuellen DLL, die bisher im Wrapper fehlten
Public Function FLACAddPictureArray(arr() As Byte, ByVal Length As Long, ByVal Description As String, ByVal PictureType As Integer) As Integer
  FLACAddPictureArray = FLACAddPictureArrayW(VarPtr(arr(0)), Length, StrPtr(Description), PictureType)
End Function
Public Function FLACAddPictureFile(ByVal FileName As String, ByVal Description As String, ByVal PictureType As Integer, ByVal asLink As Integer) As Integer
  FLACAddPictureFile = FLACAddPictureFileW(StrPtr(FileName), StrPtr(Description), PictureType, asLink)
End Function
Public Function FLACDeletePicture(ByVal Index As Integer) As Integer
  FLACDeletePicture = FLACDeletePictureW(Index)
End Function
Public Sub FLACDeletePictures()
  Call FLACDeletePicturesW
End Sub
Public Function FLACGetPictureArray(ByVal Index As Integer) As Byte()
  Dim maxLen As Long, ln As Long
  Dim dummy() As Byte
  maxLen = FLACGetPictureSizeW(Index)
  If maxLen <= 0 Then Exit Function
  ReDim dummy(maxLen - 1)
  ln = FLACGetPictureArrayW(VarPtr(dummy(0)), maxLen, Index)
  If ln <= 0 Then
    Debug.Print "Fehler beim Laden des Bildes:" & AUDIOGetLastErrorText()
    Exit Function
  End If
  ReDim Preserve dummy(ln - 1)
  FLACGetPictureArray = dummy
End Function
Public Function FLACGetPictureCount() As Integer
  FLACGetPictureCount = FLACGetPictureCountW()
End Function
Public Function FLACGetPictureDescription(ByVal Index As Integer) As String
  FLACGetPictureDescription = StrConv(FLACGetPictureDescriptionW(Index), vbFromUnicode)
End Function
Public Function FLACGetPictureFile(ByVal FileName As String, ByVal Index As Integer) As Integer
  FLACGetPictureFile = FLACGetPictureFileW(StrPtr(FileName), Index)
End Function
Public Function FLACGetPictureMime(ByVal Index As Integer) As String
  FLACGetPictureMime = StrConv(FLACGetPictureMimeW(Index), vbFromUnicode)
End Function
Public Function FLACGetPictureSize(ByVal Index As Integer) As Long
  FLACGetPictureSize = FLACGetPictureSizeW(Index)
End Function
Public Function FLACGetPictureTypeText(ByVal Index As Integer) As String
  FLACGetPictureTypeText = StrConv(FLACGetPictureTypeTextW(Index), vbFromUnicode)
End Function
Public Function FLACGetPictureType(ByVal Index As Integer) As Integer
  FLACGetPictureType = FLACGetPictureTypeW(Index)
End Function
Public Function ID3V2GetPictureSize(ByVal Index As Integer) As Long
  ID3V2GetPictureSize = ID3V2GetPictureSizeW(Index)
End Function
Public Function MP4GetPictureSize(ByVal Index As Integer) As Long
  MP4GetPictureSize = MP4GetPictureSizeW(Index)
End Function
Public Function WMAGetPictureSize(ByVal Index As Integer) As Long
  WMAGetPictureSize = WMAGetPictureSizeW(Index)
End Function
Public Function WAVGetDisplayText() As String
  WAVGetDisplayText = StrConv(WAVGetDisplayTextW(), vbFromUnicode)
End Function
Public Sub WAVSetDisplayText(ByVal textString As String)
  Call WAVSetDisplayTextW(StrPtr(textString))
End Sub

Public Function WMAIsVBR() As Integer
  WMAIsVBR = WMPAIsVBRW()
End Function

Public Function WMASaveChangesToFile(ByVal FileName As String) As Integer
  WMASaveChangesToFile = WMASaveChangesToFileW(StrPtr(FileName))
End Function

Public Function WMASaveChanges() As Integer
  WMASaveChanges = WMASaveChangesW()
End Function

Public Function WMAAddPictureArray(arr() As Byte, ByVal maxLen As Long, ByVal Description As String, ByVal PictureType As Integer, Index As Integer) As Integer
  WMAAddPictureArray = WMAAddPictureArrayW(VarPtr(arr(0)), maxLen, StrPtr(Description), PictureType, Index)
End Function

Public Function WMAAddPictureFile(ByVal FileName As String, ByVal Description As String, ByVal PictureType As Integer, Index As Integer) As Integer
  WMAAddPictureFile = WMAAddPictureFileW(StrPtr(FileName), StrPtr(Description), PictureType, Index)
End Function


