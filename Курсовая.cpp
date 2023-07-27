#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>
#include <conio.h> 

char const* function_name[] = { "sin", "cos", "tan", "asin", "acos", "atan", "sqrt", "ln", "lg" };

char* strndup(const char* src, size_t n)
{
    size_t len = strnlen(src, n);
    char* dst = new char[len + 1];
    memcpy(dst, src, len);
    dst[len] = '\0';
    return dst;
}

double get_step(double interval)
{
    if (interval < 1) return 0.01;
    if (interval >= 1 && interval < 10) return 0.1;
    if (interval >= 10 && interval < 100) return 1;
    if (interval >= 100 && interval < 1000) return 3;
    if (interval >= 1000) return 10;
}

double result_recursive_descent(char* our_function, double x, bool* check)
{
    int length_intput_function = strlen(our_function);
    int position_operation = 0;
    int bracket = 0;
    double value = 0;
    char* left_part_function;
    char* right_part_function;
    for (int i = length_intput_function - 1; i >= 0; i--)
    {
        if (our_function[i] == ')')
        {
            bracket++;
        }
        else if (our_function[i] == '(')
        {
            bracket--;
        }
        else
        {
            if ((our_function[i] == '+' || our_function[i] == '-') && !bracket)
            {
                position_operation = i;
                break;
            }
            if ((our_function[i] == '*' || our_function[i] == '/' || our_function[i] == '^') && !bracket && !position_operation)
            {
                position_operation = i;
            }
        }
    }

    if (position_operation != 0)
    {
        left_part_function = strndup(our_function, position_operation);
        right_part_function = strndup(our_function + position_operation + 1, length_intput_function - position_operation - 1);
        switch (our_function[position_operation])
        {
            case '+':
            {
                return result_recursive_descent(left_part_function, x, check) + result_recursive_descent(right_part_function, x, check);
                break;
            }
            case '-':
            {
                return result_recursive_descent(left_part_function, x, check) - result_recursive_descent(right_part_function, x, check);
                break;
            }
            case '*':
            {
                return result_recursive_descent(left_part_function, x, check) * result_recursive_descent(right_part_function, x, check);
                break;
            }
            case '/':
            {
                double numenator = result_recursive_descent(left_part_function, x, check);
                double denominator = result_recursive_descent(right_part_function, x, check);
                if (denominator == 0) *check = false;
                else return numenator / denominator;
                break;
            }
            case '^':
            {
                left_part_function = strndup(our_function, position_operation);
                right_part_function = strndup(our_function + position_operation + 1, strlen(our_function) - position_operation - 1);
                return pow(result_recursive_descent(left_part_function, x, check), result_recursive_descent(right_part_function, x, check));
                break;
            }
        }
    }
    else
    {
        if (our_function[0] == '(' && our_function[length_intput_function - 1] == ')')
        {
            left_part_function = strndup(our_function + 1, length_intput_function - 2);
            return result_recursive_descent(left_part_function, x, check);
        }
        else if (our_function[0] == '-')
        {
            left_part_function = strndup(our_function + 1, length_intput_function - 1);
            return -result_recursive_descent(left_part_function, x, check);
        }
        else if (our_function[0] == 'x')
        {
            return x;
        }
        else if (isdigit(our_function[0]))
        {
            int count = 0, j = 0;
            char buffer[10] = { 0 };
            while ((isdigit(our_function[count]) || our_function[count] == '.'))
            {
                buffer[j] = our_function[count];
                j++;
                count++;
            }
            value = atof(buffer);
            return value;
        }
        else if (isalpha(our_function[0]))
        {
            int count = 0, j = 0;
            char buffer[10] = { 0 };
            while (isalpha(our_function[count]))
            {
                buffer[j] = our_function[count];
                j++;
                count++;
            }
            int i = 0;
            for (; i < sizeof(function_name) / sizeof(function_name[0]); i++)
            {
                if (!(strncmp(buffer, function_name[i], sizeof(buffer))))
                {
                    break;
                }
            }
            int len_buffer = strlen(buffer);
            left_part_function = strndup(our_function + len_buffer, length_intput_function - len_buffer);
            value = result_recursive_descent(left_part_function, x, check);
            switch (i)
            {
                case 0:
                {
                    return sin(value);
                    break;
                }
                case 1:
                {
                    return cos(value);
                    break;
                }
                case 2:
                {
                    return tan(value);
                    break;
                }
                case 3:
                {
                    if (value >= -1 && value <= 1) return asin(value);
                    else *check = false;
                    break;
                }
                case 4:
                {
                    if (value >= -1 && value <= 1) return acos(value);
                    else *check = false;
                    break;
                }
                case 5:
                {
                    return atan(value);
                    break;
                }
                case 6:
                {
                    if (value >= 0) return sqrt(value);
                    else *check = false;
                    break;
                }
                case 7:
                {
                    if (value > 0) return log(value);
                    else *check = false;
                    break;
                }
                case 8:
                {
                    if (value > 0) return log10(value);
                    else *check = false;
                    break;
                }
                default:
                {
                    printf("Неизвестная функция %s\n", buffer);
                    exit(1);
                }
            }
        }
    }
    return 0;
}

