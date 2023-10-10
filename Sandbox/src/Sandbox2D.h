#pragma once
#include <Hazel.h>

class EditorLayer : public Hazel::Layer {
public:
	EditorLayer();
	~EditorLayer() override = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Hazel::Timestep ts) override;
	void OnEvent(Hazel::Event& e) override;
	void OnImGuiRender() override;
private:
	Hazel::OrthographicCameraController m_CameraController;

	// temp
	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};