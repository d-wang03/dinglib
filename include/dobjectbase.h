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
    DObjectData();
    virtual ~DObjectData();
    DObjectData(const DObjectData &other);
    DObjectData(DObjectData &&other) noexcept;
    virtual DObjectData *clone() const = 0;
    virtual DObjectData *move() noexcept = 0;
    DObjectBase *q_ptr;
    char * m_typename;
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
    void setTypeName(const char *type);
    const char * getTypeName() const;
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
    DObjectBase(const char *name, DObjectData &dd);
    // copy ctor
    DObjectBase(const DObjectBase &other);
    DObjectBase &operator=(const DObjectBase &rhs);
    // move ctor
    DObjectBase(DObjectBase &&other) noexcept;
    DObjectBase &operator=(DObjectBase &&rhs);

    std::unique_ptr<DObjectData> d_ptr;
};

typedef std::shared_ptr<DObjectBase>(*DNewObjectFunc)();

class DObjectFactory
{
public:
    template <typename T, typename = std::enable_if_t<std::is_base_of<DObjectBase, T>::value>>
    static std::shared_ptr<T> CreateObject(const char *objectClassName)
    {
        auto ins = DObjectFactory::getInstance();
        auto iter = ins.m_dynObjectCreateMap.find(objectClassName);
        return (iter == ins.m_dynObjectCreateMap.end() ?
                    std::shared_ptr<T>{} : std::static_pointer_cast<T>(iter->second()));
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
