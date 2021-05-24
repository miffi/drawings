#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include"galogen-gl.h"
#include"resources.h"
//#include"headers/stb_image.h"

GLuint shader_type(const char* file);

GLuint load_shader(const char* file){
  GLuint shader = shader_type(file);
  FILE *f = fopen(file, "r");
  if(!f){
    printf("File not found, %s\n", file);
    exit(-1);
  }
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);
  
  char* source = malloc(fsize + 1);
  fread(source, 1, fsize, f);
  fclose(f);

  source[fsize] = 0;

  glShaderSource(shader, 1, (const char**) &source, NULL);
  glCompileShader(shader);
  free(source);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success){
    int info_length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
    char* errorMsg = malloc(info_length + 1);
    glGetShaderInfoLog(shader, info_length, NULL, errorMsg);
    printf("Shader Error, %s: %s\n", file, errorMsg);
    free(errorMsg);
  }

  return shader;
}

GLuint shader_type(const char* file){
  char* ext = strrchr(file, '.');
  if(!ext){
    fprintf(stderr,
        "ERROR %s: No extension found for shader file: %s\n",
        __func__,
        file);
    exit(-1);
  } else if(!strcmp(ext + 1, "vert")){
    return glCreateShader(GL_VERTEX_SHADER);
  } else if(!strcmp(ext + 1, "frag")){
    return glCreateShader(GL_FRAGMENT_SHADER);
  }
  fprintf(stderr,
      "ERROR %s: Shader cannot be determined from extension: %s\n",
      __func__,
      file);
  exit(-1);
}

GLuint load_program(const GLuint shaders[], size_t n){
  GLuint program = glCreateProgram();
  for(size_t i = 0; i < n; i++){
    glAttachShader(program, shaders[i]);
  }
  glLinkProgram(program);
  for(size_t i = 0; i < n; i++){
    glDeleteShader(shaders[i]);
  }

  int success;
  glGetProgramiv(program, GL_COMPILE_STATUS, &success);
  if(!success){
    int info_length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length);
    char* errorMsg = malloc(info_length + 1);
    glGetProgramInfoLog(program, info_length, NULL, errorMsg);
    printf("Program Error: %s\n", errorMsg);
    free(errorMsg);
  }

  return program;
}

GLuint image_type(const char* file){
  char* ext = strrchr(file, '.') + 1;
  if(!ext){
    fprintf(stderr,
        "ERROR %s: No extension found for shader file: %s\n",
        __func__,
        file);
    exit(-1);
  } else if(!strcmp(ext, "png")){
    return GL_RGBA;
  } else if(!strcmp(ext, "jpg")){
    return GL_RGB;
  }
  fprintf(stderr,
      "ERROR %s: Image type cannot be determined from extension: %s\n",
      __func__,
      file);
  exit(-1);
}

/* GLuint load_texture( */
/*     const char* file, GLuint d, GLuint params[][2], size_t len){ */

/*   GLuint texture; */
/*   glGenTextures(1, &texture); */
/*   glBindTexture(d, texture); */

/*   for(size_t i = 0; i < len; i++){ */
/*     glTexParameteri(d, params[i][0], params[i][1]); */
/*   } */

/*   int width, height, nrChannels; */
/*   unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0); */
/*   if(data){ */
/*     glTexImage2D(d, */
/*         0, */
/*         GL_RGB, width, height, */
/*         0, */
/*         image_type(file), GL_UNSIGNED_BYTE, data); */
/*     glGenerateMipmap(d); */
/*   } else { */
/*     fprintf(stderr, "Failed to load texture"); */
/*   } */

/*   stbi_image_free(data); */
/*   return texture; */
/* } */
