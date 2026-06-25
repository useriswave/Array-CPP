#pragma once

#include <iostream>   // std::ostream
#include <new>        // std::nothrow
#include <cassert>    // assert()
#include <functional> // std::function
#include <initializer_list>

template <typename T>
class Array
{
public:
    Array() = default;
    explicit Array(const std::size_t length)
        : m_length { length }
        , m_capacity { length }
    {
        m_array = new T[m_capacity];
    }

    Array(const Array& arr)
        : m_length { arr.m_length }
    {
        reserve(arr.m_capacity);

        for (std::size_t i{}; i < m_length; ++i) {
            m_array[i] = arr.m_array[i];
        }
    }

    Array(Array&& arr) noexcept
        : m_array { arr.m_array }
        , m_length { arr.m_length }
        , m_capacity { arr.m_capacity }
    {
        arr.m_array = nullptr;
        arr.m_length = 0;
        arr.m_capacity = 0;
    }

    Array(std::initializer_list<T> list)
        : Array(list.size())
    {
        std::copy(list.begin(), list.end(), m_array);
    }

    ~Array()
    {
        delete[] m_array;
    }

public:
    void add(const T& value);
    void add(T&& value);
    void insert(const T& value, std::size_t index);
    void remove(std::size_t index);
    void bubbleSort(std::function<bool(const T& a, const T&b)> fn);
    void selectionSort(std::function<bool(const T& a, const T& b)> fn);
    void reverse();
    void reserve(std::size_t capacity);
    void clear();
    void set(std::size_t index, const T& value);

public:
    [[nodiscard]] const T& get(std::size_t index) const;
    [[nodiscard]] const T& getFirst() const;
    [[nodiscard]] const T& getLast() const;
    [[nodiscard]] T& getFirst();
    [[nodiscard]] T& getLast() ;
    [[nodiscard]] T& get(std::size_t index);
    [[nodiscard]] std::size_t capacity() const { return m_capacity; }
    [[nodiscard]] std::size_t length() const { return m_length; }

public:
    T* begin() const { return m_array; }
    T* end() const { return m_array + m_length; }

public:
    template <typename U>
    friend std::ostream& operator<<(std::ostream& out, const Array<U>& arr);
    Array& operator=(const Array<T>& arr);
    Array& operator=(Array<T>&& arr) noexcept;
    Array& operator=(std::initializer_list<T> list);
    const T& operator[](std::size_t index) const;
    T& operator[](std::size_t index);
    bool operator>(const Array<T>& arr) const;
    bool operator<(const Array<T>& arr) const;
    bool operator==(const Array<T>& arr) const;
    bool operator!() const;

private:
    void moveTo(T*& other);
    void resize();

private:
    T* m_array{};
    std::size_t m_length{};
    std::size_t m_capacity{};
};

template <typename T>
void Array<T>::resize()
{
    const std::size_t tempCap { m_capacity == 0 ? 2 : m_capacity*2 };
    T* temp { new (std::nothrow) T[tempCap] };

    if (!temp) {
        std::cerr << "ERROR: Memory allocation failed.\n";
        return;
    }

    if (m_array) {
        moveTo(temp);
    }

    delete[] m_array;
    m_array = temp;
    m_capacity = tempCap;
}

template <typename T>
void Array<T>::moveTo(T*& other)
{
    for (std::size_t i{}; i < m_length; ++i) {
        other[i] = std::move(m_array[i]);
    }
}

template <typename T>
void Array<T>::add(const T& value)
{
    if (m_length >= m_capacity) {
        resize();
    }

    m_array[m_length++] = value;
}

template <typename T>
void Array<T>::add(T&& value)
{
    if (m_length >= m_capacity) {
        resize();
    }

    m_array[m_length++] = std::move(value);
}

template <typename T>
void Array<T>::insert(const T& value, std::size_t index)
{
    assert(index <= m_length && "Index out of bounds at insert");

    if (m_length >= m_capacity) {
        resize();
    }

    for (std::size_t i {m_length}; i > index; --i) {
        m_array[i] = std::move(m_array[i-1]);
    }

    m_array[index] = value;
    ++m_length;
}

template <typename T>
void Array<T>::remove(const std::size_t index)
{
    assert(index < m_length && "Index out of bounds at remove.");

    for (std::size_t i{index}; i < m_length-1; ++i) {
        m_array[i] = std::move(m_array[i+1]);   // this is amazing
    }

    --m_length;
}

