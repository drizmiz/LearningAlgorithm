
#pragma once

#ifndef SORT_H
#define SORT_H

#include <exception>
#include <future>
#include <iosfwd>
#include "BagQueueAndStack.h"

#define _MY_DEBUG_
#undef _MY_DEBUG_

#define _NO_INLINE __declspec(noinline)

template<typename _Ty>
class Sort
{
private:
	typedef typename std::vector<_Ty>::iterator iterator;
	iterator tobeSorted;
	typedef unsigned uint;
	uint N;
	_NO_INLINE void SelectionSort()
	{
		for (unsigned i = 0; i < N; i++)								// 对每一个i（从0开始）
		{
			unsigned minIndex = i;
			for (unsigned j = i; j < N; j++)
				if (tobeSorted[j] < tobeSorted[minIndex])
					minIndex = j;										// 寻找i之后所有项的最小值所在位置
			std::swap(tobeSorted[i], tobeSorted[minIndex]);				// 将其与第i项交换
		}
	}
	_NO_INLINE void InsertionSort_Slow()
	{
		for (unsigned i = 1; i < N; i++)											// 对每一个i（从1开始）
			for (unsigned j = i; j > 0 && tobeSorted[j - 1] > tobeSorted[j]; j--)	// 假定i前面的项已经排好序了，向左查找以寻找第i项应该在的位置
				std::swap(tobeSorted[j], tobeSorted[j - 1]);						// 把第i项依次交换过去
	}
	_NO_INLINE void InsertionSort()
	{
#ifdef _MY_DEBUG_
		std::cout << std::endl << "INSERTION PART" << std::endl;
		outl();
#endif
		for (unsigned i = 1; i < N; i++)							// 对每一个i（从1开始）
		{
			unsigned j = i;
			_Ty tmp = tobeSorted[i];								// 保存第i项
			for (; j > 0 && tobeSorted[j - 1] > tmp; j--)			// 假定i前面的项已经排好序了，向左查找以寻找第i项应该在的位置
				tobeSorted[j] = tobeSorted[j - 1];					// 之间的项顺次向右移动
			tobeSorted[j] = tmp;									// 把第i项插入到应该在的位置
		}
#ifdef _MY_DEBUG_
		outl();
		std::cout << "INSERTION END" << std::endl << std::endl;
#endif
	}
	_NO_INLINE void InsertionSort_NoBdry()
	{
		unsigned minIndex = 0;
		for (unsigned j = 0; j < N; j++)
			if (tobeSorted[j] < tobeSorted[minIndex])
				minIndex = j;										// 寻找所有项的最小值所在位置
		std::swap(tobeSorted[0], tobeSorted[minIndex]);				// 将其交换到第一项，以避免边界检查
		for (unsigned i = 1; i < N; i++)							// 对每一个i（从1开始）
		{
			unsigned j = i;
			_Ty tmp = tobeSorted[i];								// 保存第i项
			for (; tobeSorted[j - 1] > tmp; j--)					// 假定i前面的项已经排好序了，向左查找以寻找第i项应该在的位置
				tobeSorted[j] = tobeSorted[j - 1];					// 之间的项顺次向右移动
			tobeSorted[j] = tmp;									// 把第i项插入到应该在的位置
		}
	}
	_NO_INLINE void ShellSort()
	{
		unsigned h = 1;
		for (; h < N / 3; h = 3 * h + 1);								// 递增序列：h_{n+1} = 3 * h_n + 1
		for (; h >= 1; h /= 3)
		{
			for (unsigned i = h; i < N; i++)							// 对每一个i（从1开始）
			{
				unsigned j = i;
				_Ty tmp = tobeSorted[i];								// 保存第i项
				for (; j >= h && tobeSorted[j - h] > tmp; j -= h)		// 假定i前面的项已经排好序了，向左查找以寻找第i项应该在的位置
					tobeSorted[j] = tobeSorted[j - h];					// 之间的项顺次向右移动
				tobeSorted[j] = tmp;									// 把第i项插入到应该在的位置
			}
		}
	}
	// 假装是原地归并
	_NO_INLINE void merge(iterator begin, iterator separator, iterator end)		// end应该指向最后一个元素之后
	{
		//
		if (*(separator - 1) <= *separator)return;								// 若已排好序，则不必再排
		//
		unsigned cnt = end - begin;
		std::vector<_Ty> auxvec;
		auxvec.reserve(cnt);
		iterator aux = auxvec.begin();
		for (unsigned i = 0; i < cnt; i++)aux[i] = begin[i];			// 创建辅助数列aux（与原数列相同）
		iterator left = aux;
		iterator right = aux + (separator - begin);									// left right 分别指向两边数组的起始
		for (unsigned i = 0; i < cnt; i++)
		{
			if (left == aux + (separator - begin))								// 若左边取完了，则只能取右边
				begin[i] = *(right++);
			else if (right == aux + cnt)								// 若右边取完了，则只能取左边
				begin[i] = *(left++);
			else if (*left <= *right)											// 左边比右边小，则取左边
				begin[i] = *(left++);
			else																// 否则取右边
				begin[i] = *(right++);
		}
	}
	_NO_INLINE void mergesort_rec(iterator begin, iterator end)
	{
		if (end == begin + 1)return;
		if (end <= begin + 10)
		{
			Sort<_Ty> s(begin, end - begin);
			s.InsertionSort();
			return;
		}
		iterator sep = begin + (end - begin) / 2;
		mergesort_rec(begin, sep);
		mergesort_rec(sep, end);
		merge(begin, sep, end);
	}
	_NO_INLINE void MergeSort_T2B()
	{
		mergesort_rec(tobeSorted, tobeSorted + N);
	}
	_NO_INLINE void MergeSort_B2T()
	{
		const unsigned ini = 8;
		for (iterator place = tobeSorted; place < tobeSorted + N; place += ini)
		{
			Sort<_Ty> s(place, std::min(ini, static_cast<unsigned>((tobeSorted + N) - place)));
			s.InsertionSort();
		}
		for (unsigned size = 8; size < N; size *= 2)
			for (iterator place = tobeSorted; place < tobeSorted + N - size; place += size * 2)
				merge(place, place + size, std::min(place + size * 2, tobeSorted + N));
	}
#ifdef __OldQS
	_NO_INLINE iterator partition(iterator begin, iterator end)
	{
		//_Ty cut = *begin;
		iterator left = begin - 1, * right = end;
		iterator _cut = begin + rand() % (end - begin);
		assert(begin < end);
		bool flag = false;

		if (end - begin >= 9)
		{
			iterator _cut1, * _cut2;
			//do {
			_cut1 = begin + rand() % (end - begin);
			//} while (_cut1 != _cut);
			//do {
			_cut2 = begin + rand() % (end - begin);
			//} while ((_cut2 != _cut1) && (_cut2 != _cut));
			if (*_cut != *_cut2 && *_cut != *_cut1 && *_cut1 != *_cut2)
			{
				if (((*_cut < *_cut1) && (*_cut1 < *_cut2)) || ((*_cut > * _cut1) && (*_cut1 > * _cut2)))
					_cut = _cut1;
				if (((*_cut < *_cut2) && (*_cut2 < *_cut1)) || ((*_cut > * _cut2) && (*_cut2 > * _cut1)))
					_cut = _cut2;
				flag = true;
			}
		}

		_Ty cut = *_cut;
		/*
		if (!flag)
		{
			for (; *(++left) <= cut; )
				if (left == end - 1)
				{
					std::swap(*_cut, *left);
					return left;
				}
			left = begin - 1;
			for (; *(--right) >= cut; )
				if (right == begin)
				{
					std::swap(*_cut, *right);
					return right;
				}
			right = end;
		}
		*/
		for (;;)
		{
			for (; *(++left) <= cut && left != end - 1;);
			for (; *(--right) >= cut && right != begin;);
			if (left > right)break;
			std::swap(*left, *right);
			//outl();
		}
		iterator ret = _cut;
		if (_cut >= left)
			ret = left;
		if (_cut <= right)
			ret = right;
		std::swap(*_cut, *ret);
		return ret;
	}
#endif
	void Sort3(iterator begin, iterator mid, iterator end)
	{
		if (*mid < *begin)
			std::swap(*mid, *begin);
		if (*end < *mid)
		{
			std::swap(*end, *mid);
			if (*mid < *begin)
				std::swap(*mid, *begin);
		}
	}
	iterator guessMedian(iterator begin, iterator end)							// 会把中位数放在begin和end的中间
	{
		unsigned cnt = end - begin;
		iterator mid = begin + (end - begin) / 2;
		if (cnt > 40)
		{
			unsigned step = (cnt + 1) / 8;
			unsigned twostep = step * 2;
			Sort3(begin, begin + step, begin + twostep);
			Sort3(mid - step, mid, mid + step);
			Sort3(end - twostep, end - step, end);
			Sort3(begin + step, mid, end - step);
		}
		else
			Sort3(begin, mid, end);
		return mid;
	}
	_NO_INLINE iterator partition(iterator begin, iterator end)
	{
		typedef unsigned uint;
		iterator left = begin;
		iterator right = end;
		iterator _cut = begin;										// 第一项是 cut
		_Ty cut = *_cut;
		for (;;)
		{
			for (; *(++left) < cut && left != end - 1;);		// 从第二项开始依次向右寻找大于等于cut的元素，left指向它
			for (; cut < *(--right););							// 从最后一项开始依次向左寻找小于等于cut的元素，right指向它，且right!=begin一定成立
			if (left >= right)break;							// 若left超过了right，则不必再找
			std::swap(*left, *right);							// 将较大的元素换到右边
		}
		std::swap(*_cut, *right);								// 搜索结束后，理论上left=right+1，且right及其左侧都小于等于cut，把cut和right所在位置交换即可
																// 除非搜索结束的时候恰好left=right，此时*left=*right=cut，交换也无伤大雅
		return right;
	}
	_NO_INLINE iterator partition_with_guess(iterator begin, iterator end)
	{
		iterator left = begin - 1;
		iterator right = end;
		iterator _cut = guessMedian(begin, end - 1);				// 猜测中位数作为 cut，还提供“哨兵”作用
		_Ty cut = *_cut;
		for (;;)
		{
			do {
				for (; *(++left) < cut;);						// 从第一项开始依次向右寻找大于等于cut的元素，left指向它
			} while (left == _cut);
			do {
				for (; cut < *(--right););						// 从最后一项开始依次向左寻找小于等于cut的元素，right指向它
				if (left >= right) break;						// 防止由于恰碰见_cut导致right跑到left-2
			} while (right == _cut);
			if (left >= right) break;							// 若left超过了right，则不必再找
			std::swap(*left, *right);							// 将较大的元素换到右边
		}
		iterator ret;
		if (_cut <= right)
			ret = right;
		else
			ret = left;
		std::swap(*_cut, *ret);									// 搜索结束后，理论上left=right+1，把cut换到适当位置即可
																// 除非搜索结束的时候恰好left=right，此时*left=*right=cut，交换也无伤大雅
		return ret;
	}
	_NO_INLINE std::pair<iterator, iterator> partition_3way(iterator begin, iterator end)
	{
		iterator lessptr = begin;									// 指向小于cut的区域的右端+1，它及它的右侧是等于cut的部分
		iterator i = begin + 1;										// i是扫描指针
		iterator greaterptr = end - 1;								// 指向大于cut的区域的左端-1
		_Ty cut = *begin;										// 第一项是 cut
		for (; i <= greaterptr;)
		{
			if (*i < cut)										// 如果对应的数小于切分，则可以将其与*lessptr交换来换到左区；维护指针lessptr；右移i来扫描下一个数
				std::swap(*(lessptr++), *(i++));
			else if (*i > cut)									// 如果对应的数大于切分，则可以将其与*greaterptr交换来换到右区；维护指针greaterptr；不右移i，这是由于*greaterptr是未定的，仍需要检查
				std::swap(*i, *(greaterptr--));
			else i++;											// 如果对应的数等于切分，则右移i来扫描下一个数
		}
		return std::make_pair(lessptr, greaterptr + 1);			// 返回等于cut的区域的begin和end（尾后）
	}
	_NO_INLINE std::pair<iterator, iterator> partition_3way_with_guess(iterator begin, iterator end)
	{
		iterator lessptr = begin;									// 指向小于cut的区域的右端+1，它及它的右侧是等于cut的部分
		iterator i = begin + 1;										// i是扫描指针
		iterator greaterptr = end - 1;								// 指向大于cut的区域的左端-1
		iterator _cut = guessMedian(begin, end - 1);
		_Ty cut = *_cut;
		std::swap(*lessptr, *_cut);								// 保证lessptr所指等于cut
		for (; i <= greaterptr;)
		{
			if (*i < cut)										// 如果对应的数小于切分，则可以将其与*lessptr交换来换到左区；维护指针lessptr；右移i来扫描下一个数
				std::swap(*(lessptr++), *(i++));
			else if (*i > cut)									// 如果对应的数大于切分，则可以将其与*greaterptr交换来换到右区；维护指针greaterptr；不右移i，这是由于*greaterptr是未定的，仍需要检查
				std::swap(*i, *(greaterptr--));
			else i++;											// 如果对应的数等于切分，则右移i来扫描下一个数
			//outl();
		}
		//outl();
		return std::make_pair(lessptr, greaterptr + 1);			// 返回等于cut的区域的begin和end（尾后）
	}
	_NO_INLINE std::pair<iterator, iterator> partition_3way_with_guess_balanced(iterator begin, iterator end)
		// similar to the 2way version rather than the none-balanced version
	{
		//#define dbout() debugoutl(begin,end)
		// [begin - =cut - ][lessptr - <cut - ][left - unsorted - ][right - >cut - ][greaterptr - =cut - ]end
		// [begin - =cut - ][lessptr - <cut - right][left - >cut - ][greaterptr - =cut - ]end
		/*
		iterator lessptr = begin;									// 指向小于cut的区域的左端
		iterator left = begin;
		iterator right = end;										// left,j是扫描指针，向内扫描
		iterator greaterptr = end;									// 指向大于cut的区域的右端
		iterator _cut = guessMedian(begin, end - 1);
		_Ty cut = *_cut;
		std::swap(*lessptr, *_cut);								// 保证lessptr所指等于cut
		/*
		for (; left <= right;)
		{
			if (*left < cut)
				left++;
			else if (*left > cut)
				std::swap(*left, *(right--));
			else
				std::swap(*(lessptr++), *(left++));
		}
		*
		for (;;)
		{
		leftpart:
			for (; *(left++) < cut;);							// 从第一项开始依次向右寻找大于等于cut的元素，left指向它
			if (*left == cut)
			{
				std::swap(*left, *(lessptr++));
				goto leftpart;
			}
		rightpart:
			for (; cut < *(--right););							// 从最后一项开始依次向左寻找小于等于cut的元素，right指向它
			if (*right == cut)
			{
				std::swap(*right, *(--greaterptr));
				if (left >= right) break;
				goto rightpart;
			}
			if (left >= right) break;							// 若left超过了right，则不必再找
			std::swap(*left, *right);							// 将较大的元素换到右边
		}
		//if (right != left - 1)std::cout << "Why!!!!!!!!!\n";
		//outl(); std::cout << "Rotate!\n";
		//if (left <= begin || end <= left)std::cout << (end <= left) << "Why!!!!!!!!!!\n";
		//if (begin <= lessptr)
		std::rotate(begin, lessptr, left);
		//if (left <= greaterptr)
		std::rotate(left, greaterptr, end);
		return std::make_pair(begin + (left - lessptr), right + (end - greaterptr));			// 返回等于cut的区域的begin和end（尾后）
		*/
		//typedef std::pair<iterator, iterator> region;
		//region ecut1(begin, begin), lcut(begin, begin), unsorted(begin, end), gcut(end, end), ecut2(end, end);
		typedef iterator ptr;
		ptr lessptr = begin, left = begin, right = end - 1, greaterptr = end;
		ptr& e1_b = begin, & e1_e = lessptr, & l_b = lessptr, & l_e = left, & u_b = left, & u_e = right, & g_b = right, & g_e = greaterptr, & e2_b = greaterptr, & e2_e = end;
		ptr _cut = guessMedian(begin, end - 1);
		_Ty cut = *_cut;
		for (;;)
		{
			for (; u_b != u_e && *(u_b++) <= cut;)
				if (*(u_b - 1) == cut)
					std::swap(*(u_b - 1), *(e1_e++));						// 从第一项开始依次向右寻找大于等于cut的元素，left指向它右侧
			for (; u_b != u_e && cut <= *(--u_e);)
				if (*u_e == cut)
					std::swap(*u_e, *(--e2_b));
			// 从最后一项开始依次向左寻找小于等于cut的元素，right指向它
			//if (left > right)
			//{
			//	if (left - 1 != right + 1){std::cout << "CAUTION\t" << lessptr << " " << left << " " << right << " " << greaterptr << std::endl; return std::make_pair(begin, end);}
			//	left--; right++; break;
			//}												// 防止由于恰碰见_cut导致right跑到left-2
			//std::cout << lessptr << " " << left << " " << right << " " << greaterptr << std::endl;
			//outl(); std::cout << "Swap!\n";
			std::swap(*(u_b - 1), *u_e);							// 将较大的元素换到右边
		//outl();
			if (u_b == u_e) break;							// 若left超过了right，则不必再找

		}
		//dbout(); std::cout << "Rotate!\n";
		//std::cout << lessptr << " " << left << " " << right << " " << greaterptr << std::endl;
		std::rotate(e1_b, l_b, l_e);
		std::rotate(g_b, e2_b, e2_e);
		//dbout();
		//std::swap(*_cut, *ret);									// 搜索结束后，理论上left=right+1，把cut换到适当位置即可
																// 除非搜索结束的时候恰好left=right，此时*left=*right=cut，交换也无伤大雅
		return std::make_pair(begin + (l_e - l_b), g_b + (e2_e - e2_b));
	}
	_NO_INLINE void quicksort_rec(iterator begin, iterator end)
	{
		if (end <= begin + 1)
			return;
		if (end <= begin + 16)
		{
			Sort<_Ty> s(begin, end - begin);
			s.InsertionSort();
			return;
		}
		iterator partplace = partition_with_guess(begin, end);
		//std::cout << *partplace << std::endl; outl();
		quicksort_rec(begin, partplace);
		quicksort_rec(partplace + 1, end);
	}
	_NO_INLINE void quicksort_3way_rec(iterator begin, iterator end)
	{
		//if (end <= begin + 1)
		//	return;
		if (end <= begin + 41)
			//{
				//Sort<_Ty> s(begin, end - begin);
				//s.InsertionSort();
			return;
		//}
		//assert(end > begin + 16);
		std::pair<iterator, iterator> partplace;
		partplace = partition_3way_with_guess(begin, end);
		//std::cout << *partplace.first << " " << *partplace.second << std::endl; outl();
		quicksort_3way_rec(begin, partplace.first);
		quicksort_3way_rec(partplace.second, end);
	}
	_NO_INLINE void quicksort_3way_balanced_rec(iterator begin, iterator end)
	{
		//if (end <= begin + 1)
		//	return;
		if (end <= begin + 41)
			//{
				//Sort<_Ty> s(begin, end - begin);
				//s.InsertionSort();
			return;
		//}
		//assert(end > begin + 16);
		std::pair<iterator, iterator> partplace;
		partplace = partition_3way_with_guess_balanced(begin, end);
		//std::cout << *partplace.first << " " << *partplace.second << std::endl; outl();
		quicksort_3way_balanced_rec(begin, partplace.first);
		quicksort_3way_balanced_rec(partplace.second, end);
	}
	_NO_INLINE void QuickSort()
	{
		if (false)
			shuffle();
		//outl();
		quicksort_rec(tobeSorted, tobeSorted + N);
	}
	_NO_INLINE void QuickSort_3way()
	{
		if (false)
			shuffle();
		//outl();
		quicksort_3way_rec(tobeSorted, tobeSorted + N);
		InsertionSort();
		//outl();
	}
	_NO_INLINE void QuickSort_3way_balanced()
	{
		if (false)
			shuffle();
		//outl();
		quicksort_3way_balanced_rec(tobeSorted, tobeSorted + N);
		InsertionSort();
		//outl();
	}
	_NO_INLINE void HeapSort()
	{
		std::vector<_Ty> vec(N + 1);
		std::copy(tobeSorted, tobeSorted + N, vec.begin() + 1);
		my_priority_queue<_Ty> pq(vec);
		uint N = this->N;
		for (uint k = N / 2; k >= 1; k--)
			pq.sink(k, N);
		for (; N > 1;)
		{
			std::swap(vec[1], vec[N--]);
			pq.sink(1, N);
		}
		std::copy(vec.begin() + 1, vec.end(), tobeSorted);
	}
	void vthreadMain(std::future<void> ftr)
	{
		ftr.~future();
	}
	template<class _Fty, class Duration = std::chrono::seconds>
	void waitfor(_Fty&& func, Duration dur = std::chrono::seconds(10))
	{
		std::future<void> tmp = std::async(std::launch::async, func, this);
		std::thread trd;
		if (tmp.wait_for(dur) == std::future_status::timeout)
		{
			std::cout << "10s TLE!" << std::endl;
			trd = std::thread(&Sort::vthreadMain, this, static_cast<std::future<void>&&>(tmp));
			trd.detach();
		}
	}
	void my_assert(bool in)
	{
		if (!in)
		{
			std::cerr << "Why!" << std::endl;
			throw std::exception("ASSERT FAILED");
		}
	}
	void out(std::ostream& os, iterator begin, iterator end)
	{
		bool push = std::ios_base::sync_with_stdio(false);
		os << *begin;
		for (unsigned i = begin - tobeSorted + 1; i < end - begin; i++)
			os << " " << tobeSorted[i];
		std::ios_base::sync_with_stdio(push);
	}
	void debugoutl(iterator begin, iterator end)
	{
		std::ostream& os = std::cout;
		os << std::endl;
		out(os, begin, end);
		os << std::endl << std::endl;
	}
public:
	explicit Sort(const std::vector<_Ty>& in)
	{
		tobeSorted = in.begin();
		N = in.size();
	}
	Sort(iterator tobeSorted, unsigned N)
	{
		this->tobeSorted = tobeSorted;
		this->N = N;
		if (N <= 0)
			throw std::exception();
	}
	void out(std::ostream& os = std::cout)
	{
		/*
		bool push = std::ios_base::sync_with_stdio(false);
		os << tobeSorted[0];
		for (unsigned i = 1; i < N; i++)
			os << " " << tobeSorted[i];
		std::ios_base::sync_with_stdio(push);
		*/
		out(os, tobeSorted, tobeSorted + N);
	}
	void outl(std::ostream& os = std::cout)
	{
		os << std::endl;
		out(os);
		os << std::endl << std::endl;
	}
	_NO_INLINE void shuffle()
	{
		if (true)
		{
			std::srand(0);
			for (unsigned i = 0; i < N; i++)
				std::swap(tobeSorted[i], tobeSorted[std::rand() % N]);
		}
		else {
			for (unsigned cnt = 1; cnt <= std::log10(N); cnt++)
			{
				auto aux = new _Ty[N];
				bool flag = true;
				unsigned distance = N / 2;
				std::copy(tobeSorted + distance, tobeSorted + N, aux);
				std::copy(tobeSorted, tobeSorted + distance, aux + N - distance);
				unsigned i = 0;
				for (; 3 * i < N; i++)
				{
					tobeSorted[3 * i] = aux[i];
				}
				unsigned j = 0;
				for (; 3 * j + 1 < N; j++)
				{
					tobeSorted[3 * j + 1] = aux[i + j];
				}
				for (unsigned k = 0; 3 * k + 2 < N; k++)
				{
					tobeSorted[3 * k + 2] = aux[i + j + k];
				}
				delete[]aux;
			}
		}
	}
	enum class methods
	{
		Selection,
		Insertion_Slow,
		Insertion,
		Insertion_NoBdry,
		Shell,
		Merge_T2B,
		Merge_B2T,
		Quick,
		Quick_3way
	};
	_NO_INLINE void sort(methods method);
};

template<typename _Ty>
_NO_INLINE void Sort<_Ty>::sort(methods method)
{
	switch (method)
	{
	case methods::Selection:
		waitfor(&Sort::SelectionSort);
		break;
	case methods::Insertion_Slow:
		waitfor(&Sort::InsertionSort_Slow);
		break;
	case methods::Insertion:
		waitfor(&Sort::InsertionSort);
		break;
	case methods::Insertion_NoBdry:
		waitfor(&Sort::InsertionSort_NoBdry);
		break;
	case methods::Shell:
		waitfor(&Sort::ShellSort);
		break;
	case methods::Merge_T2B:
		waitfor(&Sort::MergeSort_T2B);
		break;
	case methods::Merge_B2T:
		waitfor(&Sort::MergeSort_B2T);
		break;
	case methods::Quick:
		waitfor(&Sort::HeapSort);
		break;
	case methods::Quick_3way:
		waitfor(&Sort::QuickSort_3way_balanced);
		break;
	default:
		break;
	}
}

#undef _NO_INLINE

#endif