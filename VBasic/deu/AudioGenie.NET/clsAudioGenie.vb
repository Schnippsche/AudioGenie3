Option Strict Off
Option Explicit On
Friend Class clsAudioGenie
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
	Private Declare Function AACGetBitRateTypeW Lib "AudioGenie3.dll" () As String
	Private Declare Function AACGetHeaderTypeW Lib "AudioGenie3.dll" () As String
	Private Declare Function AACGetProfileW Lib "AudioGenie3.dll" () As String
	Private Declare Function APEExistsW Lib "AudioGenie3.dll" () As Short
	Private Declare Function APEGetAlbumW Lib "AudioGenie3.dll" () As String
	Private Declare Function APEGetArtistW Lib "AudioGenie3.dll" () As String
	Private Declare Function APEGetCommentW Lib "AudioGenie3.dll" () As String
	Private Declare Function APEGetCopyrightW Lib "AudioGenie3.dll" () As String
	Private Declare Function APEGetGenreW Lib "AudioGenie3.dll" () As String
	Private Declare Function APEGetItemKeysW Lib "AudioGenie3.dll" () As String
	Private Declare Function APEGetSizeW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function APEGetTitleW Lib "AudioGenie3.dll" () As String
	Private Declare Function APEGetTrackW Lib "AudioGenie3.dll" () As String
	Private Declare Function APEGetUserItemW Lib "AudioGenie3.dll" (ByVal key As Integer) As String
	Private Declare Function APEGetVersionW Lib "AudioGenie3.dll" () As String
	Private Declare Function APEGetYearW Lib "AudioGenie3.dll" () As String
	Private Declare Function APERemoveTagFromFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function APERemoveTagW Lib "AudioGenie3.dll" () As Short
	Private Declare Function APESaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function APESaveChangesW Lib "AudioGenie3.dll" () As Short
	Private Declare Sub APESetAlbumW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub APESetArtistW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub APESetCommentW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub APESetCopyrightW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub APESetGenreW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub APESetTitleW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub APESetTrackW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub APESetUserItemW Lib "AudioGenie3.dll" (ByVal key As Integer, ByVal textString As Integer)
	Private Declare Sub APESetYearW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Function AUDIOAnalyzeFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function AUDIOIsValidFormatW Lib "AudioGenie3.dll" () As Short
	Private Declare Function AUDIOGetAlbumW Lib "AudioGenie3.dll" () As String
	Private Declare Function AUDIOGetArtistW Lib "AudioGenie3.dll" () As String
	Private Declare Function AUDIOGetBitrateW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function AUDIOGetChannelModeW Lib "AudioGenie3.dll" () As String
	Private Declare Function AUDIOGetChannelsW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function AUDIOGetCommentW Lib "AudioGenie3.dll" () As String
	Private Declare Function AUDIOGetComposerW Lib "AudioGenie3.dll" () As String
	Private Declare Function AUDIOGetDurationMillisW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function AUDIOGetDurationW Lib "AudioGenie3.dll" () As Single
	Private Declare Function AUDIOGetFileSizeW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function AUDIOGetGenreW Lib "AudioGenie3.dll" () As String
	Private Declare Function AUDIOGetLastErrorNumberW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function AUDIOGetLastErrorTextW Lib "AudioGenie3.dll" () As String
	Private Declare Function AUDIOGetLastFileW Lib "AudioGenie3.dll" () As String
	Private Declare Function AUDIOGetMD5ValueW Lib "AudioGenie3.dll" () As String
	Private Declare Function AUDIOGetSampleRateW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function AUDIOGetTitleW Lib "AudioGenie3.dll" () As String
	Private Declare Function AUDIOGetTrackW Lib "AudioGenie3.dll" () As String
	Private Declare Function AUDIOGetVersionW Lib "AudioGenie3.dll" () As String
	Private Declare Function AUDIOGetYearW Lib "AudioGenie3.dll" () As String
	Private Declare Function AUDIOSaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function AUDIOSaveChangesW Lib "AudioGenie3.dll" () As Short
	Private Declare Sub AUDIOSetAlbumW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub AUDIOSetArtistW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub AUDIOSetCommentW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub AUDIOSetComposerW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub AUDIOSetGenreW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub AUDIOSetTitleW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub AUDIOSetTrackW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub AUDIOSetYearW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Function FLACGetBitsPerSampleW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function FLACGetCompressionRatioW Lib "AudioGenie3.dll" () As Single
	Private Declare Function FLACGetItemKeysW Lib "AudioGenie3.dll" () As String
	Private Declare Function FLACGetMaxBlockSizeW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function FLACGetMaxFrameSizeW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function FLACGetMinBlockSizeW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function FLACGetMinFrameSizeW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function FLACGetSamplesW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function FLACGetUserItemW Lib "AudioGenie3.dll" (ByVal ItemKey As Integer) As String
	Private Declare Function FLACGetVendorW Lib "AudioGenie3.dll" () As String
	Private Declare Function FLACSaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function FLACSaveChangesW Lib "AudioGenie3.dll" () As Short
	Private Declare Sub FLACSetUserItemW Lib "AudioGenie3.dll" (ByVal ItemKey As Integer, ByVal textString As Integer)
	Private Declare Sub FLACSetVendorW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Function GetAudioGenieVersionW Lib "AudioGenie3.dll" () As String
	Private Declare Function GetConfigValueW Lib "AudioGenie3.dll" (ByVal key As Integer) As Integer
	Private Declare Function GetMD5ValueFromFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As String
	Private Declare Function ID3V1ExistsW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V1GetAlbumW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V1GetArtistW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V1GetCommentW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V1GetGenreIDW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V1GetGenreItemW Lib "AudioGenie3.dll" (ByVal number As Short) As String
	Private Declare Function ID3V1GetGenreW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V1GetGenresW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V1GetTitleW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V1GetTrackW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V1GetVersionW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V1GetYearW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V1RemoveTagFromFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function ID3V1RemoveTagW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V1SaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function ID3V1SaveChangesW Lib "AudioGenie3.dll" () As Short
	Private Declare Sub ID3V1SetAlbumW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub ID3V1SetArtistW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub ID3V1SetCommentW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub ID3V1SetGenreIDW Lib "AudioGenie3.dll" (ByVal nNewValue As Short)
	Private Declare Sub ID3V1SetGenreW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub ID3V1SetTitleW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub ID3V1SetTrackW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub ID3V1SetYearW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Function ID3V2AddAudioEncryptionW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal URL As Integer, ByVal PreviewStart As Short, ByVal PreviewLength As Short) As Short
	Private Declare Function ID3V2AddAudioSeekPointW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal start As Integer, ByVal Length As Integer, ByVal numbers As Short, ByVal BitsPerPoint As Byte) As Short
	Private Declare Function ID3V2AddChapterW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal Title As Integer, ByVal Description As Integer, ByVal startTime As Integer, ByVal endTime As Integer) As Short
	Private Declare Function ID3V2AddChildElementW Lib "AudioGenie3.dll" (ByVal ParentTocID As Integer, ByVal ChildID As Integer) As Short
	Private Declare Function ID3V2AddCommentW Lib "AudioGenie3.dll" (ByVal Language As Integer, ByVal Description As Integer, ByVal text As Integer) As Short
	Private Declare Function ID3V2AddCommercialFrameW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Price As Integer, ByVal validUntil As Integer, ByVal contactUrl As Integer, ByVal receivedAs As Short, ByVal Seller As Integer, ByVal Description As Integer) As Short
	Private Declare Function ID3V2AddEncryptionW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal URL As Integer, ByVal symbol As Short) As Short
	Private Declare Function ID3V2AddEqualisationW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Interpolationmethod As Byte, ByVal Identification As Integer) As Short
	Private Declare Function ID3V2AddEventTimingCodesW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal TimestampFormat As Byte) As Short
	Private Declare Function ID3V2AddGeneralObjectW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Mime As Integer, ByVal FileName As Integer, ByVal Description As Integer) As Short
	Private Declare Function ID3V2AddGroupIdentificationW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal Length As Integer, ByVal URL As Integer, ByVal symbol As Byte) As Short
	Private Declare Function ID3V2AddLinkedInformationW Lib "AudioGenie3.dll" (ByVal FrameIdentifier As Integer, ByVal URL As Integer, ByVal additionalData As Integer) As Short
	Private Declare Function ID3V2AddLyricW Lib "AudioGenie3.dll" (ByVal Language As Integer, ByVal Description As Integer, ByVal text As Integer) As Short
	Private Declare Function ID3V2AddMpegLocationLookupTableW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal Length As Integer, ByVal Frames As Integer, ByVal Bytes As Integer, ByVal Milliseconds As Integer, ByVal BytesDeviation As Byte, ByVal MillisecondsDeviation As Byte) As Short
	Private Declare Function ID3V2AddMusicCdIdentifierW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal Length As Integer) As Short
	'UPGRADE_NOTE: dateString wurde aktualisiert auf dateString_Renamed. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
	Private Declare Function ID3V2AddOwnershipW Lib "AudioGenie3.dll" (ByVal Price As Integer, ByVal dateString_Renamed As Integer, ByVal Seller As Integer) As Short
	Private Declare Function ID3V2AddPictureArrayW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Description As Integer, ByVal PictureType As Short) As Short
	Private Declare Function ID3V2AddPictureFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer, ByVal Description As Integer, ByVal PictureType As Short, ByVal asLink As Short) As Short
	Private Declare Function ID3V2AddPopularimeterW Lib "AudioGenie3.dll" (ByVal Email As Integer, ByVal Rating As Short, ByVal Counter As Integer) As Short
	Private Declare Function ID3V2AddPositionSynchronisationW Lib "AudioGenie3.dll" (ByVal TimestampFormat As Byte, ByVal Position As Integer) As Short
	Private Declare Function ID3V2AddPrivateFrameW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal Length As Integer, ByVal URL As Integer) As Short
	Private Declare Function ID3V2AddRecommendedBufferSizeW Lib "AudioGenie3.dll" (ByVal BufferSize As Integer, ByVal EmbeddedInfoFlag As Byte, ByVal offset As Integer) As Short
	Private Declare Function ID3V2AddRelativeVolumeAdjustmentW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal Length As Integer, ByVal Identifier As Integer) As Short
	Private Declare Function ID3V2AddReverbW Lib "AudioGenie3.dll" (ByVal reverbLeft As Short, ByVal reverbRight As Short, ByVal bouncesLeft As Byte, ByVal bouncesRight As Byte, ByVal feedbackLeftToLeft As Byte, ByVal feedbackLeftToRight As Byte, ByVal feedbackRightToRight As Byte, ByVal feedbackRightToLeft As Byte, ByVal premixLeftToRight As Byte, ByVal premixRightToLeft As Byte) As Short
	Private Declare Function ID3V2AddSignatureFrameW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal Length As Integer, ByVal GroupSymbol As Short) As Short
	Private Declare Function ID3V2AddSyncLyricW Lib "AudioGenie3.dll" (ByVal Language As Integer, ByVal Description As Integer, ByVal text As Integer, ByVal ContentType As Short, ByVal TimestampFormat As Short) As Short
	'UPGRADE_NOTE: format wurde aktualisiert auf format_Renamed. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
	Private Declare Function ID3V2AddSynchronizedTempoW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal Length As Integer, ByVal format_Renamed As Short) As Short
	Private Declare Function ID3V2AddTableOfContentW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal Title As Integer, ByVal Description As Integer, ByVal isOrdered As Short) As Short
	Private Declare Function ID3V2AddUniqueFileIdentifierW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal Length As Integer, ByVal Owner As Integer) As Short
	Private Declare Function ID3V2AddUserFrameW Lib "AudioGenie3.dll" (ByVal Language As Integer, ByVal text As Integer) As Short
	Private Declare Function ID3V2AddUserTextW Lib "AudioGenie3.dll" (ByVal Description As Integer, ByVal text As Integer) As Short
	Private Declare Function ID3V2AddUserURLW Lib "AudioGenie3.dll" (ByVal Description As Integer, ByVal URL As Integer) As Short
	Private Declare Function ID3V2DeleteAddendumW Lib "AudioGenie3.dll" (ByVal ID As Integer) As Short
	Private Declare Function ID3V2DeleteAllFramesW Lib "AudioGenie3.dll" (ByVal frameID As Integer) As Short
	Private Declare Function ID3V2DeleteChildElementW Lib "AudioGenie3.dll" (ByVal ParentTocID As Integer, ByVal ChildID As Integer) As Short
	Private Declare Function ID3V2DeleteSelectedFrameW Lib "AudioGenie3.dll" (ByVal frameID As Integer, ByVal Index As Short) As Short
	Private Declare Function ID3V2DeleteSubFrameW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal frameID As Integer) As Short
	Private Declare Function ID3V2ExistsW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetAddendumDescriptionW Lib "AudioGenie3.dll" (ByVal ID As Integer) As String
	Private Declare Function ID3V2GetAddendumTitleW Lib "AudioGenie3.dll" (ByVal ID As Integer) As String
	Private Declare Function ID3V2GetAddendumTypeW Lib "AudioGenie3.dll" (ByVal ID As Integer) As Short
	Private Declare Function ID3V2GetAllFrameIDsW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V2GetAudioEncryptionDataW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetAudioEncryptionPreviewLengthW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Function ID3V2GetAudioEncryptionPreviewStartW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Function ID3V2GetAudioEncryptionURLW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetAudioSeekPointBitsPerIndexpointW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetAudioSeekPointDataW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer) As Integer
	Private Declare Function ID3V2GetAudioSeekPointLengthW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetAudioSeekPointNumberW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetAudioSeekPointStartW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetChapterEndTimeW Lib "AudioGenie3.dll" (ByVal ID As Integer) As Integer
	Private Declare Function ID3V2GetChapterStartTimeW Lib "AudioGenie3.dll" (ByVal ID As Integer) As Integer
	Private Declare Function ID3V2GetChildElementIDW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal Index As Short) As String
	Private Declare Function ID3V2GetChildElementsW Lib "AudioGenie3.dll" (ByVal ID As Integer) As Short
	Private Declare Function ID3V2GetCommentDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetCommentLanguageW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetCommentW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetCommercialFrameContactURLW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetCommercialFrameDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetCommercialFramePictureMimeW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetCommercialFramePictureW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetCommercialFramePriceW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetCommercialFrameReceivedAsW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Function ID3V2GetCommercialFrameSellerNameW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetCommercialFrameValidUntilW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetEncryptionDataW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetEncryptionSymbolW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Function ID3V2GetEncryptionURLW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetEqualisationAdjustmentBitsW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Function ID3V2GetEqualisationDataW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetEqualisationIdentificationW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetEqualisationInterpolationW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Function ID3V2GetEventTimingCodesDataW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer) As Integer
	Private Declare Function ID3V2GetEventTimingCodesTimestampFormatW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetFrameCountW Lib "AudioGenie3.dll" (ByVal frameID As Integer) As Short
	Private Declare Function ID3V2GetGeneralObjectDataW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetGeneralObjectDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetGeneralObjectFilenameW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetGeneralObjectMimeW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetGenreW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V2GetGroupIdentificationDataW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetGroupIdentificationSymbolW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Function ID3V2GetGroupIdentificationURLW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetLinkedInformationAdditionalDataW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetLinkedInformationFrameIdentifierW Lib "AudioGenie3.dll" (ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetLinkedInformationURLW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetLyricDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetLyricLanguageW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetLyricW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetMpegLocationLookupTableBytesDeviationW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetMpegLocationLookupTableBytesW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetMpegLocationLookupTableDataW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer) As Integer
	Private Declare Function ID3V2GetMpegLocationLookupTableFramesW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetMpegLocationLookupTableMillisecondsDeviationW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetMpegLocationLookupTableMillisecondsW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetMusicCdIdentifierDataW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer) As Integer
	Private Declare Function ID3V2GetOwnershipDateW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V2GetOwnershipPriceW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V2GetOwnershipSellerW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V2GetPictureArrayW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetPictureDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetPictureFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer, ByVal Index As Short) As Short
	Private Declare Function ID3V2GetPictureMimeW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetPictureTypeTextW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetPictureTypeW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Function ID3V2GetPlayCounterW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetPopularimeterCounterW Lib "AudioGenie3.dll" (ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetPopularimeterEmailW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetPopularimeterRatingW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Function ID3V2GetPositionSynchronisationTimestampFormatW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetPositionSynchronisationValueW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetPossibleCHAPIDW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V2GetPossibleTOCIDW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V2GetPrivateFrameDataW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetPrivateFrameURLW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetRecommendedBufferSizeFlagW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetRecommendedBufferSizeOffsetW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetRecommendedBufferSizeValueW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetRelativeVolumeAdjustmentDataW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetRelativeVolumeAdjustmentIdentifierW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetReverbBouncesLeftW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetReverbBouncesRightW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetReverbFeedbackLeftToLeftW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetReverbFeedbackLeftToRightW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetReverbFeedbackRightToLeftW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetReverbFeedbackRightToRightW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetReverbLeftW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetReverbPremixLeftToRightW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetReverbPremixRightToLeftW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetReverbRightW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetSeekOffsetW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetSignatureFrameDataW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetSignatureFrameGroupSymbolW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Function ID3V2GetSizeW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function ID3V2GetSubFrameDescriptionW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal Index As Short) As String
	Private Declare Function ID3V2GetSubFrameIDW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetSubFrameImageTypeW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal Index As Short) As Short
	Private Declare Function ID3V2GetSubFrameImageW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal ID As Integer, ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetSubFrameTextW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal Index As Short) As String
	Private Declare Function ID3V2GetSubFrameTypeW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal Index As Short) As Short
	Private Declare Function ID3V2GetSubFramesW Lib "AudioGenie3.dll" (ByVal ID As Integer) As Short
	Private Declare Function ID3V2GetSyncLyricContentTypeW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Function ID3V2GetSyncLyricDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetSyncLyricLanguageW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetSyncLyricTimeFormatW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Function ID3V2GetSyncLyricW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetSynchronizedTempoDataW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer) As Integer
	Private Declare Function ID3V2GetSynchronizedTempoFormatW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2GetTOCIsOrderedW Lib "AudioGenie3.dll" (ByVal ID As Integer) As Short
	Private Declare Function ID3V2GetTOCRootIDW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V2GetTextFrameW Lib "AudioGenie3.dll" (ByVal frameID As Integer) As String
	Private Declare Function ID3V2GetURLFrameW Lib "AudioGenie3.dll" (ByVal frameID As Integer) As String
	Private Declare Function ID3V2GetUniqueFileIdentifierDataW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Index As Short) As Integer
	Private Declare Function ID3V2GetUniqueFileIdentifierOwnerW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetUserFrameLanguageW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetUserFrameW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetUserTextDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetUserTextW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetUserURLDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetUserURLW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function ID3V2GetVersionW Lib "AudioGenie3.dll" () As String
	Private Declare Function ID3V2ImportCueFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function ID3V2RemoveTagFromFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function ID3V2RemoveTagW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2SaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function ID3V2SaveChangesW Lib "AudioGenie3.dll" () As Short
	Private Declare Function ID3V2SetAddendumDescriptionW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal newDescription As Integer) As Short
	Private Declare Function ID3V2SetAddendumTitleW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal newTitle As Integer) As Short
	Private Declare Function ID3V2SetChapterTimesW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal startTime As Integer, ByVal endTime As Integer) As Short
	'UPGRADE_NOTE: format wurde aktualisiert auf format_Renamed. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
	Private Declare Function ID3V2SetFormatAndEncodingW Lib "AudioGenie3.dll" (ByVal format_Renamed As Short, ByVal encoding As Short) As Short
	Private Declare Sub ID3V2SetGenreW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Function ID3V2SetImageSubFrameW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal ID As Integer, ByVal Description As Integer, ByVal PictureType As Short) As Short
	Private Declare Sub ID3V2SetPlayCounterW Lib "AudioGenie3.dll" (ByVal Counter As Integer)
	Private Declare Function ID3V2AddSeekOffsetW Lib "AudioGenie3.dll" (ByVal offset As Integer) As Short
	Private Declare Function ID3V2SetTOCIsOrderedW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal status As Short) As Short
	Private Declare Sub ID3V2SetTextFrameW Lib "AudioGenie3.dll" (ByVal frameID As Integer, ByVal textString As Integer)
	Private Declare Function ID3V2SetTextSubFrameW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal frameID As Integer, ByVal textString As Integer, ByVal Description As Integer) As Short
	Private Declare Sub ID3V2SetURLFrameW Lib "AudioGenie3.dll" (ByVal frameID As Integer, ByVal textString As Integer)
	Private Declare Function ID3V2SetURLSubFrameW Lib "AudioGenie3.dll" (ByVal ID As Integer, ByVal frameID As Integer, ByVal urlString As Integer, ByVal Description As Integer) As Short
	Private Declare Function LYRICSExistsW Lib "AudioGenie3.dll" () As Short
	Private Declare Function LYRICSGetAlbumW Lib "AudioGenie3.dll" () As String
	Private Declare Function LYRICSGetArtistW Lib "AudioGenie3.dll" () As String
	Private Declare Function LYRICSGetAuthorW Lib "AudioGenie3.dll" () As String
	Private Declare Function LYRICSGetGenreW Lib "AudioGenie3.dll" () As String
	Private Declare Function LYRICSGetImageLinkW Lib "AudioGenie3.dll" () As String
	Private Declare Function LYRICSGetIndicationW Lib "AudioGenie3.dll" () As String
	Private Declare Function LYRICSGetInformationW Lib "AudioGenie3.dll" () As String
	Private Declare Function LYRICSGetLyricsW Lib "AudioGenie3.dll" () As String
	Private Declare Function LYRICSGetSizeW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function LYRICSGetStartPositionW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function LYRICSGetTitleW Lib "AudioGenie3.dll" () As String
	Private Declare Function LYRICSGetVersionW Lib "AudioGenie3.dll" () As String
	Private Declare Function LYRICSRemoveTagFromFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function LYRICSRemoveTagW Lib "AudioGenie3.dll" () As Short
	Private Declare Function LYRICSSaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function LYRICSSaveChangesW Lib "AudioGenie3.dll" () As Short
	Private Declare Sub LYRICSSetAlbumW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub LYRICSSetArtistW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub LYRICSSetAuthorW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub LYRICSSetGenreW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub LYRICSSetImageLinkW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub LYRICSSetIndicationW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub LYRICSSetInformationW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub LYRICSSetLyricsW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub LYRICSSetTitleW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Function MONKEYGetBitsW Lib "AudioGenie3.dll" () As Short
	Private Declare Function MONKEYGetCompressionRatioW Lib "AudioGenie3.dll" () As Single
	Private Declare Function MONKEYGetCompressionW Lib "AudioGenie3.dll" () As String
	Private Declare Function MONKEYGetFramesW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function MONKEYGetPeakW Lib "AudioGenie3.dll" () As Single
	Private Declare Function MONKEYGetSamplesPerFrameW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function MONKEYGetSamplesW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function MP4GetiTuneFrameW Lib "AudioGenie3.dll" (ByVal frameID As Integer) As String
	Private Declare Function MP4GetTextFrameW Lib "AudioGenie3.dll" (ByVal frameID As Integer) As String
	Private Declare Function MP4GetAllFrameIDsW Lib "AudioGenie3.dll" () As String
	Private Declare Function MP4GetGenreW Lib "AudioGenie3.dll" () As String
	Private Declare Function MP4GetPictureArrayW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Index As Short) As Integer
	Private Declare Function MP4GetPictureMimeW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function MP4GetPictureCountW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function MP4GetPictureFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer, ByVal Index As Short) As Short
	Private Declare Function MP4GetTrackW Lib "AudioGenie3.dll" () As String
	Private Declare Function MP4GetVersionW Lib "AudioGenie3.dll" () As String
	Private Declare Function MP4SaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function MP4SaveChangesW Lib "AudioGenie3.dll" () As Short
	Private Declare Sub MP4DeleteEntriesW Lib "AudioGenie3.dll" ()
	Private Declare Sub MP4SetGenreW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub MP4SetiTuneFrameW Lib "AudioGenie3.dll" (ByVal frameID As Integer, ByVal newText As Integer)
	Private Declare Sub MP4SetTextFrameW Lib "AudioGenie3.dll" (ByVal frameID As Integer, ByVal newText As Integer)
	Private Declare Sub MP4SetTrackW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Function MP4AddPictureArrayW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal Length As Integer) As Short
	Private Declare Function MP4AddPictureFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Sub MP4DeletePicturesW Lib "AudioGenie3.dll" ()
	Private Declare Function MP4DeletePictureW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Function MPEGGetEmphasisW Lib "AudioGenie3.dll" () As String
	Private Declare Function MPEGGetEncoderW Lib "AudioGenie3.dll" () As String
	Private Declare Function MPEGGetFramePositionW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function MPEGGetFrameSizeW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function MPEGGetFramesW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function MPEGGetLayerW Lib "AudioGenie3.dll" () As String
	Private Declare Function MPEGGetVersionW Lib "AudioGenie3.dll" () As String
	Private Declare Function MPEGIsCopyrightedW Lib "AudioGenie3.dll" () As Short
	Private Declare Function MPEGIsOriginalW Lib "AudioGenie3.dll" () As Short
	Private Declare Function MPEGIsPaddingW Lib "AudioGenie3.dll" () As Short
	Private Declare Function MPEGIsPrivateW Lib "AudioGenie3.dll" () As Short
	Private Declare Function MPEGIsProtectedW Lib "AudioGenie3.dll" () As Short
	Private Declare Function MPEGIsVBRW Lib "AudioGenie3.dll" () As Short
	Private Declare Function MPEGSetCopyrightedW Lib "AudioGenie3.dll" (ByVal FileName As Integer, ByVal newValue As Short) As Short
	Private Declare Function MPEGSetOriginalW Lib "AudioGenie3.dll" (ByVal FileName As Integer, ByVal newValue As Short) As Short
	Private Declare Function MPEGSetPrivateW Lib "AudioGenie3.dll" (ByVal FileName As Integer, ByVal newValue As Short) As Short
	Private Declare Function MPPGetFramesW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function MPPGetStreamVersionW Lib "AudioGenie3.dll" () As Short
	Private Declare Function OGGGetAlbumW Lib "AudioGenie3.dll" () As String
	Private Declare Function OGGGetArtistW Lib "AudioGenie3.dll" () As String
	Private Declare Function OGGGetBitRateNominalW Lib "AudioGenie3.dll" () As Integer
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
	Private Declare Function OGGGetUserItemW Lib "AudioGenie3.dll" (ByVal ItemKey As Integer) As String
	Private Declare Function OGGGetVendorW Lib "AudioGenie3.dll" () As String
	Private Declare Function OGGGetVersionW Lib "AudioGenie3.dll" () As String
	Private Declare Function OGGRemoveTagFromFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function OGGRemoveTagW Lib "AudioGenie3.dll" () As Short
	Private Declare Function OGGSaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function OGGSaveChangesW Lib "AudioGenie3.dll" () As Short
	Private Declare Sub OGGSetAlbumW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetArtistW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetCommentW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetCopyrightW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetDateW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetDescriptionW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetGenreW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetISRCW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetLicenseW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetLocationW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetOrganizationW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetPerformerW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetTitleW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetTrackW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetUserItemW Lib "AudioGenie3.dll" (ByVal ItemKey As Integer, ByVal textString As Integer)
	Private Declare Sub OGGSetVendorW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub OGGSetVersionW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Function OGGGetContactW Lib "AudioGenie3.dll" () As String
	Private Declare Sub OGGSetContactW Lib "AudioGenie3.dll" (ByVal textString As Integer)
	Private Declare Sub SetConfigValueW Lib "AudioGenie3.dll" (ByVal key As Integer, ByVal value As Integer)
	Private Declare Sub SetLogFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer)
	Private Declare Function WAVGetBitsPerSampleW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function WAVGetBlockAlignW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function WAVGetBytesPerSecondW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function WAVGetChannelsW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function WAVGetFormatIDW Lib "AudioGenie3.dll" () As Short
	Private Declare Function WAVGetFormatW Lib "AudioGenie3.dll" () As String
	Private Declare Function WAVGetHeaderSizeW Lib "AudioGenie3.dll" () As Integer
	Private Declare Function WAVGetTextFrameW Lib "AudioGenie3.dll" (ByVal frameID As Integer) As String
	Private Declare Function WAVSaveChangesW Lib "AudioGenie3.dll" () As Short
	Private Declare Sub WAVSetTextFrameW Lib "AudioGenie3.dll" (ByVal frameID As Integer, ByVal textString As Integer)
	Private Declare Function WMADeletePictureW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Sub WMADeletePicturesW Lib "AudioGenie3.dll" ()
	Private Declare Function WMAGetItemKeysW Lib "AudioGenie3.dll" () As String
	Private Declare Function WMAGetPictureArrayW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Index As Short) As Integer
	Private Declare Function WMAGetPictureDescriptionW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function WMAGetPictureFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer, ByVal Index As Short) As Short
	Private Declare Function WMAGetPictureMimeW Lib "AudioGenie3.dll" (ByVal Index As Short) As String
	Private Declare Function WMAGetPictureTypeW Lib "AudioGenie3.dll" (ByVal Index As Short) As Short
	Private Declare Function WMAGetPictureCountW Lib "AudioGenie3.dll" () As Short
	Private Declare Function WMAGetUserItemW Lib "AudioGenie3.dll" (ByVal ItemKey As Integer) As String
	Private Declare Function WMASaveChangesToFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer) As Short
	Private Declare Function WMASaveChangesW Lib "AudioGenie3.dll" () As Short
	Private Declare Function WMAAddPictureArrayW Lib "AudioGenie3.dll" (ByVal arr As Integer, ByVal maxLen As Integer, ByVal Description As Integer, ByVal PictureType As Short, ByVal Index As Short) As Short
	Private Declare Function WMAAddPictureFileW Lib "AudioGenie3.dll" (ByVal FileName As Integer, ByVal Description As Integer, ByVal PictureType As Short, ByVal Index As Short) As Short
	Private Declare Sub WMASetUserItemW Lib "AudioGenie3.dll" (ByVal ItemKey As Integer, ByVal textString As Integer)
	
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
		WAV_IARL = &H4941524C
		WAV_IART = &H49415254
		WAV_ICMS = &H49434D53
		WAV_ICMT = &H49434D54
		WAV_ICOP = &H49434F50
		WAV_ICRD = &H49435244
		WAV_ICRP = &H49435250
		WAV_IDIM = &H4944494D
		WAV_IDPI = &H49445049
		WAV_IENG = &H49454E47
		WAV_IGNR = &H49474E52
		WAV_IKEY = &H494B4559
		WAV_ILGT = &H494C4754
		WAV_IMED = &H494D4544
		WAV_INAM = &H494E414D
		WAV_IPLT = &H49504C54
		WAV_IPRD = &H49505244
		WAV_ISBJ = &H4953424A
		WAV_ISFT = &H49534654
		WAV_ISHP = &H49534850
		WAV_ISRC = &H49535243
		WAV_ISRF = &H49535246
		WAV_ITCH = &H49544348
		WAV_ITRK = &H4954524B
	End Enum
	
	Public Enum MP4FRAMES
		MP4_ALBUM = &HA9616C62 ' ©alb
		MP4_ARTIST = &HA9617274 ' ©art
		MP4_AUTHOR = &HA9617574 ' ©aut
		MP4_ALBUMARTIST = &H61415254 ' aART
		MP4_BEATSPERMINUTE = &H746D706F ' tmpo
		MP4_CATEGORY = &H63617467 ' catg
		MP4_CDSETNUMBER = &H6469736B ' disk
		MP4_COMMENT = &HA9636D74 ' ©cmt
		MP4_COMPOSER = &HA9777274 ' ©wrt
		MP4_COMPILATION = &H6370696C ' cpil
		MP4_DAY = &HA9646179 ' ©day
		MP4_COPYRIGHT = &HA9637079 ' ©cpy
		MP4_CREATEDATE = &HA9646179 ' ©day
		MP4_DESCRIPTION = &HA9646573 ' ©des
		MP4_INFORMATION = &HA9696E66 ' ©inf
		MP4_DIRECTOR = &HA9646972 ' ©dir
		MP4_DISCLAIMER = &HA9646973 ' ©dis
		MP4_ENCODER = &HA9746F6F ' ©too
		MP4_GROUPING = &HA9677270 ' ©grp
		MP4_KEYWORD = &H6B657977 ' keyw
		MP4_LYRICS = &HA96C7972 ' ©lyr
		MP4_TITLE = &HA96E616D ' ©nam
		MP4_NETURL = &HA975726C ' ©url
		MP4_ORIGINALARTIST = &HA96F7065 ' ©ope
		MP4_ORIGINALFORMAT = &HA9666D74 ' ©fmt
		MP4_ORIGINALSOURCE = &HA9737263 ' ©src
		MP4_PERFORMER = &HA9707266 ' ©prf
		MP4_PRODUCER = &HA9707264 ' ©prd
		MP4_WARNING = &HA977726E ' ©wrn
	End Enum
	
	Const WM_ALBUMARTIST As String = "WM/AlbumArtist"
	Const WM_ALBUMTITLE As String = "WM/AlbumTitle"
	Const WM_AUTHOR As String = "WM/Author"
	Const WM_AUTHORURL As String = "WM/AuthorURL"
	Const WM_BEATSPERMINUTE As String = "WM/BeatsPerMinute"
	Const WM_CATEGORY As String = "WM/Category"
	Const WM_COMPOSER As String = "WM/Composer"
	Const WM_CONDUCTOR As String = "WM/Conductor"
	Const WM_CONTENTDISTRIBUTOR As String = "WM/ContentDistributor"
	Const WM_CONTENTGROUPDESCRIPTION As String = "WM/ContentGroupDescription"
	Const WM_DESCRIPTION As String = "WM/Description"
	Const WM_ENCODEDBY As String = "WM/EncodedBy"
	Const WM_ENCODINGSETTINGS As String = "WM/EncodingSettings"
	Const WM_ENCODINGTIME As String = "WM/EncodingTime"
	Const WM_GENRE As String = "WM/Genre"
	Const WM_INITIALKEY As String = "WM/InitialKey"
	Const WM_ISRC As String = "WM/ISRC"
	Const WM_LANGUAGE As String = "WM/Language"
	Const WM_LYRICS As String = "WM/Lyrics"
	Const WM_MOOD As String = "WM/Mood"
	Const WM_PARENTALRATING As String = "WM/ParentalRating"
	Const WM_PARTOFSET As String = "WM/PartOfSet"
	Const WM_PERIOD As String = "WM/Period"
	Const WM_PRODUCER As String = "WM/Producer"
	Const WM_PROMOTIONURL As String = "WM/PromotionURL"
	Const WM_PROVIDER As String = "WM/Provider"
	Const WM_PROVIDERCOPYRIGHT As String = "WM/ProviderCopyright"
	Const WM_PROVIDERRATING As String = "WM/ProviderRating"
	Const WM_PUBLISHER As String = "WM/Publisher"
	Const WM_TITLE As String = "WM/Title"
	Const WM_TOOLNAME As String = "WM/ToolName"
	Const WM_TOOLVERSION As String = "WM/ToolVersion"
	Const WM_TRACK As String = "WM/Track"
	Const WM_TRACKNUMBER As String = "WM/TrackNumber"
	Const WM_USERWEBURL As String = "WM/UserWebURL"
	Const WM_WRITER As String = "WM/Writer"
	Const WM_YEAR As String = "WM/Year"
	
	
	' *****************************************************
	'                     Properties
	' *****************************************************
	Public Property APEAlbum() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			APEAlbum = StrConv(APEGetAlbumW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call APESetAlbumW(StrPtr(Value))
		End Set
	End Property
	
	Public Property APEArtist() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			APEArtist = StrConv(APEGetArtistW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call APESetArtistW(StrPtr(Value))
		End Set
	End Property
	
	Public Property APEComment() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			APEComment = StrConv(APEGetCommentW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call APESetCommentW(StrPtr(Value))
		End Set
	End Property
	
	Public Property APECopyright() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			APECopyright = StrConv(APEGetCopyrightW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call APESetCopyrightW(StrPtr(Value))
		End Set
	End Property
	
	Public Property APEGenre() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			APEGenre = StrConv(APEGetGenreW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call APESetGenreW(StrPtr(Value))
		End Set
	End Property
	
	Public Property APETitle() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			APETitle = StrConv(APEGetTitleW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call APESetTitleW(StrPtr(Value))
		End Set
	End Property
	
	Public Property APETrack() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			APETrack = StrConv(APEGetTrackW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call APESetTrackW(StrPtr(Value))
		End Set
	End Property
	
	Public Property APEUserItem(ByVal key As String) As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			APEUserItem = StrConv(APEGetUserItemW(StrPtr(key)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call APESetUserItemW(StrPtr(key), StrPtr(Value))
		End Set
	End Property
	
	Public Property APEYear() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			APEYear = StrConv(APEGetYearW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call APESetYearW(StrPtr(Value))
		End Set
	End Property
	
	Public Property AUDIOAlbum() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			AUDIOAlbum = StrConv(AUDIOGetAlbumW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call AUDIOSetAlbumW(StrPtr(Value))
		End Set
	End Property
	
	Public Property AUDIOArtist() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			AUDIOArtist = StrConv(AUDIOGetArtistW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call AUDIOSetArtistW(StrPtr(Value))
		End Set
	End Property
	
	Public Property AUDIOComment() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			AUDIOComment = StrConv(AUDIOGetCommentW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call AUDIOSetCommentW(StrPtr(Value))
		End Set
	End Property
	
	Public Property AUDIOComposer() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			AUDIOComposer = StrConv(AUDIOGetComposerW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call AUDIOSetComposerW(StrPtr(Value))
		End Set
	End Property
	
	Public Property AUDIOGenre() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			AUDIOGenre = StrConv(AUDIOGetGenreW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call AUDIOSetGenreW(StrPtr(Value))
		End Set
	End Property
	
	Public Property AUDIOTitle() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			AUDIOTitle = StrConv(AUDIOGetTitleW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call AUDIOSetTitleW(StrPtr(Value))
		End Set
	End Property
	
	Public Property AUDIOTrack() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			AUDIOTrack = StrConv(AUDIOGetTrackW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call AUDIOSetTrackW(StrPtr(Value))
		End Set
	End Property
	
	Public Property AUDIOYear() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			AUDIOYear = StrConv(AUDIOGetYearW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call AUDIOSetYearW(StrPtr(Value))
		End Set
	End Property
	
	Public Property ConfigValue(ByVal key As CONFIG_VALUES) As Integer
		Get
			ConfigValue = GetConfigValueW(key)
		End Get
		Set(ByVal Value As Integer)
			Call SetConfigValueW(key, Value)
		End Set
	End Property
	
	Public Property FLACAlbum() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACAlbum = StrConv(FLACGetUserItemW(StrPtr("ALBUM")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("ALBUM"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACArranger() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACArranger = StrConv(FLACGetUserItemW(StrPtr("ARRANGER")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("ARRANGER"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACArtist() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACArtist = StrConv(FLACGetUserItemW(StrPtr("ARTIST")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("ARTIST"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACAuthor() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACAuthor = StrConv(FLACGetUserItemW(StrPtr("AUTHOR")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("AUTHOR"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACComment() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACComment = StrConv(FLACGetUserItemW(StrPtr("COMMENT")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("COMMENT"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACComposer() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACComposer = StrConv(FLACGetUserItemW(StrPtr("COMPOSER")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("COMPOSER"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACContact() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACContact = StrConv(FLACGetUserItemW(StrPtr("CONTACT")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("CONTACT"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACCopyright() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACCopyright = StrConv(FLACGetUserItemW(StrPtr("COPYRIGHT")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("COPYRIGHT"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACDate() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACDate = StrConv(FLACGetUserItemW(StrPtr("DATE")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("DATE"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACDescription() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACDescription = StrConv(FLACGetUserItemW(StrPtr("DESCRIPTION")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("DESCRIPTION"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACDiscNumber() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACDiscNumber = StrConv(FLACGetUserItemW(StrPtr("DISCNUMBER")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("DISCNUMBER"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACEnsemble() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACEnsemble = StrConv(FLACGetUserItemW(StrPtr("ENSEMBLE")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("ENSEMBLE"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACGenre() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACGenre = StrConv(FLACGetUserItemW(StrPtr("GENRE")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("GENRE"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACISRC() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACISRC = StrConv(FLACGetUserItemW(StrPtr("ISRC")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("ISRC"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACLicense() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACLicense = StrConv(FLACGetUserItemW(StrPtr("LICENSE")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("LICENSE"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACLocation() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACLocation = StrConv(FLACGetUserItemW(StrPtr("LOCATION")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("LOCATION"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACLyricist() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACLyricist = StrConv(FLACGetUserItemW(StrPtr("LYRICIST")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("LYRICIST"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACOrganization() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACOrganization = StrConv(FLACGetUserItemW(StrPtr("ORGANIZATION")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("ORGANIZATION"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACPerformer() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACPerformer = StrConv(FLACGetUserItemW(StrPtr("PERFORMER")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("PERFORMER"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACPublisher() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACPublisher = StrConv(FLACGetUserItemW(StrPtr("PUBLISHER")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("PUBLISHER"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACTitle() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACTitle = StrConv(FLACGetUserItemW(StrPtr("TITLE")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("TITLE"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACTrack() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACTrack = StrConv(FLACGetUserItemW(StrPtr("TRACKNUMBER")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("TRACKNUMBER"), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACUserItem(ByVal ItemKey As String) As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACUserItem = StrConv(FLACGetUserItemW(StrPtr(ItemKey)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr(ItemKey), StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACVendor() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			FLACVendor = StrConv(FLACGetVendorW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetVendorW(StrPtr(Value))
		End Set
	End Property
	
	Public Property FLACVersion() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			FLACVersion = StrConv(FLACGetUserItemW(StrPtr("VERSION")), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call FLACSetUserItemW(StrPtr("VERSION"), StrPtr(Value))
		End Set
	End Property
	
	Public Property ID3V1Album() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			ID3V1Album = StrConv(ID3V1GetAlbumW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call ID3V1SetAlbumW(StrPtr(Value))
		End Set
	End Property
	
	Public Property ID3V1Artist() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			ID3V1Artist = StrConv(ID3V1GetArtistW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call ID3V1SetArtistW(StrPtr(Value))
		End Set
	End Property
	
	Public Property ID3V1Comment() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			ID3V1Comment = StrConv(ID3V1GetCommentW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call ID3V1SetCommentW(StrPtr(Value))
		End Set
	End Property
	
	Public Property ID3V1GenreID() As Short
		Get
			ID3V1GenreID = ID3V1GetGenreIDW()
		End Get
		Set(ByVal Value As Short)
			Call ID3V1SetGenreIDW(Value)
		End Set
	End Property
	
	Public Property ID3V1Genre() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			ID3V1Genre = StrConv(ID3V1GetGenreW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call ID3V1SetGenreW(StrPtr(Value))
		End Set
	End Property
	
	Public Property ID3V1Title() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			ID3V1Title = StrConv(ID3V1GetTitleW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call ID3V1SetTitleW(StrPtr(Value))
		End Set
	End Property
	
	Public Property ID3V1Track() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			ID3V1Track = StrConv(ID3V1GetTrackW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call ID3V1SetTrackW(StrPtr(Value))
		End Set
	End Property
	
	Public Property ID3V1Year() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			ID3V1Year = StrConv(ID3V1GetYearW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call ID3V1SetYearW(StrPtr(Value))
		End Set
	End Property
	
	Public Property ID3V2Album() As String
		Get ' TALB
			ID3V2Album = ID3V2TextFrame(ID3V2FRAMES.ID3F_TALB)
		End Get
		Set(ByVal Value As String) ' TALB
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TALB) = Value
		End Set
	End Property
	
	Public Property ID3V2AlbumArtistSort() As String
		Get ' TSO2 ITunes
			ID3V2AlbumArtistSort = ID3V2TextFrame(ID3V2FRAMES.ID3F_TSO2)
		End Get
		Set(ByVal Value As String) ' TSO2
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TSO2) = Value
		End Set
	End Property
	
	Public Property ID3V2AlbumSort() As String
		Get ' TSOA ITunes
			ID3V2AlbumSort = ID3V2TextFrame(ID3V2FRAMES.ID3F_TSOA)
		End Get
		Set(ByVal Value As String) ' TSOA
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TSOA) = Value
		End Set
	End Property
	
	Public Property ID3V2Artist() As String
		Get ' TPE1
			ID3V2Artist = ID3V2TextFrame(ID3V2FRAMES.ID3F_TPE1)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TPE1) = Value
		End Set
	End Property
	
	Public Property ID3V2ArtistSort() As String
		Get ' TSOP itunes
			ID3V2ArtistSort = ID3V2TextFrame(ID3V2FRAMES.ID3F_TSOP)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TSOP) = Value
		End Set
	End Property
	
	Public Property ID3V2ArtistURL() As String
		Get ' WOAR
			ID3V2ArtistURL = ID3V2URLFrame(ID3V2FRAMES.ID3F_WOAR)
		End Get
		Set(ByVal Value As String)
			ID3V2URLFrame(ID3V2FRAMES.ID3F_WOAR) = Value
		End Set
	End Property
	
	Public Property ID3V2AudiofileURL() As String
		Get ' WOAF
			ID3V2AudiofileURL = ID3V2URLFrame(ID3V2FRAMES.ID3F_WOAF)
		End Get
		Set(ByVal Value As String)
			ID3V2URLFrame(ID3V2FRAMES.ID3F_WOAF) = Value
		End Set
	End Property
	
	Public Property ID3V2AudiosourceURL() As String
		Get ' WOAS
			ID3V2AudiosourceURL = ID3V2URLFrame(ID3V2FRAMES.ID3F_WOAS)
		End Get
		Set(ByVal Value As String)
			ID3V2URLFrame(ID3V2FRAMES.ID3F_WOAS) = Value
		End Set
	End Property
	
	Public Property ID3V2BeatsPerMinute() As String
		Get ' TBPM
			ID3V2BeatsPerMinute = ID3V2TextFrame(ID3V2FRAMES.ID3F_TBPM)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TBPM) = Value
		End Set
	End Property
	
	Public Property ID3V2Category() As String
		Get ' TIT1
			ID3V2Category = ID3V2TextFrame(ID3V2FRAMES.ID3F_TIT1)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TIT1) = Value
		End Set
	End Property
	
	Public Property ID3V2CommercialURL() As String
		Get ' WCOM
			ID3V2CommercialURL = ID3V2URLFrame(ID3V2FRAMES.ID3F_WCOM)
		End Get
		Set(ByVal Value As String) ' WCOM
			ID3V2URLFrame(ID3V2FRAMES.ID3F_WCOM) = Value
		End Set
	End Property
	
	Public Property ID3V2Composer() As String
		Get ' TCOM
			ID3V2Composer = ID3V2TextFrame(ID3V2FRAMES.ID3F_TCOM)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TCOM) = Value
		End Set
	End Property
	
	Public Property ID3V2ComposerSort() As String
		Get ' TSOC ITunes
			ID3V2ComposerSort = ID3V2TextFrame(ID3V2FRAMES.ID3F_TSOC)
		End Get
		Set(ByVal Value As String) ' TSOC
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TSOC) = Value
		End Set
	End Property
	
	Public Property ID3V2Conductor() As String
		Get ' TPE3
			ID3V2Conductor = ID3V2TextFrame(ID3V2FRAMES.ID3F_TPE3)
		End Get
		Set(ByVal Value As String) ' TPE3
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TPE3) = Value
		End Set
	End Property
	
	Public Property ID3V2ContentType() As String
		Get ' TCON
			ID3V2Conductor = ID3V2TextFrame(ID3V2FRAMES.ID3F_TCON)
		End Get
		Set(ByVal Value As String) ' TCON
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TCON) = Value
		End Set
	End Property
	
	Public Property ID3V2Copyright() As String
		Get ' TCOP
			ID3V2Copyright = ID3V2TextFrame(ID3V2FRAMES.ID3F_TCOP)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TCOP) = Value
		End Set
	End Property
	
	Public Property ID3V2CopyrightURL() As String
		Get ' WCOP
			ID3V2CopyrightURL = ID3V2URLFrame(ID3V2FRAMES.ID3F_WCOP)
		End Get
		Set(ByVal Value As String)
			ID3V2URLFrame(ID3V2FRAMES.ID3F_WCOP) = Value
		End Set
	End Property
	
	Public Property ID3V2Date() As String
		Get ' TDAT
			ID3V2Date = ID3V2TextFrame(ID3V2FRAMES.ID3F_TDAT)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TDAT) = Value
		End Set
	End Property
	
	Public Property ID3V2Encoder() As String
		Get ' TENC
			ID3V2Encoder = ID3V2TextFrame(ID3V2FRAMES.ID3F_TENC)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TENC) = Value
		End Set
	End Property
	
	Public Property ID3V2EncodingSettings() As String
		Get ' TSSE
			ID3V2EncodingSettings = ID3V2TextFrame(ID3V2FRAMES.ID3F_TSSE)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TSSE) = Value
		End Set
	End Property
	
	Public Property ID3V2EncodingTime() As String
		Get ' TDEN
			ID3V2EncodingTime = ID3V2TextFrame(ID3V2FRAMES.ID3F_TDEN)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TDEN) = Value
		End Set
	End Property
	
	Public Property ID3V2FileOwner() As String
		Get ' TOWN
			ID3V2FileOwner = ID3V2TextFrame(ID3V2FRAMES.ID3F_TOWN)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TOWN) = Value
		End Set
	End Property
	
	Public Property ID3V2FileType() As String
		Get ' TFLT
			ID3V2FileType = ID3V2TextFrame(ID3V2FRAMES.ID3F_TFLT)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TFLT) = Value
		End Set
	End Property
	
	Public Property ID3V2Genre() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			ID3V2Genre = StrConv(ID3V2GetGenreW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call ID3V2SetGenreW(StrPtr(Value))
		End Set
	End Property
	
	Public Property ID3V2InetRadioName() As String
		Get ' TRSN
			ID3V2InetRadioName = ID3V2TextFrame(ID3V2FRAMES.ID3F_TRSN)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TRSN) = Value
		End Set
	End Property
	
	Public Property ID3V2InetRadioOwner() As String
		Get ' TRSO
			ID3V2InetRadioOwner = ID3V2TextFrame(ID3V2FRAMES.ID3F_TRSO)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TRSO) = Value
		End Set
	End Property
	
	Public Property ID3V2InitialKey() As String
		Get ' TKEY
			ID3V2InitialKey = ID3V2TextFrame(ID3V2FRAMES.ID3F_TKEY)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TKEY) = Value
		End Set
	End Property
	
	Public Property ID3V2Interpreted() As String
		Get ' TPE4
			ID3V2Interpreted = ID3V2TextFrame(ID3V2FRAMES.ID3F_TPE4)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TPE4) = Value
		End Set
	End Property
	
	Public Property ID3V2InvolvedPeopleList() As String
		Get ' TIPL && IPLS
			ID3V2InvolvedPeopleList = ID3V2TextFrame(ID3V2FRAMES.ID3F_TIPL)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TIPL) = Value
		End Set
	End Property
	
	Public Property ID3V2ISRC() As String
		Get ' TSRC
			ID3V2ISRC = ID3V2TextFrame(ID3V2FRAMES.ID3F_TSRC)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TSRC) = Value
		End Set
	End Property
	
	Public Property ID3V2Language() As String
		Get ' TLAN
			ID3V2Language = ID3V2TextFrame(ID3V2FRAMES.ID3F_TLAN)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TLAN) = Value
		End Set
	End Property
	
	Public Property ID3V2Length() As String
		Get ' TLEN
			ID3V2Length = ID3V2TextFrame(ID3V2FRAMES.ID3F_TLEN)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TLEN) = Value
		End Set
	End Property
	
	Public Property ID3V2MediaType() As String
		Get ' TMED
			ID3V2MediaType = ID3V2TextFrame(ID3V2FRAMES.ID3F_TMED)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TMED) = Value
		End Set
	End Property
	
	Public Property ID3V2Mood() As String
		Get ' TMOO
			ID3V2Mood = ID3V2TextFrame(ID3V2FRAMES.ID3F_TMOO)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TMOO) = Value
		End Set
	End Property
	
	Public Property ID3V2MusicianCreditsList() As String
		Get ' TMCL
			ID3V2MusicianCreditsList = ID3V2TextFrame(ID3V2FRAMES.ID3F_TMCL)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TMCL) = Value
		End Set
	End Property
	
	Public Property ID3V2Orchestra() As String
		Get ' TPE2
			ID3V2Orchestra = ID3V2TextFrame(ID3V2FRAMES.ID3F_TPE2)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TPE2) = Value
		End Set
	End Property
	
	Public Property ID3V2OriginalAlbum() As String
		Get ' TOAL
			ID3V2OriginalAlbum = ID3V2TextFrame(ID3V2FRAMES.ID3F_TOAL)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TOAL) = Value
		End Set
	End Property
	
	Public Property ID3V2OriginalArtist() As String
		Get ' TOPE
			ID3V2OriginalArtist = ID3V2TextFrame(ID3V2FRAMES.ID3F_TOPE)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TOPE) = Value
		End Set
	End Property
	
	Public Property ID3V2OriginalFilename() As String
		Get ' TOFN
			ID3V2OriginalFilename = ID3V2TextFrame(ID3V2FRAMES.ID3F_TOFN)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TOFN) = Value
		End Set
	End Property
	
	Public Property ID3V2OriginalReleaseYear() As String
		Get ' TORY
			ID3V2OriginalReleaseYear = ID3V2TextFrame(ID3V2FRAMES.ID3F_TORY)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TORY) = Value
		End Set
	End Property
	
	Public Property ID3V2OriginalReleaseTime() As String
		Get ' TDOR
			ID3V2OriginalReleaseTime = ID3V2TextFrame(ID3V2FRAMES.ID3F_TDOR)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TDOR) = Value
		End Set
	End Property
	
	Public Property ID3V2OriginalTextwriter() As String
		Get ' TOLY
			ID3V2OriginalTextwriter = ID3V2TextFrame(ID3V2FRAMES.ID3F_TOLY)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TOLY) = Value
		End Set
	End Property
	
	Public Property ID3V2PartofSet() As String
		Get ' TPOS
			ID3V2PartofSet = ID3V2TextFrame(ID3V2FRAMES.ID3F_TPOS)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TPOS) = Value
		End Set
	End Property
	
	Public Property ID3V2PaymentURL() As String
		Get ' WPAY
			ID3V2PaymentURL = ID3V2URLFrame(ID3V2FRAMES.ID3F_WPAY)
		End Get
		Set(ByVal Value As String)
			ID3V2URLFrame(ID3V2FRAMES.ID3F_WPAY) = Value
		End Set
	End Property
	
	Public Property ID3V2PlayCounter() As Integer
		Get
			ID3V2PlayCounter = ID3V2GetPlayCounterW()
		End Get
		Set(ByVal Value As Integer)
			Call ID3V2SetPlayCounterW(Value)
		End Set
	End Property
	
	Public Property ID3V2PlaylistDelay() As String
		Get ' TDLY
			ID3V2PlaylistDelay = ID3V2TextFrame(ID3V2FRAMES.ID3F_TDLY)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TDLY) = Value
		End Set
	End Property
	
	Public Property ID3V2ProducedNotice() As String
		Get ' TPRO
			ID3V2ProducedNotice = ID3V2TextFrame(ID3V2FRAMES.ID3F_TPRO)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TPRO) = Value
		End Set
	End Property
	
	Public Property ID3V2Publisher() As String
		Get ' TPUB
			ID3V2Publisher = ID3V2TextFrame(ID3V2FRAMES.ID3F_TPUB)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TPUB) = Value
		End Set
	End Property
	
	Public Property ID3V2PublisherURL() As String
		Get ' WPUB
			ID3V2PublisherURL = ID3V2URLFrame(ID3V2FRAMES.ID3F_WPUB)
		End Get
		Set(ByVal Value As String)
			ID3V2URLFrame(ID3V2FRAMES.ID3F_WPUB) = Value
		End Set
	End Property
	
	Public Property ID3V2RadiostationURL() As String
		Get ' WORS
			ID3V2RadiostationURL = ID3V2URLFrame(ID3V2FRAMES.ID3F_WORS)
		End Get
		Set(ByVal Value As String)
			ID3V2URLFrame(ID3V2FRAMES.ID3F_WORS) = Value
		End Set
	End Property
	
	Public Property ID3V2RecordingDate() As String
		Get ' TRDA
			ID3V2RecordingDate = ID3V2TextFrame(ID3V2FRAMES.ID3F_TRDA)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TRDA) = Value
		End Set
	End Property
	
	Public Property ID3V2RecordingTime() As String
		Get ' TDRC
			ID3V2RecordingTime = ID3V2TextFrame(ID3V2FRAMES.ID3F_TDRC)
		End Get
		Set(ByVal Value As String) ' TDRC
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TDRC) = Value
		End Set
	End Property
	
	Public Property ID3V2ReleaseTime() As String
		Get ' TDRL
			ID3V2ReleaseTime = ID3V2TextFrame(ID3V2FRAMES.ID3F_TDRL)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TDRL) = Value
		End Set
	End Property
	
	Public Property ID3V2Size() As String
		Get ' TSIZ
			ID3V2Size = ID3V2TextFrame(ID3V2FRAMES.ID3F_TSIZ)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TSIZ) = Value
		End Set
	End Property
	
	Public Property ID3V2Subtitle() As String
		Get ' TIT3
			ID3V2Subtitle = ID3V2TextFrame(ID3V2FRAMES.ID3F_TIT3)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TIT3) = Value
		End Set
	End Property
	
	Public Property ID3V2SubtitleSet() As String
		Get ' TSST
			ID3V2SubtitleSet = ID3V2TextFrame(ID3V2FRAMES.ID3F_TSST)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TSST) = Value
		End Set
	End Property
	
	Public Property ID3V2TaggingTime() As String
		Get ' TDTG
			ID3V2TaggingTime = ID3V2TextFrame(ID3V2FRAMES.ID3F_TDTG)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TDTG) = Value
		End Set
	End Property
	
	Public Property ID3V2TextWriter() As String
		Get ' TEXT
			ID3V2TextWriter = ID3V2TextFrame(ID3V2FRAMES.ID3F_TEXT)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TEXT) = Value
		End Set
	End Property
	
	Public Property ID3V2Time() As String
		Get ' TIME
			ID3V2Time = ID3V2TextFrame(ID3V2FRAMES.ID3F_TIME)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TIME) = Value
		End Set
	End Property
	
	Public Property ID3V2Title() As String
		Get ' TIT2
			ID3V2Title = ID3V2TextFrame(ID3V2FRAMES.ID3F_TIT2)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TIT2) = Value
		End Set
	End Property
	
	Public Property ID3V2TitleSort() As String
		Get ' TSOT itunes
			ID3V2TitleSort = ID3V2TextFrame(ID3V2FRAMES.ID3F_TSOT)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TSOT) = Value
		End Set
	End Property
	Public Property ID3V2Track() As String
		Get ' TRCK
			ID3V2Track = ID3V2TextFrame(ID3V2FRAMES.ID3F_TRCK)
		End Get
		Set(ByVal Value As String)
			ID3V2TextFrame(ID3V2FRAMES.ID3F_TRCK) = Value
		End Set
	End Property
	
	Public Property ID3V2TextFrame(ByVal frameID As ID3V2FRAMES) As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			ID3V2TextFrame = StrConv(ID3V2GetTextFrameW(frameID), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call ID3V2SetTextFrameW(frameID, StrPtr(Value))
		End Set
	End Property
	
	Public Property ID3V2Year() As String
		Get ' TYER
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			ID3V2Year = StrConv(ID3V2GetTextFrameW(ID3V2FRAMES.ID3F_TYER), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call ID3V2SetTextFrameW(ID3V2FRAMES.ID3F_TYER, StrPtr(Value))
		End Set
	End Property
	
	Public Property ID3V2URLFrame(ByVal frameID As Integer) As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			ID3V2URLFrame = StrConv(ID3V2GetURLFrameW(frameID), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call ID3V2SetURLFrameW(frameID, StrPtr(Value))
		End Set
	End Property
	
	Public Property LYRICSAlbum() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			LYRICSAlbum = StrConv(LYRICSGetAlbumW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call LYRICSSetAlbumW(StrPtr(Value))
		End Set
	End Property
	
	Public Property LYRICSArtist() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			LYRICSArtist = StrConv(LYRICSGetArtistW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call LYRICSSetArtistW(StrPtr(Value))
		End Set
	End Property
	
	Public Property LYRICSAuthor() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			LYRICSAuthor = StrConv(LYRICSGetAuthorW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call LYRICSSetAuthorW(StrPtr(Value))
		End Set
	End Property
	
	Public Property LYRICSGenre() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			LYRICSGenre = StrConv(LYRICSGetGenreW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call LYRICSSetGenreW(StrPtr(Value))
		End Set
	End Property
	
	Public Property LYRICSImageLink() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			LYRICSImageLink = StrConv(LYRICSGetImageLinkW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call LYRICSSetImageLinkW(StrPtr(Value))
		End Set
	End Property
	
	Public Property LYRICSIndication() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			LYRICSIndication = StrConv(LYRICSGetIndicationW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call LYRICSSetIndicationW(StrPtr(Value))
		End Set
	End Property
	
	Public Property LYRICSInformation() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			LYRICSInformation = StrConv(LYRICSGetInformationW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call LYRICSSetInformationW(StrPtr(Value))
		End Set
	End Property
	
	Public Property LYRICSLyrics() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			LYRICSLyrics = StrConv(LYRICSGetLyricsW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call LYRICSSetLyricsW(StrPtr(Value))
		End Set
	End Property
	
	Public Property LYRICSTitle() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			LYRICSTitle = StrConv(LYRICSGetTitleW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call LYRICSSetTitleW(StrPtr(Value))
		End Set
	End Property
	
	Public Property MP4Album() As String
		Get
			MP4Album = MP4GetTextFrame(MP4FRAMES.MP4_ALBUM)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_ALBUM, Value)
		End Set
	End Property
	
	Public Property MP4Artist() As String
		Get
			MP4Artist = MP4GetTextFrame(MP4FRAMES.MP4_ARTIST)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_ARTIST, Value)
		End Set
	End Property
	
	Public Property MP4Author() As String
		Get
			MP4Author = MP4GetTextFrame(MP4FRAMES.MP4_AUTHOR)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_AUTHOR, Value)
		End Set
	End Property
	
	Public Property MP4BeatsPerMinute() As String
		Get
			MP4BeatsPerMinute = MP4GetTextFrame(MP4FRAMES.MP4_BEATSPERMINUTE)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_BEATSPERMINUTE, Value)
		End Set
	End Property
	
	Public Property MP4CDSetNumber() As String
		Get
			MP4CDSetNumber = MP4GetTextFrame(MP4FRAMES.MP4_CDSETNUMBER)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_CDSETNUMBER, Value)
		End Set
	End Property
	
	Public Property MP4Comment() As String
		Get
			MP4Comment = MP4GetTextFrame(MP4FRAMES.MP4_COMMENT)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_COMMENT, Value)
		End Set
	End Property
	
	Public Property MP4Compilation() As String
		Get
			MP4Compilation = MP4GetTextFrame(MP4FRAMES.MP4_COMPILATION)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_COMPILATION, Value)
		End Set
	End Property
	
	Public Property MP4Composer() As String
		Get
			MP4Composer = MP4GetTextFrame(MP4FRAMES.MP4_COMPOSER)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_COMPOSER, Value)
		End Set
	End Property
	
	Public Property MP4Copyright() As String
		Get
			MP4Copyright = MP4GetTextFrame(MP4FRAMES.MP4_COPYRIGHT)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_COPYRIGHT, Value)
		End Set
	End Property
	
	Public Property MP4Date() As String
		Get
			MP4Copyright = MP4GetTextFrame(MP4FRAMES.MP4_CREATEDATE)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_CREATEDATE, Value)
		End Set
	End Property
	
	Public Property MP4Description() As String
		Get
			MP4Description = MP4GetTextFrame(MP4FRAMES.MP4_DESCRIPTION)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_DESCRIPTION, Value)
		End Set
	End Property
	
	Public Property MP4Director() As String
		Get
			MP4Director = MP4GetTextFrame(MP4FRAMES.MP4_DIRECTOR)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_DIRECTOR, Value)
		End Set
	End Property
	
	Public Property MP4Disclaimer() As String
		Get
			MP4Disclaimer = MP4GetTextFrame(MP4FRAMES.MP4_DISCLAIMER)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_DISCLAIMER, Value)
		End Set
	End Property
	
	Public Property MP4Encoder() As String
		Get
			MP4Encoder = MP4GetTextFrame(MP4FRAMES.MP4_ENCODER)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_ENCODER, Value)
		End Set
	End Property
	
	Public Property MP4Genre() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			MP4Genre = StrConv(MP4GetGenreW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call MP4SetGenreW(StrPtr(Value))
		End Set
	End Property
	
	Public Property MP4NetUrl() As String
		Get
			MP4NetUrl = MP4GetTextFrame(MP4FRAMES.MP4_NETURL)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_NETURL, Value)
		End Set
	End Property
	
	Public Property MP4OriginalArtist() As String
		Get
			MP4OriginalArtist = MP4GetTextFrame(MP4FRAMES.MP4_ORIGINALARTIST)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_ORIGINALARTIST, Value)
		End Set
	End Property
	
	Public Property MP4OriginalFormat() As String
		Get
			MP4OriginalFormat = MP4GetTextFrame(MP4FRAMES.MP4_ORIGINALFORMAT)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_ORIGINALFORMAT, Value)
		End Set
	End Property
	
	Public Property MP4OriginalSource() As String
		Get
			MP4OriginalSource = MP4GetTextFrame(MP4FRAMES.MP4_ORIGINALSOURCE)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_ORIGINALSOURCE, Value)
		End Set
	End Property
	
	Public Property MP4Performer() As String
		Get
			MP4Performer = MP4GetTextFrame(MP4FRAMES.MP4_PERFORMER)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_PERFORMER, Value)
		End Set
	End Property
	
	Public Property MP4Producer() As String
		Get
			MP4Producer = MP4GetTextFrame(MP4FRAMES.MP4_PRODUCER)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_PRODUCER, Value)
		End Set
	End Property
	
	Public Property MP4Title() As String
		Get
			MP4Title = MP4GetTextFrame(MP4FRAMES.MP4_TITLE)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_TITLE, Value)
		End Set
	End Property
	
	Public Property MP4Track() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			MP4Track = StrConv(MP4GetTrackW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call MP4SetTrackW(StrPtr(Value))
		End Set
	End Property
	
	Public Property MP4Warning() As String
		Get
			MP4Warning = MP4GetTextFrame(MP4FRAMES.MP4_WARNING)
		End Get
		Set(ByVal Value As String)
			Call MP4SetTextFrame(MP4FRAMES.MP4_WARNING, Value)
		End Set
	End Property
	
	' ---------------------------------------------------------------------------------------
	Public Property OGGAlbum() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGAlbum = StrConv(OGGGetAlbumW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetAlbumW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGArtist() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGArtist = StrConv(OGGGetArtistW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetArtistW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGComment() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGComment = StrConv(OGGGetCommentW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetCommentW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGContact() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGContact = StrConv(OGGGetContactW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetContactW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGCopyright() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGCopyright = StrConv(OGGGetCopyrightW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetCopyrightW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGDate() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGDate = StrConv(OGGGetDateW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetDateW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGDescription() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGDescription = StrConv(OGGGetDescriptionW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetDescriptionW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGGenre() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGGenre = StrConv(OGGGetGenreW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetGenreW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGISRC() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGISRC = StrConv(OGGGetISRCW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetISRCW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGLicense() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGLicense = StrConv(OGGGetLicenseW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetLicenseW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGLocation() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGLocation = StrConv(OGGGetLocationW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetLocationW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGOrganization() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGOrganization = StrConv(OGGGetOrganizationW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetOrganizationW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGPerformer() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGPerformer = StrConv(OGGGetPerformerW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetPerformerW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGTitle() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGTitle = StrConv(OGGGetTitleW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetTitleW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGTrack() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGTrack = StrConv(OGGGetTrackW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetTrackW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGUserItem(ByVal ItemKey As String) As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			OGGUserItem = StrConv(OGGGetUserItemW(StrPtr(ItemKey)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetUserItemW(StrPtr(ItemKey), StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGVendor() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGVendor = StrConv(OGGGetVendorW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetVendorW(StrPtr(Value))
		End Set
	End Property
	
	Public Property OGGVersion() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			OGGVersion = StrConv(OGGGetVersionW(), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call OGGSetVersionW(StrPtr(Value))
		End Set
	End Property
	
	Public Property WAVArchivalLocation() As String
		Get
			WAVArchivalLocation = WAVTextFrame(WAVFRAMES.WAV_IARL)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_IARL) = Value
		End Set
	End Property
	
	Public Property WAVArtist() As String
		Get
			WAVArtist = WAVTextFrame(WAVFRAMES.WAV_IART)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_IART) = Value
		End Set
	End Property
	
	Public Property WAVComissioned() As String
		Get
			WAVComissioned = WAVTextFrame(WAVFRAMES.WAV_ICMS)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_ICMS) = Value
		End Set
	End Property
	
	Public Property WAVComment() As String
		Get
			WAVComment = WAVTextFrame(WAVFRAMES.WAV_ICMT)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_ICMT) = Value
		End Set
	End Property
	
	Public Property WAVCopyright() As String
		Get
			WAVCopyright = WAVTextFrame(WAVFRAMES.WAV_ICOP)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_ICOP) = Value
		End Set
	End Property
	
	Public Property WAVCreationDate() As String
		Get
			WAVCreationDate = WAVTextFrame(WAVFRAMES.WAV_ICRD)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_ICRD) = Value
		End Set
	End Property
	
	Public Property WAVCropped() As String
		Get
			WAVCropped = WAVTextFrame(WAVFRAMES.WAV_ICRP)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_ICRP) = Value
		End Set
	End Property
	
	Public Property WAVDimension() As String
		Get
			WAVDimension = WAVTextFrame(WAVFRAMES.WAV_IDIM)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_IDIM) = Value
		End Set
	End Property
	
	Public Property WAVDotsPerInch() As String
		Get
			WAVDotsPerInch = WAVTextFrame(WAVFRAMES.WAV_IDPI)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_IDPI) = Value
		End Set
	End Property
	
	Public Property WAVEngineer() As String
		Get
			WAVEngineer = WAVTextFrame(WAVFRAMES.WAV_IENG)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_IENG) = Value
		End Set
	End Property
	
	Public Property WAVGenre() As String
		Get
			WAVGenre = WAVTextFrame(WAVFRAMES.WAV_IGNR)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_IGNR) = Value
		End Set
	End Property
	
	Public Property WAVKeywords() As String
		Get
			WAVKeywords = WAVTextFrame(WAVFRAMES.WAV_IKEY)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_IKEY) = Value
		End Set
	End Property
	
	Public Property WAVLightness() As String
		Get
			WAVLightness = WAVTextFrame(WAVFRAMES.WAV_ILGT)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_ILGT) = Value
		End Set
	End Property
	
	Public Property WAVMedium() As String
		Get
			WAVMedium = WAVTextFrame(WAVFRAMES.WAV_IMED)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_IMED) = Value
		End Set
	End Property
	
	Public Property WAVName() As String
		Get
			WAVName = WAVTextFrame(WAVFRAMES.WAV_INAM)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_INAM) = Value
		End Set
	End Property
	
	Public Property WAVPaletteSetting() As String
		Get
			WAVPaletteSetting = WAVTextFrame(WAVFRAMES.WAV_IPLT)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_IPLT) = Value
		End Set
	End Property
	
	Public Property WAVProduct() As String
		Get
			WAVProduct = WAVTextFrame(WAVFRAMES.WAV_IPRD)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_IPRD) = Value
		End Set
	End Property
	
	Public Property WAVSharpness() As String
		Get
			WAVSharpness = WAVTextFrame(WAVFRAMES.WAV_ISHP)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_ISHP) = Value
		End Set
	End Property
	
	Public Property WAVSoftware() As String
		Get
			WAVSoftware = WAVTextFrame(WAVFRAMES.WAV_ISFT)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_ISFT) = Value
		End Set
	End Property
	
	Public Property WAVSourceForm() As String
		Get
			WAVSourceForm = WAVTextFrame(WAVFRAMES.WAV_ISRF)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_ISRF) = Value
		End Set
	End Property
	
	Public Property WAVSource() As String
		Get
			WAVSource = WAVTextFrame(WAVFRAMES.WAV_ISRC)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_ISRC) = Value
		End Set
	End Property
	
	Public Property WAVSubject() As String
		Get
			WAVSubject = WAVTextFrame(WAVFRAMES.WAV_ISBJ)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_ISBJ) = Value
		End Set
	End Property
	
	Public Property WAVTechnician() As String
		Get
			WAVTechnician = WAVTextFrame(WAVFRAMES.WAV_ITCH)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_ITCH) = Value
		End Set
	End Property
	
	Public Property WAVTextFrame(ByVal frameID As Integer) As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			WAVTextFrame = StrConv(WAVGetTextFrameW(frameID), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WAVSetTextFrameW(frameID, StrPtr(Value))
		End Set
	End Property
	
	Public Property WAVTrack() As String
		Get
			WAVTrack = WAVTextFrame(WAVFRAMES.WAV_ITRK)
		End Get
		Set(ByVal Value As String)
			WAVTextFrame(WAVFRAMES.WAV_ITRK) = Value
		End Set
	End Property
	
	Public Property WMAAlbumArtist() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAAlbumArtist = StrConv(WMAGetUserItemW(StrPtr(WM_ALBUMARTIST)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_ALBUMARTIST), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAAlbum() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAAlbum = StrConv(WMAGetUserItemW(StrPtr(WM_ALBUMTITLE)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_ALBUMTITLE), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAArtist() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAArtist = StrConv(WMAGetUserItemW(StrPtr(WM_AUTHOR)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_AUTHOR), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAAuthorURL() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAAuthorURL = StrConv(WMAGetUserItemW(StrPtr(WM_AUTHORURL)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_AUTHORURL), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMABeatsPerMinute() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMABeatsPerMinute = StrConv(WMAGetUserItemW(StrPtr(WM_BEATSPERMINUTE)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_BEATSPERMINUTE), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMACategory() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMACategory = StrConv(WMAGetUserItemW(StrPtr(WM_CATEGORY)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_CATEGORY), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAComment() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAComment = StrConv(WMAGetUserItemW(StrPtr(WM_DESCRIPTION)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_DESCRIPTION), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAComposer() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAComposer = StrConv(WMAGetUserItemW(StrPtr(WM_COMPOSER)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_COMPOSER), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAConductor() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAConductor = StrConv(WMAGetUserItemW(StrPtr(WM_CONDUCTOR)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_CONDUCTOR), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAContentGroupDescription() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAContentGroupDescription = StrConv(WMAGetUserItemW(StrPtr(WM_CONTENTGROUPDESCRIPTION)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_CONTENTGROUPDESCRIPTION), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMACopyright() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMACopyright = StrConv(WMAGetUserItemW(StrPtr(WM_PROVIDERCOPYRIGHT)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_PROVIDERCOPYRIGHT), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMADistributor() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMADistributor = StrConv(WMAGetUserItemW(StrPtr(WM_CONTENTDISTRIBUTOR)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_CONTENTDISTRIBUTOR), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAEncodedBy() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAEncodedBy = StrConv(WMAGetUserItemW(StrPtr(WM_ENCODEDBY)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_ENCODEDBY), StrPtr(Value))
		End Set
	End Property
	
	Public ReadOnly Property WMAEncodingSettings() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAEncodingSettings = StrConv(WMAGetUserItemW(StrPtr(WM_ENCODINGSETTINGS)), vbFromUnicode)
		End Get
	End Property
	Public WriteOnly Property WMAEncodingtings() As String
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_ENCODINGSETTINGS), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAEncodingTime() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAEncodingTime = StrConv(WMAGetUserItemW(StrPtr(WM_ENCODINGTIME)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_ENCODINGTIME), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAGenre() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAGenre = StrConv(WMAGetUserItemW(StrPtr(WM_GENRE)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_GENRE), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAISRC() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAISRC = StrConv(WMAGetUserItemW(StrPtr(WM_ISRC)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_ISRC), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAInitialKey() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAInitialKey = StrConv(WMAGetUserItemW(StrPtr(WM_INITIALKEY)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_INITIALKEY), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMALanguage() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMALanguage = StrConv(WMAGetUserItemW(StrPtr(WM_LANGUAGE)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_LANGUAGE), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMALyrics() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMALyrics = StrConv(WMAGetUserItemW(StrPtr(WM_LYRICS)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_LYRICS), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAMood() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAMood = StrConv(WMAGetUserItemW(StrPtr(WM_MOOD)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_MOOD), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAParentalRating() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAParentalRating = StrConv(WMAGetUserItemW(StrPtr(WM_PARENTALRATING)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_PARENTALRATING), StrPtr(Value))
		End Set
	End Property
	
	Public ReadOnly Property WMAPartOfSet() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAPartOfSet = StrConv(WMAGetUserItemW(StrPtr(WM_PARTOFSET)), vbFromUnicode)
		End Get
	End Property
	Public WriteOnly Property WMAPartOf() As String
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_PARTOFSET), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAPeriod() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAPeriod = StrConv(WMAGetUserItemW(StrPtr(WM_PERIOD)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_PERIOD), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAProducer() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAProducer = StrConv(WMAGetUserItemW(StrPtr(WM_PRODUCER)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_PRODUCER), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAPromotionURL() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAPromotionURL = StrConv(WMAGetUserItemW(StrPtr(WM_PROMOTIONURL)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_PROMOTIONURL), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAProvider() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAProvider = StrConv(WMAGetUserItemW(StrPtr(WM_PROVIDER)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_PROVIDER), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAPublisher() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAPublisher = StrConv(WMAGetUserItemW(StrPtr(WM_PUBLISHER)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_PUBLISHER), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAProviderRating() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAProviderRating = StrConv(WMAGetUserItemW(StrPtr(WM_PROVIDERRATING)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_PROVIDERRATING), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMATitle() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMATitle = StrConv(WMAGetUserItemW(StrPtr(WM_TITLE)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_TITLE), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAToolName() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAToolName = StrConv(WMAGetUserItemW(StrPtr(WM_TOOLNAME)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_TOOLNAME), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAToolVersion() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAToolVersion = StrConv(WMAGetUserItemW(StrPtr(WM_TOOLVERSION)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_TOOLVERSION), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMATrackNumber() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMATrackNumber = StrConv(WMAGetUserItemW(StrPtr(WM_TRACKNUMBER)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_TRACKNUMBER), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAUserItem(ByVal ItemKey As String) As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAUserItem = StrConv(WMAGetUserItemW(StrPtr(ItemKey)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(ItemKey), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAUserWebURL() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAUserWebURL = StrConv(WMAGetUserItemW(StrPtr(WM_USERWEBURL)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_USERWEBURL), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAWriter() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAWriter = StrConv(WMAGetUserItemW(StrPtr(WM_WRITER)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_WRITER), StrPtr(Value))
		End Set
	End Property
	
	Public Property WMAYear() As String
		Get
			'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			WMAYear = StrConv(WMAGetUserItemW(StrPtr(WM_YEAR)), vbFromUnicode)
		End Get
		Set(ByVal Value As String)
			'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
			Call WMASetUserItemW(StrPtr(WM_YEAR), StrPtr(Value))
		End Set
	End Property
	
	' *****************************************************
	'                     Functions
	' *****************************************************
	Public Function AACGetBitRateType() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		AACGetBitRateType = StrConv(AACGetBitRateTypeW(), vbFromUnicode)
	End Function
	
	Public Function AACGetHeaderType() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		AACGetHeaderType = StrConv(AACGetHeaderTypeW(), vbFromUnicode)
	End Function
	
	Public Function AACGetProfile() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		AACGetProfile = StrConv(AACGetProfileW(), vbFromUnicode)
	End Function
	
	Public Function APEExists() As Short
		APEExists = APEExistsW()
	End Function
	
	Public Function APEGetItemKeys() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		APEGetItemKeys = StrConv(APEGetItemKeysW(), vbFromUnicode)
	End Function
	
	Public Function APEGetSize() As Integer
		APEGetSize = APEGetSizeW()
	End Function
	
	Public Function APEGetVersion() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		APEGetVersion = StrConv(APEGetVersionW(), vbFromUnicode)
	End Function
	
	Public Function APERemoveTagFromFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		APERemoveTagFromFile = APERemoveTagFromFileW(StrPtr(FileName))
	End Function
	
	Public Function APERemoveTag() As Short
		APERemoveTag = APERemoveTagW()
	End Function
	
	Public Function APESaveChangesToFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		APESaveChangesToFile = APESaveChangesToFileW(StrPtr(FileName))
	End Function
	
	Public Function APESaveChanges() As Short
		APESaveChanges = APESaveChangesW()
	End Function
	
	Public Function AUDIOAnalyzeFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		AUDIOAnalyzeFile = AUDIOAnalyzeFileW(StrPtr(FileName))
	End Function
	
	Public Function AUDIOIsValidFormat() As Short
		AUDIOIsValidFormat = AUDIOIsValidFormatW()
	End Function
	
	Public Function AUDIOGetBitrate() As Integer
		AUDIOGetBitrate = AUDIOGetBitrateW()
	End Function
	
	Public Function AUDIOGetChannelMode() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		AUDIOGetChannelMode = StrConv(AUDIOGetChannelModeW(), vbFromUnicode)
	End Function
	
	Public Function AUDIOGetChannels() As Integer
		AUDIOGetChannels = AUDIOGetChannelsW()
	End Function
	
	Public Function AUDIOGetDurationMillis() As Integer
		AUDIOGetDurationMillis = AUDIOGetDurationMillisW()
	End Function
	
	Public Function AUDIOGetDuration() As Single
		AUDIOGetDuration = AUDIOGetDurationW()
	End Function
	
	Public Function AUDIOGetFileSize() As Integer
		AUDIOGetFileSize = AUDIOGetFileSizeW()
	End Function
	
	Public Function AUDIOGetLastErrorNumber() As Integer
		AUDIOGetLastErrorNumber = AUDIOGetLastErrorNumberW()
	End Function
	
	Public Function AUDIOGetLastErrorText() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		AUDIOGetLastErrorText = StrConv(AUDIOGetLastErrorTextW(), vbFromUnicode)
	End Function
	
	Public Function AUDIOGetLastFile() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		AUDIOGetLastFile = StrConv(AUDIOGetLastFileW(), vbFromUnicode)
	End Function
	
	Public Function AUDIOGetMD5Value() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		AUDIOGetMD5Value = StrConv(AUDIOGetMD5ValueW(), vbFromUnicode)
	End Function
	
	Public Function AUDIOGetSampleRate() As Integer
		AUDIOGetSampleRate = AUDIOGetSampleRateW()
	End Function
	
	Public Function AUDIOGetVersion() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		AUDIOGetVersion = StrConv(AUDIOGetVersionW(), vbFromUnicode)
	End Function
	
	Public Function AUDIOSaveChangesToFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		AUDIOSaveChangesToFile = AUDIOSaveChangesToFileW(StrPtr(FileName))
	End Function
	
	Public Function AUDIOSaveChanges() As Short
		AUDIOSaveChanges = AUDIOSaveChangesW()
	End Function
	
	Public Function GetAudioGenieVersion() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		GetAudioGenieVersion = StrConv(GetAudioGenieVersionW(), vbFromUnicode)
	End Function
	
	Public Function FLACGetBitsPerSample() As Integer
		FLACGetBitsPerSample = FLACGetBitsPerSampleW()
	End Function
	
	Public Function FLACGetCompressionRatio() As Single
		FLACGetCompressionRatio = FLACGetCompressionRatioW()
	End Function
	
	Public Function FLACGetItemKeys() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		FLACGetItemKeys = StrConv(FLACGetItemKeysW(), vbFromUnicode)
	End Function
	
	Public Function FLACGetMaxBlockSize() As Integer
		FLACGetMaxBlockSize = FLACGetMaxBlockSizeW()
	End Function
	
	Public Function FLACGetMaxFrameSize() As Integer
		FLACGetMaxFrameSize = FLACGetMaxFrameSizeW()
	End Function
	
	Public Function FLACGetMinBlockSize() As Integer
		FLACGetMinBlockSize = FLACGetMinBlockSizeW()
	End Function
	
	Public Function FLACGetMinFrameSize() As Integer
		FLACGetMinFrameSize = FLACGetMinFrameSizeW()
	End Function
	
	Public Function FLACGetSamples() As Integer
		FLACGetSamples = FLACGetSamplesW()
	End Function
	
	Public Function FLACSaveChangesToFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		FLACSaveChangesToFile = FLACSaveChangesToFileW(StrPtr(FileName))
	End Function
	
	Public Function FLACSaveChanges() As Short
		FLACSaveChanges = FLACSaveChangesW()
	End Function
	
	Public Function GetMD5ValueFromFile(ByVal FileName As String) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		GetMD5ValueFromFile = StrConv(GetMD5ValueFromFileW(StrPtr(FileName)), vbFromUnicode)
	End Function
	
	Public Function ID3V1Exists() As Short
		ID3V1Exists = ID3V1ExistsW()
	End Function
	
	Public Function ID3V1GetGenreItem(ByVal number As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V1GetGenreItem = StrConv(ID3V1GetGenreItemW(number), vbFromUnicode)
	End Function
	
	Public Function ID3V1GetGenres() As Short
		ID3V1GetGenres = ID3V1GetGenresW()
	End Function
	
	Public Function ID3V1GetVersion() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V1GetVersion = StrConv(ID3V1GetVersionW(), vbFromUnicode)
	End Function
	
	Public Function ID3V1RemoveTagFromFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V1RemoveTagFromFile = ID3V1RemoveTagFromFileW(StrPtr(FileName))
	End Function
	
	Public Function ID3V1RemoveTag() As Short
		ID3V1RemoveTag = ID3V1RemoveTagW()
	End Function
	
	Public Function ID3V1SaveChangesToFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V1SaveChangesToFile = ID3V1SaveChangesToFileW(StrPtr(FileName))
	End Function
	
	Public Function ID3V1SaveChanges() As Short
		ID3V1SaveChanges = ID3V1SaveChangesW()
	End Function
	
	Public Function ID3V2AddChapter(ByVal ID As String, ByVal Title As String, ByVal Description As String, ByVal startTime As Integer, ByVal endTime As Integer) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddChapter = ID3V2AddChapterW(StrPtr(ID), StrPtr(Title), StrPtr(Description), startTime, endTime)
	End Function
	Public Function ID3V2AddChildElement(ByVal TocID As String, ByVal ChildID As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddChildElement = ID3V2AddChildElementW(StrPtr(TocID), StrPtr(ChildID))
	End Function
	Public Function ID3V2AddTableOfContent(ByVal ID As String, ByVal Title As String, ByVal Description As String, ByVal isOrdered As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddTableOfContent = ID3V2AddTableOfContentW(StrPtr(ID), StrPtr(Title), StrPtr(Description), isOrdered)
	End Function
	Public Function ID3V2DeleteAddendum(ByVal ID As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2DeleteAddendum = ID3V2DeleteAddendumW(StrPtr(ID))
	End Function
	Public Function ID3V2DeleteChildElement(ByVal TocID As String, ByVal ChildID As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2DeleteChildElement = ID3V2DeleteChildElementW(StrPtr(TocID), StrPtr(ChildID))
	End Function
	Public Function ID3V2DeleteSubFrame(ByVal ID As String, ByVal Index As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2DeleteSubFrame = ID3V2DeleteSubFrameW(StrPtr(ID), Index)
	End Function
	Public Function ID3V2GetAddendumTitle(ByVal ID As String) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetAddendumTitle = StrConv(ID3V2GetAddendumTitleW(StrPtr(ID)), vbFromUnicode)
	End Function
	Public Function ID3V2GetAddendumDescription(ByVal ID As String) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetAddendumDescription = StrConv(ID3V2GetAddendumDescriptionW(StrPtr(ID)), vbFromUnicode)
	End Function
	Public Function ID3V2GetAddendumType(ByVal ID As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetAddendumType = ID3V2GetAddendumTypeW(StrPtr(ID))
	End Function
	Public Function ID3V2GetAllFrameIDs() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetAllFrameIDs = StrConv(ID3V2GetAllFrameIDsW(), vbFromUnicode)
	End Function
	Public Function ID3V2GetChildElements(ByVal ID As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetChildElements = ID3V2GetChildElementsW(StrPtr(ID))
	End Function
	Public Function ID3V2GetChildElementID(ByVal ID As String, ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetChildElementID = StrConv(ID3V2GetChildElementIDW(StrPtr(ID), Index), vbFromUnicode)
	End Function
	Public Function ID3V2GetSubFrames(ByVal ID As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetSubFrames = ID3V2GetSubFramesW(StrPtr(ID))
	End Function
	Public Function ID3V2GetSubFrameID(ByVal ID As String, ByVal Index As Short) As Integer
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetSubFrameID = ID3V2GetSubFrameIDW(StrPtr(ID), Index)
	End Function
	Public Function ID3V2GetSubFrameImageType(ByVal ID As String, ByVal Index As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetSubFrameImageType = ID3V2GetSubFrameImageTypeW(StrPtr(ID), Index)
	End Function
	Public Function ID3V2GetSubFrameImage(ByRef arr() As Byte, ByVal maxLen As Integer, ByRef ID As String, ByRef Index As Short) As Integer
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetSubFrameImage = ID3V2GetSubFrameImageW(VarPtr(arr(0)), maxLen, StrPtr(ID), Index)
	End Function
	Public Function ID3V2GetSubFrameText(ByVal ID As String, ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetSubFrameText = StrConv(ID3V2GetSubFrameTextW(StrPtr(ID), Index), vbFromUnicode)
	End Function
	Public Function ID3V2GetSubFrameDescription(ByVal ID As String, ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetSubFrameDescription = StrConv(ID3V2GetSubFrameDescriptionW(StrPtr(ID), Index), vbFromUnicode)
	End Function
	Public Function ID3V2GetSubFrameType(ByVal ID As String, ByVal Index As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetSubFrameType = ID3V2GetSubFrameTypeW(StrPtr(ID), Index)
	End Function
	Public Function ID3V2GetTOCRootID() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetTOCRootID = StrConv(ID3V2GetTOCRootIDW(), vbFromUnicode)
	End Function
	Public Function ID3V2SetAddendumTitle(ByVal ID As String, ByVal newTitle As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2SetAddendumTitle = ID3V2SetAddendumTitleW(StrPtr(ID), StrPtr(newTitle))
	End Function
	Public Function ID3V2SetAddendumDescription(ByVal ID As String, ByVal newDescription As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2SetAddendumDescription = ID3V2SetAddendumDescriptionW(StrPtr(ID), StrPtr(newDescription))
	End Function
	Public Function ID3V2SetImageSubFrame(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal ParentID As String, ByVal Description As String, ByVal PictureType As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2SetImageSubFrame = ID3V2SetImageSubFrameW(VarPtr(arr(0)), maxLen, StrPtr(ParentID), StrPtr(Description), PictureType)
	End Function
	Public Function ID3V2SetTextSubFrame(ByVal ParentID As String, ByVal frameID As ID3V2FRAMES, ByVal text As String, ByVal Description As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2SetTextSubFrame = ID3V2SetTextSubFrameW(StrPtr(ParentID), frameID, StrPtr(text), StrPtr(Description))
	End Function
	Public Function ID3V2SetURLSubFrame(ByVal ParentID As String, ByVal frameID As ID3V2FRAMES, ByVal text As String, ByVal Description As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2SetURLSubFrame = ID3V2SetURLSubFrameW(StrPtr(ParentID), frameID, StrPtr(text), StrPtr(Description))
	End Function
	Public Function ID3V2AddAudioEncryption(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal URL As String, ByVal PreviewStart As Short, ByVal PreviewLength As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddAudioEncryption = ID3V2AddAudioEncryptionW(VarPtr(arr(0)), maxLen, StrPtr(URL), PreviewStart, PreviewLength)
	End Function
	
	Public Function ID3V2AddSeekOffset(ByVal Counter As Integer) As Short ' SEEK
		ID3V2AddSeekOffset = ID3V2AddSeekOffsetW(Counter)
	End Function
	
	Public Function ID3V2AddAudioSeekPoint(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal start As Integer, ByVal Length As Integer, ByVal numbers As Short, ByVal BitsPerPoint As Short) As Short
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddAudioSeekPoint = ID3V2AddAudioSeekPointW(VarPtr(arr(0)), maxLen, start, Length, numbers, BitsPerPoint)
	End Function
	
	Public Function ID3V2AddComment(ByVal Language As String, ByVal Description As String, ByVal text As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddComment = ID3V2AddCommentW(StrPtr(Language), StrPtr(Description), StrPtr(text))
	End Function
	
	Public Function ID3V2AddCommercialFrame(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Price As String, ByVal validUntil As String, ByVal contactUrl As String, ByVal receivedAs As Short, ByVal Seller As String, ByVal Description As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddCommercialFrame = ID3V2AddCommercialFrameW(VarPtr(arr(0)), maxLen, StrPtr(Price), StrPtr(validUntil), StrPtr(contactUrl), receivedAs, StrPtr(Seller), StrPtr(Description))
	End Function
	
	Public Function ID3V2AddEncryption(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal URL As String, ByVal symbol As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddEncryption = ID3V2AddEncryptionW(VarPtr(arr(0)), maxLen, StrPtr(URL), symbol)
	End Function
	
	Public Function ID3V2AddEqualisation(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Interpolationmethod As Short, ByVal Identification As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddEqualisation = ID3V2AddEqualisationW(VarPtr(arr(0)), maxLen, Interpolationmethod, StrPtr(Identification))
	End Function
	
	Public Function ID3V2AddGeneralObject(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Mime As String, ByVal FileName As String, ByVal Description As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddGeneralObject = ID3V2AddGeneralObjectW(VarPtr(arr(0)), maxLen, StrPtr(Mime), StrPtr(FileName), StrPtr(Description))
	End Function
	
	Public Function ID3V2AddGroupIdentification(ByRef arr() As Byte, ByVal Length As Integer, ByVal URL As String, ByVal symbol As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddGroupIdentification = ID3V2AddGroupIdentificationW(VarPtr(arr(0)), Length, StrPtr(URL), symbol)
	End Function
	
	Public Function ID3V2AddLinkedInformation(ByVal FrameIdentifier As Integer, ByVal URL As String, ByVal additionalData As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddLinkedInformation = ID3V2AddLinkedInformationW(FrameIdentifier, StrPtr(URL), StrPtr(additionalData))
	End Function
	
	Public Function ID3V2AddLyric(ByVal Language As String, ByVal Description As String, ByVal text As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddLyric = ID3V2AddLyricW(StrPtr(Language), StrPtr(Description), StrPtr(text))
	End Function
	
	Public Function ID3V2AddMpegLocationLookupTable(ByRef arr() As Byte, ByVal Length As Integer, ByVal Frames As Integer, ByVal Bytes As Integer, ByVal Milliseconds As Integer, ByVal BytesDeviation As Short, ByVal MillisecondsDeviation As Short) As Short
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddMpegLocationLookupTable = ID3V2AddMpegLocationLookupTableW(VarPtr(arr(0)), Length, Frames, Bytes, Milliseconds, BytesDeviation, MillisecondsDeviation)
	End Function
	
	Public Function ID3V2AddMusicCdIdentifier(ByRef arr() As Byte, ByVal Length As Integer) As Short
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddMusicCdIdentifier = ID3V2AddMusicCdIdentifierW(VarPtr(arr(0)), Length)
	End Function
	
	'UPGRADE_NOTE: dateString wurde aktualisiert auf dateString_Renamed. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
	Public Function ID3V2AddOwnership(ByVal Price As String, ByVal dateString_Renamed As String, ByVal Seller As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddOwnership = ID3V2AddOwnershipW(StrPtr(Price), StrPtr(dateString_Renamed), StrPtr(Seller))
	End Function
	
	Public Function ID3V2AddPictureArray(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Description As String, ByVal PictureType As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddPictureArray = ID3V2AddPictureArrayW(VarPtr(arr(0)), maxLen, StrPtr(Description), PictureType)
	End Function
	
	Public Function ID3V2AddPictureFile(ByVal FileName As String, ByVal Description As String, ByVal PictureType As Short, ByVal asLink As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddPictureFile = ID3V2AddPictureFileW(StrPtr(FileName), StrPtr(Description), PictureType, asLink)
	End Function
	
	Public Function ID3V2AddPopularimeter(ByVal Email As String, ByVal Rating As Short, ByVal Counter As Integer) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddPopularimeter = ID3V2AddPopularimeterW(StrPtr(Email), Rating, Counter)
	End Function
	
	Public Function ID3V2AddPositionSynchronisation(ByVal TimestampFormat As Short, ByVal Position As Integer) As Short
		ID3V2AddPositionSynchronisation = ID3V2AddPositionSynchronisationW(TimestampFormat, Position)
	End Function
	
	Public Function ID3V2AddPrivateFrame(ByRef arr() As Byte, ByVal Length As Integer, ByVal URL As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddPrivateFrame = ID3V2AddPrivateFrameW(VarPtr(arr(0)), Length, StrPtr(URL))
	End Function
	
	Public Function ID3V2AddRecommendedBufferSize(ByVal BufferSize As Integer, ByVal EmbeddedInfoFlag As Short, ByVal offset As Integer) As Short
		ID3V2AddRecommendedBufferSize = ID3V2AddRecommendedBufferSizeW(BufferSize, EmbeddedInfoFlag, offset)
	End Function
	
	Public Function ID3V2AddRelativeVolumeAdjustment(ByRef arr() As Byte, ByVal Length As Integer, ByVal Identifier As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddRelativeVolumeAdjustment = ID3V2AddRelativeVolumeAdjustmentW(VarPtr(arr(0)), Length, StrPtr(Identifier))
	End Function
	
	Public Function ID3V2AddReverb(ByVal reverbLeft As Short, ByVal reverbRight As Short, ByVal bouncesLeft As Short, ByVal bouncesRight As Short, ByVal feedbackLeftToLeft As Short, ByVal feedbackLeftToRight As Short, ByVal feedbackRightToRight As Short, ByVal feedbackRightToLeft As Short, ByVal premixLeftToRight As Short, ByVal premixRightToLeft As Short) As Short
		ID3V2AddReverb = ID3V2AddReverbW(reverbLeft, reverbRight, bouncesLeft, bouncesRight, feedbackLeftToLeft, feedbackLeftToRight, feedbackRightToRight, feedbackRightToLeft, premixLeftToRight, premixRightToLeft)
	End Function
	
	Public Function ID3V2AddSignatureFrame(ByRef arr() As Byte, ByVal Length As Integer, ByVal GroupSymbol As Short) As Short
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddSignatureFrame = ID3V2AddSignatureFrameW(VarPtr(arr(0)), Length, GroupSymbol)
	End Function
	
	Public Function ID3V2AddSyncLyric(ByVal Language As String, ByVal Description As String, ByVal text As String, ByVal ContentType As Short, ByVal TimestampFormat As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddSyncLyric = ID3V2AddSyncLyricW(StrPtr(Language), StrPtr(Description), StrPtr(text), ContentType, TimestampFormat)
	End Function
	
	'UPGRADE_NOTE: format wurde aktualisiert auf format_Renamed. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
	Public Function ID3V2AddSynchronizedTempo(ByRef arr() As Byte, ByVal Length As Integer, ByVal format_Renamed As Short) As Short
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddSynchronizedTempo = ID3V2AddSynchronizedTempoW(VarPtr(arr(0)), Length, format_Renamed)
	End Function
	
	Public Function ID3V2AddUniqueFileIdentifier(ByRef arr() As Byte, ByVal Length As Integer, ByVal Owner As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddUniqueFileIdentifier = ID3V2AddUniqueFileIdentifierW(VarPtr(arr(0)), Length, StrPtr(Owner))
	End Function
	
	Public Function ID3V2AddUserFrame(ByVal Language As String, ByVal text As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddUserFrame = ID3V2AddUserFrameW(StrPtr(Language), StrPtr(text))
	End Function
	
	Public Function ID3V2AddUserText(ByVal Description As String, ByVal text As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddUserText = ID3V2AddUserTextW(StrPtr(Description), StrPtr(text))
	End Function
	
	Public Function ID3V2AddUserURL(ByVal Description As String, ByVal URL As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddUserURL = ID3V2AddUserURLW(StrPtr(Description), StrPtr(URL))
	End Function
	
	Public Function ID3V2DeleteAllFrames(ByVal ID As Integer) As Short
		ID3V2DeleteAllFrames = ID3V2DeleteAllFramesW(ID)
	End Function
	
	Public Sub ID3V2DeleteComments()
		Call ID3V2DeleteAllFramesW(ID3V2FRAMES.ID3F_COMM)
	End Sub
	
	Public Sub ID3V2DeletePopularimeters()
		Call ID3V2DeleteAllFramesW(ID3V2FRAMES.ID3F_POPM)
	End Sub
	
	Public Sub ID3V2DeleteLyrics()
		Call ID3V2DeleteAllFramesW(ID3V2FRAMES.ID3F_USLT)
	End Sub
	
	Public Function ID3V2DeletePicture(ByRef Index As Short) As Short
		ID3V2DeletePicture = ID3V2DeleteSelectedFrameW(ID3V2FRAMES.ID3F_APIC, Index)
	End Function
	
	Public Sub ID3V2DeletePictures() ' APIC
		Call ID3V2DeleteAllFramesW(ID3V2FRAMES.ID3F_APIC)
	End Sub
	
	Public Function ID3V2DeleteSelectedFrame(ByVal ID As Integer, ByVal Index As Short) As Short
		ID3V2DeleteSelectedFrame = ID3V2DeleteSelectedFrameW(ID, Index)
	End Function
	
	Public Sub ID3V2DeleteSyncLyrics() ' SYLT
		Call ID3V2DeleteAllFramesW(ID3V2FRAMES.ID3F_SYLT)
	End Sub
	
	Public Sub ID3V2DeleteUserFrames() ' USER
		Call ID3V2DeleteAllFramesW(ID3V2FRAMES.ID3F_USER)
	End Sub
	
	Public Sub ID3V2DeleteUserTexts()
		Call ID3V2DeleteAllFramesW(ID3V2FRAMES.ID3F_TXXX)
	End Sub
	
	Public Sub ID3V2DeleteUserURLs()
		Call ID3V2DeleteAllFramesW(ID3V2FRAMES.ID3F_WXXX)
	End Sub
	
	Public Function ID3V2Exists() As Short
		ID3V2Exists = ID3V2ExistsW()
	End Function
	
	Public Function ID3V2GetAudioEncryptionData(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Index As Short) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetAudioEncryptionData = ID3V2GetAudioEncryptionDataW(VarPtr(arr(0)), maxLen, Index)
	End Function
	
	Public Function ID3V2GetAudioEncryptionPreviewLength(ByVal Index As Short) As Short
		ID3V2GetAudioEncryptionPreviewLength = ID3V2GetAudioEncryptionPreviewLengthW(Index)
	End Function
	
	Public Function ID3V2GetAudioEncryptionPreviewStart(ByVal Index As Short) As Short
		ID3V2GetAudioEncryptionPreviewStart = ID3V2GetAudioEncryptionPreviewStartW(Index)
	End Function
	
	Public Function ID3V2GetAudioEncryptionURL(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetAudioEncryptionURL = StrConv(ID3V2GetAudioEncryptionURLW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetAudioSeekPointBitsPerIndexpoint() As Integer
		ID3V2GetAudioSeekPointBitsPerIndexpoint = ID3V2GetAudioSeekPointBitsPerIndexpointW()
	End Function
	
	Public Function ID3V2GetAudioSeekPointData(ByRef arr() As Byte, ByVal maxLen As Integer) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetAudioSeekPointData = ID3V2GetAudioSeekPointDataW(VarPtr(arr(0)), maxLen)
	End Function
	
	Public Function ID3V2GetAudioSeekPointLength() As Integer
		ID3V2GetAudioSeekPointLength = ID3V2GetAudioSeekPointLengthW()
	End Function
	
	Public Function ID3V2GetAudioSeekPointNumber() As Integer
		ID3V2GetAudioSeekPointNumber = ID3V2GetAudioSeekPointNumberW()
	End Function
	
	Public Function ID3V2GetAudioSeekPointStart() As Integer
		ID3V2GetAudioSeekPointStart = ID3V2GetAudioSeekPointStartW()
	End Function
	
	Public Function ID3V2GetChapterStartTime(ByVal Index As String) As Integer
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetChapterStartTime = ID3V2GetChapterStartTimeW(StrPtr(Index))
	End Function
	
	Public Function ID3V2GetChapterEndTime(ByVal Index As String) As Integer
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetChapterEndTime = ID3V2GetChapterEndTimeW(StrPtr(Index))
	End Function
	
	Public Function ID3V2GetComments() As Short
		ID3V2GetComments = ID3V2GetFrameCountW(ID3V2FRAMES.ID3F_COMM)
	End Function
	
	Public Function ID3V2GetCommentDescription(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetCommentDescription = StrConv(ID3V2GetCommentDescriptionW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetCommentLanguage(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetCommentLanguage = StrConv(ID3V2GetCommentLanguageW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetComment(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetComment = StrConv(ID3V2GetCommentW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetCommercialFrameContactURL(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetCommercialFrameContactURL = StrConv(ID3V2GetCommercialFrameContactURLW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetCommercialFrameDescription(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetCommercialFrameDescription = StrConv(ID3V2GetCommercialFrameDescriptionW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetCommercialFramePictureMime(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetCommercialFramePictureMime = StrConv(ID3V2GetCommercialFramePictureMimeW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetCommercialFramePicture(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Index As Short) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetCommercialFramePicture = ID3V2GetCommercialFramePictureW(VarPtr(arr(0)), maxLen, Index)
	End Function
	
	Public Function ID3V2GetCommercialFramePrice(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetCommercialFramePrice = StrConv(ID3V2GetCommercialFramePriceW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetCommercialFrameReceivedAs(ByVal Index As Short) As Short
		ID3V2GetCommercialFrameReceivedAs = ID3V2GetCommercialFrameReceivedAsW(Index)
	End Function
	
	Public Function ID3V2GetCommercialFrameSellerName(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetCommercialFrameSellerName = StrConv(ID3V2GetCommercialFrameSellerNameW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetCommercialFrameValidUntil(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetCommercialFrameValidUntil = StrConv(ID3V2GetCommercialFrameValidUntilW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetEncryptionData(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Index As Short) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetEncryptionData = ID3V2GetEncryptionDataW(VarPtr(arr(0)), maxLen, Index)
	End Function
	
	Public Function ID3V2GetEncryptionSymbol(ByVal Index As Short) As Short
		ID3V2GetEncryptionSymbol = ID3V2GetEncryptionSymbolW(Index)
	End Function
	
	Public Function ID3V2GetEncryptionURL(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetEncryptionURL = StrConv(ID3V2GetEncryptionURLW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetEqualisationAdjustmentBits(ByVal Index As Short) As Short
		ID3V2GetEqualisationAdjustmentBits = ID3V2GetEqualisationAdjustmentBitsW(Index)
	End Function
	
	Public Function ID3V2GetEqualisationData(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Index As Short) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetEqualisationData = ID3V2GetEqualisationDataW(VarPtr(arr(0)), maxLen, Index)
	End Function
	
	Public Function ID3V2GetEqualisationIdentification(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetEqualisationIdentification = StrConv(ID3V2GetEqualisationIdentificationW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetEqualisationInterpolation(ByVal Index As Short) As Short
		ID3V2GetEqualisationInterpolation = ID3V2GetEqualisationInterpolationW(Index)
	End Function
	
	Public Function ID3V2GetEventTimingCodesData(ByRef arr() As Byte, ByVal maxLen As Integer) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetEventTimingCodesData = ID3V2GetEventTimingCodesDataW(VarPtr(arr(0)), maxLen)
	End Function
	
	Public Function ID3V2GetEventTimingCodesTimestampFormat() As Short
		ID3V2GetEventTimingCodesTimestampFormat = ID3V2GetEventTimingCodesTimestampFormatW()
	End Function
	
	Public Function ID3V2GetFrameCount(ByVal ID As Integer) As Short
		ID3V2GetFrameCount = ID3V2GetFrameCountW(ID)
	End Function
	
	Public Function ID3V2GetGeneralObjectData(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Index As Short) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetGeneralObjectData = ID3V2GetGeneralObjectDataW(VarPtr(arr(0)), maxLen, Index)
	End Function
	
	Public Function ID3V2GetGeneralObjectDescription(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetGeneralObjectDescription = StrConv(ID3V2GetGeneralObjectDescriptionW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetGeneralObjectFilename(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetGeneralObjectFilename = StrConv(ID3V2GetGeneralObjectFilenameW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetGeneralObjectMime(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetGeneralObjectMime = StrConv(ID3V2GetGeneralObjectMimeW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetGroupIdentificationData(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Index As Short) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetGroupIdentificationData = ID3V2GetGroupIdentificationDataW(VarPtr(arr(0)), maxLen, Index)
	End Function
	
	Public Function ID3V2GetGroupIdentificationSymbol(ByVal Index As Short) As Short
		ID3V2GetGroupIdentificationSymbol = ID3V2GetGroupIdentificationSymbolW(Index)
	End Function
	
	Public Function ID3V2GetGroupIdentificationURL(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetGroupIdentificationURL = StrConv(ID3V2GetGroupIdentificationURLW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetLinkedInformationAdditionalData(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetLinkedInformationAdditionalData = StrConv(ID3V2GetLinkedInformationAdditionalDataW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetLinkedInformationFrameIdentifier(ByVal Index As Short) As Integer
		ID3V2GetLinkedInformationFrameIdentifier = ID3V2GetLinkedInformationFrameIdentifierW(Index)
	End Function
	
	Public Function ID3V2GetLinkedInformationURL(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetLinkedInformationURL = StrConv(ID3V2GetLinkedInformationURLW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetLyrics() As Short
		ID3V2GetLyrics = ID3V2GetFrameCountW(ID3V2FRAMES.ID3F_USLT)
	End Function
	
	Public Function ID3V2GetLyricDescription(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetLyricDescription = StrConv(ID3V2GetLyricDescriptionW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetLyricLanguage(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetLyricLanguage = StrConv(ID3V2GetLyricLanguageW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetLyric(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetLyric = StrConv(ID3V2GetLyricW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetMpegLocationLookupTableBytesDeviation() As Integer
		ID3V2GetMpegLocationLookupTableBytesDeviation = ID3V2GetMpegLocationLookupTableBytesDeviationW()
	End Function
	
	Public Function ID3V2GetMpegLocationLookupTableBytes() As Integer
		ID3V2GetMpegLocationLookupTableBytes = ID3V2GetMpegLocationLookupTableBytesW()
	End Function
	
	Public Function ID3V2GetMpegLocationLookupTableData(ByRef arr() As Byte, ByVal maxLen As Integer) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetMpegLocationLookupTableData = ID3V2GetMpegLocationLookupTableDataW(VarPtr(arr(0)), maxLen)
	End Function
	
	Public Function ID3V2GetMpegLocationLookupTableFrames() As Integer
		ID3V2GetMpegLocationLookupTableFrames = ID3V2GetMpegLocationLookupTableFramesW()
	End Function
	
	Public Function ID3V2GetMpegLocationLookupTableMillisecondsDeviation() As Integer
		ID3V2GetMpegLocationLookupTableMillisecondsDeviation = ID3V2GetMpegLocationLookupTableMillisecondsDeviationW()
	End Function
	
	Public Function ID3V2GetMpegLocationLookupTableMilliseconds() As Integer
		ID3V2GetMpegLocationLookupTableMilliseconds = ID3V2GetMpegLocationLookupTableMillisecondsW()
	End Function
	
	Public Function ID3V2GetMusicCdIdentifierData(ByRef arr() As Byte, ByVal maxLen As Integer) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetMusicCdIdentifierData = ID3V2GetMusicCdIdentifierDataW(VarPtr(arr(0)), maxLen)
	End Function
	
	Public Function ID3V2GetOwnershipDate() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetOwnershipDate = StrConv(ID3V2GetOwnershipDateW(), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetOwnershipPrice() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetOwnershipPrice = StrConv(ID3V2GetOwnershipPriceW(), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetOwnershipSeller() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetOwnershipSeller = StrConv(ID3V2GetOwnershipSellerW(), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetPictureCount() As Short
		ID3V2GetPictureCount = ID3V2GetFrameCountW(ID3V2FRAMES.ID3F_APIC)
	End Function
	
	Public Function ID3V2GetPictureArray(ByVal Index As Short) As Byte()
		Dim maxLen, ln As Integer
		maxLen = 5000000
		Dim dummy() As Byte
		ReDim dummy(maxLen)
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ln = ID3V2GetPictureArrayW(VarPtr(dummy(0)), maxLen, Index)
		If ln = 0 Then
			Debug.Print("Fehler beim Laden des Bildes:" & AUDIOGetLastErrorText())
		End If
		ReDim Preserve dummy(ln)
		ID3V2GetPictureArray = VB6.CopyArray(dummy)
	End Function
	
	Public Function ID3V2GetPictureDescription(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetPictureDescription = StrConv(ID3V2GetPictureDescriptionW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetPictureFile(ByVal FileName As String, ByVal Index As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetPictureFile = ID3V2GetPictureFileW(StrPtr(FileName), Index)
	End Function
	
	Public Function ID3V2GetPictureMime(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetPictureMime = StrConv(ID3V2GetPictureMimeW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetPictureType(ByVal Index As Short) As Short
		ID3V2GetPictureType = ID3V2GetPictureTypeW(Index)
	End Function
	
	Public Function ID3V2GetPictureTypeText(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetPictureTypeText = StrConv(ID3V2GetPictureTypeTextW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetPopularimeterCounter(ByVal Index As Short) As Integer
		ID3V2GetPopularimeterCounter = ID3V2GetPopularimeterCounterW(Index)
	End Function
	
	Public Function ID3V2GetPopularimeterEmail(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetPopularimeterEmail = StrConv(ID3V2GetPopularimeterEmailW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetPopularimeterRating(ByVal Index As Short) As Short
		ID3V2GetPopularimeterRating = ID3V2GetPopularimeterRatingW(Index)
	End Function
	
	Public Function ID3V2GetPositionSynchronisationTimestampFormat() As Short
		ID3V2GetPositionSynchronisationTimestampFormat = ID3V2GetPositionSynchronisationTimestampFormatW()
	End Function
	
	Public Function ID3V2GetPositionSynchronisationValue() As Integer
		ID3V2GetPositionSynchronisationValue = ID3V2GetPositionSynchronisationValueW()
	End Function
	
	Public Function ID3V2GetPossibleCHAPID() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetPossibleCHAPID = StrConv(ID3V2GetPossibleCHAPIDW(), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetPossibleTOCID() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetPossibleTOCID = StrConv(ID3V2GetPossibleTOCIDW(), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetPrivateFrameData(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Index As Short) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetPrivateFrameData = ID3V2GetPrivateFrameDataW(VarPtr(arr(0)), maxLen, Index)
	End Function
	
	Public Function ID3V2GetPrivateFrameURL(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetPrivateFrameURL = StrConv(ID3V2GetPrivateFrameURLW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetRelativeVolumeAdjustmentData(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Index As Short) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetRelativeVolumeAdjustmentData = ID3V2GetRelativeVolumeAdjustmentDataW(VarPtr(arr(0)), maxLen, Index)
	End Function
	
	Public Function ID3V2GetRelativeVolumeAdjustmentIdentifier(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetRelativeVolumeAdjustmentIdentifier = StrConv(ID3V2GetRelativeVolumeAdjustmentIdentifierW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetReverbBouncesLeft() As Short
		ID3V2GetReverbBouncesLeft = ID3V2GetReverbBouncesLeftW()
	End Function
	
	Public Function ID3V2GetReverbBouncesRight() As Short
		ID3V2GetReverbBouncesRight = ID3V2GetReverbBouncesRightW()
	End Function
	
	Public Function ID3V2GetReverbFeedbackLeftToLeft() As Short
		ID3V2GetReverbFeedbackLeftToLeft = ID3V2GetReverbFeedbackLeftToLeftW()
	End Function
	
	Public Function ID3V2GetReverbFeedbackLeftToRight() As Short
		ID3V2GetReverbFeedbackLeftToRight = ID3V2GetReverbFeedbackLeftToRightW()
	End Function
	
	Public Function ID3V2GetReverbFeedbackRightToLeft() As Short
		ID3V2GetReverbFeedbackRightToLeft = ID3V2GetReverbFeedbackRightToLeftW()
	End Function
	
	Public Function ID3V2GetReverbFeedbackRightToRight() As Short
		ID3V2GetReverbFeedbackRightToRight = ID3V2GetReverbFeedbackRightToRightW()
	End Function
	
	Public Function ID3V2GetReverbLeft() As Short
		ID3V2GetReverbLeft = ID3V2GetReverbLeftW()
	End Function
	
	Public Function ID3V2GetReverbPremixLeftToRight() As Short
		ID3V2GetReverbPremixLeftToRight = ID3V2GetReverbPremixLeftToRightW()
	End Function
	
	Public Function ID3V2GetReverbPremixRightToLeft() As Short
		ID3V2GetReverbPremixRightToLeft = ID3V2GetReverbPremixRightToLeftW()
	End Function
	
	Public Function ID3V2GetReverbRight() As Short
		ID3V2GetReverbRight = ID3V2GetReverbRightW()
	End Function
	
	Public Function ID3V2GetSeekOffset() As Integer ' SEEK
		ID3V2GetSeekOffset = ID3V2GetSeekOffsetW()
	End Function
	
	Public Function ID3V2GetSignatureFrameData(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Index As Short) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetSignatureFrameData = ID3V2GetSignatureFrameDataW(VarPtr(arr(0)), maxLen, Index)
	End Function
	
	Public Function ID3V2GetSignatureFrameGroupSymbol(ByVal Index As Short) As Short
		ID3V2GetSignatureFrameGroupSymbol = ID3V2GetSignatureFrameGroupSymbolW(Index)
	End Function
	
	Public Function ID3V2GetSize() As Integer
		ID3V2GetSize = ID3V2GetSizeW()
	End Function
	
	Public Function ID3V2GetSyncLyrics() As Short
		ID3V2GetSyncLyrics = ID3V2GetFrameCountW(ID3V2FRAMES.ID3F_SYLT)
	End Function
	
	Public Function ID3V2GetSyncLyricContentType(ByVal Index As Short) As Short
		ID3V2GetSyncLyricContentType = ID3V2GetSyncLyricContentTypeW(Index)
	End Function
	
	Public Function ID3V2GetSyncLyricDescription(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetSyncLyricDescription = StrConv(ID3V2GetSyncLyricDescriptionW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetSyncLyricLanguage(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetSyncLyricLanguage = StrConv(ID3V2GetSyncLyricLanguageW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetSyncLyricTimeFormat(ByVal Index As Short) As Short
		ID3V2GetSyncLyricTimeFormat = ID3V2GetSyncLyricTimeFormatW(Index)
	End Function
	
	Public Function ID3V2GetSyncLyric(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetSyncLyric = StrConv(ID3V2GetSyncLyricW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetSynchronizedTempoData(ByRef arr() As Byte, ByVal maxLen As Integer) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetSynchronizedTempoData = ID3V2GetSynchronizedTempoDataW(VarPtr(arr(0)), maxLen)
	End Function
	
	Public Function ID3V2GetSynchronizedTempoFormat() As Short
		ID3V2GetSynchronizedTempoFormat = ID3V2GetSynchronizedTempoFormatW()
	End Function
	Public Function ID3V2GetTOCIsOrdered(ByVal ID As String) As Boolean
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetTOCIsOrdered = ID3V2GetTOCIsOrderedW(StrPtr(ID))
	End Function
	Public Function ID3V2GetUniqueFileIdentifierData(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Index As Short) As Integer
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2GetUniqueFileIdentifierData = ID3V2GetUniqueFileIdentifierDataW(VarPtr(arr(0)), maxLen, Index)
	End Function
	
	Public Function ID3V2GetUniqueFileIdentifierOwner(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetUniqueFileIdentifierOwner = StrConv(ID3V2GetUniqueFileIdentifierOwnerW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetUserFrames() As Short
		ID3V2GetUserFrames = ID3V2GetFrameCountW(ID3V2FRAMES.ID3F_USER)
	End Function
	
	Public Function ID3V2GetUserFrameLanguage(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetUserFrameLanguage = StrConv(ID3V2GetUserFrameLanguageW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetUserFrame(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetUserFrame = StrConv(ID3V2GetUserFrameW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetUserTexts() As Short
		ID3V2GetUserTexts = ID3V2GetFrameCountW(ID3V2FRAMES.ID3F_TXXX)
	End Function
	
	Public Function ID3V2GetUserTextDescription(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetUserTextDescription = StrConv(ID3V2GetUserTextDescriptionW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetUserText(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetUserText = StrConv(ID3V2GetUserTextW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetUserURLDescription(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetUserURLDescription = StrConv(ID3V2GetUserURLDescriptionW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetUserURLs() As Short
		ID3V2GetUserURLs = ID3V2GetFrameCountW(ID3V2FRAMES.ID3F_WXXX)
	End Function
	
	Public Function ID3V2GetUserURL(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetUserURL = StrConv(ID3V2GetUserURLW(Index), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetVersion() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		ID3V2GetVersion = StrConv(ID3V2GetVersionW(), vbFromUnicode)
	End Function
	
	Public Function ID3V2GetRecommendedBufferSizeFlag() As Short
		ID3V2GetRecommendedBufferSizeFlag = ID3V2GetRecommendedBufferSizeFlagW()
	End Function
	
	Public Function ID3V2GetRecommendedBufferSizeOffset() As Integer
		ID3V2GetRecommendedBufferSizeOffset = ID3V2GetRecommendedBufferSizeOffsetW()
	End Function
	
	Public Function ID3V2GetRecommendedBufferSizeValue() As Integer
		ID3V2GetRecommendedBufferSizeValue = ID3V2GetRecommendedBufferSizeValueW()
	End Function
	
	Public Function ID3V2ImportCueFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2ImportCueFile = ID3V2ImportCueFileW(StrPtr(FileName))
	End Function
	
	Public Function ID3V2RemoveTagFromFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2RemoveTagFromFile = ID3V2RemoveTagFromFileW(StrPtr(FileName))
	End Function
	
	Public Function ID3V2RemoveTag() As Short
		ID3V2RemoveTag = ID3V2RemoveTagW()
	End Function
	
	Public Function ID3V2AddEventTimingCodes(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal TimestampFormat As Short) As Short
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2AddEventTimingCodes = ID3V2AddEventTimingCodesW(VarPtr(arr(0)), maxLen, TimestampFormat)
	End Function
	
	Public Function ID3V2SaveChangesToFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2SaveChangesToFile = ID3V2SaveChangesToFileW(StrPtr(FileName))
	End Function
	
	Public Function ID3V2SaveChanges() As Short
		ID3V2SaveChanges = ID3V2SaveChangesW()
	End Function
	
	Public Function ID3V2SetChapterTimes(ByVal ID As String, ByVal startTime As Integer, ByRef endTime As Integer) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2SetChapterTimes = ID3V2SetChapterTimesW(StrPtr(ID), startTime, endTime)
	End Function
	
	'UPGRADE_NOTE: format wurde aktualisiert auf format_Renamed. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
	Public Function ID3V2SetFormatAndEncoding(ByVal format_Renamed As Short, ByVal encoding As Short) As Short
		ID3V2SetFormatAndEncoding = ID3V2SetFormatAndEncodingW(format_Renamed, encoding)
	End Function
	
	Public Function ID3V2SetTOCIsOrdered(ByVal ID As String, ByVal status As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ID3V2SetTOCIsOrdered = ID3V2SetTOCIsOrderedW(StrPtr(ID), status)
	End Function
	
	Public Function LYRICSExists() As Short
		LYRICSExists = LYRICSExistsW()
	End Function
	
	Public Function LYRICSGetSize() As Integer
		LYRICSGetSize = LYRICSGetSizeW()
	End Function
	
	Public Function LYRICSGetStartPosition() As Integer
		LYRICSGetStartPosition = LYRICSGetStartPositionW()
	End Function
	
	Public Function LYRICSGetVersion() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		LYRICSGetVersion = StrConv(LYRICSGetVersionW(), vbFromUnicode)
	End Function
	
	Public Function LYRICSRemoveTagFromFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		LYRICSRemoveTagFromFile = LYRICSRemoveTagFromFileW(StrPtr(FileName))
	End Function
	
	Public Function LYRICSRemoveTag() As Short
		LYRICSRemoveTag = LYRICSRemoveTagW()
	End Function
	
	Public Function LYRICSSaveChangesToFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		LYRICSSaveChangesToFile = LYRICSSaveChangesToFileW(StrPtr(FileName))
	End Function
	
	Public Function LYRICSSaveChanges() As Short
		LYRICSSaveChanges = LYRICSSaveChangesW()
	End Function
	
	Public Function MONKEYGetBits() As Short
		MONKEYGetBits = MONKEYGetBitsW()
	End Function
	
	Public Function MONKEYGetCompressionRatio() As Single
		MONKEYGetCompressionRatio = MONKEYGetCompressionRatioW()
	End Function
	
	Public Function MONKEYGetCompression() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		MONKEYGetCompression = StrConv(MONKEYGetCompressionW(), vbFromUnicode)
	End Function
	
	Public Function MONKEYGetFrames() As Integer
		MONKEYGetFrames = MONKEYGetFramesW()
	End Function
	
	Public Function MONKEYGetPeak() As Single
		MONKEYGetPeak = MONKEYGetPeakW()
	End Function
	
	Public Function MONKEYGetSamplesPerFrame() As Integer
		MONKEYGetSamplesPerFrame = MONKEYGetSamplesPerFrameW()
	End Function
	
	Public Function MONKEYGetSamples() As Integer
		MONKEYGetSamples = MONKEYGetSamplesW()
	End Function
	
	Public Sub MP4DeletePictures()
		Call MP4DeletePicturesW()
	End Sub
	
	Public Function MP4DeletePicture(ByVal Index As Short) As Short
		MP4DeletePicture = MP4DeletePictureW(Index)
	End Function
	
	Public Function MP4GetPictureArray(ByVal Index As Short) As Byte()
		Dim maxLen, ln As Integer
		maxLen = 5000000
		Dim dummy() As Byte
		ReDim dummy(maxLen)
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ln = MP4GetPictureArrayW(VarPtr(dummy(0)), maxLen, Index)
		If ln = 0 Then
			Debug.Print("Fehler beim Laden des Bildes:" & AUDIOGetLastErrorText())
		End If
		ReDim Preserve dummy(ln)
		MP4GetPictureArray = VB6.CopyArray(dummy)
	End Function
	
	Public Function MP4GetAllFrameIDs() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		MP4GetAllFrameIDs = StrConv(MP4GetAllFrameIDsW(), vbFromUnicode)
	End Function
	
	Public Function MP4GetPictureCount() As Integer
		MP4GetPictureCount = MP4GetPictureCountW()
	End Function
	
	Public Function MP4GetPictureFile(ByVal FileName As String, ByVal Index As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		MP4GetPictureFile = MP4GetPictureFileW(StrPtr(FileName), Index)
	End Function
	
	Public Function MP4AddPictureArray(ByRef arr() As Byte, ByVal maxLen As Integer) As Short
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		MP4AddPictureArray = MP4AddPictureArrayW(VarPtr(arr(0)), maxLen)
	End Function
	
	Public Function MP4AddPictureFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		MP4AddPictureFile = MP4AddPictureFileW(StrPtr(FileName))
	End Function
	
	Public Sub MP4DeleteEntries()
		MP4DeleteEntriesW()
	End Sub
	
	Public Function MP4GetPictureMime(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		MP4GetPictureMime = StrConv(MP4GetPictureMimeW(Index), vbFromUnicode)
	End Function
	
	Public Function MP4GetVersion() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		MP4GetVersion = StrConv(MP4GetVersionW(), vbFromUnicode)
	End Function
	
	Public Function MP4SaveChangesToFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		MP4SaveChangesToFile = MP4SaveChangesToFileW(StrPtr(FileName))
	End Function
	
	Public Function MP4SaveChanges() As Short
		MP4SaveChanges = MP4SaveChangesW()
	End Function
	
	Public Sub MP4SetTextFrame(ByVal frameID As Integer, ByVal textString As String)
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		Call MP4SetTextFrameW(frameID, StrPtr(textString))
	End Sub
	
	Public Function MP4GetTextFrame(ByVal frameID As Integer) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		MP4GetTextFrame = StrConv(MP4GetTextFrameW(frameID), vbFromUnicode)
	End Function
	
	Public Sub MP4SetiTuneFrame(ByVal frameID As String, ByVal textString As String)
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		Call MP4SetiTuneFrameW(StrPtr(frameID), StrPtr(textString))
	End Sub
	
	Public Function MP4GetiTuneFrame(ByVal frameID As String) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		MP4GetiTuneFrame = StrConv(MP4GetiTuneFrameW(StrPtr(frameID)), vbFromUnicode)
	End Function
	
	Public Function MPEGGetEmphasis() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		MPEGGetEmphasis = StrConv(MPEGGetEmphasisW(), vbFromUnicode)
	End Function
	
	Public Function MPEGGetEncoder() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		MPEGGetEncoder = StrConv(MPEGGetEncoderW(), vbFromUnicode)
	End Function
	
	Public Function MPEGGetFramePosition() As Integer
		MPEGGetFramePosition = MPEGGetFramePositionW()
	End Function
	
	Public Function MPEGGetFrameSize() As Integer
		MPEGGetFrameSize = MPEGGetFrameSizeW()
	End Function
	
	Public Function MPEGGetFrames() As Integer
		MPEGGetFrames = MPEGGetFramesW()
	End Function
	
	Public Function MPEGGetLayer() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		MPEGGetLayer = StrConv(MPEGGetLayerW(), vbFromUnicode)
	End Function
	
	Public Function MPEGGetVersion() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		MPEGGetVersion = StrConv(MPEGGetVersionW(), vbFromUnicode)
	End Function
	
	Public Function MPEGIsCopyrighted() As Short
		MPEGIsCopyrighted = MPEGIsCopyrightedW()
	End Function
	
	Public Function MPEGIsOriginal() As Short
		MPEGIsOriginal = MPEGIsOriginalW()
	End Function
	
	Public Function MPEGIsPadding() As Short
		MPEGIsPadding = MPEGIsPaddingW()
	End Function
	
	Public Function MPEGIsPrivate() As Short
		MPEGIsPrivate = MPEGIsPrivateW()
	End Function
	
	Public Function MPEGIsProtected() As Short
		MPEGIsProtected = MPEGIsProtectedW()
	End Function
	
	Public Function MPEGIsVBR() As Short
		MPEGIsVBR = MPEGIsVBRW()
	End Function
	
	Public Function MPEGSetCopyrighted(ByVal FileName As String, ByVal newValue As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		MPEGSetCopyrighted = MPEGSetCopyrightedW(StrPtr(FileName), newValue)
	End Function
	
	Public Function MPEGSetOriginal(ByVal FileName As String, ByVal newValue As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		MPEGSetOriginal = MPEGSetOriginalW(StrPtr(FileName), newValue)
	End Function
	
	Public Function MPEGSetPrivate(ByVal FileName As String, ByVal newValue As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		MPEGSetPrivate = MPEGSetPrivateW(StrPtr(FileName), newValue)
	End Function
	
	Public Function MPPGetFrames() As Integer
		MPPGetFrames = MPPGetFramesW()
	End Function
	
	Public Function MPPGetStreamVersion() As Short
		MPPGetStreamVersion = MPPGetStreamVersionW()
	End Function
	
	Public Function OGGGetBitRateNominal() As Integer
		OGGGetBitRateNominal = OGGGetBitRateNominalW()
	End Function
	
	Public Function OGGGetItemKeys() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		OGGGetItemKeys = StrConv(OGGGetItemKeysW(), vbFromUnicode)
	End Function
	
	Public Function OGGRemoveTagFromFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		OGGRemoveTagFromFile = OGGRemoveTagFromFileW(StrPtr(FileName))
	End Function
	
	Public Function OGGRemoveTag() As Short
		OGGRemoveTag = OGGRemoveTagW()
	End Function
	
	Public Function OGGSaveChangesToFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		OGGSaveChangesToFile = OGGSaveChangesToFileW(StrPtr(FileName))
	End Function
	
	Public Function OGGSaveChanges() As Short
		OGGSaveChanges = OGGSaveChangesW()
	End Function
	
	Public Sub SetLogFile(ByVal FileName As String)
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		Call SetLogFileW(StrPtr(FileName))
	End Sub
	
	Public Function WAVGetBitsPerSample() As Integer
		WAVGetBitsPerSample = WAVGetBitsPerSampleW()
	End Function
	
	Public Function WAVGetBlockAlign() As Integer
		WAVGetBlockAlign = WAVGetBlockAlignW()
	End Function
	
	Public Function WAVGetBytesPerSecond() As Integer
		WAVGetBytesPerSecond = WAVGetBytesPerSecondW()
	End Function
	
	Public Function WAVGetChannels() As Integer
		WAVGetChannels = WAVGetChannelsW()
	End Function
	
	Public Function WAVGetFormatID() As Short
		WAVGetFormatID = WAVGetFormatIDW()
	End Function
	
	Public Function WAVGetFormat() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		WAVGetFormat = StrConv(WAVGetFormatW(), vbFromUnicode)
	End Function
	
	Public Function WAVGetHeaderSize() As Integer
		WAVGetHeaderSize = WAVGetHeaderSizeW()
	End Function
	
	Public Function WAVSaveChanges() As Short
		WAVSaveChanges = WAVSaveChangesW()
	End Function
	
	Public Function WMADeletePicture(ByVal Index As Short) As Short
		WMADeletePicture = WMADeletePictureW(Index)
	End Function
	
	Public Sub WMADeletePictures()
		Call WMADeletePicturesW()
	End Sub
	
	Public Function WMAGetItemKeys() As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		WMAGetItemKeys = StrConv(WMAGetItemKeysW(), vbFromUnicode)
	End Function
	
	Public Function WMAGetPictureArray(ByVal Index As Short) As Byte()
		Dim maxLen, ln As Integer
		maxLen = 5000000
		Dim dummy() As Byte
		ReDim dummy(maxLen)
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		ln = WMAGetPictureArrayW(VarPtr(dummy(0)), maxLen, Index)
		If ln = 0 Then
			Debug.Print("Fehler beim Laden des Bildes:" & AUDIOGetLastErrorText())
		End If
		ReDim Preserve dummy(ln)
		WMAGetPictureArray = VB6.CopyArray(dummy)
	End Function
	
	Public Function WMAGetPictureDescription(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		WMAGetPictureDescription = StrConv(WMAGetPictureDescriptionW(Index), vbFromUnicode)
	End Function
	
	Public Function WMAGetPictureMime(ByVal Index As Short) As String
		'UPGRADE_ISSUE: Die Konstante vbFromUnicode wurde nicht aktualisiert. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="55B59875-9A95-4B71-9D6A-7C294BF7139D"'
		WMAGetPictureMime = StrConv(WMAGetPictureMimeW(Index), vbFromUnicode)
	End Function
	
	Public Function WMAGetPictureType(ByVal Index As Short) As Short
		WMAGetPictureType = WMAGetPictureTypeW(Index)
	End Function
	
	Public Function WMAGetPictureFile(ByVal FileName As String, ByVal Index As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		WMAGetPictureFile = WMAGetPictureFileW(StrPtr(FileName), Index)
	End Function
	
	Public Function WMAGetPictureCount() As Short
		WMAGetPictureCount = WMAGetPictureCountW()
	End Function
	
	Public Function WMASaveChangesToFile(ByVal FileName As String) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		WMASaveChangesToFile = WMASaveChangesToFileW(StrPtr(FileName))
	End Function
	
	Public Function WMASaveChanges() As Short
		WMASaveChanges = WMASaveChangesW()
	End Function
	
	Public Function WMAAddPictureArray(ByRef arr() As Byte, ByVal maxLen As Integer, ByVal Description As String, ByVal PictureType As Short, ByRef Index As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		'UPGRADE_ISSUE: Die Funktion VarPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		WMAAddPictureArray = WMAAddPictureArrayW(VarPtr(arr(0)), maxLen, StrPtr(Description), PictureType, Index)
	End Function
	
	Public Function WMAAddPictureFile(ByVal FileName As String, ByVal Description As String, ByVal PictureType As Short, ByRef Index As Short) As Short
		'UPGRADE_ISSUE: Die Funktion StrPtr wird nicht unterstützt. Klicken Sie hier für weitere Informationen: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="367764E5-F3F8-4E43-AC3E-7FE0B5E074E2"'
		WMAAddPictureFile = WMAAddPictureFileW(StrPtr(FileName), StrPtr(Description), PictureType, Index)
	End Function
End Class