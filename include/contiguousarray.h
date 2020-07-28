#pragma once
#include <optional>

template <typename T, size_t S>
class ContigousArray
{
public:
    ContigousArray() = default;
    ~ContigousArray() = default;
    ContigousArray(const ContigousArray& other)
    {
        for (int i = 0; i < S; ++i) m_array[i] = other.m_array[i];
    }

    ContigousArray(ContigousArray&& other)
    {
        for (int i = 0; i < S; ++i)
        {
            m_array[i] = other.m_array[i];
            other.m_array[i].reset();
        }
    }

    ContigousArray& operator=(const ContigousArray& other)
    {
        for (int i = 0; i < S; ++i) m_array[i] = other.m_array[i];
        return *this;
    }

    ContigousArray& operator=(ContigousArray&& other)
    {
        for (int i = 0; i < S; ++i)
        {
            m_array[i] = other.m_array[i];
            other.m_array[i].reset();
        }
        return *this;
    }

    std::optional<T>& operator[](size_t index) { return m_array[index]; }
    const std::optional<T>& at(size_t index) const { return m_array[index]; }

    void push(T val)
    {
        auto it = std::find(begin(), end(), val);
        if (it != end())
        {
            for (it; it != begin(); --it) *it = *(it - 1);
        }
        else
        {
            for (int i = S - 1; i > 0; --i)
                if (m_array[i - 1] != std::nullopt) m_array[i] = m_array[i - 1];
        }
        m_array[0] = val;
    }

    void remove(T val)
    {
        auto it = std::find(begin(), end(), val);
        if (it == end()) return;
        for (it; it != end(); ++it)
            if (it == back())
                *it = std::nullopt;
            else
                *it = *(it + 1);
    }

    bool contains(const T& val)
    {
        for (int i = 0; i < S; ++i)
            if (m_array[i] == val) return true;
        return false;
    }

    std::optional<T>* begin() { return &m_array[0]; }
    std::optional<T>* end() { return &m_array[S]; }
    const std::optional<T>* begin() const { return &m_array[0]; }
    const std::optional<T>* end() const { return &m_array[S]; }

    const std::optional<T>* cbegin() const { return &m_array[0]; }
    const std::optional<T>* cend() const { return &m_array[S]; }

    std::optional<T>* front() { return &m_array[0]; }
    std::optional<T>* back() { return &m_array[S - 1]; }

    constexpr size_t size() const { return S; }

private:
    std::optional<T> m_array[S];
};
