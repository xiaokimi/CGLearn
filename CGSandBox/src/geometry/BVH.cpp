#include "cgpch.h"
#include "BVH.h"

BVH::BVH(Object* objectList, int nCount, int maxPrimsInNode /*= 1*/, SplitMethod splitMethod /*= SplitMethod::NAIVE*/)
: m_MaxPrimsInNode(maxPrimsInNode)
, m_SplitMethod(splitMethod)
{
	m_Root = createBVHNode(objectList, nCount);
}

BVH::~BVH()
{
	deleteBVHNode(m_Root);
}

bool BVH::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	return testHit(m_Root, ray, tMin, tMax, record);
}

BVHNode* BVH::createBVHNode(Object* objectList, int nCount) const
{
	if (!objectList || nCount == 0)
	{
		return nullptr;
	}

	Vec3f pMin = objectList[0].getPointMin();
	Vec3f pMax = objectList[0].getPointMax();
	for (int i = 1; i < nCount; i++)
	{
		pMin = Vec3f::min(pMin, objectList[i].getPointMin());
		pMax = Vec3f::max(pMax, objectList[i].getPointMax());
	}

	BVHNode* node = new BVHNode();
	node->bbox = AABBox(pMin, pMax);

	if (nCount <= m_MaxPrimsInNode)
	{
		node->objectList = objectList;
		node->primsNum = nCount;
		return node;
	}
	
	Vec3f centroid = node->bbox.getCentroid();
	switch (centroid.getMaxExtend())
	{
	case 0:
	{
		qsort(objectList, nCount, sizeof(Object*), [](const void* a, const void* b) {
			Object* obj0 = (Object*)a;
			Object* obj1 = (Object*)b;
			if (obj0->getBoundingBox().getCentroid()[0] < obj1->getBoundingBox().getCentroid()[0])
			{
				return 1;
			}

			return 0;
		});
		break;
	}
	case 1:
	{
		qsort(objectList, nCount, sizeof(Object*), [](const void* a, const void* b) {
			Object* obj0 = (Object*)a;
			Object* obj1 = (Object*)b;
			if (obj0->getBoundingBox().getCentroid()[1] < obj1->getBoundingBox().getCentroid()[1])
			{
				return 1;
			}

			return 0;
		});
		break;
	}
	case 2:
	{
		qsort(objectList, nCount, sizeof(Object*), [](const void* a, const void* b) {
			Object* obj0 = (Object*)a;
			Object* obj1 = (Object*)b;
			if (obj0->getBoundingBox().getCentroid()[2] < obj1->getBoundingBox().getCentroid()[2])
			{
				return 1;
			}

			return 0;
		});
		break;
	}
	default:
		break;
	}

	node->left = createBVHNode(objectList, nCount / 2);
	node->right = createBVHNode(objectList + nCount / 2, nCount - nCount / 2);
	return node;
}

void BVH::deleteBVHNode(BVHNode* node) const
{
	if (node)
	{
		deleteBVHNode(node->left);
		deleteBVHNode(node->right);

		delete node;
		node = nullptr;
	}
}

bool BVH::testHit(BVHNode* node, const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	if (!node || !node->bbox.hit(ray, tMin, tMax))
	{
		return false;
	}

	if (node->objectList)
	{
		bool hitAnything = false;
		HitRecord tmpRecord;

		for (int i = 0; i < node->primsNum; i++)
		{
			if (node->objectList[i].hit(ray, tMin, tMax, tmpRecord))
			{
				hitAnything = true;
				tMax = tmpRecord.t;

				record = tmpRecord;
			}
		}

		return hitAnything;
	}

	HitRecord leftRecord, rightRecord;
	bool hitLeft = testHit(node->left, ray, tMin, tMax, leftRecord);
	bool hitRight = testHit(node->right, ray, tMin, tMax, rightRecord);

	if (hitLeft && hitRight)
	{
		record = leftRecord.t < rightRecord.t ? leftRecord : rightRecord;
		return true;
	}
	else if (hitLeft)
	{
		record = leftRecord;
		return true;
	}
	else if (hitRight)
	{
		record = rightRecord;
		return true;
	}

	return false;
}
