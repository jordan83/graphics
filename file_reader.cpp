#include "file_reader.h"

#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

static string BASE_DIR = "/home/jordan/CLionProjects/graphics/";

static void read_file(std::string file_name, int buf_size, char* buffer) {
    ifstream infile;

    infile.open(file_name.c_str());
    if (!infile) {
        cerr << "Unable to open file: " << file_name << ".";
        exit(1);
    }

    string data;
    int i = 0;
    while(getline(infile, data)) {
        if (i + data.length() + 1 > buf_size) {
            cerr << "buffer is too small";
            exit(1);
        }

        strcpy(buffer + i, data.c_str());
        i += data.length();

        buffer[i] = '\n';
        i++;
    }

    buffer[i] = 0;

    infile.close();
}

void load_shader_source(GLuint shader, string file_name) {
  GLchar buf[1024];
  read_file(BASE_DIR + file_name, 1024, buf);
  const GLchar *source[] = {buf};

  glShaderSource(shader, 1, source, NULL);
}
