; goal: prevent programmers from copy pasting
; 1) if in Visual Studio, don't allow ^V
; 2) else, if clipboard has *.cxx, *.ja, *.c, *.h, *.hxx etc, don't allow ^V

#if WinActive("ahk_exe devenv.exe")
^v::
MsgBox, No copy pasting code!
return

#if
$^v::
text:=Clipboard
IF RegExMatch(text, "(.*\.c|.*\.c[a-zA-Z][a-zA-Z]|.*\.h|.*\.[a-zA-Z][a-zA-Z]|.*\.ja|.*\.py|.*\.def)\b") {
   Msgbox, No copy pasting source files! such as %text%
} else {
   SendInput, ^v
   ;SendInput,{Ctrl}
}
return