void help(HANDLE hConsole)
{
    system("cls");
    printf("Hello,world");
    SetConsoleTextAttribute(hConsole, 05);
    printf("Программу разработала студентка ИГЭУ им. Ленина гр. 1-42 Самойлова А. И.\n\n");
    SetConsoleTextAttribute(hConsole, 04);
    printf("    Справка о функциях и операциях\n");
    SetConsoleTextAttribute(hConsole, 02);
    printf("|-------------------------------------|\n");
    printf("|    Название функции  |  Обозначение |\n");
    printf("|-------------------------------------|\n");
    SetConsoleTextAttribute(hConsole, 04);
    printf("     Тригонометрические функции        \n");
    SetConsoleTextAttribute(hConsole, 02);
    printf("|-------------------------------------|\n");
    printf("|Синус                 |      sin(x)  |\n");
    printf("|-------------------------------------|\n");
    printf("|Косинус               |      cos(x)  |\n");
    printf("|-------------------------------------|\n");
    printf("|Тангенс               |      tan(x)  |\n");
    printf("|-------------------------------------|\n");
    printf("|Арксинус              |     asin(x)  |\n");
    printf("|-------------------------------------|\n");
    printf("|Арккосинус            |     acos(x)  |\n");
    printf("|-------------------------------------|\n");
    printf("|Арктангенс            |     atan(x)  |\n");
    printf("|-------------------------------------|\n");
    printf("|-------------------------------------|\n");
    printf("|Квадратный корень     |     sqrt(x)  |\n");
    printf("|-------------------------------------|\n");
    SetConsoleTextAttribute(hConsole, 04);
    printf("      Логарифмические функции          \n");
    SetConsoleTextAttribute(hConsole, 02);
    printf("|-------------------------------------|\n");
    printf("|Натуральный логарифм  |       ln(x)  |\n");
    printf("|-------------------------------------|\n");
    printf("|Десятичный логарифм   |       lg(x)  |\n");
    printf("|-------------------------------------|\n");
    SetConsoleTextAttribute(hConsole, 04);
    printf("       Арифметические операции         \n");
    SetConsoleTextAttribute(hConsole, 02);
    printf("|-------------------------------------|\n");
    printf("|Сложение              |       +      |\n");
    printf("|-------------------------------------|\n");
    printf("|Вычитание             |       -      |\n");
    printf("|-------------------------------------|\n");
    printf("|Умножение             |       *      |\n");
    printf("|-------------------------------------|\n");
    printf("|Деление               |       /      |\n");
    printf("|-------------------------------------|\n");
    printf("|Возведение в степень  |       ^      |\n");
    printf("|-------------------------------------|\n");
    SetConsoleTextAttribute(hConsole, 04);
    printf("ВНИМАНИЕ!\n");
    SetConsoleTextAttribute(hConsole, 02);
    printf("1. Движение по меню осуществляется с помощью стрелок и нажатия кнопки ENTER\n");
    printf("2. После того, как Вы ввели данные ЧЕРЕЗ ПРОБЕЛ, нужно нажать кнопку ENTER\n3. Если Вы не так ввели данные, у Вас будет возможность повторить ввод\n");
    SetConsoleTextAttribute(hConsole, 04);
    printf("ПРЕДУПРЕЖДЕНИЕ по поводу 3 пункта\n");
    SetConsoleTextAttribute(hConsole, 02);
    printf("3.1 Если Вы неправильно ввели название функции, то программу придется запускать заново\n");
    printf("4. Ввод самой функции осуществляется без пробелов\n5. Чем больше интервал,тем больше размер сетки, учтите это при вводе интервала\n6. При вводе тригонометрических функций учитывайте размер окна, для лучшей визуализации длина окна должна быть гораздо больше,чем ширина(Например, 800 350)\n");
    printf("7. Размеры самого окна должны быть только целыми числами, то есть никаких дробных частей в числах не должно быть(Например, 500 500)\n");
    printf("8. Заранее подумайте над размерами окна, на больших интервалах шаг будет крупный,поэтому,если ввести маленький размер окна,то Вы ничего не увидете\n");
    SetConsoleTextAttribute(hConsole, 04);
    printf("ПРЕДУПРЕЖДЕНИЕ!\n");
    SetConsoleTextAttribute(hConsole, 02);
    printf("9. Пожалуйста, не вводите слишком маленький размер окна (меньше 100), так как с большей долью вероятности Вы ничего не увидете\n");
    printf("10. Координаты левого верхнего угла должны быть больше или равно 30 как по оси X, так и по оси Y\n");
    printf("11. Проверяйте, пожалуйста, скобки, если их ввести неправильно, то программа будет работать некорректно\n");
    system("pause");
}

