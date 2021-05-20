#include <iostream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <conio.h>
#include <string>
#include <Windows.h>
#include <fstream>
#include <direct.h>

using namespace std;

const unsigned short sidesize = 4; // Константа размера поля

vector<int> in_game_map(sidesize);
vector<vector<int>> game_map(sidesize, in_game_map); // Игровая карта

vector<int> in_right_map(sidesize);
vector<vector<int>> right_map(sidesize, in_right_map); // Правильная итоговая карта

int UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77, QUIT = 113, SAVEQUIT = 115;

string save1 = "C://ProgramData//15nashki//saves//", save2 = "C://ProgramData//15nashki//saves//", save3 = "C://ProgramData//15nashki//saves//";

bool playgo; // Проверка на наличие игры

bool fileExists(string path){ // Проверка существует ли файл
    ifstream fin(path);
    if (fin.is_open())  return true;
    else    return false;
    fin.close();
}

struct coordinate { // Переменные для хранения координат нулевого элемента
    int x;
    int y;
}zero; // объект

void create_right_map(){ // Создаем правильную карту заполненую по порядку
    int right_value = 1;
    for (int i = 0; i < sidesize; i++)
        for (int j = 0; j < sidesize; j++)
            right_map[i][j] = right_value++;
    right_map[sidesize - 1][sidesize - 1] = 0; // Нулевой элемент в нижний правый угол
}

bool checkSolution(){  // Проверка на решаемость поля (Головоломка 14-15) 
    vector<int> solution(sidesize * sidesize); // Массив на 16 элементов
    int index = 0;
    for (int i = 0; i < sidesize; i++)
        for (int j = 0; j < sidesize; j++, index++)
            solution[index] = game_map[i][j];
    int inv = 0;
    for (int i = 0; i < sidesize * sidesize; i++)
        if (solution[i] != 0)
            for (int j = 0; j < i; j++)
                if (solution[j] > solution[i])
                    ++inv;
    for (int i = 0; i < sidesize * sidesize; i++)
        if (solution[i] == 0){
            inv += 1 + i / 4;
            break;
        }
    return (inv & 1) ? 0 : 1;
}

void create_game_map(){ // Рандомно создаем игровую карту
    unsigned limit = sidesize * sidesize;
    vector<int> tempomas;
    for (int i = 0; i < limit; i++)
        tempomas.push_back(i); // Заполнение массива

    int value;
    do{
        limit = sidesize * sidesize;
        for (int i = 0; i < limit; i++)
            tempomas.push_back(i);
        for (int i = 0; i < sidesize; i++){
            for (int j = 0; j < sidesize; j++){
                value = rand() % limit--;
                game_map[i][j] = tempomas[value];
                if (tempomas[value] == 0){     // сохраняем координаты нулевого элемента
                    zero.x = j;
                    zero.y = i;
                }
                tempomas.erase(tempomas.begin() + value);
            }
        }
    }while (!checkSolution());
}

bool check_map(){ // Проверяет карту на правильность
    if (game_map == right_map)  return true;
    return false;
}

