#include <stdio.h>
#include <stdlib.h>
#define X 82
#define Y 27

int move_x(int ball_x, int ball_y, int ugol_x, int right_rocket, int left_rocket);  // движение мяча по оси Х
int move_y(int ball_x, int ball_y, int ugol_y);  // движение мячи по оси Y
// расчет угла движения мяча при столкновении со стенками, потолком, пола
// перезапуск шарика в начало игры в центр
int angle();
// расчет траектории движения ракеток
int move_rocket(int key, int center_of_rocket, int check_players_turn);
int read_user_input_from_keyboard();  // считывание нажатия клавиши игроками
//  отрисовка элементов на экране
void draw(int ball_x, int ball_y,
int left_rocket, int right_rocket,
int score_player_one, int score_player_two);

int main(void) {
  int ball_x = X / 2;  // позиция шарика в центре по x
  int ball_y = (Y - 1) / 2;  // позиция шарика в центре по y
  int left_rocket = (Y - 1) / 2;  // координаты центра левой ракетки на старте
  int right_rocket = (Y - 1) / 2;  // координаты центра правой ракетки на старте
  int ugol_x = 1;  // проекция вектора движения на x на старте
  int ugol_y = 0;  // прекция вектора движения по y на старте (изначально (1; 1) - правый верхний угол)
  int score_player_one = 0;  // счет первого игрока
  int score_player_two = 0;  // счет второго игрока
  int check_players_turn = 1;  // проверка хода - нечетные ходы у левого, четные у правого
  // кадый раз отрисовка поля
  draw(ball_x, ball_y, left_rocket, right_rocket, score_player_one, score_player_two);
  int read_key = read_user_input_from_keyboard();  // сразу считываем первую нажатую клавишу

  while (read_key != -1) {  // считывание клавиши до того, как нажмем q и игра остановится
        printf("\e[H\e[2J\e[3J");  // очистка экрана
        // кадый раз отрисовка поля
    if (read_key != 1000) {  // если нажатая клавиша не пробел
      if (check_players_turn % 2 == 1) {  // если ход нечетный - движется левая ракетка
      // ДВИЖЕНИЕ РАКЕТКИ вызываем функцию ракетки,
      // передаем клавишу/текущий центр ракетки /счетчик четности хода
        left_rocket = move_rocket(read_key, left_rocket, check_players_turn);
        check_players_turn++;  // увеличиваем счетчик хода, ход становится четным
      } else {  // если ход четный - движется правая ракетка
      // ДВИЖЕНИЕ РАКЕТКИ вызываем функцию ракетки,
      // передаем клавишу/текущий центр ракетки /счетчик четности хода
        right_rocket = move_rocket(read_key, right_rocket, check_players_turn);
        check_players_turn++;  // увеличиваем счетчик хода, ход становится нечетным
      }
      if ((ball_x == 2) &&
      ((ball_y == left_rocket) ||
      (ball_y == (left_rocket + 1)) ||
      (ball_y == (left_rocket - 1)))) {
        // новая задача угла МЯЧИКА если сталкиваемся с левой ракеткой
        ugol_x = 1;  // меняем направление движения, теперь направляемся вправо
      } else if (ball_x == (X - 3) &&
      ((ball_y == right_rocket) ||
      (ball_y == (right_rocket + 1)) ||
      (ball_y == (right_rocket - 1)))) {
        // новая задача угла МЯЧИКА если сталкиваемся с правой ракеткой
        ugol_x = 0;  // меняем направление движения, теперь движемся влево
      }

      ball_y = move_y(ball_x, ball_y, ugol_y);  // получаем текущую координату МЯЧИКА по y
      // получаем текущую координату МЯЧИКА по x
      ball_x = move_x(ball_x, ball_y, ugol_x, right_rocket, left_rocket);
      if (ball_y == 1) {  // если сталкиваемся с потолком
        ugol_y = 0;  // меняем направление движения, теперь направляемся вниз
      } else if (ball_y == (Y - 2)) {  // если сталкиваемся с полом
        ugol_y = 1;  // меняем направление, теперь движемся вверх
      }
      if (ball_x == (X - 2)) {  // если врезаемся в правую стену
        score_player_one++;  // добавляем балл левому игроку
        ugol_x = angle();  // рандомно меняем проекцию вектора по x перед возвращением в центр
        ugol_y = angle();  // рандомное меняем проекцию вектора по y перед возвраением в центр
      } else if (ball_x == 1) {  //  если врезаемся в левую стену
        score_player_two++;  // добавляем балл правому игроку
        ugol_x = angle();  // рандомно меняем проекцию вектора по x перед возвращением в центр
        ugol_y = angle();  // рандомное меняем проекцию вектора по y перед возвраением в центр
      }
      if (score_player_one == 21) {  // если счет 21 достигнут одним из игроков
        draw(ball_x, ball_y, left_rocket, right_rocket, score_player_one, score_player_two);
        printf("Игра окончена. Победила левая ракетка");
        break;
      } else if (score_player_two == 21) {
        draw(ball_x, ball_y, left_rocket, right_rocket, score_player_one, score_player_two);
        printf("Игра окончена. Победила правая ракетка");
        break;
      }
      }
      draw(ball_x, ball_y, left_rocket, right_rocket, score_player_one, score_player_two);
      read_key = read_user_input_from_keyboard();
      // считывание новой клавиши, while начинаеся сначала
  }
  return 0;  // конец main
  }

