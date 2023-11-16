#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"

char *errormsg;

Tbuffer *selectedTbuffer;
Sbuffer *selectedSbuffer;

int countStop;
void *bufferStop;

Sbuffer *sbuffer_create(size_t size){
  Sbuffer *buffer = (Sbuffer*)malloc(sizeof(Sbuffer));

  buffer->size = size;
  buffer->buffer = (void*)calloc(buffer->size, sizeof(char));
  buffer->bufferStop = 0;

  return buffer;
}

int sbuffer_free(Sbuffer *buffer){
  free(buffer->buffer);
  free(buffer);
  return 0;
}

char *sbuffer_appendstring(Sbuffer *buffer, char *string, size_t length){
  char *ret;
  int newStop = buffer->bufferStop+length+1;

  if(newStop > buffer->size){
    char *tmp = (char*)realloc(buffer->buffer, buffer->size+blockSize);
    if(tmp == NULL){
      free(buffer->buffer);
      return NULL;
    }else 
  }
}

Tbuffer *tbuffer_create(size_t numOfElements, size_t elementSize){
  Tbuffer *buffer = (Tbuffer*)malloc(sizeof(Tbuffer));

  buffer->numOfElements = numOfElements;
  buffer->elementSize = elementSize;
  buffer->sectionSize = numOfElements*elementSize;

  buffer->buffer = (void*)calloc(buffer->sectionSize*3, 1);
  buffer->past = buffer->buffer;
  buffer->present = buffer->buffer+buffer->sectionSize;
  buffer->future = buffer->buffer+(buffer->sectionSize*2);

  return buffer;
}

int tbuffer_free(Tbuffer *buffer){
  free(buffer->buffer);
  free(buffer);
  return 0;
}

int tbuffer_select(Tbuffer *buffer, TbufferSection section){
  if(buffer->status != IDLE){
    return -1;
  }else if(section == FUTURE){
    bufferStop = buffer->past;
  }else if(section == PAST){
    bufferStop = buffer->future;
  }else if(section == PRESENT){
    return -1;
  }
  selectedTbuffer = buffer;
  buffer->status = WRITING;
  countStop = 0;

  return 0;
}

int tbuffer_writedata(void *data){
  if(selectedTbuffer->status == WRITING && countStop < selectedTbuffer->numOfElements){
    memcpy(bufferStop, data, selectedTbuffer->elementSize);
    ++countStop;
    bufferStop += selectedTbuffer->elementSize;
  }else if(selectedTbuffer->status != WRITING){
    return -1;
  }else if(countStop >= selectedTbuffer->numOfElements){
    return -1;
  }

  return 0;
}

int tbuffer_finalize(){
  if(selectedTbuffer->status != WRITING){
    return -1;
  }else{
    selectedTbuffer->status = IDLE;
    countStop = 0;
  }

  return 0;
}
