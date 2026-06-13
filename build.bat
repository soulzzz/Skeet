@echo off
call "C:\VS\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul 2>&1
"C:\VS\Community\MSBuild\Current\Bin\MSBuild.exe" "C:\AMD\DMA-kaka\DMA-KaKa.sln" /p:Configuration=Release /p:Platform=x64 /v:minimal
