#ifndef DPARAM_H
#define DPARAM_H

#include "dglobal.h"
#include "dobjectbase.h"

namespace ding
{
class DParamPrivate;
class DObject;

class DParam : public DObjectBase
{
    DECLARE_PRIVATE(DParam)
public:
    // ctor
    DParam() noexcept;
    // copy ctor
    DParam(const DParam &other);
    DParam &operator=(const DParam &other);
    // move ctor
    DParam(DParam &&other) noexcept;
    DParam &operator=(DParam &&rhs);
    // dtor
    virtual ~DParam() override;
    // clone
    virtual DParam *clone() const override;
    // move
    virtual DParam *move() noexcept override;

    // to string
    virtual std::string toString() const;

    // equal
    virtual bool equals(const DParam &other) const;
    bool operator==(const DParam &rhs) const;

    // properties
    std::string getTriggerName() const;
    std::string getTriggerSignal() const;
    void setTrigger(std::string obj, std::string signal);

protected:
    DParam(const std::string &type) noexcept;
    DParam(const std::string &type, DParamPrivate &dd);
};

} // namespace ding
#endif // DPARAM_H
