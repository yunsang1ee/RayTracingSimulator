#pragma once
#include "ysResource.h"
#include <ysGraphics.h>

namespace ys::graphics
{
	class Shader : public Resource
	{
	public:
		Shader();
		virtual ~Shader();

		virtual HRESULT Load(const std::wstring& path) override;

		bool Create(const ys::graphics::ShaderStage stage, const std::wstring& fileName);

		GLuint CreateVertexShader(std::wstring path);
		GLuint CreateFragmentShader(std::wstring path);
		GLuint CreateComputeShader(std::wstring path);
		GLuint GetShaderID() const { return shaderID; }

		void Bind() const 
		{
			glUseProgram(shaderID);
			if (computeShader) 
			{
				glDispatchCompute(16, 16, 1);
				glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			}
		}
		void Unbind() const { glUseProgram(0); }

	private:
		GLuint shaderID;
		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint computeShader;
	};
}

