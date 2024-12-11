/*

Начало - 11.12.2024

Сделана начальная структра проекта 


Требования

3. Составить программу «Финальная часть Чемпионата мира». Структура
исходных данных: стадия чемпионата (1/8, четвертьфинал, полуфинал, финал),
название команды 1, название команды 2, количество мячей, забитое 1 и 2 ко-
10
мандой соответственно, фамилии игроков забивших голы (по командам соответственно). Сформировать список команд, отсортированный по количеству
мячей, забитых на чемпионате. Вывести десятку лучших бомбардиров финальной части чемпионата (с указанием страны). Вывести список призеров чемпионата. Предусмотреть защиту от некорректного ввода (т.е. команда, проигравшая
в 1/8 финала, не может играть в последующих турах и т.п.).

Таблица 1 - Требования к курсовой работе на тему «Формирование текстового файла.
Требования
1. программа должна иметь модульную структуру, т.е. должна состоять
из отдельных функций
2. формирование файла (запись введенных данных в файл);
3. редактирование данных (добавление, удаление, изменение сведений);
4. использование отдельного диалога для ввода и проверка корректности
всех входных данных;
5. вывод исходных данных (просмотр всех сведений) и реализация всех
указанных в задании действий по сортировке и выборке данных;
6. использование главного и контекстного меню;
7. результаты расчетов и поиска должны быть оформлены в соответствующем виде (с использованием форматированного вывода);
8. наличие в главном меню раздела «О программе», в котором указать
версию программного продукта, дату внесения последних изменений в программе и координаты автора;
9. наличие в главном меню раздела «Руководство пользователя (Справка)», содержание которого загружается из текстового файла;
10. наличие в главном меню раздела «Задание», содержащего постановку
задачи.
11. корректная обработка диалога работы с файлами (например, запрет открытия несуществующего файла, либо запрос на перезапись уже существующего файла, настройка фильтров и др.);
12. проверка сохранения сведений в файле при выходе из программы;
13. сортировка выводимой информации по любому из полей записи (критерий сортировки указывается в меню), используя одну процедуру сортировки
для всех пунктов меню;


by MMXXXII - https://github.com/MMXXXII


*/


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <windows.h>

using namespace std;

// Структуры для хранения информации о матчах и игроках
struct Player {
    string name;
    string country;
    int goals;

    Player(string n, string c, int g) : name(n), country(c), goals(g) {}
};

struct Match {
    string stage;
    string team1;
    string team2;
    int goals1;
    int goals2;
    vector<Player> team1_players;
    vector<Player> team2_players;

    Match(string s, string t1, string t2, int g1, int g2)
        : stage(s), team1(t1), team2(t2), goals1(g1), goals2(g2) {}
};

void inputTeamData(string& teamName, int& goals, vector<Player>& players) {
    string player_name, player_country;
    int player_goals;
    int total_goals = 0;

    cout << "Введите название команды: ";
    cin >> teamName;
    cout << "Введите количество мячей, забитых командой: ";
    cin >> goals;

    cout << "Введите фамилии игроков, забивших голы для команды " << teamName << " (для завершения введите 'end'):\n";

    // Ввод данных игроков
    int player_number = 1;  // Нумерация игроков
    while (total_goals < goals) {  // Цикл продолжается, пока не введено достаточно голов
        cout << "Футболист №" << player_number << ":\n";  // Нумерация футболистов
        cout << "Фамилия игрока: ";
        cin >> player_name;

        // Завершаем ввод, если введено 'end'
        if (player_name == "end") {
            if (total_goals == goals) {
                break;  // Если количество голов совпадает, завершить ввод
            }
            else {
                cout << "Ошибка: количество голов игроков не совпадает с количеством голов команды. Попробуйте снова.\n";
                players.clear();  // Очистим введенные данные, чтобы начать ввод заново
                return;  // Завершаем выполнение функции
            }
        }

        cout << "Страна игрока: ";
        cin >> player_country;

        // Запрос количества голов
        while (true) {
            cout << "Количество голов: ";
            cin >> player_goals;

            // Проверка, не превышает ли количество голов, введенное игроками, количество голов команды
            if (total_goals + player_goals > goals) {
                cout << "Ошибка: количество голов у игроков превышает количество голов команды! Попробуйте снова.\n";
            }
            else {
                break;  // Выход из цикла, если количество голов корректно
            }
        }

        // Добавляем игрока в список
        players.push_back(Player(player_name, player_country, player_goals));
        total_goals += player_goals;
        player_number++;  // Увеличиваем номер игрока
    }

    // Финальная проверка на правильность данных
    if (total_goals != goals) {
        cout << "Ошибка: количество голов игроков не совпадает с количеством голов команды. Попробуйте снова.\n";
        players.clear();  // Очистим введенные данные, чтобы начать ввод заново
    }
}




// Функция для ввода данных о матче
void inputMatchData(vector<Match>& matches) {
    string stage, team1, team2;
    int goals1, goals2;

    // Ввод данных о матче
    cout << "Введите стадию матча (например, 1/8, 1/4, полуфинал, финал): ";
    cin >> stage;

    vector<Player> team1_players, team2_players;

    // Ввод данных для команды 1
    cout << "\nВведите данные для команды 1:\n";  // Уведомление о начале ввода первой команды
    inputTeamData(team1, goals1, team1_players);

    // Ввод данных для команды 2
    cout << "\nВведите данные для команды 2:\n";  // Уведомление о начале ввода второй команды
    inputTeamData(team2, goals2, team2_players);

    // Добавление матча в список
    matches.push_back(Match(stage, team1, team2, goals1, goals2));
    matches.back().team1_players = team1_players;
    matches.back().team2_players = team2_players;
}

