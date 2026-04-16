/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 18:00:00 by yoshin            #+#    #+#             */
/*   Updated: 2026/03/26 18:00:00 by yoshin           ###   ########.fr       */
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

  static std::vector<int> _jacobsthalSequence(int n);

  static void _fordJohnsonSort(std::vector<int> &data);
  static void _fordJohnsonSort(std::deque<int> &data);

  template <typename T>
  static void _printContainer(T const &container);

  template <typename T>
  static typename T::iterator _binaryInsert(T &sorted, int value,
    typename T::iterator end);
};

template <typename T>
void PmergeMe::_printContainer(T const &container)
{
  typename T::const_iterator it;
  for (it = container.begin(); it != container.end(); ++it)
  {
    if (it != container.begin())
      std::cout << " ";
    std::cout << *it;
  }
}

template <typename T>
typename T::iterator PmergeMe::_binaryInsert(T &sorted, int value,
  typename T::iterator end)
{
  typename T::iterator pos = std::lower_bound(sorted.begin(), end, value);
  return (sorted.insert(pos, value));
}

#endif
