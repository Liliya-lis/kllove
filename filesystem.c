#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <filesystem.h>
// Функция для открытия (или создания и открытия) файла файловой системы
FILE* open_or_create_filesystem(const char* filename) {
    FILE* fs = fopen(filename, "a+"); // Открываем для чтения и записи (создаём если не существует)
    if (fs == NULL) {
        perror("Failed to open or create filesystem");
        exit(EXIT_FAILURE);
    }
    return fs;
}

// Функция для просмотра содержимого файла внутри файловой системы
char* view_file_in_filesystem(FILE* fs, const char* filename) {
    // Перемещаем указатель в начало файла
    fseek(fs, 0, SEEK_SET);
    
    // Читаем весь файл
    char* content = NULL;
    long length;
    if (fs) {
        fseek(fs, 0, SEEK_END);
        length = ftell(fs);
        fseek(fs, 0, SEEK_SET);
        content = malloc(length + 1);
        if (content) {
            fread(content, 1, length, fs);
            content[length] = '\0';
        }
    }
    
    if (!content) {
        return NULL;
    }
    
    // Разбиваем на строки
    char* line = strtok(content, "\n");
    int found = 0;
    char* file_content = NULL;
    size_t content_size = 0;
    
    while (line != NULL) {
        if (!found) {
            // Проверяем, совпадает ли имя файла
            if (strcmp(line, filename) == 0) {
                found = 1;
            }
        } else {
            // Проверяем, не начался ли новый файл
            if (line[0] == '/') {
                break;
            }
            
            // Добавляем строку к содержимому файла
            size_t line_len = strlen(line);
            char* temp = realloc(file_content, content_size + line_len + 2); // +1 для '\n' и +1 для '\0'
            if (!temp) {
                free(file_content);
                free(content);
                return NULL;
            }
            file_content = temp;
            memcpy(file_content + content_size, line, line_len);
            content_size += line_len;
            file_content[content_size++] = '\n';
        }
        
        line = strtok(NULL, "\n");
    }
    
    free(content);
    
    if (file_content && content_size > 0) {
        file_content[content_size - 1] = '\0'; // Убираем последний '\n'
    }
    
    return file_content ? file_content : strdup(""); // Возвращаем пустую строку, если файл пуст
}

// Функция для удаления файла внутри файловой системы
int delete_file_in_filesystem(FILE** fs, const char* filename, const char* fs_filename) {
    // Перемещаем указатель в начало файла
    fseek(*fs, 0, SEEK_SET);
    
    // Читаем весь файл
    char* content = NULL;
    long length;
    if (*fs) {
        fseek(*fs, 0, SEEK_END);
        length = ftell(*fs);
        fseek(*fs, 0, SEEK_SET);
        content = malloc(length + 1);
        if (content) {
            fread(content, 1, length, *fs);
            content[length] = '\0';
        }
    }
    
    if (!content) {
        return 0;
    }
    
    // Разбиваем на строки и собираем новый контент
    char* line = strtok(content, "\n");
    int found = 0;
    char* new_content = malloc(length + 1);
    new_content[0] = '\0';
    size_t new_content_size = 0;
    
    while (line != NULL) {
        if (!found) {
            // Проверяем, совпадает ли имя файла
            if (strcmp(line, filename) == 0) {
                found = 1;
                line = strtok(NULL, "\n");
                continue; // Пропускаем имя файла и его содержимое
            }
            
            // Копируем строку в новый контент
            size_t line_len = strlen(line);
            memcpy(new_content + new_content_size, line, line_len);
            new_content_size += line_len;
            new_content[new_content_size++] = '\n';
        } else {
            // Пропускаем содержимое файла, пока не найдём начало нового файла
            if (line[0] == '/') {
                found = 0;
                // Возвращаемся к обработке как обычную строку
                size_t line_len = strlen(line);
memcpy(new_content + new_content_size, line, line_len);
                new_content_size += line_len;
                new_content[new_content_size++] = '\n';
            }
        }
        
        line = strtok(NULL, "\n");
    }
    
    free(content);
    
    // Закрываем и переоткрываем файл для перезаписи
    fclose(*fs);
    *fs = fopen(fs_filename, "w");
    if (*fs == NULL) {
        free(new_content);
        return 0;
    }
    
    // Записываем новый контент
    if (new_content_size > 0) {
        fwrite(new_content, 1, new_content_size, *fs);
    }
    
    free(new_content);
    return 1;
}
//  Функция для создания нового файла в файловой системе
int create_new_file(FILE* fs, const char* filename, const char* content) {
    // Переходим в конец файла
    fseek(fs, 0, SEEK_END);
    
    // Записываем имя файла
    if (fprintf(fs, "%s\n", filename) < 0) {
        return 0;
    }
    
    // Записываем содержимое файла
    if (fprintf(fs, "%s\n", content) < 0) {
        return 0;
    }
    
    // Добавляем разделитель
    if (fprintf(fs, "/\n") < 0) {
        return 0;
    }
    
    return 1;
}

// Функция для изменения существующего файла
int modify_file_in_filesystem(FILE** fs, const char* filename, const char* new_content, const char* fs_filename) {
    // Читаем весь файл
    fseek(*fs, 0, SEEK_SET);
    char* file_content = NULL;
    long length;
    
    if (*fs) {
        fseek(*fs, 0, SEEK_END);
        length = ftell(*fs);
        fseek(*fs, 0, SEEK_SET);
        file_content = malloc(length + 1);
        if (file_content) {
            fread(file_content, 1, length, *fs);
            file_content[length] = '\0';
        }
    }
    
    if (!file_content) {
        return 0;
    }
    
    // Разбираем содержимое и формируем новую версию
    char* line = strtok(file_content, "\n");
    char* new_file_content = malloc(length + strlen(new_content) + 1024);
    new_file_content[0] = '\0';
    size_t new_size = 0;
    int in_target_file = 0;
    int file_found = 0;
    
    while (line != NULL) {
        if (!in_target_file) {
            // Проверяем, совпадает ли имя файла
            if (strcmp(line, filename) == 0) {
                in_target_file = 1;
                file_found = 1;
                // Добавляем имя файла и новое содержимое
                strcat(new_file_content, line);
                strcat(new_file_content, "\n");
                strcat(new_file_content, new_content);
                strcat(new_file_content, "\n");
                new_size = strlen(new_file_content);
                // Пропускаем старое содержимое
                while ((line = strtok(NULL, "\n")) != NULL && line[0] != '/') {}
                if (line && line[0] == '/') {
                    strcat(new_file_content, line);
                    strcat(new_file_content, "\n");
                }
            } else {
                // Копируем строки других файлов
                strcat(new_file_content, line);
                strcat(new_file_content, "\n");
            }
        } else {
            // Уже обработали этот файл, продолжаем
            in_target_file = 0;
        }
        
        line = strtok(NULL, "\n");
    }
    
    free(file_content);
    
    if (!file_found) {
        free(new_file_content);
        return 0;
    }
    
    // Перезаписываем файл
    fclose(*fs);
    *fs = fopen(fs_filename, "w");
    if (*fs == NULL) {
        free(new_file_content);
        return 0;
    }
    
    fwrite(new_file_content, 1, strlen(new_file_content), *fs);
    free(new_file_content);
    return 1;
}
