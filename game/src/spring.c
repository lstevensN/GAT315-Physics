#include "spring.h"

#include <raylib.h>
#include <stdlib.h>
#include <assert.h>

ncSpring_t* ncSprings = NULL;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restLength, float stiffness)
{
	ncSpring_t* spring = (ncSpring_t*)malloc(sizeof(ncSpring_t));
	assert(spring);

	memset(spring, 0, sizeof(ncSpring_t));

	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restLength;
	spring->stiffness = stiffness;

	return spring;
}

void AddSpring(ncSpring_t* spring)
{
	assert(spring);

	spring->prev = NULL;
	spring->next = ncSprings;

	if (ncSprings) ncSprings->prev = spring;

	ncSprings = spring;
}

void DestroySpring(ncSpring_t* spring)
{
	assert(spring);

	if (spring->prev) spring->prev->next = spring->next;
	if (spring->next) spring->next->prev = spring->prev;

	if (ncSprings == spring) ncSprings = spring->next;

	free(spring);
}

void DestroyAllSprings()
{

}

void ApplySpringForce(ncSpring_t* springs)
{

}