namespace Figures {
  /* Figures */
  const int SQUARE[4][2] = {
    {5, 20}, {5, 19}, {6, 20}, {6, 19}
  }; 

  const int LINE[4][2] = {
    {5, 20}, {5, 19}, {5, 18}, {5, 17}
  }; 

  const int L[4][2] = {
    {5, 20}, {5, 19}, {5, 18}, {6, 18}
  }; 

  const int MIRRORED_L[4][2] = {
    {6, 20}, {6, 19}, {6, 18}, {5, 18}
  }; 

  const int Z[4][2] = {
    {4, 20}, {5, 20}, {5, 19}, {6, 19}
  }; 

  const int MIRRORED_Z[4][2] = {
    {6, 20}, {5, 20}, {5, 19}, {4, 19}
  }; 

  const int NOSE[4][2] = {
    {5, 20}, {5, 19}, {5, 18}, {6, 19}
  }; 

  /* Next figures */
  const int NEXT_SQUARE[4][2] = {
    {15, 7}, {15, 6}, {16, 7}, {16, 6}
  }; 

  const int NEXT_LINE[4][2] = {
    {15, 7}, {15, 6}, {15, 5}, {15, 4}
  }; 

  const int NEXT_L[4][2] = {
    {15, 7}, {15, 6}, {15, 5}, {16, 5}
  }; 

  const int NEXT_MIRRORED_L[4][2] = {
    {16, 7}, {16, 6}, {16, 5}, {15, 5}
  }; 

  const int NEXT_Z[4][2] = {
    {14, 7}, {15, 7}, {15, 6}, {16, 6}
  }; 

  const int NEXT_MIRRORED_Z[4][2] = {
    {16, 7}, {15, 7}, {15, 6}, {14, 6}
  }; 

  const int NEXT_NOSE[4][2] = {
    {15, 7}, {15, 6}, {15, 5}, {16, 6}
  };

  /* Rotate figures */

  const int ROTATE_LINE[4][2] = {
    {-1, -2}, {0, -1}, {+1, 0}, {+2, +1}
  };

  const int ROTATE_L_0[4][2] = {
    {0, -2}, {+1, -1}, {+2, 0}, {+1, +1}
  };

  const int ROTATE_L_1[4][2] = {
    {+2, 0}, {+1, +1}, {0, +2}, {-1, +1}
  };

  const int ROTATE_MIRRORED_L_0[4][2] = {
    {-2, 0}, {-1, +1}, {0, +2}, {+1, +1}
  };

  const int ROTATE_MIRRORED_L_1[4][2] = {
    {0, -2}, {-1, -1}, {-2, 0}, {-1, +1}
  };

  const int ROTATE_Z[4][2] = {
    {+1, -1}, {0, 0}, {+1, +1}, {0, +2}
  };

  const int ROTATE_MIRRORED_Z[4][2] = {
    {-1, +1}, {0, 0}, {+1, +1}, {+2, 0}
  };

  const int ROTATE_NOSE_0[4][2] = {
    {0, -2}, {+1, -1}, {+2, 0}, {0, 0}
  };

  const int ROTATE_NOSE_1[4][2] = {
    {+2, 0}, {+1, +1}, {0, +2}, {0, 0}
  };
}
