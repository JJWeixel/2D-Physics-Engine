#include "physics.h"
#include "raylib.h"
#include "raymath.h"

void check_boundaries(Ball *obj) {
	float left = obj->p.x - obj->r;
	float right = obj->p.x + obj->r;
	float top = obj->p.y - obj->r;
	float bottom = obj->p.y + obj->r;
	if (left < 0) {
		obj->v.x = fabsf(obj->v.x);
		obj->p.x = obj->r;
	}
	if (right > GetScreenWidth()) {
		obj->v.x = -fabsf(obj->v.x);
		obj->p.x = GetScreenWidth() - obj->r;
	}
	if (top < 0) {
		obj->v.y = fabsf(obj->v.y);
		obj->p.y = obj->r;
	}
	if (bottom > GetScreenHeight()) {
		obj->v.y = -fabsf(obj->v.y);
		obj->p.y = GetScreenHeight() - obj->r;
	}
}

void handle_collision(Ball *a, Ball *b) {
	// Perfectly elastic collison
	Vector2 normalVec = Vector2Normalize((Vector2){b->p.x - a->p.x, b->p.y - a->p.y});
	Vector2 relativeVel = Vector2Subtract(b->v, a->v);
	float normalVel = Vector2DotProduct(relativeVel, normalVec);
	Vector2 impulseVec = Vector2Scale(normalVec, -1 * normalVel);

	a->v = Vector2Subtract(a->v, impulseVec);
    b->v = Vector2Add(b->v, impulseVec);

	// Separates balls
	float dist = Vector2Distance(a->p, b->p);
	float penetration = (a->r + b->r) - dist;
	Vector2 correction = Vector2Scale(normalVec, penetration / 2);
	a->p.x -= correction.x;
	a->p.y -= correction.y;
	b->p.x += correction.x;
	b->p.y += correction.y;
}

int get_nearby(Quadtree *qt, Ball *b, Ball **nearby, int max) {
    Ball *results[max];
    int n = qt_query(qt, b->p, b->r * 2, results, max);

    int count = 0;
    for (int i = 0; i < n && count < max; i++) {
        Ball *other = results[i];
        if (other->id != b->id)
            nearby[count++] = other;
    }
    return count;
}

void check_collisions(Quadtree *qt, Ball *b) {
    Ball *nearby[64];
    int n = get_nearby(qt, b, nearby, 64);
    for (int i = 0; i < n; i++) {
        if (nearby[i]->id < b->id) continue;
        if (CheckCollisionCircles(
            b->p, b->r,
            nearby[i]->p, nearby[i]->r 
            ))
            handle_collision(b, nearby[i]);
    }
}

