#include "dlogmsg.h"
#include "dparam_p.h"
#include <stdarg.h>
#include <memory>
#include <sstream>
#include <iomanip>

using namespace std::chrono_literals;
namespace ding
{
/*!
    \class ding::DLogMsg
    \brief The DLogMsg class is a parameter class contains a log message.
    \ingroup logging
    \inmodule ding

    A log message contains the module name, content string, time stamp, and the logging
    level.

    It is defined as immutable object. Once contructed, it remains unchange.
*/

/*!
    \enum ding::DLogMsg::MsgLevel

    This enum type specifies the level of log message:

    \value Debug
           The message is debug level.
    \value Info
           The message is infomation level.
    \value Warning
           The message is warning level.
    \value Error
           The message is error level.
*/

// class DLogMsgPrivate
class DLogMsgPrivate : public DParamPrivate
{
    DECLARE_PUBLIC(DLogMsg)
public:
    DLogMsgPrivate() = default;
    virtual ~DLogMsgPrivate() override = default;
    DLogMsgPrivate(const DLogMsgPrivate &other) = default;
    DLogMsgPrivate &operator=(const DLogMsgPrivate &rhs) = default;
    virtual DLogMsgPrivate *clone() const override;

private:
    DLogMsg::MsgLevel m_level;
    std::string m_module;
    std::string m_content;
    std::chrono::steady_clock::duration m_timestamp;
};

DLogMsgPrivate *DLogMsgPrivate::clone() const
{
    return new DLogMsgPrivate(*this);
}

// class DLogMsg
/*!
    Constructs a empty DLogMsg with current time as timestamp.
 */
DLogMsg::DLogMsg()
    : DParam(__func__, *new DLogMsgPrivate)
{
    D_D(DLogMsg);
    d->m_level = Debug;
    d->m_timestamp = std::chrono::steady_clock::now().time_since_epoch();
}

/*!
    Constructs a DLogMsg instance with \a level and \a content.
 */
DLogMsg::DLogMsg(MsgLevel level, const std::string &module, const std::string &content)
    : DLogMsg()
{
    D_D(DLogMsg);
    d->m_level = level;
    d->m_module = module;
    d->m_content = content;
}

/*!
    \fn DLogMsg::DLogMsg(MsgLevel level, const char* fmt, Args...args)

    Constructs a DLogMsg instance with \a level, \a fmt, and \a args .
 */

/*!
    Destructor.
 */
DLogMsg::~DLogMsg() = default;

/*!
    Copy ctor based on \a other.
 */
DLogMsg::DLogMsg(const DLogMsg &other) = default;

/*!
    Copy assignment operator based on \a rhs.
 */
DLogMsg &DLogMsg::operator=(const DLogMsg &rhs) = default;

/*!
    Move ctor for \a other.
 */
DLogMsg::DLogMsg(DLogMsg &&other) noexcept = default;

/*!
    Move assignment operator for \a rhs.
 */
DLogMsg &DLogMsg::operator=(DLogMsg &&rhs) = default;

/*!
    Returns \c DLogMsg* points to a clone of \c this.
 */
DLogMsg *DLogMsg::clone() const
{
    return new DLogMsg(*this);
}

/*!
    Returns \c DLogMsg* points to a move of \c this.
 */
DLogMsg *DLogMsg::move() noexcept
{
    return new DLogMsg(std::move(*this));
}

/*!
    Returns \c string to print DLogMsg.
 */
std::string DLogMsg::toString() const
{
    D_D_CONST(DLogMsg);
    if (!d)
        return std::string();
    const char *levelName[] = { "[Debug]", "[Info]", "[Warning]", "[Error]" };
    std::stringstream ss;
    ss << "[" << std::chrono::duration_cast<std::chrono::seconds>(d->m_timestamp).count() << "."
       << std::chrono::duration_cast<std::chrono::microseconds>(d->m_timestamp).count() % 1000000 << "]"
       << levelName[d->m_level] << "[" << d->m_module << "]" << d->m_content;
    return ss.str();
}

/*!
    Returns \c true if this object equals to \a other.
 */
bool DLogMsg::equals(const DParam &other) const
{
    if (DParam::equals(other))
    {
        auto &rhs = static_cast<const DLogMsg &>(other);
        auto d = d_func();
        auto d2 = rhs.d_func();
        if (d && d2 && d->m_level == d2->m_level)
            //   && d->m_timestamp == d2->m_timestamp
            //   && d->m_module == d2->m_module
            //   && d->m_content == d2->m_content)
        return true;
    }

    return false;
}

/*!
    Returns \c DLogMsg::MsgLevel of message.
 */
DLogMsg::MsgLevel DLogMsg::getLevel() const
{
    D_D_CONST(DLogMsg);
    return d ? d->m_level : Debug;
}

/*!
    Returns \c string contains the log message's module name.
 */
std::string DLogMsg::getModule()const
{
    D_D_CONST(DLogMsg);
    return d ? d->m_module : std::string();
}

/*!
    Returns \c string contains the log message's content.
 */
std::string DLogMsg::getContent() const
{
    D_D_CONST(DLogMsg);
    return d ? d->m_content : std::string();
}

/*!
    Returns \c timespec contains the log message's time stamp.
 */
std::chrono::steady_clock::duration DLogMsg::getTimestamp() const
{
    D_D_CONST(DLogMsg);
    return d ? d->m_timestamp : 0ms;
}

/*!
    Serialize the TSimMsg to \a data in JSON format.
    Returns \c true if no error, otherwise \c false.
 */
//bool DLogMsg::serialize(json &data) const
//{
//    if (!DParam::serialize(data))
//        return false;

//    D_D_CONST(DLogMsg);
//    data["Level"] = d->m_level;
//    data["Content"] = d->m_content;
//    data["TimeStamp"]["sec"] = std::chrono::duration_cast<std::chrono::seconds>(d->m_timestamp).count();
//    data["TimeStamp"]["nsec"] = std::chrono::duration_cast<std::chrono::microseconds>(d->m_timestamp).count() % 1000000;

//    return true;
//}

/*!
    Deserialize the TSimMsg based on \a data in JSON format.
    Returns \c true if no error, otherwise \c false.
 */
//bool DLogMsg::deserialize(const json &data)
//{
//    D_D(DLogMsg);
//    bool ret = false;

//    if (!DParam::deserialize(data))
//        return false;

//    if (data.find("TimeStamp") == data.end())
//        return false;
//    long tv_sec = json_obj_get(data["TimeStamp"], "sec", 0);
//    long tv_nsec = json_obj_get(data["TimeStamp"], "nsec", 0);
//    d->m_timestamp = std::chrono::seconds(tv_sec) + std::chrono::microseconds(tv_nsec);
//    d->m_level = static_cast<DLogMsg::MsgLevel>(json_obj_get(data, "Level", 0));
//    d->m_content = json_obj_get(data, "Content", "");

//    return ret;
//}

} // namespace ding
