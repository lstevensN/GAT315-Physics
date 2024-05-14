#pragma once

typedef struct ncSpring
{
	struct ncBody* body1;
	struct ncBody* body2;

	float restLength;
	float stiffness;

	struct ncSpring* prev;
	struct ncSpring* next;
} ncSpring_t;

extern ncSpring_t* ncSprings;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restLength, float stiffness);
void AddSpring(ncSpring_t* spring);
void DestroySpring(ncSpring_t* spring);
void DestroyAllSprings();

void ApplySpringForce(ncSpring_t* springs);
