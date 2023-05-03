#include "Mesh.h"

#define DIFFUSE(x) ("texture_diffuse"#x)
#define SPECULAR(x) ("specular_diffuse"#x)

Mesh::Mesh(std::vector<MyVertex> vertices, std::vector<unsigned int> indices, std::vector<MyTexture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}


Mesh::Mesh(const Mesh& m)
{
	this->vertices = m.vertices;
	this->indices = m.indices;
	this->textures = m.textures;

	setupMesh();

}

void Mesh::setupMesh()
{
	m_Vbo.Assign(&vertices[0], vertices.size() * sizeof(MyVertex));
	m_Ibo.Assign(&indices[0], indices.size());
	m_Layout.Push<float>(3);
	m_Layout.Push<float>(3);
	m_Layout.Push<float>(2);
	m_Vao.BindBuffer(m_Vbo, m_Layout);
}


void Mesh::Draw(ShaderProgram& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		Texture& texture = textures[i].texture;
		std::string name = textures[i].type;
		std::string textureName;
		if (name == "texture_diffuse")
			textureName = "texture_diffuse" + std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			textureName = "specular_diffuse" + std::to_string(specularNr++);
		shader.SetInt((/*"material." + */textureName).c_str(), i);
		texture.Bind(i);
	}

	Renderer::render(m_Vao, m_Ibo, shader);
}
