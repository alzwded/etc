" Vim syntax file
" Language: vlad's note taking
" Version: 1.0
"
" How it looks like:
" # comment
" ::superbold::
" ! Subtitle 1 .
"     a definition := talk talk talk talk ; talk talk talk
"     some other notes %but this is very important% <- really
" ! important note !


set syntax=

syn match NOTES_spacharacters "[*+-;:]"
syn match NOTES_connectingChars "[|&^@]"
syn match NOTES_def "^.*[:][=]"
syn region NOTES_bolded matchgroup=NOTES_bolded start="!" end="[!.]"
syn region NOTES_header matchgroup=NOTES_header start="::" end="::"
syn region NOTES_important matchgroup=NOTES_important start="%" end="%"
syn match NOTES_note "[<][-]"
syn match NOTES_emdash "[-][-]"
syn match NOTES_results "[=-][>]"
syn match NOTES_parentheses "[(){}\[\]]"
syn region NOTES_header2 matchgroup=NOTES_header2 start="[#]" end="$"

highlight link NOTES_spacharacters Operator
highlight link NOTES_def Identifier
highlight link NOTES_bolded String
hi link NOTES_header Statement
hi link NOTES_header2 Comment
hi link NOTES_important Structure
hi link NOTES_note Structure
hi link NOTES_emdash Structure
hi link NOTES_results Structure
hi link NOTES_parentheses Structure
hi link NOTES_connectingChars Structure
