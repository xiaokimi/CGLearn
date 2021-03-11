#include "BVH.h"

BVH::BVH(std::vector<Object*> objectList, int maxPrimsInNode /*= 1*/, SplitMethod splitMethod /*= SplitMethod::NAIVE*/)
: m_MaxPrimsInNode(maxPrimsInNode)
, m_SplitMethod(splitMethod)
{
	std::vector<Object*> primitiveList;
	for (auto object : objectList)
	{
		for (auto prim : object->getPrimitiveList())
		{
			primitiveList.push_back(prim);
		}
	}

	m_Root = createBVHNode(primitiveList);
}

BVH::~BVH()
{
	deleteBVHNode(m_Root);
}

bool BVH::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	return testHit(m_Root, ray, tMin, tMax, record);
}

BVHNode* BVH::createBVHNode(std::vector<Object*> objectList) const
{
	int nCount = objectList.size();

	if (nCount == 0)
	{
		return nullptr;
	}

	Vec3f pMin = objectList[0]->getPointMin();
	Vec3f pMax = objectList[0]->getPointMax();
	for (int i = 1; i < nCount; i++)
	{
		pMin = Vec3f::min(pMin, objectList[i]->getPointMin());
		pMax = Vec3f::max(pMax, objectList[i]->getPointMax());
	}

	BVHNode* node = new BVHNode();
	node->bbox = AABBox(pMin, pMax);

	if (nCount <= m_MaxPrimsInNode)
	{
		node->objectList = objectList;
		return node;
	}
	
	AABBox centroidBox;
	for (const auto& object : objectList)
	{
		centroidBox = AABBox::getUnion(centroidBox, object->getBoundingBox().getCentroid());
	}
	int maxExtend = (centroidBox.getPointMax() - centroidBox.getPointMin()).getMaxExtend();
	std::sort(objectList.begin(), objectList.end(), [=](Object* obj0, Object* obj1) {
		return obj0->getBoundingBox().getCentroid()[maxExtend] < obj1->getBoundingBox().getCentroid()[maxExtend];
	});

	int middleIndex = 0;
	switch (m_SplitMethod)
	{
	case SplitMethod::NAIVE:
	{
		middleIndex = nCount / 2;
		break;
	}
	case SplitMethod::SAH:
	{
		constexpr int nBuckets = 12;
		BucketInfo buckets[nBuckets];

		for (const auto& object : objectList)
		{
			int b = nBuckets * centroidBox.getOffset(object->getBoundingBox().getCentroid())[maxExtend];

			b = b < 0 ? 0 : b;
			b = b > nBuckets - 1 ? nBuckets - 1 : b;

			buckets[b].nCount++;
			buckets[b].box = AABBox::getUnion(buckets[b].box, object->getBoundingBox());
		}

		float cost[nBuckets - 1];
		float invMaxArea = 1.0f / node->bbox.getSurfaceArea();

		for (int i = 0; i < nBuckets - 1; i++)
		{
			AABBox box0, box1;
			int count0 = 0, count1 = 0;

			for (int j = 0; j <= i; j++)
			{
				box0 = AABBox::getUnion(box0, buckets[j].box);
				count0 += buckets[j].nCount;
			}

			for (int j = i + 1; j < nBuckets; j++)
			{
				box1 = AABBox::getUnion(box1, buckets[j].box);
				count1 += buckets[j].nCount;
			}

			cost[i] = 1 + (count0 * box0.getSurfaceArea() + count1 * box1.getSurfaceArea()) * invMaxArea;
		}

		float minCost = cost[0];
		int minSplitBucket = 0;

		for (int i = 1; i < nBuckets - 1; i++)
		{
			if (cost[i] < minCost)
			{
				minCost = cost[i];
				minSplitBucket = i;
			}
		}

		for (int i = 0; i <= minSplitBucket; i++)
		{
			middleIndex += buckets[i].nCount;
		}

		break;
	}
	default:
		break;
	}

	if (middleIndex == 0 || middleIndex == nCount)
	{
		middleIndex = nCount / 2;
	}

	std::vector<Object*> leftObjectList = std::vector<Object*>(objectList.begin(), objectList.begin() + middleIndex);
	std::vector<Object*> rightObjectList = std::vector<Object*>(objectList.begin() + middleIndex, objectList.end());

	node->left = createBVHNode(leftObjectList);
	node->right = createBVHNode(rightObjectList);
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

	if (node->objectList.size() > 0)
	{
		bool hitAnything = false;
		HitRecord tmpRecord;

		for (const auto& object : node->objectList)
		{
			if (object->hit(ray, tMin, tMax, tmpRecord))
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
