#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "defs.h"

typedef enum{
  WRITING,
  IDLE
}STATUS ; 

STATUS status;
char *errormsg;

int countStop;
void *bufferStop;

Buffer *CreateBuffer(size_t numOfElements, size_t elementSize){
  Buffer *buffer = (Buffer*)malloc(sizeof(Buffer));

  buffer->numOfElements = numOfElements;
  buffer->elementSize = elementSize;
  buffer->sectionSize = numOfElements*elementSize;

  buffer->buffer = (void*)calloc(buffer->sectionSize*3, 1);
  buffer->past = buffer->buffer;
  buffer->present = buffer->buffer+buffer->sectionSize;
  buffer->future = buffer->buffer+(buffer->sectionSize*2);

  return buffer;
}

int FreeBuffer(Buffer *buffer){
  free(buffer->buffer);
  free(buffer);
  return 0;
}

int SelectBuffer(Buffer *buffer, BufferSection section){
  if(status != IDLE){
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

  return 0;
}

int WriteDataBuffer(Buffer *buffer, void *data){
  if(status == WRITING && countStop < buffer->numOfElements){
    memcpy(bufferStop, data, buffer->elementSize);
    ++countStop;
    bufferStop += buffer->elementSize;
  }else if(status != WRITING){
    return -1;
  }else if(countStop >= buffer->numOfElements){
    return -1;
  }

  return 0;
}

int FinalizeBuffer(Buffer buffer){
  if(status != WRITING){
    return -1;
  }else{
    status = IDLE;
    countStop = 0;
  }

  return 0;
}

