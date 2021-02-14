#include <glm/glm.hpp>
#include <iostream>

#include "Editor.h"

int main(int argc, char** argv) {
    try {
        Editor& editor = Editor::GetInstance();
        editor.Init();
        editor.Run();
    }
    catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }
    int x;
    std::cin >> x;
}
