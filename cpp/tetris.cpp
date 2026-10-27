#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <GL/gl.h>
#ifndef __EMSCRIPTEN__
#include <GL/glu.h>
#else
#include <emscripten.h>
#endif
#include <GL/glut.h>
#include <iostream>
#include <ctime>
#include <cstring>
#include <functional>
#include "utils.cpp"
#include "file.cpp"
#include "texts.cpp"
#include "board.cpp"
#include "engine.cpp"

enum MENU {
  MAIN_MENU = 0,
  GAME = 1,
  HELP = 2,
  SETTINGS = 3,
  RESULTS = 4,
  GAME_OVER_WITH_RESULT_SAVE = 10,
  GAME_OVER = 11,
  PAUSE = 22,
};

GLint speed = 1000;

void ExitGame() {
#ifdef __EMSCRIPTEN__
  emscripten_run_script("window.location.reload()");
#else
  exit(0);
#endif
}

int menu = MAIN_MENU;
int selectedMenuItem = 0;

int points = 0;

char playerName[64];
int playerNameLength = 0;
int typing = 0;

int resultPoints[6] = {0,0,0,0,0,0};
char resultNames[6][64];
char resultCPoints[6][64];

int figure[4][3];
int nextFigure[4][3];

int figureCode = 0;
int nextFigureCode = 0;

int board[22][22] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,5,5,5,5,5,5,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,3,3,3,3,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0}
};

