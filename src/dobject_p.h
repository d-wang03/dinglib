#ifndef DOBJECT_P_H_
#define DOBJECT_P_H_

#include "dobject.h"
#include <string.h>
#include <vector>
#include <memory>
#include <map>
#include <list>
#include <algorithm>

namespace ding
{
// class DSignal;
#define MAX_SIGNAL_NUM 16U
#define MAX_SLOT_PER_SIGNAL_NUM 5U
class DSignal
{
public:
    using SigFunc = void (DObject::*)();
    struct DSlot
    {
        std::weak_ptr<DObject> m_obj;
        SigFunc m_slot;
        DSlot(): m_slot(nullptr){}
        void set(std::weak_ptr<DObject>&&obj, SigFunc&& slot)
        {
            m_obj = std::move(obj);
            m_slot = std::move(slot);
        }
        void clear()
        {
            m_obj.reset();
            m_slot = nullptr;
        }
        bool empty()const
        {
            return m_obj.expired() || !m_slot;
        }
    };

    DSignal():m_name(nullptr), m_func(nullptr){}
    void set(const char *name, SigFunc&& func)
    {
        m_name = strdup(name);
        m_func = std::move(func);
    }
    DSignal(const DSignal& other) = default;
    DSignal& operator=(const DSignal& other) = default;
    DSignal(DSignal&& other)noexcept = default;
    DSignal& operator=(DSignal&& other) = default;
    ~DSignal() = default;

    void clear()
    {
        char* tmp = const_cast<char*>(m_name);
        if(tmp)
            free(tmp);
        m_name = nullptr;
        m_func = nullptr;
        removeSlot();
    }

    bool empty()const
    {
        return m_func == nullptr;
    }

    void removeSlot()
    {
        for (int i = 0; i < MAX_SLOT_PER_SIGNAL_NUM; ++i)
            m_slots[i].clear();
    }

    void removeSlot(std::weak_ptr<DObject>&& obj)
    {
        if (obj.expired())
            return;
        for (int i = 0; i < MAX_SLOT_PER_SIGNAL_NUM; ++i)
        {
            auto &m_obj = m_slots[i].m_obj;
            if (!m_obj.owner_before(obj) && !obj.owner_before(m_obj))
            {
                m_slots[i].clear();
            }
        }
    }

    void removeSlot(std::weak_ptr<DObject>&& obj, SigFunc&& slot)
    {
        if (obj.expired() || !slot)
            return;
        for (int i = 0; i < MAX_SLOT_PER_SIGNAL_NUM; ++i)
        {
            auto &m_obj = m_slots[i].m_obj;
            if (!m_obj.owner_before(obj) && !obj.owner_before(m_obj) && m_slots[i].m_slot == slot)
            {
                m_slots[i].clear();
            }
        }
    }

    const char *getName()const
    {
        return m_name;
    }
    SigFunc getFunc()const
    {
        return m_func;
    }

    const DSlot* getSlots()const {return m_slots;}
    void addSlot(std::weak_ptr<DObject>&& obj, SigFunc&& slot)
    {
        for (int i = 0; i < MAX_SLOT_PER_SIGNAL_NUM; ++i)
        {
            if (m_slots[i].empty())
            {
                m_slots[i].set(std::move(obj), std::move(slot));
                break;
            }
        }
    }

private:
    const char *m_name;
    SigFunc m_func;
    DSlot m_slots[MAX_SLOT_PER_SIGNAL_NUM];
};
class DObjectPrivate : public DObjectData
{
public:
    DECLARE_PUBLIC(DObject)
    DObjectPrivate();
    DObjectPrivate(const DObjectPrivate &other);
    DObjectPrivate &operator=(const DObjectPrivate &rhs);
    DObjectPrivate(DObjectPrivate &&other) noexcept;
    DObjectPrivate &operator=(DObjectPrivate &&rhs);
    virtual ~DObjectPrivate() override;
    virtual DObjectPrivate *clone() const override;
    virtual DObjectPrivate *move() noexcept override;

    DSignal m_signals[MAX_SIGNAL_NUM];
 
    DSignal* find(const DObject::SigSlotFunc &signal)
    {
        for(int i = 0; i < MAX_SIGNAL_NUM; ++i)
        {
            if (m_signals[i].getFunc() == signal)
                return &m_signals[i];
        }
        return nullptr;
    }

    DSignal* find(const char *signal)
    {
        for(int i = 0; i < MAX_SIGNAL_NUM; ++i)
        {
            if (!strcmp(m_signals[i].getName(), signal))
                return &m_signals[i];
        }
        return nullptr;
    }

    bool exists(const DObject::SigSlotFunc &signal)const
    {
        for(int i = 0; i < MAX_SIGNAL_NUM; ++i)
        {
            if (m_signals[i].getFunc() == signal)
                return true;
        }
        return false;
    }

    bool exists(const char *signal)const
    {
        for(int i = 0; i < MAX_SIGNAL_NUM; ++i)
        {
            auto name = m_signals[i].getName();
            if (name && !strcmp(name, signal))
                return true;
        }
        return false;
    }

    bool addSignal(const char *name, DObject::SigSlotFunc&& signal)
    {
        auto exist_sig = find(signal);
        if (exist_sig)
            return false;
        for(int i = 0; i < MAX_SIGNAL_NUM; ++i)
        {
            if (m_signals[i].empty())
            {
                m_signals[i].set(name, std::move(signal));
                return true;
            }
        }
        return false;
    }
    void clearSignal()
    {
        for(int i = 0; i < MAX_SIGNAL_NUM; ++i)
        {
            m_signals[i].clear();
        }
    }
};

} // namespace ding

#endif /* DOBJECT_P_H_ */
