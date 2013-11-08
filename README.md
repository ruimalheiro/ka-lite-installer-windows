KA Lite Windows
==========

This project provides a smoother way to install and run KA Lite in a Windows Machine.

---
#### This project was built using the following software:
* Inno Setup 5.5.3
* Inno Script Studio 1.0.0.24
* Microsoft Visual Studio Express 2012

---
#### Instructions to build "KALiteSetup.exe":
* Clone this repository;
* Run "make.vbs" and wait until the file is built;
* The output file named "KALiteSetup.exe" will appear within this project folder.

---
##### To clone this repository, run the following line: 
    git clone --recursive https://github.com/learningequality/ka-lite-windows.git
###### (the `--recursive` is required due to the `ka-lite` submodule)

---
##### If you wish to build it using Wine, run the following line:
    wine inno-compiler/ISCC.exe installer-source/KaliteSetupScript.iss