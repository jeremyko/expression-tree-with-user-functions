#ifndef	_GENERIC_DELEGATE_H_
#define	_GENERIC_DELEGATE_H_

template<typename return_type, typename... params>
class GenericMemberFuncDelegate
{
    typedef return_type(*Type)(void* callee, params...);

public:
    GenericMemberFuncDelegate()
        : fpCallee(NULL)
        , fpCallbackFunction(NULL) {}

    GenericMemberFuncDelegate(void* callee, Type function)
        : fpCallee(callee)
        , fpCallbackFunction(function) {}

    template <class T, return_type(T::*TMethod)(params...)>
    static GenericMemberFuncDelegate from_function(T* callee)
    {
        GenericMemberFuncDelegate d(callee, &methodCaller<T, TMethod>);
        return d;
    }

    return_type operator()(params... xs) const
    {
        return (*fpCallbackFunction)(fpCallee, xs...);
    }

private:

    void* fpCallee;
    Type fpCallbackFunction;

    template <class T, return_type(T::*TMethod)(params...)>
    static return_type methodCaller(void* callee, params... xs)
    {
        T* p = static_cast<T*>(callee);
        return (p->*TMethod)(xs...);
    }
};

#endif

/*
A a;
auto d = GenericMemberFuncDelegate<int, int>::from_function<A, &A::foo>(&a);
auto d2 = GenericMemberFuncDelegate<int, int, int, char>::from_function<A, &A::bar>(&a);
printf("delegate with return value: d(42)=%d\n", d(42));
printf("for d2: d2(42, 2, 'a')=%d\n", d2(42, 2, 'a'));

CUserFunctions userFuncs;
auto user_func_delegate = GenericMemberFuncDelegate<int, char*>::from_function<CUserFunctions, &CUserFunctions::StrLen>(&userFuncs);
int nLen = user_func_delegate ("test!");
*/