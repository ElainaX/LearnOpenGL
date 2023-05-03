#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "assimp/Importer.hpp"

#include <string>
#include <vector>

#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

struct MyVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct MyTexture
{
	MyTexture() {}
	MyTexture(const MyTexture& t)
		: texture(t.texture), type(t.type), path(t.path) {}
	Texture texture;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	/* 网格数据 */
	std::vector<MyVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<MyTexture> textures;
	Mesh(std::vector<MyVertex> vertices, std::vector<unsigned int> indices,
		std::vector<MyTexture> textures);
	Mesh(const Mesh& m);
	void Draw(ShaderProgram& shader);

private:
	/* 渲染数据 */
	VertexBuffer m_Vbo;
	VertexArray m_Vao;
	IndexBuffer m_Ibo;
	VertexBufferLayout m_Layout;
	//unsigned int VAO, VBO, EBO;
	/* 函数 */
	void setupMesh();
};