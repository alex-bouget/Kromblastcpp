# Kromblastcpp

Use native functions of operating system in a website.

## Dependencies

- [Mini](libs/Mini.h) - C++ library who decode ini files. [original repository](https://github.com/pulzed/mINI/)
- [webview](libs/webview/webview.h) - C++ library for create a webview. [original repository](https://github.com/webview/webview)


## How to use

- Create a file named `kromblast.ini`
- he file must contain the following lines:

```ini
[Kromblast]
; optional
; default: false
; for debuging the webview
; add multiple print and the inspector
debug=false

; required
; folder where the library is located compared to current folder (where the ini file is)
lib="kromlib"

; required
; mode of webview file
; local | server | hosted
; local: load the file from the local file system
; server: load the file from a server
; hosted: open a server and open the index - NOT WORKING FOR THE MOMENT
mode="server"

; required
; path of the file or the server
path="http://github/Github/Kromblastcpp/test/"


[Window]
; required
; title of the window
title="Kromblast"

; required
; width of the window
width=800

; required
; height of the window
height=600

; optional
; default: false
; if the window is in fullscreen
fullscreen=false

; optional
; default: false
; if the window is frameless
frameless=false
```

for this exemple you have:
```
├── kromblast.ini
└── kromlib
   ├── lib1.[so|dll|dylib]
   └── lib2.[so|dll|dylib]
```