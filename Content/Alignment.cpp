#include "Alignment.h"

ALIGNMENT_API bool Alignment::vector_xmm_enable = false;
ALIGNMENT_API bool Alignment::vector_ymm_enable = false;
ALIGNMENT_API bool Alignment::vector_zmm_enable = false;
ALIGNMENT_API bool Alignment::vector_enable = false;

void* Alignment::Allocate(size_t size, AlignmentSize alignment) {
    return operator new(size, std::align_val_t(alignment));
}

void Alignment::Free(void* ptr, AlignmentSize alignment) {
    operator delete(ptr, std::align_val_t(alignment));
}

void Alignment::Reallocate(void*& ptr, size_t new_size, AlignmentSize new_alignment, size_t old_size, AlignmentSize old_alignment) {
    void* new_ptr = operator new(new_size, std::align_val_t(new_alignment));
    
    // 不能确保size是alignment的整数倍，使用memcpy_s会更安全一点
    if (old_size < new_size)
        memcpy_s(new_ptr, old_size, ptr, old_size);
    else
        memcpy_s(new_ptr, new_size, ptr, new_size);
    operator delete(ptr, std::align_val_t(old_alignment));
    ptr = new_ptr;
}

bool Alignment::IsAligned(void* ptr, AlignmentSize alignment) {
    if (ptr == nullptr) return false;

    return !(((uintptr_t)ptr) % alignment);
}

bool Alignment::Copy(void* dst, const void* src, size_t size) {
    if (dst == src) return false;
    memcpy_s(dst, size, src, size);
    return true;
}

template <typename T> Alignment::Array<T>::Array() {
    m_data = nullptr;
    m_size = 0;
    r_size = 0;
    m_alignment = AlignmentSize::Invalid;
}
template <typename T> Alignment::Array<T>::Array(size_t size, AlignmentSize alignment) {
    m_size = size;
    m_alignment = alignment;
    r_size = size % alignment ? (size / alignment) * alignment + alignment : size;
    m_data = (T*)Alignment::Allocate(r_size, alignment);
}
template <typename T> Alignment::Array<T>::~Array() {
    if (m_data != nullptr) {
        Free(m_data, m_alignment);
        m_data = nullptr;
    }
}
template <typename T> T& Alignment::Array<T>::operator[](size_t index)
{
    return m_data[index];
}
template <typename T> bool Alignment::Array<T>::initialize(size_t size, AlignmentSize alignment) {
    if (m_data != nullptr) return false;

    // allocate
    m_size = size;
    m_alignment = alignment;
    r_size = size % alignment ? (size / alignment) * alignment + alignment : size;
    m_data = (T*)Alignment::Allocate(r_size, alignment);

    if (m_data == nullptr) return false;
    return true;
}
template <typename T> bool Alignment::Array<T>::resize(size_t size, bool copy) {
    if (m_data == nullptr || size < m_size) return false;

    r_size = size % m_alignment ? (size / m_alignment) * m_alignment + m_alignment : size;

    T* new_ptr = (T*)Alignment::Allocate(r_size, m_alignment);
    if (new_ptr == nullptr) return false;

    if (copy) {
        if (vector_zmm_enable && m_alignment >= AlignmentSize::Large) {
            Assembly::CopyZ(new_ptr, m_data, r_size);
        }
        else if (vector_ymm_enable && m_alignment >= AlignmentSize::Medium) {
            Assembly::CopyY(new_ptr, m_data, r_size);
        }
        else if (vector_xmm_enable && m_alignment >= AlignmentSize::Common) {
            Assembly::CopyX(new_ptr, m_data, r_size);
        }
        else {
            memcpy_s(new_ptr, m_size, m_data, m_size);
        }
    }

    m_size = size;
    Alignment::Free(m_data, m_alignment);
    m_data = new_ptr;

    return true;
}
template <typename T> bool Alignment::Array<T>::realign(AlignmentSize alignment, bool copy) {
    if (m_data == nullptr) return false;

    r_size = m_size % alignment ? (m_size / alignment) * alignment + alignment : m_size;

    T* new_ptr = (T*)Alignment::Allocate(r_size, alignment);
    if (new_ptr == nullptr) return false;

    if (copy) {
        if (vector_zmm_enable && m_alignment >= AlignmentSize::Large && alignment >= AlignmentSize::Large) {
            Assembly::CopyZ(new_ptr, m_data, r_size);
        }
        else if (vector_ymm_enable && m_alignment >= AlignmentSize::Medium && alignment >= AlignmentSize::Medium) {
            Assembly::CopyY(new_ptr, m_data, r_size);
        }
        else if (vector_xmm_enable && m_alignment >= AlignmentSize::Common && alignment >= AlignmentSize::Common) {
            Assembly::CopyX(new_ptr, m_data, r_size);
        }
        else {
            memcpy_s(new_ptr, m_size, m_data, m_size);
        }
    }

    Alignment::Free(m_data, m_alignment);
    m_alignment = alignment;
    m_data = new_ptr;

    return true;
}
template <typename T> bool Alignment::Array<T>::fill(T val) {
    if (m_data == nullptr) return false;

    size_t steps = m_size / sizeof(T);
    for (size_t i = 0; i < steps; i++) {
        m_data[i] = val;
    }
    return true;
}
template <typename T> bool Alignment::Array<T>::fill(T& val) {
    if (m_data == nullptr) return false;

    size_t steps = m_size / sizeof(T);
    for (size_t i = 0; i < steps; i++) {
        m_data[i] = val;
    }
    return true;
}
template <typename T> bool Alignment::Array<T>::zero() {
    if (m_data == nullptr) return false;

    if (vector_zmm_enable && m_alignment >= AlignmentSize::Large) {
        Assembly::ZeroZ(m_data, r_size);
    }
    else if (vector_ymm_enable && m_alignment >= AlignmentSize::Medium) {
        Assembly::ZeroY(m_data, r_size);
    }
    else if (vector_xmm_enable && m_alignment >= AlignmentSize::Common) {
        Assembly::ZeroX(m_data, r_size);
    }
    else {
        memset(m_data, 0, m_size);
    }

    return true;
}
template <typename T> void Alignment::Array<T>::clear() {
    if (m_data != nullptr) {
        Free(m_data, m_alignment);
        m_data = nullptr;
    }
    m_size = 0;
    r_size = 0;
    m_alignment = AlignmentSize::Invalid;
}

