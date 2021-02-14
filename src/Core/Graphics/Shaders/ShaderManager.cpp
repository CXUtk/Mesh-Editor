#include "ShaderManager.h"

static std::string defaultVert2Path = "Resources/Shaders/color.vs";
static std::string defaultVert4Path = "Resources/Shaders/default.vs";
static std::string defaultFragPath = "Resources/Shaders/color.frag";
static std::string spriteFragPath = "Resources/Shaders/sprite.frag";
static std::string spriteVertPath = "Resources/Shaders/sprite.vs";
static std::string fontFragPath = "Resources/Shaders/font.frag";
static std::string circleFragPath = "Resources/Shaders/circle.frag";
static std::string lightFragPath = "Resources/Shaders/light.frag";

ShaderManager::ShaderManager() {
    auto default2Shader = std::make_shared<ShaderData>(ShaderData::loadShaderProgram(defaultVert2Path, defaultFragPath));
    _shaders["line_draw"] = default2Shader;

    auto default4Shader = std::make_shared<ShaderData>(ShaderData::loadShaderProgram(spriteVertPath, defaultFragPath));
    _shaders["quad_draw"] = default4Shader;

    auto circle4Shader = std::make_shared<ShaderData>(ShaderData::loadShaderProgram(spriteVertPath, circleFragPath));
    _shaders["circle_draw"] = circle4Shader;

    auto defaultSpriteShader = std::make_shared<ShaderData>(ShaderData::loadShaderProgram(spriteVertPath, spriteFragPath));
    _shaders["sprite_draw"] = defaultSpriteShader;

    auto defaultFontShader = std::make_shared<ShaderData>(ShaderData::loadShaderProgram(spriteVertPath, fontFragPath));
    _shaders["font_draw"] = defaultFontShader;

    auto phongLightShader = std::make_shared<ShaderData>(ShaderData::loadShaderProgram(defaultVert4Path, lightFragPath));
    _shaders["light_draw"] = phongLightShader;

    auto defaultTriangleShader = std::make_shared<ShaderData>(ShaderData::loadShaderProgram(defaultVert4Path, defaultFragPath));
    _shaders["triangle_draw"] = defaultTriangleShader;
}

ShaderManager::~ShaderManager() {
}

void ShaderManager::update(float delta) {
}
