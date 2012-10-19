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

//CPP file
const string Tokenizer::DELIMITERS(" \t\n\r");

Tokenizer::Tokenizer(const std::string& s) :
m_string(s), m_offset(0), m_delimiters(DELIMITERS) {
}

Tokenizer::Tokenizer(const std::string& s, const std::string& delimiters) :
m_string(s), m_offset(0), m_delimiters(delimiters) {
}

bool Tokenizer::NextToken() {
  return NextToken(m_delimiters);
}

bool Tokenizer::NextToken(const std::string& delimiters) {
  size_t i = m_string.find_first_not_of(delimiters, m_offset);
  if (string::npos == i) {
    m_offset = m_string.length();
    return false;
  }
  
  size_t j = m_string.find_first_of(delimiters, i);
  if (string::npos == j) {
    m_token = m_string.substr(i);
    m_offset = m_string.length();
    return true;
  }
  
  m_token = m_string.substr(i, j - i);
  m_offset = j;
  return true;
}

const std::string Tokenizer::GetToken() const
{
  return m_token;
}
