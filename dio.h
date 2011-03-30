#ifndef DIO_H
#define DIO_H

#include <stdio.h>
#include <string>
#include <fstream>

/*
Helper func for printing debug data and reading
custom filedata 
*/

void *file_contents(const char *filename, GLint *length)
{
    FILE *f = fopen(filename, "r");
    void *buffer;
	
    if (!f) {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }
	
    fseek(f, 0, SEEK_END);
    *length = ftell(f);
    fseek(f, 0, SEEK_SET);
	
    buffer = malloc(*length+1);
    *length = fread(buffer, 1, *length, f);
    fclose(f);
    ((char*)buffer)[*length] = '\0';
	
    return buffer;
}

static std::string LoadShaderFromFile(const std::string& name)
{
    std::string ret, line;
    std::ifstream f(name.c_str());
	std::cout << ((f.is_open()) ? std::string("No error") : std::string("Error")) << std::endl;
    while(std::getline(f, line))
		ret += line + std::string("\n");
	std::cout << ret << std::endl;
    return ret;
}



#endif