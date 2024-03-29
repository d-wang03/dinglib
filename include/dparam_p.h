#ifndef DMSG_P_H_
#define DMSG_P_H_

#include "dparam.h"

namespace ding
{
class DParamPrivate : public DObjectData
{
public:
    DECLARE_PUBLIC(DParam)
    DParamPrivate();
    virtual ~DParamPrivate() override;
    DParamPrivate(const DParamPrivate &other);
    DParamPrivate &operator=(const DParamPrivate &rhs);
    virtual DParamPrivate *clone() const override;

private:
    char *m_triggerName;
    char *m_triggerSignal;
};

} // namespace ding

#endif /* TMSG_P_H_ */
