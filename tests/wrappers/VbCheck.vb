Imports System
Imports System.IO
Imports AudioGenie.AudioGenie2

' Smoke test of the VB.NET wrapper (Wrapper/DotNET/audiogenie3.vb) against the DLL.
Module VbCheck
    Dim failures As Integer = 0

    Sub Check(ByVal name As String, ByVal ok As Boolean, ByVal detail As String)
        If ok Then
            Console.WriteLine("  ok    " & name)
        Else
            Console.WriteLine("  FAIL  " & name & "  (" & detail & ")")
            failures += 1
        End If
    End Sub

    Function Main(ByVal args As String()) As Integer
        Dim fixtures As String = args(0)
        Console.WriteLine("process: " & If(Environment.Is64BitProcess, "64 bit", "32 bit"))

        Dim version As String = GetAudioGenieVersion()
        Console.WriteLine("DLL version: " & version)
        ' the version of the DLL (read from its version resource) equals the version of the DLL file
        Dim dllPath As String = Path.Combine(Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location), "AudioGenie3.dll")
        Dim fileVersion As String = System.Diagnostics.FileVersionInfo.GetVersionInfo(dllPath).FileVersion
        Check("version equals the file version of the DLL (" & fileVersion & ")", version = fileVersion, version)

        ' read tags
        Dim tagged As String = Path.Combine(fixtures, "mp3", "tagged.mp3")
        Dim fmt As AudioGenie.AudioFormatID = AUDIOAnalyzeFile(tagged)
        Check("format is MPEG", fmt = AudioGenie.AudioFormatID.MPEG, fmt.ToString())
        Console.WriteLine("  title='" & AUDIOTitle & "' artist='" & AUDIOArtist & "' album='" & AUDIOAlbum & "' year='" & AUDIOYear & "'")
        Console.WriteLine("  duration=" & AUDIOGetDuration().ToString("F2") & " s, bit rate=" & AUDIOGetBitrate() & " kbit/s, sample rate=" & AUDIOGetSampleRate())
        Check("title", AUDIOTitle = "Testtitel", AUDIOTitle)
        Check("artist", AUDIOArtist = "Testkuenstler", AUDIOArtist)
        Check("duration > 0", AUDIOGetDuration() > 0, AUDIOGetDuration().ToString())
        Check("bit rate > 0", AUDIOGetBitrate() > 0, AUDIOGetBitrate().ToString())
        Dim md5 As String = AUDIOGetMD5Value()
        Check("MD5 has 32 characters", md5.Length = 32, md5)

        ' write tags with special characters and read them back
        Dim work As String = Path.Combine(Path.GetTempPath(), "vbcheck_" & If(Environment.Is64BitProcess, "x64", "x86") & ".mp3")
        File.Copy(tagged, work, True)
        Dim special As String = "Titel " & ChrW(228) & ChrW(246) & ChrW(252) & ChrW(8364)
        AUDIOAnalyzeFile(work)
        AUDIOTitle = special
        AUDIOArtist = "VB.NET"
        Check("save", AUDIOSaveChanges(), "AUDIOSaveChanges returned False")
        AUDIOAnalyzeFile(work)
        Check("title round trip", AUDIOTitle = special, AUDIOTitle)
        Check("artist round trip", AUDIOArtist = "VB.NET", AUDIOArtist)
        Check("MD5 of the audio data unchanged", AUDIOGetMD5Value() = md5, AUDIOGetMD5Value())

        ' enhanced ID3v1 tag: speed, genre text and times
        AUDIOAnalyzeFile(work)
        ID3V1Speed = 2
        ID3V1EnhancedGenre = "Wrapper"
        ID3V1StartTime = "000:10"
        ID3V1EndTime = "003:20"
        Check("ID3v1 save", ID3V1SaveChanges(), "ID3V1SaveChanges returned False")
        AUDIOAnalyzeFile(work)
        Check("ID3v1 speed", ID3V1Speed = 2, ID3V1Speed.ToString())
        Check("ID3v1 genre text", ID3V1EnhancedGenre = "Wrapper", ID3V1EnhancedGenre)
        Check("ID3v1 start time", ID3V1StartTime = "000:10", ID3V1StartTime)
        Check("ID3v1 end time", ID3V1EndTime = "003:20", ID3V1EndTime)
        Check("MD5 unchanged after the enhanced ID3v1 tag", AUDIOGetMD5Value() = md5, AUDIOGetMD5Value())
        File.Delete(work)

        ' unknown file
        Check("missing file is not recognized", AUDIOAnalyzeFile(Path.Combine(fixtures, "does_not_exist.mp3")) = AudioGenie.AudioFormatID.UNKNOWN, "")

        Console.WriteLine(If(failures = 0, "ALL OK", failures & " check(s) failed"))
        Return failures
    End Function
End Module
