" Vim syntax file for Bisquit
" Enables syntax higlighting for the Bisquit programming language.
"
" Installation: 
" Vim: Move this file to your ~/.vim/syntax/ directory, then
" add the following to yor vim.rc:
" `au BufRead,BufNewFile *.stc set filetype=cel`
"
" NeoVim: Move this file to your ~/.nvim/syntax/ directory, then
" add the following to yor init.vim:
" `au BufRead,BufNewFile *.stc set filetype=cel`
"
" For further alternatives:
" https://vim.fandom.com/wiki/Creating_your_own_syntax_files

if exists("b:current_syntax")
	finish
endif

" Keywords
syn keyword bisqKeyword PRINT ASSIGN ADD SUB MUL DIV
syn keyword bisqKeyword EXIT GOTO JUMP STRIN NUMIN
syn keyword bisqKeyword RAND EQUAL
syn keyword bisqKeyword print assign add sub mul div
syn keyword bisqKeyword exit goto jump strin numin
syn keyword bisqKeyword rand equal

" String literal 
syntax region bisqString start=/\v"/ skip=/\v\\./ end=/\v"/
" Integral number
syn match bisqDecimalLiteral	'[-+]\?\d\+'
" Floating point number with decimal
syn match bisqDecimalLiteral	'[-+]\?\d\+\.\d*'
" Variable
syn match bisqVariable			'[_A-Za-z][A-Za-z\d_]*'
" Comments
syn match bisqComment			'#.*\n'

let b:current_syntax = "bisquit"

" hi def link bisqDecimalLiteral 	Constant
hi def link bisqKeyword			Operator
hi def link bisqComment			Comment
hi def link bisqString			Constant
hi def link bisqVariable		Identifier
