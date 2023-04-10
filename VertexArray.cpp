#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_VAO);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void VertexArray::BindBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const
{
	Bind();
	vb.Bind();
	const auto& Elements = layout.getElements();
	unsigned int stride = layout.getStride();
	unsigned int offset = 0;
	for (int i = 0; i < Elements.size(); ++i)
	{
		const auto& layoutElement = Elements[i];
		glVertexAttribPointer(i, layoutElement.count, layoutElement.type, layoutElement.normalize,
			layout.getStride(), (void*)offset);
		glEnableVertexAttribArray(i);
		offset += layoutElement.count * LayoutElement::getTypeSize(layoutElement.type);
	}
	UnBind();
	vb.UnBind();
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_VAO);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}

