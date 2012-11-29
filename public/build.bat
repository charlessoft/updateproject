rem 时间信息
set CurYear=%DATE:~0,4%
set CurMonth=%DATE:~5,2%
set CurDay=%DATE:~8,2%
set CurDateTime=%CurYear%-%CurMonth%-%CurDay%-%time:~0,2%-%time:~3,2%-%time:~6,2%
set CurToday=%CurYear%-%CurMonth%-%CurDay%
rem set PackPath=%CurToday%\%CurDateTime%
rem 打包的文件夹
set PackPath=%CurDateTime%
rem mkdir %CurToday%
mkdir "%PackPath%"

cd ../
PATH=C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\;%PATH%
devenv Update.sln /Clean "Release|Win32" /project "Update"
devenv Update.sln /Build "Release|Win32" /project "Update"
copy "release\Update.exe" "public\%PackPath%"
copy "Update\update.config" "public\%PackPath%"
copy "Update\UPDATE_properties" "public\%PackPath%"
copy "Update\UpdateConfig.ini" "public\%PackPath%"




