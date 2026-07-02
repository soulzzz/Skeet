param(
    [switch]$Build,
    [switch]$IncludeSymbols
)

$ErrorActionPreference = 'Stop'

$RepoRoot = Resolve-Path (Join-Path $PSScriptRoot '..')
$Solution = Join-Path $RepoRoot 'Skeet.sln'
$OutputDir = Join-Path $RepoRoot 'x64\Skeet'
$DistDir = Join-Path $RepoRoot 'dist\Skeet'
$MSBuild = 'D:\VS\Community\MSBuild\Current\Bin\MSBuild.exe'

if ($Build) {
    if (-not (Test-Path -LiteralPath $MSBuild)) {
        throw "MSBuild not found: $MSBuild"
    }

    & $MSBuild $Solution /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m /v:minimal /nologo
    if ($LASTEXITCODE -ne 0) {
        throw "MSBuild failed with exit code $LASTEXITCODE"
    }
}

if (-not (Test-Path -LiteralPath $OutputDir)) {
    throw "Release output not found: $OutputDir"
}

New-Item -ItemType Directory -Force -Path $DistDir | Out-Null

$RequiredFiles = @(
    'Skeet.exe',
    'ddll64.dll',
    'embree4.dll',
    'freetype.dll',
    'FTD3XX.dll',
    'leechcore.dll',
    'tbb12.dll',
    'vmm.dll',
    'VMProtectSDK64.dll',
    'zstd.dll'
)

if ($IncludeSymbols) {
    $RequiredFiles += @('Skeet.pdb', 'Skeet.lib', 'Skeet.exp')
}

$Copied = @()
foreach ($File in $RequiredFiles) {
    $Source = Join-Path $OutputDir $File
    if (-not (Test-Path -LiteralPath $Source)) {
        throw "Required output missing: $Source"
    }

    Copy-Item -LiteralPath $Source -Destination (Join-Path $DistDir $File) -Force
    $Copied += $File
}

Copy-Item -LiteralPath (Join-Path $RepoRoot 'README.md') -Destination (Join-Path $DistDir 'README.md') -Force

$Manifest = [ordered]@{
    name = 'Skeet'
    configuration = 'Release'
    platform = 'x64'
    source = "$OutputDir"
    output = "$DistDir"
    createdAt = (Get-Date).ToString('yyyy-MM-dd HH:mm:ss zzz')
    files = $Copied
}

$Manifest | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath (Join-Path $DistDir 'package-manifest.json') -Encoding UTF8

Write-Host "Packaged Skeet Release to $DistDir"
