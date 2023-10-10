#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Hazel
{
	class HazelEditorApp : public Application
	{
	public:
		HazelEditorApp()
			: Application("Hazel Editor")
		{
			PushLayer(new EditorLayer());
		}

		~HazelEditorApp() override = default;
	};

	Application* CreateApplication()
	{
		return new HazelEditorApp();
	}
}