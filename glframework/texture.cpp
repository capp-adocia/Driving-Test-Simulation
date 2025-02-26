/*****************************************************************//**
 * \file   texture.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../application/stb_image.h"
#include "../application/Application.h"

std::unordered_map<std::string, std::shared_ptr<Texture>> Texture::mTextureCache{};

std::shared_ptr<Texture> Texture::createTexture(const std::string& path, unsigned int unit) {
    // 1 检查是否缓存过本路径对应的纹理对象
    auto iter = mTextureCache.find(path);
    if (iter != mTextureCache.end()) {
        // 返回缓存的纹理
        return iter->second;
    }

    // 2 如果本路径对应的 texture 没有生成过，则重新生成
    auto texture = std::make_shared<Texture>(path, unit, GL_SRGB_ALPHA);
    mTextureCache[path] = texture;

    return texture;
}

std::shared_ptr<Texture> Texture::createTexture(
    const std::string& path,
    unsigned int unit,
    unsigned char* dataIn,
    uint32_t widthIn,
    uint32_t heightIn
) {
    // 1 检查是否缓存过本路径对应的纹理对象
    auto iter = mTextureCache.find(path);
    if (iter != mTextureCache.end()) {
        // 返回缓存的纹理
        return iter->second;
    }

    // 2 如果本路径对应的 texture 没有生成过，则重新生成
    auto texture = std::make_shared<Texture>(unit, dataIn, widthIn, heightIn);
    mTextureCache[path] = texture;

    return texture;
}

std::shared_ptr<Texture> Texture::createColorAttachmentTexture(unsigned int width, unsigned int height, unsigned int unit)
{
    return std::make_shared<Texture>(width, height, unit);
}

std::shared_ptr<Texture> Texture::createDepthStencilmentTexture(unsigned int width, unsigned int height, unsigned int unit)
{
    std::shared_ptr<Texture> dsTex = std::make_shared<Texture>();

    unsigned int depthStencil;
    glGenTextures(1, &depthStencil);
    glBindTexture(GL_TEXTURE_2D, depthStencil);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8
        , glApp.getWidth(), glApp.getHeight(), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    dsTex->setTexture(depthStencil);
    dsTex->setWidth(width);
    dsTex->setHeight(height);
    dsTex->setUnit(unit);

    return dsTex;
}

std::shared_ptr<Texture> Texture::createMultiSampleTexture(
    const unsigned int width
    , const unsigned int height
    , const unsigned int samples
    , const unsigned int format
    , const unsigned int unit
)
{
    std::shared_ptr<Texture> tex = std::make_shared<Texture>();

    GLuint glTex;
    glGenTextures(1, &glTex);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, glTex);

    glTexImage2DMultisample(
        GL_TEXTURE_2D_MULTISAMPLE,
        samples,
        format,
        width, height,
        GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    tex->setTexture(glTex);
    tex->setWidth(width);
    tex->setHeight(height);
    tex->setUnit(unit);
    tex->setTextureTarget(GL_TEXTURE_2D_MULTISAMPLE);
    return tex;
}

Texture::Texture(const std::string& path, unsigned int unit, unsigned int internalFormat, bool Mipmap)
    : mTexture(0)
    , mWidth(0)
    , mHeight(0)
    , mUnit(unit)
    , mTextureTarget(GL_TEXTURE_2D)
{
    mUnit = unit;

    // 1 stbImage 读取图片
    int channels;
    //--反转y轴
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &channels, STBI_rgb_alpha);
    assert(data && "Failed to load texture");
    // 2 生成纹理并且激活单元绑定
    glGenTextures(1, &mTexture);
    //--激活纹理单元--
    glActiveTexture(GL_TEXTURE0 + mUnit);
    //--绑定纹理对象--
    glBindTexture(GL_TEXTURE_2D, mTexture);
    // 3 传输纹理数据,开辟显存
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    if (Mipmap)
        glGenerateMipmap(GL_TEXTURE_2D); // 生成mipmap
    
    // 释放数据
    stbi_image_free(data);

    // 4 设置纹理的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (Mipmap)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // 5 设置纹理的包裹方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // u
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // v
}

Texture::Texture(
    unsigned int unit,
    unsigned char* dataIn,
    uint32_t widthIn,
    uint32_t heightIn
)
    : mTexture(0)
    , mWidth(0)
    , mHeight(0)
    , mUnit(unit)
    , mTextureTarget(GL_TEXTURE_2D)
{
    // 1 stbImage 读取图片
    int channels;

    //--反转y轴
    stbi_set_flip_vertically_on_load(true);

    // 计算整张图片的大小
    uint32_t dataInSize = 0;
    if (!heightIn) {
        dataInSize = widthIn;
    }
    else {
        // 如果内嵌图片不是压缩格式，height = 正常高度，width = 正常宽度
        // 假设数据格式都是 RGBA
        dataInSize = widthIn * heightIn * 4; // 字节为单位
    }

    unsigned char* data = stbi_load_from_memory(dataIn, dataInSize, &mWidth, &mHeight, &channels, STBI_rgb_alpha);
    assert(data);
    // 2 生成纹理并且激活单元绑定
    glGenTextures(1, &mTexture);
    //--激活纹理单元--
    glActiveTexture(GL_TEXTURE0 + mUnit);
    //--绑定纹理对象--
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // 3 传输纹理数据,开辟显存
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    // 释放数据
    stbi_image_free(data);

    // 4 设置纹理的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    // 5 设置纹理的包裹方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // u
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // v
}

Texture::Texture(unsigned int width, unsigned int height, unsigned int unit)
    : mTexture(0)
    , mWidth(width)
    , mHeight(height)
    , mUnit(unit)
    , mTextureTarget(GL_TEXTURE_2D)
{
    glGenTextures(1, &mTexture);
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// 蒙鼓人天空盒顺序: 右面、左面、上面、下面、前面、后面
//#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
//#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
//#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
//#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
//#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
//#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
Texture::Texture(const std::vector<std::string>& paths, unsigned int unit)
    : mTexture(0)
    , mWidth(0)
    , mHeight(0)
    , mUnit(unit)
    , mTextureTarget(GL_TEXTURE_2D)
{
    mUnit = unit;
    setTextureTarget(GL_TEXTURE_CUBE_MAP); // 设置目标为CubeMap
    stbi_set_flip_vertically_on_load(false);
    // 创建CubeMap对象
    // 生成纹理并且激活单元绑定
    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(getTextureTarget(), mTexture);
    // 循环读取六张贴图，并放置到CubeMap的六个gpu空间
    int channels;
    int width = 0, height = 0;
    unsigned char* data;

    for (int i = 0; i < paths.size(); i++)
    {
        data = stbi_load(paths[i].c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            // 释放数据
            stbi_image_free(data);
        }
        else
        {
            assert(false && "Cubemap texture load failed!");
        }
    }

    // 设置纹理的过滤方式
    glTexParameteri(getTextureTarget(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(getTextureTarget(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // 设置纹理的包裹方式
    glTexParameteri(getTextureTarget(), GL_TEXTURE_WRAP_S, GL_REPEAT); // u
    glTexParameteri(getTextureTarget(), GL_TEXTURE_WRAP_T, GL_REPEAT); // v

}

Texture::~Texture() {
    if (mTexture != 0) {
        glDeleteTextures(1, &mTexture);
    }
}

void Texture::bind() {
    // 先切换纹理单元，然后绑定 texture 对象
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(getTextureTarget(), mTexture);
}
