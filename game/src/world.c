#include "world.h"
#include "body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBodies = NULL;
int ncBodyCount = 0;
Vector2 ncGravity;

ncBody* CreateBody()
{
	ncBody* body = (ncBody*)malloc(sizeof(ncBody));
	assert(body);

	memset(body, 0, sizeof(ncBody));

	body->prev = NULL;
	body->next = ncBodies;

	if (ncBodies) ncBodies->prev = body;

	ncBodies = body;
	ncBodyCount++;

	return body;
}

void DestroyBody(ncBody* body)
{
	assert(body);

	if (body->prev) body->prev->next = body->next;
	if (body->next) body->next->prev = body->prev;

	if (ncBodies == body) ncBodies = body->next;

	ncBodyCount--;
	free(body);
}