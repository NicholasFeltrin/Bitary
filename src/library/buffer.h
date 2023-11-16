#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

const int blockSize = 500;

typedef enum{
  WRITING,
  IDLE
} BufferStatus ; 

typedef enum{
  PAST,
  PRESENT,
  FUTURE
} TbufferSection ; 

typedef struct{
  int size;
  BufferStatus status; 

  int bufferStop;
  char *buffer;
} Sbuffer;

typedef struct{
  uint32_t sectionSize;
  uint16_t numOfElements;
  uint8_t elementSize;
  BufferStatus status; 

  void *buffer;

  void *past;
  void *present;
  void *future;
} Tbuffer;

#endif // !BUFFER_H
