#!/bin/bash

echo "Запуск тестов..."
success_count=0
failure_count=0

s21_grep_path="./s21_grep"

for file in tests/test*.txt; do   
    echo "Тестирование файла: $file"
    
    for pattern in "test" "example" "line" "grep"; do
        for flag in "-i" "-v" "-c" "-l" "-n" "-h" "-s" "-o" "-e" "--ignore-case" "--invert-match" "--count" "--files-with-matches" "--line-number" "--no-filename" "--no-messages" "--only-matching"; do
            if ! diff <(grep $flag "$pattern" "$file") <("$s21_grep_path" $flag "$pattern" "$file") > /dev/null; then
                echo "  Неудачный тест: $flag с паттерном '$pattern' и файлом $file"
                echo "  Разница между grep и s21_grep:"
                diff <(grep $flag "$pattern" "$file") <("$s21_grep_path" $flag "$pattern" "$file")
                ((failure_count++))
            else
                ((success_count++))
            fi
        done
        
        for combo in "-iv" "-in" "-ic" "-il" "-ih" "-is" "-io" "-vn" "-vc" "-vl" "-vh" "-vs" "-vo" "-cn" "-cl" "-ch" "-cs" "-co" "-ln" "-lh" "-ls" "-lo" "-nh" "-ns" "-no" "-sh" "-so" "-oh"; do
            if ! diff <(grep $combo "$pattern" "$file") <("$s21_grep_path" $combo "$pattern" "$file") > /dev/null; then
                echo "  Неудачный тест: $combo с паттерном '$pattern' и файлом $file"
                echo "  Разница между grep и s21_grep:"
                diff <(grep $combo "$pattern" "$file") <("$s21_grep_path" $combo "$pattern" "$file")
                ((failure_count++))
            else
                ((success_count++))
            fi
        done
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