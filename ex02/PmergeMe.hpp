/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 18:00:00 by yoshin            #+#    #+#             */
/*   Updated: 2026/06/18 10:00:00 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <vector>
# include <deque>
# include <string>
# include <iostream>
# include <sstream>
# include <ctime>
# include <cstdlib>
# include <stdexcept>
# include <algorithm>
# include <cstddef>

/*
** Ford-Johnson (merge-insertion sort), 비교 횟수 최소화 버전.
**
** 핵심 설계
**  - 블록(block) 단위 재귀: 원소를 크기 2^k 블록으로 묶어 통째로 이동하므로
**    "큰값(winner) - 작은값(loser)" 짝 대응이 재귀 내내 보존된다.
**  - 짝 정보 활용: 각 loser 블록은 자기 짝 winner 블록보다 작음이 보장되므로,
**    삽입 시 winner 의 현재 위치 앞까지로 이진 탐색 구간을 제한한다.
**  - Jacobsthal 순서: loser 블록을 b1, b3, b2, b5, b4, ... 순으로 삽입해
**    매 삽입의 탐색 구간 크기가 항상 2^k - 1 이 되도록 맞춘다.
*/
class PmergeMe
{
public:
  PmergeMe(void);
  PmergeMe(PmergeMe const &other);
  PmergeMe &operator=(PmergeMe const &other);
  ~PmergeMe(void);

  void parseArgs(int argc, char **argv);
  void sortAndDisplay(void);

private:
  std::vector<int> _vecData;
  std::deque<int> _deqData;

  static std::vector<std::size_t> _insertionOrder(std::size_t pairCount);
  static std::size_t _indexInChain(std::vector<std::size_t> const &chain,
    std::size_t blockIdx);

  template <typename C>
  static void _fordJohnsonSort(C &data);

  template <typename C>
  static void _mergeInsert(C &data, std::size_t blockSize);

  template <typename C>
  static void _pairwiseSort(C &data, std::size_t blockSize,
    std::size_t blockCount);

  template <typename C>
  static void _insertionMerge(C &data, std::size_t blockSize,
    std::size_t blockCount);

  template <typename C>
  static void _swapBlocks(C &data, std::size_t i, std::size_t j,
    std::size_t blockSize);

  template <typename C>
  static int _blockMax(C const &data, std::size_t blockIdx,
    std::size_t blockSize);

  template <typename C>
  static std::size_t _searchPos(C const &data,
    std::vector<std::size_t> const &chain, std::size_t hi, int key,
    std::size_t blockSize);

  template <typename C>
  static void _applyOrder(C &data, std::vector<std::size_t> const &chain,
    std::size_t blockSize);

  template <typename C>
  static void _printContainer(C const &container);
};

/* 정렬 진입점: 블록 크기 1 에서 시작한다. */
template <typename C>
void PmergeMe::_fordJohnsonSort(C &data)
{
  _mergeInsert(data, 1);
}

/*
** 블록 크기 blockSize 수준에서의 merge-insertion 한 단계.
**  1) 인접 블록끼리 비교해 큰 쪽을 뒤로 보낸다 (pairwiseSort).
**  2) 블록 크기를 2배로 키워 재귀 -> 큰값(winner)들이 정렬된다.
**  3) 작은값(loser)들을 정렬된 뼈대에 이진 삽입한다 (insertionMerge).
*/
template <typename C>
void PmergeMe::_mergeInsert(C &data, std::size_t blockSize)
{
  std::size_t blockCount;

  blockCount = data.size() / blockSize;
  if (blockCount < 2)
    return ;
  _pairwiseSort(data, blockSize, blockCount);
  _mergeInsert(data, blockSize * 2);
  _insertionMerge(data, blockSize, blockCount);
}

/* 인접 블록 쌍마다 1회 비교: 끝값이 큰 블록을 뒤(홀수 인덱스)로 보낸다. */
template <typename C>
void PmergeMe::_pairwiseSort(C &data, std::size_t blockSize,
  std::size_t blockCount)
{
  std::size_t j;

  for (j = 0; j + 1 < blockCount; j += 2)
  {
    if (_blockMax(data, j, blockSize) > _blockMax(data, j + 1, blockSize))
      _swapBlocks(data, j, j + 1, blockSize);
  }
}

