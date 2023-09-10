#include "helper.h"
#include "library.h"
#include "defs.h"
#include <string.h>
#include <sqlite3.h>

extern int initLibrary();
extern int openDatabase();
extern int closeDatabase();
extern int createDatabase();
extern int createBook(Book *book);
extern int createBorrow(Borrow *borrow);
extern int deleteBook(int bookID);
extern int fetchBook(Book **buffer, int pageSize);

Book **bookDataBuffer;
Borrow **borrowDataBuffer;

sqlite3 *dataBase;
char databasePath[] = DATABASE_FILE_PATH;

int testCreate(){
  Book myBook;

  for(int i = 0;i < 50;++i){

    myBook.bookID = 0;
    myBook.borrowID = 0;
    myBook.title = "lol";
    myBook.libraryID = "awo";

    createBook(&myBook);
  }

  deleteBook(20);

  return 0;
}

int testFetch(){
  Book *myBook;
  myBook = (Book *)malloc(100*sizeof(Book));
  fetchBook(&myBook, 20);

  printf("here: %p\n", myBook);

  for(int i = 0;i < 20;++i){
    printf("ID: %d\n", myBook[i].bookID);
    printf("borrowID: %d\n", myBook[i].borrowID);
    printf("title: %s\n", myBook[i].title);
  }

  free(myBook);

  return 0;
}

int initLibrary(){
  int retval;
  retval = openDatabase();
  CHECK(retval != 0, "Error in openDatabase()", return -1);

  return 0;
}

int closeLibrary(){
  int retval;
  retval = closeDatabase();
  CHECK(retval != 0, "Error in closeDatabase()", return -1) ;

  return 0;
}

int openDatabase(){
  int retval = sqlite3_open(databasePath, &dataBase);
  CHECK(retval != SQLITE_OK, "Unable to open database", return -1);
  return 0;
}

int closeDatabase(){
  int retval = sqlite3_close(dataBase);
  CHECK(retval != SQLITE_OK, "Problem closing database", return -1);
  return 0;
}

int createDatabase(){
  char *sqlErrmsg;
  int retval;

  // Create file
  FILE *filePtr = fopen(databasePath, "w");
  CHECK(filePtr == NULL, "Failed to create database", return -1);
  fclose(filePtr);

  const char *createTableBooks = "CREATE TABLE IF NOT EXISTS " DATABASE_BOOKTABLE_NAME " ("
                            "bookID INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "borrowID INTEGER, "
                            "title TEXT NOT NULL, "
                            "libraryID TEXT"
                            ");"
  ;

  const char *createTableBorrow = "CREATE TABLE IF NOT EXISTS " DATABASE_BORROWTABLE_NAME " ("
                            "borrowID INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "bookID INTEGER NOT NULL,"
                            "status INTEGER NOT NULL,"
                            "startTimestamp INTEGER,"
                            "endTimestamp INTEGER,"
                            "name TEXT NOT NULL,"
                            "classSequence TEXT"
                            ");"
  ;

  // Execute
  retval = sqlite3_exec(dataBase, createTableBooks, 0, 0, &sqlErrmsg);
  CHECK(retval != SQLITE_OK, sqlErrmsg, return -1);

  retval = sqlite3_exec(dataBase, createTableBorrow, 0, 0, &sqlErrmsg);
  CHECK(retval != SQLITE_OK, sqlErrmsg, return -1);

  return 0;
}

int createBook(Book *book){
  int retval = 0;
  sqlite3_stmt* stmt;
  const char* sqlInsert = 
      "INSERT INTO " DATABASE_BOOKTABLE_NAME 
      " (borrowID, title, libraryID)"
      " VALUES (?, ?, ?)";

  // Prepare
  retval |= sqlite3_prepare_v2(dataBase, sqlInsert, -1, &stmt, NULL);

  retval |= sqlite3_bind_int(stmt, 1, book->borrowID);
  retval |= sqlite3_bind_text(stmt, 2, book->title, -1, SQLITE_STATIC);
  retval |= sqlite3_bind_text(stmt, 3, book->libraryID, -1, SQLITE_STATIC);
  CHECK(retval != SQLITE_OK, sqlite3_errmsg(dataBase), return -1);

  // Execute
  retval = sqlite3_step(stmt);
  CHECK(retval != SQLITE_DONE, sqlite3_errmsg(dataBase), return -1);

  // Finalize
  sqlite3_finalize(stmt);

  return 0;
}

