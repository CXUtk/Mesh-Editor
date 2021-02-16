#pragma once
#include <algorithm>
#include <glm/glm.hpp>
#include "Structures/Ray.h"



class BoundingBox {
public:
    BoundingBox();
    BoundingBox(const glm::vec3& minPos, const glm::vec3& maxPos);
    ~BoundingBox();

    glm::vec3 getMinPos()const { return _minPos; }
    glm::vec3 getMaxPos()const { return _maxPos; }

    bool RayIntersect(const Ray& ray, float& tMin, float& tMax) const;

    bool Intersects(const BoundingBox& box) const;

    glm::vec3 GetCenter() const { return (_minPos + _maxPos) / 2.f; }

    BoundingBox Union(const BoundingBox& box) const {
        glm::vec3 minn(std::min(_minPos.x, box._minPos.x), std::min(_minPos.y, box._minPos.y), std::min(_minPos.z, box._minPos.z));
        glm::vec3 maxx(std::max(_maxPos.x, box._maxPos.x), std::max(_maxPos.y, box._maxPos.y), std::max(_maxPos.z, box._maxPos.z));
        return BoundingBox(minn, maxx);
    }

    BoundingBox Union(const glm::vec3& pos) const {
        glm::vec3 minn(std::min(_minPos.x, pos.x), std::min(_minPos.y, pos.y), std::min(_minPos.z, pos.z));
        glm::vec3 maxx(std::max(_maxPos.x, pos.x), std::max(_maxPos.y, pos.y), std::max(_maxPos.z, pos.z));
        return BoundingBox(minn, maxx);
    }

    BoundingBox Intersection(const BoundingBox& box) const {
        glm::vec3 minn(std::max(_minPos.x, box._minPos.x), std::max(_minPos.y, box._minPos.y), std::max(_minPos.z, box._minPos.z));
        glm::vec3 maxx(std::min(_maxPos.x, box._maxPos.x), std::min(_maxPos.y, box._maxPos.y), std::min(_maxPos.z, box._maxPos.z));
        return BoundingBox(minn, maxx);
    }

    int MaxExtent() const {
        float t = _maxPos[0] - _minPos[0];
        int d = 0;
        if (_maxPos[1] - _minPos[1] > t) d = 1;
        if (_maxPos[2] - _minPos[2] > t) d = 2;
        return d;
    }

    float SurfaceArea() const {
        float area = 0;
        for (int i = 0; i < 3; i++) {
            area += 2 * (_maxPos[i] - _minPos[i]) * (_maxPos[(i + 1) % 3] - _minPos[(i + 1) % 3]);
        }
        return area;
    }

private:
    glm::vec3 _minPos, _maxPos;
};
