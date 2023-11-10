#ifndef LIBRARY_H
#define LIBRARY_H

#include "stdint.h"
#include <time.h>


#define BOOK_CHUNK 1024
#define BORROW_CHUNK 1024


typedef enum{
  ONGOING = 0,
  RETURNED
}BorrowStatus ; 

typedef enum {
  LOADBEGINNING,
  LOADREFRESH,
  LOADNEXT,
  LOADPREV
}Scrolling ;


typedef struct{
  uint32_t bookID;
  uint32_t borrowID;

  char *title;
  char *libraryID;
}Book ;

typedef struct{
  uint32_t borrowID;
  uint32_t bookID;
  time_t startTimestamp;
  union{
    BorrowStatus status;
    time_t endTimestamp;
  };

  char *name;
  char *classSequence;
}Borrow ;


extern Book *dataBookBuffer;
extern Borrow *dataBorrowBuffer;

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

extern int testCreate();
extern int testFetch();

#endif // !LIBRARY_H
