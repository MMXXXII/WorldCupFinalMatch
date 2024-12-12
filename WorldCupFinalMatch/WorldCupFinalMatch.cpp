#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <windows.h>
#include <filesystem>
#include <regex>
#include <iomanip>

using namespace std;
namespace fs = std::filesystem;

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

// Функция для проверки корректности имени команды с использованием регулярных выражений
bool isValidTeamName(const string& teamName) {
    // Регулярное выражение: первая буква заглавная, все остальные буквы могут быть как заглавными, так и строчными (включая кириллицу)
    regex pattern("^[А-ЯЁ][а-яёA-Za-z]*$");

    // Проверяем строку с регулярным выражением
    return regex_match(teamName, pattern);
}

// Функция для проверки корректности страны
bool isValidCountryName(const string& countryName) {
    regex pattern("^[А-ЯЁ][а-яёA-Za-z]*$");
    return regex_match(countryName, pattern);
}

// Функция для проверки корректности фамилии футболиста
bool isValidPlayerName(const string& playerName) {
    regex pattern("^[А-ЯЁ][а-яёA-Za-z]*$");
    return regex_match(playerName, pattern);
}


// Функция для проверки корректности числа голов
bool isValidGoalsInput(int& goals) {
    while (true) {
        string input;
        cout << "Введите количество мячей, забитых командой (от 0 до 50): ";
        cin >> input;

        // Проверка, что введенная строка состоит только из цифр
        if (input.empty() || input.find_first_not_of("0123456789") != string::npos) {
            cout << "Ошибка: введено некорректное значение! Введите только цифры.\n";
            continue;
        }

        // Преобразуем строку в число
        goals = stoi(input);

        if (goals < 0 || goals >= 50) {  // Проверка, что количество голов в пределах допустимого диапазона
            cout << "Ошибка: количество голов должно быть от 0 до 50. Попробуйте снова.\n";
        }
        else {
            return true;  // Ввод корректен
        }
    }
}
// Функция для ввода фамилий игроков
void inputPlayersData(vector<Player>& players, int goals) {
    string playerName, playerCountry;
    int total_goals = 0;
    int player_goals;

    cout << "Введите фамилии игроков, забивших голы для команды:\n";
    while (total_goals < goals) {
        cout << "Футболист №" << players.size() + 1 << ":\n";

        // Проверка имени игрока
        while (true) {
            cout << "Фамилия игрока: ";
            cin >> playerName;

            // Проверка корректности фамилии
            if (!isValidPlayerName(playerName)) {
                cout << "Ошибка: Фамилия игрока должна начинаться с заглавной буквы и содержать только буквы. Попробуйте снова.\n";
            }
            else {
                break;
            }
        }

        cout << "Страна игрока: ";
        cin >> playerCountry;

        // Проверка на корректность страны
        while (!isValidCountryName(playerCountry)) {
            cout << "Ошибка: Страна должна начинаться с заглавной буквы и содержать только буквы. Попробуйте снова.\n";
            cout << "Страна игрока: ";
            cin >> playerCountry;
        }

        while (true) {
            cout << "Количество голов: ";
            if (!(cin >> player_goals)) {
                cout << "Ошибка: введите корректное количество голов.\n";
                cin.clear();
                cin.ignore();
                continue;
            }

            if (total_goals + player_goals > goals) {
                cout << "Ошибка: количество голов у игроков превышает количество голов команды! Попробуйте снова.\n";
            }
            else {
                break;
            }
        }

        players.push_back(Player(playerName, playerCountry, player_goals));
        total_goals += player_goals;
    }

    // Проверка, что количество голов игроков совпадает с количеством голов команды
    if (total_goals != goals) {
        cout << "Ошибка: количество голов игроков не совпадает с количеством голов команды. Попробуйте снова.\n";
        players.clear();
    }
}

// Функция для ввода данных о команде
void inputTeamData(string& teamName, int& goals, vector<Player>& players) {
    string player_name, player_country;
    int player_goals;
    int total_goals = 0;

    // Ввод имени команды с проверкой
    while (true) {
        cout << "Введите название команды: ";
        cin >> teamName;

        if (isValidTeamName(teamName)) {
            break;
        }
        else {
            cout << "Ошибка: Название команды должно начинаться с заглавной буквы и содержать только буквы. Попробуйте снова.\n";
        }
    }

    // Ввод количества голов с проверкой
    while (!isValidGoalsInput(goals)) {}

    // Ввод данных о игроках
    inputPlayersData(players, goals);

    // Проверка, что количество голов игроков совпадает с количеством голов команды
    int total_player_goals = 0;
    for (const auto& player : players) {
        total_player_goals += player.goals;
    }

    if (total_player_goals != goals) {
        cout << "Ошибка: количество голов игроков не совпадает с количеством голов команды. Попробуйте снова.\n";
        players.clear();
    }
}


