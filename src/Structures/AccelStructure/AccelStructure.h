#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Structures\BoundingBox.h"
#include "Structures\DCEL.h"
#include <memory>
#include "Structures\Ray.h"

struct Info {
    BoundingBox box;
    int splitAxis;
    float splitPos;
};

class AccelStructure {
public:
    AccelStructure() {}
    virtual void build(const std::vector<DCEL::const_PFace>& objects) = 0;
    virtual bool rayIntersect(const Ray& ray, IntersectionInfo& info) const = 0;
    virtual int numOfNodes() const { return 0; }

    static std::unique_ptr<AccelStructure> makeAccelStructure(const std::string& name, int maxnodes);
};
