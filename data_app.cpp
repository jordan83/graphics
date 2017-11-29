#include <sb7.h>
#include <vmath.h>
#include <vector>
#include "file_reader.h"

class DataApp : public sb7::application {

private:

    GLuint rendering_program;
    GLuint vertex_array_object;

    GLuint compile_shaders() {
        // Create and compile vertex shader
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        load_shader_source(vertex_shader, "data/vertex.glsl");
        glCompileShader(vertex_shader);

        // Create and compile fragment shader
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        load_shader_source(fragment_shader, "data/fragment.glsl");
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

public:

    void init() override {
        static const char title[] = "Data";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void startup() override {
        rendering_program = compile_shaders();
        glCreateVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);

        GLuint buffer;

        // Two triangles
        vmath::vec4 vertices[] = {
          vmath::vec4( 0.25, -0.25, 0.5, 1.0),
          vmath::vec4(-0.25, -0.25, 0.5, 1.0),
          vmath::vec4( 0.25,  0.25, 0.5, 1.0),
          vmath::vec4(-0.25, -0.25, 0.5, 1.0),
          vmath::vec4(-0.25,  0.25, 0.5, 1.0),
          vmath::vec4( 0.25,  0.25, 0.5, 1.0),
        };

        // Create buffer to store vertices (init with vertex data)
        glCreateBuffers(1, &buffer);
        glNamedBufferStorage(
          buffer,
          sizeof(vertices),
          vertices,
          0
        );
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        // Bind buffer to one of the vertex buffer bindings (how many per vao?)
        glVertexArrayVertexBuffer(vertex_array_object, 0, buffer, 0, sizeof(vmath::vec4));

        // Tell opengl how the data is laid out.
        glVertexArrayAttribFormat(vertex_array_object, 0, 4, GL_FLOAT, GL_FALSE, 0);

        glEnableVertexArrayAttrib(vertex_array_object, 0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void shutdown() override {
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteProgram(rendering_program);
    }

    void render(double currentTime) override {
        const GLfloat color[] = {0.0f, 0.2f, 0.0f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, color);

        glUseProgram(rendering_program);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
};

//DECLARE_MAIN(DataApp)
