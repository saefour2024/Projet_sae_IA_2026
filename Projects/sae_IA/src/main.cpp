#include "M5Unified.h"
#include "M5GFX.h"
#include "mnist_int16.h" // TON NOUVEAU MODÈLE INT16
#include <math.h> 

M5GFX display;
uint8_t image28x28[28][28]; 

// Réinitialisation du dessin
void clearImageBuffer() {
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 28; x++) image28x28[y][x] = 0;
    }
    Serial.println("CLR"); // Effacer IHM Python
}

// --- PRE-PROCESSING : CADRAGE ET CENTRAGE ---
void preprocess(float output[28][28]) {
    for (int y = 0; y < 28; y++) for (int x = 0; x < 28; x++) output[y][x] = 0.0f;
    int x_min = 28, x_max = 0, y_min = 28, y_max = 0;
    bool has_pixels = false;
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 28; x++) {
            if (image28x28[y][x] == 1) {
                if (x < x_min) x_min = x; if (x > x_max) x_max = x;
                if (y < y_min) y_min = y; if (y > y_max) y_max = y;
                has_pixels = true;
            }
        }
    }
    if (!has_pixels) return;
    int draw_w = x_max - x_min + 1, draw_h = y_max - y_min + 1;
    for (int iy = 0; iy < 20; iy++) {
        for (int ix = 0; ix < 20; ix++) {
            int src_x = x_min + (ix * draw_w) / 20, src_y = y_min + (iy * draw_h) / 20;
            if (image28x28[src_y][src_x] == 1) output[iy + 4][ix + 4] = 1.0f;
        }
    }
}

void setup(void) {
    auto cfg = M5.config();
    M5.begin(cfg);
    Serial.begin(921600); // Haute vitesse pour fluidité
    display.init();
    display.clear();
    clearImageBuffer();
}

void loop(void) {
    M5.update();
    static bool drawed = false;
    lgfx::touch_point_t tp[3];
    int nums = display.getTouchRaw(tp, 3);

    if (nums > 0) {
        display.convertRawXY(tp, nums);
        for (int i = 0; i < nums; ++i) {
            display.fillCircle(tp[i].x, tp[i].y, 8, TFT_WHITE);
            int ix = (tp[i].x * 28) / display.width();
            int iy = (tp[i].y * 28) / display.height();
            if (ix >= 0 && ix < 28 && iy >= 0 && iy < 28 && image28x28[iy][ix] == 0) {
                image28x28[iy][ix] = 1;
                Serial.printf("P:%d,%d\n", ix, iy); // Envoi point pour IHM
            }
        }
        drawed = true;
    } 
    else if (drawed) {
        // --- CHRONO PRE-PROCESSING ---
        unsigned long tStartPre = micros();
        static float processed[28][28]; 
        preprocess(processed);
        unsigned long tPre = (micros() - tStartPre) / 1000;

        // --- CONVERSION FLOAT -> INT16 (QUANTIFICATION) ---
        // Le scale_factor est de 8, donc on multiplie par 2^8 = 256
        static input_t input_data; // input_t est maintenant int16_t[28][28][1]
        for (int y = 0; y < 28; y++) {
            for (int x = 0; x < 28; x++) {
                input_data[y][x][0] = (int16_t)(processed[y][x] * 256.0f); 
            }
        }

        // --- CHRONO INFÉRENCE IA (INT16) ---
        unsigned long tStartCNN = millis();
        static dense_10_output_type scores; // dense_output_type est maintenant int16_t[10]
        cnn(input_data, scores);
        unsigned long tCNN = millis() - tStartCNN;

        // --- CALCUL SOFTMAX (AVEC DÉ-QUANTIFICATION) ---
        float sum_exp = 0;
        float probabilities[10];
        for (int i = 0; i < 10; i++) {
            // On divise par 256.0f pour retrouver l'échelle réelle des scores avant l'exponentielle
            probabilities[i] = expf((float)scores[i] / 256.0f); 
            sum_exp += probabilities[i];
        }
        float max_prob = 0;
        int prediction = 0;
        for (int i = 0; i < 10; i++) {
            probabilities[i] /= sum_exp;
            if (probabilities[i] > max_prob) {
                max_prob = probabilities[i];
                prediction = i;
            }
        }

        // --- ENVOI RÉSULTATS (Format DATA) ---
        int batLevel = M5.Power.getBatteryLevel();
        bool isCharging = M5.Power.isCharging();
        Serial.printf("DATA:Digit:%d,Conf:%.1f,tIA:%lu,tPre:%lu,Bat:%d,Chg:%d\n", 
                     prediction, max_prob * 100.0f, tCNN, tPre, batLevel, (int)isCharging);

        // --- AFFICHAGE ÉCRAN ---
        display.clear();
        display.setTextColor(TFT_GREEN, TFT_BLACK);
        display.setFont(&fonts::Font7);
        display.drawCenterString(String(prediction), display.width()/2, 30);
        
        display.setFont(&fonts::Font4);
        display.drawCenterString(String(max_prob*100, 1) + "%", display.width()/2, 110);
        
        display.setTextColor(TFT_WHITE, TFT_BLACK);
        display.setFont(&fonts::Font2);
        display.setCursor(10, 180);
        display.printf("IA(int16): %lu ms | Pre: %lu ms", tCNN, tPre);
        display.setCursor(10, 205);
        display.printf("Batterie: %d%% %s", batLevel, isCharging ? "(Charge)" : "");

        delay(1500); // 1.5 seconde de lecture
        display.clear();
        clearImageBuffer();
        drawed = false;
    }
    vTaskDelay(1);
}