#include "dobjectbase.h"

namespace ding
{
/*!
    \class ding::DObjectBase
    \brief The DObjectBase class is the root class of ding library.
    \ingroup core
    \inmodule ding

    Ding library is based on two models, object model and parameter model.
    These models are all derived from DObjectBase.

    DObjectBase defined the Pimpl Idiom, the outer class only define interfaces, the implementations
    are in the related private class. To follow the Pimpl Idiom, the outer class inherits DObjectBase,
    while the private implementation class derived from DObjectData.

    Several macros are offered to assist the use of Pimpl Idiom, including D_DECLARE_PRIVATE,
    D_DECLARE_PUBLIC, D_D, D_Q, D_D_CONST, D_Q_CONST.

    As an reinforcement of the type system, DObjectBase has a type name string as its property.
    The type string can be set and get through accessor methods. It should be set to the class name,
    and then can tell you its type.

    An recommended way to set type string is using __func__ in ctor for derived classes.
    \code
    MyObject():DObjectBase(__func__, new MyObjectPrivate)
    \endcode

    We use RTTI to identify the object's name.
    Template method is_type<T>() and is_derived_from<T>() are good choices.
*/
/*!
    \class ding::DObjectData
    \brief The DObjectData class is the abstract root class for the Pimpl Idiom.
    \ingroup core
    \inmodule ding
    \internal

    The Pimpl Idiom is the mechanism to resolve the problems like "DLL HELL".
    When dynamic library is updated, if its interface header changed with memory topology,
    simply replace the old file without recompiling the client programs (the programs using this library),
    will lead to crash.

    The Pimpl Idiom avoids these problems by hidden the data structures to source file,
    and keep the interface unchanged.
*/

/*!
    \class ding::DObjectFactory
    \brief The DObjectFactory class provides dynamic creation for objects and parameters.
    \ingroup core
    \inmodule ding

    The objects and parameters must enable dynamic creation by D_REGISTER_OBJECT_CLASS().

    A user can create an object or a parameter like this:
    \code
    auto obj = DObjectFactory::CreateObject<DObject>("DObject");//obj is std::unique_ptr<DObject>
    //or
    std::shared_ptr<DObject> obj2 = DObjectFactory::Create<DObject>("DObject");
    \endcode

    \sa D_REGISTER_OBJECT_CLASS()
*/

/*!
    \class ding::DRegister
    \brief The DRegister class offers necessary information for dynamic creation.
    \ingroup core
    \inmodule ding
    \internal
    The DRegister class offers necessary information for dynamic creation.
    To simplify the use of dynamic creation, D_REGISTER_OBJECT_CLASS() is created as grammar sugar.

    \note Do not use DRegister, it is used internally.
    \sa DObjectFactory, D_REGISTER_OBJECT_CLASS(), CREATE_OBJECT_PTR()
*/

// Class DObjectBase
/*!
    Ctor with type \a name and implementation \a dd.
    It is only used for derived classes.
 */
DObjectBase::DObjectBase(const std::string &name, DObjectData &dd)
    : d_ptr(std::unique_ptr<DObjectData>(&dd))
{
    d_ptr->q_ptr = this;
    setTypeName(name);
}
/*!
    Copy ctor based on \a other.
 */
DObjectBase::DObjectBase(const DObjectBase &other)
    : d_ptr(std::unique_ptr<DObjectData>(other.d_ptr->clone()))
{
}
/*!
    Copy assignment operator for \a rhs.
 */
DObjectBase &DObjectBase::operator=(const DObjectBase &rhs)
{
    d_ptr = std::unique_ptr<DObjectData>(rhs.d_ptr->clone());
    return *this;
}

/*!
    \fn DObjectBase::DObjectBase(DObjectBase&& other)
    Move ctor for \a other.
 */
DObjectBase::DObjectBase(DObjectBase &&other) noexcept
    : d_ptr(other.d_ptr->move())
{
}

/*!
    \fn DObjectBase& DObjectBase::operator=(DObjectBase&& rhs)
    Move assignment operator for \a rhs.
 */
DObjectBase &DObjectBase::operator=(DObjectBase &&rhs)
{
    d_ptr = std::unique_ptr<DObjectData>(rhs.d_ptr->move());
    return *this;
}

/*!
    Sets \a type of the object.
    \sa getTypeName()
 */
void DObjectBase::setTypeName(const std::string &type)
{
    if (!d_ptr)
        return;
    d_ptr->m_typename = type.empty() ? d_ptr->m_typename : type;
}

/*!
    Returns \c type of the object.
    \sa setTypeName()
 */
std::string DObjectBase::getTypeName() const
{
    return d_ptr ? d_ptr->m_typename : "";
}

/*!
    \fn DObjectBase* DObjectBase::clone() const
    Returns a clone of this object in \c DObjectBase* .
 */

/*!
    \fn DObjectBase* DObjectBase::move()
    Move this object to a new object and return it as \c DObjectBase* .
 */

/*!
    \fn bool DObjectBase::is_type() const
    Determine whether the object's type equals to template type T.
    Returns \c true or \c false.

    This method uses RTTI features, with low performance.
    \code
    auto obj = createObject<TLogic>();
    bool ret = obj->is_type<DObject>(); // false
    ret = obj->is_type<TLogic>(); // true
    \endcode
 */

/*!
    \fn bool DObjectBase::is_type(const std::string& type) const
    Determine whether the object's type equals to \a type.
    Returns \c true or \c false.

    Compared to RTTI version, this method is very fast.
    \code
    auto obj = createObject<TLogic>();
    bool ret = obj->is_type("DObject"); // false
    ret = obj->is_type("TLogic"); // true
    \endcode
 */

/*!
    \fn bool DObjectBase::is_derived_from() const
    Determine whether the object is derived from template type T.
    Returns \c true or \c false.

    This method uses RTTI features, with low performance.
    \code
    auto obj = createObject<TLogic>();
    bool ret = obj->is_derived_from<DObject>(); // true
    ret = obj->is_derived_from<TLogic>(); // true
    \endcode
 */
// class DObjectFactory
/*!
 * \internal DObjectFactory::getInstance
 */
DObjectFactory &DObjectFactory::getInstance()
{
    static DObjectFactory factory;
    return factory;
}

/*!
    \internal
    Register new object creator \a np with \a objectClassName
    \note Do not call this function manually. It is used internally.
 */
void DObjectFactory::RegisterObjectClass(const char *objectClassName, DNewObjectFunc np)
{
    DObjectFactory::getInstance().m_dynObjectCreateMap.emplace(objectClassName, np);
}

// class DRegister
/*!
    \internal
    Register new object creator \a np with \a objectClassName
    \note Do not call this function manually. It is used internally.
 */
DRegister::DRegister(const char *objectClassName, DNewObjectFunc np)
{
    DObjectFactory::RegisterObjectClass(objectClassName, np);
}

} // namespace ding