void displayMatchData(const vector<Match>& matches) {
    if (matches.empty()) {
        cout << "Нет данных о матчах.\n";
        return;
    }

    int matchNumber = 1;
    for (const auto& match : matches) {
        // Выводим номер матча и стадию
        cout << "\nМатч " << matchNumber++ << endl;
        cout << "Стадия: " << match.stage << endl;

        // Заголовки таблицы
        cout << "\n| " << setw(20) << left << "Команда"
            << "| " << setw(20) << left << "Название команды"
            << "| " << setw(20) << left << "Футболисты"
            << "| " << setw(20) << left << "Страна"
            << "| " << setw(15) << left << "Голы футболистов" << " |" << endl;

        cout << string(100, '-') << endl;

        // Вывод команды 1
        cout << "| " << setw(20) << left << "Команда 1"
            << "| " << setw(20) << left << match.team1
            << "| " << setw(20) << left << " " << "| " << setw(20) << left << " " << "| " << endl;

        // Игроки команды 1
        for (const auto& player : match.team1_players) {
            cout << "| " << setw(20) << left << " "
                << "| " << setw(20) << left << player.name
                << "| " << setw(20) << left << player.name
                << "| " << setw(20) << left << player.country
                << "| " << setw(10) << left << player.goals
                << " |" << endl;
        }

        // Разделитель между командами
        cout << string(100, '-') << endl;

        // Вывод команды 2
        cout << "| " << setw(20) << left << "Команда 2"
            << "| " << setw(20) << left << match.team2
            << "| " << setw(20) << left << " " << "| " << setw(20) << left << " " << "| " << endl;

        // Игроки команды 2
        for (const auto& player : match.team2_players) {
            cout << "| " << setw(20) << left << " "
                << "| " << setw(20) << left << player.name
                << "| " << setw(20) << left << player.name
                << "| " << setw(20) << left << player.country
                << "| " << setw(10) << left << player.goals
                << " |" << endl;
        }

        // Разделитель после вывода
        cout << string(100, '-') << endl;
    }
}


// Функция для ввода данных о матче
void inputMatchData(vector<Match>& matches) {
    string stage, team1, team2;
    int goals1, goals2;

    // Проверка на корректность стадии матча
    while (true) {
        cout << "Введите стадию матча (например, 1/8, 1/4, полуфинал, финал): ";
        cin >> stage;

        if (stage == "1/8" || stage == "1/4" || stage == "полуфинал" || stage == "финал" || stage == "Полуфинал" || stage == "Финал") {
            break;
        }
        else {
            cout << "Ошибка: Неверная стадия матча. Попробуйте снова.\n";
        }
    }

    vector<Player> team1_players, team2_players;

    cout << "\nВведите данные для команды 1:\n";
    inputTeamData(team1, goals1, team1_players);

    cout << "\nВведите данные для команды 2:\n";
    inputTeamData(team2, goals2, team2_players);

    matches.push_back(Match(stage, team1, team2, goals1, goals2));
    matches.back().team1_players = team1_players;
    matches.back().team2_players = team2_players;
}



// Функция для редактирования данных (добавление, удаление, изменение)
void editMatchData(vector<Match>& matches) {
    int choice;
    cout << "Редактирование данных о матче:\n";
    cout << "1. Добавить матч\n";
    cout << "2. Удалить матч\n";
    cout << "3. Изменить данные матча\n";
    cout << "Введите ваш выбор: ";
    cin >> choice;

    switch (choice) {
    case 1: {
        inputMatchData(matches);
        break;
    }
    case 2: {
        if (matches.empty()) {
            cout << "Нет матчей для удаления.\n";
            break;
        }

        cout << "Список матчей:\n";
        displayMatchData(matches);

        int match_number;
        cout << "Введите номер матча для удаления (начиная с 1): ";
        cin >> match_number;

        if (match_number < 1 || match_number > matches.size()) {
            cout << "Неверный номер матча.\n";
        }
        else {
            matches.erase(matches.begin() + match_number - 1);
            cout << "Матч удален.\n";
        }
        break;
    }
    case 3: {
        int match_number;
        cout << "Введите номер матча для изменения (начиная с 1): ";
        cin >> match_number;

        if (match_number < 1 || match_number > matches.size()) {
            cout << "Неверный номер матча.\n";
        }
        else {
            Match& match = matches[match_number - 1];
            cout << "Редактирование данных для матча: " << match.team1 << " vs " << match.team2 << "\n";
            cout << "Введите новую стадию матча: ";
            cin >> match.stage;

            vector<Player> team1_players, team2_players;
            inputTeamData(match.team1, match.goals1, team1_players);
            inputTeamData(match.team2, match.goals2, team2_players);

            match.team1_players = team1_players;
            match.team2_players = team2_players;
            cout << "Данные матча обновлены.\n";
        }
        break;
    }
    default:
        cout << "Неверный выбор.\n";
    }
}

