#ifndef __CREATE_KEYMAP_H__
#define __CREATE_KEYMAP_H__

#include <iostream>
#include <string>
#include <sstream>
#include "utils.h"
#include <vector>

#include "Tokenizer.h"

#define FILESEPARATOR "/"

struct keyPair {
  std::string key;
  std::string poseName;
};

class KeymapCreator 
{
 public: 
  KeymapCreator();
  ~KeymapCreator();

  bool loadOriginalKeymap(char * filename, char * filepath);
  bool writeToXML(char *filename, char *filepath);

 private: 
  std::vector<keyPair> keyPairList;
};

#endif // __CREATE_KEYMAP_H__
