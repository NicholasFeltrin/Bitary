#ifndef LIBRARY_H
#define LIBRARY_H

#include "defs.h"
#include <time.h>
#include <stdlib.h>

extern int initLibrary();
extern int closeLibrary();
extern int createDatabase();

extern int testCreate();
extern int testFetch();

typedef enum{
  ONGOING,
  RETURNED
}BorrowStatus ; 

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

#endif // !LIBRARY_H
