#ifndef HEADER_H
#define HEADER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { NONE = 0, UP = 1, LEFT = 2, DOWN = 3, RIGHT = 4 } direction_t;
typedef unsigned char cell_t;

typedef int (*shift_fn)();

extern shift_fn shift[5];
extern cell_t board[16];

extern void init_display(int*, char* argv[]);
extern void loop();

extern void addRandomTile();

#ifdef __cplusplus
}
#endif

#endif
