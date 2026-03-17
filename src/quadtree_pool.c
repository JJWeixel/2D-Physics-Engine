#include "physics.h"
#include "stdlib.h"

Quadtree *pool_alloc(QtPool *pool, Rectangle rect) {
    Quadtree *qt = &pool->nodes[pool->count++];
    qt->rect = rect;
    qt->count = 0;
    qt->divided = false;
    qt->nw = qt->ne = qt->sw = qt->se = NULL;
    return qt;
}

void pool_reset(QtPool *pool) {
    pool->count = 0;
}