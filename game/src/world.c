#include "world.h"
#include <stdlib.h>
#include <assert.h>

Body* bodies = NULL;
int bodyCount = 0;

Body* CreateBody()
{
	Body* body = malloc(sizeof(Body));
	assert(body);

	body->prev = NULL;
	body->next = bodies;

	if (bodies) bodies->prev = body;

	bodies = body;
	bodyCount++;

	return body;
}

void DestroyBody(Body* body)
{
	assert(body);

	if (body->prev) body->prev->next = body->next;
	if (body->next) body->next->prev = body->prev;

	if (bodies == body) bodies = body->next;

	bodyCount--;
	free(body);
}