#!/bin/bash

echo "Запуск тестов..."
success_count=0
failure_count=0

s21_cat_path="./s21_cat"

for file in tests/test*.txt; do   
    echo "Тестирование файла: $file"
    
    for flag in "-b" "-e" "-n" "-s" "-t" "-v" "--number-nonblank" "--number" "--squeeze-blank" "--show-ends" "--show-nonprinting" "--show-tabs"; do
        if ! diff <(cat $flag "$file") <("$s21_cat_path" $flag "$file") > /dev/null; then
            echo "  Неудачный тест: $flag с файлом $file"
            diff <(cat $flag "$file") <("$s21_cat_path" $flag "$file") | grep '^[<>]'
            ((failure_count++))
        else
            ((success_count++))
        fi
    done
    
    for combo in "-be" "-bn" "-bs" "-bt" "-en" "-es" "-et" "-ns" "-nt" "-st" "-bens" "-bent" "-best" "-bnt" "-bns" "-ens" "-ent" "-est" "-nst"; do
        if ! diff <(cat $combo "$file") <("$s21_cat_path" $combo "$file") > /dev/null; then
            echo "  Неудачный тест: $combo с файлом $file"
            diff <(cat $combo "$file") <("$s21_cat_path" $combo "$file") | grep '^[<>]'
            ((failure_count++))
        else
            ((success_count++))
        fi
    done
    
    echo "Завершено тестирование файла: $file"
    echo "==============================================================="
done

echo "Успешных тестов: $success_count"
echo "Проваленных тестов: $failure_count"

if [ $failure_count -gt 0 ]; then
    exit 1
else
    echo "Все тесты пройдены успешно!"
fi