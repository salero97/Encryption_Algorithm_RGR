#include "bigram/bigram_cipher.h"
#include "api.h"
#include <cstring>

extern "C" {
    
    const char* GetCipherName() {
        return "Bigram Cipher (Tritemius)";
    }
    
    bool ValidateKey(const std::string &key) {
        return !key.empty() && key.length() <= 256;
    }
    
    void EncryptData(const uint8_t *inputData, uint8_t *outputData, 
                    size_t dataSize, const std::string &key) {
        if (!inputData || !outputData || dataSize == 0) return;
        
        std::vector<uint8_t> input(inputData, inputData + dataSize);
        std::vector<uint8_t> encrypted = BigramCipher::processBytes(input, key, true);
        
        std::memcpy(outputData, encrypted.data(), encrypted.size());
    }
    
    void DecryptData(const uint8_t *inputData, uint8_t *outputData, 
                    size_t dataSize, const std::string &key) {
        if (!inputData || !outputData || dataSize == 0) return;
        
        std::vector<uint8_t> input(inputData, inputData + dataSize);
        std::vector<uint8_t> decrypted = BigramCipher::processBytes(input, key, false);
        
        std::memcpy(outputData, decrypted.data(), decrypted.size());
    }
}
