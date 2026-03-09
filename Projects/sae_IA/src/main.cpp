#include "M5Unified.h"
#include "M5GFX.h"

M5GFX display;
uint8_t image28x28[28][28]; // Notre matrice de travail

// Fonction pour afficher le tableau 2D dans le terminal série
void displayTerminal() {
  Serial.println("\n--- Capture 28x28 ---");
  for (int y = 0; y < 28; y++) {
    for (int x = 0; x < 28; x++) {
      if (image28x28[y][x] == 1) {
        Serial.print("s "); // Pixel dessiné
      } else {
        Serial.print(". "); // Pixel vide
      }
    }
    Serial.println(); // Nouvelle ligne
  }
  Serial.println("---------------------\n");
}

// Fonction pour vider le tableau (remettre à zéro)
void clearImageBuffer() {
  for (int y = 0; y < 28; y++) {
    for (int x = 0; x < 28; x++) {
      image28x28[y][x] = 0;
    }
  }
}

void setup(void) {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  // INITIALISATION DU TERMINAL SÉRIE (Indispensable)
  Serial.begin(115200);
  
  display.init();
  display.setFont(&fonts::Font4);
  display.setTextColor(TFT_WHITE);

  if (!display.touch()) {
    display.setTextDatum(textdatum_t::middle_center);
    display.drawString("Touch not found.", display.width() / 2, display.height() / 2);
  }

  display.clear();
  clearImageBuffer();
  Serial.println("Pret ! Dessinez un chiffre sur l'ecran.");
}

void loop(void) {
  static bool drawed = false;
  lgfx::touch_point_t tp[3];

  int nums = display.getTouchRaw(tp, 3);

  if (nums > 0) {
    display.convertRawXY(tp, nums);

    for (int i = 0; i < nums; ++i) {
      // 1. Dessin sur l'écran (pour l'utilisateur)
      display.fillCircle(tp[i].x, tp[i].y, 8, TFT_WHITE);

      // 2. Enregistrement dans le tableau 28x28
      // On mappe 320px -> 28 et 240px -> 28
      int ix = (tp[i].x * 28) / display.width();
      int iy = (tp[i].y * 28) / display.height();

      if (ix >= 0 && ix < 28 && iy >= 0 && iy < 28) {
        image28x28[iy][ix] = 1; 
      }
    }
    drawed = true;
  } 
  else if (drawed) {
    // L'utilisateur vient de lever le doigt
    
    // 3. Affichage du tableau dans le terminal
    displayTerminal();

    // Pause pour laisser le temps de voir
    delay(1000); 

    // 4. Reset pour le prochain dessin
    drawed = false;
    display.clear();
    clearImageBuffer();
    Serial.println("Tableau vide. Dessinez a nouveau.");
  }
  
  vTaskDelay(1);
}