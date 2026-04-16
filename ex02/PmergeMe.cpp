/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 18:00:00 by yoshin            #+#    #+#             */
/*   Updated: 2026/03/26 18:00:00 by yoshin           ###   ########.fr       */
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

std::vector<int> PmergeMe::_jacobsthalSequence(int n)
{
  std::vector<int> seq;

  if (n <= 0)
    return (seq);
  int prev2 = 0;
  int prev1 = 1;
  while (prev1 < n)
  {
    seq.push_back(prev1);
    int next = prev1 + 2 * prev2;
    prev2 = prev1;
    prev1 = next;
  }
  return (seq);
}

void PmergeMe::_fordJohnsonSort(std::vector<int> &data)
{
  if (data.size() <= 1)
    return ;

  std::vector<std::pair<int, int> > pairs;
  int straggler = -1;
  bool hasStraggler = false;

  if (data.size() % 2 != 0)
  {
    straggler = data.back();
    hasStraggler = true;
  }
  for (std::vector<int>::size_type i = 0; i + 1 < data.size(); i += 2)
  {
    if (data[i] > data[i + 1])
      pairs.push_back(std::make_pair(data[i], data[i + 1]));
    else
      pairs.push_back(std::make_pair(data[i + 1], data[i]));
  }

  std::vector<int> mainChain;
  std::vector<int> pend;
  for (std::vector<std::pair<int, int> >::size_type i = 0;
       i < pairs.size(); i++)
  {
    mainChain.push_back(pairs[i].first);
    pend.push_back(pairs[i].second);
  }

  _fordJohnsonSort(mainChain);

  std::vector<int> sorted(mainChain);
  std::vector<int> pendReordered;

  std::vector<int> jacobsthal = _jacobsthalSequence(
    static_cast<int>(pend.size()));

  std::vector<bool> inserted(pend.size(), false);
  for (std::vector<int>::size_type ji = 0; ji < jacobsthal.size(); ji++)
  {
    int idx = jacobsthal[ji];
    for (int k = idx; k >= 1; k--)
    {
      int pendIdx = k - 1;
      if (pendIdx < static_cast<int>(pend.size()) && !inserted[pendIdx])
      {
        _binaryInsert(sorted, pend[pendIdx], sorted.end());
        inserted[pendIdx] = true;
      }
    }
  }
  for (std::vector<bool>::size_type i = 0; i < inserted.size(); i++)
  {
    if (!inserted[i])
      _binaryInsert(sorted, pend[i], sorted.end());
  }
  if (hasStraggler)
    _binaryInsert(sorted, straggler, sorted.end());

  data = sorted;
}

void PmergeMe::_fordJohnsonSort(std::deque<int> &data)
{
  if (data.size() <= 1)
    return ;

  std::deque<std::pair<int, int> > pairs;
  int straggler = -1;
  bool hasStraggler = false;

  if (data.size() % 2 != 0)
  {
    straggler = data.back();
    hasStraggler = true;
  }
  for (std::deque<int>::size_type i = 0; i + 1 < data.size(); i += 2)
  {
    if (data[i] > data[i + 1])
      pairs.push_back(std::make_pair(data[i], data[i + 1]));
    else
      pairs.push_back(std::make_pair(data[i + 1], data[i]));
  }

  std::deque<int> mainChain;
  std::deque<int> pend;
  for (std::deque<std::pair<int, int> >::size_type i = 0;
       i < pairs.size(); i++)
  {
    mainChain.push_back(pairs[i].first);
    pend.push_back(pairs[i].second);
  }

  _fordJohnsonSort(mainChain);

  std::deque<int> sorted(mainChain.begin(), mainChain.end());
  std::vector<int> jacobsthal = _jacobsthalSequence(
    static_cast<int>(pend.size()));

  std::vector<bool> inserted(pend.size(), false);
  for (std::vector<int>::size_type ji = 0; ji < jacobsthal.size(); ji++)
  {
    int idx = jacobsthal[ji];
    for (int k = idx; k >= 1; k--)
    {
      int pendIdx = k - 1;
      if (pendIdx < static_cast<int>(pend.size()) && !inserted[pendIdx])
      {
        _binaryInsert(sorted, pend[pendIdx], sorted.end());
        inserted[pendIdx] = true;
      }
    }
  }
  for (std::vector<bool>::size_type i = 0; i < inserted.size(); i++)
  {
    if (!inserted[i])
      _binaryInsert(sorted, pend[i], sorted.end());
  }
  if (hasStraggler)
    _binaryInsert(sorted, straggler, sorted.end());

  data.assign(sorted.begin(), sorted.end());
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
