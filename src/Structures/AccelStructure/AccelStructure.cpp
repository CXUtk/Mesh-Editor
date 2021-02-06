#include "AccelStructure.h"
#include <string>
#include "BVHSAH.h"

std::unique_ptr<AccelStructure> AccelStructure::makeAccelStructure(const std::string& name, int maxnodes) {
    return std::make_unique<BVHSAH>();
}
