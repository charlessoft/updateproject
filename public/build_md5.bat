rem 时间信息
set CurYear=%DATE:~0,4%
set CurMonth=%DATE:~5,2%
set CurDay=%DATE:~8,2%
set CurDateTime=%CurYear%-%CurMonth%-%CurDay%-%time:~0,2%-%time:~3,2%-%time:~6,2%
set CurToday=%CurYear%-%CurMonth%-%CurDay%
rem set PackPath=%CurToday%\%CurDateTime%
rem 打包的文件夹
set PackPath=Build_%CurDateTime%
rem mkdir Build_%CurToday%
mkdir "%PackPath%"

cd ../
PATH=C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\;%PATH%
devenv BuildMd5File.sln /Clean "Release|Win32" /project "BuildMd5File"
devenv BuildMd5File.sln /Build "Release|Win32" /project "BuildMd5File"
copy "release\BuildMd5File.exe" "public\%PackPath%"
copy "BuildMd5File\fciv.exe" "public\%PackPath%"
copy "BuildMd5File\BuildMd5File.ini" "public\%PackPath%"
pause