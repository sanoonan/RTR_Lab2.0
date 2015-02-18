#include "CubeMap.h"

using namespace std;

CubeMap :: CubeMap()
{
	
}

CubeMap :: CubeMap(string folder, const char *mesh_name)
{
	file_prefix = folder;
	mesh = Mesh(mesh_name);
}


bool CubeMap :: loadToTexture()
{
	int n;
	int force_channels = 4;
	int _ix, _iy;

	const char *suffixes[] = 
	{
		"posx", "negx", "posy", "negy", "posz", "negz"
	};

	GLuint targets[] = 
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

	for(int i=0; i<6; i++)
	{
		string filename = file_prefix + suffixes[i] + ".jpg";
		const char *file_name = filename.c_str();

		unsigned char* _img = stbi_load(file_name, &_ix, &_iy, &n, force_channels);

		if(!_img)
		{
			fprintf(stderr, "ERROR: could not load image %s. Check file type and path\n", file_name);
			return false;
		}

		// FLIP UP-SIDE DIDDLY-DOWN
		// make upside-down copy for GL
		unsigned char* imagePtr = &_img[0];
		int half_height_in_pixels = _iy/2;
		int height_in_pixels = _iy;

		//assuming RGBA for 4 components per pixel
		int num_colour_components = 4;

		//assuming each colour component is an unsigned char
		int width_in_chars = _ix * num_colour_components;

		unsigned char* top = NULL;
		unsigned char* bottom = NULL;
		unsigned char temp = 0;
		/*
		for(int h=0; h<half_height_in_pixels; h++)
		{
			top = imagePtr + h * width_in_chars;
			bottom = imagePtr + (height_in_pixels - h - 1) * width_in_chars;

			for(int w=0; w<width_in_chars; w++)
			{
				//swap the chars around
				temp = *top;
				*top = *bottom;
				*bottom = temp;
				++top;
				++bottom;
			}
		}
		*/
		//copy into an OpenGL texture
	
		glTexImage2D(targets[i], 0, GL_RGBA, _ix, _iy, 0, GL_RGBA, GL_UNSIGNED_BYTE, _img);
	}



	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubeMap :: draw(GLuint spID)
{
	glm::mat4 model_mat = glm::scale(glm::mat4(), glm::vec3(20.0f));

	mesh.draw(spID, model_mat);
}