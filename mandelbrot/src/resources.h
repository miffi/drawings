#ifndef _RESOURCES_H_
#define _RESOURCES_H_
GLuint load_shader(const char* file);
GLuint load_program(const GLuint shaders[], size_t n);
//GLuint load_texture(const char* file, GLuint dimensions, GLuint params[][2], size_t len);
#endif
