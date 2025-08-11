#pragma once
#include "solid.h"
#include <memory>

using std::make_shared;
using std::shared_ptr;

class Scene : public Solid {
    public:
        std::vector<shared_ptr<solid>> objects;

        Scene();
        Scene(shared_ptr < solid >> object);

        void clear();
        void add(std::shared_ptr<solid> object);

    private:
        aabb bbox;
}
