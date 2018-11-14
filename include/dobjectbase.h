#ifndef DBASEOBJECT_H
#define DBASEOBJECT_H

#include <string>
#include <memory>
#include <unordered_map>

namespace ding
{
class DObjectBase;
class DObjectData
{
public:
    DObjectData() = default;
    virtual ~DObjectData() = default;
    DObjectData(const DObjectData &other) = default;
    DObjectData &operator=(const DObjectData &rhs) = default;
    DObjectData(DObjectData &&other) noexcept = default;
    DObjectData &operator=(DObjectData &&rhs) = default;
    virtual DObjectData *clone() const = 0;
    virtual DObjectData *move() noexcept = 0;
    DObjectBase *q_ptr;
    std::string m_typename;
};

class DObjectBase
{
public:
    // dtor
    virtual ~DObjectBase() = default;
    // clone
    virtual DObjectBase *clone() const = 0;
    // move
    virtual DObjectBase *move() noexcept = 0;
    // type name
    void setTypeName(const std::string &type);
    std::string getTypeName() const;
    // type checker
    template <typename T>
    bool is_type() const
    {
        return typeid(T) == typeid(*this);
    }
    template <typename T>
    bool is_derived_from() const
    {
        return dynamic_cast<const T *>(this) != nullptr;
    }

protected:
    // ctor
    DObjectBase(const std::string &name, DObjectData &dd);
    // copy ctor
    DObjectBase(const DObjectBase &other);
    DObjectBase &operator=(const DObjectBase &rhs);
    // move ctor
    DObjectBase(DObjectBase &&other) noexcept;
    DObjectBase &operator=(DObjectBase &&rhs);

    std::unique_ptr<DObjectData> d_ptr;
};

typedef DObjectBase *(*DNewObjectFunc)();

class DObjectFactory
{
public:
    template <typename T, typename = std::enable_if_t<std::is_base_of<DObjectBase, T>::value>>
    static std::unique_ptr<T> CreateObject(const char *objectClassName)
    {
        auto ins = DObjectFactory::getInstance();
        auto iter = ins.m_dynObjectCreateMap.find(objectClassName);
        auto ptr = iter == ins.m_dynObjectCreateMap.end() ? nullptr : iter->second();
        return std::unique_ptr<T>(static_cast<T *>(ptr));
    }
    static void RegisterObjectClass(const char *objectClassName, DNewObjectFunc np);

private:
    static DObjectFactory &getInstance();
    std::unordered_map<std::string, DNewObjectFunc> m_dynObjectCreateMap;
};

class DRegister
{
public:
    DRegister(const char *objectClassName, DNewObjectFunc np);
};

} // namespace ding

#endif // DBASEOBJECT_H
