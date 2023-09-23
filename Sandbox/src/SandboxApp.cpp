#include <Hazel.h>

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example")
	{}

	void OnUpdate() override {
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
			HZ_TRACE("Tab key is pressed (poll)!");
	}

	void OnEvent(Hazel::Event& event) override {
		if (event.GetEventType() == Hazel::EventType::KeyPressed) {
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			HZ_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

	void OnImGuiRender() override {

	}
};

class Sandbox : public Hazel::Application {
public:
	Sandbox(){
		PushLayer(new ExampleLayer());
	}

	~Sandbox(){}
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}