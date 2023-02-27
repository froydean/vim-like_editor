//In write mode there is no events for home&end keys, because Esc in this project = HOME key.
//In this project only .txt files are supposed.
//Search_word can't be more than max_x (even not tracking if more)
//Seacrh finds only word, not the text
//In Search and Command mode status bar dissapeears for the input. Doesn't support scroll in this bar.
//Tab and other non one-literal symbols are not supposed
//Word deletion deletes ' ' or '\n' on the right 
//Page up and Page down for the frist time lead to the end/start of the page
//String deletion do not act if tis is frisrt line and it is empty

//Searches imlementation may be suck, could use vector for searching one time and then return index of wanted.
//View::print_command_screen() same as search and also bad decision
//May be more easier if actual lines and lines correlation
//MyString class do not throw exeption but print "Index is out of range"
//Calculation of actual lines, position may be in one calculation. not separated
//May be easier if actual lines, postion would be 0..28
//Const methods, private, public inherit are not checked.
//Code could consist repeated code
//Line showing code ugly
//Command parsing not perfect :over opens "ver" file