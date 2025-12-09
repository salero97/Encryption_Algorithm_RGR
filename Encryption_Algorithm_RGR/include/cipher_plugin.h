#ifndef CIPHER_PLUGIN_H
#define CIPHER_PLUGIN_H

#include <string>
#include <vector>
#include <cstdint>
#include <dlfcn.h>

// Типы для указателей на функции из API
typedef const char* (*GetCipherName_t)();
typedef bool (*ValidateKey_t)(const std::string &);
typedef void (*EncryptData_t)(const uint8_t *, uint8_t *, size_t, const std::string &);
typedef void (*DecryptData_t)(const uint8_t *, uint8_t *, size_t, const std::string &);

// Структура для хранения информации о загруженном плагине
struct CipherPlugin {
    void *handle;  // Указатель на загруженную .so библиотеку (из dlopen)
    std::string name;  // Название алгоритма
    
    // Указатели на функции из API
    GetCipherName_t getCipherName;
    ValidateKey_t validateKey;
    EncryptData_t encryptData;
    DecryptData_t decryptData;
};

// Глобальная коллекция всех загруженных плагинов
extern std::vector<CipherPlugin> loadedCiphers;

#endif