// Функция для вывода данных о матче
void displayMatchData(const vector<Match>& matches) {
    for (const auto& match : matches) {
        cout << "Стадия: " << match.stage << "\n";
        cout << "Матч: " << match.team1 << " vs " << match.team2 << "\n";
        cout << "Результат: " << match.goals1 << " - " << match.goals2 << "\n";

        cout << "Игроки команды " << match.team1 << " (с забитыми голами):\n";
        for (const auto& player : match.team1_players) {
            cout << "  " << player.name << " (" << player.country << "): " << player.goals << " гол(ов)\n";
        }

        cout << "Игроки команды " << match.team2 << " (с забитыми голами):\n";
        for (const auto& player : match.team2_players) {
            cout << "  " << player.name << " (" << player.country << "): " << player.goals << " гол(ов)\n";
        }

        cout << "\n";
    }
}

// Функция для сортировки команд по количеству забитых голов
void sortTeamsByGoals(vector<Match>& matches) {
    vector<pair<string, int>> team_goals;

    for (const auto& match : matches) {
        team_goals.push_back({ match.team1, match.goals1 });
        team_goals.push_back({ match.team2, match.goals2 });
    }

    sort(team_goals.begin(), team_goals.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
        });

    cout << "Команды, отсортированные по количеству забитых голов:\n";
    for (const auto& team : team_goals) {
        cout << "Команда: " << team.first << ", Голов: " << team.second << "\n";
    }
    cout << "\n";
}

// Функция для вывода десятки лучших бомбардиров
void displayTopScorers(const vector<Match>& matches) {
    vector<Player> all_players;

    for (const auto& match : matches) {
        all_players.insert(all_players.end(), match.team1_players.begin(), match.team1_players.end());
        all_players.insert(all_players.end(), match.team2_players.begin(), match.team2_players.end());
    }

    sort(all_players.begin(), all_players.end(), [](const Player& a, const Player& b) {
        return a.goals > b.goals;
        });

    cout << "Десятка лучших бомбардиров:\n";
    for (int i = 0; i < 10 && i < all_players.size(); i++) {
        cout << "Игрок: " << all_players[i].name << " (" << all_players[i].country << ") - " << all_players[i].goals << " гол(ов)\n";
    }
    cout << "\n";
}

// Функция для сохранения данных в файл
void saveToFile(const vector<Match>& matches) {
    ofstream file("matches.txt");
    if (!file) {
        cout << "Не удалось открыть файл для записи.\n";
        return;
    }

    for (const auto& match : matches) {
        file << match.stage << "\n";
        file << match.team1 << "\n";
        file << match.team2 << "\n";
        file << match.goals1 << " " << match.goals2 << "\n";

        for (const auto& player : match.team1_players) {
            file << player.name << " " << player.country << " " << player.goals << "\n";
        }

        for (const auto& player : match.team2_players) {
            file << player.name << " " << player.country << " " << player.goals << "\n";
        }
    }

    cout << "Данные успешно сохранены в файл.\n";
}

// Функция для загрузки данных из файла
void loadFromFile(vector<Match>& matches) {
    ifstream file("matches.txt");
    if (!file) {
        cout << "Не удалось открыть файл для чтения.\n";
        return;
    }

    string stage, team1, team2;
    int goals1, goals2;
    while (getline(file, stage)) {
        getline(file, team1);
        getline(file, team2);
        file >> goals1 >> goals2;
        file.ignore();

        vector<Player> team1_players, team2_players;
        string player_name, player_country;
        int player_goals;

        while (file >> player_name) {
            if (player_name == "end") break;
            file >> player_country >> player_goals;
            team1_players.push_back(Player(player_name, player_country, player_goals));
        }

        while (file >> player_name) {
            if (player_name == "end") break;
            file >> player_country >> player_goals;
            team2_players.push_back(Player(player_name, player_country, player_goals));
        }

        matches.push_back(Match(stage, team1, team2, goals1, goals2));
        matches.back().team1_players = team1_players;
        matches.back().team2_players = team2_players;
    }

    cout << "Данные успешно загружены из файла.\n";
}

// Основная функция для работы с меню
int main() {
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    vector<Match> matches;
    int choice;

    while (true) {
        cout << "Меню:\n";
        cout << "1. Ввести данные о матче\n";
        cout << "2. Показать данные о матчах\n";
        cout << "3. Отсортировать команды по количеству голов\n";
        cout << "4. Показать десятку лучших бомбардиров\n";
        cout << "5. Сохранить данные в файл\n";
        cout << "6. Загрузить данные из файла\n";
        cout << "7. Выход\n";
        cout << "Введите ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
            inputMatchData(matches);
            break;
        case 2:
            displayMatchData(matches);
            break;
        case 3:
            sortTeamsByGoals(matches);
            break;
        case 4:
            displayTopScorers(matches);
            break;
        case 5:
            saveToFile(matches);
            break;
        case 6:
            loadFromFile(matches);
            break;
        case 7:
            cout << "Выход...\n";
            return 0;
        default:
            cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }

    return 0;
}
