#ifndef DSYNCQUEUE_H
#define DSYNCQUEUE_H

#include <deque>
#include "dparam.h"
#include "semaphore.h"

namespace ding
{
template <typename Msg, typename = std::enable_if_t<std::is_base_of<DParam, std::decay_t<Msg>>::value>>
class DSyncQueue
{
public:
    DSyncQueue()
    {
        sem_init(&m_semMsg, 0, 0);
        sem_init(&m_semMutex, 0, 1);
    }
    ~DSyncQueue()
    {
        sem_destroy(&m_semMsg);
        sem_destroy(&m_semMutex);
    }
    DSyncQueue(const DSyncQueue &) = default;
    DSyncQueue &operator=(const DSyncQueue &) = default;
    DSyncQueue(DSyncQueue && other) noexcept
    {
        m_queue = std::move(other.m_queue);
    }
    DSyncQueue &operator=(DSyncQueue &&) = default;

    template <typename T, typename = std::enable_if_t<std::is_same<Msg, std::decay_t<T>>::value>>
    void push_back(T &&item)
    {
        sem_wait(&m_semMutex);
        m_queue.emplace_back(item.move());
        sem_post(&m_semMutex);
        sem_post(&m_semMsg);
    }

    void push(Msg *item)
    {
        sem_wait(&m_semMutex);
        m_queue.emplace_back(item);
        sem_post(&m_semMutex);
        sem_post(&m_semMsg);
    }

    template <typename T, typename = std::enable_if_t<std::is_same<Msg, std::decay_t<T>>::value>>
    void push_front(T &&item)
    {
        sem_wait(&m_semMutex);
        m_queue.emplace_front(item.move());
        sem_post(&m_semMutex);
        sem_post(&m_semMsg);
    }

    Msg* pop_front()
    {
        sem_wait(&m_semMsg);
        sem_wait(&m_semMutex);
        auto ret = m_queue.front();
        m_queue.pop_front();
        sem_post(&m_semMutex);
        return ret;
    }

    Msg* pop_back()
    {
        sem_wait(&m_semMsg);
        sem_wait(&m_semMutex);
        auto ret = m_queue.back();
        m_queue.pop_back();
        sem_post(&m_semMutex);
        return ret;
    }

    bool empty()
    {
        sem_wait(&m_semMutex);
        bool ret = m_queue.empty();
        sem_post(&m_semMutex);
        return ret;
    }

    size_t size()
    {
        sem_wait(&m_semMutex);
        auto ret = m_queue.size();
        sem_post(&m_semMutex);
        return ret;
    }

    void clear()
    {
        sem_wait(&m_semMutex);
        for(auto item : m_queue)
            if (item)
                delete item;
        m_queue.clear();
        sem_post(&m_semMutex);
    }

    std::deque<Msg*> take_all()
    {
        sem_wait(&m_semMutex);
        auto ret = std::move(m_queue);
        sem_post(&m_semMutex);
        return ret;
    }

private:
    std::deque<Msg*> m_queue;
    sem_t m_semMsg;
    sem_t m_semMutex;
};

} // namespace ding

#endif // DSYNCQUEUE_H
