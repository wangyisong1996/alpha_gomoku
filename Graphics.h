#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "math.h"

struct GLFWwindow;

class Graphics {
	public:
	Graphics(const char *title, int width, int height);
	~Graphics();
	
	v2d get_cursor_pos();
	bool get_key_state(int key);
	bool get_mouse_state(int btn);
	
	// main loop. Will exit when callback returns false
	void main_loop(bool (*callback)());
	
	// coordinates: (0, 0) ~ (x / y, 1)
	void draw_line(double x1, double y1, double x2, double y2);
	
	void draw_rect(double x1, double y1, double x2, double y2,
		double x3, double y3, double x4, double y4);
	
	void draw_line(const v2d &a, const v2d &b);
	void draw_rect(const v2d &a, const v2d &b, const v2d &c, const v2d &d);
	
	void draw_text_p(int x, int y, const char *s);
	void draw_text(const v2d &a, const char *s);
	
	void draw_circle(const v2d &a, double R);
	
	void draw_polygon(int n, const v2d *A);
	
	private:
	GLFWwindow *window;
	
	void render_main_loop(bool (*call_back)());
};

#endif
