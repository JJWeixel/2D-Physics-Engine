/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"
#include "physics.h"
#include "stdio.h"

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	QtPool pool = {0};

	int left = R;
	int right = GetScreenWidth() - R;
	int top = R;
	int bottom = GetScreenHeight() - R;
	int maxVel = 500;
	Ball balls[NUM_PARTICLES];
	for (int i = 0; i < NUM_PARTICLES; i++) {
		int xVel = GetRandomValue(-maxVel, maxVel);
		int yVel = GetRandomValue(-maxVel, maxVel);
		int yPos = GetRandomValue(top, bottom);
		int xPos = GetRandomValue(left, right);
		balls[i] = (Ball){R, {xPos, yPos}, {xVel, yVel}, {0, G}, i};
	}
	
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		double t0 = GetTime();
		float dt = GetFrameTime();
		pool_reset(&pool);
    	Quadtree *qt = qt_create(&pool, (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()});

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// physics
		for (int i = 0; i < NUM_PARTICLES; i++) {
			sim_physics(&balls[i], dt);
		}
		double t1 = GetTime();

		// ball collisions
		for (int i = 0; i < NUM_PARTICLES; i++) {
			qt_insert(&pool, qt, &balls[i]);
		}
		double t2 = GetTime();

		for (int i = 0; i < NUM_PARTICLES; i++) {
			check_collisions(qt, &balls[i]);
		}

		// boundaries
		for (int i = 0; i < NUM_PARTICLES; i++) {
			check_boundaries(&balls[i]);
		}
		double t3 = GetTime();

		// drawing
		for (int i = 0; i < NUM_PARTICLES; i++) {
			Color color = velocity_to_color(balls[i].v);
			DrawPoly(balls[i].p, 8, balls[i].r, 0, color);
		}

		qt_draw(qt);

		double t4 = GetTime();

		DrawText(TextFormat("insert: %.2fms", (t2-t1)*1000), 10, 30, 20, WHITE);
		DrawText(TextFormat("collide: %.2fms", (t3-t2)*1000), 10, 50, 20, WHITE);
		DrawText(TextFormat("total time: %.2fms", (t4-t0)*1000), 10, 70, 20, WHITE);

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}