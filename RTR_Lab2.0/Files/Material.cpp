#include "Material.h"

using namespace std;

Material :: Material()
{
	ambient = diffuse = specular = cool = warm = glm::vec3(0.0f);
	shininess = beckmann_m = refraction = 0.0f;
	eta = glm::vec3(0.0f);
}


void Material :: load_material(const aiScene* scene, bool &has_tex, const char *filename)
{
	//get material
	if (scene->mNumMaterials > 0)
	{
		const aiMaterial* a_mat = scene->mMaterials[0];


		int num_textures = a_mat->GetTextureCount(aiTextureType_DIFFUSE);

		aiString tex_filename;
		if(num_textures > 0)
		{
			has_tex = true;
			a_mat->GetTexture(aiTextureType_DIFFUSE, 0, &tex_filename);
			string dir = filename;
			dir = dir.substr(0, dir.find_last_of('/'));
			dir += "/";
			dir += tex_filename.C_Str();
			const char* s_tex = dir.c_str();
			texture.addImageFile(s_tex);
		}
		else
			has_tex = false;
		
	
		
		aiColor4D a_diffuse;
		aiGetMaterialColor(a_mat, AI_MATKEY_COLOR_DIFFUSE, &a_diffuse);

		aiColor4D a_specular;
		aiGetMaterialColor(a_mat, AI_MATKEY_COLOR_SPECULAR, &a_specular);

		float a_shininess = 0.0f;
		aiGetMaterialFloat(a_mat, AI_MATKEY_SHININESS, &a_shininess);


		if(!has_tex)
			diffuse = glm::vec3(a_diffuse.r, a_diffuse.g, a_diffuse.b);
		else
			diffuse = glm::vec3(1.0f);


		specular = glm::vec3(a_specular.r, a_specular.g, a_specular.b);
		shininess = a_shininess;

		beckmann_m = 0.3f;
		refraction = 0.9f;

		eta = glm::vec3(0.8f);

		reflection = 0.5f;

		ambient = (diffuse) * (0.1f);

		cool = glm::vec3(0.0, 0.0, 1.0);
		warm = glm::vec3(1.0, 1.0, 0.4);
	}
}