int list[20][20] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,7,7,7,7,7,7,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,7,7,7,7,7,7,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,7,7,7,7,7,7,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,7,7,7,7,7,7,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,7,7,7,7,7,7,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,0,2,2,2,0,3,0,4,0,4,4,0,5,0,6,6,0},
  {0,0,1,0,2,0,0,0,3,0,4,0,4,0,0,5,0,0,6,0},
  {0,0,1,0,2,2,0,0,3,0,4,4,4,4,0,5,0,0,6,0},
  {0,0,1,0,2,0,0,0,3,0,4,0,0,4,0,5,0,6,6,0},
  {0,0,1,0,2,2,2,0,3,0,4,4,4,4,0,5,0,6,0,0},
  {0,0,1,0,0,0,0,0,3,0,0,0,0,0,0,5,0,6,0,0},
  {0,1,1,1,1,1,0,3,3,3,3,3,3,3,0,5,0,6,6,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

int popover[20][20] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,3,3,3,3,3,3,5,5,5,5,5,5,5,0,0,0,0,0},
  {0,0,3,3,3,3,3,3,5,5,5,5,5,5,5,0,0,0,0,0},
  {0,0,3,3,3,3,3,3,5,5,5,5,5,5,5,0,0,0,0,0},
  {0,0,3,3,3,3,3,3,5,5,5,5,5,5,5,0,0,0,0,0},
  {0,0,3,3,3,3,3,3,5,5,5,5,5,5,5,0,0,0,0,0},
  {0,0,3,3,3,3,3,3,5,5,5,5,5,5,5,0,0,0,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,0,2,2,2,0,3,0,4,0,4,4,0,5,0,6,6,0},
  {0,0,1,0,2,0,0,0,3,0,4,0,4,0,0,5,0,0,6,0},
  {0,0,1,0,2,2,0,0,3,0,4,4,4,4,0,5,0,0,6,0},
  {0,0,1,0,2,0,0,0,3,0,4,0,0,4,0,5,0,6,6,0},
  {0,0,1,0,2,2,2,0,3,0,4,4,4,4,0,5,0,6,0,0},
  {0,0,1,0,0,0,0,0,3,0,0,0,0,0,0,5,0,6,0,0},
  {0,1,1,1,1,1,0,3,3,3,3,3,3,3,0,5,0,6,6,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

namespace Support {
  void renderBoard(Board::Board board) {
    Engine::renderBoard(board.xy);
  }

  void renderTexts(vector<Texts::Text> texts) {
    for (size_t i = 0; i < texts.size(); i++)
      Engine::renderText(texts[i].value, texts[i].x, texts[i].y);
  }
}

void ReadResults() {
  File::readResultsFile(resultNames, resultPoints, resultCPoints);
}

void SaveResults() {
  File::saveResultFile(resultNames, resultPoints, playerName, points);
  ReadResults();
  playerName[0] = 0;
  playerNameLength = 0;
  menu = RESULTS;
}

void GameOver() {
  if(points>resultPoints[5]) menu = GAME_OVER_WITH_RESULT_SAVE;
  else menu = GAME_OVER;
}

void SetNextFigure() {
  nextFigureCode = rand()%(0-7)+0;
  Utils::drawNextFigure(nextFigure, nextFigureCode);  
}

void SetFigure() {
  figureCode = nextFigureCode;
  Utils::drawFigure(figure, figureCode);

  if(Utils::isCanNotDraw(board, figure)) GameOver();
}

void RotateFigure() {
  figureCode = Utils::drawRotate(board, figure, figureCode);
}

void MoveFigureDown() {
  if(Utils::moveFigureY(board, figure)) {
    SetFigure();
    SetNextFigure();
  }    
}

void MoveFigureLeft() {
  Utils::moveFigureX(board, figure, -1);
}

void MoveFigureRight() {
  Utils::moveFigureX(board, figure, +1);
}

void DeleteLines() {
  points += Utils::deleteLine(board);
}

void RenderBoard() {
  Support::renderBoard(Board::drawBoard(board, figure, nextFigure, menu, typing));
}

void RenderPopover() {
  Support::renderBoard(Board::drawBoard(popover));
}

void RenderList() {    
  Support::renderBoard(Board::drawBoard(list, menu, selectedMenuItem));
}

void RenderGameTexts() {
  Support::renderTexts(Texts::GameTexts(menu, resultNames[1], resultCPoints[1], points));
}

void NewGame() {
  Utils::clearBoard(board);
  points = 0;
  SetNextFigure();
  SetFigure();   
  SetNextFigure(); 
}

void RenderSaveTexts() {
  typing = 1;
  Support::renderTexts(Texts::SaveTexts(playerName));
}

#ifdef __EMSCRIPTEN__
EM_JS(void, updateWebNameForm, (int active), {
  const form = document.getElementById('name-form');
  if (form.hidden === Boolean(active)) {
    form.hidden = !active;
    if (active) document.getElementById("player-name").value = "";
  }
});
#endif

void MainLoop() {
#ifdef __EMSCRIPTEN__
  updateWebNameForm(menu == GAME_OVER_WITH_RESULT_SAVE);
#endif
  if(menu == MAIN_MENU) {
    RenderList();
    Support::renderTexts(Texts::MainMenuTexts());
  }

  if(menu == GAME) {
    DeleteLines();
    RenderBoard();
    RenderGameTexts();
  }
  
  if(menu == HELP) {
    RenderPopover();
    Support::renderTexts(Texts::HelpTexts());
  }
  
  if(menu == SETTINGS) {
    RenderPopover();
    Support::renderTexts(Texts::SettingsTexts(speed));
  }
  
  if(menu == RESULTS) {
    RenderList();
    Support::renderTexts(Texts::BestResultsTexts(resultNames, resultCPoints));
  }
  
  if(menu == GAME_OVER_WITH_RESULT_SAVE) {
    RenderBoard();
    RenderGameTexts();
    RenderSaveTexts();
  }
  
  if(menu == GAME_OVER) {
    RenderBoard();
    RenderGameTexts();
  }

  if(menu == PAUSE) {
    RenderBoard();
    RenderGameTexts();
    Support::renderTexts(Texts::PauseTexts());
  }
}

void Timer() {   
  if(menu == GAME) MoveFigureDown();
}

void Keyboard(unsigned char key, int x, int y) {   
  ReadResults();

  if((key == 'P' || key == 'p') && menu == GAME) {
    menu = PAUSE;
    key = 0;
  }

  if((key == 'P' || key == 'p') && menu == PAUSE) {
    menu = GAME;
  }

  if(key == '+' && speed<2000) {
    speed+=100;
  }

  if(key == '-' && speed>100) {
    speed-=100;
  }

  if(key == 'L' || key == 'l') {
    I18N::switchLang();
  }

  if(key == ' ' && menu == MAIN_MENU) {
    switch(selectedMenuItem) {  
      case 0:
        menu = GAME;
        NewGame();
        break; 
      case 1:
        menu = HELP;
        break; 
      case 2:
        menu = SETTINGS;
        break; 
      case 3:
        menu = RESULTS;
        break; 
      case 4:
        ExitGame();
        break;
    }
  }
  
  if(typing == 1 
      && ((key >= 48 && key <= 57) || (key >= 65 && key <= 90) || (key >= 97 && key <=122)) 
      && playerNameLength <=10 ) {

      playerName[playerNameLength] = key;
      playerNameLength++;
  }
  
  if(typing == 1 && key == 8 && playerNameLength > 0) {
      playerNameLength--;
      playerName[playerNameLength] = 0;
  }
  
  if(key == 27 && typing == 1) {
      menu = MAIN_MENU;
      typing = 0;
      key = 0;
      SaveResults();
  }
  else if(key == 27 && menu == GAME_OVER) {
      menu = RESULTS;
  }
  else if(key == 27 && menu == MAIN_MENU) {
      ExitGame();
  }
  else if(key == 27) {
      menu = MAIN_MENU;
  }
}

void SpecialKeys(int key, int x, int y) { 
  if(key == GLUT_KEY_F1 && menu == GAME) {
    menu = HELP;
    key = 0;
  }
  
  if(key == GLUT_KEY_F1 && menu == HELP) {
    menu = GAME;
    key = 0;
  }

  if(menu == GAME) {
    switch(key) {  
      case GLUT_KEY_UP:
        RotateFigure();
        break; 
      case GLUT_KEY_DOWN:
        MoveFigureDown();
        break;  
      case GLUT_KEY_LEFT:
        MoveFigureLeft();
        break; 
      case GLUT_KEY_RIGHT:
        MoveFigureRight();
        break; 
      case GLUT_KEY_END:
        menu = MAIN_MENU;
        key = 0;      
        break; 
    }
  }
  if(menu == MAIN_MENU) {
    switch(key) {  
      case GLUT_KEY_DOWN:
        if(selectedMenuItem == 4) selectedMenuItem = 0;
        else selectedMenuItem++;
        break; 
      case GLUT_KEY_UP:
        if(selectedMenuItem == 0) selectedMenuItem = 4;
        else selectedMenuItem--;
        break;
      case GLUT_KEY_END:
        ExitGame();
        break; 
    }
  }
}

#ifdef __EMSCRIPTEN__
extern "C" {
  EMSCRIPTEN_KEEPALIVE void web_key(int key, int special) {
    if (special) SpecialKeys(key, 0, 0);
    else Keyboard(static_cast<unsigned char>(key), 0, 0);
    glutPostRedisplay();
  }

  EMSCRIPTEN_KEEPALIVE void web_name_char(int key, int reset) {
    if (menu != GAME_OVER_WITH_RESULT_SAVE) return;
    if (reset) {
      playerNameLength = 0;
      playerName[0] = 0;
    } else if (playerNameLength < 11 &&
        ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') ||
         (key >= '0' && key <= '9'))) {
      playerName[playerNameLength++] = static_cast<char>(key);
      playerName[playerNameLength] = 0;
    }
  }
}
#endif

int main(int argc, char * argv[]) {   
  ReadResults();
  srand(time(NULL));
  Engine::Init(argc, argv, &speed, MainLoop, Timer, Keyboard, SpecialKeys);
  return 0;
}
