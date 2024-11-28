#include "ysSpriteRenderer.h"
#include "ysGameObject.h"
#include "ysTransform.h"
#include "ysTexture.h"
#include "ysRenderer.h"
#include "ysTimer.h"
#include "ysInputManager.h"
#include <cassert>
#include <ysCollider.h>
#include <iostream>

ys::SpriteRenderer::SpriteRenderer()
	: Component(enums::ComponentType::SpriteRenderer)
	, texture(nullptr), shader(nullptr), mesh(nullptr)
	//, size (glm::vec2(1.0f, 1.0f)), offsetLT(glm::vec2()), offsetRB(glm::vec2())
{
}

ys::SpriteRenderer::~SpriteRenderer()
{
}

void ys::SpriteRenderer::Init()
{
}

void ys::SpriteRenderer::Update()
{
}

void ys::SpriteRenderer::LateUpdate()
{
}

void ys::SpriteRenderer::Render(HDC hDC)
{
	shader->Bind();
	unsigned int projectionLocation = glGetUniformLocation(shader->GetShaderID(), "projectionTrans"); //--- 투영 변환 값 설정
	unsigned int viewLocation = glGetUniformLocation(shader->GetShaderID(), "viewTrans"); //--- 뷰잉 변환 설정
	unsigned int transformLocation = glGetUniformLocation(shader->GetShaderID(), "worldTrans");

	glm::mat4 projection = renderer::mainCamera->GetmainProjectionMatrix(); //--- 투영 공간 설정: fovy, aspect, near, far
	glm::mat4 view = renderer::mainCamera->GetmainViewMatrix();
	glm::mat4 worldTrans = GetOwner()->GetComponent<Transform>()->GetWorldMatrix();
	//axisWorldTrans = glm::scale(axisWorldTrans, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(worldTrans));

	mesh->Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (void*)offsetof(graphics::Vertex, texture));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (void*)offsetof(graphics::Vertex, normal));
	glEnableVertexAttribArray(2);

	unsigned int isTexture = glGetUniformLocation(shader->GetShaderID(), "isTexture");
	unsigned int viewPos = glGetUniformLocation(shader->GetShaderID(), "viewPos");
	unsigned int lightCount = glGetUniformLocation(shader->GetShaderID(), "lightCount");
	glUniform1i(isTexture, GL_FALSE);
	glUniform3fv(viewPos, 1, glm::value_ptr(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition()));
	struct Light
	{
		glm::vec3 position;
		glm::vec3 color;
	};
	std::vector<Light> lightsInfo;
	for (auto& light : this->lights)
		lightsInfo.emplace_back(light.first->GetComponent<Transform>()->GetPosition(), light.second);

	for (int i = 0; i < lightsInfo.size(); ++i)
	{
		unsigned int lightsPosition = glGetUniformLocation(shader->GetShaderID(), ("lights[" + std::to_string(i) + "].position").c_str());
		unsigned int lightsColor = glGetUniformLocation(shader->GetShaderID(), ("lights[" + std::to_string(i) + "].color").c_str());
		glUniform3fv(lightsPosition, 1, glm::value_ptr(lightsInfo[i].position));
		glUniform3fv(lightsColor, 1, glm::value_ptr(lightsInfo[i].color));
	}	
	glUniform1i(lightCount, lightsInfo.size());

	std::vector<GLfloat> values(10 * 6);
	for (int i = 0; i < lightsInfo.size(); ++i)
	{
		unsigned int lightsPosition = glGetUniformLocation(shader->GetShaderID(), ("lights[" + std::to_string(i) + "].position").c_str());
		unsigned int lightsColor = glGetUniformLocation(shader->GetShaderID(), ("lights[" + std::to_string(i) + "].color").c_str());
		glGetUniformfv(shader->GetShaderID(), lightsPosition, &values[i * 6]);
		glGetUniformfv(shader->GetShaderID(), lightsColor, &values[i * 6 + 3]);
		std::cout << "Light " << i << " Position: (" << values[i * 6] << ", " << values[i * 6 + 1] << ", " << values[i * 6 + 2] << ")" << std::endl;
		std::cout << "Light " << i << " Color: (" << values[i * 6 + 3] << ", " << values[i * 6 + 4] << ", " << values[i * 6 + 5] << ")" << std::endl; 
	}
	int a{};
	glGetUniformiv(shader->GetShaderID(), lightCount, &a);
	std::cout << a << std::endl;

	glDrawElements(mesh->GetUsageType(), mesh->GetIndicesCount(), GL_UNSIGNED_INT, (void*)0);
	mesh->UnBind();
	shader->Unbind();
	//if (texture == nullptr) 
	//	assert(false, L"File error!");
	//auto tr = GetOwner()->GetComponent<Transform>();
	//
	//glm::vec2 position = tr->GetPosition();
	//glm::vec2 scale = tr->GetScale();
	////float rotation = tr->GetRotation();
	//
	//auto cd = GetOwner()->GetComponent<Collider>();
	//if(cd != nullptr && cd->isRender())
	//{
	//	return;
	//}
	//
	//if (texture->GetTextureType() == graphics::Texture::TextureType::Bmp)
	//{
	//	auto check = TransparentBlt(hDC
	//		, position.x
	//		, position.y
	//		, texture->GetWidth() * size.x * scale.x
	//		, texture->GetHeight() * size.y * scale.y
	//		, texture->GetDC()
	//		, offsetLT.x
	//		, offsetLT.y
	//		, texture->GetWidth() - offsetLT.x - offsetRB.x
	//		, texture->GetHeight() - offsetLT.y - offsetRB.y
	//		, RGB(255, 0, 255));
	//	assert(check);
	//}
	//else if (texture->GetTextureType() == graphics::Texture::TextureType::Png)
	//{
	//	Gdiplus::Graphics graphics(hDC);
	//	Gdiplus::ImageAttributes imageAttr;
	//	imageAttr.SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 0, 255));
	//
	//	graphics.TranslateTransform(position.x, position.y);
	//	graphics.RotateTransform(rotation / kPi * 180.0f);
	//	graphics.TranslateTransform(-position.x, -position.y);
	//
	//	graphics.DrawImage(texture->GetImage()
	//		, Gdiplus::Rect(
	//			position.x
	//			, position.y
	//			, texture->GetWidth() * size.x * scale.x
	//			, texture->GetHeight() * size.y * scale.y)
	//		, offsetLT.x
	//		, offsetLT.y
	//		, texture->GetWidth() - offsetLT.x - offsetRB.x
	//		, texture->GetHeight() - offsetLT.y - offsetRB.y
	//		, Gdiplus::UnitPixel, &imageAttr);
	//}
}


