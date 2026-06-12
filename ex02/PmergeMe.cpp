/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 18:00:00 by yoshin            #+#    #+#             */
/*   Updated: 2026/06/12 12:30:29 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

PmergeMe::PmergeMe(void)
{
}

PmergeMe::PmergeMe(PmergeMe const &other)
  : _vecData(other._vecData), _deqData(other._deqData)
{
}

PmergeMe &PmergeMe::operator=(PmergeMe const &other)
{
  if (this != &other)
  {
    _vecData = other._vecData;
    _deqData = other._deqData;
  }
  return (*this);
}

PmergeMe::~PmergeMe(void)
{
}

void PmergeMe::parseArgs(int argc, char **argv)
{
  for (int i = 1; i < argc; i++)
  {
    std::string arg(argv[i]);
    if (arg.empty())
      throw std::runtime_error("Error");
    for (std::string::size_type j = 0; j < arg.length(); j++)
    {
      if (arg[j] < '0' || arg[j] > '9')
        throw std::runtime_error("Error");
    }
    long val = std::atol(arg.c_str());
    if (val <= 0 || val > 2147483647L)
      throw std::runtime_error("Error");
    _vecData.push_back(static_cast<int>(val));
    _deqData.push_back(static_cast<int>(val));
  }
  if (_vecData.empty())
    throw std::runtime_error("Error");
}

/*
** pend 블록(짝수 인덱스 loser)을 삽입할 순서를 1-based rank 로 돌려준다.
** rank 1(b1)은 호출 측에서 미리 chain 앞에 두므로 제외하고,
** Jacobsthal 경계(1, 3, 5, 11, ...)마다 그룹을 내림차순으로 나열한다.
** 예) pairs == 5  ->  [3, 2, 5, 4]
*/
std::vector<std::size_t> PmergeMe::_insertionOrder(std::size_t pairs)
{
  std::vector<std::size_t> order;
  std::vector<std::size_t> jacob;
  std::size_t prev;

  if (pairs < 2)
    return (order);
  jacob.push_back(1);
  jacob.push_back(3);
  while (jacob.back() < pairs)
    jacob.push_back(jacob[jacob.size() - 1] + 2 * jacob[jacob.size() - 2]);
  prev = 1;
  for (std::size_t i = 1; i < jacob.size(); ++i)
  {
    std::size_t hi = jacob[i] < pairs ? jacob[i] : pairs;
    for (std::size_t r = hi; r > prev; --r)
      order.push_back(r);
    if (jacob[i] >= pairs)
      break ;
    prev = jacob[i];
  }
  return (order);
}

/* chain 안에서 블록 인덱스 unitIdx 가 놓인 현재 위치를 찾는다. */
std::size_t PmergeMe::_indexInChain(std::vector<std::size_t> const &chain,
  std::size_t unitIdx)
{
  for (std::size_t i = 0; i < chain.size(); ++i)
  {
    if (chain[i] == unitIdx)
      return (i);
  }
  return (chain.size());
}

void PmergeMe::sortAndDisplay(void)
{
  std::cout << "Before: ";
  _printContainer(_vecData);
  std::cout << std::endl;

  std::vector<int> vecCopy(_vecData);
  std::deque<int> deqCopy(_deqData);

  clock_t vecStart = clock();
  _fordJohnsonSort(vecCopy);
  clock_t vecEnd = clock();

  clock_t deqStart = clock();
  _fordJohnsonSort(deqCopy);
  clock_t deqEnd = clock();

  std::cout << "After:  ";
  _printContainer(vecCopy);
  std::cout << std::endl;

  double vecTime = static_cast<double>(vecEnd - vecStart)
    / CLOCKS_PER_SEC * 1000000.0;
  double deqTime = static_cast<double>(deqEnd - deqStart)
    / CLOCKS_PER_SEC * 1000000.0;

  std::cout << "Time to process a range of " << vecCopy.size()
            << " elements with std::vector : " << vecTime << " us"
            << std::endl;
  std::cout << "Time to process a range of " << deqCopy.size()
            << " elements with std::deque  : " << deqTime << " us"
            << std::endl;
}
