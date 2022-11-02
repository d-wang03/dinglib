#include "dnormalmsg.h"
#include "dparam_p.h"
#include "dutility.h"

#include <stdarg.h>
#include <memory>
#include <string>
#include <cstdio>

namespace ding
{
/*!
    \class ding::DNormalMsg
    \brief The DNormalMsg class is a parameter class contains a single string.
    \ingroup core
    \inmodule ding

    It is designed as immutable object.
    Once the content is set in ctor, it will remain unchanged during the whole life cycle.

    User can using format string to construct the message.
    The format is somehow like qt.
    \code
    DNormalMsg msg("%1 and %2 love each other.", "Bob", "Sue");
    //"Bob and Sue love each other."

    DNormalMsg msg("%1 has %2 apples.", "Bob", 2);
    //"Bob has 2 apples."
    \endcode

*/

// class DNormalMsgPrivate
class DNormalMsgPrivate : public DParamPrivate
{
    DECLARE_PUBLIC(DNormalMsg)
public:
    DNormalMsgPrivate() = default;
    virtual ~DNormalMsgPrivate() override = default;
    DNormalMsgPrivate(const DNormalMsgPrivate &other) = default;
    DNormalMsgPrivate &operator=(const DNormalMsgPrivate &rhs) = default;
    DNormalMsgPrivate(DNormalMsgPrivate &&other) noexcept = default;
    DNormalMsgPrivate &operator=(DNormalMsgPrivate &&rhs) = default;
    virtual DNormalMsgPrivate *clone() const noexcept override;
    virtual DNormalMsgPrivate *move() noexcept override;

private:
    std::string m_content;
};

DNormalMsgPrivate *DNormalMsgPrivate::clone() const noexcept
{
    return new DNormalMsgPrivate(*this);
}

DNormalMsgPrivate *DNormalMsgPrivate::move() noexcept
{
    return new DNormalMsgPrivate(std::move(*this));
}

// class DNormalMsg
D_REGISTER_PARAM_CLASS(DNormalMsg);

/*!
    \fn DNormalMsg::DNormalMsg()
    Constructs a DNormalMsg instance with empty content.
 */
DNormalMsg::DNormalMsg()
    : DParam(__func__, *new DNormalMsgPrivate)
{
}

/*!
    \fn DNormalMsg::DNormalMsg(const std::string& content);
    Constructs a DNormalMsg instance with \a content.
 */
DNormalMsg::DNormalMsg(const std::string &content)
    : DNormalMsg()
{
    D_D(DNormalMsg);
    d.m_content = content;
}

/*!
    \fn DNormalMsg::DNormalMsg(const char* fmt, Args... args)
    Constructs a DNormalMsg instance with \a fmt and \a args.
 */
/*!
    Destruct the DNormalMsg instance.
 */
DNormalMsg::~DNormalMsg() = default;
/*!
    Copy ctor based on \a other.
 */
DNormalMsg::DNormalMsg(const DNormalMsg &other) = default;
/*!
    Copy assignment operator based on \a rhs.
 */
DNormalMsg &DNormalMsg::operator=(const DNormalMsg &rhs) = default;
/*!
    Move ctor for \a other.
 */
DNormalMsg::DNormalMsg(DNormalMsg &&other) noexcept = default;
/*!
    Move assignment operator for \a rhs.
 */
DNormalMsg &DNormalMsg::operator=(DNormalMsg &&rhs) = default;
/*!
    Returns \c DNormalMsg* points to a clone of \c this.
 */
DNormalMsg *DNormalMsg::clone() const
{
    return new DNormalMsg(*this);
}
/*!
    Returns \c DNormalMsg* points to a move of \c this.
 */
DNormalMsg *DNormalMsg::move() noexcept
{
    return new DNormalMsg(std::move(*this));
}
/*!
    Returns \c string to print DNormalMsg.
 */
std::string DNormalMsg::toString() const
{
    D_D_CONST(DNormalMsg);
    return d.m_content;
}

/*!
    Returns \c true if this object equals to \a other.
 */
bool DNormalMsg::equals(const DParam &other) const
{
    if (DParam::equals(other))
    {
        auto rhs = static_cast<const DNormalMsg &>(other);
        return d_func().m_content == rhs.d_func().m_content;
    }

    return false;
}

} // namespace ding
