#ifndef LIBRARY_H
#define LIBRARY_H

#include "buffer.h"
#include <time.h>

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

typedef struct{
  Tbuffer *buffer;
  Sbuffer *titleBuffer;
  Sbuffer *libraryIDBuffer;
}BookBuffer ;

typedef struct{
  Tbuffer *buffer;
  Sbuffer *nameBuffer;
  Sbuffer *classSequenceBuffer;
}BorrowBuffer ;

typedef struct{
  BookBuffer bookBuffer;
  BorrowBuffer borrowBuffer;
}SearchBuffer ;


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

#endif // !LIBRARY_H
