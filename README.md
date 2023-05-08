## Qt project: file Indexer Using a Lexer
### Authors : Lucas Girard et Joan Glendinning

How to launch : You may select the path of the directory to index by running the command 
in the interface INDEXER ADD WHITELIST <folder_name>
e.g : "INDEXER ADD WHITELIST C:/fakefolder" and pressing the search button

You may then choose among the options :


# IMPORTANT
Some of task are uncompleted/perfectible due to short time to complete the project
SYNTAX IS CASE SENSITIVE.
Some syntax are slighty different from doc, please consider reading the readme if you have trouble with execution of commands.

## INDEXING SYNTAX 

**INDEXER <STATUS|START|STOP|PAUSE|RESUME>**

**INDEXER START** : will start indexing the files in the designated location. The progress bar is displayed below. The files info (name, size etc.. ) 
are then stored in a qtdb.db file located in the project's latetst build folder. qtdb.db should be deleted and recreated if changes are made. 

**INDEXER STOP** : Stops indexing files

**INDEXER PAUSE** : Pauses indexing files

**INDEXER RESUME** : Resumes indexing files after pause command

**INDEXER STATUS** : will return one of 6 options available (indexing, ready, stopped, paused, querying, resultes_available...) 


## GET ADD PUSH

**GET <WHITELIST|BLACKLIST|FILTERS|SKIPPED_FILTERS>** : No problem with this command
**ADD <WHITELIST|BLACKLIST|FILTERS|SKIPPED_FILTERS> <folder to add>** : No problem with this command
**PUSH <WHITELIST|BLACKLIST|FILTERS|SKIPPED_FILTERS> <folders to add> DONE** : No problem with this command
** CLEAR <WHITELIST|BLACKLIST|FILTERS|SKIPPED_FILTERS> ** : No problem with this command
  
  
## QUERYING
  
  ** SEARCH <filename_part> [OPTIONS] **
  Options
LAST_MODIFIED <date_spec>
CREATED <date_spec>
MAX_SIZE <size_spec>
MIN_SIZE <size_spec>
SIZE <size_spec|size_spec_ex>
EXT <list_of_extensions>
TYPE <type_list_spec>
 ** date_spec : **
  dd/mm/yyyy
  mm/yyyy
  yyyy
  yy
  
  
  size_spec:
<number><K|M|G>
  
  
  size_spec_ex :
BETWEEN <size_spec> AND <size_spec>
  
  ext_list_spec :
<ext>, <ext>, ….
<ext> OR <ext> OR ….
type_list_spec :
<type>, <type>, ….
<type> OR <type> OR ….
type:
image : jpg,png,gif, ….
text: txt, html,.....
exe: exe, bat, sh
  
  
  
Example
SEARCH testme please
LAST_MODIFIED BETWEEN 2 days and 3 days
CREATED 31/12/2020
MAX_SIZE 10M
MIN_SIZE 1M
SIZE BETWEEN 10M AND 20M
EXT txt,doc,xlsx
TYPE image OR text

  
**You may easily view the .db file on https://inloop.github.io/sqlite-viewer/ **

for example : 
![image](https://user-images.githubusercontent.com/41628418/236674134-9c5aaf36-bada-477d-8eda-d06a06a16cde.png)

