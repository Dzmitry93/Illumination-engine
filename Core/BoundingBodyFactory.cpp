#include "BoundingBodyFactory.h"

namespace OGE
{
	namespace util
	{
		OBB BoundingBodyFactory::createOBB(const IterableMesh& rMesh)
		{
			Vertex meanValue;
			meanValue.vx = 0;
			meanValue.vy = 0;
			meanValue.vz = 0;

			uint32 countVertices = rMesh.getCountVertices();

			// calculate mean point of vertices 
			ConstArrayIterator<V3TNT2Triangle> cit = rMesh.citerator();

			uint32 i = 0;
			while(cit.hasNext())
			{
				const V3TNT2Triangle& rTriangle = cit.next();
				meanValue.vx += rTriangle.v1.x + rTriangle.v2.x + rTriangle.v3.x;
				meanValue.vy += rTriangle.v1.y + rTriangle.v2.y + rTriangle.v3.y;
				meanValue.vz += rTriangle.v1.z + rTriangle.v2.z + rTriangle.v3.z;
				i++;
			}

			meanValue.vx /= countVertices;
			meanValue.vy /= countVertices;
			meanValue.vz /= countVertices;

			// calculate covariance matrix
			Matrix covarianceMatrix;

			float xx = 0.0f;
			float xy = 0.0f;
			float xz = 0.0f;
			float yy = 0.0f;
			float yz = 0.0f;
			float zz = 0.0f;

			cit = rMesh.citerator();

			while(cit.hasNext())
			{
				const V3TNT2Triangle& rTriangle = cit.next();
				xx += (rTriangle.v1.x - meanValue.vx) * (rTriangle.v1.x - meanValue.vx);
				xx += (rTriangle.v2.x - meanValue.vx) * (rTriangle.v2.x - meanValue.vx);
				xx += (rTriangle.v3.x - meanValue.vx) * (rTriangle.v3.x - meanValue.vx);

				xy += (rTriangle.v1.x - meanValue.vx) * (rTriangle.v1.y - meanValue.vy);
				xy += (rTriangle.v2.x - meanValue.vx) * (rTriangle.v2.y - meanValue.vy);
				xy += (rTriangle.v3.x - meanValue.vx) * (rTriangle.v3.y - meanValue.vy);

				xz += (rTriangle.v1.x - meanValue.vx) * (rTriangle.v1.z - meanValue.vz);
				xz += (rTriangle.v2.x - meanValue.vx) * (rTriangle.v2.z - meanValue.vz);
				xz += (rTriangle.v3.x - meanValue.vx) * (rTriangle.v3.z - meanValue.vz);

				yy += (rTriangle.v1.y - meanValue.vy) * (rTriangle.v1.y - meanValue.vy);
				yy += (rTriangle.v2.y - meanValue.vy) * (rTriangle.v2.y - meanValue.vy);
				yy += (rTriangle.v3.y - meanValue.vy) * (rTriangle.v3.y - meanValue.vy);

				yz += (rTriangle.v1.y - meanValue.vy) * (rTriangle.v1.z - meanValue.vz);
				yz += (rTriangle.v2.y - meanValue.vy) * (rTriangle.v2.z - meanValue.vz);
				yz += (rTriangle.v3.y - meanValue.vy) * (rTriangle.v3.z - meanValue.vz);

				zz += (rTriangle.v1.z - meanValue.vz) * (rTriangle.v1.z - meanValue.vz);
				zz += (rTriangle.v2.z - meanValue.vz) * (rTriangle.v2.z - meanValue.vz);
				zz += (rTriangle.v3.z - meanValue.vz) * (rTriangle.v3.z - meanValue.vz);
			}

			xx /= countVertices;
			yy /= countVertices;
			zz /= countVertices;

			xy /= countVertices;
			xz /= countVertices;

			yz /= countVertices;

			covarianceMatrix.data[0][0] = xx;
			covarianceMatrix.data[1][1] = yy;
			covarianceMatrix.data[2][2] = zz;

			covarianceMatrix.data[0][1] = xy;
			covarianceMatrix.data[1][0] = xy;

			covarianceMatrix.data[0][2] = xz;
			covarianceMatrix.data[2][0] = xz;

			covarianceMatrix.data[1][2] = yz;
			covarianceMatrix.data[2][1] = yz;

			// calculate eigenvectors
			Vector lambda;
			Matrix eigenVectors;

			NumericalMethods::CalculateEigensystem(covarianceMatrix, lambda, eigenVectors);

			// calculate obb planes and halfdistances
			float maxDP = FLT_MIN;
			float minDP = FLT_MAX;
			float maxDR = FLT_MIN;
			float minDR = FLT_MAX;
			float maxDS = FLT_MIN;
			float minDS = FLT_MAX;

			Vector p = eigenVectors.getColumn(0);
			Vector r = eigenVectors.getColumn(1);
			Vector s = eigenVectors.getColumn(2);

			p.normalize();
			r.normalize();
			s.normalize();

			cit = rMesh.citerator();

			while(cit.hasNext())
			{
				const V3TNT2Triangle& rTriangle = cit.next(); 
				float dp = p.dotProduct(rTriangle.v1.x, rTriangle.v1.y, rTriangle.v1.z);
				float dr = r.dotProduct(rTriangle.v1.x, rTriangle.v1.y, rTriangle.v1.z);
				float ds = s.dotProduct(rTriangle.v1.x, rTriangle.v1.y, rTriangle.v1.z);

				if(FloatGreaterOrEquals(minDP, dp)) minDP = dp;
				if(FloatGreaterOrEquals(dp, maxDP)) maxDP = dp;

				if(FloatGreaterOrEquals(minDR, dr)) minDR = dr;
				if(FloatGreaterOrEquals(dr, maxDR)) maxDR = dr;

				if(FloatGreaterOrEquals(minDS, ds)) minDS = ds;
				if(FloatGreaterOrEquals(ds, maxDS)) maxDS = ds;

				dp = p.dotProduct(rTriangle.v2.x, rTriangle.v2.y, rTriangle.v2.z);
				dr = r.dotProduct(rTriangle.v2.x, rTriangle.v2.y, rTriangle.v2.z);
				ds = s.dotProduct(rTriangle.v2.x, rTriangle.v2.y, rTriangle.v2.z);

				if(FloatGreaterOrEquals(minDP, dp)) minDP = dp;
				if(FloatGreaterOrEquals(dp, maxDP)) maxDP = dp;

				if(FloatGreaterOrEquals(minDR, dr)) minDR = dr;
				if(FloatGreaterOrEquals(dr, maxDR)) maxDR = dr;

				if(FloatGreaterOrEquals(minDS, ds)) minDS = ds;
				if(FloatGreaterOrEquals(ds, maxDS)) maxDS = ds;

				dp = p.dotProduct(rTriangle.v3.x, rTriangle.v3.y, rTriangle.v3.z);
				dr = r.dotProduct(rTriangle.v3.x, rTriangle.v3.y, rTriangle.v3.z);
				ds = s.dotProduct(rTriangle.v3.x, rTriangle.v3.y, rTriangle.v3.z);

				if(FloatGreaterOrEquals(minDP, dp)) minDP = dp;
				if(FloatGreaterOrEquals(dp, maxDP)) maxDP = dp;

				if(FloatGreaterOrEquals(minDR, dr)) minDR = dr;
				if(FloatGreaterOrEquals(dr, maxDR)) maxDR = dr;

				if(FloatGreaterOrEquals(minDS, ds)) minDS = ds;
				if(FloatGreaterOrEquals(ds, maxDS)) maxDS = ds;
			}	

			Plane pl1(p, -minDP);
			Plane pl2(r, -minDR);
			Plane pl3(s, -minDS);
			Plane pl4(-p, maxDP);
			Plane pl5(-r, maxDR);
			Plane pl6(-s, maxDS);

			float a = ( minDP + maxDP ) / 2.0f;
			float b = ( minDR + maxDR ) / 2.0f;
			float c = ( minDS + maxDS ) / 2.0f;

			Vector centerPoint = p * a + r * b + s * c;
			centerPoint.w = 1.0f;

			return OBB(centerPoint, pl1, pl2, pl3, pl4, pl5, pl6);
		}

