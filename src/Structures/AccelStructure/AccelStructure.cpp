#include "AccelStructure.h"
#include "BVHSAH.h"
#include <string>

std::unique_ptr<AccelStructure> AccelStructure::makeAccelStructure(const std::string& name) {
    return std::make_unique<BVHSAH>();
}
