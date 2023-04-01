/**
 * @file multiplayer.h
 * @brief Multiplayer logic.
 * 
 */
#ifndef MULTIPLAYER_MULTIPLAYER_H
#define MULTIPLAYER_MULTIPLAYER_H

#include <stdbool.h>

typedef struct Multiplayer Multiplayer;

/**
 * @brief Initializes multiplayer component.
 * 
 * @return true if initialization was successful.
 * @return false if initialization failed.
 */
bool multiplayer_init(void);

/**
 * @brief Destroys multiplayer component.
 */
void multiplayer_destroy(void);

/**
 * @brief Processes multiplayer component's internal state. Switches states.
 */
void multiplayer_tick(void); 

/**
 * @brief Returns pointer to multiplayer component instance.
 * 
 * @return Multiplayer* Pointer to multiplayer component instance.
*/
Multiplayer* multiplayer_instance(void);

#endif /* MULTIPLAYER_MULTIPLAYER_H */
