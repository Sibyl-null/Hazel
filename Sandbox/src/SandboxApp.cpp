#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Hazel::Application {
public:
	Sandbox(){
		// PushLayer(new ExampleLayer());
		PushLayer(new EditorLayer());
	}

	~Sandbox(){}
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}