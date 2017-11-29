
#include <sb7.h>
#include <vector>
#include "file_reader.h"

class TriangleApp : public sb7::application {

private:

    GLuint rendering_program;
    GLuint vertex_array_object;

    GLuint compile_shaders() {
        // Create and compile vertex shader
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        load_shader_source(vertex_shader, "triangle/vertex.glsl");
        glCompileShader(vertex_shader);

        // Create and compile fragment shader
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        load_shader_source(fragment_shader, "triangle/fragment.glsl");
        glCompileShader(fragment_shader);

        // Create and compile tess control shader
        GLuint tess_control_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
        load_shader_source(tess_control_shader, "triangle/tess_control.glsl");
        glCompileShader(tess_control_shader);

        // Create and compile tess eval shader
        GLuint tess_eval_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
        load_shader_source(tess_eval_shader, "triangle/tess_eval.glsl");
        glCompileShader(tess_eval_shader);

        // Create program, attach shaders, link
        GLuint program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glAttachShader(program, tess_control_shader);
        glAttachShader(program, tess_eval_shader);
        glLinkProgram(program);

        // Delete the shaders as the program now has them
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteShader(tess_control_shader);
        glDeleteShader(tess_eval_shader);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        return program;
    }

public:

    void init() override {
        static const char title[] = "Triangle";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void startup() override {
        rendering_program = compile_shaders();
        glCreateVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);
    }

    void shutdown() override {
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteProgram(rendering_program);
    }

    void render(double currentTime) override {
        const GLfloat color[] = {0.0f, 0.2f, 0.0f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, color);

        glUseProgram(rendering_program);

        // Pass an offset to the vertex shader.
        /*GLfloat attrib[] = {
                (float)sin(currentTime) * 0.5f,
                (float)cos(currentTime) * 0.6f,
                0.0f,
                0.0f
        };
        glVertexAttrib4fv(0, attrib);

        GLfloat tri_color[] = {0.0f, 0.8f, 1.0f, 1.0f};
        glVertexAttrib4fv(1, tri_color);*/

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_PATCHES, 0, 3);
    }
};

//DECLARE_MAIN(TriangleApp)
