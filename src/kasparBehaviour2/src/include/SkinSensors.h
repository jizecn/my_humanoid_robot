#ifndef __SKIN_SENSORS__
#define __SKIN_SENSORS__

#include <vector>
#include <string>
using namespace std;

class SkinSensors {
 public:
  SkinSensors() {
  }

  ~SkinSensors() {
  }
  
 private:
  int numOfSensorPatches;
  std::vector<SkinPart> sensorPatchList;
  std::string partName;
};

#endif
