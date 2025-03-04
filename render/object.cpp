/*****************************************************************//**
 * \file   object.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "object.h"
#include "../util/logger/log.h"

Object::Object()
    : mPhysXMatrix(glm::mat4(1.0f))
    , mPosition(glm::vec3(0.0f))
    , mAngleX(0.0f)
    , mAngleY(0.0f)
    , mAngleZ(0.0f)
    , mScale(glm::vec3(1.0f))
    , mChildren({})
    , mParent(std::weak_ptr<Object>())
    , mType(ObjectType::Object)
{}

Object::~Object() {}

void Object::setPosition(glm::vec3 pos) {
    mPosition = pos;
}

//增量旋转
void Object::rotateX(float angle) {
    mAngleX += angle;
}

void Object::rotateY(float angle) {
    mAngleY += angle;
}

void Object::rotateZ(float angle) {
    mAngleZ += angle;
}

void Object::setAngleX(float angle) {
    mAngleX = angle;
}

void Object::setAngleY(float angle) {
    mAngleY = angle;
}

void Object::setAngleZ(float angle) {
    mAngleZ = angle;
}

void Object::setScale(glm::vec3 scale) {
    mScale = scale;
}

glm::mat4 Object::getModelMatrix() {

    //首先获取父亲的变换矩阵
    glm::mat4 parentMatrix{ 1.0f };
    if (auto parent = mParent.lock()) {
        parentMatrix = parent->getModelMatrix();
    }
    //unity：缩放 旋转 平移
    glm::mat4 transform{ 1.0f };
    transform = glm::scale(transform, mScale);
    //unity旋转标准：pitch yaw roll
    transform = glm::rotate(transform, glm::radians(mAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(mAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(mAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = getModelPhysXMatrix() * parentMatrix * glm::translate(glm::mat4(1.0f), mPosition) * transform;
    return transform;
}

glm::mat4 Object::getModelPhysXMatrix() const { return mPhysXMatrix; }

void Object::setModelPhysXMatrix(const glm::mat4& model) { this->mPhysXMatrix = model; }

// 检查是否曾经加入过这个孩子
void Object::addChild(std::shared_ptr<Object> obj) {
    auto iter = std::find(mChildren.begin(), mChildren.end(), obj);
    if (iter != mChildren.end()) {
        LOG_ERROR("Duplicated Child added");
        return;
    }
    mChildren.push_back(obj);
    // 设置孩子的父对象
    obj->setParent(shared_from_this());  // 使用 shared_from_this 获取父对象的 shared_ptr
}

std::vector<std::shared_ptr<Object>>& Object::getChildren()
{
    return mChildren;
}

std::shared_ptr<Object> Object::getParent() const
{
    return mParent.lock();
}

void Object::setParent(std::shared_ptr<Object> parent)
{
    mParent = parent;
}