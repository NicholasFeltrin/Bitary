#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 128

typedef struct{
  int size;

  int stringStop;
  char *buffer;
} Buffer;


extern Buffer *CreateBuffer(size_t size);
extern char *BufferWriteString(Buffer *buffer, const char *string, const size_t length);
extern int FreeBuffer(Buffer *buffer);

#endif // !BUFFER_H