template <typename T> Alignment::Array<T>::Array(const Array& other) {
    m_size = other.m_size;
    r_size = other.r_size;
    m_alignment = other.m_alignment;
    m_data = (T*)Alignment::Allocate(r_size, m_alignment);

    // copy
    if (vector_zmm_enable && m_alignment >= AlignmentSize::Large) {
        Assembly::CopyZ(m_data, other.m_data, r_size);
    }
    else if (vector_ymm_enable && m_alignment >= AlignmentSize::Medium) {
        Assembly::CopyY(m_data, other.m_data, r_size);
    }
    else if (vector_xmm_enable && m_alignment >= AlignmentSize::Common) {
        Assembly::CopyX(m_data, other.m_data, r_size);
    }
    else {
        memcpy_s(m_data, m_size, other.m_data, m_size);
    }
}
template <typename T> Alignment::Array<T>::Array(Array&& other) {
    m_size = other.m_size;
    r_size = other.r_size;
    m_alignment = other.m_alignment;
    m_data = other.m_data;
    other.m_data = nullptr;
}
template <typename T> Alignment::Array<T>& Alignment::Array<T>::operator=(const Array& other) {
    if (m_data != nullptr) Free(m_data, m_alignment);
    m_size = other.m_size;
    r_size = other.r_size;
    m_alignment = other.m_alignment;
    m_data = (T*)Alignment::Allocate(r_size, m_alignment);

    // copy
    if (vector_zmm_enable && m_alignment >= AlignmentSize::Large) {
        Assembly::CopyZ(m_data, other.m_data, r_size);
    }
    else if (vector_ymm_enable && m_alignment >= AlignmentSize::Medium) {
        Assembly::CopyY(m_data, other.m_data, r_size);
    }
    else if (vector_xmm_enable && m_alignment >= AlignmentSize::Common) {
        Assembly::CopyX(m_data, other.m_data, r_size);
    }
    else {
        memcpy_s(m_data, m_size, other.m_data, m_size);
    }
    return *this;
}
template <typename T> Alignment::Array<T>& Alignment::Array<T>::operator=(Array&& other) {
    if (m_data != nullptr) Alignment::Free(m_data, m_alignment);
    m_size = other.m_size;
    r_size = other.r_size;
    m_alignment = other.m_alignment;
    m_data = other.m_data;
    other.m_data = nullptr;
    return *this;
}
template <typename T> void Alignment::Array<T>::copyFromAligned(void* src, size_t size) {
    size_t minSize = size < m_size ? size : m_size;

    if (vector_zmm_enable && m_alignment >= AlignmentSize::Large && IsAligned(src, AlignmentSize::Large)) {
        Assembly::CopyZ(m_data, src, minSize);
    }
    else if (vector_ymm_enable && m_alignment >= AlignmentSize::Medium && IsAligned(src, AlignmentSize::Medium)) {
        Assembly::CopyY(m_data, src, minSize);
    }
    else if (vector_xmm_enable && m_alignment >= AlignmentSize::Common && IsAligned(src, AlignmentSize::Common)) {
        Assembly::CopyX(m_data, src, minSize);
    }
    else {
        memcpy_s(m_data, minSize, src, minSize);
    }
}
template <typename T> void Alignment::Array<T>::swap(Array& other) {
    if (vector_ymm_enable) {
        Assembly::SwapY(this, &other);
    }
    else if (vector_xmm_enable) {
        Assembly::SwapX(this, &other);
        Assembly::SwapX(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(this) + 16), reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(&other) + 16));
    }
    else {
        size_t temp = reinterpret_cast<size_t>(m_data);
        m_data = other.m_data;
        other.m_data = reinterpret_cast<T*>(temp);

        temp = static_cast<size_t>(m_alignment);
        m_alignment = other.m_alignment;
        other.m_alignment = static_cast<Alignment::AlignmentSize>(temp);

        temp = m_size;
        m_size = other.m_size;
        other.m_size = temp;

        temp = r_size;
        r_size = other.r_size;
        other.r_size = temp;
    }
}