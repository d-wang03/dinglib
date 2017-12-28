#ifndef DOBJECT_H
#define DOBJECT_H

#include <unordered_map>
#include <list>
#include <memory>
#include <algorithm>
#include <iostream>

namespace ding
{

#define D_UNUSED(x) (void)x;
#define EMIT(class_name,signal_name,...) emitSignal(&class_name::signal_name,##__VA_ARGS__);

class DObject;
class DSignal
{
public:
    using SigFunc = void (DObject::*)();
    DSignal(const std::string& name, SigFunc func);
    DSignal(const DSignal& other);
    DSignal& operator=(const DSignal& other);
    DSignal(DSignal&& other)noexcept;
    DSignal& operator=(DSignal&& other)noexcept;
    virtual ~DSignal();

    virtual void clear() = 0;
    virtual void removeSlotFromObj(std::weak_ptr<DObject> obj) = 0;

    std::string getName()
    {
        return m_name;
    }
    SigFunc getFunc()
    {
        return m_func;
    }
private:
    std::string m_name;
    SigFunc m_func;
};

template<typename... Args>
class DSignalImp : public DSignal
{
public:
    using SigFuncImp = void (DObject::*)(Args...);
    struct DSlot
    {
        std::weak_ptr<DObject> m_obj;
        SigFuncImp m_slot;
        DSlot(std::weak_ptr<DObject>&&obj, SigFuncImp slot)
            : m_obj(std::move(obj)), m_slot(slot)
        {}
    };

    DSignalImp(const std::string& name, SigFuncImp func): DSignal(name, reinterpret_cast<DSignal::SigFunc>(func))
    {
    }

    virtual void clear()override
    {
        m_slots.clear();
    }

    virtual void removeSlotFromObj(std::weak_ptr<DObject> obj)
    {
        m_slots.remove_if([&obj](const DSlot & slot)
        {
            return !slot.m_obj.owner_before(obj) && !obj.owner_before(slot.m_obj);
        });
    }

    void emit(Args&&... args)
    {
        for(auto it = m_slots.begin(); it != m_slots.end(); ++it)
        {
            DSlot& slot = *it;
            auto ptr = slot.m_obj.lock();
            auto func = slot.m_slot;
            if(ptr)
                ptr->call(func, std::forward<Args>(args)...);
        }
    }
    void addSlot(std::weak_ptr<DObject>&& obj, SigFuncImp slot)
    {
        m_slots.emplace_back(std::move(obj), slot);
    }

    void removeSlot(std::weak_ptr<DObject>obj, SigFuncImp slot)
    {
        m_slots.remove_if([&obj, &slot](const DSlot & item)
        {
            return !item.owner_before(obj) && !obj.owner_before(item)
                   && item.m_slot == slot;
        });
    }
private:
    std::list <DSlot> m_slots;
};

class DObject
{
public:
    DObject();
    virtual ~DObject();

    template<typename F, typename... Args>
    void call(F&& func, Args&&... args)
    {
        (this->*func)(args...);
    }

    template<typename T,
             typename... Args,
             typename = std::enable_if_t<std::is_base_of<DObject, T>::value>>
    bool addSignal(const std::string& name,
                   void(T::*signal)(Args...))
    {
        auto it = findSignal(reinterpret_cast<DSignal::SigFunc>(signal));
        if(it != m_signalList.cend())
            return false;
        m_signalList.emplace_back(
            std::make_shared<DSignalImp<Args...>>
            (name, static_cast<typename DSignalImp<Args...>::SigFuncImp>(signal)));
        return true;
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
        auto it = findSignal(reinterpret_cast<DSignal::SigFunc>(signal));
        if(it == m_signalList.cend())
            return false;

        auto sigImp = static_cast<DSignalImp<Args...>*>((*it).get());
        sigImp->addSlot(std::move(obj), static_cast<typename DSignalImp<Args...>::SigFuncImp>(slot));
        return true;
    }

    bool disconnect(const std::string& signalname, std::weak_ptr<DObject> obj);

    template<typename T,
             typename... Args,
             typename = std::enable_if_t<std::is_base_of<DObject, std::decay_t<T>>::value>>
    bool disconnect(const std::string& signalname,
                    std::weak_ptr<DObject> obj,
                    void (DObject::*slot)(Args...))
    {
        auto it = findSignal(signalname);
        if(it == m_signalList.cend())
        {
            return false;
        }
        auto sigImp = static_cast<DSignalImp<Args...>*>((*it).get());
        sigImp->removeSlot(std::move(obj), slot);
        return true;
    }

    template<typename T,
             typename... Args,
             typename... Args2,
             typename = std::enable_if_t<std::is_base_of<DObject, T>::value>>
    void emitSignal(void(T::*signal)(Args...), Args2 && ... args)
    {
        auto it = findSignal(reinterpret_cast<DSignal::SigFunc>(signal));
        if(it == m_signalList.cend())
        {
            return;
        }
        auto sigImp = static_cast<DSignalImp<Args...>*>((*it).get());
        sigImp->emit(std::forward<Args>(args)...);
    }
private:
    std::vector<std::shared_ptr<DSignal>>::const_iterator
                                       findSignal(DSignal::SigFunc signal)const;
    std::vector<std::shared_ptr<DSignal>>::const_iterator
                                       findSignal(std::string signal)const;

    std::vector<std::shared_ptr<DSignal>> m_signalList;
};

}


#endif // DOBJECT_H
