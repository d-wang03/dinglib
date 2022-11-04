#include "dobject.h"
#include "dobject_p.h"

#include <list>
#include <algorithm>
#include <iostream>

namespace ding
{

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
    return d.addSignal(name.c_str(), std::move(signal));
}

bool DObject::connectImp(SigSlotFunc&& signal, std::weak_ptr<DObject>&& receiver, SigSlotFunc&& slot)
{
    if (!signal || receiver.expired() || !slot)
        return false;

    D_D(DObject);
    auto found = d.find(signal);
    if (!found)
        return false;
    found->addSlot(std::move(receiver), std::move(slot));

    return true;
}

bool DObject::disconnectImp(SigSlotFunc&& signal, std::weak_ptr<DObject>&& receiver, SigSlotFunc&& slot)
{
    D_D(DObject);
    if (!signal)
    {
        for(int i = 0; i < MAX_SIGNAL_NUM; ++i)
        {
            d.m_signals[i].removeSlot();
        }
        return true;
    }

    auto found = d.find(signal);
    if (!found)
        return false;
    
    if (receiver.expired())
        found->removeSlot();
    else if (!slot)
        found->removeSlot(std::move(receiver));
    else
        found->removeSlot(std::move(receiver), std::move(slot));
    return true;
}

void DObject::emitSignalImp(SigSlotFunc&& signal, std::function<void (const std::shared_ptr<DObject>&, SigSlotFunc)>&& func)
{
    D_D(DObject);
    auto found = d.find(signal);
    if (!found)
        return;
    for (int i = 0; i < MAX_SLOT_PER_SIGNAL_NUM; ++i)
    {
        auto slot = found->getSlots()[i];
        auto ptr = slot.m_obj.lock();
        if(ptr)
            func(ptr,slot.m_slot);
    }
}

bool DObject::isSignalImp(SigSlotFunc&& signal)const
{
    D_D_CONST(DObject);
    return d.exists(signal);
}

bool DObject::isSignal(std::string name)const
{
    D_D_CONST(DObject);
    return d.exists(name.c_str());
}

void testFunc()
{
    int sum = 0;
    for (int i = 0; i < 100; ++i)
        sum += i;
}

}


