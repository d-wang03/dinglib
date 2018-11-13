#ifndef DMSG_P_H_
#define DMSG_P_H_

#include "dobjectbase.h"
#include "dglobal.h"
#include <string>

namespace ding
{
class DParam;
class DObject;
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
    std::weak_ptr<DObject> m_triggerObj;
    std::string m_triggerSig;
};

} // namespace UmbrellaFramework

#endif /* TMSG_P_H_ */
