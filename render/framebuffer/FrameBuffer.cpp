/*****************************************************************//**
 * \file   FrameBuffer.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "FrameBuffer.h"
#include "../../util/logger/log.h"

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
	: mWidth(width)
	, mHeight(height)
	, mFBO(0)
	, mColorAttachment(nullptr)
	, mDepthStencilAttachment(nullptr)
	, mDepthAttachment(nullptr)
{
	// 创建一个帧缓冲对象
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	// 生成颜色附件
	setColorAttachment(Texture::createColorAttachmentTexture(getWidth(), getHeight(), 0));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D
		, getColorAttachment()->getTexture(), 0);
	// 生成depth Stencil附件，加入fbo
	setDepthStencilAttachment(Texture::createDepthStencilmentTexture(getWidth(), getHeight(), 0));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT
		, GL_TEXTURE_2D, getDepthStencilAttachment()->getTexture(), 0);

	// 检查帧缓冲对象是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LOG_ERROR("Error:FrameBuffer is not complete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// MASS抗锯齿
std::shared_ptr<FrameBuffer> FrameBuffer::createMultiSampleFbo(const unsigned int width, const unsigned int height, const unsigned int samples)
{
	std::shared_ptr<FrameBuffer> fb = std::make_shared<FrameBuffer>();

	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	auto colorAttachment = Texture::createMultiSampleTexture(width, height, samples, GL_RGBA, 0);
	auto dsAttachment = Texture::createMultiSampleTexture(width, height, samples, GL_DEPTH24_STENCIL8, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorAttachment->getTexture(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, dsAttachment->getTexture(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	fb->setFBO(fbo);
	fb->setDepthAttachment(dsAttachment);
	fb->setColorAttachment(colorAttachment);
	fb->setWidth(width);
	fb->setHeight(height);

	return fb;
}

FrameBuffer::~FrameBuffer()
{
	if (getFBO())
	{
		glDeleteFramebuffers(1, &mFBO);
	}
}