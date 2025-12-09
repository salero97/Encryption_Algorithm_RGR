#ifndef FUNCS_H
#define FUNCS_H

#include <string>
#include <vector>

bool LoadCipher(const std::string &libraryPath);

void UnloadAllCiphers();

void ProcessTextEncryption();

void ProcessFileEncryption();

void ShowKeyGenerator();

std::string getUserNameForCipher(const std::string &internalName);

#endif
