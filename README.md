## Qt project: file Indexer
### Authors : Lucas Girard et Joan Glendinning

How to launch : You may select the path of the directory to index by running the command 
in the interface INDEXER ADD WHITELIST <folder_name>
e.g : "INDEXER ADD WHITELIST C:/fakefolder" and pressing the search button

You may then choose among the options :

**INDEXER <STATUS|START|STOP|PAUSE|RESUME>**

**INDEXER START** : will start indexing the files in the designated location. The progress bar is displayed below. The files info (name, size etc.. ) 
are then stored in a qtdb.db file located in the project's latetst build folder. qtdb.db should be deleted and recreated if changes are made. 

**INDEXER STOP** : Stops indexing files

**INDEXER PAUSE** : Pauses indexing files

**INDEXER RESUME** : Resumes indexing files after pause command

**INDEXER STATUS** : will return one of 6 options available (indexing, ready, stopped, paused, querying, resultes_available...) 

You may easily view the .db file on https://inloop.github.io/sqlite-viewer/ 

for example : 
![image](https://user-images.githubusercontent.com/41628418/236674134-9c5aaf36-bada-477d-8eda-d06a06a16cde.png)

