#pragma once
#include "ysResource.h"

namespace ys::graphics
{
	class Shader : public Resource
	{
	public:
		Shader();
		virtual ~Shader();

		virtual HRESULT Load(const std::wstring& path) override;

		bool Create(const std::wstring stage, const std::wstring& fileName);

		GLuint CreateVertexShader(std::wstring path);
		GLuint CreateFragmentShader(std::wstring path);
		GLuint CreateComputeShader(std::wstring path);
		GLuint GetShaderID() const { return shaderID; }

		void Bind(GLuint computeShaderOffset = 0) const
		{
			glUseProgram(shaderID);
			if (isComputeShader)
			{
				glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, dispatchIndirectBuffer);
				glDispatchComputeIndirect(computeShaderOffset);
				glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			}
		}

		void Unbind() const 
		{
			glUseProgram(0);
			glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, 0);
		}

	private:
		GLuint shaderID;
		GLuint vertexShader;
		GLuint fragmentShader;
		bool isComputeShader;
		GLuint computeShader;
		GLuint dispatchIndirectBuffer;
	};
}