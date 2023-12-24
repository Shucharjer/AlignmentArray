#pragma once
#include "InstructionSet.h"
#include "Assembly.h"

namespace  Alignment
{
    /// <summary>
    /// �Ƿ���ʹ��xmm�Ĵ���
    /// </summary>
    ALIGNMENT_API extern bool vector_xmm_enable;
    /// <summary>
    /// �Ƿ���ʹ��ymm�Ĵ���
    /// </summary>
    ALIGNMENT_API extern bool vector_ymm_enable;
    /// <summary>
    /// �Ƿ���ʹ��zmm�Ĵ���
    /// </summary>
    ALIGNMENT_API extern bool vector_zmm_enable;
    /// <summary>
    /// �Ƿ���ʹ��������չ
    /// </summary>
    ALIGNMENT_API extern bool vector_enable;

    /// <summary>
    /// ��������
    /// </summary>
    ALIGNMENT_API typedef enum : size_t { Invalid = 0, Common = 16, Medium = 32, Large = 64 } AlignmentSize;
    /// <summary>
    /// ���������ڴ�
    /// </summary>
    /// <param name="size">�ڴ�Ĵ�С</param>
    /// <param name="alignment">�ڴ�Ķ������ȣ�Ĭ��Ϊ16�ֽ�</param>
    /// <returns></returns>
    ALIGNMENT_API void* Allocate(size_t size, AlignmentSize alignment = AlignmentSize::Common);
    /// <summary>
    /// �ͷŶ�����ڴ�
    /// </summary>
    /// <param name="ptr">ָ��</param>
    /// <param name="alignment">��������</param>
    ALIGNMENT_API void Free(void* ptr, AlignmentSize alignment);
    /// <summary>
    /// ���·��������ڴ�
    /// <para>�˴���Ϊ����ȷ��size��alignment����������û��ʹ������</para>
    /// </summary>
    /// <param name="ptr">ָ��</param>
    /// <param name="new_size">�µ��ڴ��С</param>
    /// <param name="new_alignment">�µ��ڴ��������</param>
    /// <param name="old_size">�ɵ��ڴ����</param>
    /// <param name="old_alignment">�ɵ��ڴ��������</param>
    ALIGNMENT_API void Reallocate(void*& ptr, size_t new_size, AlignmentSize new_alignment, size_t old_size, AlignmentSize old_alignment);
    /// <summary>
    /// �Ƿ񰴸����ķ�ʽ����
    /// </summary>
    /// <param name="ptr">ָ��</param>
    /// <param name="alignment">��������</param>
    /// <returns></returns>
    ALIGNMENT_API bool IsAligned(void* ptr, AlignmentSize alignment);
    /// <summary>
    /// ����
    /// </summary>
    /// <param name="dst">Ŀ�ĵ�ַ</param>
    /// <param name="src">Դ��ַ</param>
    /// <param name="size">��С</param>
    /// <returns></returns>
    ALIGNMENT_API bool Copy(void* dst, const void* src, size_t size);

    /// <summary>
    /// ���������
    /// </summary>
    /// <typeparam name="T">Ԫ������</typeparam>
    template <typename T>
    class alignas(32) Array {
    private:
        T* m_data;
        size_t m_size;
        size_t r_size;
        AlignmentSize m_alignment;
    public:
        /// <summary>
        /// ����������ʽ��Ҫʹ��initialize()���г�ʼ��
        /// </summary>
        Array();
        /// <summary>
        /// Array����
        /// <para>ʵ���ϻ����ݻ�ռ��alignment�����������ֽ�</para>
        /// </summary>
        /// <param name="size">��������Ĵ�С�����ֽڼƣ�</param>
        /// <param name="alignment">��������</param>
        Array(size_t size, AlignmentSize alignment = AlignmentSize::Common);
        ~Array();

        T& operator[](size_t index);

        Array(const Array& other);
        Array(Array&& other);
        Array& operator=(const Array& other);
        Array& operator=(Array&& other);

        /// <summary>
        /// ���ݴ�ŵ�λ��
        /// </summary>
        /// <returns></returns>
        T* data() { return m_data; }
        /// <summary>
        /// ���ݴ�ŵ�λ��
        /// </summary>
        /// <returns></returns>
        T* data() const { return m_data; }
        /// <summary>
        /// ����Ĵ�С
        /// </summary>
        /// <returns></returns>
        size_t size() { return m_size; }
        /// <summary>
        /// ����Ĵ�С
        /// </summary>
        /// <returns></returns>
        size_t size() const { return m_size; }
        /// <summary>
        /// ���ڴ�����ʵռ�õĴ�С
        /// </summary>
        /// <returns></returns>
        size_t realSize() { return r_size; }
        /// <summary>
        /// ���ڴ�����ʵռ�õĴ�С
        /// </summary>
        /// <returns></returns>
        size_t realSize() const { return r_size; }
        /// <summary>
        /// ��������
        /// </summary>
        /// <returns></returns>
        AlignmentSize alignment() { return m_alignment; }
        /// <summary>
        /// ��������
        /// </summary>
        /// <returns></returns>
        AlignmentSize alignment() const { return m_alignment; }

        /// <summary>
        /// ��ʼ��
        /// </summary>
        /// <param name="size">��������Ĵ�С�����ֽڼƣ�</param>
        /// <param name="alignment">��������</param>
        /// <returns></returns>
        bool initialize(size_t size, AlignmentSize alignment = AlignmentSize::Common);
        /// <summary>
        /// ��������С
        /// </summary>
        /// <param name="size">�µĴ�С</param>
        /// <param name="copy">�Ƿ񿽱�ԭֵ</param>
        /// <returns></returns>
        bool resize(size_t size, bool copy = true);
        /// <summary>
        /// ���������뷽ʽ
        /// </summary>
        /// <param name="alignment">�µĶ�������</param>
        /// <param name="copy">�Ƿ񿽱�ԭֵ</param>
        /// <returns></returns>
        bool realign(AlignmentSize alignment = AlignmentSize::Common, bool copy = true);
        /// <summary>
        /// �������ڵ�ֵȫ����Ϊval
        /// </summary>
        /// <param name="val"></param>
        /// <returns></returns>
        bool fill(T val);
        /// <summary>
        /// �������ڵ�ֵȫ����Ϊval
        /// </summary>
        /// <param name="val"></param>
        /// <returns></returns>
        bool fill(T& val);
        /// <summary>
        /// �����������Ϊ0
        /// <para>��Ϊ�᳢��ʹ���������÷�����ʱ���fill(0)��ܶ�</para>
        /// </summary>
        /// <returns></returns>
        bool zero();
        /// <summary>
        /// �ͷŵ������ڵ�����
        /// <para>�ͷź������Ҫ�ٴ�ʹ�øö�����Ҫ��ʹ��initialize()</para>
        /// </summary>
        void clear();
        /// <summary>
        /// �Ӷ�����ڴ濽������
        /// <para>�᳢��ʹ������</para>
        /// <para>�������Ĵ�С���������м�¼�Ĵ�С��ʱ��������������м�¼�Ĵ�С</para>
        /// </summary>
        /// <param name="src">Դ��ַ</param>
        /// <param name="size">��С</param>
        void copyFromAligned(void* src, size_t size);
        /// <summary>
        /// ������������������
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