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

[Dirs]
Name: "{app}\tools"; Permissions: users-modify
Name: "{app}\tools\data"; Permissions: users-modify
Name: "{app}\tools\data\forms"; Permissions: users-modify
Name: "{app}\tools\data\records"; Permissions: users-modify
Name: "{app}\tools\exports"; Permissions: users-modify
Name: "{app}\tools\backups"; Permissions: users-modify
Name: "{app}\tools\templates"; Permissions: users-modify

[Icons]
Name: "{group}\FormDB"; Filename: "{app}\formdb.exe"
Name: "{commondesktop}\FormDB"; Filename: "{app}\formdb.exe"
