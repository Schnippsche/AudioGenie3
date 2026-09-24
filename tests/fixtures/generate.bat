@echo off
rem Creates the test files under tests\fixtures\ with ffmpeg (1 s sine tone 440 Hz, a few KB per file).
rem Usage: tests\fixtures\generate.bat        (ffmpeg must be in the PATH or set in %FFMPEG%)
rem
rem Not creatable with ffmpeg: APE (mac.exe from monkeysaudio.com) and MPC (mpcenc.exe from musepack.net);
rem these tools are used below if they are installed, otherwise the files are left as they are.
setlocal
cd /d "%~dp0"
if "%FFMPEG%"=="" set "FFMPEG=ffmpeg"
"%FFMPEG%" -version >nul 2>nul || (echo ffmpeg not found. Check PATH or set FFMPEG=full\path. & exit /b 1)

rem bitexact: same output independent of the ffmpeg version (no "encoder=Lavf..." tag)
set "FF=%FFMPEG% -y -hide_banner -loglevel error -fflags +bitexact -flags:a +bitexact"
set "SINE=-f lavfi -i sine=frequency=440:duration=1:sample_rate=44100"
set "TAGS=-metadata title=Testtitel -metadata artist=Testkuenstler -metadata album=Testalbum -metadata date=2024 -metadata track=3 -metadata genre=Rock -metadata comment=Kommentar"
set "STEREO=-ac 2 -ar 44100"
set "MONO=-ac 1 -ar 22050"
set "NOMETA=-map_metadata -1"

set "COVER=%~dp0cover_64x64.jpg"
%FF% -f lavfi -i color=c=blue:s=64x64:d=1 -frames:v 1 "%COVER%" || goto :fail

rem ---- MP3 (without ID3v2, without Xing header -> the parser has to count the frames itself)
%FF% %SINE% %STEREO% %NOMETA% -b:a 128k -id3v2_version 0 -write_xing 0 mp3\no_tags_cbr.mp3 || goto :fail
%FF% %SINE% %STEREO% %NOMETA% -b:a 128k mp3\no_tags_xing.mp3 || goto :fail
%FF% %SINE% %STEREO% %NOMETA% %TAGS% -b:a 128k mp3\tagged.mp3 || goto :fail
%FF% %SINE% -i "%COVER%" -map 0:a -map 1:v %STEREO% -b:a 128k -c:v copy -id3v2_version 3 %TAGS% -disposition:v attached_pic mp3\with_cover.mp3 || goto :fail
%FF% %SINE% %MONO% %NOMETA% -b:a 64k mp3\mono_22k.mp3 || goto :fail

rem MP3 with an ID3v1.1 tag only (ffmpeg always writes ID3v2 as well when metadata is given, hence the tag is appended by hand)
powershell -NoProfile -Command "$f='mp3\id3v1_only.mp3'; Copy-Item 'mp3\no_tags_cbr.mp3' $f -Force; $t=New-Object byte[] 128; function put($s,$o){ $e=[Text.Encoding]::GetEncoding(28591).GetBytes($s); [Array]::Copy($e,0,$t,$o,$e.Length) }; put 'TAG' 0; put 'Testtitel' 3; put 'Testkuenstler' 33; put 'Testalbum' 63; put '2024' 93; put 'Kommentar' 97; $t[126]=3; $t[127]=17; $fs=[IO.File]::Open($f,'Append'); $fs.Write($t,0,128); $fs.Close()" || goto :fail

rem ---- WAV
%FF% %SINE% %STEREO% %NOMETA% wav\no_tags.wav || goto :fail
%FF% %SINE% %STEREO% %NOMETA% %TAGS% wav\tagged.wav || goto :fail
%FF% %SINE% %MONO% %NOMETA% wav\mono_22k.wav || goto :fail

rem ---- FLAC
%FF% %SINE% %STEREO% %NOMETA% flac\no_tags.flac || goto :fail
%FF% %SINE% %STEREO% %NOMETA% %TAGS% flac\tagged.flac || goto :fail
%FF% %SINE% -i "%COVER%" -map 0:a -map 1:v %STEREO% -c:v copy -disposition:v attached_pic %TAGS% flac\with_cover.flac || goto :fail
%FF% %SINE% %MONO% %NOMETA% flac\mono_22k.flac || goto :fail

