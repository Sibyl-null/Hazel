#pragma once
#include <glm/glm.hpp>

namespace Hazel {
	class HAZEL_API Shader{
	public:
		Shader(const std::string& vertexSrc, const std::string fragmentSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		unsigned int m_RendererID;
	};
}