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
    DParam(const char *type) noexcept;
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
    //init
    void init(const char *type);
    // to string
    virtual std::string toString() const;

    // equal
    virtual bool equals(const DParam &other) const;
    bool operator==(const DParam &rhs) const;

    // properties
    const char * getTriggerName() const;
    const char * getTriggerSignal() const;
    void setTrigger(const char *obj, const char *signal);

protected:
    DParam(const char *type, DParamPrivate &dd);
};

} // namespace ding
#endif // DPARAM_H
