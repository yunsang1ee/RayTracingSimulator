#pragma once
#include <ysResources.h>
#include <ysShader.h>
#include <ysMesh.h>

namespace ys
{
	void ResourceLoad();
}

void ys::ResourceLoad()
{
	Resources::Load<graphics::Shader>(L"mix", L"mix,VS,FS");

	Resources::Load<graphics::Shader>(L"raytracing8", L"legeno8,CS");
	Resources::Load<graphics::Shader>(L"raytracing16", L"legeno16,CS");
	Resources::Load<graphics::Shader>(L"raytracing32", L"legeno32,CS");
	Resources::Load<graphics::Shader>(L"renderTexture", L"renderTexture,FS");

	Resources::Load<graphics::Shader>(L"test", L"test,VS,FS");

	Resources::Load<graphics::Shader>(L"phong", L"phong,VS,FS");

	Resources::Load<graphics::Shader>(L"vc", L"vc,VS,FS");

	Resources::Load<Mesh>(L"Cube", L"Cube");
	Resources::Load<Mesh>(L"Sphere", L"Sphere");
	//Resources::Load<Mesh>(L"Bunny", L"stanford-bunny");
	//Resources::Load<Mesh>(L"Dragon", L"stanford-dragon");
	return;
}