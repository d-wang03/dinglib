#ifndef DMSGS_H_
#define DMSGS_H_

#include "dparam.h"
#include "dutility.h"

namespace ding
{
class DNormalMsgPrivate;
class DNormalMsg : public DParam
{
    DECLARE_PRIVATE(DNormalMsg)
public:
    DNormalMsg();
    DNormalMsg(const std::string &content);
    template <typename... Args>
    DNormalMsg(const char *fmt, Args&&... args)
        : DNormalMsg(string_format(fmt, std::forward<Args>(args)...))
    {
    }

    virtual ~DNormalMsg() override;
    DNormalMsg(const DNormalMsg &other);
    DNormalMsg &operator=(const DNormalMsg &rhs);
    DNormalMsg(DNormalMsg &&other) noexcept;
    DNormalMsg &operator=(DNormalMsg &&rhs);

    virtual DNormalMsg *clone() const override;
    virtual DNormalMsg *move() noexcept override;
    virtual std::string toString() const override;
    virtual bool equals(const DParam &other) const override;
};

} // namespace ding

#endif /* DMSGS_H_ */
