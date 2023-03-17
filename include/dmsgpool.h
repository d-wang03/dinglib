#ifndef D_MESSAGE_POOL_H
#define D_MESSAGE_POOL_H

#include "dparam.h"
#include "utility/dlockfreequeue.h"
namespace ding
{

template <typename Message, typename = std::enable_if_t<std::is_base_of<DParam, std::decay_t<Message>>::value>>
class DMessagePool
{
    public:
    static DMessagePool<Message>& getInstance()
    {
        static DMessagePool<Message> ins;
        return ins;
    }

    template<typename... Args>
    Message* alloc(Args&&... args)
    {
        Message* ret = m_pool.pop();
        if (ret)
            ret->init(std::forward<Args>(args)...);
        else
            ret = new Message(std::forward<Args>(args)...);
        return ret;
    }

    void free(Message* msg)
    {
        if(!msg)
            return;

        if (m_pool.contains(msg))
            return;
        if (!m_pool.push(msg))
        {
            //the pool is full, discard msg.
            delete msg;
            return;
        }
    }

    size_t size()const
    {
        return m_pool.size();
    }

    bool empty()const
    {
        return m_pool.empty();
    }

    void clear()
    {
        m_pool.clear();
    }

    private:
    DLockFreeQueue<Message> m_pool;
};

template <typename Message, typename = std::enable_if_t<std::is_base_of<DParam, Message>::value>, typename... Args>
Message* allocMsg(Args&&... args)
{
    auto &ins = DMessagePool<Message>::getInstance();
    return ins.alloc(std::forward<Args>(args)...); 
}

template <typename Message, typename = std::enable_if_t<std::is_base_of<DParam, Message>::value>>
void freeMsg(Message* msg)
{
    auto &ins = DMessagePool<Message>::getInstance();
    ins.free(msg);
}

} // namespace AdvancedServiceFramework
#endif
