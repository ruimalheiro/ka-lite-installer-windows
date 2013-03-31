Set oShell = WScript.CreateObject("WSCript.shell")
oShell.run "cmd /K inno-compiler\ISCC.exe /cc installer-source\KaliteSetupScript.iss & exit"