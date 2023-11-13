#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define BLOCK_SIZE 128

typedef struct{
  uint32_t size;

  uint32_t bufferStop;
  char *buffer;
} singleBuffer;

typedef struct{
  uint32_t size;

  uint32_t bufferStop;
  void *buffer;

  void *past;
  void *present;
  void *future;

  //singleBuffer *
} Buffer;



extern Buffer *CreateBuffer(size_t size);
extern char *BufferWriteString(Buffer *buffer, const char *string, const size_t length);
extern int FreeBuffer(Buffer *buffer);

#endif // !BUFFER_H