void get_direction(){ // Управление на игровой карте
    int move = static_cast<int> (_getch());
    if (move == DOWN){
        if (zero.y < sidesize - 1){
            game_map[zero.y][zero.x] = game_map[zero.y + 1][zero.x];
            zero.y++;
            game_map[zero.y][zero.x] = 0;
        }
    }
    else if (move == UP){
        if (zero.y > 0){
            game_map[zero.y][zero.x] = game_map[zero.y - 1][zero.x];
            zero.y--;
            game_map[zero.y][zero.x] = 0;
        }
    }
    else if (move == LEFT){
        if (zero.x > 0){
            game_map[zero.y][zero.x] = game_map[zero.y][zero.x - 1];
            zero.x--;
            game_map[zero.y][zero.x] = 0;
        }
    }
    else if (move == RIGHT){
        if (zero.x < sidesize - 1){
            game_map[zero.y][zero.x] = game_map[zero.y][zero.x + 1];
            zero.x++;
            game_map[zero.y][zero.x] = 0;
        }
    }
    else if (move == QUIT) // q - Выход
        playgo = false;
    else if (move == SAVEQUIT){ // s - Сохранение текущей игровой карты
        string selection, savename;
        system("cls");
        cout << "Saving...";
        ifstream ffsaves("C://ProgramData//15nashki//saves//Saves.txt"); // Открывает файл поэтому пути
        getline(ffsaves, save1);
        getline(ffsaves, save2);
        getline(ffsaves, save3);
        ffsaves.close();
        Sleep(500);
        system("cls");
        string sname, temp;
        if (save1 == "NO")
            cout << "1)_SAVE_IS_EMPTY_\n";
        else if (save1 != "NO" && fileExists(save1 + "save1.txt")){
            ifstream ffsave(save1 + "save1.txt");
            getline(ffsave, sname);
            cout << "1) " << sname << endl;
            ffsave.close();
        }
        else{
            cout << "1) File is deleted or incorrect!\nClearing memory...(Can be used)\n";
            temp = "";
            ifstream ffsavedw("C://ProgramData//15nashki//saves//Saves.txt");
            getline(ffsavedw, sname);
            temp += "NO\n";
            getline(ffsavedw, sname);
            temp += sname + '\n';
            getline(ffsavedw, sname);
            temp += sname;
            ffsavedw.close();
            ofstream ffsaved;
            ffsaved.open("C://ProgramData//15nashki//saves//Saves.txt");
            ffsaved << temp;
        }
        if (save2 == "NO")  cout << "2)_SAVE_IS_EMPTY_\n";
        else if (save2 != "NO" && fileExists(save2 + "save2.txt")){
            ifstream sssave(save2 + "save2.txt");
            getline(sssave, sname);
            cout << "2) " << sname << endl;
            sssave.close();
        }
        else{
            cout << "\n2) File is deleted or incorrect!\nClearing memory...(Can be used)\n";
            temp = "";
            ifstream sssavedw("C://ProgramData//15nashki//saves//Saves.txt");
            getline(sssavedw, sname);
            temp += sname + '\n';
            getline(sssavedw, sname);
            temp += "NO\n";
            getline(sssavedw, sname);
            temp += sname;
            sssavedw.close();
            ofstream sssaved;
            sssaved.open("C://ProgramData//15nashki//saves//Saves.txt");
            sssaved << temp;
        }
        if (save3 == "NO")  cout << "3)_SAVE_IS_EMPTY_\n";
        else if (save3 != "NO" && fileExists(save3 + "save3.txt")){
            ifstream ttsave(save3 + "save3.txt");
            getline(ttsave, sname);
            cout << "3) " << sname << endl;
            ttsave.close();
        }
        else{
            cout << "\n3) File is deleted or incorrect!\nClearing memory...(Can be used)";
            temp = "";
            ifstream ttsavedw("C://ProgramData//15nashki//saves//Saves.txt");
            getline(ttsavedw, sname);
            temp += sname + '\n';
            getline(ttsavedw, sname);
            temp += sname + '\n';
            getline(ttsavedw, sname);
            temp += "NO";
            ttsavedw.close();
            ofstream ttsaved;
            ttsaved.open("C://ProgramData//15nashki//saves//Saves.txt");
            ttsaved << temp;
        }
        cout << "\n>";
        cin >> selection;
        if (selection == "1"){
            system("cls");
            cout << "Name your save: ";
            cin >> savename;
            ofstream ff(save1 + "save1.txt");
            ff << savename + '\n';
            for (int i = 0; i < sidesize; i++)
                for (int j = 0; j < sidesize; j++){
                    ff << game_map[i][j];
                    ff << '\n';
                }
            cout << "Saved successfully!";
            Sleep(700);
            playgo = false;
        }
        else if (selection == "2"){
            system("cls");
            cout << "Name your save: ";
            cin >> savename;
            ofstream fs(save1 + "save2.txt");
            fs << savename + '\n';
            for (int i = 0; i < sidesize; i++)
                for (int j = 0; j < sidesize; j++){
                    fs << game_map[i][j];
                    fs << '\n';
                }
            cout << "Saved successfully!";
            Sleep(700);
            playgo = false;
        }
        else if (selection == "3"){
            system("cls");
            cout << "Name your save: ";
            cin >> savename;
            ofstream ft(save1 + "save3.txt");
            ft << savename + '\n';
            for (int i = 0; i < sidesize; i++)
                for (int j = 0; j < sidesize; j++){
                    ft << game_map[i][j];
                    ft << '\n';
                }
            cout << "Saved successfully!";
            Sleep(700);
            playgo = false;
        }
        else{
            cout << "Wrong input!";
            Sleep(700);
        }
    }
    else
        get_direction();
}
void screen(){ // выводим игровую карту на экран
    system("cls");
    cout << "+----+----+----+----+\n";
    for (int i = 0; i < sidesize; i++){
        cout << "|";
        for (int j = 0; j < sidesize; j++){
            if (game_map[i][j] != 0)
                cout << " " << std::setw(2) << std::setfill('0') << game_map[i][j] << " |";
            else
                cout << "    |"; // нулевой элемент
        }
        cout << "\n+----+----+----+----+\n";
    }
    cout << char(QUIT) << " - quit without saving\n" << char(SAVEQUIT) << " - save & quit\n>";
}

