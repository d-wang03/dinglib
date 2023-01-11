#include "dparam_p.h"
#include "dutility.h"
#include "dobject.h"
#include <cstring>

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
        auto &logMsg = static_cast<DLogMsg&>(param);
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
DParamPrivate::DParamPrivate():DObjectData(),m_triggerName(nullptr),m_triggerSignal(nullptr)
{
}

DParamPrivate::~DParamPrivate()
{
    if (m_triggerName)
        free(m_triggerName);
    if (m_triggerSignal)
        free(m_triggerSignal);
    m_triggerName = nullptr;
    m_triggerSignal = nullptr;
}
DParamPrivate::DParamPrivate(const DParamPrivate &other):DObjectData(other), m_triggerName(nullptr), m_triggerSignal(nullptr)
{
    if (other.m_triggerName)
        m_triggerName = strdup(other.m_triggerName);
    if (other.m_triggerSignal)
        m_triggerSignal = strdup(other.m_triggerSignal);
}

DParamPrivate *DParamPrivate::clone() const
{
    return new DParamPrivate(*this);
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
DParam::DParam(const char *type) noexcept
    : DObjectBase(type, *new DParamPrivate)
{
}

/*!
    Constructs a DParam instance with \a type and the private instance \a dd.

    \note This constructor is used for Pimpl Idiom.
 */
DParam::DParam(const char *type, DParamPrivate &dd)
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
const char * DParam::getTriggerName() const
{
    D_D_CONST(DParam);
    return d ? d->m_triggerName : nullptr;
}

/*!
    Returns the \c signal emitted with this parameter.
    If there is no trigger, returns empty std::string.
 */
const char * DParam::getTriggerSignal() const
{
    D_D_CONST(DParam);
    return d ? d->m_triggerSignal : nullptr;
}

/*!
    \internal
    Sets the trigger with \a obj and \a signal.
    \note This function is used internally.
 */
void DParam::setTrigger(const char *obj, const char *signal)
{
    D_D(DParam);
    if (!d)
        return;
    if (!obj)
    {
        if (d->m_triggerName)
            free(d->m_triggerName);
        d->m_triggerName = nullptr;
    }
    else
    {
        if (!d->m_triggerName)
            d->m_triggerName = strdup(obj);
        else if (strcmp(obj, d->m_triggerName) != 0)
        {
            free(d->m_triggerName);
            d->m_triggerName = strdup(obj);
        }
        //else obj is equals to d->m_triggerName. No need to do deep copy.
    }
    if (!signal)
    {
        if (d->m_triggerSignal)
            free(d->m_triggerSignal);
        d->m_triggerSignal = nullptr;
    }
    else
    {
        if (!d->m_triggerSignal)
            d->m_triggerSignal = strdup(signal);
        else if (strcmp(signal, d->m_triggerSignal) != 0)
        {
            free(d->m_triggerSignal);
            d->m_triggerSignal = strdup(signal);
        }
        //else obj is equals to d->m_triggerSignal. No need to do deep copy.
    }
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
    auto type = getTypeName();
    return type ? std::string(type) : std::string();
}
/*!
    Returns \c true if this object equals to \a other.

    \note This function must be override in derived classes.
 */
bool DParam::equals(const DParam &other) const
{
    // auto type1 = getTypeName();
    // auto type2 = other.getTypeName();
    // if (type1 && type2 && strcmp(type1, type2) == 0)
    //     return true;
    // else if (!type1 && !type2)
    //     return true;
    // else
    //     return false;
    return typeid(other) == typeid(*this);
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
