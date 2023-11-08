#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct{
  int size;

  int stringStop;
  char *buffer;
} Buffer;

Buffer *createBuffer(size_t size){
  Buffer *buffer = (Buffer*)malloc(sizeof(Buffer));

  buffer->buffer = (char*)calloc(size, sizeof(char));
  buffer->stringStop = 0;
  buffer->size = size;

  return buffer;
}

char *BufferWriteString(Buffer *buffer, const char *string, const size_t length){
  int newStop = buffer->stringStop+length+1;
  if(newStop <= buffer->size){
    strcpy(buffer->buffer+buffer->stringStop, string);
    *(buffer->buffer+buffer->stringStop+length) = '\0';
    buffer->stringStop = newStop;
  }else{

  }
}
