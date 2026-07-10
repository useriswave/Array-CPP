#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <functional>
#include <initializer_list>
#include <exception>

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
    void insert(T&& value, std::size_t index);
    void remove(std::size_t index);
    void bubbleSort(std::function<bool(const T& a, const T&b)> sortOrder);
    void selectionSort(std::function<bool(const T& a, const T& b)> sortOrder);
    void reverse();
    void reserve(std::size_t capacity);
    void clear();
    void set(std::size_t index, const T& value);

public:
    const T& get(std::size_t index) const;
    const T& getFirst() const;
    const T& getLast() const;
    T& getFirst();
    T& getLast() ;
    T& get(std::size_t index);
    std::size_t capacity() const { return m_capacity; }
    std::size_t length() const { return m_length; }

public:
    T* begin() const { return m_array; }
    T* end() const { return m_array + m_length; }

public:
    Array& operator=(const Array<T>& arr);
    Array& operator=(Array<T>&& arr) noexcept;
    void operator=(std::initializer_list<T> list);
    const T& operator[](std::size_t index) const noexcept;
    T& operator[](std::size_t index) noexcept;
    bool operator>(const Array<T>& arr) const noexcept;
    bool operator<(const Array<T>& arr) const noexcept;
    bool operator==(const Array<T>& arr) const noexcept;
    explicit operator bool() const noexcept;

    template <typename U>
    friend std::ostream& operator<<(std::ostream& out, const Array<U>& arr);

private:
    void moveTo(std::reference_wrapper<T*> other);
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
    T* temp { new T[tempCap] };

    if (m_array) {
        moveTo(temp);
    }

    delete[] m_array;
    m_array = temp;
    m_capacity = tempCap;
}

template <typename T>
void Array<T>::moveTo(std::reference_wrapper<T*> other)
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
    if (index > m_length) {
        throw std::out_of_range{ "ERROR: Index out of bounds at insertion." };
    }

    if (m_length >= m_capacity) {
        resize();
    }

    for (std::size_t i{ m_length }; i > index; --i) {
        m_array[i] = std::move(m_array[i-1]);
    }

    m_array[index] = value;
    ++m_length;
}

template <typename T>
void Array<T>::insert(T&& value, std::size_t index)
{
    if (index > m_length) {
        throw std::out_of_range{ "ERROR: Index out of bounds at insertion." };
    }

    if (m_length >= m_capacity) {
        resize();
    }

    for (std::size_t i{ m_length }; i > index; --i) {
        m_array[i] = std::move(m_array[i-1]);
    }

    m_array[index] = std::move(value);
    ++m_length;
}

template <typename T>
void Array<T>::remove(const std::size_t index)
{
    if (index >= m_length) {
        throw std::out_of_range{ "ERROR: Index out of bounds at insertion." };
    }

    for (std::size_t i{index}; i < m_length-1; ++i) {
        m_array[i] = std::move(m_array[i+1]);   // this is amazing
    }

    --m_length;
}

template <typename T>
void Array<T>::bubbleSort(std::function<bool(const T& a, const T& b)> sortOrder)
{
    if (m_length == 0) {
        throw std::runtime_error{ "ERROR: Bubble sort failed. Array contains no elements. "};
    }

    for (std::size_t i{}; i < m_length - 1; ++i) {
        for (std::size_t j{}; j < m_length - 1 - i; ++j) {
            if (sortOrder(m_array[j], m_array[j+1])) {
                std::swap(m_array[j], m_array[j+1]);
            }
        }
    }
}

template <typename T>
void Array<T>::selectionSort(std::function<bool(const T& a, const T& b)> sortOrder)
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
        T* temp { new T[capacity] };

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
    if (m_length < 0) {
        throw std::runtime_error { "ERROR: getFirst() failed. Array contains no elements." };
    }

    return m_array[0];
}

template <typename T>
const T& Array<T>::getLast() const
{
    if (m_length < 0) {
        throw std::runtime_error { "ERROR: getLast() failed. Array contains no elements." };
    }

    return m_array[m_length-1];
}

template <typename T>
const T& Array<T>::get(std::size_t index) const
{
    if (index > m_length) {
        throw std::out_of_range{ "ERROR: get() failed. Index out of bounds" };
    }

    return m_array[index];
}

template <typename T>
void Array<T>::set(std::size_t index, const T& value)
{
    if (index >= m_length) {
        throw std::out_of_range{ "ERROR: set() failed. Index out of bounds" };
    }

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
void Array<T>::operator=(std::initializer_list<T> list)
{
    clear();

    m_capacity = list.size();
    m_length = list.size();
    m_array = new T[m_capacity];

    std::copy(list.begin(), list.end(), m_array);
}

template <typename T>
const T& Array<T>::operator[](std::size_t index) const noexcept
{
    return m_array[index];
}

template <typename T>
T& Array<T>::operator[](std::size_t index) noexcept
{
    return m_array[index];
}

template <typename T>
bool Array<T>::operator>(const Array<T>& arr) const noexcept
{
    return m_length > arr.m_length;
}

template <typename T>
bool Array<T>::operator<(const Array<T>& arr) const noexcept
{
    return m_length < arr.m_length;
}

template <typename T>
bool Array<T>::operator==(const Array<T>& arr) const noexcept
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
Array<T>::operator bool() const noexcept
{
    return m_array != nullptr;
}

#endif
