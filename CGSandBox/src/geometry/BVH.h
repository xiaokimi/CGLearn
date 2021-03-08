#pragma once

#include "global.h"
#include "../shape/Object.h"

struct BVHNode
{
	BVHNode* left = nullptr;
	BVHNode* right = nullptr;

	std::vector<Object*> objectList;

	AABBox bbox = AABBox();
};

enum class SplitMethod
{
	NAIVE       = 0,
	SAH         = 1
};

struct BucketInfo
{
	int nCount = 0;
	AABBox box;
};

class BVH
{
public:
	BVH(std::vector<Object*> objectList, int maxPrimsInNode = 1, SplitMethod splitMethod = SplitMethod::NAIVE);
	virtual ~BVH();

	bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;

private:
	BVHNode* createBVHNode(std::vector<Object*> objectList) const;
	void deleteBVHNode(BVHNode* node) const;

	bool testHit(BVHNode* node, const Ray& ray, float tMin, float tMax, HitRecord& record) const;
private:
	BVHNode* m_Root;

	SplitMethod m_SplitMethod;
	int m_MaxPrimsInNode;
};