'This script sets a link at the windows startup folder for KA Lite. The options are 0, 1 and 2.

Dim value : value = WScript.Arguments(0)
Set oShell = CreateObject("WScript.Shell")
sPath = oShell.SpecialFolders("Startup")

'Option 0 is to create the startup link at the windows startup folder.
If value = 0 Then
    
    Set oShortcut = oShell.CreateShortcut(sPath & "\KA Lite.lnk")
    oShortcut.TargetPath = oShell.CurrentDirectory & "\KA Lite.exe"
    oShortcut.WorkingDirectory = oShell.CurrentDirectory
    oShortcut.Save
   
'Option 1 is to delete the startup link at the window startup folder.   
ElseIf value = 1 Then

    Set objFSO = CreateObject("Scripting.FileSystemObject")
    If objFSO.FileExists(sPath & "\KA Lite.lnk") Then
        objFSO.DeleteFile(sPath & "\KA Lite.lnk")
    End if
    
'Option 2 is to check if the link exists and the windows startup folder. If it exists, the script return 0. It will return 1 if it doesn't exist.
ElseIf value = 2 Then

    Set objFSO = CreateObject("Scripting.FileSystemObject")
    If objFSO.FileExists(sPath & "\KA Lite.lnk") Then
        WScript.Quit(0)
    End if
    WScript.Quit(1)
    
End If

WScript.Quit(2)




