#include <string>

namespace I18N {
  struct I18N {
    string MENU_NEW_GAME;
    string MENU_HELP;
    string MENU_SETTINGS;
    string MENU_RESULT;
    string MENU_EXIT;

    string GAME_POINTS;
    string GAME_OVER_WRITE_NAME;
    string GAME_OVER;
    string GAME_PAUSE;

    string HELP_CONTROLS;
    string HELP_ARROWS;
    string HELP_ARROW_UP;
    string HELP_ARROW_DOWN;
    string HELP_ARROW_RIGHT;
    string HELP_ARROW_LEFT;
    string HELP_OTHERS;
    string HELP_ESC;
    string HELP_SPACE;
    string HELP_F1;
    string HELP_P;

    string SETTINGS;
    string SETTINGS_SPEED;
    string SETTINGS_LANG;
    string SETTINGS_LANG_VALUE;
  };

  I18N getPL(I18N i18n) {
    i18n.MENU_NEW_GAME = "Nowa Gra";
    i18n.MENU_HELP = "Pomoc";
    i18n.MENU_SETTINGS = "Ustawienia";
    i18n.MENU_RESULT = "Wyniki";
    i18n.MENU_EXIT = "Wyjscie";

    i18n.GAME_POINTS = "Punkty";
    i18n.GAME_OVER_WRITE_NAME = "Podaj imie i koncz [ESC]";
    i18n.GAME_OVER = "Przegrales i koncz [ESC]";
    i18n.GAME_PAUSE = "Pauza! Kliknij [P] i graj";

    i18n.HELP_CONTROLS = "Sterowanie:";
    i18n.HELP_ARROWS = "Strzalki:";
    i18n.HELP_ARROW_UP = "W gore - obrot";
    i18n.HELP_ARROW_DOWN = "W dol - opadanie";
    i18n.HELP_ARROW_RIGHT = "W prawo";
    i18n.HELP_ARROW_LEFT = "W lewo";
    i18n.HELP_OTHERS = "Inne:";
    i18n.HELP_ESC = "[ESC] Wyjscie/Powrot";
    i18n.HELP_SPACE = "[SPACJA] Wybor z Menu";
    i18n.HELP_F1 = "[F1] Pomoc";
    i18n.HELP_P = "[P] Pauza";

    i18n.SETTINGS = "Ustawienia:";
    i18n.SETTINGS_SPEED = "Predkosc [+][-]";
    i18n.SETTINGS_LANG = "Jezyk [L]";
    i18n.SETTINGS_LANG_VALUE = "Polski";
    return i18n;
  }

  I18N getEN(I18N i18n) {
    i18n.MENU_NEW_GAME = "New Game";
    i18n.MENU_HELP = "Help";
    i18n.MENU_SETTINGS = "Settings";
    i18n.MENU_RESULT = "Results";
    i18n.MENU_EXIT = "Exit";

    i18n.GAME_POINTS = "Points";
    i18n.GAME_OVER_WRITE_NAME = "Write your name [ESC]";
    i18n.GAME_OVER = "Game Over, press [ESC]";
    i18n.GAME_PAUSE = "Pause! Press [P] and play";

    i18n.HELP_CONTROLS = "Controls:";
    i18n.HELP_ARROWS = "Arrows:";
    i18n.HELP_ARROW_UP = "UP - rotate";
    i18n.HELP_ARROW_DOWN = "DOWN - fall";
    i18n.HELP_ARROW_RIGHT = "RIGHT";
    i18n.HELP_ARROW_LEFT = "LEFT";
    i18n.HELP_OTHERS = "Others:";
    i18n.HELP_ESC = "[ESC] Exit/Back";
    i18n.HELP_SPACE = "[SPACE] Confirm select";
    i18n.HELP_F1 = "[F1] Help";
    i18n.HELP_P = "[P] Pause";

    i18n.SETTINGS = "Settings:";
    i18n.SETTINGS_SPEED = "Speed [+][-]";
    i18n.SETTINGS_LANG = "Language [L]";
    i18n.SETTINGS_LANG_VALUE = "English";
    return i18n;
  }

  I18N i18n = getEN(I18N());
  bool lang = true;

  void switchLang() {
    if(lang) {
      lang = false;
      i18n = getPL(i18n);
    } else {
      lang = true;
      i18n = getEN(i18n);
    }
#ifdef __EMSCRIPTEN__
    EM_ASM({ window.TetrisI18n.setLanguage($0 ? 'en' : 'pl', true); }, lang ? 1 : 0);
#endif
  }

}
