#ifndef API_H
#define API_H

#include <cstdint>
#include <string>
#include <vector>

// Все алгоритмы ДОЛЖНЫ экспортировать эти функции через extern "C"

extern "C" {
    // Возврат названия алгоритма шифрования
    const char* GetCipherName();
    
    // Проверка корректности ключа
    bool ValidateKey(const std::string &key);
    
    // Шифрование данных
    // inputData  - исходные данные
    // outputData - буфер для результата (размер >= inputSize)
    // dataSize   - размер данных в байтах
    // key        - ключ шифрования
    void EncryptData(const uint8_t *inputData, uint8_t *outputData, 
                    size_t dataSize, const std::string &key);
    
    // Дешифрование данных (аналогично EncryptData)
    void DecryptData(const uint8_t *inputData, uint8_t *outputData, 
                    size_t dataSize, const std::string &key);
}

#endif
