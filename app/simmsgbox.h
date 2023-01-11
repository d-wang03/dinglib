#ifndef SIM_MSG_BOX_H
#define SIM_MSG_BOX_H

#include "dobject.h"
#include <iostream>

using namespace ding;

struct Msg
{
    uint8_t src;
    uint8_t dst;
    uint8_t length;
    uint8_t data[10];
};

class SimMsg : public ding::DParam
{
public:
    SimMsg(const Msg& msg): DParam("SimMsg"), m_content(msg){}
    SimMsg(uint8_t dst): DParam("SimMsg"){ m_content.dst = dst;}
    SimMsg(const SimMsg&) = default;
    SimMsg(SimMsg&& other) noexcept: DParam(std::move(other))
    {
        m_content = other.m_content;
    }
    DParam *move() noexcept override
    {
        return new SimMsg(std::move(*this));
    }

    bool equals(const DParam& other)const override
    {
        if (DParam::equals(other))
        {
            auto &rhs = static_cast<const SimMsg&>(other);
            return m_content.dst == rhs.m_content.dst;
        }
        return false;
    }

    std::string toString()const
    {
        return string_format("src:%1, dst:%2, length:%3, data:%4 %5 %6", 
            m_content.src, m_content.dst, m_content.length, m_content.data[0], m_content.data[1], m_content.data[2]);
    }
private:
    Msg m_content;
};

class SimMsgBox : public ding::DObject
{
    public:
    SimMsgBox(uint8_t id): DObject(__func__), m_id(id)
    {
        ADD_SIGNAL(SimMsgBox, sendData);
    }
    SIGNAL(SimMsgBox, sendData)
    // SIGNAL(SimMsgBox, received)
    void send(const Msg& msg)
    {
        if (msg.src != m_id)
            return;
        auto param = new SimMsg(msg);
        sendData(*param);
    }
    void received(DParam& param)
    {
        auto msg = dynamic_cast<SimMsg*>(&param);
        if (msg)
        {
            std::cout << "SimMsgBox " << m_id << " received message " << msg->toString() << std::endl;
        }
        else
        {
            std::cerr << "SimMsgBox " << m_id << " error message" << std::endl;
        }
    }

    private:
    uint8_t m_id;
};

#endif