#ifndef ARGS_H
#define ARGS_H

#include <stdint.h>

typedef struct {
	int date1;
	int date2;
} dates;

dates parse(int argc, char *argv[]);

#endif