/*
** 정렬된 winner 블록(main chain)에 loser 블록(pend)을 삽입한다.
** chain 은 "블록 번호의 정렬 순서"를 담는다. 실제 데이터 이동은
** 마지막 _applyOrder 한 번으로 끝낸다.
*/
template <typename C>
void PmergeMe::_insertionMerge(C &data, std::size_t blockSize,
  std::size_t blockCount)
{
  std::vector<std::size_t> chain;
  std::vector<std::size_t> insertOrder;
  std::size_t pairCount;
  std::size_t j;

  pairCount = blockCount / 2;
  chain.push_back(0);
  for (j = 0; j < pairCount; ++j)
    chain.push_back(2 * j + 1);
  insertOrder = _insertionOrder(pairCount);
  for (std::size_t i = 0; i < insertOrder.size(); ++i)
  {
    std::size_t loser = 2 * (insertOrder[i] - 1);
    std::size_t winner = 2 * (insertOrder[i] - 1) + 1;
    std::size_t searchHi = _indexInChain(chain, winner);
    std::size_t insertPos = _searchPos(data, chain, searchHi,
      _blockMax(data, loser, blockSize), blockSize);
    chain.insert(chain.begin() + insertPos, loser);
  }
  if (blockCount % 2 != 0)
  {
    std::size_t straggler = blockCount - 1;
    std::size_t insertPos = _searchPos(data, chain, chain.size(),
      _blockMax(data, straggler, blockSize), blockSize);
    chain.insert(chain.begin() + insertPos, straggler);
  }
  _applyOrder(data, chain, blockSize);
}

/* 두 블록(각 blockSize 개의 원소)을 통째로 맞바꾼다. */
template <typename C>
void PmergeMe::_swapBlocks(C &data, std::size_t i, std::size_t j,
  std::size_t blockSize)
{
  std::size_t k;

  for (k = 0; k < blockSize; ++k)
    std::swap(data[i * blockSize + k], data[j * blockSize + k]);
}

/* 블록의 대표값 = 블록 마지막(가장 큰) 원소. */
template <typename C>
int PmergeMe::_blockMax(C const &data, std::size_t blockIdx,
  std::size_t blockSize)
{
  return (data[blockIdx * blockSize + blockSize - 1]);
}

/*
** chain[0, hi) 구간을 블록 대표값 기준으로 이진 탐색(lower_bound)한다.
** hi 가 짝 winner 의 위치이므로 탐색 구간이 winner 앞으로 제한된다.
*/
template <typename C>
std::size_t PmergeMe::_searchPos(C const &data,
  std::vector<std::size_t> const &chain, std::size_t hi, int key,
  std::size_t blockSize)
{
  std::size_t lo;
  std::size_t mid;

  lo = 0;
  while (lo < hi)
  {
    mid = lo + (hi - lo) / 2;
    if (_blockMax(data, chain[mid], blockSize) < key)
      lo = mid + 1;
    else
      hi = mid;
  }
  return (lo);
}

/* chain 순서대로 블록을 이어붙여 data 앞부분을 재배치한다. */
template <typename C>
void PmergeMe::_applyOrder(C &data, std::vector<std::size_t> const &chain,
  std::size_t blockSize)
{
  C ordered;
  std::size_t i;
  std::size_t k;

  for (i = 0; i < chain.size(); ++i)
  {
    for (k = 0; k < blockSize; ++k)
      ordered.push_back(data[chain[i] * blockSize + k]);
  }
  for (i = 0; i < ordered.size(); ++i)
    data[i] = ordered[i];
}

template <typename C>
void PmergeMe::_printContainer(C const &container)
{
  typename C::const_iterator it;

  for (it = container.begin(); it != container.end(); ++it)
  {
    if (it != container.begin())
      std::cout << " ";
    std::cout << *it;
  }
}

#endif
