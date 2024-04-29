#include "body.h"
#include "world.h"
#include "raylib.h"
#include <stdlib.h>
#include <raymath.h>
#include <assert.h>

#define MAX_BODIES 1000

int main(void)
{
	InitWindow(1200, 720, "Physics Engine");
	SetTargetFPS(60);

	// game loop
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		if (IsMouseButtonDown(0))
		{
			Body* body = CreateBody();

			body->position = position;
			body->velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
		}

		// render
		BeginDrawing();
		ClearBackground(BLACK);

		// stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 20, YELLOW);

		Body* body = bodies;

		// update bodies
		while (body)
		{
			body->position = Vector2Add(body->position, body->velocity);
			DrawCircle((int)body->position.x, (int)body->position.y, 10, RED);

			body = body->next;
		}

		EndDrawing();
	}

	CloseWindow();

	while (bodies) DestroyBody(bodies);

	return 0;
}