#include "body.h"
#include "mathf.h"
#include "integrator.h"
#include "world.h"
#include "force.h"
#include "render.h"
#include "editor.h"
#include "spring.h"

#include <raylib.h>
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 1000

int main(void)
{
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;

	InitWindow(1200, 720, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	// initialize world
	int screenX = GetScreenWidth(), screenY = GetScreenHeight();
	ncScreenZoom = 10;

	// game loop
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();
		ncGravity = (Vector2){ 0, ncEditorData.GravityValue };

		Vector2 position = GetMousePosition();
		ncScreenZoom -= GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 20);

		UpdateEditor(position);

		selectedBody = GetBodyIntersect(ncBodies, position);
		if (selectedBody)
		{
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines((int)screen.x, (int)screen.y, ConvertWorldToPixel(selectedBody->mass) + 5, YELLOW);
		}

		if (!ncEditorIntersect && IsMouseButtonPressed(0))
		{
			float angle = GetRandomFloatValue(0, 360);
			Color color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);

			for (int i = 0; i < 1; i++)
			{
				ncBody* body = CreateBody(ConvertScreenToWorld(position), ncEditorData.MassValue, ncEditorData.BodyTypeActive);

				body->damping = ncEditorData.DampingValue;
				body->gravityScale = ncEditorData.GravityScaleValue;
				body->color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);

				AddBody(body);
			}
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody)
		{
			if (selectedBody && selectedBody != connectBody)
			{
				ncSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), 20);
				AddSpring(spring);
			}
		}

		// apply force
		ApplyGravitation(ncBodies, ncEditorData.GravitationValue);
		ApplySpringForce(ncSprings);

		// update bodies
		for (ncBody* body = ncBodies; body; body = body->next) Step(body, dt);

		// render
		BeginDrawing();
		ClearBackground(BLACK);

		// stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		if (!ncEditorIntersect) { DrawCircle((int)position.x, (int)position.y, ConvertWorldToPixel(1), YELLOW); HideCursor(); }
		else ShowCursor();

		// draw bodies
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			/*if ((int)body->position.x < 0 || (int)body->position.x > screenX || (int)body->position.y < 0 || (int)body->position.y > screenY) { body = body->prev; DestroyBody(body->next); }
			else DrawCircle((int)body->position.x, (int)body->position.y, body->mass, body->color);*/

			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, body->mass, body->color);
		}

		// draw springs
		for (ncSpring_t* spring = ncSprings; spring; spring = spring->next)
		{
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine((int)screen1.x, (int)screen1.y, (int)screen2.x, (int)screen2.y, YELLOW);
		}

		DrawEditor();

		EndDrawing();
	}

	CloseWindow();

	while (ncBodies) DestroyBody(ncBodies);

	return 0;
}