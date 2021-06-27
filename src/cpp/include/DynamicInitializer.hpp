#pragma once

// https://stackoverflow.com/questions/32979033/how-can-i-ensure-that-global-variables-are-initialized-in-the-correct-order

template<class T>
struct DynamicInitializer
{
    // These members have to be POD types to be zero-initialized at static initialization phase
    // prior to the dynamic initialization phase which invokes constructors of global objects.
    static T* instance_;
    static unsigned ref_count_;

    DynamicInitializer() {
        if(!ref_count_++)
            instance_ = new T;
    }

    ~DynamicInitializer() {
        if(!--ref_count_)
            delete instance_;
    }

    operator T&() const { return *instance_; }
    T* operator->() const { return instance_; }

    DynamicInitializer(DynamicInitializer const&) = delete;
    DynamicInitializer& operator=(DynamicInitializer const&) = delete;
};

template<class T>
unsigned DynamicInitializer<T>::ref_count_ = 0;

template<class T>
T* DynamicInitializer<T>::instance_ = 0;