void play(){
    playgo = true;
    create_right_map();
    do{
        screen();
        get_direction();
    } while (!check_map() && playgo);
    screen();
    if (check_map()){
        cout << "\nYOU WIN!\nPress space to go back to menu\n>";
        int s = static_cast<int> (_getch());
        while (s != 32)
            s = static_cast<int> (_getch());
    }
}

void Menu(){
    string selection = "";
    while (true){
        system("cls");
        cout << "New - play a new game\nContinue - continue last game\nOptions - game options\nExit - close console\n>";
        cin >> selection;
        if (selection == "New"){
            do{
                create_game_map();
            } while (check_map());
            play();
        }
        else if (selection == "Continue"){
            if (!fileExists("C://ProgramData//15nashki//saves//Saves.txt")){
                ofstream fpath;
                fpath.open("C://ProgramData//15nashki//saves//Saves.txt");
                fpath << "C://ProgramData//15nashki//saves//\nC://ProgramData//15nashki//saves//\nC://ProgramData//15nashki//saves//";
                fpath.close();
            }
            ifstream fsaves("C://ProgramData//15nashki//saves//Saves.txt");
            getline(fsaves, save1);
            getline(fsaves, save2);
            getline(fsaves, save3);
            fsaves.close();
            while (true){
                system("cls");
                string sname, temp;
                if (save1 == "NO")  cout << "1)_SAVE_IS_EMPTY_\n";
                else if (save1 != "NO" && fileExists(save1 + "save1.txt")){
                    ifstream fsave(save1 + "save1.txt");
                    getline(fsave, sname);
                    cout << "1) " << sname << endl;
                    fsave.close();
                }
                else{
                    cout << "File is deleted or incorrect!\nClearing memory...\n";
                    temp = "";
                    ifstream fsavedw("C://ProgramData//15nashki//saves//Saves.txt");
                    getline(fsavedw, sname);
                    temp += "C://ProgramData//15nashki//saves//\n";
                    getline(fsavedw, sname);
                    temp += sname + '\n';
                    getline(fsavedw, sname);
                    temp += sname;
                    fsavedw.close();
                    ofstream fsaved;
                    fsaved.open("C://ProgramData//15nashki//saves//Saves.txt");
                    fsaved << temp;
                }

                if (save2 == "NO")  cout << "2)_SAVE_IS_EMPTY_\n";
                else if (save2 != "NO" && fileExists(save2 + "save2.txt")){
                    ifstream ssave(save2 + "save2.txt");
                    getline(ssave, sname);
                    cout << "2) " << sname << endl;
                    ssave.close();
                }
                else{
                    cout << "File is deleted or incorrect!\nClearing memory...";
                    temp = "";
                    ifstream ssavedw("C://ProgramData//15nashki//saves//Saves.txt");
                    getline(ssavedw, sname);
                    temp += sname + '\n';
                    getline(ssavedw, sname);
                    temp += "C://ProgramData//15nashki//saves//\n";
                    getline(ssavedw, sname);
                    temp += sname;
                    ssavedw.close();
                    ofstream ssaved;
                    ssaved.open("C://ProgramData//15nashki//saves//Saves.txt");
                    ssaved << temp;
                }

                if (save3 == "NO")  cout << "3)_SAVE_IS_EMPTY_\n";
                else if (save3 != "NO" && fileExists(save3 + "save3.txt")){
                    ifstream tsave(save3 + "save3.txt");
                    getline(tsave, sname);
                    cout << "3) " << sname << endl;
                    tsave.close();
                }
                else{
                    cout << "File is deleted or incorrect!\nClearing memory...";
                    temp = "";
                    ifstream tsavedw("C://ProgramData//15nashki//saves//Saves.txt");
                    getline(tsavedw, sname);
                    temp += sname + '\n';
                    getline(tsavedw, sname);
                    temp += sname + '\n';
                    getline(tsavedw, sname);
                    temp += "C://ProgramData//15nashki//saves//";
                    tsavedw.close();
                    ofstream tsaved;
                    tsaved.open("C://ProgramData//15nashki//saves//Saves.txt");
                    tsaved << temp;
                }

                cout << "Back - go back to menu\n>";
                cin >> selection;
                if (selection == "Back")    break;
                else if (selection == "1" && fileExists(save1 + "save1.txt")){ // Проверка на возможеность сохранения 1 файла
                    ifstream firstsave(save1 + "save1.txt");
                    getline(firstsave, sname);
                    firstsave.close();
                    if (sname != "_SAVE_IS_EMPTY_"){
                        ifstream fsavep(save1 + "save1.txt");
                        getline(fsavep, sname);

                        for (int i = 0; i < sidesize; i++)
                            for (int j = 0; j < sidesize; j++){
                                getline(fsavep, temp);
                                game_map[i][j] = atoi(temp.c_str());
                                if (temp == "0"){
                                    zero.y = i;
                                    zero.x = j;
                                }
                            }
                        fsavep.close();
                        play();
                        break;
                    }
                }

                else if (selection == "2" && fileExists(save2 + "save2.txt")){ // Проверка на возможеность сохранения 2 файла 
                    ifstream firstsave(save1 + "save1.txt");
                    getline(firstsave, sname);
                    firstsave.close();
                    if (sname != "_SAVE_IS_EMPTY_"){
                        ifstream secondsave(save1 + "save1.txt");
                        getline(secondsave, sname);
                        secondsave.close();
                        if (sname != "_SAVE_IS_EMPTY_"){
                            ifstream ssavep(save2 + "save2.txt");
                            getline(ssavep, sname);
                            for (int i = 0; i < sidesize; i++)
                                for (int j = 0; j < sidesize; j++){
                                    getline(ssavep, temp);
                                    game_map[i][j] = atoi(temp.c_str());
                                    if (temp == "0"){
                                        zero.y = i;
                                        zero.x = j;
                                    }
                                }
                            ssavep.close();
                            play();
                            break;
                        }
                    }
                }

                else if (selection == "3" && fileExists(save3 + "save3.txt")){ // Проверка на возможеность сохранения 3 файла
                    ifstream tsavep(save3 + "save3.txt");
                    getline(tsavep, sname);
                    for (int i = 0; i < sidesize; i++)
                        for (int j = 0; j < sidesize; j++){
                            getline(tsavep, temp);
                            game_map[i][j] = atoi(temp.c_str());
                            if (temp == "0"){
                                zero.y = i;
                                zero.x = j;
                            }
                        }
                    tsavep.close();
                    play();
                    break;
                }

                else{
                    cout << "Wrong input!";
                    Sleep(700);
                }
            }
        }

        else if (selection == "Options"){ // Настройки управления
            while (true){
                system("cls");
                cout << "UP - change upmove button\t!try not to use s & q keys!\nDOWN - change downmove button\nRIGHT - change rightmove button\nLEFT - change leftmove button\nDischarge - set the original buttons\nBack - go back to menu\n>";
                cin >> selection;
                if (selection == "UP"){
                    cout << "Press the new key.";
                    UP = static_cast<int> (_getch());
                }
                else if (selection == "DOWN"){
                    cout << "Press the new key.";
                    DOWN = static_cast<int> (_getch());
                }
                else if (selection == "RIGHT"){
                    cout << "Press the new key.";
                    RIGHT = static_cast<int> (_getch());
                }
                else if (selection == "LEFT"){
                    cout << "Press the new key.";
                    LEFT = static_cast<int> (_getch());
                }
                else if (selection == "Discharge"){
                    cout << "Discharged successfully!";
                    UP = 72;
                    DOWN = 80;
                    RIGHT = 77;
                    LEFT = 75;
                    Sleep(700);
                }
                else    if (selection == "Back")   break;
                else{
                    cout << "Wrong input!";
                    Sleep(700);
                }
            }
        }
        else    if (selection == "Exit")   break; 
        else{
            cout << "Wrong input!";
            Sleep(700);
        }
    }
}

int main(){
    srand(static_cast<int>(time(NULL)));
    _mkdir("C://ProgramData//15nashki"); // Создание папки
    _mkdir("C://ProgramData//15nashki//saves");  // Создание файла с путями save 1/2/3
    if (!fileExists("C://ProgramData//15nashki//saves//save1.txt")){
        ofstream fi("C://ProgramData//15nashki//saves//save1.txt");
        fi << "_SAVE_IS_EMPTY_";
        fi.close();
    }
    if (!fileExists("C://ProgramData//15nashki//saves//save2.txt")){
        ofstream fo("C://ProgramData//15nashki//saves//save2.txt");
        fo << "_SAVE_IS_EMPTY_";
        fo.close();
    }
    if (!fileExists("C://ProgramData//15nashki//saves//save3.txt")){
        ofstream fa("C://ProgramData//15nashki//saves//save3.txt");
        fa << "_SAVE_IS_EMPTY_";
        fa.close();
    }
    Menu();
}