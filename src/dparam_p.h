#ifndef DMSG_P_H_
#define DMSG_P_H_

#include "dparam.h"

namespace ding
{
class DParamPrivate : public DObjectData
{
public:
    DECLARE_PUBLIC(DParam)
    DParamPrivate() = default;
    virtual ~DParamPrivate() override = default;
    DParamPrivate(const DParamPrivate &other) = default;
    DParamPrivate &operator=(const DParamPrivate &rhs) = default;
    DParamPrivate(DParamPrivate &&other) noexcept = default;
    DParamPrivate &operator=(DParamPrivate &&rhs) = default;
    virtual DParamPrivate *clone() const override;
    virtual DParamPrivate *move() noexcept override;

private:
    std::string m_triggerName;
    std::string m_triggerSignal;
};

} // namespace ding

#endif /* TMSG_P_H_ */
