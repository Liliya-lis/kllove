#include <filesystem.h>
#include <stdio.h>

int main() {
    // Создаем новый файл
    if (create_file("test.txt") == 0) {
        printf("Файл успешно создан\n");
    } else {
        printf("Ошибка при создании файла\n");
    }

    // Записываем данные в файл
    write_to_file("test.txt", "Привет, это тестовая запись!");

    // Читаем данные из файла
    char* content = read_from_file("test.txt");
    if (content != NULL) {
        printf("Содержимое файла: %s\n", content);
        free(content);
    }

    // Выводим список файлов
    printf("Файлы в текущей директории:\n");
    list_files();

    return 0;
}
