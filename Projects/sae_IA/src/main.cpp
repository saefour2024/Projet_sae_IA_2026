#include "M5Unified.h"
#include "M5GFX.h"
#include "mnist_float32.h"
#include <WiFi.h>      // Ajouté pour le WiFi
#include <WiFiUdp.h>   // Ajouté pour l'envoi vers le PC
#include <math.h> 

// ==========================================================
// CONFIGURATION RÉSEAU (À REMPLIR)
// ==========================================================
const char* ssid     = "ESP32_IA_PROJECT";       // Nom de ta box ou partage de connexion
const char* password = "GeiiTailscale2024$";   // Mot de passe WiFi
const char* pc_ip    = "134.59.23.235";       // L'IP de ton PC (trouvée avec ipconfig)
const int udp_port   = 5005;                 // Le port utilisé par ton script Python

WiFiUDP udp;
M5GFX display;
uint8_t image28x28[28][28]; 

// --- FONCTIONS DE GESTION ---

void clearImageBuffer() {
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 28; x++) image28x28[y][x] = 0;
    }
}

void displayTerminal(float processed[28][28]) {
    Serial.println("\n--- Aperçu de la Matrice 28x28 ---");
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 28; x++) {
            if (processed[y][x] >= 0.1f) Serial.print("s "); 
            else Serial.print(". "); 
        }
        Serial.println();
    }
    Serial.println("----------------------------------\n");
}

void preprocess(float output[28][28]) {
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 28; x++) output[y][x] = 0.0f;
    }
    int x_min = 28, x_max = 0, y_min = 28, y_max = 0;
    bool has_pixels = false;
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 28; x++) {
            if (image28x28[y][x] == 1) {
                if (x < x_min) x_min = x;
                if (x > x_max) x_max = x;
                if (y < y_min) y_min = y;
                if (y > y_max) y_max = y;
                has_pixels = true;
            }
        }
    }
    if (!has_pixels) return;
    int draw_w = x_max - x_min + 1;
    int draw_h = y_max - y_min + 1;
    for (int iy = 0; iy < 20; iy++) {
        for (int ix = 0; ix < 20; ix++) {
            int src_x = x_min + (ix * draw_w) / 20;
            int src_y = y_min + (iy * draw_h) / 20;
            if (image28x28[src_y][src_x] == 1) {
                output[iy + 4][ix + 4] = 1.0f;
            }
        }
    }
}

// --- SETUP ---

void setup(void) {
    auto cfg = M5.config();
    M5.begin(cfg);
    Serial.begin(115200);
    
    display.init();
    display.clear();
    clearImageBuffer();

    // 1. Connexion WiFi
    display.drawString("Connexion WiFi...", 10, 10);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connecte !");
    display.clear();
    display.drawString("WiFi OK - Pret", 10, 10);
}

// --- LOOP ---

void loop(void) {
    static bool drawed = false;
    lgfx::touch_point_t tp[3];
    int nums = display.getTouchRaw(tp, 3);

    static float processed[28][28]; 
    static input_t input_data;
    static dense_1_output_type scores; 

    if (nums > 0) {
        display.convertRawXY(tp, nums);
        for (int i = 0; i < nums; ++i) {
            display.fillCircle(tp[i].x, tp[i].y, 8, TFT_WHITE);
            int ix = (tp[i].x * 28) / display.width();
            int iy = (tp[i].y * 28) / display.height();
            if (ix >= 0 && ix < 28 && iy >= 0 && iy < 28) {
                image28x28[iy][ix] = 1;
                if (ix+1 < 28) image28x28[iy][ix+1] = 1; 
            }
        }
        drawed = true;
    } 
    else if (drawed) {
        preprocess(processed);
        displayTerminal(processed);

        for (int y = 0; y < 28; y++) {
            for (int x = 0; x < 28; x++) input_data[y][x][0] = processed[y][x];
        }

        // --- INFÉRENCE ET TEMPS ---
        unsigned long startTime = millis();
        cnn(input_data, scores);
        unsigned long inferenceTime = millis() - startTime;

        // --- SOFTMAX ---
        float sum_exp = 0;
        float probabilities[10];
        for (int i = 0; i < 10; i++) {
            probabilities[i] = expf(scores[i]); 
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
        float confidence = max_prob * 100.0f;

        // --- ENVOI VERS L'IHM PYTHON (WIFI) ---
        if (WiFi.status() == WL_CONNECTED) {
            udp.beginPacket(pc_ip, udp_port);
            // Format du message : "chiffre,confiance,temps"
            String msg = String(prediction) + "," + String(confidence, 1) + "," + String(inferenceTime);
            udp.print(msg);
            udp.endPacket();
            Serial.println("WiFi -> Envoi : " + msg);
        }

        // --- AFFICHAGE ÉCRAN ---
        display.clear();
        display.setTextColor(TFT_GREEN, TFT_BLACK);
        display.setFont(&fonts::Font7);
        display.drawCenterString(String(prediction), display.width()/2, display.height()/2 - 60);
        
        display.setFont(&fonts::Font4);
        display.drawCenterString(String(confidence, 1) + "%", display.width()/2, display.height()/2 + 10);
        
        display.setTextColor(TFT_WHITE, TFT_BLACK);
        display.setFont(&fonts::Font2);
        display.drawCenterString("Time: " + String(inferenceTime) + " ms", display.width()/2, display.height()/2 + 50);
        
        delay(2500);
        display.clear();
        clearImageBuffer();
        drawed = false;
    }
    vTaskDelay(1);
}