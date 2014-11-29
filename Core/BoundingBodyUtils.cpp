#include "BoundingBodyUtils.h"

namespace OGE
{
	namespace util
	{
		List<Vector> BoundingBodyUtils::getBoundingBodyPoints(const OBB& rOBB)
		{
			List<Vector> list;

			float fd = rOBB.planes[0].halfDistance;
			float sd = rOBB.planes[1].halfDistance;
			float td = rOBB.planes[2].halfDistance;

			Vector point = 
				rOBB.middlePoint + rOBB.planes[0].dir * fd + 
				rOBB.planes[1].dir * sd + rOBB.planes[2].dir * td;

			list.push_back(point);

			point = 
				rOBB.middlePoint + rOBB.planes[0].dir * fd + 
				rOBB.planes[1].dir * sd - rOBB.planes[2].dir * td;

			list.push_back(point);

			point = 
				rOBB.middlePoint + rOBB.planes[0].dir * fd - 
				rOBB.planes[1].dir * sd - rOBB.planes[2].dir * td;

			list.push_back(point);

			point = 
				rOBB.middlePoint + rOBB.planes[0].dir * fd -
				rOBB.planes[1].dir * sd + rOBB.planes[2].dir * td;

			list.push_back(point);

			point = 
				rOBB.middlePoint - rOBB.planes[0].dir * fd + 
				rOBB.planes[1].dir * sd + rOBB.planes[2].dir * td;

			list.push_back(point);

			point = 
				rOBB.middlePoint - rOBB.planes[0].dir * fd + 
				rOBB.planes[1].dir * sd - rOBB.planes[2].dir * td;

			list.push_back(point);

			point = 
				rOBB.middlePoint - rOBB.planes[0].dir * fd - 
				rOBB.planes[1].dir * sd - rOBB.planes[2].dir * td;

			list.push_back(point);

			point = 
				rOBB.middlePoint - rOBB.planes[0].dir * fd -
				rOBB.planes[1].dir * sd + rOBB.planes[2].dir * td;

			list.push_back(point);		

			return list;
		}
	}
}