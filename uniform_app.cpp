#include <sb7.h>
#include <vmath.h>
#include <vector>
#include "file_reader.h"

class UniformApp : public sb7::application {

private:

    GLuint rendering_program;
    GLuint vertex_array_object;
    vmath::mat4 projection_matrix;
    GLuint mv_location;
    GLuint buffer;
    GLuint projection_location;

    GLuint compile_shaders() {
        // Create and compile vertex shader
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        load_shader_source(vertex_shader, "uniform/vertex.glsl");
        glCompileShader(vertex_shader);

        // Create and compile fragment shader
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        load_shader_source(fragment_shader, "uniform/fragment.glsl");
        glCompileShader(fragment_shader);

        // Create program, attach shaders, link
        GLuint program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        // Delete the shaders as the program now has them
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program;
    }

    vmath::mat4 build_model_view_matrix(int i, double current_time) {
      float f = (float)i + (float)current_time * 0.3f;
      /*return
        vmath::translate(0.0f, 0.0f, -4.0f) *
        vmath::translate(sinf(2.1f * f) * 0.5f, cosf(1.7f * f) * 2.0f, sinf(1.3f * f) * cosf(1.5f * f) * 2.0f) *
        vmath::rotate((float)current_time * 45.0f, 0.0f, 1.0f, 0.0f) *
        vmath::rotate((float)current_time * 81.0f, 1.0f, 0.0f, 0.0f);*/

        return vmath::translate(0.0f, 0.0f, -6.0f) *
              vmath::rotate((float)current_time * 45.0f, 0.0f, 1.0f, 0.0f) *
              vmath::rotate((float)current_time * 21.0f, 1.0f, 0.0f, 0.0f) *
              vmath::translate(sinf(2.1f * f) * 2.0f,
                               cosf(1.7f * f) * 2.0f,
                               sinf(1.3f * f) * cosf(1.5f * f) * 2.0f);
    }

public:

    void init() override {
        static const char title[] = "Uniform";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void startup() override {
        rendering_program = compile_shaders();

        mv_location = glGetUniformLocation(rendering_program, "mv_matrix");
        projection_location = glGetUniformLocation(rendering_program, "proj_matrix");

        glCreateVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);

        static const GLfloat vertex_positions[] = {
          -0.25f,  0.25f, -0.25f,
          -0.25f, -0.25f, -0.25f,
           0.25f, -0.25f, -0.25f,

           0.25f, -0.25f, -0.25f,
           0.25f,  0.25f, -0.25f,
          -0.25f,  0.25f, -0.25f,

           0.25f, -0.25f, -0.25f,
           0.25f, -0.25f,  0.25f,
           0.25f,  0.25f, -0.25f,

           0.25f, -0.25f,  0.25f,
           0.25f,  0.25f,  0.25f,
           0.25f,  0.25f, -0.25f,

           0.25f, -0.25f,  0.25f,
          -0.25f, -0.25f,  0.25f,
           0.25f,  0.25f,  0.25f,

          -0.25f, -0.25f,  0.25f,
          -0.25f,  0.25f,  0.25f,
           0.25f,  0.25f,  0.25f,

          -0.25f, -0.25f,  0.25f,
          -0.25f, -0.25f, -0.25f,
          -0.25f,  0.25f,  0.25f,

          -0.25f, -0.25f, -0.25f,
          -0.25f,  0.25f, -0.25f,
          -0.25f,  0.25f,  0.25f,

          -0.25f, -0.25f,  0.25f,
           0.25f, -0.25f,  0.25f,
           0.25f, -0.25f, -0.25f,

           0.25f, -0.25f, -0.25f,
          -0.25f, -0.25f, -0.25f,
          -0.25f, -0.25f,  0.25f,

          -0.25f,  0.25f, -0.25f,
           0.25f,  0.25f, -0.25f,
           0.25f,  0.25f,  0.25f,

           0.25f,  0.25f,  0.25f,
          -0.25f,  0.25f,  0.25f,
          -0.25f,  0.25f, -0.25f
        };

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        onResize(info.windowWidth, info.windowHeight);
    }

    void shutdown() override {
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteProgram(rendering_program);
        glDeleteBuffers(1, &buffer);
    }

    void render(double currentTime) override {
        const GLfloat color[] = {0.0f, 0.2f, 0.0f, 1.0f};
        static const GLfloat one = 1.0f;

        glViewport(0, 0, info.windowWidth, info.windowHeight);
        glClearBufferfv(GL_COLOR, 0, color);
        glClearBufferfv(GL_DEPTH, 0, &one);

        glUseProgram(rendering_program);

        glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection_matrix);

        for (int i = 0; i < 24; i++) {
          glDrawArrays(GL_TRIANGLES, 0, 36);
          glUniformMatrix4fv(mv_location, 1, GL_FALSE, build_model_view_matrix(i, currentTime));
        }
    }

    void onResize(int w, int h) {
      sb7::application::onResize(w, h);
      float aspect = (float)w / (float)h;
      projection_matrix = vmath::perspective(50.0f, aspect, 0.1f, 1000.0f);
    }
};

DECLARE_MAIN(UniformApp)
