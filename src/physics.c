#include "physics.h"
#include "raylib.h"
#include "raymath.h"

void sim_physics(Ball *obj, float dt) {
    obj->p.x += obj->v.x * dt;
	obj->p.y += obj->v.y * dt;

	obj->v.x += obj->a.x * dt;
	obj->v.y += obj->a.y * dt;
}

Color velocity_to_color(Vector2 v) {
    float speed = Vector2Length(v);
    
    float minSpeed = 0.0f;
    float maxSpeed = 500.0f; // tune this to your typical max speed
    
    float t = Clamp(speed, minSpeed, maxSpeed) / maxSpeed; // 0.0 to 1.0
    float hue = (1.0f - t) * 240.0f; // 240 (blue) -> 0 (red)
    
    return ColorFromHSV(hue, 1.0f, 1.0f);
}