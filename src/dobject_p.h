#ifndef DOBJECT_P_H_
#define DOBJECT_P_H_

#include "dobject.h"
#include <vector>

namespace ding
{
class DSignal;
class DObjectPrivate : public DObjectData
{
public:
    DECLARE_PUBLIC(DObject)
    DObjectPrivate();
    DObjectPrivate(const DObjectPrivate &other);
    DObjectPrivate &operator=(const DObjectPrivate &rhs);
    DObjectPrivate(DObjectPrivate &&other) noexcept;
    DObjectPrivate &operator=(DObjectPrivate &&rhs);
    virtual ~DObjectPrivate() override;
    virtual DObjectPrivate *clone() const override;
    virtual DObjectPrivate *move() noexcept override;

    std::vector<std::shared_ptr<DSignal>>::const_iterator
                                       findSignal(void (DObject::*signal)())const;
    std::vector<std::shared_ptr<DSignal>>::const_iterator
                                       findSignal(std::string signal)const;

    std::vector<std::shared_ptr<DSignal>> m_signalList;
};

} // namespace ding

#endif /* DOBJECT_P_H_ */
