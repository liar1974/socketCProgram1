a.Full Name:Chenchen Yang

b.The pdf is the description of homework

c.In the assignment,I have done Phase 1, Phase 2 and Phase3. In Phase3 I use fork function in Library1.c Library2.c and Library3.c and I make the makefile.

d.My code files are Library1.c Library2.c Library3.c Database.c User1.c User2.c.
  Library1.c will set up UDP connection with Database.c and Library1.c will read the book title from library1.txt and send them to Database.In addition, Library1.c will set up TCP connection with User1.c and User2.c. User1.c and User2.c will choose the suitable library to ask Library1.c Library2.c and Library3.c for the description of each book.Library2.c and Library3.c have the same function with Library1.c.Database.c will set up UDP connection with Library1.c Library2.c and Library3.c to store the book titles coming from different libraries.Then Database.c will set up UDP connection with User1.c and User2.c to communicate with each other. User1.c and User2.c will read the book title from queries1.txt and queries2.txt and send them to Database.c.The Database.c will find the location of each book titles and send the location of each book to User1.c and User2.c in order to make the User1.c and User2.c to choose the suitable libraries to set up TCP connection to ask for description.

e.TA should open 5 X-win32. One is to execute Library1.c. One is to execute Library2.c. One is to execute Library3.c. One is to execute Database.c. One is to execute User1.c and User2.c.TA should run Database.c firstly,run Library1.c secondly,run Library2.c thirdly,run Library3.c fourthly. In the end TA can run User1.c and User2.c

f.All the book tile and book description will be stored in array.

g.TA should run Library1.c Library2.c and Library3.c in order. If TA run Libray1.c Library2.c and Library3.c in any order, it will have some mistakes.In every input file, library1.txt,library2.txt,library3.txt,queries1.txt and queries2.txt should use enter button at the end of last line.

h.Reused Code: Some parts of codes come from Beej's Book.The code of getting the dynamic port number in User and Library comes from TA's example.