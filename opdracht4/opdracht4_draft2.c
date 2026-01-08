#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Versimpelen van tekst 
// we verwijderen alles behalve letters en zetten alles om naar kleine letters
// normalisatie van tekst 
void versimpel(const char *oorspronkelijke_text, char *vereenvoudigde_tekst) {
    int i = 0, j = 0;

    // We doorlopen alle karakters van de oorspronkelijke tekst
    while (oorspronkelijke_text[i] != '\0') {
        // isalpha() controleert of het karakter een letter is
        if (isalpha(oorspronkelijke_text[i])) {
            // tolower() zet de letter om naar kleine letters
            vereenvoudigde_tekst[j] = tolower(oorspronkelijke_text[i]);
            j++;
        }
        i++;
    }
    // We voegen het einde van de string toe
    vereenvoudigde_tekst[j] = '\0';
}

// Caesar-cijfer - de eenvoudigste substitutie-cijfer met een vaste verschuiving

void caesar_versleutel(const char *open_text, char *gecodeerde_text, int schuiving) {
    int i = 0;
    
    while (open_text[i] != '\0') {
        // We controleren of het karakter een kleine letter is
        if (open_text[i] >= 'a' && open_text[i] <= 'z') {
            // Formule voor Caesar-versleuteling: C = (P + K) mod 26
            // Waarbij P de positie is van de letter in de open tekst, K de sleutel (verschuiving)
            gecodeerde_text[i] = 'a' + (open_text[i] - 'a' + schuiving) % 26;
        } else {
            // Als het geen letter is, laten we het zoals het is
            gecodeerde_text[i] = open_text[i];
        }
        i++;
    }
    gecodeerde_text[i] = '\0';
}

void caesar_ontleutel(const char *gecodeerde_text, char *open_text, int schuiving) {
    int i = 0;
    
    while (gecodeerde_text[i] != '\0') {
        if (gecodeerde_text[i] >= 'a' && gecodeerde_text[i] <= 'z') {
            // Formule voor Caesar-ontsleuteling: P = (C - K + 26) mod 26
            // +26 is nodig om negatieve getallen te voorkomen
            open_text[i] = 'a' + (gecodeerde_text[i] - 'a' - schuiving + 26) % 26;
        } else {
            open_text[i] = gecodeerde_text[i];
        }
        i++;
    }
    open_text[i] = '\0';
}

// Vigenère-cijfer - polyalfabetische substitutie-cijfer, de eerste historische cijfer, bestand tegen frequentieanalyse

void vigenere_versleutel(const char *open_text, char *gecodeerde_text, const char *sleutel) {
    int i = 0, j = 0;
    int lengte_sleutel = strlen(sleutel);
    
    while (open_text[i] != '\0') {
        if (open_text[i] >= 'a' && open_text[i] <= 'z') {
            // Bepaal de verschuiving op basis van de huidige letter van de sleutel
            // 'a' = 0, 'b' = 1, ..., 'z' = 25
            int huidige_schuiving = sleutel[j % lengte_sleutel] - 'a';

            // We versleutelen zoals Caesar, maar met een veranderende verschuiving
            gecodeerde_text[i] = 'a' + (open_text[i] - 'a' + huidige_schuiving) % 26;
            j++; // We gaan naar de volgende letter van de sleutel
        } else {
            gecodeerde_text[i] = open_text[i];
        }
        i++;
    }
    gecodeerde_text[i] = '\0';
}

void vigenere_ontsleutel(const char *gecodeerde_text, char *open_text, const char *sleutel) {
    int i = 0, j = 0;
    int lengte_sleutel = strlen(sleutel);
    
    while (gecodeerde_text[i] != '\0') {
        if (gecodeerde_text[i] >= 'a' && gecodeerde_text[i] <= 'z') {
            int huidige_schuiving = sleutel[j % lengte_sleutel] - 'a';

            // We ontsleutelen met de inverse verschuiving
            open_text[i] = 'a' + (gecodeerde_text[i] - 'a' - huidige_schuiving + 26) % 26;
            j++;
        } else {
            open_text[i] = gecodeerde_text[i];
        }
        i++;
    }
    open_text[i] = '\0';
}

// Frequentieanalyse - analyse van de frequentie van tekens

void bepaal_frequenties(const char *tekst, double *frequentie) {
    int tellersi[26] = {0}; // Array voor het tellen van elke letter
    int totaal_letters = 0;
    int i = 0;

    // We tellen elke letter
    while (tekst[i] != '\0') {

        if (tekst[i] >= 'a' && tekst[i] <= 'z') {
                    tellersi[tekst[i] - 'a']++;
                    totaal_letters++;
                }
                i++;
            }
            // We berekenen de frequenties (relatieve waarschijnlijkheden)
            for (int letter = 0; letter < 26; letter++) {
                if (totaal_letters > 0) {
                    frequentie[letter] = (double)tellersi[letter] / totaal_letters;
                } else {
                    frequentie[letter] = 0.0;
                }
    }
}

// Schaalproduct - maat voor de gelijkenis van frequentieverdelingen, hoe groter het resultaat, 
// hoe dichter de verdeling bij de referentie ligt 

double gelijkenis(const double *frequentie1, const double *frequentie2) {
    double resultat = 0.0;
    
    for (int i = 0; i < 26; i++) {
        resultat += frequentie1[i] * frequentie2[i];
    }
    
    return resultat;
}