void draw_grath(HANDLE hConsole, HWND hcon)
{
    system("cls");
    char* ptr;
    char mathematical_function[255] = { 0 };
    do {
        printf("Введите математическую функцию ,которая должна быть после y = \n");
        ptr = fgets(mathematical_function, sizeof(mathematical_function), stdin);
        ptr[strlen(ptr) - 1] = '\0';
        if (strlen(ptr) == 0) {
            SetConsoleTextAttribute(hConsole, 04);
            printf("Функция не должна быть пустой! Повторите ввод!\n");
            SetConsoleTextAttribute(hConsole, 02);
        }
        else { break; }
    } while (true);

    double left_board_interval, right_board_interval;
    do {
        printf("\nВведите границы интервала,на котором будет строиться график функции:\n");
        printf("От: ");
        scanf_s("%lf", &left_board_interval);
        printf("До: ");
        scanf_s("%lf",&right_board_interval);
        if (left_board_interval > right_board_interval) {
            double buffer = left_board_interval;
            left_board_interval = right_board_interval;
            right_board_interval = buffer;
            break;
        }
        if (left_board_interval != right_board_interval) { break; }
        SetConsoleTextAttribute(hConsole, 04);
        printf("Интервал состоит всего из одной точки! Повторите ввод еще раз!\n");
        SetConsoleTextAttribute(hConsole, 02);       
    } while (true);

    int position_X_window, position_Y_window;
    do {
        printf("\nВведите координаты левого верхнего угла окна:\n");
        printf("Координата по оси X: ");
        scanf_s("%d", &position_X_window);
        printf("Координата по оси Y: ");
        scanf_s("%d",&position_Y_window);
        if (position_X_window < 0 || position_Y_window < 0) {
            SetConsoleTextAttribute(hConsole, 04);
            printf("Координаты угла не могут быть отрицательными! Повторите ввод еще раз!\n");
            SetConsoleTextAttribute(hConsole, 02);
        }
        else { break; }
    } while (true);

    int size_x, size_y;
    do {
        printf("\nВведите длину и ширину окна для рисования графика:\n");
        printf("Длина окна: ");
        scanf_s("%d", &size_x);
        printf("Ширина окна: ");
        scanf_s("%d",&size_y);
        if (size_x < 0 || size_y < 0) {
            SetConsoleTextAttribute(hConsole, 04);
            printf("Размеры окна не могут быть отрицательными! Повторите ввод еще раз!\n");
            SetConsoleTextAttribute(hConsole, 02);
        }
        else { break; }
    } while (true);

    HDC hdc = GetDC(hcon);
    bool flag = false;
    char answer[5] = { 0 };
    do {
        printf("\nХотите вывести название функции рядом с графиком[(н/д) | (n/y)]?\n");
        scanf_s("%s", answer, sizeof(answer));
        if (!(strcmp(answer, "Yes")) || !(strcmp(answer, "yes")) || !(strcmp(answer, "y")) || !(strcmp(answer, "д")) || !(strcmp(answer, "да")) || !(strcmp(answer, "Да"))) {
            flag = true;
            break;
        }
        else if (!(strcmp(answer, "No")) || !(strcmp(answer, "no")) || !(strcmp(answer, "n")) || !(strcmp(answer, "н")) || !(strcmp(answer, "нет")) || !(strcmp(answer, "Нет"))) { break; } 
        SetConsoleTextAttribute(hConsole, 04);
        printf("Вы ввели некорректный ответ. Повторите ввод еще раз!\n");
        SetConsoleTextAttribute(hConsole, 02);
    } while (true);

    int option = 0;
    do {
        printf("\nНажмите 1,если хотите,чтобы график выводился в начале координат, иначе нажмите любую другую цифру\n");
        scanf_s("%d", &option);
        break;
    } while (true);

    getchar();
    system("cls");
    HPEN white_pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HPEN pale_pen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    HPEN red_pen = CreatePen(PS_SOLID, 2, RGB(235, 0, 0));

    double interval = fabs(left_board_interval - right_board_interval);
    double step_X = get_step(interval);
    double values_function = 0;
    double our_const = 1E-6;
    double max_Y = 1E-8;
    double min_Y = 1234567890;

    for (double x = left_board_interval; x <= right_board_interval; x += step_X) {
        bool check = true;
        if (fabs(x) - abs((int)x) < our_const) { x = (float)x; }
        values_function = result_recursive_descent(ptr, x, &check);
        if (values_function > max_Y && check != false) { max_Y = values_function; }
        if (values_function < min_Y && check != false) { min_Y = values_function; }
    }

    double value_interval = max_Y - min_Y;
    double step_Y = get_step(value_interval);

    double X_Coeff = (right_board_interval - left_board_interval) / (double)size_x;
    double Y_Coeff = value_interval / (double)size_y;

    int size_grid_X = size_x / ((right_board_interval - left_board_interval) / step_X);
    int size_grid_Y = size_y / (value_interval / step_Y);

    if (option == 1) {
        if (max_Y < 0) {max_Y += fabs(max_Y);}
        if (min_Y > 0) { min_Y -= min_Y; }
    }
    SelectObject(hdc, pale_pen);
    for (int i = 0; i <= size_x; i += size_grid_X) {
        MoveToEx(hdc, position_X_window + i, position_Y_window, NULL);
        LineTo(hdc, position_X_window + i, position_Y_window + size_y);
        double num = i * X_Coeff + left_board_interval;
        char number[25] = { 0 };
        sprintf_s(number, "%.2lf", num);
        TextOutA(hdc, position_X_window + i, position_Y_window + size_y + 30, number, strlen(number));
    }
    for (int i = 0; i <= size_y; i += size_grid_Y) {
        MoveToEx(hdc, position_X_window, position_Y_window + i, NULL);
        LineTo(hdc, position_X_window + size_x, position_Y_window + i);
        double num = max_Y - i * Y_Coeff;
        char number[25] = { 0 };
        sprintf_s(number, "%.2lf", num);
        TextOutA(hdc, position_X_window - 32, position_Y_window + i, number, strlen(number)); 
    }

    SelectObject(hdc, red_pen);
    bool previousPoint = false;
    double num = 0;
    double temp = 123456456;
    int save_offset_X = 0;
    int save_offset_Y = 0;
    int save_pos_X = 0;
    int save_pos_Y = 0;
    for (int i = 0; i < size_x; i++) {
        bool check = true;
        num = i * X_Coeff + left_board_interval;
        double result = result_recursive_descent(ptr, num , &check);
        int offset = (position_Y_window + size_y) - (int)((result - min_Y) / Y_Coeff);

        if (result > max_Y) { result = max_Y; }
        else if (result < min_Y) { result = min_Y; }

        if (num == 0) { //  for axis Y
            save_pos_Y = i;
            save_offset_Y = offset;
        }
        if (fabs(result) < fabs(temp)) { //  for axis X
            temp = result;
            save_pos_X = i;
            save_offset_X = offset;
        }
        if (check && previousPoint && offset >= position_Y_window && offset <= (position_Y_window + size_y)) {
            previousPoint = true;
            LineTo(hdc, position_X_window + i, offset);
            Sleep(10);
        }
        else if (check && offset >= position_Y_window && offset <= (position_Y_window + size_y)) {
            previousPoint = true;
            MoveToEx(hdc, position_X_window + i, offset, NULL);
        }
        else { previousPoint = false; }
    }
    num = save_pos_Y * X_Coeff + left_board_interval;
    if ((temp || temp == 0)) { // axis X
        SelectObject(hdc, white_pen);
        MoveToEx(hdc, position_X_window + size_x, save_offset_X, NULL);
        LineTo(hdc, position_X_window, save_offset_X);
        TextOutA(hdc, position_X_window + size_x + 13, save_offset_X, "X", 1);
    }
    if (num == 0) { // axis Y
        SelectObject(hdc, white_pen);
        MoveToEx(hdc, position_X_window + save_pos_Y, position_Y_window + size_y, NULL);
        LineTo(hdc, position_X_window + save_pos_Y, position_Y_window);
        TextOutA(hdc, position_X_window + save_pos_Y, position_Y_window - 20, "Y", 1);
    }

    SelectObject(hdc, white_pen);
    MoveToEx(hdc, position_X_window, position_Y_window, NULL);
    LineTo(hdc, position_X_window + size_x, position_Y_window); 
    LineTo(hdc, position_X_window + size_x, position_Y_window + size_y);
    LineTo(hdc, position_X_window, position_Y_window + size_y);
    LineTo(hdc, position_X_window, position_Y_window); 
    if (flag) {
        TextOutA(hdc, position_X_window + size_x / 3 - strlen(ptr), position_Y_window - 25, "y = ", 4);
        TextOutA(hdc, position_X_window + size_x / 3 - strlen(ptr) + 24, position_Y_window - 25, ptr, strlen(ptr));
    }
    _getch();
    DeleteObject(white_pen);
    DeleteObject(pale_pen);
    DeleteObject(red_pen);
}

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HWND hcon = GetConsoleWindow();
    CONSOLE_FONT_INFO cfi;
    GetCurrentConsoleFont(hConsole, false, &cfi);
    COORD fs = GetConsoleFontSize(hConsole, cfi.nFont);
    PCONSOLE_FONT_INFOEX ccf = new CONSOLE_FONT_INFOEX;
    (*ccf).dwFontSize.X = 12;
    (*ccf).dwFontSize.Y = 20;
    (*ccf).nFont = 11;
    (*ccf).cbSize = sizeof(CONSOLE_FONT_INFOEX);
    ccf->FontWeight = 400;
    lstrcpyW((*ccf).FaceName, L"Lucida Console");
    (*ccf).FontFamily = FF_DONTCARE;
    bool b = SetCurrentConsoleFontEx(hConsole, false, ccf);
    fs = GetConsoleFontSize(hConsole, cfi.nFont);
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    const int n = 3;
    COORD positionMenuItem[n];
    int punkt = 0;
    char names[n][25] = { "Построить график", "Справка", "Выход" };
    do
    {
        int max_X, max_Y;
        PCONSOLE_SCREEN_BUFFER_INFO pwi = new CONSOLE_SCREEN_BUFFER_INFO;
        PWINDOWINFO pgwi = new WINDOWINFO;
        GetConsoleScreenBufferInfo(hConsole, pwi);
        GetWindowInfo(hcon, pgwi);
        max_X = pwi->dwSize.X;
        max_Y = pwi->dwSize.Y;
        int y_0 = 10;
        for (int i = 0; i < n; i++)
        {
            positionMenuItem[i].X = (max_X - strlen(names[i])) / 2;
            positionMenuItem[i].Y = y_0 + i;
        }
        system("cls");
        char title[] = "Построение графиков";
        COORD pos;
        pos.X = (max_X - strlen(title)) / 2;
        pos.Y = n;
        SetConsoleTextAttribute(hConsole, 02);
        SetConsoleCursorPosition(hConsole, pos);
        puts(title);
        for (int i = 0; i < n; i++)
        {
            SetConsoleCursorPosition(hConsole, positionMenuItem[i]);
            puts(names[i]);
        }
        unsigned char ch;
        do
        {
            ch = _getch();
            if (ch == 224)
            {
                ch = _getch();
                switch (ch)
                {
                case 72:
                    SetConsoleCursorPosition(hConsole, positionMenuItem[punkt]);
                    SetConsoleTextAttribute(hConsole, 02);
                    puts(names[punkt]);
                    punkt--;
                    if (punkt < 0) { punkt = n - 1; }
                    SetConsoleTextAttribute(hConsole, 02 | FOREGROUND_INTENSITY);
                    SetConsoleCursorPosition(hConsole, positionMenuItem[punkt]);
                    puts(names[punkt]);
                    break;
                case 80:
                    SetConsoleCursorPosition(hConsole, positionMenuItem[punkt]);
                    SetConsoleTextAttribute(hConsole, 02);
                    puts(names[punkt]);
                    punkt++;
                    if (punkt > n - 1) { punkt = 0; }
                    SetConsoleTextAttribute(hConsole, 02 | FOREGROUND_INTENSITY);
                    SetConsoleCursorPosition(hConsole, positionMenuItem[punkt]);
                    puts(names[punkt]);
                    break;
                }
            }
        } while (ch != 13);
        switch (punkt)
        {
            case 0:
                draw_grath(hConsole, hcon);
                break;
            case 1:
                help(hConsole);
                break;
        }
    } while (punkt != n - 1);
    return 0;
}