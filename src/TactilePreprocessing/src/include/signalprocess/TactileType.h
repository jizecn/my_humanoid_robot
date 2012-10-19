#ifndef _TOUCHLOCATION_H_
#define _TOUCHLOCATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_statistics.h>

class TactileType {

public: 
	// temporarily hardcoded.
	enum TouchType {soft, hard, none};  

	enum TouchLocation {left, right, palm, dorsal};  // left, right for cheeks, palm and dorsal for hand parts

	struct TouchEvent {
		TouchType type;
		TouchLocation loc; 
		double level;  // to quantify the level of touch event, 
	};
};


#endif // _TACTILETYPE_H_
