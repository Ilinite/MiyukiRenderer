//
// Created by Shiina Miyuki on 2019/3/5.
//

#ifndef MIYUKI_SCATTERINGEVENT_H
#define MIYUKI_SCATTERINGEVENT_H

#include "core/geometry.h"
#include "core/ray.h"
#include "samplers/sampler.h"
#include "bsdfs/bsdf.h"

namespace Miyuki {
    class BSDF;

    enum class TransportMode {
        radiance,
        importance
    };

    class Medium;

    class ScatteringEvent {
    protected:
        Intersection *intersection;
        CoordinateSystem coordinateSystem;
    public:
        Point2f u;
        BSDF *bsdf;
        Vec3f wi, wo;
        Vec3f wiW, woW;
        Float pdf;
        BSDFLobe bsdfLobe;
        TransportMode mode;

        ScatteringEvent() : bsdf(nullptr), intersection(nullptr) {}

        ScatteringEvent(Sampler *, Intersection *, BSDF *, TransportMode mode);

        const Vec3f Ns() const {
            return intersection->Ns;
        }

        const Vec3f Ng() const {
            return intersection->Ng;
        }

        Vec3f localToWord(const Vec3f &w) const {
            return coordinateSystem.localToWorld(w);
        }

        Vec3f worldToLocal(const Vec3f &w) const {
            return coordinateSystem.worldToLocal(w);
        }

        void setWi(const Vec3f &w) {
            wi = w;
            wiW = localToWord(wi);
        }

        Ray spawnRay(const Vec3f &wi) const {
            Ray ray(intersection->ref, wi);
            ray.excludeGeomId = getIntersection()->geomId;
            ray.excludePrimId = getIntersection()->primId;
            return ray;
        }

        Spectrum Le(const Vec3f &wi) const;

        Intersection *getIntersection() const { return intersection; }

        const Point2f &uv() const {
            return intersection->uv;
        }

        const Point2f textureUV() const;
    };
}
#endif //MIYUKI_SCATTERINGEVENT_H
