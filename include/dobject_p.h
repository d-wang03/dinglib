#ifndef DOBJECT_P_H_
#define DOBJECT_P_H_

#include "dobject.h"
#include <string.h>
#include <memory>

namespace ding
{
// struct DSignal;
#define MAX_SIGNAL_NUM 16U
#define MAX_SLOT_PER_SIGNAL_NUM 5U
struct DSignal
{
    using SigFunc = void (DObject::*)();
    struct DSlot
    {
        std::weak_ptr<DObject> m_obj;
        SigFunc m_slot;
        DObject* m_raw;
        DSlot(): m_slot(nullptr){}
        ~DSlot() = default;
        void set(std::weak_ptr<DObject>&&obj, SigFunc&& slot)
        {
            m_obj = std::move(obj);
            m_slot = std::move(slot);
            m_raw = m_obj.lock().get();
        }
        void clear()
        {
            m_obj.reset();
            m_slot = nullptr;
            m_raw = nullptr;
        }
        bool empty()const
        {
            return !m_slot || m_obj.expired();
        }
    };

    DSignal():m_name(nullptr), m_func(nullptr){}
    DSignal(const DSignal &other) : m_name(nullptr), m_func(nullptr)
    {
        if(other.m_name)
            m_name = strdup(other.m_name);
        if (other.m_func)
        {
            m_func = other.m_func;
            for (int i = 0; i < MAX_SLOT_PER_SIGNAL_NUM; ++i)
                m_slots[i] = other.m_slots[i];
        }
    }
    ~DSignal()
    {
        if(m_name)
            free(m_name);
    }

    void set(const char *name, SigFunc&& func)
    {
        if (m_name)
            free(m_name);
        m_name = strdup(name);
        m_func = std::move(func);
    }

    bool empty()const
    {
        return m_func == nullptr;
    }

    void updateBegin()
    {
        for (int i = 0; i < MAX_SLOT_PER_SIGNAL_NUM; ++i)
        {
            if (!m_slots[i].empty())
            {
                m_begin = i;
                return;
            }
        }
        m_begin = MAX_SLOT_PER_SIGNAL_NUM;
    }

    void updateEnd()
    {
        for (int i = MAX_SLOT_PER_SIGNAL_NUM - 1; i >= 0; --i)
        {
            if (!m_slots[i].empty())
            {
                m_end = i + 1;
                return;
            }
        }
        m_end = 0;
    }

    void removeSlot()
    {
        for (int i = 0; i < MAX_SLOT_PER_SIGNAL_NUM; ++i)
            m_slots[i].clear();
        m_begin = MAX_SLOT_PER_SIGNAL_NUM;
        m_end = 0;
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
        updateBegin();
        updateEnd();
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
        updateBegin();
        updateEnd();
    }

    void addSlot(std::weak_ptr<DObject>&& obj, SigFunc&& slot)
    {
        for (int i = 0; i < MAX_SLOT_PER_SIGNAL_NUM; ++i)
        {
            if (m_slots[i].empty())
            {
                m_slots[i].set(std::move(obj), std::move(slot));
                if (i < m_begin)
                    m_begin = i;
                if (i >= m_end)
                    m_end = i + 1;
                break;
            }
        }
    }

    char *m_name;
    SigFunc m_func;
    DSlot m_slots[MAX_SLOT_PER_SIGNAL_NUM];
    uint32_t m_begin;
    uint32_t m_end;
};
class DObjectPrivate : public DObjectData
{
public:
    DECLARE_PUBLIC(DObject)
    DObjectPrivate();
    DObjectPrivate(const DObjectPrivate &other);
    // DObjectPrivate(DObjectPrivate &&other) noexcept;
    ~DObjectPrivate() override;
    DObjectPrivate *clone() const override;
    // DObjectPrivate *move() noexcept override;

    DSignal m_signals[MAX_SIGNAL_NUM];
 
    DSignal* find(const DObject::SigSlotFunc &signal)
    {
        for(int i = 0; i < MAX_SIGNAL_NUM; ++i)
        {
            if (m_signals[i].m_func == signal)
                return &m_signals[i];
        }
        return nullptr;
    }

    bool exists(const DObject::SigSlotFunc &signal)const
    {
        for(int i = 0; i < MAX_SIGNAL_NUM; ++i)
        {
            if (m_signals[i].m_func == signal)
                return true;
        }
        return false;
    }

    bool exists(const char *signal)const
    {
        for(int i = 0; i < MAX_SIGNAL_NUM; ++i)
        {
            auto name = m_signals[i].m_name;
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
};

} // namespace ding

#endif /* DOBJECT_P_H_ */