template <typename T>
void Array<T>::bubbleSort(std::function<bool(const T& a, const T& b)> fn)
{
    if (m_length == 0) 
        return;

    for (std::size_t i{}; i < m_length - 1; ++i) {
        for (std::size_t j{}; j < m_length - 1 - i; ++j) {
            if (fn(m_array[j], m_array[j+1])) {
                std::swap(m_array[j], m_array[j+1]);
            }
        }
    }
}

template <typename T>
void Array<T>::selectionSort(std::function<bool(const T& a, const T& b)> fn)
{
    if (m_length == 0)
        return;

    for (std::size_t i{}; i < m_length; ++i) {
        auto checkpoint { i };

        for (std::size_t j{i}; j < m_length; ++j) {
            if (fn(m_array[j], m_array[checkpoint])) {
                checkpoint = j;
            }
        }

        std::swap(m_array[i], m_array[checkpoint]);
    }
}

template <typename T>
void Array<T>::reverse()
{
    if (m_length == 0)
        return;

    std::size_t start{};
    std::size_t end{ m_length-1 };

    while (start < end) {
        std::swap(m_array[start++], m_array[end--]);
    }
}

template <typename T>
void Array<T>::reserve(const std::size_t capacity)
{
    if (capacity > m_capacity) {
        T* temp { new (std::nothrow) T[capacity] };

        if (!temp) {
            std::cerr << "ERROR: Memory allocation failed.";
            return;
        }

        if (m_array)
            moveTo(temp);

        delete[] m_array;
        m_array = temp;
        m_capacity = capacity;
    }
}

template <typename T>
void Array<T>::clear()
{
    delete[] m_array;

    m_array = nullptr;
    m_length = 0;
    m_capacity = 0;
}

template <typename T>
T& Array<T>::getFirst()
{
    return getFirst();
}

template <typename T>
T& Array<T>::getLast()
{
    return getLast();
}

template <typename T>
T& Array<T>::get(std::size_t index)
{
    return get();
}

template <typename T>
const T& Array<T>::getFirst() const
{
    assert(m_length > 0);
    return m_array[0];
}

template <typename T>
const T& Array<T>::getLast() const
{
    assert(m_length > 0);
    return m_array[m_length-1];
}

template <typename T>
const T& Array<T>::get(std::size_t index) const
{
    assert(index < m_length);
    return m_array[index];
}

template <typename T>
void Array<T>::set(std::size_t index, const T& value)
{
    assert(index < m_length);
    m_array[index] = value;
}

template <typename U>
std::ostream& operator<<(std::ostream& out, const Array<U>& arr)
{
    out << '[';
    auto sep { "" };

    for (std::size_t i{}; i < arr.m_length; ++i) {
        out << sep << *(arr.m_array+i);
        sep = ", ";
    }

    out << ']';
    return out;
}

template <typename T>
Array<T>& Array<T>::operator=(const Array<T>& arr)
{
    if (this != &arr) {
        clear();
        reserve(arr.m_capacity);

        for (std::size_t i{}; i < arr.m_length; ++i) {
            m_array[i] = arr[i];
        }

        m_length = arr.m_length;
    }

    return *this;
}

template <typename T>
Array<T>& Array<T>::operator=(Array<T>&& arr) noexcept
{
    if (this != &arr) {
        clear();

        m_array = arr.m_array;
        m_length = arr.m_length;
        m_capacity = arr.m_capacity;

        arr.m_array = nullptr;
        arr.m_length = 0;
        arr.m_capacity = 0;
    }

    return *this;
}

template <typename T>
Array<T>& Array<T>::operator=(std::initializer_list<T> list)
{
    // delete[] m_array;
    clear();

    m_capacity = list.size();
    m_length = list.size();
    m_array = new (std::nothrow) T[m_capacity];

    std::copy(list.begin(), list.end(), m_array);
    return *this;
}

template <typename T>
const T& Array<T>::operator[](std::size_t index) const
{
    return m_array[index];
}

template <typename T>
T& Array<T>::operator[](std::size_t index)
{
    return m_array[index];
}

template <typename T>
bool Array<T>::operator>(const Array<T>& arr) const
{
    return m_length > arr.m_length;
}

template <typename T>
bool Array<T>::operator<(const Array<T>& arr) const
{
    return m_length < arr.m_length;

}

template <typename T>
bool Array<T>::operator==(const Array<T>& arr) const
{
    if (m_length != arr.m_length) {
        return false;
    }

    for (std::size_t i{}; i < m_length; ++i) {
        if (m_array[i] != arr.m_array[i]) {
            return false;
        }
    }

    return true;
}

template <typename T>
bool Array<T>::operator!() const
{
    return m_array == nullptr;
}
