#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <vector>

struct LayoutElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalize;

	static size_t getTypeSize(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:   return 4;
			case GL_UNSIGNED_BYTE:  return 4;
		}
		// �������ִ�е���˴�����ôû�к��ʵ����ʹ�С�ɹ�ѡ��ֱ�ӱ���
		assert(false);
		return 0;
	}

};

class VertexBufferLayout
{
public:
	VertexBufferLayout() : m_Stride(0) {}
	~VertexBufferLayout() {}

	template<typename T>
	void Push(unsigned int count)
	{
		assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * LayoutElement::getTypeSize(GL_FLOAT);
	}

	const std::vector<LayoutElement>& getElements() const
	{
		return m_Elements;
	}

	unsigned int getStride() const
	{
		return m_Stride;
	}

private:
	std::vector<LayoutElement> m_Elements;
	unsigned int m_Stride;
};