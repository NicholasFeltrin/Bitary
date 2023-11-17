#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

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
  uint64_t sectionSize;
  uint32_t numOfElements;
  uint16_t elementSize;
  BufferStatus status; 
  TbufferSection section;

  void *buffer;

  void *past;
  void *present;
  void *future;
} Tbuffer;

extern Tbuffer *tbuffer_create(size_t numOfElements, size_t elementSize);
extern int tbuffer_free(Tbuffer *buffer);
extern int tbuffer_select(Tbuffer *buffer, TbufferSection section);
extern int tbuffer_writedata(void *data);
extern int tbuffer_finalize();
extern Sbuffer *sbuffer_create();
extern int sbuffer_free(Sbuffer *buffer);
extern char *sbuffer_appendstring(Sbuffer *buffer, char *string, size_t length);
extern int sbuffer_clear(Sbuffer *buffer);

#endif // !BUFFER_H
