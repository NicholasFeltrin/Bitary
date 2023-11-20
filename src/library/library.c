#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "library.h"
#include "database.h"
#include "buffer.h"
#include "defs.h"
#include "helper.h"

extern int LibraryInit();
extern int LibraryClose();
extern int LibraryCreateDatabase(char *path);
extern int LibraryCreateBook(Book *book);
extern int LibraryDeleteBook(int bookID);
extern int LibraryCreateBorrow(Borrow *borrow);
extern int LibraryLoadBookChunk(Tbuffer **buffer, Scrolling scrolling);
extern int LibraryLoadBorrowChunk(Tbuffer **buffer, Scrolling scrolling);
extern int LibrarySearchBookChunk(Tbuffer **buffer, Scrolling scrolling, const char *keyword);
extern int LibrarySearchBorrowChunk(Tbuffer **buffer, Scrolling scrolling, const char *keyword);


Database *database;

BookBuffer bookBuffer;
BorrowBuffer borrowBuffer;
SearchBuffer searchBuffer;


int LibraryInit(){
  int retval;
  // Add Database (not connected)
  database = addDatabase(database_file_path);
  CHECK(database == NULL, "Error in adding database", return -1);

  // Create database
  retval = LibraryCreateDatabase(database_file_path);
  CHECK(retval != 0, "Error in creating", return -1);

  // Connect newly created database
  retval = connectDatabase(database);
  CHECK(retval != 0, "Error in oppening database", return -1);

  // Allocate buffer for a chunk of books and borrows
  bookBuffer.buffer = tbuffer_create(number_of_allocated_elements, sizeof(Book));
  CHECK(bookBuffer.buffer == NULL, "Error allocating tbuffer", return -1);

  bookBuffer.titleBuffer = sbuffer_create();
  CHECK(bookBuffer.titleBuffer == NULL, "Error allocating sbuffer", return -1);

  bookBuffer.libraryIDBuffer = sbuffer_create();
  CHECK(bookBuffer.libraryIDBuffer == NULL, "Error allocating sbuffer", return -1);

  borrowBuffer.buffer = tbuffer_create(number_of_allocated_elements, sizeof(Borrow));
  CHECK(borrowBuffer.buffer == NULL, "Error allocating tbuffer", return -1);

  borrowBuffer.nameBuffer = sbuffer_create();
  CHECK(borrowBuffer.nameBuffer == NULL, "Error allocating sbuffer", return -1);

  borrowBuffer.classSequenceBuffer = sbuffer_create();
  CHECK(borrowBuffer.classSequenceBuffer == NULL, "Error allocating sbuffer", return -1);

  return 0;
}

int LibraryClose(){
  int retval;
  retval = disconnectDatabase(database);
  CHECK(retval != 0, "Error in closeDatabase()", return -1) ;

  tbuffer_free(bookBuffer.buffer);
  sbuffer_free(bookBuffer.titleBuffer);
  sbuffer_free(bookBuffer.libraryIDBuffer);
  tbuffer_free(borrowBuffer.buffer);
  sbuffer_free(borrowBuffer.nameBuffer);
  sbuffer_free(borrowBuffer.classSequenceBuffer);

  return 0;
}

int LibraryCreateDatabase(char *path){
  char *sqlErrmsg;
  int retval;

  // Create database.db file
  FILE *filePtr = fopen(path, "w");
  CHECK(filePtr == NULL, "Failed to create database", return -1);
  fclose(filePtr);

  // Create book and borrow table
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
                            "startTimestamp BIGINT,"
                            "endTimestamp BIGINT,"
                            "name TEXT NOT NULL,"
                            "classSequence TEXT"
                            ");"
  ;

    
  retval = connectDatabase(database);
  CHECK(retval != 0, "Error in oppening database", return -1);

  // Execute
  retval = sqlite3_exec(database->database, createTableBooks, 0, 0, &sqlErrmsg);
  CHECK(retval != SQLITE_OK, sqlErrmsg, return -1);

  retval = sqlite3_exec(database->database, createTableBorrow, 0, 0, &sqlErrmsg);
  CHECK(retval != SQLITE_OK, sqlErrmsg, return -1);

  disconnectDatabase(database);
  CHECK(retval != 0, "Error in closeDatabase()", return -1) ;

  return 0;
}

