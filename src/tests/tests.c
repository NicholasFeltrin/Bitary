#include <stdlib.h>
#include <unistd.h>
#include "tests.h"
#include "../library/library.h"
#include "../library/helper.h"

extern void TestCreateBooks(size_t n);

void startUpTest(){
  Book *ptr;

  TestCreateBooks(128);
  //LibraryLoadBookChunk(&ptr, LOADBEGINNING);
  //TestCreateBooks(128);
  //LibraryLoadBookChunk(&ptr, LOADNEXT);
  //printf("PASS"); fflush(stdout);
}

void TestCreateBooks(size_t n){
  char word[5] = "AAAA";

  Book myBook;
  myBook.title = word;
  myBook.libraryID = word;

  for(int i = 0;i < n;++i){
    int carry = 1;
    for (int i = 3; i >= 0; i--) {
      if (carry == 0) break;
      
      int digit = word[i] - 'A' + carry;
      carry = digit / 26;
      word[i] = 'A' + digit % 26;
    }
    
    if (carry == 1) {
      // Reached 'ZZZZ', exit the loop
      break;
    }

    myBook.borrowID = 0;

    CHECK(LibraryCreateBook(&myBook), "error creating books", return;)
    usleep(500);
  }
}

void TestCreateBorrows(size_t n){
  char word[5] = "AAAA";

  Borrow myBorrow;
  myBorrow.name = word;
  myBorrow.classSequence = word;

  for(int i = 0;i < n;++i){
    int carry = 1;
    for (int i = 3; i >= 0; i--) {
      if (carry == 0) break;
      
      int digit = word[i] - 'A' + carry;
      carry = digit / 26;
      word[i] = 'A' + digit % 26;
    }
    
    if (carry == 1) {
      // Reached 'ZZZZ', exit the loop
      break;
    }

    myBorrow.bookID = i;
    time(&(myBorrow.startTimestamp));
    myBorrow.status = ONGOING;

    CHECK(LibraryCreateBorrow(&myBorrow) != 0, "error creating borrows", return;)
    usleep(500);
  }
}

int testFetch(){
  //Book *myBook;
 // myBook = (Book *)malloc(100*sizeof(Book));
//  loadBookDataChunk(&, LOADBEGINNING);

  for(int i = 0;i < 20;++i){
    printf("ID: %d\n", dataBookBuffer[i].bookID);
    printf("borrowID: %d\n", dataBookBuffer[i].borrowID);
    printf("title: %s\n", dataBookBuffer[i].title);
  }

  //free(myBook);

  return 0;
}
