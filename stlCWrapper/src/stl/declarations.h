#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#define MAX_HANDLE 32000

typedef int HANDLE;
#define NULL_HANDLE 0

#define NO_DEALLOC ((dealloc_t)NULL)
#define DEFAULT_DEALLOC (&dealloc_free)

typedef void (*dealloc_t)(void**);

#endif
