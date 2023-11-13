#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "src/library/defs.h"


Buffer *CreateBuffer(size_t size){
  Buffer *buffer = (Buffer*)malloc(sizeof(Buffer));

  buffer->size = size*3;
  buffer->buffer = (void*)calloc(buffer->size, 1);
  buffer->past = buffer->buffer;
  buffer->present = buffer->buffer+size;
  buffer->future = buffer->buffer+(size*2);
  buffer->bufferStop = 0;

  return buffer;
}

char *BufferWrite(Buffer *buffer, const void *object){
  buffer->buffer


  char *ret;
  int newStop = buffer->stringStop+length+1;

  if(newStop > buffer->size){
    char* tmp = (char*)realloc(buffer->buffer, buffer->size+BUFFER_LENGTH*sizeof(char));
    if(tmp == NULL){
      free(buffer->buffer);
      return NULL;
    }
    else if(buffer->buffer != tmp){
      buffer->buffer = tmp;
    }
    tmp = NULL;
  }

  strcpy(buffer->buffer+buffer->stringStop, string);
  ret = buffer->buffer+buffer->stringStop;
  *(buffer->buffer+buffer->stringStop+length) = '\0';
  buffer->stringStop = newStop;

  return ret;
}

int FreeBuffer(Buffer *buffer){
  free(buffer->buffer);
  free(buffer);
  return 0;
}
