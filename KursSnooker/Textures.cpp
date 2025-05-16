#include "Textures.h"

void Textures::Bind(ShaderProgram& shaderProgram)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if(name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);

		glUniform1i(glGetUniformLocation(shaderProgram.GetShaderProgramID(), (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
}

void Textures::InsertTexture(Textures tex)
{
	textures.insert(textures.end(), tex.Begin(), tex.End());
}

void Textures::AddTexture(Texture tex)
{
	textures.push_back(tex);
}

std::vector<Texture>::iterator Textures::Begin()
{
	return textures.begin();
}

std::vector<Texture>::iterator Textures::End()
{
	return textures.end();
}

