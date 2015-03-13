KA Lite Installer for Windows
==========

This project provides a smoother way to install and run KA Lite in a Windows Machine.

---
#### This project was built using the following software:
* Inno Setup 5.5.3 [Download] (http://files.jrsoftware.org/is/5/)
* Microsoft Visual Studio Express 2012 [Download] (https://www.microsoft.com/en-us/download/details.aspx?id=34673)
* Git (note: install with the option to place the `git` executable in the path, so it can be run within `cmd`)

---
#### Instructions to update Microsoft Visual Studio 2012
###### Steps to update:
* Click on TOOLS menu
* Select Extensions and Updates... then another dialog will appear.
* Click on Update.

###### Install the downloaded update in your machine
* Click on BUILD.
* Select Build Solution.

---
#### Instructions to build "KALiteSetup.exe" in Windows:
The build process doesn't work reliably in Windows. Build on linux using wine instead.
* ~~Clone this repository;~~
* ~~Open `cmd` -- the Windows command prompt;~~
* ~~Run `git submodule update --init`~~
* ~~Run `make.vbs` and wait until the file is built;~~
* ~~The output file named "KALiteSetup.exe" will appear within this project folder.~~

---
##### To clone this repository, run the following line:
    git clone --recursive https://github.com/learningequality/ka-lite-windows.git
###### (the `--recursive` is required due to the `ka-lite` submodule)

---
##### Instructions to build "KALiteSetup.exe" in Linux:
First, install `wine`. Then in the base directory run the following commands:
```bash
> echo "0.13.0" > installer-source/version.temp
> wine inno-compiler/ISCC.exe installer-source/KaliteSetupScript.iss
```
