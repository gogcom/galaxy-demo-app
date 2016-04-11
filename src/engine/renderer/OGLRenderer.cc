#include "OGLRenderer.h"
#include <engine/core/SDLResourceManager.h>

using namespace gogtron;
using namespace gogtron::renderer;

OGLRenderer::OGLRenderer(const system::SDLWindowPtr& _window)
	: AbstractRenderer(_window)
{
}

bool OGLRenderer::StartScene()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return true;
}

bool OGLRenderer::EndScene()
{
	SDL_GL_SwapWindow(window->GetHandle());
	return true;
}

bool OGLRenderer::DisplayText(const std::string& text, const Sprite& sprite, const std::string& fontName, const Color& color)
{
	int texture = core::SDLResourceManager::GetInstance().GetTexture(fontName);
	if (!texture)
	{
		const core::FontPtr<TTF_Font>& font = core::SDLResourceManager::GetInstance().GetFont("FreeSans");
		if (!core::SDLResourceManager::GetInstance().CreateTextureFromFont(font, text, color, fontName))
			return false;

		texture = core::SDLResourceManager::GetInstance().GetTexture(fontName);
	}

	return DisplayTexture(texture, sprite);
}

bool OGLRenderer::DisplayTexture(int texture, const Sprite& sprite)
{
	float x = static_cast<float>(sprite.GetX()) / (window->GetWidth() / 2);
	float y = static_cast<float>(sprite.GetY()) / (window->GetHeight() / 2);
	float w = static_cast<float>(sprite.GetWidth()) / (window->GetWidth() / 2);
	float h = static_cast<float>(sprite.GetHeight()) / (window->GetHeight() / 2);

	float x1 = x;
	float x2 = x + w;
	float y1 = -y;
	float y2 = -h - y;

	glLoadIdentity();
	glTranslatef(-1.0f, 1.0f, 0);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(x1, y1, 0);
	glTexCoord2f(1, 0);
	glVertex3f(x2, y1, 0);
	glTexCoord2f(1, 1);
	glVertex3f(x2, y2, 0);
	glTexCoord2f(0, 1);
	glVertex3f(x1, y2, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool OGLRenderer::Display3DGrid()
{
	glColor3ub(0, 255, 0);

	for (float i = -50; i <= 50; i += 1)
	{
		glBegin(GL_LINES);
		glVertex3f(-50, 0, i);
		glVertex3f(50, 0, i);
		glVertex3f(i, 0, -50);
		glVertex3f(i, 0, 50);
		glEnd();
	}

	glColor3ub(255, 255, 255);

	return true;
}

bool OGLRenderer::DisplayCube(const glm::vec3& position, const glm::vec4& color)
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	if (color.w != 1)
		glScalef(1, 0.5, 1);

	glBegin(GL_QUADS);
	glColor4f(color.x, color.y, color.z, color.w);
	glVertex3f(0.5f, 1.0f, -0.5f);
	glVertex3f(-0.5f, 1.0f, -0.5f);
	glVertex3f(-0.5f, 1.0f, 0.5f);
	glVertex3f(0.5f, 1.0f, 0.5f);

	glColor4f(color.x, color.y, color.z, color.w);
	glVertex3f(0.5f, 0.0f, 0.5f);
	glVertex3f(-0.5f, 0.0f, 0.5f);
	glVertex3f(-0.5f, 0.0f, -0.5f);
	glVertex3f(0.5f, 0.0f, -0.5f);

	glColor4f(color.x, color.y, color.z, color.w);
	glVertex3f(0.5f, 1.0f, 0.5f);
	glVertex3f(-0.5f, 1.0f, 0.5f);
	glVertex3f(-0.5f, 0.0f, 0.5f);
	glVertex3f(0.5f, 0.0f, 0.5f);

	glColor4f(color.x, color.y, color.z, color.w);
	glVertex3f(0.5f, 0.0f, -0.5f);
	glVertex3f(-0.5f, 0.0f, -0.5f);
	glVertex3f(-0.5f, 1.0f, -0.5f);
	glVertex3f(0.5f, 1.0f, -0.5f);

	glColor4f(color.x, color.y, color.z, color.w);
	glVertex3f(-0.5f, 1.0f, 0.5f);
	glVertex3f(-0.5f, 1.0f, -0.5f);
	glVertex3f(-0.5f, 0.0f, -0.5f);
	glVertex3f(-0.5f, 0.0f, 0.5f);

	glColor4f(color.x, color.y, color.z, color.w);
	glVertex3f(0.5f, 1.0f, -0.5f);
	glVertex3f(0.5f, 1.0f, 0.5f);
	glVertex3f(0.5f, 0.0f, 0.5f);
	glVertex3f(0.5f, 0.0f, -0.5f);
	glEnd();
	glPopMatrix();

	glColor3f(255, 255, 255);
	return true;
}

bool OGLRenderer::DisplayTrial(const glm::vec3& position, float angle, const glm::vec3& rotation, const glm::vec4& color)
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);

	glBegin(GL_QUADS);

	if (rotation.x > 0)
	{
		glColor4f(color.x, color.y, color.z, color.w);
		glVertex3f(0.5f, 1.0f, 0.5f);
		glVertex3f(-0.5f, 1.0f, 0.5f);
		glVertex3f(-0.5f, 0.0f, 0.5f);
		glVertex3f(0.5f, 0.0f, 0.5f);
	}
	else if (rotation.x < 0)
	{
		glColor4f(color.x, color.y, color.z, color.w);
		glVertex3f(0.5f, 0.0f, -0.5f);
		glVertex3f(-0.5f, 0.0f, -0.5f);
		glVertex3f(-0.5f, 1.0f, -0.5f);
		glVertex3f(0.5f, 1.0f, -0.5f);

	}
	else if (rotation.z > 0)
	{
		glColor4f(color.x, color.y, color.z, color.w);
		glVertex3f(-0.5f, 1.0f, 0.5f);
		glVertex3f(-0.5f, 1.0f, -0.5f);
		glVertex3f(-0.5f, 0.0f, -0.5f);
		glVertex3f(-0.5f, 0.0f, 0.5f);
	}
	else
	{
		glColor4f(color.x, color.y, color.z, color.w);
		glVertex3f(0.5f, 1.0f, -0.5f);
		glVertex3f(0.5f, 1.0f, 0.5f);
		glVertex3f(0.5f, 0.0f, 0.5f);
		glVertex3f(0.5f, 0.0f, -0.5f);
	}

	glEnd();

	glPopMatrix();

	glColor3f(255, 255, 255);
	return true;
}