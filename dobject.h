#ifndef DOBJECT_H
#define DOBJECT_H

#include <unordered_map>
#include <list>
#include <memory>
#include <algorithm>
#include <iostream>

#include "dobjectbase.h"
#include "dglobal.h"

namespace ding
{

class DObjectPrivate;
class DObject : public DObjectBase
{
    DECLARE_PRIVATE(DObject)
    SIGNAL(DObject,logging)
public:
    using SigSlotFunc = void (DObject::*)();

    DObject();
    DObject(const DObject &other);
    DObject &operator=(const DObject &rhs);
    DObject(DObject &&other) noexcept;
    DObject &operator=(DObject &&rhs);
    virtual ~DObject() override;
    virtual DObject *clone() const override;
    virtual DObject *move() noexcept override;


    template<typename F, typename... Args>
    void call(F&& func, Args&&... args)
    {
        (this->*func)(std::forward<Args>(args)...);
    }

    template<typename T,
             typename... Args,
             typename = std::enable_if_t<std::is_base_of<DObject, T>::value>>
    bool addSignal(const std::string& name,
                   void(T::*signal)(Args...))
    {
        return addSignalImp(name, reinterpret_cast<SigSlotFunc>(signal));
    }

    template < typename T,
               typename U,
               typename... Args,
               typename = std::enable_if_t <
                   std::is_base_of<DObject, T>::value
                   && std::is_base_of<DObject, U>::value >>
    bool connect(void (T::*signal)(Args...),
                 std::weak_ptr<DObject>&& obj,
                 void (U::*slot)(Args...))
    {
        return connectImp(reinterpret_cast<SigSlotFunc>(signal), std::move(obj),reinterpret_cast<SigSlotFunc>(slot));
    }

    bool disconnect(const std::string& signalname, std::weak_ptr<DObject> obj);

    template < typename T,
               typename U,
               typename... Args,
               typename = std::enable_if_t <
                   std::is_base_of<DObject, T>::value
                   && std::is_base_of<DObject, U>::value >>
    bool disconnect(void (T::*signal)(Args...),
                    std::weak_ptr<DObject>&& obj,
                    void (U::*slot)(Args...))
    {
        return disconnectImp(reinterpret_cast<SigSlotFunc>(signal), std::move(obj),reinterpret_cast<SigSlotFunc>(slot));
    }

    template<typename T,
             typename... Args,
             typename... Args2,
             typename = std::enable_if_t<std::is_base_of<DObject, T>::value>>
    void emitSignal(void(T::*signal)(Args...), Args2 && ... args)
    {
        emitSignalImp(reinterpret_cast<SigSlotFunc>(signal), [&](const std::shared_ptr<DObject>& obj,SigSlotFunc slot){
            auto func = reinterpret_cast<void (DObject::*)(Args...)>(slot);
            if(obj)
                obj->call(func, std::forward<Args>(args)...);
        });
    }
protected:
    DObject(const std::string &type);
    DObject(const std::string &type, DObjectPrivate &dd);
private:
    bool addSignalImp(const std::string &name, SigSlotFunc&& signal);
    bool connectImp(SigSlotFunc&& signal, std::weak_ptr<DObject>&& receiver, SigSlotFunc&& slot);
    bool disconnectImp(SigSlotFunc&& signal, std::weak_ptr<DObject>&& receiver, SigSlotFunc&& slot);
    void emitSignalImp(SigSlotFunc&& signal, std::function<void (const std::shared_ptr<DObject>&, SigSlotFunc)>&& func);
};

}


#endif // DOBJECT_H
