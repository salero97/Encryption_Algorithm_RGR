#include "utils.h"
#include "vigenere/vigenere_cipher_interface.h"
#include "great/great_cipher_interface.h"
#include "bigram/bigram_cipher_interface.h"
#include "funcs.h"
#include "cipher_plugin.h"
#include <iostream>
#include <filesystem>
#include <limits>

using namespace std;
namespace fs = filesystem;

extern vector<CipherPlugin> loadedCiphers;

struct CipherMenuItem {
    string internalName;
    string userName;
    int menuNumber;
    bool available;
};

bool hasPlugin(const string &internalName) {
    for (const auto &p : loadedCiphers) {
        if (p.name == internalName) return true;
    }
    return false;
}

bool checkPassword() {
    const string correctPassword = "securepass";
    int attemptsLeft = 3;

    while (attemptsLeft > 0) {
        cout << "Осталось попыток: " << attemptsLeft << endl;
        string password = getLine("Введите пароль: ", true);
        if (password == correctPassword) {
            cout << "Доступ разрешён!" << endl;
            return true;
        } else {
            attemptsLeft--;
            if (attemptsLeft > 0) {
                cout << "Неверный пароль. Повторите попытку." << endl;
            }
        }
    }

    cout << "Превышено количество попыток. Доступ запрещён." << endl;
    return false;
}

int main() {
    initLocale();

    cout << "Загрузка криптомодулей (.so)..." << endl;

    if (fs::exists("libs")) {
        for (const auto &entry : fs::directory_iterator("libs")) {
            if (entry.path().extension() == ".so") {
                if (!LoadCipher(entry.path().string())) {
                    string libName = entry.path().filename().string();
                    cout << "Модуль из библиотеки " << libName
                         << " не был загружен и будет недоступен для использования." << endl;
                }
            }
        }
    } else {
        cerr << "Папка libs/ не найдена." << endl;
    }

    cout << "Загруженные криптомодули (.so):" << endl;
    if (loadedCiphers.empty()) {
        cout << "  (нет загруженных модулей, используются встроенные реализации)" << endl;
    } else {
        for (const auto &plugin : loadedCiphers) {
            cout << "  - " << plugin.name
                 << " (" << getUserNameForCipher(plugin.name) << ")" << endl;
        }
    }
    cout << endl;

    if (!checkPassword()) {
        UnloadAllCiphers();
        return 1;
    }

    vector<CipherMenuItem> menuItems = {
        { "Vigenere Cipher",           "Шифр Виженера",             1, true },
        { "Great Cipher (Louis XIV)",  "Великий шифр (Louis XIV)",  2, true },
        { "Bigram Cipher (Tritemius)", "Биграммный шифр Тритемия",  3, true }
    };

    bool running = true;
    while (running) {
        cout << "Выберите шифр:" << endl;
        for (auto &item : menuItems) {
            bool pluginPresent = hasPlugin(item.internalName);
            item.available = pluginPresent;
            cout << item.menuNumber << ". " << item.userName;
            if (!pluginPresent) {
                cout << " [НЕДОСТУПЕН: модуль не найден]";
            }
            cout << endl;
        }
        cout << "4. Выход" << endl;
        cout << "Ваш выбор: ";

        int choice = getIntegerInput("", 1, 4);

        if (choice == 4) {
            running = false;
        } else {
            CipherMenuItem *selected = nullptr;
            for (auto &item : menuItems) {
                if (item.menuNumber == choice) {
                    selected = &item;
                    break;
                }
            }

            if (!selected) {
                cout << "Неверный выбор." << endl;
            } else if (!selected->available) {
                cout << "Ошибка: модуль \"" << selected->userName
                     << "\" не загружен. Шифрование данным алгоритмом временно недоступно из-за технического сбоя." << endl;
            } else {
                if (choice == 1) {
                    runVigenereCipher();
                } else if (choice == 2) {
                    runGreatCipher();
                } else if (choice == 3) {
                    runBigramCipher();
                }
            }
        }

        if (running) {
            cout << "\n\nНажмите Enter для продолжения...";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    UnloadAllCiphers();
    return 0;
}
