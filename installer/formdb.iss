[Setup]
AppName=FormDB
AppVersion=1.0.0
DefaultDirName={pf}\FormDB
DefaultGroupName=FormDB
OutputDir=..\dist
OutputBaseFilename=FormDB-Setup
Compression=lzma
SolidCompression=yes

[Files]
Source: "..\bin\formdb.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\msys-2.0.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\FormDB"; Filename: "{app}\formdb.exe"
Name: "{commondesktop}\FormDB"; Filename: "{app}\formdb.exe"
