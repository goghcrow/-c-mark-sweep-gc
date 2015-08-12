// debug.c

void debug_printf(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	//vfprintf(stderr, fmt, ap);
	vprintf(fmt, ap);
	va_end(ap);
}

void assert(int condition, const char* msg) {
	if(!condition) {
		printf("%s\n", msg);
		exit(1);
	}
}
