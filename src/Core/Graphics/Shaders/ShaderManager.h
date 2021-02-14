#pragma once
#include <string>
#include <map>
#include <memory>

#include "ShaderData.h"

class ShaderManager {
public:
    ShaderManager();
    ~ShaderManager();

    std::shared_ptr<ShaderData> getShaderData(const std::string& name) { return _shaders[name]; }

    void update(float delta);

private:
    std::map<std::string, std::shared_ptr<ShaderData>> _shaders;
};
