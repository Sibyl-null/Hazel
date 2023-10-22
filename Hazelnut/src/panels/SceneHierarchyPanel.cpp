#include "SceneHierarchyPanel.h"
#include "imgui/imgui.h"

namespace Hazel
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
    {
        SetContext(context);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
    {
        m_Context = context;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");

        m_Context->m_Registry.each([&](entt::entity entityId)
        {
            const Entity entity{ entityId, m_Context.get() };
            DrawEntityNode(entity);
        });
        
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        std::string& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (m_SelectionContext == entity)
            flags |= ImGuiTreeNodeFlags_Selected;

        const bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
        if (ImGui::IsItemClicked())
            m_SelectionContext = entity;

        if (opened)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            bool op = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
            if (op)
                ImGui::TreePop();
            ImGui::TreePop();
        }
    }
}
