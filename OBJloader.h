// the OBJloader provided in class

#include <glm/glm.hpp>
#include <cstring>
#include <vector>
#include <string>
#include <stdlib.h>

bool loadOBJ(
	const char * path,
	std::vector<int> & vertexIndices,
	std::vector<glm::vec3> & temp_vertices,
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::vec2> & out_uvs){

	std::vector<int> uvIndices, normalIndices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

    FILE * file = fopen(path, "r");
    if (file == NULL) {
        printf("Impossible to open the file ! Are you in the right path ?\n");
        getchar();
        return false;
    }

	while (1){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader,128);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0){
			int vertexIndex[3], uvIndex[3], normalIndex[3];
			bool uv = true;
			bool norm = true;
            char line [128];
            fgets(line, 128, file);
            //vertex, uv, norm
            int matches = sscanf(line, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9){
                //vertex, norm
                matches = sscanf(line, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
                if (matches != 6){
                    //vertex, uv
                    matches = sscanf(line, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
                    if (matches != 6){
                        //vertex
                        matches = sscanf(line, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                        if (matches != 6){
                            printf("File can't be read by our simple parser. 'f' format expected: d/d/d d/d/d d/d/d || d/d d/d d/d || d//d d//d d//d\n");
                            printf("Character at %d", ftell(file));
                            return false;
                        }
                        uv, norm = false;
                    }else{
                        norm = false;
                    }
                }else{
                    uv = false;
                }
            }
            vertexIndices.push_back(abs(vertexIndex[0])-1);
            vertexIndices.push_back(abs(vertexIndex[1])-1);
            vertexIndices.push_back(abs(vertexIndex[2])-1);
            if(norm){
            normalIndices.push_back(abs(normalIndex[0])-1);
            normalIndices.push_back(abs(normalIndex[1])-1);
            normalIndices.push_back(abs(normalIndex[2])-1);
            }
            if(uv){
            uvIndices.push_back(abs(uvIndex[0])-1);
            uvIndices.push_back(abs(uvIndex[1])-1);
            uvIndices.push_back(abs(uvIndex[2])-1);
            }
		}
		else{
			char clear[1000];
			fgets(clear, 1000, file);
		}

	}
	if (normalIndices.size() != 0)
		out_normals.resize(temp_normals.size());
	if (uvIndices.size() != 0)
		out_uvs.resize(temp_normals.size());
	for (unsigned int i = 0; i<vertexIndices.size(); i++) {
		int vi = vertexIndices[i];
		if (normalIndices.size() != 0) {
			int ni = normalIndices[i];
			out_normals[vi] = temp_normals[ni];
		}
		if (uvIndices.size() != 0 && i < uvIndices.size()) {
			int ui = uvIndices[i];
			out_uvs[vi] = temp_uvs[ui];
		}
	}



	return true;
}
