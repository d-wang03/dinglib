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
    }
    void signal1(int a, double b, float c)
    {
        std::cout << "signal1 triggered:"
                  << a << b << c << std::endl;
//        emitSignal(__func__, a, b, c);
        call_func(&MyObject::slot1, a, b, c);
    }
    void slot1(int a, double b, float c)
    {
        std::cout << "Slot1 triggered:"
                  << a << b << c << std::endl;
    }
    void signal2(int a)
    {
        std::cout << __func__ << a << std::endl;
        emitSignal(__func__, &MyObject::signal2, a);
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
    ptr1->connect("signal1", ptr2, &MyObject::slot1);
    ptr1->connect("signal2", ptr2, &MyObject::slot2);
    ptr1->signal1(10, 10.888888888888888888, 3.2222f);
    ptr1->signal2(100);
    std::cout << "ptr1:" << (int)ptr1.get() << "\nptr2:" << (int)ptr2.get() << std::endl;
    return 0;
}
