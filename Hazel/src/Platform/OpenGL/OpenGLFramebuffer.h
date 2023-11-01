#pragma once
#include "Hazel/Renderer/Framebuffer.h"

namespace Hazel
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer();

        void Invalidate();
        
        void Bind() const override;
        void UnBind() const override;

        void Resize(uint32_t width, uint32_t height) override;
        
        uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { return m_ColorAttachments[index]; }
        const FramebufferSpecification& GetSpecification() const override { return m_Spec; }
    private:
        uint32_t m_RendererID = 0;
        FramebufferSpecification m_Spec;

        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
        FramebufferTextureSpecification m_DepthAttachmentSpecification;

        std::vector<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment = 0;
    };
}