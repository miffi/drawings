#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include"galogen-gl.h"
#include<GLFW/glfw3.h>
#include<cglm/cglm.h>

#include"resources.h"

#define WIDTH 1366
#define HEIGHT 768
float map(float value, float low1, float high1, float low2, float high2){
  return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

float* check_mouse(GLFWwindow* window){
  float* mouse = calloc(4, sizeof(float));

  if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
    mouse[0] = 1;
  }
  if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
    mouse[1] = 1;
  }

  double x, y;
  glfwGetCursorPos(window, &x, &y);
  mouse[2] = x;
  mouse[3] = y;

  return mouse;
}

int main(){
  //{{{ glfw window creation
  if(!glfwInit()){
    return -1;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mandelbrot Set", NULL, NULL);
  if(!window){
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  //}}}

  //{{{ data
  const float buffer[] = {
    1,  1,
    1, -1,
    -1, -1,
    -1,  1,
  };

  float coords[] = {
    1,  1,
    1, -1,
    -2.5, -1,
    -2.5,  1,
  };

  GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
  };

  GLuint vao, buffers[3];
  glGenVertexArrays(1, &vao);
  glGenBuffers(3, buffers);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
  //}}}
  //{{{ program initialisation
  GLuint shaders[] = {
    load_shader("resources/shader.frag"),
    load_shader("resources/shader.vert")
  };
  GLuint program = load_program(shaders, sizeof(shaders)/sizeof(shaders[0]));
  //}}}

  mat4 trans;
  float panx = 0.05,
    pany = 0.05,
    escape = 1000;
  glm_mat4_identity(trans);
  while(!glfwWindowShouldClose(window)){
    if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
      glm_translate_x(trans, panx);
    }
    if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){
      glm_translate_x(trans, -panx);
    }
    if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
      glm_translate_y(trans, pany);
    }
    if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
      glm_translate_y(trans, -pany);
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
      glm_scale(trans, (vec3){1/1.005, 1/1.005, 0});
    }
    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
      glm_scale(trans, (vec3){1.005, 1.005, 0});
    }

    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
      escape += 10;
    }

    glUniformMatrix4fv(glGetUniformLocation(program, "trans"), 1, GL_FALSE, (float*)trans);
    glUniform1f(glGetUniformLocation(program, "escape"), escape);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}
