#include "dobject.h"

namespace ding
{

DSignal::DSignal(const std::string& name, SigFunc func): m_name(name), m_func(func)
{}
DSignal::~DSignal() = default;
DSignal::DSignal(const DSignal& other) = default;
DSignal& DSignal::operator=(const DSignal& other)
{
    m_name = other.m_name;
    m_func = other.m_func;
    return *this;
}
DSignal::DSignal(DSignal&& other)noexcept = default;
DSignal& DSignal::operator=(DSignal&& other)noexcept
{
    m_name = std::move(other.m_name);
    m_func = std::move(other.m_func);
    return *this;
}

DObject::DObject()
{

}

DObject::~DObject() = default;

std::vector<std::shared_ptr<DSignal>>::const_iterator DObject::findSignal(DSignal::SigFunc signal)const
{
    return std::find_if(m_signalList.cbegin(), m_signalList.cend(), [signal](const std::shared_ptr<DSignal>& item)
    {
        return item->getFunc() == signal;
    });
}

std::vector<std::shared_ptr<DSignal>>::const_iterator DObject::findSignal(std::string signal)const
{
    return std::find_if(m_signalList.cbegin(), m_signalList.cend(), [signal](const std::shared_ptr<DSignal>& item)
    {
        return item->getName() == signal;
    });
}

bool DObject::disconnect(const std::string& signalname, std::weak_ptr<DObject> obj)
{
    if(signalname.empty())
    {
        m_signalList.clear();
        return true;
    }
    auto it = findSignal(signalname);
    if(it == m_signalList.cend())
    {
        return false;
    }
    if(obj.lock())
        (*it)->removeSlotFromObj(std::move(obj));
    else
        (*it)->clear();
    return true;
}

}


