#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


// Упрощение текста - удаляем всё, кроме букв, и приводим к нижнему регистру
// нормализация текста
void uprostit_text(const char *isxodny_text, char *uproshenny_text) {
    int i = 0, j = 0;
    
    // Проходим по всем символам исходного текста
    while (isxodny_text[i] != '\0') {
        // isalpha() проверяет, является ли символ буквой
        if (isalpha(isxodny_text[i])) {
            // tolower() преобразует букву к нижнему регистру
            uproshenny_text[j] = tolower(isxodny_text[i]);
            j++;
        }
        i++;
    }
    // Важно: добавляем конец строки
    uproshenny_text[j] = '\0';
}

// Шифр Цезаря - простейший шифр подстановки шифр с фиксированным сдвигом //

void shifr_cezarya_zashifrovat(const char *otkryty_text, char *shifrotekst, int sdvig) {
    int i = 0;
    
    while (otkryty_text[i] != '\0') {
        // Проверяем, что символ - маленькая буква
        if (otkryty_text[i] >= 'a' && otkryty_text[i] <= 'z') {
            // Формула шифрования Цезаря: C = (P + K) mod 26
            // где P - позиция буквы открытого текста, K - ключ (сдвиг)
            shifrotekst[i] = 'a' + (otkryty_text[i] - 'a' + sdvig) % 26;
        } else {
            // Если не буква, оставляем как есть
            shifrotekst[i] = otkryty_text[i];
        }
        i++;
    }
    shifrotekst[i] = '\0';
}

void shifr_cezarya_rasshifrovat(const char *shifrotekst, char *otkryty_text, int sdvig) {
    int i = 0;
    
    while (shifrotekst[i] != '\0') {
        if (shifrotekst[i] >= 'a' && shifrotekst[i] <= 'z') {
            // Формула дешифрования: P = (C - K + 26) mod 26
            // +26 нужно чтобы избежать отрицательных чисел
            otkryty_text[i] = 'a' + (shifrotekst[i] - 'a' - sdvig + 26) % 26;
        } else {
            otkryty_text[i] = shifrotekst[i];
        }
        i++;
    }
    otkryty_text[i] = '\0';
}

 // Шифр Виженера - полиалфавитный шифр подстановки первый исторический шифр, устойчивый к частотному анализу  //

void shifr_vizhenera_zashifrovat(const char *otkryty_text, char *shifrotekst, const char *klyuch) {
    int i = 0, j = 0;
    int dlinna_klyucha = strlen(klyuch);
    
    while (otkryty_text[i] != '\0') {
        if (otkryty_text[i] >= 'a' && otkryty_text[i] <= 'z') {
            // Определяем сдвиг по текущей букве ключа
            // 'a' = 0, 'b' = 1, ..., 'z' = 25
            int tekushiy_sdvig = klyuch[j % dlinna_klyucha] - 'a';
            
            // Шифруем как Цезарь, но с меняющимся сдвигом
            shifrotekst[i] = 'a' + (otkryty_text[i] - 'a' + tekushiy_sdvig) % 26;
            j++; // Переходим к следующей букве ключа
        } else {
            shifrotekst[i] = otkryty_text[i];
        }
        i++;
    }
    shifrotekst[i] = '\0';
}

void shifr_vizhenera_rasshifrovat(const char *shifrotekst, char *otkryty_text, const char *klyuch) {
    int i = 0, j = 0;
    int dlinna_klyucha = strlen(klyuch);
    
    while (shifrotekst[i] != '\0') {
        if (shifrotekst[i] >= 'a' && shifrotekst[i] <= 'z') {
            int tekushiy_sdvig = klyuch[j % dlinna_klyucha] - 'a';
            
            // Дешифруем обратным сдвигом
            otkryty_text[i] = 'a' + (shifrotekst[i] - 'a' - tekushiy_sdvig + 26) % 26;
            j++;
        } else {
            otkryty_text[i] = shifrotekst[i];
        }
        i++;
    }
    otkryty_text[i] = '\0';
}

