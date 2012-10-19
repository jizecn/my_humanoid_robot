#include "createKeymapXML.h"
using namespace std;
KeymapCreator::KeymapCreator()
{

}

KeymapCreator::~KeymapCreator()
{

}

bool KeymapCreator::loadOriginalKeymap(char * filename, char *filepath = "./")
{
  FILE *file = fopen((string(filepath) + FILESEPARATOR + filename).c_str(), "r");
  int ret = 0;
  // how long? 100 is enough for here
  char buf[150]; // = (char *)malloc(sizeof(char)*100);
  std::string tmpTok;

  if (file == NULL) {
    fprintf(stderr, "Failed to open key map file %s", filename);
    return false;
  }
  
  if (fgets(buf, 150, file) == NULL) {
    printf("Seems file format is wrong, when reading first line (NULL).\n"); 
    return false;
  }
  
  zjStripNewline(buf);
  
  if (strcmp(strcat(buf, ".skm"), filename)) {
    printf("Seems file format is wrong, when reading first line \n"); 
    return false;
  }
  
  int i = 0; // count line number
  //char poseName[100];
  do {
    //keyMap km;
    char tmpBuf[150];
    if (fgets(buf, 150, file) == NULL) {
      break;
    }
    strcpy(tmpBuf, buf);
    zjStripNewline(tmpBuf);
    
    printf("READ %d LINE: %s\n", i + 1, tmpBuf);
    if (strlen(tmpBuf) == 0) {
      printf("ERROR HERE. String length is 0\n");
      continue;
    } else if (tmpBuf[0] == ';') {
      continue;
    }
    // process buf
    // content part
    
    std::string tmpStr = "";//tmpBuf;
    std::string del = ",";
    Tokenizer tok(tmpStr, del);
    keyPair kpair; 
    
    if(tok.NextToken()) {
      tmpTok = tok.GetToken();
      kpair.poseName = tmpTok;
      printf(kpair.poseName.c_str());
      printf("  ");
    }
    else {
      printf("Keymap file (pose name) format incorrect (NULL)\n");
      return false;
    }
    
    if(tok.NextToken()) {
      tmpTok = tok.GetToken();
      kpair.key = tmpTok;
      printf(kpair.key.c_str());
      printf("   \n");
    }
    else {
      printf("Keymap file (pose key) format incorrect (NULL)\n");
      return false;
    }
    this->keyPairList.push_back(kpair);
    
    i++;
  } while (true);
  
  if (ferror(file) || !feof(file)) {
    fprintf(stderr, "Failed to read keymap file %s", filename);
    return false;
  }
  ret = fclose(file);
  if (ret == EOF || ret != 0) {
    fprintf(stderr, "Failed to close keymap file %s", filename);
    return false;
  } else {
    return true;
    // no problem
  }
}

