#include "hzpch.h"
#include "Application.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Hazel
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		HZ_PROFILE_FUNCTION();
		
		HZ_CORE_ASSERT(s_Instance == nullptr, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		HZ_PROFILE_FUNCTION();
	}

	void Application::Run()
	{
		HZ_PROFILE_FUNCTION();
		
		while (m_Running)
		{
			HZ_PROFILE_SCOPE("RunLoop");

			const float time = static_cast<float>(glfwGetTime());
			const Timestep st = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					HZ_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(st);
				}

				m_ImGuiLayer->Begin();
				{
					HZ_PROFILE_SCOPE("LayerStack OnImGuiRender");
					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}
			
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		HZ_PROFILE_FUNCTION();
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			if (e.Handled)
				break;
			
			(*--it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		HZ_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		HZ_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(layer);
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();
		
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;		// 窗口最小化
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}