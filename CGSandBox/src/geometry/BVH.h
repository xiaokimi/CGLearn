#pragma once

#include "../shape/Object.h"

struct BVHNode
{
	BVHNode* left = nullptr;
	BVHNode* right = nullptr;

	int primsNum = 0;
	Object* objectList = nullptr;

	AABBox bbox = AABBox();
};

enum class SplitMethod
{
	NAIVE       = 0,
	SAH         = 1
};

class BVH
{
public:
	BVH(Object* objectList, int nCount, int maxPrimsInNode = 1, SplitMethod splitMethod = SplitMethod::NAIVE);
	virtual ~BVH();

	bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;

private:
	BVHNode* createBVHNode(Object* objectList, int nCount) const;
	void deleteBVHNode(BVHNode* node) const;

	bool testHit(BVHNode* node, const Ray& ray, float tMin, float tMax, HitRecord& record) const;
private:
	BVHNode* m_Root;
	SplitMethod m_SplitMethod;
	int m_MaxPrimsInNode;
};