int LibraryCreateBook(Book *book){
  int retval = 0;
  sqlite3_stmt* stmt;
  const char* sqlInsert = 
      "INSERT INTO " DATABASE_BOOKTABLE_NAME 
      " (borrowID, title, libraryID)"
      " VALUES (?, ?, ?)";

  // Prepare
  retval |= sqlite3_prepare_v2(database->database, sqlInsert, -1, &stmt, NULL);

  retval |= sqlite3_bind_int(stmt, 1, book->borrowID);
  retval |= sqlite3_bind_text(stmt, 2, book->title, -1, SQLITE_STATIC);
  retval |= sqlite3_bind_text(stmt, 3, book->libraryID, -1, SQLITE_STATIC);
  CHECK(retval != SQLITE_OK, sqlite3_errmsg(database->database), return -1);

  // Execute
  retval = sqlite3_step(stmt);
  CHECK(retval != SQLITE_DONE, sqlite3_errmsg(database->database), return -1);

  // Finalize
  sqlite3_finalize(stmt);

  return 0;
}

int LibraryDeleteBook(int bookID){
  int retval = 0;
  sqlite3_stmt* stmt;
  const char* sqlInsert = "DELETE FROM " DATABASE_BOOKTABLE_NAME " WHERE bookID = ?";

  // Prepare
  retval |= sqlite3_prepare_v2(database->database, sqlInsert, -1, &stmt, NULL);

  retval |= sqlite3_bind_int(stmt, 1, bookID);
  CHECK(retval != SQLITE_OK, sqlite3_errmsg(database->database), return -1);

  // Execute
  retval = sqlite3_step(stmt);
  CHECK(retval != SQLITE_DONE, sqlite3_errmsg(database->database), return -1);

  // Finalize
  sqlite3_finalize(stmt);

  return 0;
}

int LibraryCreateBorrow(Borrow *borrow){
  int retval = 0;
  sqlite3_stmt* stmt;
  const char* sqlInsert = 
      "INSERT INTO " DATABASE_BORROWTABLE_NAME  
      " (bookID, startTimestamp, endTimestamp, name, classSequence)" 
      " VALUES (?, ?, ?, ?, ?)";

  // Prepare
  retval |= sqlite3_prepare_v2(database->database, sqlInsert, -1, &stmt, NULL);

  retval |= sqlite3_bind_int(stmt, 1, borrow->bookID);
  retval |= sqlite3_bind_int(stmt, 2, borrow->startTimestamp);
  retval |= sqlite3_bind_int(stmt, 3, borrow->endTimestamp);
  retval |= sqlite3_bind_text(stmt, 4, borrow->name, -1, SQLITE_STATIC);
  retval |= sqlite3_bind_text(stmt, 5, borrow->classSequence, -1, SQLITE_STATIC);
  CHECK(retval != SQLITE_OK, sqlite3_errmsg(database->database), return -1);

  // Execute
  retval = sqlite3_step(stmt);
  CHECK(retval != SQLITE_DONE, sqlite3_errmsg(database->database), return -1);

  // Finalize
  sqlite3_finalize(stmt);

  return 0;
}

