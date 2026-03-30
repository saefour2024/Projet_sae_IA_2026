import socket
import customtkinter as ctk
import tkinter as tk # Pour le Canvas qui reste en tk standard

# --- CONFIGURATION RÉSEAU ---
# En mode Point d'Accès (AP), l'IP du M5Stack est TOUJOURS 192.168.4.1
M5_IP = "192.168.4.1" 
M5_PORT = 5005

ctk.set_appearance_mode("dark")
ctk.set_default_color_theme("blue")

class AppIA(ctk.CTk):
    def __init__(self):
        super().__init__()
        self.title("GEII - Dashboard IA WiFi (TCP/AP)")
        self.geometry("750x700")
        self.configure(fg_color='#1a1a1a')

        # --- TITRE ---
        self.title_lbl = ctk.CTkLabel(self, text="M5STACK CORE S3 - MONITOR", font=("Impact", 32), text_color="#00ff00")
        self.title_lbl.pack(pady=20)

        # --- ZONE CENTRALE (GRILLE + CHIFFRE) ---
        main_frame = ctk.CTkFrame(self, fg_color="transparent")
        main_frame.pack(pady=10)

        # Grille 28x28
        self.canvas = tk.Canvas(main_frame, width=280, height=280, bg='black', highlightthickness=2, highlightbackground="#333")
        self.canvas.grid(row=0, column=0, padx=20)
        self.pixels = [[self.canvas.create_rectangle(x*10, y*10, (x+1)*10, (y+1)*10, fill='black', outline='#111') for x in range(28)] for y in range(28)]

        # Chiffre Géant
        self.val_chiffre = ctk.StringVar(value="?")
        self.lbl_chiffre = ctk.CTkLabel(main_frame, textvariable=self.val_chiffre, font=("Helvetica", 150, "bold"), text_color="#f1c40f")
        self.lbl_chiffre.grid(row=0, column=1, padx=20)

        # --- STATISTIQUES ---
        self.stats_frame = ctk.CTkFrame(self, border_width=2, border_color="#333")
        self.stats_frame.pack(fill="x", padx=40, pady=20)

        self.lbl_ia = ctk.CTkLabel(self.stats_frame, text="Temps IA: -- ms", font=("Consolas", 18), text_color="#3498db")
        self.lbl_ia.pack(pady=5)
        
        self.lbl_pre = ctk.CTkLabel(self.stats_frame, text="Temps Pre-proc: -- ms", font=("Consolas", 18), text_color="#9b59b6")
        self.lbl_pre.pack(pady=5)
        
        self.lbl_conf = ctk.CTkLabel(self.stats_frame, text="Confiance: -- %", font=("Consolas", 18), text_color="#e67e22")
        self.lbl_conf.pack(pady=5)

        # Batterie
        self.lbl_bat = ctk.CTkLabel(self.stats_frame, text="Batterie: --% (Déconnecté)", font=("Helvetica", 14))
        self.lbl_bat.pack(pady=5)
        self.bar_bat = ctk.CTkProgressBar(self.stats_frame, width=500)
        self.bar_bat.set(0)
        self.bar_bat.pack(pady=10)

        # --- SOCKET TCP ---
        self.sock = None
        self.connect_to_m5()

    def connect_to_m5(self):
        try:
            # Création du socket TCP
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.settimeout(1) # Attente max de 1s pour la connexion
            self.sock.connect((M5_IP, M5_PORT))
            self.sock.setblocking(False) # Mode non-bloquant pour ne pas figer l'IHM
            self.lbl_bat.configure(text="Connecté au M5Stack !", text_color="white")
            self.update_data()
        except:
            self.lbl_bat.configure(text="Recherche du WiFi 'ESP32_IA_PROJECT'...", text_color="orange")
            # Réessayer dans 2 secondes
            self.after(2000, self.connect_to_m5)

    def update_data(self):
        try:
            # On lit les données arrivant sur le WiFi
            data = self.sock.recv(2048).decode('utf-8')
            if data:
                # On découpe par ligne car plusieurs messages peuvent arriver en même temps
                for line in data.split('\n'):
                    line = line.strip()
                    if not line: continue

                    # 1. Dessin d'un point
                    if line.startswith("P:"):
                        coords = line.replace("P:", "").split(',')
                        if len(coords) == 2:
                            x, y = int(coords[0]), int(coords[1])
                            self.canvas.itemconfig(self.pixels[y][x], fill="white")

                    # 2. Effacement
                    elif line == "CLR":
                        for row in self.pixels:
                            for p in row: self.canvas.itemconfig(p, fill='black')
                        self.val_chiffre.set("?")

                    # 3. Résultats DATA
                    elif line.startswith("DATA:"):
                        parts = line.replace("DATA:", "").split(',')
                        d = {}
                        for p in parts:
                            if ':' in p:
                                key, val = p.split(':', 1)
                                d[key] = val
                        
                        if 'Digit' in d:
                            self.val_chiffre.set(d['Digit'])
                            self.lbl_ia.configure(text=f"Temps IA: {d['tIA']} ms")
                            self.lbl_pre.configure(text=f"Temps Pre-proc: {d['tPre']} ms")
                            self.lbl_conf.configure(text=f"Confiance: {d['Conf']} %")
                            
                            bat = int(d['Bat'])
                            self.bar_bat.set(bat / 100)
                            chg = " (EN CHARGE...)" if d['Chg'] == "1" else ""
                            self.lbl_bat.configure(text=f"Batterie: {bat}%{chg}", 
                                                 text_color="#2ecc71" if d['Chg']=="1" else "white")
        except (BlockingIOError, socket.error):
            pass # Pas de données pour le moment, c'est normal
        
        # Fréquence de rafraîchissement (10ms pour fluidité)
        self.root.after(10, self.update_data)

if __name__ == "__main__":
    app = AppIA()
    app.mainloop()