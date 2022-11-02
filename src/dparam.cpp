#include "dparam_p.h"
#include "dutility.h"
#include "dobject.h"

namespace ding
{
/*!
    \class ding::DParam
    \brief The DParam class is the root class of ding libraries' parameter model.
    \ingroup core
    \inmodule ding

    The DParam class is derived from \l DObjectBase as the root class of parameter model. It is designed to
    carry the communication information between the signals and slots.

    To cover the needs for different objects, a lot of parameter classes are designed to inherit DParam class.

    Since there is many parameter classes, it is recommended to check if the parameter's type is expected,
    using the methods introduced in \l DObjectBase. Like the following code:
    \code
    void mySlotFunc(DParam& param)
    {
        if (!param.is_type<DLogMsg>())
            return;
        auto logMsg = static_cast<DLogMsg&>(param);
        //do something
    }
    \endcode

    Equalization is an important method in parameter classes. \l equals() must be override in derived classes.
    Then, checking equalization can simply use operator ==.

    As introduced in \l DObjectBase, type string must be set correctly. It is recommended to set type in constructor.
    \code
    MyParam():DParam(__func__)
    {
        //...
    }
    \endcode

    To assist debugging, virtual function toString() is provided. Programmers can check the parameter content
    simply by printing this string.

    \sa DObjectBase
*/

// Class DParamPrivate
DParamPrivate *DParamPrivate::clone() const
{
    return new DParamPrivate(*this);
}
DParamPrivate *DParamPrivate::move() noexcept
{
    return new DParamPrivate(std::move(*this));
}
// Class DParam
D_REGISTER_PARAM_CLASS(DParam);
/*!
    Constructs a DParam instance with type "DParam".
    \note It is NOT recommended to use this constructor.
 */
DParam::DParam() noexcept
    : DObjectBase(__func__, *new DParamPrivate)
{
}

/*!
    Constructs a DParam instance with \a type.
    \note It is recommended to use this constructor.
 */
DParam::DParam(const std::string &type) noexcept
    : DObjectBase(type, *new DParamPrivate)
{
}

/*!
    Constructs a DParam instance with \a type and the private instance \a dd.

    \note This constructor is used for Pimpl Idiom.
 */
DParam::DParam(const std::string &type, DParamPrivate &dd)
    : DObjectBase(type, dd)
{
}
/*!
    Copy ctor based on \a other.
 */
DParam::DParam(const DParam &) = default;
/*!
    Copy assignment operator based on \a rhs.
 */
DParam &DParam::operator=(const DParam &) = default;
/*!
    Default move ctor for \a other.
 */
DParam::DParam(DParam &&) noexcept = default;
/*!
    Default move assignment operator for \a rhs.
 */
DParam &DParam::operator=(DParam &&) = default;

/*!
    Destroys a DObject instance.
 */
DParam::~DParam() {}

/*!
    Returns the \c trigger name.
    If there is no trigger, returns empty std::string.
 */
std::string DParam::getTriggerName() const
{
    D_D_CONST(DParam);
    return d.m_triggerName;
}

/*!
    Returns the \c signal emitted with this parameter.
    If there is no trigger, returns empty std::string.
 */
std::string DParam::getTriggerSignal() const
{
    D_D_CONST(DParam);
    return d.m_triggerSignal;
}

/*!
    \internal
    Sets the trigger with \a obj and \a signal.
    \note This function is used internally.
 */
void DParam::setTrigger(std::string obj, std::string signal)
{
    D_D(DParam);
    d.m_triggerName = std::move(obj);
    d.m_triggerSignal = std::move(signal);
}

/*!
    Returns the pointer of copied parameter object.

    It is strongly recommended to implement own copy function for each
    derived class. Do deep copy, avoid shadow copy.
 */
DParam *DParam::clone() const
{
    return new DParam(*this);
}

/*!
    Returns the pointer of moved parameter object.

    It is a performance optimization for clone().
 */
DParam *DParam::move() noexcept
{
    return new DParam(std::move(*this));
}

/*!
    Returns a \c string of parameter object.
    The default behavior is printing the parameter type.

    This function is usually used in printing the parameter object.
    \note toString does not equal to serialization. It may ignore many details
    that no need to show but important to serialization.
 */
std::string DParam::toString() const
{
    return getTypeName();
}
/*!
    Returns \c true if this object equals to \a other.

    \note This function must be override in derived classes.
 */
bool DParam::equals(const DParam &other) const
{
    return getTypeName() == other.getTypeName();
}
/*!
    Override operator ==.
    Returns \c true if this object equals to \a rhs.

    \note It just return result of virtual function equals().
    Thus derived classes don't need to override it.
    \sa equals()
 */
bool DParam::operator==(const DParam &rhs) const
{
    return equals(rhs);
}

} // namespace ding
