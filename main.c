#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <locale.h>

typedef const char** TEXT;
typedef const char*  STR;
typedef const int    CI;
//--------------------------------------------------------------------------------------------------------------------------------------------
//---------ГЛАВНАЯ ЧАСТЬ----------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
//Объявление функций
void info();
void algorithm(FILE* is, FILE* os);
STR get_str(FILE* stream, STR message);
char get_next_symb(FILE* stream){
    char c;
    do{
        c = fgetc(stream);
    } while(c == '\n' || c == '\r');
    return c;
}
void clear_f(FILE* stream){
    char c;
    do{
        c = fgetc(stream);
    } while(c != '\n' && c != "\r");
}
//ГЛАВНАЯ ФУНКЦИЯ-----------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Rus");
    int n = 0;

    //Выводит меню, пока пользователь не введёт символ для закрытия программы
    while(1){
        printf("Выберите вариант\n"
               "1. Информация об алгоритме\n"
               "2. Выполнить алгоритм\n"
               "3. Закончить выполнение программы\n");
        printf(">>> "); scanf("%d", &n);
        clear_f(stdin);

        switch(n){
            case 1:{
                info(); break;
            }

            case 2:
            {
                FILE* istream = NULL;
                FILE* ostream = NULL;
                int m;
                while(istream == NULL && istream == NULL) {
                    puts("Вы будете использовать консольный (1) или файловый (2) ввод/вывод?");
                    scanf("%d", &m);
                    clear_f(stdin);
                    if(m == 1) {
                        istream = stdin;
                        ostream = stdout;
                    }
                    else if(m == 2) {
                        istream = fopen(get_str(stdin, "Введите название файла (включая его формат .txt): "), "r");
                        ostream = fopen("results.txt", "w");
                        if (istream == NULL || ostream == NULL){
                            puts("ОШИБКА ОПРЕДЕЛЕНИЯ ТИПА ВВОДА!"); break;
                        }
                    }
                    else
                        puts("Неправильный ввод! Вы можете вводить только символы 1 или 2!");
                }
                if(istream != NULL && ostream != NULL)
                    algorithm(istream, ostream);
                break;
            }
            case 3:
                return 0;
            default:
                puts("Вы ввели неправильный символ! Вы можете ввести только 1, 2 или 3!\n");
        }
    }
}
//ИНФОРМАЦИЯ----------------------------------------------------------------------------------------------------------------------------------
//Вывод описание алгоритма, т.е. его задачу, ввод и вывод
void info()
{
    printf("\nАлгоритм был разработан студентом СПбГЭТУ ЛЭТИ Боевом Иваном (группа 0305)\n"
           "\nЗадача алгоритма\n"
           "Из строк исходного массива сформировать другой текст, в который войдут только строки из исходного текста, "
           "в которых после слов, содержащих больше заданного в исходных данных количества одинаковых символов, "
           "будет вставлено новое введенное слово.\n"
           "\nВходные данные\n"
           "1) Количество строк исходного текста\n"
           "2) Количество одинаковых символов\n"
           "3) Строка символов-разделителей\n"
           "4) Исходный текст (в процессе ввода строк текста в них не записываются повторяющиеся после каждого слова символы разделители)\n"
           "5) Слова, которые будут вставлены в формируемый текст\n"
           "\nВывод\n"
           "1) Исходный текст\n"
           "2) Сформированный текст, или сообщение о том, что текст не был сформирован\n\n");
}
//Считывает из потока символьную строку и возвращает её.
//Если пользователь выбрал консольный ввод, то также выводит сообщение
STR get_str(FILE* stream, STR message)
{
    char* str;
    int n;
    char c;
    if(stream == stdin)
        puts(message);
    c = get_next_symb(stream);
    n = 0;
    str = (char*)malloc(1 * sizeof(char));
    do{
        if(c == EOF)
            return NULL;
        str[n] = c;
        if (!(str = (char *) realloc(str, (++n + 1) * sizeof(char))))
            return NULL;
        c = fgetc(stream);
    } while(c != '\n' && c != '\r');

    str[n] = '\0';
    return str;
}
//Просит пользователя ввести тип ввода, которым он будет пользоваться.
//Если это файл, то запрашивает ввести его название
//--------------------------------------------------------------------------------------------------------------------------------------------
//---------АЛГОРИТМ---------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
//Func declarations
STR read_text_str(FILE* stream, int*** p, STR del, CI repN, CI i, CI strN);
TEXT form_text(FILE* stream, TEXT t, int** p, int* k, STR del);
//Выводит текст в консоль
void print_text(FILE* stream, TEXT t, STR message, CI n){
    fprintf(stream, message);
    for(int i = 0; i < n; i++)
        fprintf(stream, "%s\n", *(t+i));
}
bool enter_number(FILE* stream, STR message, int* n){
    if(stream == stdin)
        puts(message);
    if(fscanf(stream, "%d", n) == EOF)
        return false;
    return true;
}
void free_text(TEXT t, CI n){
    if(t){
        for(int i = 0; i < n; i++)
            if(t[i])
                free(t[i]);
        free(t);
    }
}
void free_algorithm(STR message, FILE* stream1, FILE* stream2, TEXT t1, CI n1, TEXT t2, CI n2, STR str1, int** p){
    puts(message);
    if(stream1)
        if(stream1 != stdin)
            fclose(stream1);
    if(stream2)
        if(stream2 != stdout)
            fclose(stream2);
    if(str1 != NULL)
        free(str1);
    if(p) {
        for(int i = 0; p[i][0] != -1; i++)
            free(p[i]);
        free(p);
    }
    free_text(t1, n1);
    free_text(t2, n2);
}
//ГЛАВНАЯ ФУНКЦИЯ-----------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
void algorithm(FILE* is, FILE* os)
{
    int strN = 0;           //количество строчек в исходном тексте
    int strfN = 0;          //количество строчек в формируещемся текста
    int repN = 0;           //количество повторений символов
    STR del = NULL;         //строка символов разделителей
    TEXT pText = NULL;      //исходный текст
    TEXT fText = NULL;      //формирующийся текст
    int** p = NULL;
    //Начало ввода из потока (файла/консоли)
    if(!enter_number(is, "Введите количество строчек исходного текста: ", &strN) || strN < 1)
        return free_algorithm("ОШИБКА ВВОДА КОЛИЧЕСТВА СТРОК ТЕКСТА!", is, os, pText, strN, fText, strfN, del, p);
    clear_f(is);
    if(!enter_number(is, "Введите количество одинаковых символов в слове: ", &repN) || repN < 0)
        return free_algorithm("ОШИБКА ВВОДА КОЛИЧЕСТВА ОДИНАКОВЫХ СИМВОЛОВ!", is, os, pText, strN, fText, strfN, del, p);
    clear_f(is);
    del = get_str(is, "Введите строку символов-разделителей: ");

    if(!del || del[0] == '\0')
        return free_algorithm("ОШИБКА ВВОДА СТРОКИ СИМВОЛОВ РАЗДЕЛИТЕЛЕЙ!", is, os, pText, strN, fText, strfN, del, p);

    if(!(pText = (TEXT)malloc(strN * sizeof(STR))))
        return free_algorithm("ОШИБКА ВЫДЕЛЕНИЯ ПАМЯТИ ДЛЯ ИСХОДНОГО ТЕКСТА!", is, os, pText, strN, fText, strfN, del, p);

    if(!(p = (int**)malloc(sizeof(int*))))
        return free_algorithm("ОШИБКА ВЫДЕЛЕНИЯ ПАМЯТИ ДЛЯ МАССИВА УКАЗАТЕЛЕЙ!", is, os, pText, strN, fText, strfN, del, p);
    if(!(*p = (int*)malloc(2 * sizeof(int))))
        return free_algorithm("ОШИБКА ВЫДЕЛЕНИЯ ПАМЯТИ ДЛЯ МАССИВА УКАЗАТЕЛЕЙ!", is, os, pText, strN, fText, strfN, del, p);
    p[0][0] = -1;

    if(is == stdin)
        puts("Введите текст:");
    for(int i = 0; i < strN; i++) {
        *(pText+i) = read_text_str(is, &p, del, repN, i, strN);
        if(*(pText+i) == NULL)
            return free_algorithm("ОШИБКА ВВОДА СТРОКИ ИСХОДНОГО ТЕКСТА!", is, os, pText, i, fText, strfN, del, p);
    }

    fText = form_text(is, pText, p, &strfN, del);
    if(strfN == -1)
        return free_algorithm("ОШИБКА ФОРМИРОВАНИЯ ТЕКСТА!", is, os, pText, strN, fText, strfN, del, p);

    print_text(os, pText, "Исходный текст\n", strN);
    if(!strfN)
        fprintf(os, "Текст не был сформирован\n");
    else
        print_text(os, fText, "Сформированный текст\n", strfN);

    if(is != stdin)
        puts("Результат работы алгоритма записан в файл results.txt");
    free_algorithm("\n", is, os, pText, strN, fText, strfN, del, p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------
//-----------ВВОД СТРОКИ----------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
//Объявления функций
int** add_int_p(int*** parr, int i, int y);
int** counter_alloc();
int** free_counter(int*** arr);
char* add_symb(char** s, const char c);
int** count_symbol(int*** m, char c);
bool counter_repeat_check(int** counter, CI repN);
bool str_repeat_check(STR s, const char c);
//ГЛАВНАЯ ФУНКЦИЯ--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
STR read_text_str(FILE* stream, int*** p, STR del, CI repN, CI i, CI strN)
{
    int n; //количество введенных символов
    char* str; //вводимая строка
    int** counter; //счётчик букв в слове
    char c; //вводимый символ
    char* dCheck; //введённые разделители
    if(!(str = (char*)malloc(sizeof(char))))
        return NULL;
    if(!(counter = counter_alloc()))
        return NULL;
    if(!(dCheck = (char*)malloc(sizeof(char))))
        return NULL;

    dCheck[0] = '\0';
    n = 0;
    c = 0;
    while(c != '\n' && c != '\r' && c != EOF) {
        if(stream != stdin && n == 0) //если это файловый ввод и вводится первый символ
            c = get_next_symb(stream);
        else
            c = fgetc(stream);
        if(c == EOF && i != strN - 1)
            return NULL;
        else if (str_repeat_check(del, c) || c == '\r' || c == '\n' || c == EOF) //если введенный символ - конец строки
        {
            if (counter_repeat_check(counter, repN))  //проверка на одинаковые буквы в слове
                if (!(*p = add_int_p(p, i, n)))
                    return NULL;
            counter = free_counter(&counter);         //освобождаем память, выделенную под счётчик
            if (c == '\r' || c == '\n' || c == EOF)
                str[n] = '\0';
            else {
                if (!(counter = counter_alloc()))  //создаём новый
                    return NULL;
                if (!str_repeat_check(dCheck, c))  //если разделитель не повторяется
                {
                    if (!(dCheck = add_symb(&dCheck, c)))
                        return NULL;      //добавялем его в повторенные
                    if (!(str = (char *) realloc(str, (n + 2) * sizeof(char))))
                        return NULL;
                    str[n++] = c;        //вписываем разделитель в текст
                }
            }
        }
        else{
            if (!(counter = count_symbol(&counter, c))) //подсчитываем символ
                return NULL;
            if (!(str = (char *) realloc(str, (n + 2) * sizeof(char))))
                return NULL;
            str[n++] = c; //вставляем символ в текст
        }

    }
    free(dCheck);

    return str;
}
//Проверяет счётчик повторений символов в слове
//Если в введённом слове есть символ, количество повторений которой больше
//заданного, то функция возвращает true. Иначе - false
bool counter_repeat_check(int** c, CI repN)
{
    int i;
    i = 0;
    while(c[i][0] != 0){
        if(c[i++][1] > repN)
            return true;
    }
    return false;
}
//Проверяет строку s на повторение символа c.
//Если символ c есть в строке, функция возвращает true.
//Если символа в строке нет, функция возвращает false
bool str_repeat_check(STR s, const char c)
{
    int i = 0;
    while(s[i] != '\0')
        if(c == s[i++])
            return true;
    return false;
}
//Добавляет символ c в строку s
char* add_symb(char** str, const char c)
{
    int i = 0;
    while((*str)[i] != '\0')
        i++;
    if(!(*str = (char*)realloc(*str, (i + 2) * sizeof(char))))
        return NULL;
    (*str)[i] = c; (*str)[i+1] = '\0';
    return *str;
}
//Добавляет указатель на символ строки в массив указателей
int** add_int_p(int*** parr, int x, int y)
{
    int i = 0;
    while((*parr)[i][0] != -1)
        i++;
    if(!(*parr = (int**)realloc(*parr, (i + 2) * sizeof(int*))))
        return NULL;
    (*parr)[i] = malloc(2 * sizeof(int));
    (*parr)[i][0] = x;   (*parr)[i][1] = y;
    (*parr)[i+1] = malloc(2 * sizeof(int));
    (*parr)[i+1][0] = -1;
    return *parr;
}
//Создаёт счётчик
int** counter_alloc()
{
    int** arr;
    arr = (int**)malloc(sizeof(int*));
    arr[0] = (int*)malloc(2 * sizeof(int));
    arr[0][0] = 0; arr[0][1] = 0;
    return arr;
}
//В счётчике m обавляет +1 к количеству
//повторений символа c
int** count_symbol(int*** m, char c)
{
    int i = 0;
    while((*m)[i][0] != (int)c)
        if((*m)[i][0] == 0)
        {
            (*m)[i][0] = (int)c; (*m)[i][1] = 1;
            int** temp = (int**)realloc(*m, (i + 2) * sizeof(int*));
            if(temp == NULL)
            {
                puts("REALLOC ОШИБКА!!!");
                return NULL;
            }
            *m = temp;
            if(!((*m)[i+1] = (int*)malloc(2 * sizeof(int))))
                return NULL;
            (*m)[i+1][0] = 0;
            return *m;
        }
        else
            i++;
    (*m)[i][1]++;
    return *m;
}
//Освобождает память, выделенную для счётчика
int** free_counter(int*** arr)
{
    int i = 0;
    if(*arr == NULL)
        return NULL;
    while((*arr)[i][0] != 0){
        i++;
    }
    while(i){
        free((*arr)[i]);
        i--;
    }
    free(*arr[0]);
    free(*arr);
    return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------
//----------ФУНКЦИЯ ФОРМИРОВАНИЯ ТЕКСТА-------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
//Объявление функций
int insert_word(FILE* stream, char** s, int index, bool last);
//ГЛАВНАЯ ФУНКЦИЯ-----------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
TEXT form_text(FILE* stream, TEXT t, int** p, int* k, STR del) {
    TEXT fT;
    int ind;
    char *str;
    int m = 0;
    *k = 0;
    int i, j;

    fT = NULL;
    m = 0;
    i = 0;
    while (p[m][0] != -1) {
        if(p[m][0] == i)
        {
            str = (char *) malloc(sizeof(char));
            ind = 0;
            j = 0;
            while (t[i][j] != '\0' || p[m][0] == i && p[m][1] == j) {
                if (p[m][0] == i && p[m][1] == j) { //если мы встретили строку, куда надо вставить слово
                    if (!m && stream == stdin)
                        puts("Введите слова для вставки в текст");
                    int b = insert_word(stream, &str, j + ind, p[m+1][0] == -1);
                    if(b == 0){
                        str[j + ind] = del[0];
                        ind++;
                    }
                    else
                        ind += b;
                    m++;
                }
                str = (char *) realloc(str, (j + ind + 2) * sizeof(char));
                str[j + ind] = t[i][j];

                j++;
            }
            str[j + ind] = '\0';
            if (!fT)
                fT = (TEXT) malloc(sizeof(STR));
            else
                fT = (TEXT) realloc(fT, (*k + 1) * sizeof(STR));
            if (!fT) {
                *k = -1;
                return NULL;
            }
            *(fT + *k) = str;
            (*k)++;
        }
        i++;
    }

    return fT;
}
//Вставляет слово из потока в вводимую строку s
int insert_word(FILE* stream, char** s, int index, bool last)
{
    int n = 0;
    char c;
    if(stream != stdin)
        c = get_next_symb(stream);
    else
        c = fgetc(stream);
    while(c != '\n' && c != '\r' && c != EOF) {
        *s = (char *) realloc(*s, (index + n + 1));
        (*s)[index + n] = c;
        n++;
        c = fgetc(stream);
    }

    return n;
}