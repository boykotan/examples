#include <iostream>

template <class T>
struct scoped_ptr {
    scoped_ptr(T* init = nullptr);
    T* get() const;
    void reset(T* other = nullptr);
    T* release();
    T& operator=(const T&) = delete;
    scoped_ptr(const T&) = delete;
    scoped_ptr(T&) = delete;
    ~scoped_ptr();
    explicit operator bool() const;
    T* operator->() const;
    T operator*() const;
  private:
    T* value;
    /*T& operator=(const T&);
    T& operator=(T&);
    scoped_ptr(const T&);
    scoped_ptr(T&);*/
};

template <class T>
scoped_ptr<T> :: scoped_ptr(T* init) : value(init) {}

template <class T>
T* scoped_ptr<T> :: get() const {
    return value;
}

template <class T>
void scoped_ptr<T> :: reset(T* other){
    if (value != nullptr)
        delete value;
    
    //std::cout << " reset " << other << std::endl;
    value = other;
}

template <class T>
T* scoped_ptr<T> :: release() {
    T* tmp = value;
    value = nullptr;
    return tmp;
}

template <class T>
scoped_ptr<T> :: ~scoped_ptr() {
    delete value;
}

template <class T>
scoped_ptr<T> ::operator bool() const {
    return value != nullptr;
}

template <class T>
T* scoped_ptr<T> ::operator->() const {
    return value;
}

template <class T>
T scoped_ptr<T> ::operator*() const {
    return *value;
}

struct foo {
    int field1;
    int field2;
    foo(int value, int v2) : field1(value), field2(v2) {};
};

void bar() {
    std::cout << "in bar" << std::endl;
	scoped_ptr<foo> foo_ptr(new foo {0, 111});
    std::cout << "made foo" << std::endl;
	std::cout << (*foo_ptr).field1 << " "
        << foo_ptr->field1 << " "
        << foo_ptr.get() << " "
        << std::endl;
	if (foo_ptr)
		foo_ptr->field1 += 1;
    
    std::cout << foo_ptr->field1 << std::endl;
		
	//auto foo_ptr2(foo_ptr);// - compilation fails
	//foo_ptr = foo_ptr; // - compilation fails
	//bool st_arg_func(foo_ptr); // - compilation fails
    
    //foo_ptr.reset(); //now foo_ptr->field1 fails in runtime
    //std::cout << foo_ptr->field1;
    //foo_ptr.reset(new foo {0, 111}); //now ok
    //foo* ptr = foo_ptr.release(); //foo_ptr->field1 - fails
    //std::cout << foo_ptr->field1;
    //scoped_ptr<foo> foo_ptr2;
    //std::cout << foo_ptr2->field1;
} 

int main() {
    bar();
    return 0;
}
