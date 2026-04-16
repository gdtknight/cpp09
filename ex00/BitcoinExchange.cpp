/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 18:00:00 by yoshin            #+#    #+#             */
/*   Updated: 2026/03/26 18:00:00 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(void)
{
}

BitcoinExchange::BitcoinExchange(std::string const &dbFile)
{
  loadDatabase(dbFile);
}

BitcoinExchange::BitcoinExchange(BitcoinExchange const &other)
  : _database(other._database)
{
}

BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange const &other)
{
  if (this != &other)
  {
    _database = other._database;
  }
  return (*this);
}

BitcoinExchange::~BitcoinExchange(void)
{
}

void BitcoinExchange::loadDatabase(std::string const &dbFile)
{
  std::ifstream file(dbFile.c_str());
  std::string line;

  if (!file.is_open())
  {
    std::cerr << "Error: could not open file." << std::endl;
    return ;
  }
  std::getline(file, line);
  while (std::getline(file, line))
  {
    std::string::size_type pos = line.find(',');
    if (pos == std::string::npos)
      continue ;
    std::string date = line.substr(0, pos);
    std::string rateStr = line.substr(pos + 1);
    float rate = static_cast<float>(std::atof(rateStr.c_str()));
    _database[date] = rate;
  }
  file.close();
}

bool BitcoinExchange::_isValidDate(std::string const &date) const
{
  if (date.length() != 10)
    return (false);
  if (date[4] != '-' || date[7] != '-')
    return (false);
  for (int i = 0; i < 10; i++)
  {
    if (i == 4 || i == 7)
      continue ;
    if (date[i] < '0' || date[i] > '9')
      return (false);
  }
  int year = std::atoi(date.substr(0, 4).c_str());
  int month = std::atoi(date.substr(5, 2).c_str());
  int day = std::atoi(date.substr(8, 2).c_str());
  if (year < 1 || month < 1 || month > 12 || day < 1 || day > 31)
    return (false);
  return (true);
}

float BitcoinExchange::_getRate(std::string const &date) const
{
  std::map<std::string, float>::const_iterator it;

  it = _database.lower_bound(date);
  if (it != _database.end() && it->first == date)
    return (it->second);
  if (it == _database.begin())
    return (-1);
  --it;
  return (it->second);
}

void BitcoinExchange::processInput(std::string const &inputFile)
{
  std::ifstream file(inputFile.c_str());
  std::string line;

  if (!file.is_open())
  {
    std::cerr << "Error: could not open file." << std::endl;
    return ;
  }
  std::getline(file, line);
  while (std::getline(file, line))
  {
    std::string::size_type pos = line.find(" | ");
    if (pos == std::string::npos)
    {
      std::cerr << "Error: bad input => " << line << std::endl;
      continue ;
    }
    std::string date = line.substr(0, pos);
    std::string valueStr = line.substr(pos + 3);
    if (!_isValidDate(date))
    {
      std::cerr << "Error: bad input => " << line << std::endl;
      continue ;
    }
    char *end;
    double value = std::strtod(valueStr.c_str(), &end);
    if (*end != '\0' && *end != '\n')
    {
      std::cerr << "Error: bad input => " << line << std::endl;
      continue ;
    }
    if (value < 0)
    {
      std::cerr << "Error: not a positive number." << std::endl;
      continue ;
    }
    if (value > 1000)
    {
      std::cerr << "Error: too large a number." << std::endl;
      continue ;
    }
    float rate = _getRate(date);
    if (rate < 0)
    {
      std::cerr << "Error: bad input => " << date << std::endl;
      continue ;
    }
    std::cout << date << " => " << value << " = "
              << static_cast<float>(value) * rate << std::endl;
  }
  file.close();
}
