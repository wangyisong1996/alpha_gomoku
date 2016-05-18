#include "graphics.h"

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>

#include <thread>

static void error_callback(int error, const char *description) {
	fprintf(stderr, "%s\n", description);
	fflush(stderr);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	
}

Graphics::Graphics(const char *title, int width, int height) {
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
}

Graphics::~Graphics() {
	
}

v2d Graphics::get_cursor_pos() {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return v2d(x / height, 1.0 - y / height);
}

bool Graphics::get_key_state(int key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Graphics::get_mouse_state(int btn) {
	return glfwGetMouseButton(window, btn) == GLFW_PRESS;
}

void Graphics::render_main_loop(bool (*call_back)()) {
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	
	while (!glfwWindowShouldClose(window)) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		// printf("w = %d, h = %d\n", width, height);fflush(stdout);
		float ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClearColor(1, 1, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, ratio, 0, 1, 1, -1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		if (!call_back()) {
			break;
		}
		
		glfwSwapBuffers(window);
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Graphics::main_loop(bool (*call_back)()) {
	std::thread render_thread(&Graphics::render_main_loop, this, call_back);
	
	while (1) {
		glfwWaitEvents();
	}
}


void Graphics::draw_line(double x1, double y1, double x2, double y2) {
	// x1 = x1 * render_y;
	// y1 = (1.0 - y1) * render_y;
	// x2 = x2 * render_y;
	// y2 = (1.0 - y2) * render_y;
	// draw_line_p(x1, y1, x2, y2);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void Graphics::draw_rect(double x1, double y1, double x2, double y2,
	double x3, double y3, double x4, double y4) {
	// draw_line(x1, y1, x2, y2);
	// draw_line(x2, y2, x3, y3);
	// draw_line(x3, y3, x4, y4);
	// draw_line(x4, y4, x1, y1);
	
	glBegin(GL_POLYGON);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);
	glEnd();
}

void Graphics::draw_line(const v2d &a, const v2d &b) {
	draw_line(a.x, a.y, b.x, b.y);
}

void Graphics::draw_rect(const v2d &a, const v2d &b,
	const v2d &c, const v2d &d) {
	draw_rect(a.x, a.y, b.x, b.y, c.x, c.y, d.x, d.y);
}

void Graphics::draw_text_p(int x, int y, const char *s) {
	// TextOut(hdc_back, x, y, s, strlen(s));
}

void Graphics::draw_text(const v2d &a, const char *s) {
	// draw_text_p(a.x * render_y, (1.0 - a.y) * render_y, s);
}

void Graphics::draw_circle(const v2d &a, double R) {
	const int cnt = 36;
	double s = sin(2.0 * PI / cnt), c = cos(2.0 * PI / cnt);
	v2d tmp(R, 0);
	
	glBegin(GL_POLYGON);
	glColor3f(0.5f, 0.5f, 1.0f);
	for (int i = 0; i < cnt; i++) {
		glVertex2f(a.x + tmp.x, a.y + tmp.y);
		tmp = Rotate(tmp, s, c);
	}
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 0.0f);
	tmp = v2d(R, 0);
	for (int i = 0; i < cnt; i++) {
		glVertex2f(a.x + tmp.x, a.y + tmp.y);
		tmp = Rotate(tmp, s, c);
	}
	glEnd();
}

void Graphics::draw_polygon(int n, const v2d *A) {
	glBegin(GL_POLYGON);
	glColor3f(0.5f, 1.0f, 0.5f);
	for (int i = 0; i < n; i++) {
		glVertex2f(A[i].x, A[i].y);
	}
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < n; i++) {
		glVertex2f(A[i].x, A[i].y);
	}
	glEnd();
}
