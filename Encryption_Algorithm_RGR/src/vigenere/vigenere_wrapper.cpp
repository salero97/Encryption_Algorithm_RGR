#include "vigenere/vigenere_cipher.h"
#include "api.h"
#include <cstring>

extern "C" {
    
    const char* GetCipherName() {
        return "Vigenere Cipher";
    }
    
    bool ValidateKey(const std::string &key) {
        return !key.empty() && key.length() <= 256;
    }
    
    void EncryptData(const uint8_t *inputData, uint8_t *outputData, 
                    size_t dataSize, const std::string &key) {
        if (!inputData || !outputData || dataSize == 0) return;
        
        std::vector<unsigned char> input(inputData, inputData + dataSize);
        std::vector<unsigned char> encrypted = vigenereEncrypt(input, key);
        
        std::memcpy(outputData, encrypted.data(), encrypted.size());
    }
    
    void DecryptData(const uint8_t *inputData, uint8_t *outputData, 
                    size_t dataSize, const std::string &key) {
        if (!inputData || !outputData || dataSize == 0) return;
        
        std::vector<unsigned char> input(inputData, inputData + dataSize);
        std::vector<unsigned char> decrypted = vigenereDecrypt(input, key);
        
        std::memcpy(outputData, decrypted.data(), decrypted.size());
    }
}
