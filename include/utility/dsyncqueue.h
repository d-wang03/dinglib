#ifndef DSYNCQUEUE_H
#define DSYNCQUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>
#include "dparam.h"

namespace ding
{
template <typename TMsg, typename = std::enable_if_t<std::is_base_of<DParam, std::decay_t<TMsg>>::value>>
class DSyncQueue
{
public:
    DSyncQueue() = default;
    ~DSyncQueue() = default;
    DSyncQueue(const DSyncQueue &) = default;
    DSyncQueue &operator=(const DSyncQueue &) = default;
    DSyncQueue(DSyncQueue && other) noexcept
    {
        m_queue = std::move(other.m_queue);
    }
    DSyncQueue &operator=(DSyncQueue &&) = default;

    template <typename T, typename = std::enable_if_t<std::is_same<TMsg, std::decay_t<T>>::value>>
    void push_back(T &&item)
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        m_queue.emplace_back(item.move());
        m_notEmpty.notify_one();
    }

    template <typename T, typename = std::enable_if_t<std::is_same<TMsg, std::decay_t<T>>::value>>
    void push_front(T &&item)
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        m_queue.emplace_front(item.move());
        m_notEmpty.notify_one();
    }

    std::shared_ptr<TMsg> pop_front()
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait(locker, [this] { return !m_queue.empty(); });
        auto ret = m_queue.front();
        m_queue.pop_front();
        return ret;
    }

    std::shared_ptr<TMsg> pop_back()
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait(locker, [this] { return !m_queue.empty(); });
        auto ret = m_queue.back();
        m_queue.pop_front();
        return ret;
    }

    bool empty()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.empty();
    }

    size_t size()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size();
    }

    void clear()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        m_queue.clear();
    }

    std::deque<std::shared_ptr<TMsg>> take_all()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        auto ret = std::move(m_queue);
        return ret;
    }

private:
    std::deque<std::shared_ptr<TMsg>> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_notEmpty;
};

} // namespace ding

#endif // DSYNCQUEUE_H
