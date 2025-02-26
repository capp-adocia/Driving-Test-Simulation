/*****************************************************************//**
 * \file   object.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef OBJECT_H
#define OBJECT_H
#include "core.h"

enum class ObjectType {
    Object,
    Mesh,
    Scene,
    InstanceMesh,
};

class Object : public std::enable_shared_from_this<Object> {
public:
    Object();
    virtual ~Object();

    void setPosition(glm::vec3 pos);

    // 增量旋转
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    // 设置旋转角度
    void setAngleX(float angle);
    void setAngleY(float angle);
    void setAngleZ(float angle);

    void setScale(glm::vec3 scale);

    glm::vec3 getPosition() const { return mPosition; }
    glm::mat4 getModelMatrix() const;

    // 父子关系
    void addChild(std::shared_ptr<Object> obj);  // 使用 shared_ptr
    std::vector<std::shared_ptr<Object>>& getChildren();  // 返回 shared_ptr 的 vector
    std::shared_ptr<Object> getParent() const;  // 返回 shared_ptr

    // 获取类型信息
    ObjectType getType() const { return mType; }
    void setParent(std::shared_ptr<Object> parent);

    glm::mat4 getModelPhysXMatrix() const; // physx修改modelmatrix
    void setModelMatrix(const glm::mat4& model);
protected:
    glm::mat4 mModel;
    glm::vec3 mPosition;

    // unity 旋转标准：pitch yaw roll
    float mAngleX;
    float mAngleY;
    float mAngleZ;

    glm::vec3 mScale;

    // 父子关系
    std::vector<std::shared_ptr<Object>> mChildren;  // 存储 shared_ptr 对象
    std::weak_ptr<Object> mParent;  // 父对象是 shared_ptr

    // 类型记录
    ObjectType mType;
};
#endif // OBJECT_H