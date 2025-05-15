#include <filesystem.h>
#include <stdio.h>
#include <string.h>

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

#define MAX_INPUT 256

void process_command(char* command, char* arg1) {
    if (strcmp(command, "создать") == 0) {
        create_file(arg1);
        printf("Файл %s создан\n", arg1);
    } else if (strcmp(command, "удалить") == 0) {
        delete_file(arg1);
        printf("Файл %s удален\n", arg1);
    } else if (strcmp(command, "изменить") == 0) {
        printf("Введите новое содержимое: ");
        char content[MAX_INPUT];
        fgets(content, MAX_INPUT, stdin);
        write_file(arg1, content);
    } else if (strcmp(command, "просмотреть") == 0) {
        char* content = read_file(arg1);
        printf("Содержимое:\n%s\n", content);
        free(content);
    }
}

int main() {
    char input[MAX_INPUT];
    
    while(1) {
        printf("> ");
        fgets(input, MAX_INPUT, stdin);
        
        char* cmd = strtok(input, " \n");
        char* arg1 = strtok(NULL, " \n");
        
        if (!cmd || !arg1) {
            printf("Ошибка формата команды!\n");
            continue;
        }
        
        process_command(cmd, arg1);
    }
    return 0;
}
