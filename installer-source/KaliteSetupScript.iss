#define MyAppName "KA Lite"
#define MyAppPublisher "Foundation for Learning Equality"
#define MyAppURL "http://learningequality.org/"
#define MyAppExeName "KA Lite.exe"

#define rc = Exec("getversion.bat")
#define FileHandle = FileOpen("version.dat");
#define MyVersion = StringChange(FileRead(FileHandle), " ", "");
#if FileHandle
  #expr FileClose(FileHandle)
#endif

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{75DA4F44-7F9C-4AD4-95CF-DF3BDD7F95B4}
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
OutputBaseFilename=KALiteSetup
SetupIconFile=logo48.ico
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\ka-lite\*"; DestDir: "{app}\ka-lite"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\ka-lite\content\*"; DestDir: "{app}\ka-lite\content"; Flags: ignoreversion recursesubdirs createallsubdirs uninsneveruninstall
Source: "..\ka-lite\kalite\database\*"; DestDir: "{app}\ka-lite\kalite\database"; Flags: ignoreversion recursesubdirs createallsubdirs uninsneveruninstall
Source: "..\gui-packed\KA Lite.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\gui-packed\winshortcut.vbs"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\python-setup\*"; DestDir: "{app}\python-setup"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
;Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "Uninstall KA Lite"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: shellexec postinstall skipifsilent

[Dirs]
Name: "{app}\"; Permissions: everyone-modify

[UninstallDelete]
Type: filesandordirs; Name: "{app}\ka-lite\python-packages"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\config"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\faq"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\main"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\securesync"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\utils"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\central"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\registration"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\static"
Type: filesandordirs; Name: "{app}\ka-lite\kalite\templates"
Type: Files; Name: "{app}\ka-lite\kalite\*.pyc"
Type: Files; Name: "{userstartup}\KA Lite.lnk"
Type: Files; Name: "{app}\CONFIG.dat"

[code]
var
  installFlag : boolean;
  startupFlag : String;
  UserPage: TInputQueryWizardPage;
  UsagePage: TInputOptionWizardPage;
  
