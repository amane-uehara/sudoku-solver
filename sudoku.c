#include <stdio.h>

typedef int BOARD[9][9][9];

int fix_num(int p[9]);
void init_board(BOARD a);
void copy_board(BOARD src, BOARD dst);
void print_board(BOARD a);
int reduce_board(BOARD a);
int recursive_solver(BOARD a);
int ansatz(BOARD a);

int main(void) {
  BOARD a;
  init_board(a);

  printf("\n##### problem #####\n");
  print_board(a);

  printf("\n##### solving #####\n");
  recursive_solver(a);

  printf("\n##### answer #####\n");
  print_board(a);

  return 0;
}

int fix_num(int p[9]) {/*{{{*/
  int rest = 0;
  for (int i=0; i<9; i++) {
    rest += p[i];
  }
  if (rest > 1) return 9;

  else {
    for (int i=0; i<9; i++) {
      if (p[i] == 1) return i;
    }
  }

  return -1;
}
/*}}}*/
void init_board(BOARD a) {/*{{{*/
  FILE *fp;
  fp = fopen("problem.txt", "r");

  int x=0, y=0, c;
  while ((c = fgetc(fp)) != EOF) {
    if ('1' <= c && c <= '9') {
      for (int i=0; i<9; i++) a[x][y][i] = 0;
      a[x][y][c-'1'] = 1;
      x++;
    }

    if (c == ' ') {
      for (int i=0; i<9; i++) a[x][y][i] = 1;
      x++;
    }

    if (c == '\n') {
      x = 0;
      y++;
    }
  }

  fclose(fp);
}
/*}}}*/
void copy_board(BOARD src, BOARD dst) {/*{{{*/
  for (int x=0; x<9; x++) {
    for (int y=0; y<9; y++) {
      for (int i=0; i<9; i++) {
        dst[x][y][i] = src[x][y][i];
      }
    }
  }
}
/*}}}*/
void print_board(BOARD a) {/*{{{*/
  const char sen[] = "+===+===+===+\n";
  printf(sen);
  for (int py=0; py<3; py++) {
    for (int y=py*3; y<(py+1)*3; y++) {
      for (int px=0; px<3; px++) {
        printf("|");
        for (int x=px*3; x<(px+1)*3; x++) {
          int f = fix_num(a[x][y]);
          if (f == 9) printf(" ");
          else printf("%d", f+1);
        }
      }
      printf("|\n");
    }
    printf(sen);
  }
}
/*}}}*/
int reduce_board(BOARD a) {/*{{{*/
  // line delete
  int ret = 0;
  for (int x=0; x<9; x++) {
    for (int y=0; y<9; y++) {
      int f = fix_num(a[x][y]);
      if (0 <= f && f < 9) {
        // x line delete
        for (int tx=0; tx<9; tx++) {
          if (tx == x) continue;
          if (a[tx][y][f]) ret = 1;
          a[tx][y][f] = 0;
        }

        // y line delete
        for (int ty=0; ty<9; ty++) {
          if (ty == y) continue;
          if (a[x][ty][f]) ret = 1;
          a[x][ty][f] = 0;
        }

        // 3x3 delete
        int px = (x - (x%3))/3;
        int py = (y - (y%3))/3;
        for (int tx=px*3; tx<(px+1)*3; tx++) {
          for (int ty=py*3; ty<(py+1)*3; ty++) {
            if ((tx == x) && (ty == y)) continue;
            if (a[tx][ty][f]) ret = 1;
            a[tx][ty][f] = 0;
          }
        }
      }
    }
  }
  return ret;
}
/*}}}*/
int recursive_solver(BOARD a) {/*{{{*/
  while (reduce_board(a) > 0);

  int n = ansatz(a);
  if (n == -2) return 0;
  if (n == -1) return 1;

  int x = n%9;
  int y = ((n-x)/9)%9;
  int i = (((n-x)/9)-y)/9;
  printf("[n:%d,x:%d,y:%d,i:%d]\n", n, x, y, i);

  BOARD b;

  copy_board(a, b);
  b[x][y][i] = 0;
  if (recursive_solver(b)) copy_board(b, a);

  else {
    copy_board(a,b);
    for (int ti=0; ti<9; ti++) {
      if (ti == i) continue;
      b[x][y][ti] = 0;
    }

    if (recursive_solver(b)) copy_board(b, a);
    else return 0;
  }
  return 1;
}
/*}}}*/
int ansatz(BOARD a) {/*{{{*/
  int min_rest = 10;
  int ret = -1;
  for (int x=0; x<9; x++) {
    for (int y=0; y<9; y++) {
      int rest = 0;
      for (int i=0; i<9; i++) rest += a[x][y][i];

      if (rest == 0) return -2;
      if (rest == 1) continue;

      if (rest < min_rest) {
        min_rest = rest;
        for (int i=0; i<9; i++) {
          if (a[x][y][i]) {
            ret = x + 9*y + 9*9*i;
            break;
          }
        }
      }
    }
  }
  return ret;
}
/*}}}*/
