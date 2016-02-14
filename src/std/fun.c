#include <hl.h>

vclosure *hl_alloc_closure_void( hl_type *t, void *fvalue ) {
	vclosure *c = (vclosure*)hl_gc_alloc(sizeof(vclosure));
	c->t = t;
	c->fun = fvalue;
	c->bits = 0;
	return c;
}

vclosure *hl_alloc_closure_i32( hl_type *t, void *fvalue, int v32 ) {
	vclosure *c = (vclosure*)hl_gc_alloc(sizeof(vclosure));
	c->t = t;
	c->fun = fvalue;
	c->bits = CL_HAS_V32;
	c->v32 = v32;
	return c;
}

vclosure *hl_alloc_closure_i64( hl_type *t, void *fvalue, int64 v64 ) {
	vclosure *c = (vclosure*)hl_gc_alloc(sizeof(vclosure));
	c->t = t;
	c->fun = fvalue;
	c->bits = CL_HAS_V64;
	c->v64 = v64;
	return c;
}
