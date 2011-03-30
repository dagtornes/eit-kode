#include <GL/glew.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "util.h"

/*
 * Boring, non-OpenGL-related utility functions
 */

static short le_short(unsigned char *bytes)
{
    return bytes[0] | ((char)bytes[1] << 8);
}

void *read_tga(const char *filename, int *width, int *height)
{
    struct tga_header {
       char  id_length;
       char  color_map_type;
       char  data_type_code;
       unsigned char  color_map_origin[2];
       unsigned char  color_map_length[2];
       char  color_map_depth;
       unsigned char  x_origin[2];
       unsigned char  y_origin[2];
       unsigned char  width[2];
       unsigned char  height[2];
       char  bits_per_pixel;
       char  image_descriptor;
    } header;
    int i, color_map_size, pixels_size;
    FILE *f;
    size_t read;
    void *pixels;

    f = fopen(filename, "rb");

    if (!f) {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    read = fread(&header, 1, sizeof(header), f);

    if (read != sizeof(header)) {
        fprintf(stderr, "%s has incomplete tga header\n", filename);
        fclose(f);
        return NULL;
    }
    if (header.data_type_code != 2) {
        fprintf(stderr, "%s is not an uncompressed RGB tga file\n", filename);
        fclose(f);
        return NULL;
    }
    if (header.bits_per_pixel != 24) {
        fprintf(stderr, "%s is not a 24-bit uncompressed RGB tga file\n", filename);
        fclose(f);
        return NULL;
    }

    for (i = 0; i < header.id_length; ++i)
        if (getc(f) == EOF) {
            fprintf(stderr, "%s has incomplete id string\n", filename);
            fclose(f);
            return NULL;
        }

    color_map_size = le_short(header.color_map_length) * (header.color_map_depth/8);
    for (i = 0; i < color_map_size; ++i)
        if (getc(f) == EOF) {
            fprintf(stderr, "%s has incomplete color map\n", filename);
            fclose(f);
            return NULL;
        }

    *width = le_short(header.width); *height = le_short(header.height);
    pixels_size = *width * *height * (header.bits_per_pixel/8);
    pixels = malloc(pixels_size);

    read = fread(pixels, 1, pixels_size, f);

    if (read != pixels_size) {
        fprintf(stderr, "%s has incomplete image\n", filename);
        fclose(f);
        free(pixels);
        return NULL;
    }

    return pixels;
}

void extents(const aiMesh* mesh)
{
	float minx = 0.0, maxx = 0.0;
	float miny = 0.0, maxy = 0.0;
	float minz = 0.0, maxz = 0.0;
	for (int i = 0; i != mesh->mNumVertices; ++i) {
		minx = std::min(minx, mesh->mVertices[i].x);
		maxx = std::max(maxx, mesh->mVertices[i].x);
		minz = std::min(minz, mesh->mVertices[i].z);
		maxz = std::max(maxz, mesh->mVertices[i].z);
		miny = std::min(miny, mesh->mVertices[i].y);
		maxy = std::max(maxy, mesh->mVertices[i].y);
	}
	std::cout << "(" << minx << ", " << miny << ", " << minz << ") (" <<
						maxx << ", " << maxy << ", " << maxz << ")" << std::endl;
}