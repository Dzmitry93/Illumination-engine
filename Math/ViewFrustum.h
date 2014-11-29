#ifndef __VIEW_FRUSTUM_H__
#define __VIEW_FRUSTUM_H__

#include "Plane.h"
#include "Matrix.h"

namespace OGE
{
	namespace math
	{
		class ViewFrustum			  
		{
		public:

			ViewFrustum() {};
			ViewFrustum(const Matrix& rProjection);  

			void update(const Matrix& rProjection);
		
			/* Order of planes : 
				left, right, bottom, top, near, far
			*/
			Plane planes[6]; 
		};

		inline ViewFrustum::ViewFrustum(const Matrix& m) 
		{
			update(m);
		}

		inline void ViewFrustum::update(const Matrix& m)
		{
			Plane left(m.data[0][3] + m.data[0][0], 
				m.data[1][3] + m.data[1][0],
				m.data[2][3] + m.data[2][0], 
				m.data[3][3] + m.data[3][0]);

			Plane right(m.data[0][3] - m.data[0][0], 
				m.data[1][3] - m.data[1][0],
				m.data[2][3] - m.data[2][0], 
				m.data[3][3] - m.data[3][0]);

			Plane bottom(m.data[0][3] + m.data[0][1], 
				m.data[1][3] + m.data[1][1],
				m.data[2][3] + m.data[2][1], 
				m.data[3][3] + m.data[3][1]);

			Plane top(m.data[0][3] - m.data[0][1], 
				m.data[1][3] - m.data[1][1],
				m.data[2][3] - m.data[2][1], 
				m.data[3][3] - m.data[3][1]);
		
			Plane nearPlane(m.data[0][2], 
				m.data[1][2],
				m.data[2][2], 
				m.data[3][2]);

			Plane farPlane(m.data[0][3] - m.data[0][2], 
				m.data[1][3] - m.data[1][2],
				m.data[2][3] - m.data[2][2], 
				m.data[3][3] - m.data[3][2]);	 

			planes[0] = left;
			planes[1] = right;
			planes[2] = bottom;
			planes[3] = top;
			planes[4] = nearPlane;
			planes[5] = farPlane;
		}
	}
}

#endif