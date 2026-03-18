#include "raylib.h"

#define G 0
#define R 3
#define NUM_PARTICLES 25000
#define QT_THRESHHOLD 4

typedef struct ball {
    float r;
    Vector2 p;
    Vector2 v;
    Vector2 a;
    int id;
} Ball;

typedef struct Quadtree {
    Rectangle rect;
    int count;
    bool divided;
    Ball *elements[4];
    struct Quadtree *nw, *ne, *sw, *se;
} Quadtree;

typedef struct QtPool{
    Quadtree nodes[NUM_PARTICLES];
    int count;
} QtPool;

Quadtree *qt_create(QtPool *, Rectangle);
void qt_subdivide(QtPool *, Quadtree *);
bool qt_insert(QtPool *, Quadtree *, Ball *);
void qt_free(Quadtree *);
int qt_query(Quadtree *, Rectangle, Ball **, int);
void qt_draw(Quadtree *);

void sim_physics(Ball *, float);
void check_boundaries(Ball *);
void handle_collision(Ball *, Ball *);
Color velocity_to_color(Vector2);
void check_collisions(Quadtree *, Ball *);
int get_nearby(Quadtree *, Ball *, Ball **, int);

Quadtree *pool_alloc(QtPool *, Rectangle);
void pool_reset(QtPool *);