		AABB BoundingBodyFactory::createAABB(const List<SceneObject<OBB>>& rOBBs)
		{
			float maxX = -FLT_MAX;	
			float maxY = -FLT_MAX;
			float maxZ = -FLT_MAX;
			
			ConstListIterator<SceneObject<OBB>> it = rOBBs.citerator();

			while(it.hasNext())
			{
				const SceneObject<OBB>& sceneOBB = it.next();
				OBB obb = TransformUtils::transform(
					sceneOBB.getSceneObject(),
					sceneOBB.getWorldMatrix());

				List<Vector> bbPoints = BoundingBodyUtils::getBoundingBodyPoints(obb);

				ListIterator<Vector> bbIt = bbPoints.iterator();

				while(bbIt.hasNext())
				{
					const Vector& rPoint = bbIt.next();

					if(abs(rPoint.x) > maxX) { maxX = abs(rPoint.x); }
					if(abs(rPoint.y) > maxY) { maxY = abs(rPoint.y); }
					if(abs(rPoint.z) > maxZ) { maxZ = abs(rPoint.z); }
				}
			}

			return AABB(Vector(0.0f, 0.0f, 0.0f), Vector(maxX, maxY, maxZ));
		}

		AABB BoundingBodyFactory::createAABB(ListIterator<OBB> begin, ListIterator<OBB> end)
		{
			float maxX = -FLT_MAX;	
			float maxY = -FLT_MAX;
			float maxZ = -FLT_MAX;

			float minX = FLT_MAX;	
			float minY = FLT_MAX;
			float minZ = FLT_MAX;

			if(!begin.equals(end))
			{
				do
				{
					OBB& rObb = begin.next();
				
					List<Vector> bbPoints = BoundingBodyUtils::getBoundingBodyPoints(rObb);
					ListIterator<Vector> bbIt = bbPoints.iterator();

					while(bbIt.hasNext())
					{
						const Vector& rPoint = bbIt.next();

						if(rPoint.x > maxX) { maxX = rPoint.x; }
						if(rPoint.y > maxY) { maxY = rPoint.y; }
						if(rPoint.z > maxZ) { maxZ = rPoint.z; }

						if(rPoint.x < minX) { minX = rPoint.x; }
						if(rPoint.y < minY) { minY = rPoint.y; }
						if(rPoint.z < minZ) { minZ = rPoint.z; }
					}

				} while(begin.equals(end));
			}

			float halfX = abs(maxX - minX) / 2.0f;
			float halfY = abs(maxY - minY) / 2.0f;
			float halfZ = abs(maxZ - minZ) / 2.0f;

			return AABB(Vector(maxX - halfX, maxY - halfY, maxZ - halfZ),
						Vector(halfX, halfY, halfZ));
		}

