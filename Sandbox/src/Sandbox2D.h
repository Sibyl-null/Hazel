#pragma once
#include <Hazel.h>

class Sandbox2D : public Hazel::Layer {
public:
	Sandbox2D();
	~Sandbox2D() override = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Hazel::Timestep st) override;
	void OnEvent(Hazel::Event& e) override;
	void OnImGuiRender() override;
private:
	Hazel::OrthographicCameraController m_CameraController;

	// temp
	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;
};