

struct class {
	size_t size;
	void * (* ctor) (void * self, va_list *app);
	void * (* dtor) (void * self);
	void * (* clone) (const void * self);
	int (* differ)(const void * self, const void * b);
};

void * new (const void * class_arg, ... ) {
	const struct class * c = class_arg;
	void * p = calloc(1, c-> size);

	assert(p);
	*(const struct class **)p = c;

	if(c -> ctor) {
		va_list ap;

		va_start(ap, class_arg);
		p = c -> ctor(p, &ap);
		va_end(ap);
	}

	return p;
}