// Функция для проверки допустимости имени файла
bool isValidFileName(const string& filename) {
    // Проверка на недопустимые символы
    if (filename.empty() || filename.find_first_of("<>:\"/\\|?*") != string::npos) {
        cout << "Ошибка: Недопустимое имя файла.\n";
        return false;
    }

    // Проверка на слишком короткое имя файла
    if (filename.size() < 5) {  // Минимум "a.txt" (4 символа + точка)
        cout << "Ошибка: Имя файла слишком короткое.\n";
        return false;
    }

    // Проверка расширения
    size_t dotPos = filename.rfind('.');  // Находим последнюю точку
    if (dotPos == string::npos || dotPos == filename.size() - 1) {
        cout << "Ошибка: Файл не имеет расширения.\n";
        return false;
    }

    string extension = filename.substr(dotPos);  // Получаем расширение
    if (extension != ".txt") {
        cout << "Ошибка: Неверное расширение файла. Требуется .txt.\n";
        return false;
    }

    return true;
}

#include <filesystem>  // Для работы с путями

// Функция для сохранения данных в файл
void saveToFile(const vector<Match>& matches) {
    string filename;
    cout << "Введите имя файла для сохранения данных (с расширением .txt): ";
    cin >> filename;

    // Проверка на правильное расширение файла
    while (filename.find(".txt") == string::npos) {
        cout << "Ошибка: имя файла должно заканчиваться на .txt. Попробуйте снова: ";
        cin >> filename;
    }

    // Проверка на существование файла
    if (fs::exists(filename)) {
        char choice;
        cout << "Файл уже существует. Перезаписать? (y/n): ";
        cin >> choice;
        if (choice != 'y' && choice != 'Y') {
            cout << "Сохранение отменено.\n";
            return;
        }
    }

    // Открытие файла для записи
    ofstream file(filename);
    if (!file) {
        cout << "Не удалось открыть файл для записи.\n";
        return;
    }

    // Запись данных в файл
    for (const auto& match : matches) {
        file << match.stage << "\n";
        file << match.team1 << "\n";  // Первая команда
        file << match.team2 << "\n";  // Вторая команда
        file << match.goals1 << " " << match.goals2 << "\n";  // Количество голов

        // Данные о первой команде
        file << "Игроки команды " << match.team1 << ":\n";
        for (const auto& player : match.team1_players) {
            file << player.name << " " << player.country << " " << player.goals << "\n";
        }

        // Данные о второй команде
        file << "Игроки команды " << match.team2 << ":\n";
        for (const auto& player : match.team2_players) {
            file << player.name << " " << player.country << " " << player.goals << "\n";
        }
    }

    // Печать полного пути к файлу
    filesystem::path fullPath = filesystem::absolute(filename);
    cout << "Данные успешно сохранены в файл: " << fullPath << "\n";
}



// Функция для загрузки данных из файла
void loadFromFile(vector<Match>& matches) {
    string filename;
    cout << "Введите имя файла для загрузки данных (с расширением .txt): ";
    cin >> filename;

    // Проверка на правильное расширение файла
    while (!isValidFileName(filename)) {
        cout << "Ошибка: имя файла должно заканчиваться на .txt. Попробуйте снова: ";
        cin >> filename;
    }

    if (!fs::exists(filename)) {
        cout << "Ошибка: файл не найден.\n";
        return;
    }

    ifstream file(filename);
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
        cout << "1. Редактировать данные о матче\n";
        cout << "2. Показать данные о матчах\n";
        cout << "3. Сохранить данные в файл\n";
        cout << "4. Загрузить данные из файла\n";
        cout << "5. Выход\n";
        cout << "Введите ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
            editMatchData(matches);
            break;
        case 2:
            displayMatchData(matches);
            break;
        case 3:
            saveToFile(matches);
            break;
        case 4:
            loadFromFile(matches);
            break;
        case 5:
            cout << "Выход...\n";
            return 0;
        default:
            cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }

    return 0;
}
