using System;
using System.IO;
using AudioGenie;

// Smoke test of the C# wrapper (Wrapper/C #/AudioGenie2.cs) against the DLL.
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

        string version = AudioGenie2.GetAudioGenieVersion();
        Console.WriteLine("DLL version: " + version);
        Check("version is 3.0.0.0", version == "3.0.0.0", version);

        string tagged = Path.Combine(fixtures, "mp3", "tagged.mp3");
        AudioFormatID fmt = AudioGenie2.AUDIOAnalyzeFile(tagged);
        Check("format is MPEG", fmt == AudioFormatID.MPEG, fmt.ToString());
        string title = AudioGenie2.AUDIOTitle;
        Console.WriteLine("  title='" + title + "' artist='" + AudioGenie2.AUDIOArtist + "' album='" + AudioGenie2.AUDIOAlbum + "' year='" + AudioGenie2.AUDIOYear + "'");
        Console.WriteLine("  duration=" + AudioGenie2.AUDIOGetDuration().ToString("F2") + " s, bit rate=" + AudioGenie2.AUDIOGetBitrate() + " kbit/s, sample rate=" + AudioGenie2.AUDIOGetSampleRate());
        Check("title", title == "Testtitel", title);
        Check("artist", AudioGenie2.AUDIOArtist == "Testkuenstler", AudioGenie2.AUDIOArtist);
        Check("duration > 0", AudioGenie2.AUDIOGetDuration() > 0, "");
        Check("bit rate > 0", AudioGenie2.AUDIOGetBitrate() > 0, "");
        string md5 = AudioGenie2.AUDIOGetMD5Value();
        Check("MD5 has 32 characters", md5.Length == 32, md5);

        string work = Path.Combine(Path.GetTempPath(), "cscheck_" + (Environment.Is64BitProcess ? "x64" : "x86") + ".mp3");
        File.Copy(tagged, work, true);
        string special = "Titel \u00e4\u00f6\u00fc\u20ac";
        AudioGenie2.AUDIOAnalyzeFile(work);
        AudioGenie2.AUDIOTitle = special;
        AudioGenie2.AUDIOArtist = "C#";
        Check("save", AudioGenie2.AUDIOSaveChanges(), "AUDIOSaveChanges returned false");
        AudioGenie2.AUDIOAnalyzeFile(work);
        Check("title round trip", AudioGenie2.AUDIOTitle == special, AudioGenie2.AUDIOTitle);
        Check("artist round trip", AudioGenie2.AUDIOArtist == "C#", AudioGenie2.AUDIOArtist);
        Check("MD5 of the audio data unchanged", AudioGenie2.AUDIOGetMD5Value() == md5, "");
        File.Delete(work);

        Check("missing file is not recognized", AudioGenie2.AUDIOAnalyzeFile(Path.Combine(fixtures, "does_not_exist.mp3")) == AudioFormatID.UNKNOWN, "");

        Console.WriteLine(failures == 0 ? "ALL OK" : failures + " check(s) failed");
        return failures;
    }
}
