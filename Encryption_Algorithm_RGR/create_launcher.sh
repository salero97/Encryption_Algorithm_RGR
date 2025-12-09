#!/bin/bash

PROJECT_DIR="/home/salero/Encryption_Algorithm_RGR"
EXECUTABLE="$PROJECT_DIR/bin/Encryption_Algorithm_RGR"
LIBS_DIR="$PROJECT_DIR/libs"

echo "=== Создаю глобальный лаунчер для Encryption_Algorithm_RGR ==="

echo "Проверяю наличие исполняемого файла..."
if [ ! -f "$EXECUTABLE" ]; then
    echo "❌ ОШИБКА: Файл $EXECUTABLE не найден!"
    echo "Сначала соберите проект: cd $PROJECT_DIR && make"
    exit 1
fi

echo "✅ Исполняемый файл найден: $EXECUTABLE"

# Создаем лаунчер в /usr/local/bin для Encryption_Algorithm_RGR
echo "Создаю глобальный лаунчер в /usr/local/bin..."
sudo bash -c "cat > '/usr/local/bin/Encryption_Algorithm_RGR' << 'EOF'
#!/bin/bash
# Переходим в директорию проекта, чтобы найти libs/
cd \"$PROJECT_DIR\"
exec \"$EXECUTABLE\" \"\$@\"
EOF"

sudo chmod +x "/usr/local/bin/Encryption_Algorithm_RGR"

# Создаем короткий алиас EAR
echo "Создаю короткий алиас EAR..."
sudo bash -c "cat > '/usr/local/bin/EAR' << 'EOF'
#!/bin/bash
# Переходим в директорию проекта, чтобы найти libs/
cd \"$PROJECT_DIR\"
exec \"$EXECUTABLE\" \"\$@\"
EOF"

sudo chmod +x "/usr/local/bin/EAR"

# Обновляем .desktop файл
echo "Обновляю .desktop файл..."
mkdir -p ~/.local/share/applications
cat > ~/.local/share/applications/Encryption_Algorithm_RGR.desktop << EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=Encryption Algorithm RGR
GenericName=Encryption Tool
Comment=Program for encryption and decryption
Exec=/usr/local/bin/Encryption_Algorithm_RGR
Icon=dialog-password
Categories=Utility;Security;
Terminal=true
StartupNotify=true
Path=$PROJECT_DIR
EOF

update-desktop-database ~/.local/share/applications 2>/dev/null || true

echo ""
echo "=== Всё готово! ==="
echo "✅ Создан глобальный лаунчер: /usr/local/bin/Encryption_Algorithm_RGR"
echo "✅ Создан короткий алиас: /usr/local/bin/EAR"
echo "✅ Обновлен ярлык в меню приложений"
echo ""
echo "Теперь вы можете запускать программу из любой директории:"
echo "  Encryption_Algorithm_RGR"
echo "  или коротко: EAR"
echo "Программа будет запускаться из директории проекта: $PROJECT_DIR"