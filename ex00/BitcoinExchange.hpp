/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 18:00:00 by yoshin            #+#    #+#             */
/*   Updated: 2026/03/26 18:00:00 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <string>
# include <map>
# include <fstream>
# include <iostream>
# include <sstream>
# include <cstdlib>

class BitcoinExchange
{
public:
  BitcoinExchange(void);
  BitcoinExchange(std::string const &dbFile);
  BitcoinExchange(BitcoinExchange const &other);
  BitcoinExchange &operator=(BitcoinExchange const &other);
  ~BitcoinExchange(void);

  void loadDatabase(std::string const &dbFile);
  void processInput(std::string const &inputFile);

private:
  std::map<std::string, float> _database;

  bool _isValidDate(std::string const &date) const;
  float _getRate(std::string const &date) const;
};

#endif
