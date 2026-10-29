namespace Board {
  struct Board {
    int xy[20][20];
  };

  int setupCell(int i, int j, int value, int colorA = 8, int colorB = 9) {
    return value != 0 ? value : ((i + j) % 2 == 0 ? colorA : colorB);
  }

  Board drawBoard(int board[22][22], int figure[4][3], int nextFigure[4][3], int menu, int typing) {
    Board printBoard;
    
    for(int i = 1; i < 21; i++) {
      for(int j = 1; j < 21; j++) {
        int color;
        
        if(j < 11)
          color = setupCell(i, j, board[i][j]);
        else if(j == 11)
          color = setupCell(i, j, 0, 10, 11);
        else if(j > 11)
          color = setupCell(i, j, board[i][j], 10, 11);
        
        if(i == figure[0][1] && j == figure[0][0])
          color = figure[0][2];
        if(i == figure[1][1] && j == figure[1][0])
          color = figure[1][2];
        if(i == figure[2][1] && j == figure[2][0])
          color = figure[2][2];
        if(i == figure[3][1] && j == figure[3][0])
          color = figure[3][2];

        if(i == nextFigure[0][1] && j == nextFigure[0][0])
          color = nextFigure[0][2];
        if(i == nextFigure[1][1] && j == nextFigure[1][0])
          color = nextFigure[1][2];
        if(i == nextFigure[2][1] && j == nextFigure[2][0])
          color = nextFigure[2][2];
        if(i == nextFigure[3][1] && j == nextFigure[3][0])
          color = nextFigure[3][2];

        if((typing == 1 || menu == 11 || menu == 22) && i == 15 && j > 11 && j < 20)
          // Pause has its own neutral plaque, independent of the figure palette.
          color = menu == 22 ? 0 : 6;
        
        if(typing == 1 && i == 13 && j > 12 && j < 19)
          color = 1;
          
        printBoard.xy[i-1][j-1] = color;
      }
    }

    return printBoard;
  }

  Board drawBoard(int board[20][20], int menu, int selectedItem) {
    Board printBoard;

    for(int i = 0; i < 20; i++) {
      for(int j = 0; j < 20; j++) {
        int color = setupCell(i, j, board[i][j]);

        if((board[i][j] == 7) && (
          (selectedItem == 0 && i == 9) ||
          (selectedItem == 1 && i == 7) ||
          (selectedItem == 2 && i == 5) ||
          (selectedItem == 3 && i == 3) ||
          (selectedItem == 4 && i == 1)
        )) color = 0;
          
        if((board[i][j]== 7 && menu == 4) && (
          i == 9 || i == 7 || i == 5 || i == 3 || i == 1
        )) color = 3;
        
        printBoard.xy[i][j] = color;
      }
    }

    return printBoard;
  }

  Board drawBoard(int board[20][20]) {
    Board printBoard;

    for(int i = 0; i < 20; i++)
      for(int j = 0; j < 20; j++)
        printBoard.xy[i][j] = setupCell(i, j, board[i][j]);

    return printBoard;
  }
}
