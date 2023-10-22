#include "hzpch.h"
#include "Scene.h"

#include "Entity.h"
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
        Camera* mainCamera = nullptr;
        glm::mat4* cameraTransform = nullptr;

        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for (const entt::entity entity : view)
            {
                auto& [transform, camera] = m_Registry.get<TransformComponent, CameraComponent>(entity);
                if (camera.Primary == true)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = &transform.Transform;
                    break;
                }
            }
        }

        if (mainCamera != nullptr)
        {
            Renderer2D::BeginScene(*mainCamera, *cameraTransform);
            
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (const entt::entity entity : group)
            {
                auto& [transform, spriteRenderer] = m_Registry.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::DrawQuad(transform, spriteRenderer.Color);
            }

            Renderer2D::EndScene();
        }
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        // Resize our non-FixedAspectRatio cameras
        auto view = m_Registry.view<CameraComponent>();
        for (const entt::entity entity : view)
        {
            auto& cameraComponent = m_Registry.get<CameraComponent>(entity);
            if (cameraComponent.FixedAspectRatio == false)
                cameraComponent.Camera.SetViewportSize(width, height);
        }
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
    }
}