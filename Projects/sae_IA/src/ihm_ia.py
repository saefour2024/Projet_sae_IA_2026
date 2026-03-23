import socket
import tkinter as tk

# Configuration réseau
UDP_IP = "0.0.0.0" # Écoute sur toutes les interfaces
UDP_PORT = 5005

def start_gui():
    root = tk.Tk()
    root.title("IHM Reconnaissance de Chiffres - SY BUT3 ESE")
    root.geometry("400x300")

    label_title = tk.Label(root, text="Chiffre Reconnu :", font=("Helvetica", 16))
    label_title.pack(pady=20)

    # Zone d'affichage du chiffre
    var_chiffre = tk.StringVar(value="-")
    label_chiffre = tk.Label(root, textvariable=var_chiffre, font=("Helvetica", 80), fg="blue")
    label_chiffre.pack()

    # Zone d'affichage de la confiance
    var_info = tk.StringVar(value="En attente de données...")
    label_info = tk.Label(root, textvariable=var_info, font=("Helvetica", 10))
    label_info.pack(pady=20)

    # Fonction de réception réseau (non-bloquante)
    def check_network():
        try:
            data, addr = sock.recvfrom(1024) # Buffer de 1024 octets
            message = data.decode('utf-8') # Format attendu : "chiffre,confiance,temps"
            chiffre, confiance, temps = message.split(',')
            
            var_chiffre.set(chiffre)
            var_info.set(f"Confiance: {confiance}% | Temps: {temps}ms")
        except socket.error:
            pass
        root.after(100, check_network) # Vérifier toutes les 100ms

    root.after(100, check_network)
    root.mainloop()

# Initialisation du socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
sock.setblocking(False)

start_gui()