		AABB BoundingBodyFactory::createAABB(ListIterator<OBB> begin)
		{	
			if(!begin.hasNext())
			{
				return AABB();
			}

			float maxX = -FLT_MAX;	
			float maxY = -FLT_MAX;
			float maxZ = -FLT_MAX;

			float minX = FLT_MAX;	
			float minY = FLT_MAX;
			float minZ = FLT_MAX;

			while(begin.hasNext())
			{
				OBB& rObb = begin.next();
				
				List<Vector> bbPoints = BoundingBodyUtils::getBoundingBodyPoints(rObb);
				ListIterator<Vector> bbIt = bbPoints.iterator();

				while(bbIt.hasNext())
				{
					const Vector& rPoint = bbIt.next();

					if(rPoint.x > maxX) { maxX = rPoint.x; }
					if(rPoint.y > maxY) { maxY = rPoint.y; }
					if(rPoint.z > maxZ) { maxZ = rPoint.z; }

					if(rPoint.x < minX) { minX = rPoint.x; }
					if(rPoint.y < minY) { minY = rPoint.y; }
					if(rPoint.z < minZ) { minZ = rPoint.z; }
				}
			}

			float halfX = abs(maxX - minX) / 2.0f;
			float halfY = abs(maxY - minY) / 2.0f;
			float halfZ = abs(maxZ - minZ) / 2.0f;

			return AABB(Vector(maxX - halfX, maxY - halfY, maxZ - halfZ),
						Vector(halfX, halfY, halfZ));
		}
	}
}