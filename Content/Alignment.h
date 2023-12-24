#pragma once
#include "InstructionSet.h"
#include "Assembly.h"

namespace  Alignment
{
    /// <summary>
    /// 是否能使用xmm寄存器
    /// </summary>
    ALIGNMENT_API extern bool vector_xmm_enable;
    /// <summary>
    /// 是否能使用ymm寄存器
    /// </summary>
    ALIGNMENT_API extern bool vector_ymm_enable;
    /// <summary>
    /// 是否能使用zmm寄存器
    /// </summary>
    ALIGNMENT_API extern bool vector_zmm_enable;
    /// <summary>
    /// 是否能使用向量扩展
    /// </summary>
    ALIGNMENT_API extern bool vector_enable;

    /// <summary>
    /// 对齐粒度
    /// </summary>
    ALIGNMENT_API typedef enum : size_t { Invalid = 0, Common = 16, Medium = 32, Large = 64 } AlignmentSize;
    /// <summary>
    /// 申请对齐的内存
    /// </summary>
    /// <param name="size">内存的大小</param>
    /// <param name="alignment">内存的对齐粒度，默认为16字节</param>
    /// <returns></returns>
    ALIGNMENT_API void* Allocate(size_t size, AlignmentSize alignment = AlignmentSize::Common);
    /// <summary>
    /// 释放对齐的内存
    /// </summary>
    /// <param name="ptr">指针</param>
    /// <param name="alignment">对齐粒度</param>
    ALIGNMENT_API void Free(void* ptr, AlignmentSize alignment);
    /// <summary>
    /// 重新分配对齐的内存
    /// <para>此处因为不能确保size是alignment的整数倍而没有使用向量</para>
    /// </summary>
    /// <param name="ptr">指针</param>
    /// <param name="new_size">新的内存大小</param>
    /// <param name="new_alignment">新的内存对齐粒度</param>
    /// <param name="old_size">旧的内存带下</param>
    /// <param name="old_alignment">旧的内存对齐粒度</param>
    ALIGNMENT_API void Reallocate(void*& ptr, size_t new_size, AlignmentSize new_alignment, size_t old_size, AlignmentSize old_alignment);
    /// <summary>
    /// 是否按给定的方式对齐
    /// </summary>
    /// <param name="ptr">指针</param>
    /// <param name="alignment">对齐粒度</param>
    /// <returns></returns>
    ALIGNMENT_API bool IsAligned(void* ptr, AlignmentSize alignment);
    /// <summary>
    /// 拷贝
    /// </summary>
    /// <param name="dst">目的地址</param>
    /// <param name="src">源地址</param>
    /// <param name="size">大小</param>
    /// <returns></returns>
    ALIGNMENT_API bool Copy(void* dst, const void* src, size_t size);

    /// <summary>
    /// 对齐的数组
    /// </summary>
    /// <typeparam name="T">元素类型</typeparam>
    template <typename T>
    class alignas(32) Array {
    private:
        T* m_data;
        size_t m_size;
        size_t r_size;
        AlignmentSize m_alignment;
    public:
        /// <summary>
        /// 这种声明方式需要使用initialize()进行初始化
        /// </summary>
        Array();
        /// <summary>
        /// Array类型
        /// <para>实际上会数据会占用alignment的整数倍的字节</para>
        /// </summary>
        /// <param name="size">整个数组的大小（按字节计）</param>
        /// <param name="alignment">对齐粒度</param>
        Array(size_t size, AlignmentSize alignment = AlignmentSize::Common);
        ~Array();

        T& operator[](size_t index);

        Array(const Array& other);
        Array(Array&& other);
        Array& operator=(const Array& other);
        Array& operator=(Array&& other);

        /// <summary>
        /// 数据存放的位置
        /// </summary>
        /// <returns></returns>
        T* data() { return m_data; }
        /// <summary>
        /// 数据存放的位置
        /// </summary>
        /// <returns></returns>
        T* data() const { return m_data; }
        /// <summary>
        /// 申请的大小
        /// </summary>
        /// <returns></returns>
        size_t size() { return m_size; }
        /// <summary>
        /// 申请的大小
        /// </summary>
        /// <returns></returns>
        size_t size() const { return m_size; }
        /// <summary>
        /// 在内存中真实占用的大小
        /// </summary>
        /// <returns></returns>
        size_t realSize() { return r_size; }
        /// <summary>
        /// 在内存中真实占用的大小
        /// </summary>
        /// <returns></returns>
        size_t realSize() const { return r_size; }
        /// <summary>
        /// 对齐粒度
        /// </summary>
        /// <returns></returns>
        AlignmentSize alignment() { return m_alignment; }
        /// <summary>
        /// 对齐粒度
        /// </summary>
        /// <returns></returns>
        AlignmentSize alignment() const { return m_alignment; }

        /// <summary>
        /// 初始化
        /// </summary>
        /// <param name="size">整个数组的大小（按字节计）</param>
        /// <param name="alignment">对齐粒度</param>
        /// <returns></returns>
        bool initialize(size_t size, AlignmentSize alignment = AlignmentSize::Common);
        /// <summary>
        /// 变更数组大小
        /// </summary>
        /// <param name="size">新的大小</param>
        /// <param name="copy">是否拷贝原值</param>
        /// <returns></returns>
        bool resize(size_t size, bool copy = true);
        /// <summary>
        /// 变更数组对齐方式
        /// </summary>
        /// <param name="alignment">新的对齐粒度</param>
        /// <param name="copy">是否拷贝原值</param>
        /// <returns></returns>
        bool realign(AlignmentSize alignment = AlignmentSize::Common, bool copy = true);
        /// <summary>
        /// 将数组内的值全部设为val
        /// </summary>
        /// <param name="val"></param>
        /// <returns></returns>
        bool fill(T val);
        /// <summary>
        /// 将数组内的值全部设为val
        /// </summary>
        /// <param name="val"></param>
        /// <returns></returns>
        bool fill(T& val);
        /// <summary>
        /// 将数组内填充为0
        /// <para>因为会尝试使用向量，该方法有时会比fill(0)快很多</para>
        /// </summary>
        /// <returns></returns>
        bool zero();
        /// <summary>
        /// 释放掉数组内的内容
        /// <para>释放后如果想要再次使用该对象，需要先使用initialize()</para>
        /// </summary>
        void clear();
        /// <summary>
        /// 从对齐的内存拷贝数据
        /// <para>会尝试使用向量</para>
        /// <para>当给定的大小比它属性中记录的大小大时，会采用它属性中记录的大小</para>
        /// </summary>
        /// <param name="src">源地址</param>
        /// <param name="size">大小</param>
        void copyFromAligned(void* src, size_t size);
        /// <summary>
        /// 交换两个容器的内容
        /// </summary>
        /// <param name="other"></param>
        void swap(Array& other);
    };

    template class ALIGNMENT_API Array<bool>;
    template class ALIGNMENT_API Array<char>;
    template class ALIGNMENT_API Array<unsigned char>;
    template class ALIGNMENT_API Array<int>;
    template class ALIGNMENT_API Array<unsigned int>;
    template class ALIGNMENT_API Array<short int>;
    template class ALIGNMENT_API Array<unsigned short int>;
    template class ALIGNMENT_API Array<long int>;
    template class ALIGNMENT_API Array<unsigned long int>;
    template class ALIGNMENT_API Array<long long>;
    template class ALIGNMENT_API Array<unsigned long long>;
    template class ALIGNMENT_API Array<float>;
    template class ALIGNMENT_API Array<double>;
    template class ALIGNMENT_API Array<long double>;
    template class ALIGNMENT_API Array<size_t>;
}