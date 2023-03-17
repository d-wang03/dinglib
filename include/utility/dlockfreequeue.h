#ifndef D_LOCK_FREE_QUEUE_H
#define D_LOCK_FREE_QUEUE_H

#include <cstdint>
#include <atomic>

#define __CHECK_POWER_OF_2(x) ((x) > 0 && ((x) & ((x) - 1)) == 0)

namespace ding
{
template <typename T, uint32_t CAP = 16>
class DLockFreeQueue
{
public:
    DLockFreeQueue()
    {
        m_mask = CAP - 1;
        m_size = CAP;
        m_in = 1;
        m_out = 0;

        buffer[m_in] = flag_in;
        buffer[m_out] = flag_out;
        for (uint32_t i = 2; i < CAP; ++i)
            buffer[i] = (flag_empty | i);
    }
    ~DLockFreeQueue() = default;
    DLockFreeQueue(const DLockFreeQueue&) = delete;
    DLockFreeQueue(DLockFreeQueue&&) = delete;
    DLockFreeQueue& operator=(const DLockFreeQueue&) = delete;
    DLockFreeQueue& operator=(DLockFreeQueue&&) = delete;

    bool empty() const
    {
        return m_in - m_out == 1;
    }
    size_t size() const
    {
        return m_in - m_out - 1;
    }

    bool push(T* t)
    {
        uint32_t cur;
        uint32_t next;
        uint64_t* nextVal;

        do
        {
            cur = m_in;
            next = cur + 1;
            nextVal = &buffer[next & m_mask];
            if (*nextVal & flag_out)
                return false;

        } while (!__sync_bool_compare_and_swap(nextVal, flag_empty | next, flag_in));

        buffer[cur & m_mask] = reinterpret_cast<uint64_t>(t);
        ++m_in;
        return true;
    }

    T* pop()
    {
        unsigned int cur;
        unsigned int next;
        uint64_t *nextVal;
        uint64_t ret;

        do
        {
            cur = m_out;
            next = cur + 1;
            nextVal = &buffer[next & m_mask];
            ret = *nextVal;
            if (ret == flag_in)
                return nullptr;

        } while (!__sync_bool_compare_and_swap(&buffer[cur & m_mask],
                                            flag_out | cur,
                                            flag_empty | (cur + m_size)));
        *nextVal = (flag_out | next);
        ++m_out;
        return reinterpret_cast<T*>(ret);
    }

    template <typename U, typename = std::enable_if_t<std::is_same<T, std::decay_t<U>>::value>>
    bool push_back(U &&item)
    {
        return push(item.move());
    }

    T* pop_front()
    {
        return pop();
    }

private:
    uint32_t m_mask;
    uint32_t m_size;
    std::atomic<uint32_t> m_in;
    std::atomic<uint32_t> m_out;
    uint64_t buffer[CAP];

    static_assert(__CHECK_POWER_OF_2(CAP), "CAP MUST be power of 2");
    static_assert(CAP > 2, "CAP MUST be larger than 2");

    static constexpr uint64_t flag_in = (1ull << 63);
    static constexpr uint64_t flag_out = (1ull << 62);
    static constexpr uint64_t flag_empty = (1ull << 61);
};

// template <typename T>
// using DLockFreeQueue = DLockFreeQueue<T,16>;
}

#endif