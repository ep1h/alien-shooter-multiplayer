#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include <stdbool.h>

typedef struct Multiplayer Multiplayer;

Multiplayer* multiplayer_create(void);
void multiplayer_destroy(Multiplayer* mp);

#endif /* MULTIPLAYER_H */