rem ---- OGG Vorbis (not bit-identically reproducible: ffmpeg randomizes the stream serial number; only regenerate when necessary)
%FF% %SINE% %STEREO% %NOMETA% -c:a libvorbis ogg\no_tags.ogg || goto :fail
%FF% %SINE% %STEREO% %NOMETA% %TAGS% -c:a libvorbis ogg\tagged.ogg || goto :fail
%FF% %SINE% %MONO% %NOMETA% -c:a libvorbis ogg\mono_22k.ogg || goto :fail

rem ---- M4A (AAC and ALAC)
%FF% %SINE% %STEREO% %NOMETA% -c:a aac -b:a 96k m4a\no_tags.m4a || goto :fail
%FF% %SINE% %STEREO% %NOMETA% %TAGS% -c:a aac -b:a 96k m4a\tagged.m4a || goto :fail
%FF% %SINE% -i "%COVER%" -map 0:a -map 1:v %STEREO% -c:a aac -b:a 96k -c:v copy -disposition:v attached_pic %TAGS% m4a\with_cover.m4a || goto :fail
%FF% %SINE% %STEREO% %NOMETA% %TAGS% -c:a alac m4a\alac_tagged.m4a || goto :fail

rem ---- WMA
%FF% %SINE% %STEREO% %NOMETA% -c:a wmav2 wma\no_tags.wma || goto :fail
%FF% %SINE% %STEREO% %NOMETA% %TAGS% -c:a wmav2 wma\tagged.wma || goto :fail

rem ---- WavPack, TTA, AAC (roher ADTS-Strom)
%FF% %SINE% %STEREO% %NOMETA% -c:a wavpack wv\no_tags.wv || goto :fail
%FF% %SINE% %STEREO% %NOMETA% %TAGS% -c:a wavpack wv\tagged.wv || goto :fail
%FF% %SINE% %STEREO% %NOMETA% -c:a tta tta\no_tags.tta || goto :fail
%FF% %SINE% %STEREO% %NOMETA% %TAGS% -c:a tta tta\tagged.tta || goto :fail
%FF% %SINE% %STEREO% %NOMETA% -c:a aac -b:a 96k -f adts aac\no_tags.aac || goto :fail