int LibraryLoadBookChunk(Tbuffer **buffer, Scrolling scrolling){
  int retval = 0; 
  static int offset = 0;
  int bufferTitleOffset = 0;
  int bufferLibraryIDOffset = 0;
  Book bookToAdd;

  static char sqlQuery[128];

  switch(scrolling){
    case LOADNEXT:
      tbuffer_select(bookBuffer.buffer, FUTURE);
      ++offset;
      break;
    case LOADPREV:
      tbuffer_select(bookBuffer.buffer, PAST);
      --offset;
      break;
    case LOADBEGINNING:
      tbuffer_select(bookBuffer.buffer, PRESENT);
      offset = 0;
      break;
  }

  // Prepare the query
  snprintf(sqlQuery, sizeof(sqlQuery), 
           "SELECT bookID, borrowID, title, libraryID FROM " DATABASE_BOOKTABLE_NAME " LIMIT %d OFFSET %d",
           number_of_allocated_elements, offset)
  ;

  sqlite3_stmt *stmt;
  retval |= sqlite3_prepare_v2(database->database, sqlQuery, -1, &stmt, 0);
  CHECK(retval != SQLITE_OK, sqlite3_errmsg(database->database), return -1);

  sbuffer_clear(bookBuffer.titleBuffer);
  sbuffer_clear(bookBuffer.libraryIDBuffer);

  // Execute
  int i = 0;
  retval = sqlite3_step(stmt);
  while(retval == SQLITE_ROW){
    bookToAdd.bookID = sqlite3_column_int(stmt, 0);
    bookToAdd.borrowID = sqlite3_column_int(stmt, 1);
    bookToAdd.title = sbuffer_appendstring(bookBuffer.titleBuffer, (char*)sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));
    bookToAdd.title = sbuffer_appendstring(bookBuffer.libraryIDBuffer, (char*)sqlite3_column_text(stmt, 3), sqlite3_column_bytes(stmt, 3));

    tbuffer_writedata(&bookToAdd);

    retval = sqlite3_step(stmt);
    ++i;
  }

  // Finalize
  tbuffer_finalize();
  sqlite3_finalize(stmt);
  return i;
}

int LibraryLoadBorrowChunk(Tbuffer **buffer, Scrolling scrolling){
  int retval = 0; 
  static int offset = 0;
  int bufferNameOffset = 0;
  int bufferClassSequenceOffset = 0;
  Borrow borrowToAdd;

  static char sqlQuery[128];

  switch(scrolling){
    case LOADNEXT:
      tbuffer_select(borrowBuffer.buffer, FUTURE);
      ++offset;
      break;
    case LOADPREV:
      tbuffer_select(borrowBuffer.buffer, PAST);
      --offset;
      break;
    case LOADBEGINNING:
      tbuffer_select(borrowBuffer.buffer, PRESENT);
      offset = 0;
      break;
  }

  // Prepare the query
  snprintf(sqlQuery, sizeof(sqlQuery), 
           "SELECT borrowID, bookID, startTimestamp, endTimestamp, name, classSequence "
           "FROM " DATABASE_BORROWTABLE_NAME " LIMIT %d OFFSET %d",
           number_of_allocated_elements, offset);
  ;

  sqlite3_stmt *stmt;
  retval |= sqlite3_prepare_v2(database->database, sqlQuery, -1, &stmt, 0);
  CHECK(retval != SQLITE_OK, sqlite3_errmsg(database->database), return -1);

  sbuffer_clear(borrowBuffer.nameBuffer);
  sbuffer_clear(borrowBuffer.classSequenceBuffer);

  // Execute
  int i = 0;
  retval = sqlite3_step(stmt);
  while(retval == SQLITE_ROW){
    borrowToAdd.borrowID = sqlite3_column_int(stmt, 0);
    borrowToAdd.bookID = sqlite3_column_int(stmt, 1);
    borrowToAdd.startTimestamp = (time_t)sqlite3_column_int(stmt, 2);
    borrowToAdd.endTimestamp = (time_t)sqlite3_column_int(stmt, 3);
    borrowToAdd.name = sbuffer_appendstring(bookBuffer.titleBuffer, (char*)sqlite3_column_text(stmt, 4), sqlite3_column_bytes(stmt, 4));
    borrowToAdd.classSequence = sbuffer_appendstring(bookBuffer.libraryIDBuffer, (char*)sqlite3_column_text(stmt, 5), sqlite3_column_bytes(stmt, 5));

    tbuffer_writedata(&borrowToAdd);

    retval = sqlite3_step(stmt);
    ++i;
  }


  // Finalize
  tbuffer_finalize();
  sqlite3_finalize(stmt);
  return i;
}

