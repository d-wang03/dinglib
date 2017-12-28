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
#define EMIT(class_name,signal_name,...) emitSignal(#signal_name,&class_name::signal_name,##__VA_ARGS__);

class DObject;
class DSignal
{
public:
    virtual ~DSignal() = default;
    virtual void clear() = 0;
    virtual void removeSlotFromObj(std::weak_ptr<DObject> obj) = 0;
};

template<typename... Args>
class DSignalImp : public DSignal
{

public:
    using SlotFunc = void (DObject::*)(Args...);
    struct DSlot
    {
        std::weak_ptr<DObject> m_obj;
        SlotFunc m_slot;
        DSlot(std::weak_ptr<DObject>&&obj, SlotFunc slot)
            : m_obj(std::move(obj)), m_slot(slot)
        {}
    };

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
        for(auto iter = m_slots.begin(); iter != m_slots.end(); ++iter)
        {
            DSlot& slot = *iter;
            auto ptr = slot.m_obj.lock();
            auto func = slot.m_slot;
            if(ptr)
                ptr->call(func, std::forward<Args>(args)...);
        }
    }
    void addSlot(std::weak_ptr<DObject>&& obj, SlotFunc slot)
    {
        m_slots.emplace_back(std::move(obj), slot);
    }

    void removeSlot(std::weak_ptr<DObject>obj, SlotFunc slot)
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
        D_UNUSED(signal)
        if(m_signals.find(name) != m_signals.end())
            return false;
        DSignal* metaSig = new DSignalImp<Args...>();
        m_signals.emplace(name, metaSig);
        return true;
    }

    template<typename T,
             typename... Args,
             typename = std::enable_if_t<std::is_base_of<DObject, T>::value>>
    bool connect(const std::string& signalname,
                 std::weak_ptr<DObject>&& obj,
                 void (T::*slot)(Args...))
    {
        auto iter = m_signals.find(signalname);
        if(iter == m_signals.end())
        {
            return false;
        }
        auto sigImp = static_cast<DSignalImp<Args...>*>(iter->second);
        sigImp->addSlot(std::move(obj), static_cast<typename DSignalImp<Args...>::SlotFunc>(slot));
        return true;
    }

    bool disconnect(const std::string& signalname, std::weak_ptr<DObject> obj)
    {
        if(signalname.empty())
        {
            m_signals.clear();
            return true;
        }
        auto iter = m_signals.find(signalname);
        if(iter == m_signals.end())
        {
            return false;
        }
        if(obj.lock())
            iter->second->removeSlotFromObj(std::move(obj));
        else
            iter->second->clear();
        return true;
    }

    template<typename T,
             typename... Args,
             typename = std::enable_if_t<std::is_base_of<DObject, std::decay_t<T>>::value>>
    bool disconnect(const std::string& signalname,
                    std::weak_ptr<DObject> obj,
                    void (DObject::*slot)(Args...))
    {
        auto iter = m_signals.find(signalname);
        if(iter == m_signals.end())
        {
            return false;
        }
        auto sigImp = static_cast<DSignalImp<Args...>*>(iter->second);
        sigImp->removeSlot(std::move(obj), slot);
        return true;
    }

    template<typename T,
             typename... Args,
             typename... Args2,
             typename = std::enable_if_t<std::is_base_of<DObject, T>::value>>
    void emitSignal(const std::string& name, void(T::*signal)(Args...), Args2 && ... args)
    {
        D_UNUSED(signal)
        auto iter = m_signals.find(name);
        if(iter == m_signals.end())
        {
            return;
        }
        auto sigImp = static_cast<DSignalImp<Args...>*>(iter->second);
        sigImp->emit(std::forward<Args>(args)...);
    }
private:
    std::unordered_map<std::string, DSignal*> m_signals;
};

}


#endif // DOBJECT_H
