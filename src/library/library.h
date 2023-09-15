#ifndef LIBRARY_H
#define LIBRARY_H

#include <time.h>


#define BOOK_CHUNK 64
#define BORROW_CHUNK 64


typedef enum{
  ONGOING,
  RETURNED
}BorrowStatus ; 

typedef enum {
  LOADBEGINNING,
  LOADNEXT,
  LOADPREV
}Scrolling ;


typedef struct{
  int bookID;
  int borrowID;

  char *title;
  char *libraryID;
}Book ;

typedef struct{
  int borrowID;
  int bookID;
  BorrowStatus status;
  time_t startTimestamp;
  time_t endTimestamp;

  char *name;
  char *classSequence;
}Borrow ;


extern Book *dataBookBuffer;
extern Borrow *dataBorrowBuffer;

extern int initLibrary();
extern int closeLibrary();
extern int createDatabase();
extern int loadBookDataChunk(Book **buffer, Scrolling scrolling);

extern int testCreate();
extern int testFetch();

#endif // !LIBRARY_H
