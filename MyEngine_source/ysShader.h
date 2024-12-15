#pragma once
#include "ysResource.h"

namespace ys::graphics
{
	class Shader : public Resource
	{
	public:
		enum class DispatchMode
		{
			dispatch8x8x1 = 0, dispatch16x16x1 = 3, dispatch32x32x1 = 6
		};

		Shader();
		virtual ~Shader();

		virtual HRESULT Load(const std::wstring& path) override;

		bool Create(const std::wstring stage, const std::wstring& fileName);

		GLuint CreateVertexShader(std::wstring path);
		GLuint CreateFragmentShader(std::wstring path);
		GLuint CreateComputeShader(std::wstring path);
		GLuint GetShaderID() const { return shaderID; }

		void Bind() const
		{
			glUseProgram(shaderID);
		}

		void DispatchCompute(glm::vec2 screenSize, DispatchMode mode = DispatchMode::dispatch8x8x1)
		{
			if (isComputeShader)
			{
				glm::uvec2 dispatchCommand;
				switch (mode) 
				{
				case DispatchMode::dispatch8x8x1: 
					dispatchCommand.x = (GLuint)ceil(screenSize.x / 8.0); 
					dispatchCommand.y = (GLuint)ceil(screenSize.y / 8.0); 
					break;
				case DispatchMode::dispatch16x16x1:
					dispatchCommand.x = (GLuint)ceil(screenSize.x / 16.0);
					dispatchCommand.y = (GLuint)ceil(screenSize.y / 16.0);
					break;
				case DispatchMode::dispatch32x32x1:
					dispatchCommand.x = (GLuint)ceil(screenSize.x / 32.0);
					dispatchCommand.y = (GLuint)ceil(screenSize.y / 32.0);
					break;
				}
				glDispatchCompute(dispatchCommand.x, dispatchCommand.y, 1);
				glMemoryBarrier(GL_ALL_BARRIER_BITS);
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
	};
}