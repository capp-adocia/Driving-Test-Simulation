/*****************************************************************//**
 * \file   FrameBuffer.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "../core.h"
#include "../texture.h"

class FrameBuffer
{
public:
	FrameBuffer() = default;
	explicit FrameBuffer(unsigned int width, unsigned int height);
	~FrameBuffer();

	inline unsigned int getFBO() const { return mFBO; }
	inline unsigned int getWidth() const { return mWidth; }
	inline unsigned int getHeight() const { return mHeight; }
	inline std::shared_ptr<Texture> getColorAttachment() const { return mColorAttachment; }
	inline std::shared_ptr<Texture> getDepthStencilAttachment() const { return mDepthStencilAttachment; }
	inline void setFBO(unsigned int fbo) { mFBO = fbo; }
	inline void setWidth(unsigned int width) { mWidth = width; }
	inline void setHeight(unsigned int height) { mHeight = height; }
	inline void setColorAttachment(std::shared_ptr<Texture> colorAttachment) { mColorAttachment = colorAttachment; }
	inline void setDepthStencilAttachment(std::shared_ptr<Texture> depthStencilAttachment) { mDepthStencilAttachment = depthStencilAttachment; }
	inline void setDepthAttachment(std::shared_ptr<Texture> depthAttachment) { mDepthAttachment = depthAttachment; }
	static std::shared_ptr<FrameBuffer> createMultiSampleFbo(const unsigned int width, const unsigned int height, const unsigned int samples);

private:
	unsigned int mWidth;
	unsigned int mHeight;

	unsigned int mFBO;
	std::shared_ptr<Texture> mColorAttachment;
	std::shared_ptr<Texture> mDepthStencilAttachment;
	std::shared_ptr<Texture> mDepthAttachment;
};
#endif // !FRAMEBUFFER_H