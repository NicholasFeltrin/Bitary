#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct{
  int size;

  char *buffer;
  char *stringStop;
} Buffer;

Buffer *createBuffer(size_t size){
  Buffer *buffer = (Buffer*)malloc(sizeof(Buffer));

  buffer->buffer = (char*)malloc(sizeof(char)*size);
  buffer->stringStop = buffer->buffer;
  buffer->size = size;

  return buffer;
}

char *BufferWriteString(Buffer buffer, char* string, size_t length){
   
}
