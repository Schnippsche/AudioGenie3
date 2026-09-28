using System;
using System.IO;
using AudioGenie;

// Smoke test of the C# wrapper (Wrapper/C #/AudioGenie3.cs) against the DLL.
public static class CsCheck
{
    static int failures;

    static void Check(string name, bool ok, string detail)
    {
        if (ok) Console.WriteLine("  ok    " + name);
        else { Console.WriteLine("  FAIL  " + name + "  (" + detail + ")"); failures++; }
    }

    public static int Main(string[] args)
    {
        string fixtures = args[0];
        Console.WriteLine("process: " + (Environment.Is64BitProcess ? "64 bit" : "32 bit"));

        string version = AudioGenie3.GetAudioGenieVersion();
        Console.WriteLine("DLL version: " + version);
        // the version of the DLL (read from its version resource) equals the version of the DLL file
        string dllPath = Path.Combine(Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location), "AudioGenie3.dll");
        string fileVersion = System.Diagnostics.FileVersionInfo.GetVersionInfo(dllPath).FileVersion;
        Check("version equals the file version of the DLL (" + fileVersion + ")", version == fileVersion, version);

        string tagged = Path.Combine(fixtures, "mp3", "tagged.mp3");
        AudioFormatID fmt = AudioGenie3.AUDIOAnalyzeFile(tagged);
        Check("format is MPEG", fmt == AudioFormatID.MPEG, fmt.ToString());
        string title = AudioGenie3.AUDIOTitle;
        Console.WriteLine("  title='" + title + "' artist='" + AudioGenie3.AUDIOArtist + "' album='" + AudioGenie3.AUDIOAlbum + "' year='" + AudioGenie3.AUDIOYear + "'");
        Console.WriteLine("  duration=" + AudioGenie3.AUDIOGetDuration().ToString("F2") + " s, bit rate=" + AudioGenie3.AUDIOGetBitrate() + " kbit/s, sample rate=" + AudioGenie3.AUDIOGetSampleRate());
        Check("title", title == "Testtitel", title);
        Check("artist", AudioGenie3.AUDIOArtist == "Testkuenstler", AudioGenie3.AUDIOArtist);
        Check("duration > 0", AudioGenie3.AUDIOGetDuration() > 0, "");
        Check("bit rate > 0", AudioGenie3.AUDIOGetBitrate() > 0, "");
        string md5 = AudioGenie3.AUDIOGetMD5Value();

        // LAME tag: the functions can be called; without a tag they return 0 or an empty text
        bool lame = AudioGenie3.MPEGHasLameTag();
        Console.WriteLine("  LAME tag: " + lame + " " + AudioGenie3.MPEGGetLameVersion() + " delay=" + AudioGenie3.MPEGGetEncoderDelay() + " padding=" + AudioGenie3.MPEGGetEncoderPadding());
        Check("LAME tag functions are consistent", lame || (AudioGenie3.MPEGGetEncoderDelay() == 0 && AudioGenie3.MPEGGetLameVersion() == "" && !AudioGenie3.MPEGIsLameTagCrcValid()), "");
        Check("LAME tag values are in range", AudioGenie3.MPEGGetLameLowpass() >= 0 && AudioGenie3.MPEGGetLameBitrate() >= 0 && AudioGenie3.MPEGGetLamePreset() >= 0 && AudioGenie3.MPEGGetLameMusicLength() >= 0
            && AudioGenie3.MPEGGetLameRevision() >= 0 && AudioGenie3.MPEGGetLameVBRMethod() >= 0 && AudioGenie3.MPEGGetLameMp3Gain() >= -127 && AudioGenie3.MPEGGetLamePeakSignal() >= 0f
            && Math.Abs(AudioGenie3.MPEGGetLameRadioGain()) < 60f && Math.Abs(AudioGenie3.MPEGGetLameAudiophileGain()) < 60f, "");
        bool musicCrc = AudioGenie3.MPEGIsLameMusicCrcValid();   // reads the audio data of the file
        Check("LAME music checksum needs a LAME tag", lame || !musicCrc, "");
        Check("MD5 has 32 characters", md5.Length == 32, md5);

        string work = Path.Combine(Path.GetTempPath(), "cscheck_" + (Environment.Is64BitProcess ? "x64" : "x86") + ".mp3");
        File.Copy(tagged, work, true);
        string special = "Titel \u00e4\u00f6\u00fc\u20ac";
        AudioGenie3.AUDIOAnalyzeFile(work);
        AudioGenie3.AUDIOTitle = special;
        AudioGenie3.AUDIOArtist = "C#";
        Check("save", AudioGenie3.AUDIOSaveChanges(), "AUDIOSaveChanges returned false");
        AudioGenie3.AUDIOAnalyzeFile(work);
        Check("title round trip", AudioGenie3.AUDIOTitle == special, AudioGenie3.AUDIOTitle);
        Check("artist round trip", AudioGenie3.AUDIOArtist == "C#", AudioGenie3.AUDIOArtist);
        Check("MD5 of the audio data unchanged", AudioGenie3.AUDIOGetMD5Value() == md5, "");

        // enhanced ID3v1 tag: speed, genre text and times
        AudioGenie3.AUDIOAnalyzeFile(work);
        AudioGenie3.ID3V1Speed = 2;
        AudioGenie3.ID3V1EnhancedGenre = "Wrapper";
        AudioGenie3.ID3V1StartTime = "000:10";
        AudioGenie3.ID3V1EndTime = "003:20";
        Check("ID3v1 save", AudioGenie3.ID3V1SaveChanges(), "ID3V1SaveChanges returned false");
        AudioGenie3.AUDIOAnalyzeFile(work);
        Check("ID3v1 speed", AudioGenie3.ID3V1Speed == 2, AudioGenie3.ID3V1Speed.ToString());
        Check("ID3v1 genre text", AudioGenie3.ID3V1EnhancedGenre == "Wrapper", AudioGenie3.ID3V1EnhancedGenre);
        Check("ID3v1 start time", AudioGenie3.ID3V1StartTime == "000:10", AudioGenie3.ID3V1StartTime);
        Check("ID3v1 end time", AudioGenie3.ID3V1EndTime == "003:20", AudioGenie3.ID3V1EndTime);
        Check("MD5 unchanged after the enhanced ID3v1 tag", AudioGenie3.AUDIOGetMD5Value() == md5, "");
        File.Delete(work);

        Check("missing file is not recognized", AudioGenie3.AUDIOAnalyzeFile(Path.Combine(fixtures, "does_not_exist.mp3")) == AudioFormatID.UNKNOWN, "");

        Console.WriteLine(failures == 0 ? "ALL OK" : failures + " check(s) failed");
        return failures;
    }
}
