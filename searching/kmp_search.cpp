#include <iostream>
#include <vector>
#include <string>

using namespace std;

/**
 * ==========================================
 * ALGORITHM: KMP SEARCH (Алгоритм Кнута-Морриса-Пратта)
 * ==========================================
 * 
 * ПРОБЛЕМА ОБЫЧНОГО ПОИСКА:
 * Если мы ищем "AAAAAB" в тексте "AAAAAXAAAAAB", обычный алгоритм 
 * при несовпадении на 6-м символе (X != B) сдвинется всего на 1 шаг и начнет сравнивать заново.
 * Это дает сложность O(N*M).
 * 
 * ИДЕЯ KMP:
 * Использовать информацию о том, что мы уже совпали.
 * Мы строим таблицу префиксов (LPS - Longest Prefix Suffix).
 * Она говорит: "Если ты ошибся на этом символе, не начинай с нуля. 
 * Мы знаем, что предыдущие N символов совпадают с началом паттерна, прыгай сразу туда".
 * 
 * СЛОЖНОСТЬ:
 * O(N + M), где N - длина текста, M - длина искомого паттерна.
 * Мы проходим по тексту ровно один раз.
 */

// Построение LPS массива (Longest Prefix Suffix)
// LPS[i] хранит длину самого длинного префикса, который также является суффиксом
// для подстроки pattern[0...i].
vector<int> build_lps(const string& pattern) {
    int m = pattern.length();
    vector<int> lps(m);
    
    int len = 0; // Длина предыдущего длиннейшего префикса
    lps[0] = 0;  // Для первого символа всегда 0

    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            // Не совпало
            if (len != 0) {
                // Пытаемся взять префикс покороче. 
                // Это "магия" KMP - откат не в 0, а на предыдущий лучший вариант.
                len = lps[len - 1];
            } else {
                // Если префиксов нет, ставим 0
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

void kmp_search(const string& text, const string& pattern) {
    int n = text.length();
    int m = pattern.length();
    
    if (m == 0) return;

    // Шаг 1: Предварительная обработка паттерна
    vector<int> lps = build_lps(pattern);

    int i = 0; // Индекс в тексте
    int j = 0; // Индекс в паттерне

    while (i < n) {
        if (pattern[j] == text[i]) {
            // Символы совпали, идем дальше
            j++;
            i++;
        }

        if (j == m) {
            // Ура! Весь паттерн совпал
            cout << "Found pattern at index " << (i - j) << endl;
            
            // Готовимся искать следующее вхождение
            // Вместо сброса j в 0, используем LPS
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            // Несовпадение
            if (j != 0) {
                // Не двигаем i (текст), но двигаем j (паттерн) назад по умному
                j = lps[j - 1];
            } else {
                // Если j уже 0, просто переходим к следующему символу текста
                i++;
            }
        }
    }
}

int main() {
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";
    
    cout << "Text: " << text << endl;
    cout << "Pattern: " << pattern << endl;
    
    kmp_search(text, pattern);
    
    return 0;
}
