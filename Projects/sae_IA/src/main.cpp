#include "M5Unified.h"
#include "M5GFX.h"
#include "mnist_float32.h" // Assure-toi que ce fichier est bien à jour dans ton dossier src

M5GFX display;
uint8_t image28x28[28][28];

// Vide le tableau
void clearImageBuffer() {
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 28; x++) image28x28[y][x] = 0;
    }
}

// Affichage demandé : 's' pour le tracé, '.' pour le reste (grille)
void displayTerminal(float processed[28][28]) {
    Serial.println("\n--- Aperçu 28x28 (Pre-processed) ---");
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 28; x++) {
            if (processed[y][x] == 1.0f) {
                Serial.print("s "); // Tracé principal
            } else {
                Serial.print(". "); // Grille de pointillés (fond et gris 0.5)
            }
        }
        Serial.println();
    }
    Serial.println("------------------------------------\n");
}

// Pre-processing : épaississement du trait
void preprocess(float output[28][28]) {
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 28; x++) output[y][x] = 0.0f;
    }
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 28; x++) {
            if (image28x28[y][x] == 1) {
                output[y][x] = 1.0f;
                int dy[] = {-1, 1, 0, 0}, dx[] = {0, 0, -1, 1};
                for (int i = 0; i < 4; i++) {
                    int ny = y + dy[i], nx = x + dx[i];
                    if (ny >= 0 && ny < 28 && nx >= 0 && nx < 28) {
                        if (output[ny][nx] < 1.0f) output[ny][nx] = 0.5f;
                    }
                }
            }
        }
    }
}

void setup(void) {
    auto cfg = M5.config();
    M5.begin(cfg);
    Serial.begin(115200);
    display.init();
    display.clear();
    clearImageBuffer();
    Serial.println("Système prêt.");
}

void loop(void) {
    static bool drawed = false;
    lgfx::touch_point_t tp[3];
    int nums = display.getTouchRaw(tp, 3);

    // Utilisation de STATIC pour éviter le Stack Overflow
    static float processed[28][28]; 
    static input_t input_data;
    // CORRECTION ICI : dense_8_output_type au lieu de dense_4
    static dense_8_output_type scores; 

    if (nums > 0) {
        display.convertRawXY(tp, nums);
        for (int i = 0; i < nums; ++i) {
            display.fillCircle(tp[i].x, tp[i].y, 6, TFT_WHITE);
            int ix = (tp[i].x * 28) / display.width();
            int iy = (tp[i].y * 28) / display.height();
            if (ix >= 0 && ix < 28 && iy >= 0 && iy < 28) image28x28[iy][ix] = 1;
        }
        drawed = true;
    } 
    else if (drawed) {
        Serial.println("Lancement de l'inference...");

        preprocess(processed);
        displayTerminal(processed);

        for (int y = 0; y < 28; y++) {
            for (int x = 0; x < 28; x++) {
                input_data[y][x][0] = processed[y][x];
            }
        }

        // Appel de la fonction cnn avec le nouveau type de scores
        cnn(input_data, scores);

        float max_val = scores[0];
        int prediction = 0;
        for (int i = 1; i < 10; i++) {
            if (scores[i] > max_val) {
                max_val = scores[i];
                prediction = i;
            }
        }

        Serial.printf("PREDICTION : %d (Score: %.2f)\n", prediction, max_val);
        
        display.setFont(&fonts::Font7);
        display.setTextColor(TFT_GREEN, TFT_BLACK);
        display.drawCenterString(String(prediction), display.width()/2, display.height()/2 - 20);
        
        delay(2000);
        display.clear();
        clearImageBuffer();
        drawed = false;
    }
    vTaskDelay(1);
}