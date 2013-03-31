Dim value : value = WScript.Arguments(0)
Set oShell = CreateObject("WScript.Shell")
sPath = oShell.SpecialFolders("Startup")

If value = 0 Then
    
    Set oShortcut = oShell.CreateShortcut(sPath & "\KA Lite.lnk")
    oShortcut.TargetPath = oShell.CurrentDirectory & "\KA Lite.exe"
    oShortcut.WorkingDirectory = oShell.CurrentDirectory
    oShortcut.Save
    
ElseIf value = 1 Then

    Set objFSO = CreateObject("Scripting.FileSystemObject")
    If objFSO.FileExists(sPath & "\KA Lite.lnk") Then
        objFSO.DeleteFile(sPath & "\KA Lite.lnk")
    End if
    
ElseIf value = 2 Then

    Set objFSO = CreateObject("Scripting.FileSystemObject")
    If objFSO.FileExists(sPath & "\KA Lite.lnk") Then
        WScript.Quit(0)
    End if
    WScript.Quit(1)
End If

WScript.Quit(2)




