#include "Graphics.h"

Graphics *g = NULL;

bool loop_func() {
	return true;
}

int main() {
	g = new Graphics("Alpha Gomoku", 800, 600);
	
	g->main_loop(loop_func);
	
	delete g;
}