// Частотный анализ - анализ частоты символов //


void opredelit_chastoty(const char *tekst, double *chastoty) {
    int schetchiki[26] = {0}; // Массив для подсчета каждой буквы
    int vsego_bukv = 0;
    int i = 0;
    
    // Подсчитываем каждую букву
    while (tekst[i] != '\0') {


if (tekst[i] >= 'a' && tekst[i] <= 'z') {
            schetchiki[tekst[i] - 'a']++;
            vsego_bukv++;
        }
        i++;
    }
    // Вычисляем частоты (относительные вероятности)
    for (int bukva = 0; bukva < 26; bukva++) {
        if (vsego_bukv > 0) {
            chastoty[bukva] = (double)schetchiki[bukva] / vsego_bukv;
        } else {
            chastoty[bukva] = 0.0;
        }
    }
}

// Скалярное произведение - мера схожести частотных распределений, чем больше результат, тем ближе распределение к эталонному //

double skalyarnoe_proizvedenie(const double *chastoty1, const double *chastoty2) {
    double resultat = 0.0;
    
    for (int i = 0; i < 26; i++) {
        resultat += chastoty1[i] * chastoty2[i];
    }
    
    return resultat;
}

// Взлом шифра Виженера методом Казиски //

void poisk_klyucha_dlya_dliny(const char *shifrotekst, const double *etalonnye_chastoty, 
                             int dlinna_klyucha, char *luchshiy_klyuch, double *luchshiy_score) {
    char kandidat_klyuch[dlinna_klyucha + 1];
    char rasshifrovanny_text[strlen(shifrotekst) + 1];
    double tekushie_chastoty[26];// Начинаем с ключа из всех 'a' (нулевые сдвиги)
    for (int i = 0; i < dlinna_klyucha; i++) {
        kandidat_klyuch[i] = 'a';
    }
    kandidat_klyuch[dlinna_klyucha] = '\0';
    
    // Метод "жадного" поиска: оптимизируем по одной букве за раз
    for (int poziciya = 0; poziciya < dlinna_klyucha; poziciya++) {
        double luchshiy_score_pozicii = -1.0;
        char luchshaya_bukva = 'a';
        
        // Перебираем все возможные буквы для текущей позиции ключа
        for (char bukva = 'a'; bukva <= 'z'; bukva++) {
            kandidat_klyuch[poziciya] = bukva;
            
            // Пробуем дешифровать с текущим ключом-кандидатом
            shifr_vizhenera_rasshifrovat(shifrotekst, rasshifrovanny_text, kandidat_klyuch);
            
            // Анализируем полученный текст
            opredelit_chastoty(rasshifrovanny_text, tekushie_chastoty);
            double score = skalyarnoe_proizvedenie(tekushie_chastoty, etalonnye_chastoty);
            
            // Запоминаем лучшую букву для этой позиции
            if (score > luchshiy_score_pozicii) {
                luchshiy_score_pozicii = score;
                luchshaya_bukva = bukva;
            }
        }
        
        // Фиксируем найденную лучшую букву
        kandidat_klyuch[poziciya] = luchshaya_bukva;
    }
    // Финальная проверка найденного ключа
    shifr_vizhenera_rasshifrovat(shifrotekst, rasshifrovanny_text, kandidat_klyuch);
    opredelit_chastoty(rasshifrovanny_text, tekushie_chastoty);
    *luchshiy_score = skalyarnoe_proizvedenie(tekushie_chastoty, etalonnye_chastoty);
    strcpy(luchshiy_klyuch, kandidat_klyuch);
}

