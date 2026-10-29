// Test build only: production tetris.cpp and its random generator stay unchanged.
#define main tetris_main
#include "../cpp/tetris.cpp"
#undef main
extern "C" EMSCRIPTEN_KEEPALIVE void readability_fixture(int code, int state) {
  NewGame();
  Utils::drawFigure(figure, code);
  for (int i = 0; i < 4; ++i) figure[i][1] -= 4;
  Utils::drawNextFigure(nextFigure, code);
  menu = state;
  glutPostRedisplay();
}
int main(int argc, char **argv) { return tetris_main(argc, argv); }
