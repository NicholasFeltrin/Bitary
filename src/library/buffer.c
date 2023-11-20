#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "defs.h"

char *errormsg;

Tbuffer *selectedTbuffer;
Sbuffer *selectedSbuffer;

int countStop;
void *bufferStop;

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
    bufferStop = buffer->present;
  }
  buffer->section = section;
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
  void *tmp; 

  if(selectedTbuffer->section == PRESENT && selectedTbuffer->status == WRITING){
    tmp = selectedTbuffer->present;
    selectedTbuffer->present = selectedTbuffer->future;
    selectedTbuffer->future = selectedTbuffer->past;
    selectedTbuffer->past = tmp;
  }else if(selectedTbuffer->section == FUTURE && selectedTbuffer->status == WRITING){
    tmp = selectedTbuffer->present; 
    selectedTbuffer->present = selectedTbuffer->past;
    selectedTbuffer->past = selectedTbuffer->future;
    selectedTbuffer->future = tmp;
  }else if(selectedTbuffer->section == PAST && selectedTbuffer->status == WRITING){
    tmp = selectedTbuffer->present;
    selectedTbuffer->present = selectedTbuffer->future;
    selectedTbuffer->future = selectedTbuffer->past;
    selectedTbuffer->past = tmp;
  }else{
    return -1; 
  }

  selectedTbuffer->status = IDLE;
  selectedTbuffer->section = NONE;
  countStop = 0;

  return 0;
}

void *tbuffer_read(Tbuffer *buffer, int index){
  return buffer->buffer+(index*buffer->elementSize); 
}

Sbuffer *sbuffer_create(){
  Sbuffer *buffer = (Sbuffer*)malloc(sizeof(Sbuffer));

  buffer->size = block_size;
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
    char *tmp = (char*)realloc(buffer->buffer, buffer->size+block_size);
    if(tmp == NULL){
      free(buffer->buffer);
      return NULL;
    }else if(buffer->buffer != tmp){
      buffer->buffer = tmp;
    }
    tmp = NULL;
  }

  strcpy(buffer->buffer+buffer->bufferStop, string);
  ret = buffer->buffer+buffer->bufferStop;
  *(ret+length) = '\0';
  buffer->bufferStop = newStop;

  return ret;
}

int sbuffer_clear(Sbuffer *buffer){
  if(buffer->size > block_size){
    char *tmp = (char*)realloc(buffer->buffer, block_size);
    if(tmp == NULL){
      free(buffer->buffer);
      return -1;
    }else if(buffer->buffer != tmp){
      buffer->buffer = tmp;
    }
    tmp = NULL;
  }
  buffer->bufferStop = 0;

  return 0;
} 
