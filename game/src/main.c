#include "body.h"
#include "mathf.h"
#include "integrator.h"
#include "world.h"
#include "force.h"

#include <raylib.h>
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 1000

int main(void)
{
	InitWindow(1200, 720, "Physics Engine");
	SetTargetFPS(60);
	HideCursor();

	// initialize world
	ncGravity = (Vector2){ 0, 0 };
	int screenX = GetScreenWidth(), screenY = GetScreenHeight();

	// game loop
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		if (IsMouseButtonPressed(0))
		{
			float angle = GetRandomFloatValue(0, 360);
			Color color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);

			for (int i = 0; i < 100; i++)
			{
				ncBody* body = CreateBody();

				body->position = position;
				body->mass = GetRandomFloatValue(1, 10);
				body->inverseMass = 1 / body->mass;
				body->type = BT_DYNAMIC;
				body->damping = 0.9f;
				body->gravityScale = 5;
				//body->color = color;
				body->color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);

				//Vector2 force = Vector2Scale(GetVector2FromAngle((-90 * GetRandomFloatValue(-30, 30)) * DEG2RAD), GetRandomFloatValue(1000, 2000));
				//Vector2 force = Vector2Scale(GetVector2FromAngle((-30 + GetRandomFloatValue(-30, 30)) * DEG2RAD), GetRandomFloatValue(1000, 2000));
				Vector2 force = Vector2Scale(GetVector2FromAngle((angle * GetRandomFloatValue(-30, 30)) * DEG2RAD), GetRandomFloatValue(1000, 2000));

				ApplyForce(body, force, FM_IMPULSE);
			}
		}

		// apply force
		ApplyGravitation(ncBodies, 30);

		// update bodies
		for (ncBody* body = ncBodies; body; body = body->next) Step(body, dt);

		// render
		BeginDrawing();
		ClearBackground(BLACK);

		// stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 5, YELLOW);

		// draw bodies
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			/*if ((int)body->position.x < 0 || (int)body->position.x > screenX || (int)body->position.y < 0 || (int)body->position.y > screenY) { body = body->prev; DestroyBody(body->next); }
			else DrawCircle((int)body->position.x, (int)body->position.y, body->mass, body->color);*/

			DrawCircle((int)body->position.x, (int)body->position.y, body->mass, body->color);
		}
		EndDrawing();
	}

	CloseWindow();

	while (ncBodies) DestroyBody(ncBodies);

	return 0;
}