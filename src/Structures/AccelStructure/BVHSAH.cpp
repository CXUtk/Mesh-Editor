#include "BVHSAH.h"
#include "HalfEdge/Face.h"
#include <algorithm>

#define self _nodes[p]
#define chi(p, d) _nodes[p].ch[d]
#define chd(p, d) _nodes[_nodes[p].ch[d]]


BVHSAH::BVHSAH() :AccelStructure() {
    _root = 0, _tot = 0;
    _objects.clear();

}

BVHSAH::~BVHSAH() {
}

void BVHSAH::Build(const std::vector<DCEL::const_PFace>& objects) {
    _objects.clear();
    _nodes.clear();
    _tot = 0, _root = 0;

    for (auto ptr : objects)
        _objects.push_back(ptr);

    int sz = _objects.size();

    _nodes.reserve(sz * 2);
    _nodes.push_back(BVHSAHNode());

    _build(_root, 0, sz - 1);
}



bool BVHSAH::RayIntersect(const Ray& ray, HitRecord& info) const {
    bool hit = false;
    constexpr float MAX = std::numeric_limits<float>::infinity();
    int dirIsNeg[3] = { ray.dir[0] < 0, ray.dir[1] < 0, ray.dir[2] < 0 };

    // Stack related
    int top = 0, p = _root;
    int st[64];
    while (true) {
        float tmin = 0, tmax = MAX;
        if (self.box.RayIntersect(ray, tmin, tmax) && tmin < info.t) {
            if (!self.objs.empty()) {
                HitRecord tmp;
                for (auto a : self.objs) {
                    if (a->RayInterset(ray, &tmp)) {
                        hit = true;
                        if (tmp.t < info.t) {
                            info = tmp;
                        }
                    }
                }
            }
            else {
                int d = 0;
                if (dirIsNeg[self.splitAxis]) d = 1;
                st[top++] = chi(p, !d);
                p = chi(p, d);
                continue;
            }
        }

        if (!top) break;
        p = st[--top];
    }
    return hit;
}



int BVHSAH::newNode(const std::vector<DCEL::const_PFace>& objs, const BoundingBox& box, int split) {
    ++_tot;
    _nodes.push_back(BVHSAHNode(box, objs, split));
    return _tot;
}

void BVHSAH::push_up(int p) {
    if (chi(p, 0))
        self.box = self.box.Union(chd(p, 0).box);
    if (chi(p, 1))
        self.box = self.box.Union(chd(p, 1).box);
}

void BVHSAH::_build(int& p, int l, int r) {
    BoundingBox box;
    std::vector<DCEL::const_PFace> objs;
    if (r - l + 1 <= 8) {
        for (int i = l; i <= r; i++) {
            box = box.Union(_objects[i]->GetBoundingBox());
            objs.push_back(_objects[i]);
        }
        p = newNode(objs, box, 0);
        return;
    }

    BoundingBox centerBox;
    for (int i = l; i <= r; i++)
        centerBox = centerBox.Union(_objects[i]->GetBoundingBox().GetCenter());

    int mid = (l + r) / 2;
    int split = centerBox.MaxExtent();
    auto cmp = [=](DCEL::const_PFace A, DCEL::const_PFace B) {
        return A->GetBoundingBox().GetCenter()[split] < B->GetBoundingBox().GetCenter()[split];
    };
    if (centerBox.getMaxPos()[split] == centerBox.getMinPos()[split]) {
        for (int i = l; i <= r; i++) {
            box = box.Union(_objects[i]->GetBoundingBox());
            objs.push_back(_objects[i]);
        }
        p = newNode(objs, box, 0);
        return;
    }
    if constexpr (SPLITMETHOD == EQUAL) {
        std::nth_element(_objects.begin() + l, _objects.begin() + mid, _objects.begin() + r + 1, cmp);
    }
    else {
        constexpr float tTrav = 0.1;
        float cost = std::numeric_limits<float>::infinity();
        int mincost = -1;
        sort(_objects.begin() + l, _objects.begin() + r + 1, cmp);
        std::vector<BoundingBox> suf(r - l + 2);
        suf[r - l + 1] = BoundingBox();

        for (int i = r - l; i >= 0; i--) {
            suf[i] = suf[i + 1].Union(_objects[i + l]->GetBoundingBox());
        }
        BoundingBox curbox = BoundingBox();
        for (int i = 0; i < r - l + 1; i++) {
            curbox = curbox.Union(_objects[i + l]->GetBoundingBox());
            float lArea = curbox.SurfaceArea();
            float rArea = suf[i].SurfaceArea();
            float c = tTrav + ((i + 1) * lArea + (r - l - i) * rArea) / suf[0].SurfaceArea();
            if (c < cost) {
                cost = c;
                mincost = i;
            }
        }
        mid = l + mincost;
        if (mincost == -1 || cost > (r - l + 1) * 2) {
            for (int i = l; i <= r; i++) {
                box = box.Union(_objects[i]->GetBoundingBox());
                objs.push_back(_objects[i]);
            }
            p = newNode(objs, box, 0);
            return;
        }
    }
    p = newNode(objs, box, split);
    _build(chi(p, 0), l, mid);
    _build(chi(p, 1), mid + 1, r);
    push_up(p);
}

//bool BVHSAH::ray_test(int p, const Ray& ray, HitRecord& info, float tMin, float tMax) const {
//    if (!self.box.rayIntersect(ray, tMin, tMax)) return false;
//    if (tMin >= info.t) return false;
//    bool hit = false;
//    if (!self.objs.empty()) {
//        HitRecord tmp;
//        for (auto a : self.objs) {
//            if (a->rayIntersect(ray, info)) {
//                hit = true;
//            }
//        }
//    }
//    int d = ray.dir[self.splitAxis] < 0 ? 1 : 0;
//    if (chi(p, d))
//        hit |= ray_test(chi(p, d), ray, info, tMin, info.t);
//    if (chi(p, !d))
//        hit |= ray_test(chi(p, !d), ray, info, tMin, info.t);
//    return hit;
//}
