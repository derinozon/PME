#pragma once

using namespace PME;
using namespace std;

#include "../PME.h"
#include "../include/dukglue/dukglue.h"

bool is_mod_2(int a) { return (a % 2) == 0; }

void jstest () {
	duk_context *ctx = duk_create_heap_default();
	// duk_eval_string(ctx, "1+2");
	// printf("1+2=%d\n", (int) duk_get_int(ctx, -1));

	dukglue_register_function(ctx, &is_mod_2, "is_mod_2");
	duk_eval_string(ctx, "is_mod_2(3)");
	Debug::Log( duk_get_boolean(ctx, -1) );



	dukglue_register_constructor<Entity>(ctx, "Entity");
	dukglue_register_method(ctx, &Entity::AddComponent<SpriteRenderer>, "AddRenderer");
	dukglue_register_method(ctx, &Entity::GetComponent<Transform>, "GetTransform");

	dukglue_register_function(ctx, &Debug::Log<string>, "Log");
	// dukglue_register_function(ctx, &Debug::Log<Vector2>, "Log2");

	duk_eval_string(ctx, "var ent = new Entity(); var sr = ent.AddRenderer(); Log('ent.GetTransform().position');");

	duk_destroy_heap(ctx);
}