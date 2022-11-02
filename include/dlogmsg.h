#ifndef DLOGMSG_H_
#define DLOGMSG_H_

#include "dparam.h"
#include "dutility.h"
#include <chrono>

namespace ding
{
class DLogMsgPrivate;
class DLogMsg : public DParam
{
    DECLARE_PRIVATE(DLogMsg)
public:
    enum MsgLevel
    {
        Debug,
        Info,
        Warning,
        Error
    };
    DLogMsg();
    DLogMsg(MsgLevel level, const std::string& module, const std::string &content);
    template <typename... Args>
    DLogMsg(MsgLevel level, const std::string& module, const char *fmt, Args&&... args)
        : DLogMsg(level, module, string_format(std::string(fmt), std::forward<Args>(args)...))
    {
    }

    virtual ~DLogMsg() override;
    DLogMsg(const DLogMsg &other);
    DLogMsg &operator=(const DLogMsg &rhs);
    DLogMsg(DLogMsg &&other) noexcept;
    DLogMsg &operator=(DLogMsg &&rhs);

    virtual DLogMsg *clone() const override;
    virtual DLogMsg *move() noexcept override;
    virtual std::string toString() const override;
    virtual bool equals(const DParam &other) const override;

    MsgLevel getLevel() const;
    std::string getModule() const;
    std::string getContent() const;
    std::chrono::steady_clock::duration getTimestamp() const;
};

} // namespace ding

#endif /* DLogMsg_H_ */
