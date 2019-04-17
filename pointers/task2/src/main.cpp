#include <iostream>
#include <cassert>

template <class T>
struct data {
    T* value;
    size_t counter;
    data(T* init) : value(init), counter(1) {};
};

template <class T>
struct shared_ptr {
    shared_ptr(T* init = nullptr);
    T* get() const;
    void reset(T* other = nullptr);
    T& operator=(const T&);
    shared_ptr(const T&);
    ~shared_ptr();
    explicit operator bool() const;
    T* operator->() const;
    T& operator*() const;
  private:
    data<T>* value;
};

template <class T>
shared_ptr<T> :: shared_ptr(T* init) : value(new data<T>(init)) {}

template <class T>
T* shared_ptr<T> :: get() const {
    return value->value;
}

template <class T>
T& shared_ptr<T> :: operator=(const T& rval) {
    value=rval;
    (rval->counter)++;
    return *this;
}

template <class T>
shared_ptr<T> :: shared_ptr(const T& rval) : value(rval) {
    (rval->counter)++;
}

template <class T>
void shared_ptr<T> :: reset(T* other) {
    shared_ptr <T> tmp;
    if (value != nullptr)
        swap(tmp, this);
    value = other;
}

template <class T>
shared_ptr<T> :: ~shared_ptr() {
    (value->counter)--;
    if (value->counter == 0)
        delete value;
}

template <class T>
shared_ptr<T> ::operator bool() const {
    return value != nullptr;
}

template <class T>
T* shared_ptr<T> ::operator->() const {
    return (value->value);
}

template <class T>
T& shared_ptr<T> ::operator*() const {
    return &(value->value);
}

struct foo {
    int field1;
    int field2;
    foo(int value, int v2) : field1(value), field2(v2) {};
};

int main() {
    shared_ptr<foo> foo_shptr(new foo {888, 999});
    shared_ptr<foo> foo_shptr2 = foo_shptr;

    assert(foo_shptr.get() == foo_shptr2.get());

    std::cout << foo_shptr->field1 << " "
             << foo_shptr2->field1 << std::endl;
    return 0;
}
