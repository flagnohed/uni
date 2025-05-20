#include <cassert>
#include <iostream>

// time taken: 0.5h
template<typename T>
class Function {

};

template<typename Ret, typename... Args>
class Function<Ret(Args...)> {
public:
    template<typename T>
    Function(T t) : storage{new Callable<T>{t}} { }

    template<typename T>
    Function& operator=(T t) {
        storage = new Callable<T>{t};
        return *this;
    }
    ~Function() = default;
        

    class Callable_Base {
    public:
        virtual Ret call(Args&&... args) = 0; 
    };

    template<typename T>
    class Callable : public Callable_Base {
    public:
        Callable(T t) : callback{t} {}
        Ret call(Args&&... args) override {
            return callback(std::forward<Args>(args)...);
        }
    private:
        T callback = T{};
    };
    Ret operator()(Args&&... args) {
        return storage->call(std::forward<Args>(args)...);
    }
private:
    Callable_Base* storage;
    
};

void test()
{
    std::cout << "Function call!" << std::endl;
}

int add(int a, int b)
{
    return a + b;
}

struct Test
{
    void operator()()
    {
	std::cout << "Function object call!" << std::endl;
    }
};

struct Multiply
{
    int operator()(int a, int b)
    {
	return a * b;
    }
};

/* Expected output:

==== Testcase 1: void() ====
Function call!
Function object call!
x = 5
==== Testcase 2: int(int, int) ====
7 * 4 = 28
3 + 5 = 8
11 - 5 = 6

*/

int main()
{

    std::cout << "==== Testcase 1: void() ====" << std::endl;
    {
	Function<void()> fun { test };
	fun();

	fun = Test{};
	fun();
	
	int x { 5 };
	fun = [&x]() { std::cout << "x = " << x << std::endl; };
	fun();
    }

    std::cout << "==== Testcase 2: int(int, int) ====" << std::endl;
    {
	Function<int(int, int)> fun { Multiply{} };

	std::cout << "7 * 4 = " << fun(7, 4) << std::endl;
	
	fun = add;
	std::cout << "3 + 5 = " << fun(3, 5) << std::endl;

	fun = [](int a, int b) { return a - b; };
	std::cout << "11 - 5 = " << fun(11, 5) << std::endl;
    }
}
