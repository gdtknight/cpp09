/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 18:00:00 by yoshin            #+#    #+#             */
/*   Updated: 2026/03/26 18:00:00 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
# define RPN_HPP

# include <string>
# include <stack>
# include <sstream>
# include <stdexcept>

class RPN
{
public:
  static int calculate(std::string const &expression);

private:
  RPN(void);
  RPN(RPN const &other);
  RPN &operator=(RPN const &other);
  ~RPN(void);

  static bool _isOperator(std::string const &token);
  static int _applyOp(int left, int right, std::string const &op);
};

#endif
