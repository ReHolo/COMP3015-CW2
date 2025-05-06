#pragma once
#include <vector>
#include <assert.h>

static const uint32_t s_MaxFrameBufferSize = 8192;
enum class FramebufferTextureFormat
{
	None = 0,

	// Color
	RGBA8,
	RGB32F,
	RED_INTEGER,

	// Depth/stencil
	DEPTH24STENCIL8,
	DEPTH32,
	// Defaults
	Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification
{
	FramebufferTextureSpecification() = default;
	FramebufferTextureSpecification(FramebufferTextureFormat format)
		: TextureFormat(format) {}

	FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
};

struct FramebufferAttachmentSpecification
{
	FramebufferAttachmentSpecification() = default;
	FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
		: Attachments(attachments) {}

	std::vector<FramebufferTextureSpecification> Attachments;
};

struct FramebufferSpecification
{
	uint32_t Width = 0, Height = 0;
	FramebufferAttachmentSpecification Attachments;
	uint32_t Samples = 1;

	bool SwapChainTarget = false;
};

class Framebuffer
{
public:
	Framebuffer(const FramebufferSpecification& spec);
	~Framebuffer();

	/*重新生成FrameBuffer*/
	void Invalidate();

	void Bind() ;
	void Unbind() ;

	uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const 
	{
		assert(index < m_ColorAttachments.size()); 
		return m_ColorAttachments[index];
	}
	uint32_t GetDepthAttachmentRendererID() const
	{
		return m_DepthAttachment;
	}
	void ClearAttachment(uint32_t attachmentIndex, int value);


	const FramebufferSpecification& GetSpecification() const { return m_Specification; }

	void Resize(uint32_t width, uint32_t height) ;
	int ReadPixel(uint32_t attachmentIndex, int x, int y) ;

private:
	uint32_t m_RendererID = 0;
	FramebufferSpecification m_Specification;

	std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
	FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

	std::vector<uint32_t> m_ColorAttachments;
	uint32_t m_DepthAttachment = 0;
};
