#include "pch.h"
#include "GLTexture2D.h"

#include "glad/gl.h"

#include "Data/TextureData.h"

#include <iostream>
#include <Debug/Logger.h>

void GLTexture2D::Initialize(Texture2DData& data)
{
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (data.FilteringNear == TextureFiltering::Point ? GL_NEAREST : GL_LINEAR));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (data.FilteringFar == TextureFiltering::Point ? GL_NEAREST : GL_LINEAR));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (data.Repeating == TextureRepeating::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (data.Repeating == TextureRepeating::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT));

	uint32_t ch = 0;
	switch (data.Channels)
	{
	case TextureChannels::Red:
		ch = GL_R8;
		break;
	case TextureChannels::Grayscale:
		ch = GL_RG8;
		break;
	case TextureChannels::RGB:
		ch = GL_RGB8;
		break;
	case TextureChannels::RGBA:
		ch = GL_RGBA8;
		break;
	default:
		ch = GL_RGB8;
		break;
	}

	Debug("Selected #", min(max(data.Mipmaps, 1), log2(max(data.Width, data.Height))), " mipmaps total");

	glTexStorage2D(GL_TEXTURE_2D, min(max(data.Mipmaps, 1), log2(max(data.Width, data.Height))), ch, data.Width, data.Height);
	
	for (size_t i = 0; i < min(max(data.Mipmaps, 1), log2(max(data.Width, data.Height))); i++)
		glTexSubImage2D(GL_TEXTURE_2D, i, 0, 0, data.Width / (i + 1), data.Height / (i + 1), GL_RGBA, GL_UNSIGNED_BYTE, data.Pixels);
}

void GLTexture2D::Shutdown()
{
	glDeleteTextures(1, &m_Texture);
}

void GLTexture2D::Bind(uint32_t Slot)
{
	glBindTexture(GL_TEXTURE_2D, m_Texture);
}

uint32_t GLTexture2D::GetTextureId()
{
	return m_Texture;
}
