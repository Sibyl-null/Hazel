#include "hzpch.h"
#include "OpenGLFramebuffer.h"
#include <glad/glad.h>

namespace Hazel
{
    namespace Utils
    {
        static GLenum TextureTarget(bool multiSampled)
        {
            return multiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void CreateTextures(bool multiSampled, uint32_t* outId, uint32_t count)
        {
            glCreateTextures(TextureTarget(multiSampled), count, outId);
        }

        static void BindTexture(bool multiSampled, uint32_t id)
        {
            glBindTexture(TextureTarget(multiSampled), id);
        }

        static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format,
            uint32_t width, uint32_t height, int index)
        {
            const bool multiSampled = samples > 1;
            if (multiSampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples,
                    format, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height,
                    0, format, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
                TextureTarget(multiSampled), id, 0);
        }

        static void AttachDepthTexture(uint32_t id, int samples, GLenum format,
            GLenum attachmentType, uint32_t width, uint32_t height)
        {
            const bool multiSampled = samples > 1;
            if (multiSampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples,
                    format, width, height, GL_FALSE);
            }
            else
            {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType,
                TextureTarget(multiSampled), id, 0);
        }

        static bool IsDepthFormat(FramebufferTextureFormat format)
        {
            switch (format)
            {
            case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
            }

            return false;
        }
    }
    
    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
        : m_Spec(spec)
    {
        for (FramebufferTextureSpecification sp : m_Spec.Attachments.Attachments)
        {
            if (!Utils::IsDepthFormat(sp.TextureFormat))
                m_ColorAttachmentSpecifications.emplace_back(sp);
            else
                m_DepthAttachmentSpecification = sp;
        }
        
        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(m_ColorAttachments.size(),  m_ColorAttachments.data());
        glDeleteTextures(1,  &m_DepthAttachment);
    }

    void OpenGLFramebuffer::Invalidate()
    {
        if (m_RendererID != 0)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(m_ColorAttachments.size(),  m_ColorAttachments.data());
            glDeleteTextures(1,  &m_DepthAttachment);

            m_ColorAttachments.clear();
            m_DepthAttachment = 0;
        }
        
        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        const bool multiSample = m_Spec.Samples > 1;

        // Attachments
        if (!m_ColorAttachmentSpecifications.empty())
        {
            m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
            Utils::CreateTextures(multiSample, m_ColorAttachments.data(), m_ColorAttachments.size());

            for (uint32_t i = 0; i < m_ColorAttachments.size(); ++i)
            {
                Utils::BindTexture(multiSample, m_ColorAttachments[i]);
                switch (m_ColorAttachmentSpecifications[i].TextureFormat)
                {
                case FramebufferTextureFormat::RGBA8:
                    Utils::AttachColorTexture(m_ColorAttachments[i], m_Spec.Samples,
                        GL_RGBA8, GL_RGB, m_Spec.Width, m_Spec.Height, i);
                    break;
                case FramebufferTextureFormat::RED_INTEGER:
                    Utils::AttachColorTexture(m_ColorAttachments[i], m_Spec.Samples, GL_R32I, GL_RED_INTEGER,
                        m_Spec.Width, m_Spec.Height, i);
                    break;
                }
            }
        }

        if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
        {
            Utils::CreateTextures(multiSample, &m_DepthAttachment, 1);
            Utils::BindTexture(multiSample, m_DepthAttachment);
            switch (m_DepthAttachmentSpecification.TextureFormat)
            {
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                Utils::AttachDepthTexture(m_DepthAttachment, m_Spec.Samples,GL_DEPTH24_STENCIL8,
                    GL_DEPTH_STENCIL_ATTACHMENT, m_Spec.Width, m_Spec.Height);
                break;
            }
        }

        if (m_ColorAttachments.size() > 1)
        {
            HZ_CORE_ASSERT(m_ColorAttachments.size() <= 4, "ColorAttachments Count > 4")
            constexpr GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(m_ColorAttachments.size(), buffers);
        }
        else if (m_ColorAttachments.empty())
        {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
        }
        
        // Check
        HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Spec.Width, m_Spec.Height);
    }

    void OpenGLFramebuffer::UnBind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        m_Spec.Width = width;
        m_Spec.Height = height;

        Invalidate();
    }

    int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }
}