int move_x(int ball_x, int ball_y, int ugol_x, int right_rocket, int left_rocket) {
  // функция движения шарика по x, меняет координату x передаем
  int new_position_ball_x = ball_x;  // временное хранилище положения по x
  if (ball_x == 2 && ((ball_y == left_rocket) ||
    (ball_y == left_rocket + 1) ||
    (ball_y == left_rocket - 1))) {
      // если касается левой ракетки
      new_position_ball_x++;  // отскок вправо
    } else if ((ball_x == (X - 3)) && ((ball_y == right_rocket) ||
     (ball_y == right_rocket + 1)
     || (ball_y == right_rocket - 1))) {
      // если касается правой ракеки
      new_position_ball_x--;  // отскок влево
     } else {
      if (ball_x == (X - 2)) {  // если шарик коснулся правой стенки, генерируем его в центр
      new_position_ball_x = X / 2;  // абсцисса центра - кординатора центра
      } else if (ball_x == 1) {  // если шарик коснулся левой стенки, генерируем его в центр
      new_position_ball_x = X / 2;  // абсцисса центра - кординатора центра
      } else {
        if (ugol_x == 0) {  // если до этого в прошлой итерации проекция вектора движения на х была влево
        new_position_ball_x--;  // продолжаем лететь влево
      } else {  // если до этого в прошлой итерации проекция вектора движения по х была вправо
        new_position_ball_x++;  // продолжаем лететь вправо
      }
      }
     }
  return new_position_ball_x;  // возвращаем новую координату по х
}

int move_y(int ball_x, int ball_y, int ugol_y) {  // функция движения шарика по y, меняет координату y
  int new_position_ball_y = ball_y;  // временное хранилище положения по y
  if (ball_x == (X - 2)) {  // если шарик коснулся правой стенки, генерируем его в центр
    new_position_ball_y = Y / 2;  // ордината центра
  } else if (ball_x == 1) {  // если шарик коснулся левой стенки, генерируем его в центр
    new_position_ball_y = Y / 2;  // ордината центра
  } else {  // если шарик на поле и не касается в ближашее время боковых стенок
    if (ball_y == 1) {  // если касается потолка
      new_position_ball_y++;  // отскок вниз
    } else if (ball_y == (Y - 2)) {  // если касается пола
      new_position_ball_y--;  // осткок вверх
    } else {  // если не касается ни пола, ни потолка, ни боковых стенок
      if (ugol_y == 0) {
        // если до этого в прошлой итерации проекция вектора движения на y была вниз
        new_position_ball_y++;  // продолжаем лететь вниз
      } else {
        // если до этого в прошлой итерации проекция вектора движения по y была вверх
        new_position_ball_y--;  // продолжаем лететь вверх
      }
    }
  }
  return new_position_ball_y;  // возвращаем текущую координату по y
}

int angle() {  // генерация случайного числа из отрезка [0; 1];
  int random = (rand() % 2);
  return random;
}

int move_rocket(int key, int center_of_rocket, int check_players_turn) {
  // движение ракетки - на входе нажатая клавиша, координата текущего центра ракетки, счетчик хода
  if (check_players_turn % 2 == 1) {  // если ход нечетный - левая ракетка
    if ((center_of_rocket > 2) && ((center_of_rocket) < (Y - 3))) {
      // если нет касания потолка и пола, ракетка спокойной ездит вверх-вниз
      if (key == 1) {  // если нажата А
        center_of_rocket--;  // движемся вверх
      } else if (key == 2) {  // если нажата Z
        center_of_rocket++;  // движемся вниз
      }
    }
    if ((center_of_rocket) == 2)  {
      // если касание потолка верхним краем ракетки произошло
      if (key == 2) {
        // если нажата клавиша Z - без проблем идем вниз;
        // если упорно пытаемся пробить потолок, нажимая A, ничего не происходит
        center_of_rocket++;
    }
    }
    if ((center_of_rocket) == (Y - 3)) {
      // если касание пола нижним краем ракетки произошло
      if (key == 1) {
        // если нажата клавиша A - без проблем идем вверх;
        // если упорно пытаемся пробить пол, нажимая Z, ничего не происходит
        center_of_rocket--;
      }
    }
  } else if (check_players_turn % 2 == 0) {  // если ход четный - правая ракетка
    if ((center_of_rocket > 2) && ((center_of_rocket) < (Y - 3))) {
  //        if (((center_of_rocket - 1) > 1) && ((center_of_rocket + 1) < (Y - 2))) {

      // если нет касания потолка и пола, ракетка спокойной ездит вверх-вниз
      if (key == 3) {  // если нажата K
        center_of_rocket--;  // движемся вверх
      } else if (key == 4) {  // если нажата M
        center_of_rocket++;  // движемся вниз
      }
    }
    if (center_of_rocket == 2)  {  // если касание потолка верхним краем ракетки произошло
      if (key == 4) {
      // если нажата клавиша М - без проблем идем вниз;
      // если упорно пытаемся пробить потолок, нажимая К, ничего не происходит
        center_of_rocket++;
    }
    }
    if (center_of_rocket == (Y - 3)) {  // если касание пола нижним краем ракетки произошло
    // если нажата клавиша К - без проблем идем вверх;
    // если упорно пытаемся пробить пол, нажимая М, ничего не происходит
      if (key == 3) {
        center_of_rocket--;
      }
    }
  }
  return center_of_rocket;  // возвращаем текущую координату центра
}

