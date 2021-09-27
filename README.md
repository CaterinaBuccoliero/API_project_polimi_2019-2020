#Algorithms and data structures project 2019/2020, Polimi 
##edU, or ed with multiple Undo
The projects consists in implementing a text editor. The editor considers a document as a sequence of lines, numbered starting from one.  
The UI is a textual interface based on commands.  
Every command ends in a new line character.  
A command can be followed by a text portion (one or more lines) ending with a '.' (dot) character as the only character on the following line.  
Every command is made of a single letter, optionally forerun by one or two integers which may represent an address specifier.  
The first line of the text has address one.
##Commands
| Command | What it does |
|---------|--------------|
|(ind1,ind2)c|Changes the text from line ind1 to line ind2 (ind1 and ind2 included). The text following this command must have ind2 - ind1 + 1 lines. Ind1 must be the address of an already existing line of text, or the first address after the last line of text|
|(ind1,ind2)d|Deletes all lines of text from ind1 to ind2 (ind1 and ind2 included), shifting upwards all lines following ind2. Deleting a non existing line has no effect|
|(ind1,ind2)p|Prints all lines from ind1 to ind2 (ind1 and ind2 included). If a non existing line is to be printed, a line containing '.' as its only character followed by a new line is printed|
|(integer)u|Undoes (integer) amount of commands (c or d). (integer) must be positive.|
|(integer)r|Undoes (integer) amount of undos|
|q|Quits the editor|

A line of text can contain a max of 1024 characters.  
All commands fed into the editor are assumed to be correct.
