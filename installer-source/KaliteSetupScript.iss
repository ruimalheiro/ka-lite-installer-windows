#define MyAppName "KA Lite"
#define MyAppPublisher "Foundation for Learning Equality"
#define MyAppURL "http://learningequality.org/"
#define MyAppExeName "KA Lite.exe"

#define getKALiteVerion() \
    Local[1] = Exec(SourcePath+"\getversion.bat") == 0 ? StringChange(FileRead(FileOpen(SourcePath+"\version.temp")), " ", "") : "null"

#define MyVersion = getKALiteVerion();

#expr DeleteFile(SourcePath+"\version.temp")


[Setup]
AppId={#MyAppName}-{#MyAppPublisher}
AppName={#MyAppName}
AppVersion={#MyVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
LicenseFile=..\ka-lite\LICENSE
OutputDir=..\
OutputBaseFilename=KALiteSetup-{#MyVersion}
SetupIconFile=logo48.ico
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin
UsePreviousAppDir=yes
CreateUninstallRegKey=no
UpdateUninstallLogAppName=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\ka-lite\*"; DestDir: "{app}\ka-lite"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\ka-lite\content\*"; DestDir: "{app}\ka-lite\content"; Flags: ignoreversion recursesubdirs createallsubdirs uninsneveruninstall
Source: "..\ka-lite\kalite\database\*"; DestDir: "{app}\ka-lite\kalite\database"; Flags: ignoreversion recursesubdirs createallsubdirs uninsneveruninstall
Source: "..\gui-packed\KA Lite.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\gui-packed\guitools.vbs"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\python-setup\*"; DestDir: "{app}\python-setup"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "Uninstall KA Lite"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: shellexec postinstall skipifsilent

[Dirs]
Name: "{app}\"; Permissions: everyone-modify


[InstallDelete]
Type: Files; Name: "{app}\ka-lite\kalite\updates\utils.*"

[UninstallDelete]
Type: filesandordirs; Name: "{app}\ka-lite\python-packages"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\foo"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\central"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\coachreports"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\config"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\contact"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\control_panel"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\faq"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\khanload"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\loadtesting"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\main"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\management"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\registration"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\securesync"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\shared"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\static"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\templatetags"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\templates"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\tests"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\updates"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\utils"
Type: Files; Name: "{app}\ka-lite\kalite\*.pyc"
Type: Files; Name: "{userstartup}\KA Lite.lnk"
Type: Files; Name: "{app}\CONFIG.dat"

[code]
var
  installFlag : boolean;
  startupFlag : string;
  ServerInformationPage : TInputQueryWizardPage;
  UserInformationPage : TInputQueryWizardPage;
  StartupPage : TInputOptionWizardPage;
  existDatabase : boolean;
  isUpgrade : boolean;
  stopServerCode: integer;
  removeOldGuiTool: integer;

procedure InitializeWizard;
begin

    existDatabase := False;
    isUpgrade := False;
    
    if WizardForm.PrevAppDir <> nil then
    begin
        Exec(ExpandConstant('{cmd}'),'/C ka-lite\scripts\stop.bat', WizardForm.PrevAppDir, SW_HIDE, ewWaitUntilTerminated, stopServerCode);
        Exec(ExpandConstant('{cmd}'),'/C del winshortcut.vbs', WizardForm.PrevAppDir, SW_HIDE, ewWaitUntilTerminated, removeOldGuiTool);
    end;
    
    // Server data
    ServerInformationPage := CreateInputQueryPage(wpLicense,
    'Server Information', 'General data',
    'Please specify the server name and a description, then click Next. (you can leave blank both fields if you want to use the default server name or if you do not want to insert a description)');
    ServerInformationPage.Add('Server name:', False);
    ServerInformationPage.Add('Server description:', False);
    
    // Admin user creation
    UserInformationPage := CreateInputQueryPage(ServerInformationPage.ID,
    'Admin Information', 'Create admin user',
    'Please specify username and password to create an admin, then click Next.');
    UserInformationPage.Add('Username:', False);
    UserInformationPage.Add('Password:', True);
    UserInformationPage.Add('Confirm Password:', True);
  
    // Run at windows startup.
    StartupPage := CreateInputOptionPage(UserInformationPage.ID,
    'Server Configuration', 'Startup configuration',
    'Do you wish to set the KA Lite server to run in the background automatically when you start Windows?', True, False);
    StartupPage.Add('Yes');
    StartupPage.Add('No');
    StartupPage.SelectedValueIndex := 1;
end;

function ShouldSkipPage(PageID: Integer): Boolean;
begin
    result := False;
    if isUpgrade = True then
    begin
        if PageID = ServerInformationPage.ID then
        begin
            result := True;
        end;
        if PageID = UserInformationPage.ID then
        begin
            result := True;
        end;
    end;
end;

function NextButtonClick(CurPageID: Integer): Boolean;
begin
    result := True;
    if CurPageID = UserInformationPage.ID then
    begin
        if UserInformationPage.Values[0] <> nil then
        begin
            if (UserInformationPage.Values[1] = UserInformationPage.Values[2]) And (UserInformationPage.Values[1] <> nil) And (UserInformationPage.Values[2] <> nil) then
            begin
                result := True;
            end
            else begin
                MsgBox('Error' #13#13 'Invalid password or the password does not match on both fields.', mbError, MB_OK);
                result := False;
            end;
        end
        else begin
            MsgBox('Error' #13#13 'Invalid username. You must enter a non empty username.', mbError, MB_OK);
            result := False;
        end;
    end;
    
    if CurPageID = wpLicense then
    begin
        if WizardForm.PrevAppDir <> nil then
        begin
            if FileExists(WizardForm.PrevAppDir + '\ka-lite\kalite\database\data.sqlite') then
            begin
                if MsgBox('A database file from a previous installation already exists; do you want to keep the old data and upgrade your install?', mbInformation,  MB_YESNO or MB_DEFBUTTON1) = IDYES then
                begin
                    existDatabase := True;
                    isUpgrade := True;
                end
                else if MsgBox('Installing fresh will delete all your own data; do you really want to do this?', mbInformation,  MB_YESNO or MB_DEFBUTTON2) = IDYES then
                begin
                    existDatabase := False;
                    isUpgrade := False;
                    if Not DeleteFile(WizardForm.PrevAppDir + '\ka-lite\kalite\database\data.sqlite') then
                    begin
                        MsgBox('Error' #13#13 'Failed to delete Django database; continuing install.', mbError, MB_OK);
                    end;
                end;
            end;
        end;
    end;
    
    
end;

function InitializeSetup(): Boolean;
var
  PythonVersionCodeCheck: integer;
  InstallPython: integer;
  killErrorCode: integer;
begin
    installFlag:=true;
    Result := true;
    startupFlag:=''; 
  
    ShellExec('open','taskkill.exe','/F /T /im "KA Lite.exe"','',SW_HIDE,ewNoWait,killErrorCode)
    ShellExec('open','tskill.exe',' "KA Lite"','',SW_HIDE,ewNoWait,killErrorCode);

    RegDeleteValue(HKCU, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Run', ExpandConstant('{#MyAppName}'));
   
    if ShellExec('open', 'python.exe','-c "import sys; sys.version_info[0]==2 and sys.version_info[1] >= 6 and sys.exit(0) or sys.exit(1)"', '', SW_HIDE, ewWaitUntilTerminated, PythonVersionCodeCheck) then
    begin
        if PythonVersionCodeCheck = 1 then
        begin
            if(MsgBox('Error' #13#13 'Python 2.6+ is required to run KA Lite; do you wish to first install Python 2.7.3, before continuing with the installation of KA Lite?', mbConfirmation, MB_YESNO) = idYes) then
            begin
            ExtractTemporaryFile('python-2.7.3.msi');
            ShellExec('open', ExpandConstant('{tmp}')+'\python-2.7.3.msi', '', '', SW_SHOWNORMAL, ewWaitUntilTerminated, InstallPython);  
            end
            else begin
                MsgBox('Error' #13#13 'You must manually upgrade your Python version to version 2.6 or higher after the installation, before you can run KA Lite', mbInformation, MB_OK);
            end;         
        end;
    end
    else begin
        if (MsgBox('Python' #13#13 'Python 2.6+ is required to run KA Lite; do you wish to first install Python 2.7.3, before continuing with the installation of KA Lite?', mbConfirmation, MB_YESNO) = idYes) then
        begin
            ExtractTemporaryFile('python-2.7.3.msi');
            ShellExec('open', ExpandConstant('{tmp}')+'\python-2.7.3.msi', '', '', SW_SHOWNORMAL, ewWaitUntilTerminated, InstallPython);
        end
        else begin
            MsgBox('Error' #13#13 'You must have Python 2.6+ installed to proceed! Installation will now exit.', mbError, MB_OK);
            Result := false;
        end;
    end;  
end;

function InitializeUninstall(): Boolean;
var
ErrorCode: Integer;
begin
  ShellExec('open', 'taskkill.exe', '/F /T /im "KA Lite.exe"', '', SW_HIDE, ewNoWait, ErrorCode);
  ShellExec('open', 'tskill.exe', '"KA Lite"', '', SW_HIDE, ewNoWait, ErrorCode);
  ShellExec('open', ExpandConstant('{app}') + '\ka-lite\stop.bat', '', '', SW_HIDE, ewNoWait, ErrorCode);
  result := True;
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  ServerNameDescriptionCode: integer;
  StartupCode: integer;
  moveKaliteFolderTemp: integer;
  moveContentFolderTemp: integer;
  cleanKaliteFolder: integer;
  restoreKaliteFolder: integer;
  restoreContentFolder: integer;
  informationBoxFlagged: boolean;
  setupCommand: string;
  askAboutUpgrade: boolean;
  uninstallError: integer;
  
begin
    if CurStep = ssInstall then
    begin
        informationBoxFlagged :=False;
        
        Exec(ExpandConstant('{cmd}'),'/C ka-lite\scripts\stop.bat', ExpandConstant('{app}'), SW_HIDE, ewWaitUntilTerminated, stopServerCode);
        Exec(ExpandConstant('{cmd}'),'/C del winshortcut.vbs', ExpandConstant('{app}'), SW_HIDE, ewWaitUntilTerminated, removeOldGuiTool);
    
        if DirExists(ExpandConstant('{app}') + '\kalite') then
        begin
            MsgBox('KA Lite old data structure' #13#13 'Setup detected that you have the old file structure. Setup will now move data to update the structure. Please be patient; this may take some time.', mbInformation, MB_OK);
            informationBoxFlagged :=True;      
            Exec(ExpandConstant('{cmd}'),'/C mkdir '+ExpandConstant('{tmp}')+'\ka-lite\kalite & xcopy /y /s kalite\* '+ExpandConstant('{tmp}')+'\ka-lite\kalite', ExpandConstant('{app}'), SW_HIDE, ewWaitUntilTerminated, moveKaliteFolderTemp);      
        end; 
      
        if DirExists(ExpandConstant('{app}') + '\content') then
        begin
            if Not informationBoxFlagged then
            begin
                MsgBox('KA Lite old data structure' #13#13 'Setup detected that you have the old file structure. Setup will now move data to update the structure. Please be patient; this may take some time.', mbInformation, MB_OK);
                informationBoxFlagged :=True;
            end;      
            Exec(ExpandConstant('{cmd}'),'/C mkdir '+ExpandConstant('{tmp}')+'\ka-lite\content & xcopy /y /s content\* '+ExpandConstant('{tmp}')+'\ka-lite\content', ExpandConstant('{app}'), SW_HIDE, ewWaitUntilTerminated, moveContentFolderTemp);      
        end;      
    
        if informationBoxFlagged then
        begin
            Exec(ExpandConstant('{cmd}'),'/C cd .. & del /q "'+ExpandConstant('{app}')+'\*" & for /d %x in ( "'+ExpandConstant('{app}')+'\*" ) do @rd /s /q "%x"', ExpandConstant('{app}'), SW_HIDE, ewWaitUntilTerminated, cleanKaliteFolder);
    
            if DirExists(ExpandConstant('{tmp}')+'\ka-lite\kalite') then
            begin
                Exec(ExpandConstant('{cmd}'),'/C mkdir ka-lite\kalite & xcopy /y /s '+ExpandConstant('{tmp}')+'\ka-lite\kalite\* ka-lite\kalite', ExpandConstant('{app}'), SW_HIDE, ewWaitUntilTerminated, restoreKaliteFolder);
            end;

            if DirExists(ExpandConstant('{tmp}')+'\ka-lite\content') then
            begin
                Exec(ExpandConstant('{cmd}'),'/C mkdir ka-lite\content & xcopy /y /s '+ExpandConstant('{tmp}')+'\ka-lite\content\* ka-lite\content', ExpandConstant('{app}'), SW_HIDE, ewWaitUntilTerminated, restoreContentFolder);
            end;
        end;
        
        if Not existDatabase then
        begin
            if FileExists(ExpandConstant('{app}')+'\ka-lite\kalite\database\data.sqlite') then
            begin
                if MsgBox('A database file from a previous installation already exists; do you want to keep the old data and upgrade your install?', mbInformation,  MB_YESNO or MB_DEFBUTTON1) = IDYES then
                begin
                    existDatabase := True;
                    MsgBox('The data that you have entered during setup will be discarded in order to proceed with the update. Your version of KA Lite is using a different structure, we will have to uninstall it in order to update properly. Please be patient.', mbInformation, MB_OK);
                    ShellExec('open', ExpandConstant('{app}')+'\unins000.exe', '/SILENT /VERYSILENT /SUPPRESSMSGBOXES', '', SW_SHOWNORMAL, ewWaitUntilTerminated, uninstallError);
                end
                else if MsgBox('Installing fresh will delete all your own data; do you really want to do this?', mbInformation,  MB_YESNO or MB_DEFBUTTON2) = IDYES then
                begin
                    existDatabase := False;
                    if Not DeleteFile(ExpandConstant('{app}') + '\ka-lite\kalite\database\data.sqlite') then
                    begin
                        MsgBox('Error' #13#13 'Failed to delete Django database; continuing install.', mbError, MB_OK);
                    end;
                end;
            end;
        end;
        
    end;

    if CurStep = ssPostInstall then
    begin
        if installFlag then
        begin
            setupCommand := 'manage.py setup --noinput -o "'+ServerInformationPage.Values[0]+'" -d "'+ServerInformationPage.Values[1]+'" -u "'+UserInformationPage.Values[0]+'" -p "'+UserInformationPage.Values[1]+'"';
            if existDatabase then 
            begin
                setupCommand := setupCommand + ' --noinput';
            end;
            
            MsgBox('Setup will now configure the database. This operation may take a few minutes. Please be patient.', mbInformation, MB_OK);
      
            if Not ShellExec('open', 'python.exe', setupCommand, ExpandConstant('{app}')+'\ka-lite\kalite', SW_HIDE, ewWaitUntilTerminated, ServerNameDescriptionCode) then
            begin
                MsgBox('Error' #13#13 'Failed to initialize database.', mbInformation, MB_OK);
            end;    
      
            if Not ShellExec('open', 'python.exe', 'manage.py videoscan', ExpandConstant('{app}')+'\ka-lite\kalite', SW_HIDE, ewWaitUntilTerminated, ServerNameDescriptionCode) then
            begin
                MsgBox('Error' #13#13 'Failed to scan video files.', mbInformation, MB_OK);
            end;    
      
            if StartupPage.SelectedValueIndex = 0 then
            begin
                if ShellExec('open','guitools.vbs','0',ExpandConstant('{app}'), SW_HIDE, ewWaitUntilTerminated, StartupCode) then
                begin
                    if Not SaveStringToFile(ExpandConstant('{app}')+'\CONFIG.dat', 'RUN_AT_STARTUP:TRUE;' + #13#10, False) then
                    begin
                        MsgBox('File Error' #13#13 'Failed to create config file.', mbError, MB_OK);
                    end;
                end
                else begin
                    MsgBox('Startup Error' #13#13 'Failed to add into startup folder.', mbError, MB_OK);
                end;      
            end;      
        end;
    end;
    
end;
