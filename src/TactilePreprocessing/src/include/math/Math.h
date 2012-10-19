#ifndef __ZJ_MATH_H__
#define __ZJ_MATH_H__

#include "TactileData.h"
#include <gsl/gsl_matrix.h>

namespace zjMath
{
	class Math;
}

class Math
{
public:
	static gsl_matrix * reOrderMatrix(gsl_matrix * m, int *indexOrder, bool byRow)
	{
		// TODO:
		int r = m->size1;
		int c = m->size2;
		gsl_matrix * orderedM = gsl_matrix_alloc(r, c);
		
		// //
		
		return orderedM;
	}
	
/* Do not forget to gsl_matrix_free the new matrix created*/
	static gsl_matrix * appendMatrix(gsl_matrix *m1, gsl_matrix *m2, bool byRow)
	{
		// TODO:
		int r1 = m1->size1;
		int c1 = m1->size2;
		printf("Dim of m1 is %d by %d\n", r1, c1);
		int r2 = m2->size1;
		int c2 = m2->size2;
		printf("Dim of m2 is %d by %d\n", r2, c2);
		
		size_t r, c;
		gsl_matrix * orderedM;
		
		if (byRow)
		{
			if(c1 != c2)
			{
				return NULL;
			}
			else{
				r = r1+r2;
				c = c1;
			}
		}
		else
		{
			if(r1 != r2)
			{
				return NULL;
			}
			else{
				r = r1;
				c = c1 + c2;
			}
		}
		printf("Dim of newM is %d by %d\n", r, c);
		
		orderedM = gsl_matrix_alloc(r, c);
		
		return orderedM;
	}
	
private:
	
};

#endif //__ZJ_MATH_H__
