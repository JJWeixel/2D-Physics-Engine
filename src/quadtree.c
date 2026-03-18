#include "raylib.h"
#include "physics.h"
#include "stdlib.h"

Quadtree *qt_create(QtPool *pool, Rectangle rect) {
    return pool_alloc(pool, rect);
}

void qt_subdivide(QtPool *pool, Quadtree *qt) {
    float width = qt->rect.width / 2;
    float height = qt->rect.height / 2;
    float x = qt->rect.x;
    float y = qt->rect.y;

    qt->nw = qt_create(pool, (Rectangle){x, y, width, height});
    qt->ne = qt_create(pool, (Rectangle){x + width, y, width, height});
    qt->sw = qt_create(pool, (Rectangle){x, y + height, width, height});
    qt->se = qt_create(pool, (Rectangle){x + width, y + height, width, height});

    qt->divided = true;
}

bool qt_insert(QtPool *pool, Quadtree *qt, Ball *b) {
    if (!CheckCollisionPointRec(b->p, qt->rect)) {
        return false;
    }

    if (qt->count < QT_THRESHHOLD && !qt->divided) {
        qt->elements[qt->count] = b;
        qt->count++;
        return true;
    }

    if (!qt->divided) qt_subdivide(pool, qt);

    return qt_insert(pool, qt->nw, b) ||
           qt_insert(pool, qt->ne, b) ||
           qt_insert(pool, qt->sw, b) ||
           qt_insert(pool, qt->se, b);
}

void qt_free(Quadtree *qt) {
    if (!qt) return;
    qt_free(qt->nw);
    qt_free(qt->ne);
    qt_free(qt->sw);
    qt_free(qt->se);
    free(qt);
}

int qt_query(Quadtree *qt, Vector2 center, float r, Ball **results, int max) {
    if (!CheckCollisionCircleRec(center, r, qt->rect))
        return 0;

    int found = 0;
    for (int i = 0; i < qt->count && found < max; i++) {
        if (CheckCollisionPointCircle(qt->elements[i]->p, center, r))
            results[found++] = qt->elements[i];
    }

    if (qt->divided) {
        found += qt_query(qt->nw, center, r, results + found, max - found);
        found += qt_query(qt->ne, center, r, results + found, max - found);
        found += qt_query(qt->sw, center, r, results + found, max - found);
        found += qt_query(qt->se, center, r, results + found, max - found);
    }

    return found;
}

void qt_draw(Quadtree *qt) {
    if (!qt) return;
    DrawRectangleLinesEx(qt->rect, .3f, RED);
    qt_draw(qt->nw);
    qt_draw(qt->ne);
    qt_draw(qt->sw);
    qt_draw(qt->se);
}