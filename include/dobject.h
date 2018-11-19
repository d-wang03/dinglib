#ifndef DOBJECT_H
#define DOBJECT_H

#include "dobjectbase.h"
#include "dglobal.h"
#include "dlogmsg.h"
#include <iostream>

namespace ding
{

class DObjectPrivate;
class DObject : public DObjectBase
{
    SIGNAL(DObject,first)
    SIGNAL(DObject,logging)
    DECLARE_PRIVATE(DObject)
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

    DObject(int i):DObject()
    {
        addSignal("logging",&DObject::logging);
    }

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

    bool isSignal(std::string name)const;
    template<typename T,
             typename... Args,
             typename = std::enable_if_t<std::is_base_of<DObject, T>::value>>
    bool isSignal(void(T::*signal)(Args...))const
    {
        return isSignalImp(reinterpret_cast<SigSlotFunc>(signal));
    }

    //logging methods
    void loggingRVF(DParam&& param)
    {
        logging(param);
    }

    template <typename... Args>
    void loggingD(const char *format, Args &&... args)
    {
        loggingRVF(DLogMsg(DLogMsg::Debug, getTypeName(), format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void loggingI(const char *format, Args &&... args)
    {
        loggingRVF(DLogMsg(DLogMsg::Info, getTypeName(), format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void loggingW(const char *format, Args &&... args)
    {
        loggingRVF(DLogMsg(DLogMsg::Warning, getTypeName(), format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void loggingE(const char *format, Args &&... args)
    {
        loggingRVF(DLogMsg(DLogMsg::Error, getTypeName(), format, std::forward<Args>(args)...));
    }

    virtual inline void addSignals()
    {
        addSignal("logging",&DObject::logging);
    }

protected:
    DObject(const std::string &type);
    DObject(const std::string &type, DObjectPrivate &dd);
private:
    bool addSignalImp(const std::string &name, SigSlotFunc&& signal);
    bool connectImp(SigSlotFunc&& signal, std::weak_ptr<DObject>&& receiver, SigSlotFunc&& slot);
    bool disconnectImp(SigSlotFunc&& signal, std::weak_ptr<DObject>&& receiver, SigSlotFunc&& slot);
    void emitSignalImp(SigSlotFunc&& signal, std::function<void (const std::shared_ptr<DObject>&, SigSlotFunc)>&& func);
    bool isSignalImp(SigSlotFunc&& signal)const;
};

template <typename T, typename... Args, typename=std::enable_if_t<std::is_base_of<DObject,T>::value>>
inline auto makeObject(Args &&... args)
{
    auto ret = std::make_shared<T>(std::forward<Args>(args)...);
    ret->addSignals();
    return ret;
}

template < typename T,
           typename U,
           typename... Args,
           typename = std::enable_if_t <
               std::is_base_of<DObject, T>::value
               && std::is_base_of<DObject, U>::value >>
inline bool connect(std::weak_ptr<DObject>&& sender, void (T::*signal)(Args...),
             std::weak_ptr<DObject>&& receiver, void (U::*slot)(Args...))
{
    if (sender.expired())
        return false;
    return sender.lock()->connect(signal,std::move(receiver),slot);
}

template < typename T,
           typename U,
           typename... Args,
           typename = std::enable_if_t <
               std::is_base_of<DObject, T>::value
               && std::is_base_of<DObject, U>::value >>
inline bool disconnect(std::weak_ptr<DObject>&& sender, void (T::*signal)(Args...),
             std::weak_ptr<DObject>&& receiver, void (U::*slot)(Args...))
{
    if (sender.expired())
        return false;
    return sender.lock()->disconnect(signal,std::move(receiver),slot);
}

void testFunc();

}


#endif // DOBJECT_H
