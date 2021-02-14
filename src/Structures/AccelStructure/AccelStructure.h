#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Structures\BoundingBox.h"
#include "Structures\DCEL.h"
#include <memory>
#include "Structures\Ray.h"

class AccelStructure {
public:
    AccelStructure() {}
    virtual void Build(const std::vector<DCEL::const_PFace>& objects) = 0;
    virtual bool RayIntersect(const Ray& ray, HitRecord& hit) const = 0;
    virtual int NodesCount() const { return 0; }

    static std::unique_ptr<AccelStructure> makeAccelStructure(const std::string& name);
};
