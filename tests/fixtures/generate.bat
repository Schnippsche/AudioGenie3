@echo off
rem Erzeugt die Testdateien unter tests\fixtures\ mit ffmpeg (1 s Sinuston 440 Hz, wenige KB je Datei).
rem Aufruf: tests\fixtures\generate.bat        (ffmpeg muss im PATH liegen oder in %FFMPEG% stehen)
rem
rem Nicht mit ffmpeg erzeugbar: APE (mac.exe von monkeysaudio.com) und MPC (mpcenc.exe von musepack.net);
rem diese Dateien bitte von Hand in fixtures\ape bzw. fixtures\mpc ablegen.
setlocal
cd /d "%~dp0"
if "%FFMPEG%"=="" set "FFMPEG=ffmpeg"
"%FFMPEG%" -version >nul 2>nul || (echo ffmpeg nicht gefunden. PATH pruefen oder FFMPEG=voller\Pfad setzen. & exit /b 1)

rem bitexact: gleiche Ausgabe unabhaengig von der ffmpeg-Version (kein "encoder=Lavf..."-Tag)
set "FF=%FFMPEG% -y -hide_banner -loglevel error -fflags +bitexact -flags:a +bitexact"
set "SINE=-f lavfi -i sine=frequency=440:duration=1:sample_rate=44100"
set "TAGS=-metadata title=Testtitel -metadata artist=Testkuenstler -metadata album=Testalbum -metadata date=2024 -metadata track=3 -metadata genre=Rock -metadata comment=Kommentar"
set "STEREO=-ac 2 -ar 44100"
set "MONO=-ac 1 -ar 22050"
set "NOMETA=-map_metadata -1"

set "COVER=%~dp0cover_64x64.jpg"
%FF% -f lavfi -i color=c=blue:s=64x64:d=1 -frames:v 1 "%COVER%" || goto :fail

rem ---- MP3 (ohne ID3v2, ohne Xing-Header -> Parser muss Frames selbst zaehlen)
%FF% %SINE% %STEREO% %NOMETA% -b:a 128k -write_id3v2 0 -write_xing 0 mp3\no_tags_cbr.mp3 || goto :fail
%FF% %SINE% %STEREO% %NOMETA% -b:a 128k mp3\no_tags_xing.mp3 || goto :fail
%FF% %SINE% %STEREO% %NOMETA% %TAGS% -b:a 128k mp3\tagged.mp3 || goto :fail
%FF% %SINE% -i "%COVER%" -map 0:a -map 1:v %STEREO% -b:a 128k -c:v copy -id3v2_version 3 %TAGS% -disposition:v attached_pic mp3\with_cover.mp3 || goto :fail
%FF% %SINE% %MONO% %NOMETA% -b:a 64k mp3\mono_22k.mp3 || goto :fail

rem ---- WAV
%FF% %SINE% %STEREO% %NOMETA% wav\no_tags.wav || goto :fail
%FF% %SINE% %STEREO% %NOMETA% %TAGS% wav\tagged.wav || goto :fail
%FF% %SINE% %MONO% %NOMETA% wav\mono_22k.wav || goto :fail

rem ---- FLAC
%FF% %SINE% %STEREO% %NOMETA% flac\no_tags.flac || goto :fail
%FF% %SINE% %STEREO% %NOMETA% %TAGS% flac\tagged.flac || goto :fail
%FF% %SINE% -i "%COVER%" -map 0:a -map 1:v %STEREO% -c:v copy -disposition:v attached_pic %TAGS% flac\with_cover.flac || goto :fail
%FF% %SINE% %MONO% %NOMETA% flac\mono_22k.flac || goto :fail

rem ---- OGG Vorbis
%FF% %SINE% %STEREO% %NOMETA% -c:a libvorbis ogg\no_tags.ogg || goto :fail
%FF% %SINE% %STEREO% %NOMETA% %TAGS% -c:a libvorbis ogg\tagged.ogg || goto :fail
%FF% %SINE% %MONO% %NOMETA% -c:a libvorbis ogg\mono_22k.ogg || goto :fail

rem ---- M4A (AAC und ALAC)
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

rem ---- kaputte Dateien: abgeschnitten (nur Header bzw. Haelfte)
call :truncate mp3\tagged.mp3 broken\tagged_header_only.mp3 128 || goto :fail
call :truncate mp3\tagged.mp3 broken\tagged_half.mp3 0 || goto :fail
call :truncate flac\tagged.flac broken\tagged_header_only.flac 42 || goto :fail
call :truncate flac\tagged.flac broken\tagged_half.flac 0 || goto :fail
call :truncate ogg\tagged.ogg broken\tagged_half.ogg 0 || goto :fail
call :truncate m4a\tagged.m4a broken\tagged_half.m4a 0 || goto :fail
call :truncate wav\tagged.wav broken\tagged_header_only.wav 44 || goto :fail
call :truncate wma\tagged.wma broken\tagged_half.wma 0 || goto :fail

echo Fertig. Erzeugte Dateien:
dir /s /b *.mp3 *.wav *.flac *.ogg *.m4a *.wma *.wv *.tta *.aac 2>nul
exit /b 0

rem :truncate <Quelle> <Ziel> <Bytes>   (Bytes=0 -> Haelfte der Datei)
:truncate
powershell -NoProfile -Command "$b=[IO.File]::ReadAllBytes('%~1'); $n=%~3; if($n -le 0){$n=[int]($b.Length/2)}; [IO.File]::WriteAllBytes('%~2',$b[0..($n-1)])"
exit /b %errorlevel%

:fail
echo FEHLER beim Erzeugen (siehe Meldung oben).
exit /b 1
