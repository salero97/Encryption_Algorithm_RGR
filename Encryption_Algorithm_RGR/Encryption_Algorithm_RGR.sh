#!/bin/bash

# Скрипт для запуска программы с правильными переменными окружения

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
APP_PATH="$SCRIPT_DIR/bin/Encryption_Algorithm_RGR"
LIBS_PATH="$SCRIPT_DIR/libs"

# Устанавливаем LD_LIBRARY_PATH для поиска .so файлов
export LD_LIBRARY_PATH="$LIBS_PATH:$LD_LIBRARY_PATH"

# Проверяем существование программы
if [ ! -f "$APP_PATH" ]; then
    echo "❌ Ошибка: Программа не найдена в $APP_PATH"
    echo "Пожалуйста, сначала скомпилируйте проект: make"
    exit 1
fi

# Проверяем существование папки libs
if [ ! -d "$LIBS_PATH" ]; then
    echo "❌ Ошибка: Папка libs/ не найдена"
    exit 1
fi

# Запускаем программу
exec "$APP_PATH" "$@"