int deleteBook(int bookID){
  int retval = 0;
  sqlite3_stmt* stmt;
  const char* sqlInsert = "DELETE FROM " DATABASE_BOOKTABLE_NAME " WHERE bookID = ?";

  // Prepare
  retval |= sqlite3_prepare_v2(dataBase, sqlInsert, -1, &stmt, NULL);

  retval |= sqlite3_bind_int(stmt, 1, bookID);
  CHECK(retval != SQLITE_OK, sqlite3_errmsg(dataBase), return -1);

  // Execute
  retval = sqlite3_step(stmt);
  CHECK(retval != SQLITE_DONE, sqlite3_errmsg(dataBase), return -1);

  // Finalize
  sqlite3_finalize(stmt);

  return 0;
}

// TODO create buffer to put text in
int fetchBook(Book **buffer, int pageSize){
  int retval = 0;
  int offset = 0;
  static char sqlQuery[128];
  snprintf(sqlQuery, sizeof(sqlQuery), 
           "SELECT bookID, borrowID, title, libraryID FROM " DATABASE_BOOKTABLE_NAME " LIMIT %d OFFSET %d",
           pageSize, offset)
  ;

  // Prepare the query
  sqlite3_stmt *stmt;
  retval |= sqlite3_prepare_v2(dataBase, sqlQuery, -1, &stmt, 0);
  CHECK(retval != SQLITE_OK, sqlite3_errmsg(dataBase), return -1);

  // Execute
  int i = 0;
  retval = sqlite3_step(stmt);
  while(retval == SQLITE_ROW){
    (*buffer)[i].bookID = sqlite3_column_int(stmt, 0);
    (*buffer)[i].borrowID = sqlite3_column_int(stmt, 1);
    // TODO: Seems a little sussy to me
    strcat((*buffer)[i].title, sqlite3_column_text(stmt, 2));
    strcat((*buffer)[i].libraryID, sqlite3_column_text(stmt, 3));
    // TODO: Seems a little sussy to me
    retval = sqlite3_step(stmt);
    ++i;
  }

  // Finalize the statement
  sqlite3_finalize(stmt);
  return 0;
}

int createBorrow(Borrow *borrow){
  int retval = 0;
  sqlite3_stmt* stmt;
  const char* sqlInsert = 
      "INSERT INTO " DATABASE_BORROWTABLE_NAME  
      " (bookID, status, startTimestamp, endTimestamp, name, classSequence)" 
      " VALUES (?, ?, ?, ?, ?, ?)";

  // Prepare
  retval |= sqlite3_prepare_v2(dataBase, sqlInsert, -1, &stmt, NULL);

  retval |= sqlite3_bind_int(stmt, 1, borrow->bookID);
  retval |= sqlite3_bind_int(stmt, 2, borrow->status);
  retval |= sqlite3_bind_int(stmt, 3, borrow->startTimestamp);
  retval |= sqlite3_bind_int(stmt, 4, borrow->endTimestamp);
  retval |= sqlite3_bind_text(stmt, 5, borrow->name, -1, SQLITE_STATIC);
  retval |= sqlite3_bind_text(stmt, 6, borrow->classSequence, -1, SQLITE_STATIC);
  CHECK(retval != SQLITE_OK, sqlite3_errmsg(dataBase), return -1);

  // Execute
  retval = sqlite3_step(stmt);
  CHECK(retval != SQLITE_DONE, sqlite3_errmsg(dataBase), return -1);

  // Finalize
  sqlite3_finalize(stmt);

  return 0;
}
