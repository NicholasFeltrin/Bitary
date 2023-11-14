#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "src/library/defs.h"

typedef enum{
  WRITING,
  IDLE
}STATUS ; 

STATUS status;
char *errormsg;

void *bufferStop;
int countStop;

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

int SelectBuffer(Buffer *buffer, BufferSection section, size_t size){
  if(status == WRITING){
    return -1;
  }else if(section == FUTURE){
    bufferStop = buffer->past;
  }else if(section == PAST){
    bufferStop = buffer->future;
  }else if(section == PRESENT){
    return -1;
  }
  status = WRITING;
  countStop = 0;
}

int WriteDataBuffer(Buffer *buffer, void *data, size_t size){
  if(status == WRITING){
    memcpy(bufferStop, data, size);
    bufferStop += size;
  }
}

int FinalizeBuffer(Buffer buffer){
  status = IDLE;
  countStop = 0;
}

int FreeBuffer(Buffer *buffer){
  free(buffer->buffer);
  free(buffer);
  return 0;
}
