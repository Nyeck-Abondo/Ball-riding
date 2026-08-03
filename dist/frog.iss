
[Setup]
AppName=frog
AppPublisher=NAEM
AppVersion=1.0
DefaultDirName={autopf}\frog
DefaultGroupName=frog
UninstallDisplayIcon={app}\frog.exe
DisableDirPage=no
DisableProgramGroupPage=no
Compression=lzma2
SolidCompression=yes
OutputDir=.
OutputBaseFilename=frog_setup

[Tasks]
Name: "desktopicon"; Description: "Creer un raccourci sur le bureau"; GroupDescription: "Raccourcis supplementaires :"

[Files]
Source: "C:\Users\Administrator\Documents\Github\Ball-riding\Build\Bin\Release-Windows\frog\frog.exe"; DestDir: "{app}"
Source: "C:\Users\Administrator\Documents\Github\Ball-riding\dist\frog_app.ico"; DestDir: "{app}"; DestName: "app.ico"
Source: "C:\Users\Administrator\Documents\Github\Ball-riding\assets\*"; DestDir: "{app}\assets"; Flags: recursesubdirs createallsubdirs ignoreversion

[Icons]
Name: "{group}\frog"; Filename: "{app}\frog.exe"; IconFilename: "{app}\app.ico"
Name: "{autodesktop}\frog"; Filename: "{app}\frog.exe"; IconFilename: "{app}\app.ico"; Tasks: desktopicon

[Run]

Filename: "{app}\frog.exe"; Description: "Lancer frog"; Flags: nowait postinstall skipifsilent

[UninstallRun]

