#include "funcs.h"
#include "cipher_plugin.h"
#include "utils.h"
#include "api.h"
#include <iostream>
#include <dlfcn.h>
#include <filesystem>
#include <iomanip>

using namespace std;
namespace fs = filesystem;

vector<CipherPlugin> loadedCiphers;

string getUserNameForCipher(const string &internalName) {
    if (internalName == "Vigenere Cipher") {
        return "Шифр Виженера";
    } else if (internalName == "Great Cipher (Louis XIV)") {
        return "Великий шифр (Louis XIV)";
    } else if (internalName == "Bigram Cipher (Tritemius)") {
        return "Биграммный шифр Тритемия";
    }
    return internalName;
}

bool LoadCipher(const string &libraryPath) {
    try {
        if (!fs::exists(libraryPath)) {
            cerr << "Ошибка загрузки библиотеки >> " << libraryPath << endl;
            return false;
        }

        void *handle = dlopen(libraryPath.c_str(), RTLD_LAZY);
        if (!handle) {
            cerr << "Ошибка загрузки библиотеки >> " << libraryPath << endl;
            cerr << "Деталь: " << dlerror() << endl;
            return false;
        }

        CipherPlugin plugin;
        plugin.handle = handle;

        plugin.getCipherName = (GetCipherName_t) dlsym(handle, "GetCipherName");
        plugin.validateKey = (ValidateKey_t) dlsym(handle, "ValidateKey");
        plugin.encryptData = (EncryptData_t) dlsym(handle, "EncryptData");
        plugin.decryptData = (DecryptData_t) dlsym(handle, "DecryptData");

        if (!plugin.getCipherName || !plugin.validateKey ||
            !plugin.encryptData || !plugin.decryptData) {
            cerr << "Библиотека не содержит все необходимые функции!" << endl;
            dlclose(handle);
            return false;
        }

        plugin.name = plugin.getCipherName();
        loadedCiphers.push_back(plugin);

        cout << "Загружен модуль >> " << plugin.name
             << " (" << getUserNameForCipher(plugin.name) << ")" << endl;
        return true;

    } catch (const exception& e) {
        cerr << "Неожиданная ошибка >> " << e.what() << endl;
        return false;
    }
}

void UnloadAllCiphers() {
    for (auto& plugin : loadedCiphers) {
        if (plugin.handle) {
            dlclose(plugin.handle);
            plugin.handle = nullptr;
        }
    }
    loadedCiphers.clear();
    cout << "Все модули выгружены" << endl;
}

void ProcessTextEncryption() {
    if (loadedCiphers.empty()) {
        cerr << "Нет загруженных шифров!" << endl;
        return;
    }

    cout << "\nДоступные алгоритмы:" << endl;
    for (size_t i = 0; i < loadedCiphers.size(); i++) {
        cout << (i + 1) << ". " << getUserNameForCipher(loadedCiphers[i].name) << endl;
    }

    int choice = getIntegerInput("Выберите алгоритм: ", 1, loadedCiphers.size());
    CipherPlugin &cipher = loadedCiphers[choice - 1];

    bool encrypt = getChoice("Операция:\n1. Шифровать\n2. Дешифровать\nВыбор: ", {1, 2}) == 1;

    string key = getLine("Введите ключ: ", false);

    if (!cipher.validateKey(key)) {
        cerr << "Некорректный ключ!" << endl;
        return;
    }

    string text = getLine("Введите текст: ", false);
    vector<uint8_t> input(text.begin(), text.end());
    vector<uint8_t> output(input.size());

    if (encrypt) {
        cipher.encryptData(input.data(), output.data(), input.size(), key);

        cout << "Результат (hex): ";
        for (uint8_t byte : output) {
            cout << hex << setw(2) << setfill('0') << (int)byte << " ";
        }
        cout << dec << endl;
    } else {
        cipher.decryptData(input.data(), output.data(), input.size(), key);

        string result(output.begin(), output.end());
        cout << "Результат: " << result << endl;
    }
}

void ProcessFileEncryption() {
    if (loadedCiphers.empty()) {
        cerr << "Нет загруженных шифров!" << endl;
        return;
    }

    cout << "\nДоступные алгоритмы:" << endl;
    for (size_t i = 0; i < loadedCiphers.size(); i++) {
        cout << (i + 1) << ". " << getUserNameForCipher(loadedCiphers[i].name) << endl;
    }

    int choice = getIntegerInput("Выберите алгоритм: ", 1, loadedCiphers.size());
    CipherPlugin &cipher = loadedCiphers[choice - 1];

    bool encrypt = getChoice("Операция:\n1. Шифровать\n2. Дешифровать\nВыбор: ", {1, 2}) == 1;

    string key = getLine("Введите ключ: ", false);
    if (!cipher.validateKey(key)) {
        cerr << "Некорректный ключ!" << endl;
        return;
    }

    string inputFile = getLine("Входной файл: ", false);
    string outputFile = getLine("Выходной файл: ", false);

    try {
        vector<unsigned char> inputDataUC = readFile(inputFile);
        vector<uint8_t> inputData(inputDataUC.begin(), inputDataUC.end());
        vector<uint8_t> outputData(inputData.size());

        if (encrypt) {
            cipher.encryptData(inputData.data(), outputData.data(), inputData.size(), key);
        } else {
            cipher.decryptData(inputData.data(), outputData.data(), inputData.size(), key);
        }

        vector<unsigned char> outputDataUC(outputData.begin(), outputData.end());
        writeFile(outputFile, outputDataUC);
        cout << "Файл обработан: " << outputFile << endl;

        createLogFile(outputFile, getUserNameForCipher(cipher.name),
                      encrypt ? "cipher" : "decipher",
                      key, inputFile, outputFile);

    } catch (const exception& e) {
        cerr << "Ошибка обработки файла >> " << e.what() << endl;
    }
}

void ShowKeyGenerator() {
    if (loadedCiphers.empty()) {
        cerr << "Нет загруженных шифров!" << endl;
        return;
    }

    cout << "\nДоступные алгоритмы:" << endl;
    for (size_t i = 0; i < loadedCiphers.size(); i++) {
        cout << (i + 1) << ". " << getUserNameForCipher(loadedCiphers[i].name) << endl;
    }

    string generatedKey = generateRandomKey(16);
    cout << "Сгенерированный ключ: " << generatedKey << endl;
}
