#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>

// Открывает или создает файловую систему
FILE* open_or_create_filesystem(const char* filename);

// Просмотр содержимого файла в ФС
char* view_file_in_filesystem(FILE* fs, const char* filename);

// Удаление файла из ФС
int delete_file_in_filesystem(FILE** fs, const char* filename, const char* fs_filename);

// Создание нового файла в ФС
int create_new_file(FILE* fs, const char* filename, const char* content);

// Изменение существующего файла в ФС
int modify_file_in_filesystem(FILE** fs, const char* filename, const char* new_content, const char* fs_filename);

#endif // FILESYSTEM_H
