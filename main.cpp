#include <iostream>

#include <sb7.h>

static const GLchar *vertex_shader_source[] = {
        "#version 450 core                           \n"
        "                                            \n"
        "void main(void)                             \n"
        "{                                           \n"
        "   gl_Position = vec4(0.0, 0.0, 0.5, 1.0);  \n"
        "}                                           \n"
};

static const GLchar * fragment_shader_source[] = {
        "#version 450 core                           \n"
        "                                            \n"
        "out vec4 color                              \n"
        "                                            \n"
        "void main(void)                             \n"
        "{                                           \n"
        "   color = vec4(0.0, 0.8, 1.0, 1.0);        \n"
        "}                                           \n"
};


class simpleclear_app : public sb7::application {

private:

    GLuint rendering_program;
    GLuint vertext_array_object;

public:

    void init() override {
        static const char title[] = "Point";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void startup() override {
        rendering_program = compile_shaders();
        glCreateVertexArrays(1, &vertext_array_object);
        glBindVertexArray(vertext_array_object);
    }

    void shutdown() override {
        glDeleteVertexArrays(1, &vertext_array_object);
        glDeleteProgram(rendering_program);
    }

    void render(double currentTime) override {
        const GLfloat color[] = {
                (float)sin(currentTime) * 0.5f + 0.5f,
                (float)cos(currentTime) * 0.5f + 0.5f,
                0.0f,
                1.0f
        };

        glPointSize(40.0f);

        glClearBufferfv(GL_COLOR, 0, color);

        glUseProgram(rendering_program);

        glDrawArrays(GL_POINTS, 0, 1);
    }

    GLuint compile_shaders() {
        // Create and compile vertex shader
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
        glCompileShader(vertex_shader);

        // Create and compile fragment shader
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
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
};

//DECLARE_MAIN(simpleclear_app)