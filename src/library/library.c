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
extern int LibraryLoadBookChunk(Book **buffer, Scrolling scrolling);
extern int LibraryLoadBorrowChunk(Borrow **buffer, Scrolling scrolling);
extern int LibrarySearchBookChunk(Book **buffer, Scrolling scrolling, const char *keyword);
extern int LibrarySearchBorrowChunk(Borrow **buffer, Scrolling scrolling, const char *keyword);
//extern int fetchBook(Book **buffer, int pageSize);


Database *database;

Book *dataBookBuffer;
Borrow *dataBorrowBuffer;

Buffer *bookTitleBuffer;
Buffer *bookLibraryIDBuffer;
Buffer *borrowNameBuffer;
Buffer *borrowClassSequenceBuffer;
Buffer *searchBuffer;


int LibraryInit(){
  int retval;
  // Add Database (not connected)
  database = addDatabase(DATABASE_FILE_PATH);
  CHECK(database == NULL, "Error in adding database", return -1);

  // Create database
  retval = LibraryCreateDatabase(DATABASE_FILE_PATH);
  CHECK(retval != 0, "Error in creating", return -1);

  // Connect newly created database
  retval = connectDatabase(database);
  CHECK(retval != 0, "Error in oppening database", return -1);

  // Allocate buffer for a chunk of books and borrows
  dataBookBuffer = (Book*)malloc(sizeof(Book)*BOOK_CHUNK);
  dataBorrowBuffer = (Borrow*)malloc(sizeof(Borrow)*BORROW_CHUNK);

  // Allocate buffer for strings associated book and borrow data
  bookTitleBuffer = CreateBuffer(CACHE_LENGTH+BUFFER_LENGTH);
  bookLibraryIDBuffer = CreateBuffer(CACHE_LENGTH+BUFFER_LENGTH);

  borrowNameBuffer = CreateBuffer(CACHE_LENGTH+BUFFER_LENGTH);
  borrowClassSequenceBuffer = CreateBuffer(CACHE_LENGTH+BUFFER_LENGTH);

  searchBuffer = CreateBuffer(CACHE_LENGTH+BUFFER_LENGTH);

  return 0;
}

int LibraryClose(){
  int retval;
  retval = disconnectDatabase(database);
  CHECK(retval != 0, "Error in closeDatabase()", return -1) ;

  free(dataBookBuffer);
  free(dataBorrowBuffer);
  FreeBuffer(bookTitleBuffer);
  FreeBuffer(bookLibraryIDBuffer);
  FreeBuffer(borrowNameBuffer);
  FreeBuffer(borrowClassSequenceBuffer);
  FreeBuffer(searchBuffer);

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

int LibraryLoadBookChunk(Book **buffer, Scrolling scrolling){
  int retval = 0; 
  static int offset = 0;
  int bufferTitleOffset = 0;
  int bufferLibraryIDOffset = 0;

  static char sqlQuery[128];

  switch(scrolling){
    case LOADNEXT:
      ++offset;
      break;
    case LOADPREV:
      --offset;
      break;
    case LOADBEGINNING:
      offset = 0;
      break;
  }

  // Prepare the query
  snprintf(sqlQuery, sizeof(sqlQuery), 
           "SELECT bookID, borrowID, title, libraryID FROM " DATABASE_BOOKTABLE_NAME " LIMIT %d OFFSET %d",
           50, offset)
  ;

  sqlite3_stmt *stmt;
  retval |= sqlite3_prepare_v2(database->database, sqlQuery, -1, &stmt, 0);
  CHECK(retval != SQLITE_OK, sqlite3_errmsg(database->database), return -1);

  // Execute
  int i = 0;
  retval = sqlite3_step(stmt);
  while(retval == SQLITE_ROW){
    // Copy to main buffer
    dataBookBuffer[i].bookID = sqlite3_column_int(stmt, 0);
    dataBookBuffer[i].borrowID = sqlite3_column_int(stmt, 1);

    // Copy to string buffer
    dataBookBuffer[i].title =  BufferWriteString(bookTitleBuffer, (char*)sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));

    dataBookBuffer[i].libraryID = BufferWriteString(bookLibraryIDBuffer, (char*)sqlite3_column_text(stmt, 3), sqlite3_column_bytes(stmt, 3));

    retval = sqlite3_step(stmt);
    ++i;
  }

  *buffer = dataBookBuffer;

  // Finalize
  sqlite3_finalize(stmt);
  return i;
}

