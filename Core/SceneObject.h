#ifndef __SCENE_OBJECT_H__
#define __SCENE_OBJECT_H__

#include "Matrix.h"
#include "IComparable.h"
#include "BaseLight.h"
#include "Mesh.h"
#include "OBB.h"
#include <map>

using OGE::math::Matrix;
using OGE::math::OBB;

using OGE::resource::BaseMesh;

using std::map;
using std::pair;

namespace OGE
{
	namespace scene
	{
		template<class T>
		class SceneObject : public IComparable<SceneObject<T>>
		{
		public:
			SceneObject();
			~SceneObject();

			void setWorldMatrix(const Matrix& rWorldMatrix);
			void setLocalMatrix(const Matrix& rLocalMatrix);

			void setSceneObject(const T& rObject);
			void setOBB(const OBB& rOBB);

			const Matrix& getWorldMatrix() const;
			const Matrix& getLocalMatrix() const;

			const T& getSceneObject() const;
			const OBB& getOBB() const;

			bool operator == (const SceneObject<T>& rOther);

			virtual uint32 compare(const SceneObject<T>& rOther) const;
		private:

			T object;
			Matrix worldMatrix;
			Matrix localMatrix;
			OBB obb;
		};

		template<class T>
		inline SceneObject<T>::SceneObject() {}

		template<class T>
		inline SceneObject<T>::~SceneObject() {}

		template<class T>
		inline const Matrix& SceneObject<T>::getWorldMatrix() const
		{
			return worldMatrix;
		}

		template<class T>
		inline const Matrix& SceneObject<T>::getLocalMatrix() const
		{
			return localMatrix;
		}

		template<class T>
		inline const T& SceneObject<T>::getSceneObject() const
		{
			return object;
		}

		template<class T>
		inline const OBB& SceneObject<T>::getOBB() const
		{
			return obb;
		}

		template<class T>
		inline void SceneObject<T>::setWorldMatrix(const Matrix& rWorldMatrix)
		{
			this->worldMatrix = rWorldMatrix;
		}

		template<class T>
		inline void SceneObject<T>::setLocalMatrix(const Matrix& rLocalmatrix)
		{
			this->localMatrix = rLocalmatrix;
		}

		template<class T>
		inline void SceneObject<T>::setSceneObject(const T& rObject)
		{
			this->object = rObject;
		}

		template<class T>
		inline void SceneObject<T>::setOBB(const OBB& rOBB)
		{
			this->obb = rOBB;
		}

		template<class T>
		inline uint32 SceneObject<T>::compare(const SceneObject<T>& rOther) const
		{
			if(this->object == rOther.object &&
				this->worldMatrix == rOther.worldMatrix && 
				this->localMatrix == rOther.localMatrix &&
				this->obb == rOther.obb)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}

		template<class T>
		inline bool SceneObject<T>::operator == (const SceneObject<T>& rOther)
		{
			return compare(rOther) == 0 ? false : true;
		}
	}
}

#endif