// Vervang de sleutel met een kandidaat-sleutel
void zoek_sleutel(const char *gecodeerde_text, const double *beste_frequentie, 
                             int lengte_sleutel, char *beste_sleutel, double *beste_score) {
    char kandidaat_sleutel[lengte_sleutel + 1];
    char ongecodeerde_tekst[strlen(gecodeerde_text) + 1];
    double huidige_frequentie[26];// We beginnen met een sleutel van allemaal 'a' (nul verschuivingen)
    for (int i = 0; i < lengte_sleutel; i++) {
        kandidaat_sleutel[i] = 'a';
    }
    kandidaat_sleutel[lengte_sleutel] = '\0';

    // Methode van "gulzige" zoekopdracht: optimaliseer één letter tegelijk
    for (int positie = 0; positie < lengte_sleutel; positie++) {
        double beste_score_positie = -1.0;
        char beste_letter = 'a';

        // Doorloop alle mogelijke letters voor de huidige positie van de sleutel
        for (char letter = 'a'; letter <= 'z'; letter++) {
            kandidaat_sleutel[positie] = letter;

            // proberen te ontsleutelen met de huidige kandidaat-sleutel
            vigenere_ontsleutel(gecodeerde_text, ongecodeerde_tekst, kandidaat_sleutel);

            // We analyseren de verkregen tekst
            bepaal_frequenties(ongecodeerde_tekst, huidige_frequentie);
            double score = gelijkenis(huidige_frequentie, beste_frequentie);

            // We onthouden de beste letter voor deze positie
            if (score > beste_score_positie) {
                beste_score_positie = score;
                beste_letter = letter;
            }
        }

        // We fixeren de gevonden beste letter
        kandidaat_sleutel[positie] = beste_letter;
    }
    // Finale controle van de gevonden sleutel
    vigenere_ontsleutel(gecodeerde_text, ongecodeerde_tekst, kandidaat_sleutel);
    bepaal_frequenties(ongecodeerde_tekst, huidige_frequentie);
    *beste_score = gelijkenis(huidige_frequentie, beste_frequentie);
    strcpy(beste_sleutel, kandidaat_sleutel);
}

// main kraak functie - brute force door sleutel lengtes
void opdracht_kraak(const char *gecodeerde_text, const char *etalonny_text) {
    double beste_frequentie[26];
    char uproshenny_etalon[strlen(etalonny_text) + 1];
    
    // voorbereiden van de referentietekst en berekenen van de referentiefrequenties
    printf("Voorbereiden van de referentietekst voor frequentieanalyse...\n");
    versimpel(etalonny_text, uproshenny_etalon);
    bepaal_frequenties(uproshenny_etalon, beste_frequentie);
    
    printf("Start checken van sleutel lengte van 1 tot 15...\n");
    printf("*********************************************\n");

    // We doorlopen alle mogelijke sleutel lengtes
    for (int lengte = 1; lengte <= 15; lengte++) {
        char beste_sleutel[lengte + 1];
        double beste_score;
        
        zoek_sleutel(gecodeerde_text, beste_frequentie, lengte, beste_sleutel, &beste_score);
        
        printf("Lengte %2d: sleutel = %-15s score = %.3f", lengte, beste_sleutel, beste_score);
        
        // We markeren goede kandidaten
        if (beste_score > 0.08) {

            printf("GOED");
        }
        printf("\n");
    }
}
// Demonstratie van de werking van de Caesar- en Vigenère-cijfers
void demonstratie_caesar() {
    printf("*** CAESAR DEMO ***\n");
    
    char open_text[] = "programmingisawesome";
    char shifr[100], rasshifr[100];
    
    caesar_versleutel(open_text, shifr, 7);
    printf("Open tekst: %s\n", open_text);
    printf("Gecodeerde: %s\n", shifr);
    
    caesar_ontleutel(shifr, rasshifr, 7);
    printf("Decodeerde_tekst: %s\n", rasshifr);
    printf("\n");
}

void demonstratie_vigenere() {
    printf("*** VIGENERE DEMO ***\n");
    
    char open_text[] = "cryptographyisfunandchallenging";
    char sleutel[] = "secretkey";
    char shifr[100], rasshifr[100];
    
    vigenere_versleutel(open_text, shifr, sleutel);
    printf("Open tekst: %s\n", open_text);
    printf("sleutel: %s\n", sleutel);
    printf("Gecodeerde: %s\n", shifr);
    
    vigenere_ontsleutel(shifr, rasshifr, sleutel);
    printf("Decodeerde_tekst: %s\n", rasshifr);
    printf("\n");
}

int main() {
    printf("SHIFR VIZHENERA - CRYPTOANALIZ\n");
    printf("Avtor: student eerste jaar bachelor informatica\n\n");
    
    // demonstratie van de werking van de cijfers
    demonstratie_caesar();
    demonstratie_vigenere();
    
    // kraak Vigenère-cijfer op een willekeurige tekst
    char random_shifr[] = "zicvtwqngrzgvtwavzhcqyglmgjqgvtwavzhcqyglmgjq";
    char random_etalon[] = "thisisarandomsampletextusedfordemonstratingthecryptoanalysis"
                         "thequickbrownfoxjumpsoverthelazydogandthisisjustatesttext"
                         "frequencyanalysisisapowerfultoolforbreakingclassicalciphers";
    
    printf("VZLOM SHIFRA VIZHENERA NA SLUCHAYNOM TEKSTE:\n");
    printf("============================================\n");
    opdracht_kraak(random_shifr, random_etalon);
    
    // extra tekst
    printf("\nDOPOLNITELNY TEST:\n");
    printf("==================\n");
    char test_shifr[] = "xfvmqntracevpbctbdiscuszxxefktraspypptfcoskocssiltoetib";
    char test_etalon[] = "ditiseenboekditisgeennovelenditisgeenromanhetisgeenpoezie"
                        "hetisgeenstudiehetisgeenphilosophiehetisgeentendenshetis"
                        "geenstichtinghetishelemaalnietsendatistlevenuhetisalles";
    
    opdracht_kraak(test_shifr, test_etalon);
    
    return 0;
}