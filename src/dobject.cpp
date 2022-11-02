#include "dobject.h"
#include "dobject_p.h"

#include <list>
#include <algorithm>
#include <iostream>

namespace ding
{

//Class DSignal
class DSignal
{
public:
    using SigFunc = void (DObject::*)();
    struct DSlot
    {
        std::weak_ptr<DObject> m_obj;
        SigFunc m_slot;
        DSlot(std::weak_ptr<DObject>&&obj, SigFunc&& slot)
            : m_obj(std::move(obj)), m_slot(std::move(slot))
        {}
    };

    DSignal(const std::string& name, SigFunc&& func): m_name(name), m_func(std::move(func))
    {
    }
    DSignal(const DSignal& other) = default;
    DSignal& operator=(const DSignal& other) = default;
    DSignal(DSignal&& other)noexcept = default;
    DSignal& operator=(DSignal&& other) = default;
    ~DSignal() = default;

    void clear()
    {
        m_slots.clear();
    }

    void removeSlotFromObj(std::weak_ptr<DObject>&& obj)
    {
        m_slots.remove_if([&obj](const DSlot & slot)
        {
            return !slot.m_obj.owner_before(obj) && !obj.owner_before(slot.m_obj);
        });
    }

    std::string getName()const
    {
        return m_name;
    }
    SigFunc getFunc()const
    {
        return m_func;
    }

    std::list <DSlot> getSlots()const {return m_slots;}
    void addSlot(std::weak_ptr<DObject>&& obj, SigFunc&& slot)
    {
        m_slots.emplace_back(std::move(obj), std::move(slot));
    }