int LibraryLoadBorrowChunk(Borrow **buffer, Scrolling scrolling){
  int retval = 0; 
  static int offset = 0;
  int bufferNameOffset = 0;
  int bufferClassSequenceOffset = 0;

  static char sqlQuery[128];

  switch(scrolling){
    case LOADNEXT:
      ++offset;
      break;
    case LOADPREV:
      --offset;
      break;
    case LOADBEGINNING:
      offset = 0;
      break;
  }

  // Prepare the query
  snprintf(sqlQuery, sizeof(sqlQuery), 
           "SELECT borrowID, bookID, startTimestamp, endTimestamp, name, classSequence "
           "FROM " DATABASE_BORROWTABLE_NAME " LIMIT %d OFFSET %d",
           BORROW_CHUNK, offset);
  ;

  sqlite3_stmt *stmt;
  retval |= sqlite3_prepare_v2(database->database, sqlQuery, -1, &stmt, 0);
  CHECK(retval != SQLITE_OK, sqlite3_errmsg(database->database), return -1);

  // Execute
  int i = 0;
  retval = sqlite3_step(stmt);
  while(retval == SQLITE_ROW){
    // Copy to main buffer
    dataBorrowBuffer[i].borrowID = sqlite3_column_int(stmt, 0);
    dataBorrowBuffer[i].bookID = sqlite3_column_int(stmt, 1);
    dataBorrowBuffer[i].startTimestamp = (time_t)sqlite3_column_int(stmt, 2);
    dataBorrowBuffer[i].endTimestamp = (time_t)sqlite3_column_int(stmt, 3);

    // Copy to string buffer
    dataBorrowBuffer[i].name =  BufferWriteString(borrowNameBuffer, (char*)sqlite3_column_text(stmt, 4), sqlite3_column_bytes(stmt, 4));

    dataBorrowBuffer[i].classSequence =  BufferWriteString(borrowClassSequenceBuffer, (char*)sqlite3_column_text(stmt, 5), sqlite3_column_bytes(stmt, 5));

    retval = sqlite3_step(stmt);
    ++i;
  }

  *buffer = dataBorrowBuffer;

  // Finalize
  sqlite3_finalize(stmt);
  if(scrolling == LOADNEXT){
    printf("AFJLADFJ\n");
    fflush(stdout);
  }
  return i;
}

int LibrarySearchBookChunk(Book **buffer, Scrolling scrolling, const char *keyword){
  int retval = 0; 
  static int offset = 0;
  int bufferTitleOffset = 0;
  int bufferLibraryIDOffset = 0;

  static char sqlQuery[128];

  switch(scrolling){
    case LOADNEXT:
      ++offset;
      break;
    case LOADPREV:
      --offset;
      break;
    case LOADBEGINNING:
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

  // Execute
  int i = 0;
  retval = sqlite3_step(stmt);
  while(retval == SQLITE_ROW){
    // Copy to main buffer
    dataBookBuffer[i].bookID = sqlite3_column_int(stmt, 0);
    dataBookBuffer[i].borrowID = sqlite3_column_int(stmt, 1);

    // Copy to string buffer
    dataBookBuffer[i].title =  BufferWriteString(searchBuffer, (char*)sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));

    dataBookBuffer[i].libraryID = BufferWriteString(searchBuffer, (char*)sqlite3_column_text(stmt, 3), sqlite3_column_bytes(stmt, 3));

    retval = sqlite3_step(stmt);
    ++i;
  }

  *buffer = dataBookBuffer;

  // Finalize
  sqlite3_finalize(stmt);
  return i;
}


int LibrarySearchBorrowChunk(Borrow **buffer, Scrolling scrolling, const char *keyword){
  int retval = 0; 
  static int offset = 0;
  int bufferTitleOffset = 0;
  int bufferLibraryIDOffset = 0;

  static char sqlQuery[128];

  switch(scrolling){
    case LOADNEXT:
      ++offset;
      break;
    case LOADPREV:
      --offset;
      break;
    case LOADBEGINNING:
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

  // Execute
  int i = 0;
  retval = sqlite3_step(stmt);
  while(retval == SQLITE_ROW){
    // Copy to main buffer
    dataBookBuffer[i].bookID = sqlite3_column_int(stmt, 0);
    dataBookBuffer[i].borrowID = sqlite3_column_int(stmt, 1);

    // Copy to string buffer
    dataBookBuffer[i].title =  BufferWriteString(searchBuffer, (char*)sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));

    dataBookBuffer[i].libraryID = BufferWriteString(searchBuffer, (char*)sqlite3_column_text(stmt, 3), sqlite3_column_bytes(stmt, 3));

    retval = sqlite3_step(stmt);
    ++i;
  }

  *buffer = dataBookBuffer;

  // Finalize
  sqlite3_finalize(stmt);
  return i;
}
