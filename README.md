# Devtext  - A text editor in C language using NCURSES

## Requires NCURSES to work.

## Name: Devashish Gaikwad

## MIS : 111608023


>Current Progress:

>arrow keys work

>HOME, END work

>enter works

>backspace works

>save works

>pageup, pagedown work

>search, replace work

>copy-paste and cut-paste work

>Text can now change color



### Devtext has the following Features:

* Open, Read, Write text file

* Runs in Terminal

* Accepts Command Line Arguments

* Use of Curser Keys to Navigate 

* Home and End

* Find and Replace

* Pageup and Pagedown

* Cut-Paste and Copy-Paste

_ _ _ _ _ _ _ _




### Description:


* Devtext is a Text editor made with NCURSES library for Terminal UI.

* It uses Linked list of lines as buffer to store the document line by line.

* The data structure is called as "buffer", which contains another structure "line".

* Declaring many buffers in the same program is possible (possibly for editing two text files at the same time).

* buffer.c contains all the functions necessary for using buffer data structure.

* gui_ncs.c contains a few Terminal UI functions for rendering the screen.

* NCURSES does not support scrolling with default provided WINDOW data structure(at least not in the modern sense).

* so a workaround has been used (loadwin function).

* The current maximum line width is 160 characters, It can be changed in buffer.h (a constant has been defined in it).

* Window resizing is not supported by NCURSES, thus it is not possible to implement.






_ _ _ _ _ _ _ _ _


<a href="https://fontmeme.com/calligraphy-fonts/"><img src="https://fontmeme.com/permalink/170905/15005e96d12d8bb9c5bf92460ba2bdf2.png" alt="calligraphy-fonts" border="0"></a>

>Developed by  Devashish Gaikwad : COEP IT : 111608023