procedure InitializeWizard;
begin

  UserPage := CreateInputQueryPage(wpLicense,
    'Server Information', 'General data',
    'Please specify the server name and a description, then click Next. (you can leave blank both fields if you want to use the default server name or if you do not want to insert a description)');
  UserPage.Add('Server name:', False);
  UserPage.Add('Server description:', False);
  
  UsagePage := CreateInputOptionPage(UserPage.ID,
    'Server Configuration', 'Startup configuration',
    'Do you wish to set the KA Lite server to run in the background automatically when you start Windows?',
    True, False);
  UsagePage.Add('Yes');
  UsagePage.Add('No');
  UsagePage.SelectedValueIndex := 0;
  
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
  
  if RegDeleteValue(HKCU, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Run', ExpandConstant('{#MyAppName}')) then
  begin
    
  end;
   
  
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
      end
      else begin
         //MsgBox('Error' #13#13 'You must upgrade your Python version after the installation to run KA Lite.', mbInformation, MB_OK);
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
   result := True;
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  MigrateCode: integer;
  SuperUserCreate: integer;
  GenerateKeysCode: integer;
  ServerNameDescriptionCode: integer;
  StartupCode: integer;
  existDatabase : boolean;
  createSuper : boolean;
  moveKaliteFolderTemp: integer;
  moveContentFolderTemp: integer;
  cleanKaliteFolder: integer;
  restoreKaliteFolder: integer;
  restoreContentFolder: integer;
  informationBoxFlagged: boolean;
begin
  
  existDatabase := False;
  createSuper := False;
  informationBoxFlagged :=False;
  
  if(CurStep=ssInstall) then
  begin
    
    if(DirExists(ExpandConstant('{app}')+'\kalite')) then
    begin
      MsgBox('KA Lite old data structure' #13#13 'Setup detected that you have the old file structure. Setup will now move data to update the structure. Please be patient; this may take some time.', mbInformation, MB_OK);
      informationBoxFlagged :=True;      
      if Exec(ExpandConstant('{cmd}'),'/C mkdir '+ExpandConstant('{tmp}')+'\ka-lite\kalite & xcopy /y /s kalite\* '+ExpandConstant('{tmp}')+'\ka-lite\kalite', ExpandConstant('{app}'), SW_HIDE, ewWaitUntilTerminated, moveKaliteFolderTemp) then
      begin
      end;      
    end; 
      
    if(DirExists(ExpandConstant('{app}')+'\content')) then
    begin
      if not informationBoxFlagged then
      begin
        MsgBox('KA Lite old data structure' #13#13 'Setup detected that you have the old file structure. Setup will now move data to update the structure. Please be patient; this may take some time.', mbInformation, MB_OK);
        informationBoxFlagged :=True;
      end;      
      if Exec(ExpandConstant('{cmd}'),'/C mkdir '+ExpandConstant('{tmp}')+'\ka-lite\content & xcopy /y /s content\* '+ExpandConstant('{tmp}')+'\ka-lite\content', ExpandConstant('{app}'), SW_HIDE, ewWaitUntilTerminated, moveContentFolderTemp) then
      begin
      end;        
    end;      
    
    if (informationBoxFlagged) then
    begin
      if Exec(ExpandConstant('{cmd}'),'/C cd .. & del /q "'+ExpandConstant('{app}')+'\*" & for /d %x in ( "'+ExpandConstant('{app}')+'\*" ) do @rd /s /q "%x"', ExpandConstant('{app}'), SW_HIDE, ewWaitUntilTerminated, cleanKaliteFolder) then
      begin
      end;
    
    
      if(DirExists(ExpandConstant('{tmp}')+'\ka-lite\kalite')) then
      begin
        if Exec(ExpandConstant('{cmd}'),'/C mkdir ka-lite\kalite & xcopy /y /s '+ExpandConstant('{tmp}')+'\ka-lite\kalite\* ka-lite\kalite', ExpandConstant('{app}'), SW_HIDE, ewWaitUntilTerminated, restoreKaliteFolder) then
        begin
        end;
      end;
            
      if(DirExists(ExpandConstant('{tmp}')+'\ka-lite\content')) then
      begin
        if Exec(ExpandConstant('{cmd}'),'/C mkdir ka-lite\content & xcopy /y /s '+ExpandConstant('{tmp}')+'\ka-lite\content\* ka-lite\content', ExpandConstant('{app}'), SW_HIDE, ewWaitUntilTerminated, restoreContentFolder) then
        begin
        end;
      end;
    end;
       
  end;
  

  if(CurStep=ssPostInstall) then
  begin
    if(installFlag) then
    begin
   
      if (FileExists(ExpandConstant('{app}')+'\ka-lite\kalite\database\data.sqlite')) then
      begin      
        existDatabase := True;        
        if MsgBox('A database file from a previous installation already exists; do you want to delete the old data and start fresh, or keep the old database?' #13#13 'Choose "Yes" to delete or "No" to keep. (keep is recommended)', mbInformation,  MB_YESNO or MB_DEFBUTTON2) = IDYES then
        begin
          if DeleteFile(ExpandConstant('{app}')+'\ka-lite\kalite\database\data.sqlite') then
          begin
            existDatabase := False;            
          end
          else begin
            MsgBox('Error' #13#13 'Failed to delete Django database.', mbError, MB_OK);
          end;  
        end
        else begin
          existDatabase := True;
        end;         
      end
      else
      begin
        // Database not found
        existDatabase := False;
      end;
    
      MsgBox('KA Lite Database' #13#13 'Setup will now prepare the database. Please be patient; this may take some time.', mbInformation, MB_OK);
    
      if ShellExec('open', 'python.exe', 'manage.py syncdb --migrate --noinput', ExpandConstant('{app}')+'\ka-lite\kalite', SW_HIDE, ewWaitUntilTerminated, MigrateCode) then
      begin
        installFlag:=false;
      end
      else begin
        MsgBox('Error' #13#13 'Failed to call Django database sync.', mbError, MB_OK);
      end;
    
      if existDatabase then
      begin
        if MsgBox('An administrator account already exists in the database. Do you want to create an additional administrator account now?', mbInformation,  MB_YESNO or MB_DEFBUTTON2) = IDYES then
        begin
          if ShellExec('open', 'python.exe', 'manage.py createsuperuser', ExpandConstant('{app}')+'\ka-lite\kalite', SW_SHOWNORMAL, ewWaitUntilTerminated, SuperUserCreate) then
          begin
          end
        else begin
          MsgBox('Error' #13#13 'Failed to create Django super user.', mbError, MB_OK);
        end;
      end
      else begin
        // Ignore
      end;
      end
      else begin
        MsgBox('KA Lite Database' #13#13 'You will now be asked to create an administrator account, which will be used to manage this server. You will need to remember the username and password, to login as an administrator to the KA Lite web interface.', mbInformation, MB_OK);
        if ShellExec('open', 'python.exe', 'manage.py createsuperuser --email=dummy@learningequality.org', ExpandConstant('{app}')+'\ka-lite\kalite', SW_SHOWNORMAL, ewWaitUntilTerminated, SuperUserCreate) then
        begin
        end
        else begin
          MsgBox('Error' #13#13 'Failed to create Django super user.', mbError, MB_OK);
        end;
      end;
  
      if ShellExec('open', 'python.exe', 'manage.py generatekeys', ExpandConstant('{app}')+'\ka-lite\kalite', SW_HIDE, ewWaitUntilTerminated, GenerateKeysCode) then
      begin
       //
      end
      else begin
         MsgBox('Error' #13#13 'Failed to call Django generatekeys.', mbInformation, MB_OK);
      end;
    
      if ShellExec('open', 'python.exe', 'manage.py initdevice '+UserPage.Values[0]+' '+UserPage.Values[1], ExpandConstant('{app}')+'\ka-lite\kalite', SW_HIDE, ewWaitUntilTerminated, ServerNameDescriptionCode) then
      begin
       //
      end
      else begin
        MsgBox('Error' #13#13 'Failed to add server name and description.', mbInformation, MB_OK);
      end;    
    
      if UsagePage.SelectedValueIndex = 0 then
      begin
        if ShellExec('open','winshortcut.vbs','0',ExpandConstant('{app}'), SW_HIDE, ewWaitUntilTerminated, StartupCode) then
        begin
          if SaveStringToFile(ExpandConstant('{app}')+'\CONFIG.dat', 'RUN_AT_STARTUP:TRUE;' + #13#10, False) then
          begin
          end
          else begin
            MsgBox('File Error' #13#13 'Failed to create config file.', mbError, MB_OK);
          end;
        end
        else begin
          MsgBox('Startup Error' #13#13 'Failed to add into startup folder.', mbError, MB_OK);
        end;      
      end
      else begin      
      end;
          
    end;
  end;
end;
