#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <stdint.h>
#include <stddef.h>

// animation flags
#define ANIM_USES_TOUCH 1

typedef void (*animation_init_fn)(void);
typedef void (*animation_tick_fn)(void);
typedef void (*animation_on_press_fn)(uint8_t count);

typedef struct {
    animation_init_fn init;
    animation_tick_fn tick;
    animation_on_press_fn on_press;
    uint16_t tick_interval;
    uint8_t flags;
} animation_t;

// Max number of animations
#define MAX_ANIMATIONS 16

// Called by animations to register themselves
void register_animation(const animation_t* anim);

// Accessor for animation manager
const animation_t* get_animation(size_t index);
size_t get_animation_count(void);

#endif