#include "animation.h"

static const animation_t* animations[MAX_ANIMATIONS];
static size_t animation_count = 0;

void register_animation(const animation_t* anim) {
    if (animation_count < MAX_ANIMATIONS) {
        animations[animation_count++] = anim;
    }
}

const animation_t* get_animation(size_t index) {
    if (index >= animation_count) return NULL;
    return animations[index];
}

size_t get_animation_count(void) {
    return animation_count;
}