// Основная функция взлома - перебор длин ключа //
void vzlom_shifra_vizhenera(const char *shifrotekst, const char *etalonny_text) {
    double etalonnye_chastoty[26];
    char uproshenny_etalon[strlen(etalonny_text) + 1];
    
    // Подготавливаем эталонный текст и вычисляем эталонные частоты
    printf("Podgotovka etalonnogo teksta dlya chastotnogo analiza...\n");
    uprostit_text(etalonny_text, uproshenny_etalon);
    opredelit_chastoty(uproshenny_etalon, etalonnye_chastoty);
    
    printf("Nachinaem perebor dlinn klyucha ot 1 do 15...\n");
    printf("*********************************************\n");
    
    // Перебираем возможные длины ключа
    for (int dlinna = 1; dlinna <= 15; dlinna++) {
        char luchshiy_klyuch[dlinna + 1];
        double luchshiy_score;
        
        poisk_klyucha_dlya_dliny(shifrotekst, etalonnye_chastoty, dlinna, luchshiy_klyuch, &luchshiy_score);
        
        printf("Dlina %2d: klyuch = %-15s score = %.3f", dlinna, luchshiy_klyuch, luchshiy_score);
        
        // Подсвечиваем хорошие кандидаты
        if (luchshiy_score > 0.08) {


printf("PRAVILNO");
        }
        printf("\n");
    }
}
// pokaz //
void demonstraciya_cezarya() {
    printf("*** CEZAR DEMO ***\n");
    
    char otkryty_text[] = "programmingisawesome";
    char shifr[100], rasshifr[100];
    
    shifr_cezarya_zashifrovat(otkryty_text, shifr, 7);
    printf("Otkryty text: %s\n", otkryty_text);
    printf("Zashifrovanny: %s\n", shifr);
    
    shifr_cezarya_rasshifrovat(shifr, rasshifr, 7);
    printf("Rasshifrovanny: %s\n", rasshifr);
    printf("\n");
}

void demonstraciya_vizhenera() {
    printf("*** VIZHENER DEMO ***\n");
    
    char otkryty_text[] = "cryptographyisfunandchallenging";
    char klyuch[] = "secretkey";
    char shifr[100], rasshifr[100];
    
    shifr_vizhenera_zashifrovat(otkryty_text, shifr, klyuch);
    printf("Otkryty text: %s\n", otkryty_text);
    printf("Klyuch: %s\n", klyuch);
    printf("Zashifrovanny: %s\n", shifr);
    
    shifr_vizhenera_rasshifrovat(shifr, rasshifr, klyuch);
    printf("Rasshifrovanny: %s\n", rasshifr);
    printf("\n");
}

int main() {
    printf("SHIFR VIZHENERA - CRYPTOANALIZ\n");
    printf("Avtor: student 1 kursa kriptografii Opanasuk Dmitri KKSO-05-21\n\n");
    
    // Демонстрация работы шифров
    demonstraciya_cezarya();
    demonstraciya_vizhenera();
    
    // Взлом шифра Виженера на случайном тексте
    char random_shifr[] = "zicvtwqngrzgvtwavzhcqyglmgjqgvtwavzhcqyglmgjq";
    char random_etalon[] = "thisisarandomsampletextusedfordemonstratingthecryptoanalysis"
                         "thequickbrownfoxjumpsoverthelazydogandthisisjustatesttext"
                         "frequencyanalysisisapowerfultoolforbreakingclassicalciphers";
    
    printf("VZLOM SHIFRA VIZHENERA NA SLUCHAYNOM TEKSTE:\n");
    printf("============================================\n");
    vzlom_shifra_vizhenera(random_shifr, random_etalon);
    
    // Дополнительный тест
    printf("\nDOPOLNITELNY TEST:\n");
    printf("==================\n");
    char test_shifr[] = "xfvmqntracevpbctbdiscuszxxefktraspypptfcoskocssiltoetib";
    char test_etalon[] = "ditiseenboekditisgeennovelenditisgeenromanhetisgeenpoezie"
                        "hetisgeenstudiehetisgeenphilosophiehetisgeentendenshetis"
                        "geenstichtinghetishelemaalnietsendatistlevenuhetisalles";
    
    vzlom_shifra_vizhenera(test_shifr, test_etalon);
    
    return 0;
}