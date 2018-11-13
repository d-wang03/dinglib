#ifndef DGLOBAL_H_
#define DGLOBAL_H_

#include "dosdef.h"

//macros for delete object.
#define _delete(x) if (x) {delete x; x = NULL;}

//macros for unused variables.
#define D_UNUSED(x) (void)x;

//macros for signal and slot mechanism.
#define SIGNAL(class_name, signal_name) public: \
    void signal_name(int param)\
    {\
        emitSignal(&class_name::signal_name, param);\
    }

#define EMIT(class_name,signal_name,...) emitSignal(&class_name::signal_name,##__VA_ARGS__)

//macros for reflection.
#define ADD_SIGNAL(class_name,signal_name)    do \
{\
    addSignal(#signal_name,&class_name::signal_name);\
}while(0)

//macros for disable copy ctors.
#define DISABLE_COPY(Class) \
    Class(const Class &) = delete;\
    Class &operator=(const Class &) = delete;\
    virtual Class* clone()const noexcept override {return nullptr;}

#define DISABLE_MOVE(Class) \
    Class(Class &&) = delete; \
    Class &operator=(Class &&) = delete;\
    virtual Class* move() noexcept override {return nullptr;}

//macros for Pimpl Idiom.
#define DECLARE_PRIVATE(Class) \
    private:\
    inline Class##Private& d_func() { return reinterpret_cast<Class##Private&>(*d_ptr); } \
    inline const Class##Private& d_func() const { return reinterpret_cast<const Class##Private&>(*d_ptr); } \
    friend class Class##Private;

#define DECLARE_PUBLIC(Class)                                    \
    inline Class * q_func() { return static_cast<Class *>(q_ptr); } \
    inline const Class* q_func() const { return static_cast<const Class *>(q_ptr); } \
    friend class Class;

#define D_D(Class) Class##Private & d = d_func()
#define D_Q(Class) Class * const q = q_func()
#define D_D_CONST(Class) const Class##Private & d = d_func()
#define D_Q_CONST(Class) const Class * const q = q_func()

//macros for dynamic object creation.
#define D_REGISTER_OBJECT_CLASS(class_name) \
class DRegister##class_name \
{ \
public: \
    static DObjectBase * NewInstance() \
    { \
        return new class_name(); \
    } \
private: \
    static DRegister reg; \
}; \
DRegister DRegister##class_name::reg(#class_name, DRegister##class_name::NewInstance)

#endif /* DGLOBAL_H_ */