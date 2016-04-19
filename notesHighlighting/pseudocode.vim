" Vim syntax file
" Language: vlad's pseudo code
" Version: 1.0
"
" How it looks like:

set syntax=

syn match PCODE_spacharacters "[%*+-:/.,!<=>&|~^\\]"
syn keyword PCODE_def let for each in if else return yield set call to by while do repeat until loop next continue break exit new delete malloc free cmp drop end func function sub void pointer int integer float complex shared unique char string goto gosub from push pop add tail head insert into update mutable const map grep where not null as object struct class ordered unordered vector array list register stop jump of var input output global static extern port socket slot bind wrap value unwrap release ref reference address deref unref defined undefined undef define then
syn match PCODE_results "[=-][>]"
syn match PCODE_parentheses "[(){}[\]]"
syn region PCODE_comment matchgroup=PCODE_comment start="[;]" end=/\n/
syn region PCODE_header2 matchgroup=PCODE_header2 start="[#]" end=/\n/
syn region PCODE_string1 matchgroup=PCODE_string start=/\v'/ skip=/\v\\./ end=/\v'/
syn region PCODE_string2 matchgroup=PCODE_string start=/\v"/ skip=/\v\\./ end=/\v"/

highlight link PCODE_spacharacters Operator
highlight link PCODE_def Keyword
highlight link PCODE_header2 String
highlight link PCODE_string String
highlight link PCODE_string1 String
highlight link PCODE_string2 String
hi link PCODE_header2 Statement
hi link PCODE_results Structure
hi link PCODE_parentheses Structure
hi link PCODE_comment Comment
