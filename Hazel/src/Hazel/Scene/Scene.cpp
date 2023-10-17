#include "hzpch.h"
#include "Scene.h"

#include "Hazel/Scene/Components.h"
#include "Hazel/Renderer/Renderer2D.h"

namespace Hazel
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    void Scene::OnUpdate(Timestep ts)
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (entt::entity entity : group)
        {
            auto&[transform, spriteRenderer] = m_Registry.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::DrawQuad(transform, spriteRenderer.Color);
        }
    }

    entt::entity Scene::CreateEntity()
    {
        return m_Registry.create();
    }
}