int read_user_input_from_keyboard() {  // преобразуем считанную букву в цифру
  int i = 9;  // начальное рандомное значение счетчика
  char c = getchar();  // получаем с клавиатуры букву
  if (c == ' ') {  // пропуск хода, мячик двигается, а ракетки нет
    i = 0;
  }
  if (c == 'a') {  // левая ракетка вверх
    i = 1;
  }
  if (c == 'z') {  // левая ракетка вниз
    i = 2;
  }
  if (c == 'k') {  // правая ракетка вверх
    i = 3;
  }
  if (c == 'm') {  // правая ракетка вниз
    i = 4;
  }
  if (c == 'q') {  // выход из игры
    i = -1;
  }
  if (c == '\n') {  // чтобы поле не отрисовываось после нажатия enter
    i = 1000;
  }
  printf("%d", i);  // проверка нажатой клавиши, ПОСЛЕ ТЕСТОВ УДАЛИТЬ!
  return i;
}

void draw(int ball_x, int ball_y,
int left_rocket, int right_rocket,
int score_player_one, int score_player_two) {
    for (int y = 0; y < Y; y++) {
        for (int x = 0; x < X; x++) {
            int left_rocket_x = 1;  // колонка левой ракетки
            int right_rocket_x = X - 2;  // колонка правой ракетки
            int score_position_X_for_player_one = (X / 2) - 2;  // позиция по Х для счета первого игрока
            int score_position_X_for_player_two = (X / 2) + 1;  // позиция по Х для счета второго игрока
            // позиция по Y для счета первого и второго игрока
            int score_position_Y_for_player_one_and_player_two = 0;

            // условие отрисовки первой цифры счета первого игрока
            if ((x == score_position_X_for_player_one) &&
            (y == score_position_Y_for_player_one_and_player_two)) {
                int digit_one = score_player_one / 10;
                printf("%d", digit_one);
            } else if ((x == (score_position_X_for_player_one + 1)) &&
            (y == score_position_Y_for_player_one_and_player_two)) {
              // условие отрисовки второй цифры счета первого игрока
              int digit_two = score_player_one % 10;
              printf("%d", digit_two);
            } else if ((x == score_position_X_for_player_two) &&
            (y == score_position_Y_for_player_one_and_player_two)) {
              // условие отрисовки первой цифры счета правой игрока
                int digit_one = score_player_two / 10;
                printf("%d", digit_one);
            } else if ((x == (score_position_X_for_player_two + 1)) &&
            (y == score_position_Y_for_player_one_and_player_two)) {
              // условие отрисовки второй цифры счета правой игрока
              int digit_two = score_player_two % 10;
              printf("%d", digit_two);

            } else if ((y == 0) || (y == (Y - 1)) || (x == 0) || (x == (X - 1))) {
              // условие отрисовки рамки-строки
              printf("%c", '#');
            } else if ((y == left_rocket && (x == left_rocket_x)) ||
            (y == (left_rocket + 1) && (x == left_rocket_x)) ||
             (y == (left_rocket - 1) && (x == left_rocket_x)) ||
             (y == right_rocket && (x == right_rocket_x)) ||
             (y == (right_rocket + 1) && (x == right_rocket_x)) ||
             (y == (right_rocket - 1) && (x == right_rocket_x)))  {
             // условие отрисовки ракеток
                printf("%c", '|');
            } else if ((x == ball_x) && (y == ball_y)) {
            // условие отрисовки шарика
                printf("%c", '@');
            } else {
            // цикл отрисовки пробелов - ЗАМЕНИТЬ НА ПРОБЕЛ!!!!!!!!
                printf("%c", ' ');
            }
            // проверяем перенос строки
            if (x == (X - 1)) {
               printf("\n");
            }
        }
    }
}


