/*
 * Copyright (C) 2010 Ze Ji
 * University of Hertfordshire. 
 * EU FP7 RoboSkin 
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__
#include <iostream>
#include <string>
using namespace std;
class Tokenizer
{
 public:
  static const std::string DELIMITERS;
  Tokenizer(const std::string& str);
  Tokenizer(const std::string& str, const std::string& delimiters);
  bool NextToken();
  bool NextToken(const std::string& delimiters);
  const std::string GetToken() const;
  void Reset();
 protected:
  size_t m_offset;
  const std::string m_string;
  std::string m_token;
  std::string m_delimiters;
};

#endif // __TOKENIZER_H__