rem ---- APE (Monkey's Audio, MAC.exe) from the WAV fixtures; compression levels 1000/2000/4000/5000, mono, with APE tag, with ID3v1 tag
set "MAC=%ProgramFiles%\Monkey's Audio x64\MAC.exe"
if not exist "%MAC%" (echo Note: MAC.exe not found, APE files are not recreated. & goto :skipape)
"%MAC%" wav\no_tags.wav ape\no_tags_c1000.ape -c1000 >nul || goto :fail
"%MAC%" wav\no_tags.wav ape\no_tags_c2000.ape -c2000 >nul || goto :fail
"%MAC%" wav\no_tags.wav ape\no_tags_c4000.ape -c4000 >nul || goto :fail
"%MAC%" wav\no_tags.wav ape\no_tags_c5000.ape -c5000 >nul || goto :fail
"%MAC%" wav\mono_22k.wav ape\mono_22k.ape -c2000 >nul || goto :fail
"%MAC%" wav\no_tags.wav ape\tagged.ape -c2000 -t "Title=Testtitel|Artist=Testkuenstler|Album=Testalbum|Year=2024|Track=3|Genre=Rock|Comment=Kommentar" >nul || goto :fail
copy /y ape\tagged.ape ape\tagged_id3v1.ape >nul
"%MAC%" ape\tagged_id3v1.ape -L >nul || goto :fail
:skipape

rem ---- Musepack SV8 (mpcenc 1.30 writes SV8 only) from the WAV fixtures; path can be overridden via MPCENC
if "%MPCENC%"=="" set "MPCENC=D:\Entwicklung\Musepack\64bit\mpcenc.exe"
if not exist "%MPCENC%" (echo Note: mpcenc.exe not found, SV8 files are not recreated. & goto :skipmpc)
"%MPCENC%" --silent --overwrite --thumb wav\no_tags.wav mpc\sv8_thumb.mpc || goto :fail
"%MPCENC%" --silent --overwrite --standard wav\no_tags.wav mpc\sv8_standard.mpc || goto :fail
"%MPCENC%" --silent --overwrite --insane wav\no_tags.wav mpc\sv8_insane.mpc || goto :fail
%FF% %SINE% -ac 1 -ar 44100 %NOMETA% "%TEMP%\ag3_mono44.wav" || goto :fail
"%MPCENC%" --silent --overwrite --standard "%TEMP%\ag3_mono44.wav" mpc\sv8_mono_44k.mpc || goto :fail
del "%TEMP%\ag3_mono44.wav" >nul 2>nul
:skipmpc
rem ---- Musepack SV7 (mppenc 1.16, path can be overridden via MPPENC) from the WAV fixtures; 48/32 kHz from temporary WAVs
if "%MPPENC%"=="" set "MPPENC=D:\Entwicklung\Musepack7\mppenc.exe"
if not exist "%MPPENC%" (echo Note: mppenc.exe not found, real SV7 files are not recreated. & goto :skipmpp)
for %%Q in (thumb radio standard extreme insane) do "%MPPENC%" --silent --overwrite --%%Q wav\no_tags.wav mpc\sv7_%%Q.mpc || goto :fail
%FF% -f lavfi -i sine=frequency=440:duration=1:sample_rate=48000 -ac 2 -ar 48000 %NOMETA% "%TEMP%\ag3_s48.wav" || goto :fail
%FF% -f lavfi -i sine=frequency=440:duration=1:sample_rate=32000 -ac 2 -ar 32000 %NOMETA% "%TEMP%\ag3_s32.wav" || goto :fail
%FF% %SINE% -ac 1 -ar 44100 %NOMETA% "%TEMP%\ag3_mono44.wav" || goto :fail
"%MPPENC%" --silent --overwrite --thumb "%TEMP%\ag3_s48.wav" mpc\sv7_thumb_48k.mpc || goto :fail
"%MPPENC%" --silent --overwrite --thumb "%TEMP%\ag3_s32.wav" mpc\sv7_thumb_32k.mpc || goto :fail
"%MPPENC%" --silent --overwrite --standard "%TEMP%\ag3_mono44.wav" mpc\sv7_mono_44k.mpc || goto :fail
del "%TEMP%\ag3_s48.wav" "%TEMP%\ag3_s32.wav" "%TEMP%\ag3_mono44.wav" >nul 2>nul
:skipmpp
rem Musepack: synthetic SV7 headers and appended tags (APEv2/ID3v2), needs Python
where python >nul 2>nul && python make_mpc_fixtures.py || echo Note: Python not found, SV7 fixtures are not recreated.

rem ---- raw ADTS stream from the real MP4/AAC samples (aac\sample-*.aac, taken over unchanged), without and with an ID3v2 tag
if exist aac\sample-1.aac %FF% -i aac\sample-1.aac -map 0:a -c copy -f adts aac\adts_sample-1.aac || goto :fail
if exist aac\sample-2.aac %FF% -i aac\sample-2.aac -map 0:a -c copy -map_metadata -1 %TAGS% -write_id3v2 1 -f adts aac\adts_id3_sample-2.aac || goto :fail

rem ---- hand-built ID3v2 tags (real COMM frame, TYER or TDRC timestamp), needs Python
where python >nul 2>nul && python make_id3_fixtures.py || echo Note: Python not found, ID3 fixtures are not recreated.

rem ---- broken files: truncated (header only or half)
call :truncate mp3\tagged.mp3 broken\tagged_header_only.mp3 128 || goto :fail
call :truncate mp3\tagged.mp3 broken\tagged_half.mp3 0 || goto :fail
call :truncate flac\tagged.flac broken\tagged_header_only.flac 42 || goto :fail
call :truncate flac\tagged.flac broken\tagged_half.flac 0 || goto :fail
call :truncate ogg\tagged.ogg broken\tagged_half.ogg 0 || goto :fail
call :truncate m4a\tagged.m4a broken\tagged_half.m4a 0 || goto :fail
call :truncate wav\tagged.wav broken\tagged_header_only.wav 44 || goto :fail
call :truncate wma\tagged.wma broken\tagged_half.wma 0 || goto :fail

echo Done. Created files:
dir /s /b *.mp3 *.wav *.flac *.ogg *.m4a *.wma *.wv *.tta *.aac 2>nul
exit /b 0

rem :truncate <source> <target> <bytes>   (bytes=0 -> half of the file)
:truncate
powershell -NoProfile -Command "$b=[IO.File]::ReadAllBytes('%~1'); $n=%~3; if($n -le 0){$n=[int]($b.Length/2)}; [IO.File]::WriteAllBytes('%~2',$b[0..($n-1)])"
exit /b %errorlevel%

:fail
echo ERROR while creating (see the message above).
exit /b 1
