#pragma once
#include "AccelStructure.h"
#include <vector>


enum SplitMethod {
    SAH,
    EQUAL,
};
struct BVHSAHNode {
    BoundingBox box;
    std::vector<DCEL::const_PFace> objs;
    int ch[2], splitAxis;

    BVHSAHNode() : box(), splitAxis(0) {
        ch[0] = ch[1] = 0;
    }

    BVHSAHNode(const BoundingBox& box, const std::vector<DCEL::const_PFace>& objs, int split) :box(box), objs(objs), splitAxis(split) {
        ch[0] = ch[1] = 0;
    }
};
class BVHSAH : public AccelStructure {
public:
    BVHSAH();
    ~BVHSAH();
    void build(const std::vector<DCEL::const_PFace>& objects) override;
    bool rayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    int numOfNodes() const override { return _tot; }
    int getNodes() const { return _tot; }

private:
    int _tot, _root;
    std::vector<BVHSAHNode> _nodes;
    std::vector<DCEL::const_PFace> _objects;

    int newNode(const std::vector<DCEL::const_PFace>& objs, const BoundingBox& box, int split);
    void push_up(int p);
    void _build(int& p, int l, int r);
    bool ray_test(int p, const Ray& ray, IntersectionInfo& info, float tMin, float tMax) const;

    static constexpr int SPLITMETHOD = SplitMethod::EQUAL;
};
