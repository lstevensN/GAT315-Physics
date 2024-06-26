#pragma once
#include "raylib.h"

typedef struct ncEditorData
{
	float MassValue;
	float GravityValue;
	float GravitationValue;
	bool BodyTypeEditMode;
	int BodyTypeActive;
	float DampingValue;
	float GravityScaleValue;
	float StiffnessValue;
	float FixedTimeStepValue;

	bool Simulate;
	bool Reset;
} ncEditorData_t;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 mousePosition);
void DrawEditor();

struct ncBody* GetBodyIntersect(struct ncBody* bodies, Vector2 position); void DrawLineBodyToPosition(struct ncBody* body, Vector2 position);
