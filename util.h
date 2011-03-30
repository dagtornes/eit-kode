#ifndef UTIL_H
#define UTIL_H

#include <aiMesh.h>

void *read_tga(const char *filename, int *width, int *height);
void extents(const aiMesh* mesh);

#endif