    void removeSlot(std::weak_ptr<DObject>&&obj, SigFunc&& slot)
    {
        if (obj.expired())
            clear();
        else if (!slot)
            m_slots.remove_if([&obj](const DSlot & item)
            {
                return !item.m_obj.owner_before(obj) && !obj.owner_before(item.m_obj);
            });
        else
            m_slots.remove_if([&obj, &slot](const DSlot & item)
            {
                return !item.m_obj.owner_before(obj) && !obj.owner_before(item.m_obj)
                       && item.m_slot == slot;
            });
    }
private:
    std::string m_name;
    SigFunc m_func;
    std::list <DSlot> m_slots;
};

// Class DObjectPrivate
DObjectPrivate::DObjectPrivate(){}
DObjectPrivate::DObjectPrivate(const DObjectPrivate &) = default;
DObjectPrivate &DObjectPrivate::operator=(const DObjectPrivate &) = default;
DObjectPrivate::DObjectPrivate(DObjectPrivate &&) noexcept = default;
DObjectPrivate &DObjectPrivate::operator=(DObjectPrivate &&) = default;
DObjectPrivate::~DObjectPrivate() = default;

DObjectPrivate *DObjectPrivate::clone() const
{
    return new DObjectPrivate(*this);
}

DObjectPrivate *DObjectPrivate::move() noexcept
{
    return new DObjectPrivate(std::move(*this));
}

inline static auto findSignal(DObject::SigSlotFunc signal, const std::vector<std::shared_ptr<DSignal>> &signals)
{
    auto end = signals.cend();
    for(auto it = signals.cbegin();it != end; ++it)
    {
        if (*it && (*it)->getFunc() == signal)
            return it;
    }
    return end;
}

inline static auto findSignal(const std::string &signal, const std::vector<std::shared_ptr<DSignal>> &signals)
{
    auto end = signals.cend();
    for(auto it = signals.cbegin();it != end; ++it)
    {
        if (*it && (*it)->getName() == signal)
            return it;
    }
    return end;
}


// Class DObject
D_REGISTER_OBJECT_CLASS(DObject);
/*!
    Constructs a DObject instance with type info "DObject".
    \note This ctor SHOULD NOT been used in derived classes.
 */
DObject::DObject()
    : DObject(__func__, *new DObjectPrivate())
{
}

/*!
    Constructs a DObject instance with \a type.
    \note It is recommended to use this constructor in derived classes.
 */
DObject::DObject(const std::string &type)
    : DObject(type, *new DObjectPrivate())
{
}

/*!
    Constructs a DObject instance with \a type and the private instance \a dd.
    \note It is used for Pimpl Idiom in derived classes.
 */
DObject::DObject(const std::string &type, DObjectPrivate &dd)
    : DObjectBase(type, dd)
{
    addSignal("logging",&DObject::logging);
}

/*!
    Copy ctor based on \a other.
 */
DObject::DObject(const DObject &) = default;

/*!
    Copy assignment operator based on \a rhs.
 */
DObject &DObject::operator=(const DObject &) = default;

/*!
    Move ctor for \a other.
 */
DObject::DObject(DObject &&) noexcept = default;
/*!
    Move assignment operator for \a rhs.
 */
DObject &DObject::operator=(DObject &&) = default;

/*!
    Destroys a DObject instance.
 */
DObject::~DObject() = default;
/*!
    Returns \c DObject* to the clone of this object.
 */
DObject *DObject::clone() const
{
    return new DObject(*this);
}
/*!
    Move this object to a new object, and return its pointer \c DObject*.
 */
DObject *DObject::move() noexcept
{
    return new DObject(std::move(*this));
}

bool DObject::addSignalImp(const std::string &name, SigSlotFunc&& signal)
{
    if (name.empty() || !signal)
        return false;

    D_D(DObject);
    auto it = findSignal(signal,d.m_signalList);
    if(it != d.m_signalList.cend())
        return false;
    d.m_signalList.emplace_back(std::make_shared<DSignal>(name, std::move(signal)));
    return true;
}

bool DObject::connectImp(SigSlotFunc&& signal, std::weak_ptr<DObject>&& receiver, SigSlotFunc&& slot)
{
    if (!signal || receiver.expired() || !slot)
        return false;

    D_D(DObject);
    auto it = findSignal(signal,d.m_signalList);
    if(it == d.m_signalList.cend())
        return false;
    (*it)->addSlot(std::move(receiver), std::move(slot));
    // if (d.m_signals.count((void*)signal) == 0)
    //     return false;
    // d.m_signals[(void*)signal]->addSlot(std::move(receiver), std::move(slot));
    return true;
}

bool DObject::disconnectImp(SigSlotFunc&& signal, std::weak_ptr<DObject>&& receiver, SigSlotFunc&& slot)
{
    D_D(DObject);
    if (!signal)
    {
        // clear all signals.
        std::for_each(d.m_signalList.begin(),d.m_signalList.end(),[](auto item){
            item->clear();
        });
        // for (auto kv : d.m_signals)
        // {
        //     auto sig = kv.second;
        //     if(sig)
        //         sig->clear();
        // }
        return true;
    }

    auto it = findSignal(signal,d.m_signalList);
    if(it == d.m_signalList.cend())
        return false;

    (*it)->removeSlot(std::move(receiver), std::move(slot));
    // if (d.m_signals.count((void*)signal) == 0)
    //     return false;
    // d.m_signals[(void*)signal]->removeSlot(std::move(receiver), std::move(slot));
    return true;
}

void DObject::emitSignalImp(SigSlotFunc&& signal, std::function<void (const std::shared_ptr<DObject>&, SigSlotFunc)>&& func)
{
    D_D(DObject);
    auto it = findSignal(signal,d.m_signalList);
    if(it == d.m_signalList.cend())
        return;

    for(DSignal::DSlot slot : (*it)->getSlots())
    {
       auto ptr = slot.m_obj.lock();
       if(ptr)
           func(ptr,slot.m_slot);
    }
    // if (d.m_signals.count((void*)signal) == 0)
    //     return;

    // for (auto slot : d.m_signals.at((void*)signal)->getSlots())
    // {
    //    auto ptr = slot.m_obj.lock();
    //    if(ptr)
    //        func(ptr,slot.m_slot);        
    // }
}

bool DObject::isSignalImp(SigSlotFunc&& signal)const
{
    D_D_CONST(DObject);
    // return d.m_signals.count((void*)signal) != 0;
    return findSignal(signal, d.m_signalList) != d.m_signalList.cend();
}

bool DObject::isSignal(std::string name)const
{
    D_D_CONST(DObject);
    // for(auto kv : d.m_signals)
    // {
    //     auto sig = kv.second;
    //     if (sig && sig->getName() == name)
    //         return true;
    // }
    // return false;
    return findSignal(name, d.m_signalList) != d.m_signalList.cend();
}

void testFunc()
{
    int sum = 0;
    for (int i = 0; i < 100; ++i)
        sum += i;
}

}


