import sys, subprocess
# Installation automatique de pyserial si manquant
try: 
    import serial
except: 
    subprocess.check_call([sys.executable, "-m", "pip", "install", "pyserial"])
    import serial

import tkinter as tk
from tkinter import ttk

# --- CONFIGURATION ---
SERIAL_PORT = "COM8"  
BAUD_RATE = 921600    # <--- Haute vitesse pour la fluidité

class AppIA:
    def __init__(self, root):
        self.root = root
        self.root.title("GEII - Dashboard IA Temps Réel Haute Vitesse")
        self.root.geometry("700x800")
        self.root.configure(bg='#1a1a1a')

        # --- TITRE ---
        tk.Label(root, text="M5STACK CORE S3 MONITOR", font=("Impact", 28), bg='#1a1a1a', fg='#00ff00').pack(pady=20)

        # --- CONTAINER CENTRAL (Grille + Chiffre) ---
        main_frame = tk.Frame(root, bg='#1a1a1a')
        main_frame.pack(pady=10)

        # Grille 28x28 (Miroir Temps Réel)
        self.canvas = tk.Canvas(main_frame, width=280, height=280, bg='black', highlightthickness=2, highlightbackground="#333")
        self.canvas.grid(row=0, column=0, padx=30)
        
        # Création des pixels de la grille
        self.pixels = []
        for y in range(28):
            row = []
            for x in range(28):
                p = self.canvas.create_rectangle(x*10, y*10, (x+1)*10, (y+1)*10, fill='black', outline='#111')
                row.append(p)
            self.pixels.append(row)

        # Affichage du Chiffre Géant
        self.val_chiffre = tk.StringVar(value="?")
        self.lbl_chiffre = tk.Label(main_frame, textvariable=self.val_chiffre, font=("Helvetica", 150, "bold"), bg='#1a1a1a', fg='#f1c40f')
        self.lbl_chiffre.grid(row=0, column=1, padx=30)

        # --- STATISTIQUES (LabelFrame esthétique) ---
        stats_frame = tk.LabelFrame(root, text=" Statistiques Système & IA ", font=("Helvetica", 12, "bold"), bg='#1a1a1a', fg='white', bd=2, padx=20, pady=20)
        stats_frame.pack(fill="x", padx=40, pady=20)

        # Temps IA et Pre-proc
        self.lbl_ia = tk.Label(stats_frame, text="Temps IA: -- ms", font=("Consolas", 16), bg='#1a1a1a', fg='#3498db')
        self.lbl_ia.pack(anchor="w")
        
        self.lbl_pre = tk.Label(stats_frame, text="Temps Pre-proc: -- ms", font=("Consolas", 16), bg='#1a1a1a', fg='#9b59b6')
        self.lbl_pre.pack(anchor="w", pady=5)

        # Confiance
        self.lbl_conf = tk.Label(stats_frame, text="Confiance: -- %", font=("Consolas", 16), bg='#1a1a1a', fg='#e67e22')
        self.lbl_conf.pack(anchor="w")

        # Séparateur
        ttk.Separator(stats_frame, orient='horizontal').pack(fill='x', pady=15)

        # Batterie
        self.lbl_bat = tk.Label(stats_frame, text="Batterie: --% (Calcul...)", font=("Helvetica", 14), bg='#1a1a1a', fg='white')
        self.lbl_bat.pack(pady=5)
        
        self.bar_bat = ttk.Progressbar(stats_frame, orient="horizontal", length=500, mode="determinate")
        self.bar_bat.pack(pady=5)

        # --- CONNEXION SÉRIE ---
        try:
            self.ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.001)
            self.update_data()
        except Exception as e:
            self.lbl_bat.config(text=f"Erreur Port: {e}", fg="red")

    def update_data(self):
        # On lit toutes les lignes en attente pour éviter les retards
        while self.ser.in_waiting > 0:
            try:
                line = self.ser.readline().decode('utf-8').strip()
                
                # 1. Dessin d'un point (P:x,y)
                if line.startswith("P:"):
                    coords = line.replace("P:", "").split(',')
                    x, y = int(coords[0]), int(coords[1])
                    self.canvas.itemconfig(self.pixels[y][x], fill="white")

                # 2. Effacement de l'écran (CLR)
                elif line == "CLR":
                    for y in range(28):
                        for x in range(28):
                            self.canvas.itemconfig(self.pixels[y][x], fill="black")
                    self.val_chiffre.set("?")

                # 3. Réception des résultats finaux (DATA:)
                elif line.startswith("DATA:"):
                    parts = line.replace("DATA:", "").split(',')
                    d = {}
                    for p in parts:
                        key, val = p.split(':')
                        d[key] = val
                    
                    # Mise à jour des textes
                    self.val_chiffre.set(d['Digit'])
                    self.lbl_ia.config(text=f"Temps IA: {d['tIA']} ms")
                    self.lbl_pre.config(text=f"Temps Pre-proc: {d['tPre']} ms")
                    self.lbl_conf.config(text=f"Confiance: {d['Conf']} %")
                    
                    # Mise à jour batterie
                    bat = int(d['Bat'])
                    self.bar_bat['value'] = bat
                    chg_status = " (EN CHARGE...)" if d['Chg'] == "1" else ""
                    self.lbl_bat.config(text=f"Batterie: {bat}%{chg_status}", 
                                        fg="#2ecc71" if d['Chg']=="1" else "white")
            except:
                pass
        
        # On relance la vérification toutes les 1ms pour une fluidité maximale
        self.root.after(1, self.update_data)

# Lancement de l'application
if __name__ == "__main__":
    root = tk.Tk()
    # Style pour la barre de progression
    style = ttk.Style()
    style.theme_use('clam')
    style.configure("TProgressbar", thickness=20)
    
    app = AppIA(root)
    root.mainloop()