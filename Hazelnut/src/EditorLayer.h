#pragma once
#include <Hazel.h>

namespace Hazel
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;
		void OnImGuiRender() override;
	private:
		OrthographicCameraController m_CameraController;
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		Ref<Texture2D> m_CheckerboardTexture;
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	};
}