int LibrarySearchBookChunk(Tbuffer **buffer, Scrolling scrolling, const char *keyword){
  int retval = 0; 
  static int offset = 0;
  int bufferTitleOffset = 0;
  int bufferLibraryIDOffset = 0;
  Book bookToAdd;

  static char sqlQuery[128];

  switch(scrolling){
    case LOADNEXT:
      tbuffer_select(bookBuffer.buffer, FUTURE);
      ++offset;
      break;
    case LOADPREV:
      tbuffer_select(bookBuffer.buffer, PAST);
      --offset;
      break;
    case LOADBEGINNING:
      tbuffer_select(bookBuffer.buffer, PRESENT);
      offset = 0;
      break;
  }

  // Prepare the query
  snprintf(sqlQuery, sizeof(sqlQuery), 
           "SELECT * FROM " DATABASE_BOOKTABLE_NAME " WHERE title LIKE '%%%s%%' OR libraryID LIKE '%%%s%%' LIMIT %d OFFSET %d;",
           keyword, keyword, 50, offset);
  ;

  sqlite3_stmt *stmt;
  retval |= sqlite3_prepare_v2(database->database, sqlQuery, -1, &stmt, 0);
  CHECK(retval != SQLITE_OK, sqlite3_errmsg(database->database), return -1);

  sbuffer_clear(bookBuffer.titleBuffer);
  sbuffer_clear(bookBuffer.libraryIDBuffer);

  // Execute
  int i = 0;
  retval = sqlite3_step(stmt);
  while(retval == SQLITE_ROW){
    bookToAdd.bookID = sqlite3_column_int(stmt, 0);
    bookToAdd.borrowID = sqlite3_column_int(stmt, 1);
    bookToAdd.title = sbuffer_appendstring(bookBuffer.titleBuffer, (char*)sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));
    bookToAdd.title = sbuffer_appendstring(bookBuffer.libraryIDBuffer, (char*)sqlite3_column_text(stmt, 3), sqlite3_column_bytes(stmt, 3));

    tbuffer_writedata(&bookToAdd);

    retval = sqlite3_step(stmt);
    ++i;
  }

  // Finalize
  tbuffer_finalize();
  sqlite3_finalize(stmt);
  return i;
}


int LibrarySearchBorrowChunk(Tbuffer **buffer, Scrolling scrolling, const char *keyword){
  int retval = 0; 
  static int offset = 0;
  int bufferTitleOffset = 0;
  int bufferLibraryIDOffset = 0;
  Borrow borrowToAdd;

  static char sqlQuery[128];

  switch(scrolling){
    case LOADNEXT:
      tbuffer_select(borrowBuffer.buffer, FUTURE);
      ++offset;
      break;
    case LOADPREV:
      tbuffer_select(borrowBuffer.buffer, PAST);
      --offset;
      break;
    case LOADBEGINNING:
      tbuffer_select(borrowBuffer.buffer, PRESENT);
      offset = 0;
      break;
  }

  // Prepare the query
  snprintf(sqlQuery, sizeof(sqlQuery), 
           "SELECT * " DATABASE_BOOKTABLE_NAME " WHERE title LIKE '%%%s%%' OR libraryID LIKE '%%%s%%' LIMIT %d OFFSET %d;",
           keyword, keyword, 50, offset);
  ;

  sqlite3_stmt *stmt;
  retval |= sqlite3_prepare_v2(database->database, sqlQuery, -1, &stmt, 0);
  CHECK(retval != SQLITE_OK, sqlite3_errmsg(database->database), return -1);

  sbuffer_clear(borrowBuffer.nameBuffer);
  sbuffer_clear(borrowBuffer.classSequenceBuffer);

  // Execute
  int i = 0;
  retval = sqlite3_step(stmt);
  while(retval == SQLITE_ROW){
    borrowToAdd.borrowID = sqlite3_column_int(stmt, 0);
    borrowToAdd.bookID = sqlite3_column_int(stmt, 1);
    borrowToAdd.startTimestamp = (time_t)sqlite3_column_int(stmt, 2);
    borrowToAdd.endTimestamp = (time_t)sqlite3_column_int(stmt, 3);
    borrowToAdd.name = sbuffer_appendstring(bookBuffer.titleBuffer, (char*)sqlite3_column_text(stmt, 4), sqlite3_column_bytes(stmt, 4));
    borrowToAdd.classSequence = sbuffer_appendstring(bookBuffer.libraryIDBuffer, (char*)sqlite3_column_text(stmt, 5), sqlite3_column_bytes(stmt, 5));

    tbuffer_writedata(&borrowToAdd);

    retval = sqlite3_step(stmt);
    ++i;
  }

  // Finalize
  tbuffer_finalize();
  sqlite3_finalize(stmt);
  return i;
}
