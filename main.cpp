#include "dobject.h"
#include <iostream>

using namespace ding;

class MyObject: public DObject
{
public:
    MyObject(): DObject()
    {
        addSignal("signal1", &MyObject::signal1);
        addSignal("signal2", &MyObject::signal2);
        addSignal("signal3", &MyObject::signal3);
    }
    void signal1(int a, double b, float c)
    {
        std::cout << "signal1 triggered:"
                  << a << b << c << std::endl;
        emitSignal(&MyObject::signal1, a, b, c);
    }
    void slot1(int a, double b, float c)
    {
        std::cout << "Slot1 triggered:"
                  << a << b << c << std::endl;
    }
    void signal2(int a)
    {
        std::cout << __func__ << a << std::endl;
//        emitSignal(__func__, &MyObject::signal2, a);
        EMIT(MyObject, signal2, a);
    }
    void signal3()
    {
        EMIT(MyObject, signal3)
    }
    void slot2(int a)
    {
        std::cout << __func__ << a << std::endl;
    }

    template<typename T, typename... Args>
    void call_func(T MyObject::* func, Args&&...args)
    {
        call(static_cast<T DObject::*>(func), std::forward<Args>(args)...);
    }
};

int main()
{
    std::shared_ptr<MyObject> ptr1(new MyObject);
    std::shared_ptr<MyObject> ptr2(new MyObject);
    ptr1->connect(&MyObject::signal1, ptr2, &MyObject::slot1);
    ptr1->connect(&MyObject::signal2, ptr2, &MyObject::slot2);
    ptr1->signal1(10, 10.888888888888888888, 3.2222f);
    ptr1->signal2(100);
    return 0;
}
