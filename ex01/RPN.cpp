/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 18:00:00 by yoshin            #+#    #+#             */
/*   Updated: 2026/03/26 18:00:00 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN(void)
{
}

RPN::RPN(RPN const &other)
{
  (void)other;
}

RPN &RPN::operator=(RPN const &other)
{
  (void)other;
  return (*this);
}

RPN::~RPN(void)
{
}

bool RPN::_isOperator(std::string const &token)
{
  return (token == "+" || token == "-" || token == "*" || token == "/");
}

int RPN::_applyOp(int left, int right, std::string const &op)
{
  if (op == "+")
    return (left + right);
  if (op == "-")
    return (left - right);
  if (op == "*")
    return (left * right);
  if (op == "/")
  {
    if (right == 0)
      throw std::runtime_error("Error: division by zero");
    return (left / right);
  }
  throw std::runtime_error("Error");
}

int RPN::calculate(std::string const &expression)
{
  std::stack<int> stack;
  std::istringstream iss(expression);
  std::string token;

  while (iss >> token)
  {
    if (_isOperator(token))
    {
      if (stack.size() < 2)
        throw std::runtime_error("Error");
      int right = stack.top();
      stack.pop();
      int left = stack.top();
      stack.pop();
      stack.push(_applyOp(left, right, token));
    }
    else
    {
      if (token.length() != 1 || token[0] < '0' || token[0] > '9')
        throw std::runtime_error("Error");
      stack.push(token[0] - '0');
    }
  }
  if (stack.size() != 1)
    throw std::runtime_error("Error");
  return (stack.top());
}
