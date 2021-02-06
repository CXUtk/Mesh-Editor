#include "BoundingBox.h"
BoundingBox::BoundingBox() : _minPos(glm::vec3(std::numeric_limits<float>::infinity())), _maxPos(glm::vec3(-std::numeric_limits<float>::infinity())) {
}
BoundingBox::BoundingBox(const glm::vec3& minPos, const glm::vec3& maxPos) : _minPos(minPos), _maxPos(maxPos) {
}

BoundingBox::~BoundingBox() {
}

bool BoundingBox::rayIntersect(const Ray& ray, float& tMin, float& tMax) const {
    glm::vec3 start = ray.start, dir = ray.dir;
    for (int i = 0; i < 3; i++) {
        auto invD = 1.0f / dir[i];
        auto t0 = (_minPos[i] - start[i]) * invD;
        auto t1 = (_maxPos[i] - start[i]) * invD;
        if (invD < 0) std::swap(t0, t1);
        tMin = std::max(tMin, t0);
        tMax = std::min(tMax, t1);
        if (tMax < tMin) return false;
    }
    return true;
}

bool BoundingBox::intersects(const BoundingBox& box) const {
    for (int i = 0; i < 3; i++)
        if (std::max(_minPos[i], box.getMinPos()[i]) > std::min(_maxPos[i], box.getMaxPos()[i])) return false;
    return true;
}
