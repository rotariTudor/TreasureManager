# TreasureManager

**TreasureManager** este un proiect scris în limbajul C, constând în mai multe fișiere sursă și un fișier header, având ca scop gestionarea și evaluarea unor "comori" într-un mediu definit.

## Structura Proiectului

- **Fișiere Sursă:**
  - `treasure_hub.c`: Conține funcționalități pentru gestionarea și manipularea comorilor.:contentReference[oaicite:6]{index=6}
  - `treasure_manager.c`: :contentReference[oaicite:8]{index=8}:contentReference[oaicite:10]{index=10}
  - `score_calculator.c`: :contentReference[oaicite:12]{index=12}:contentReference[oaicite:14]{index=14}
  - `treasure_managerMAIN.c`: :contentReference[oaicite:16]{index=16}:contentReference[oaicite:18]{index=18}

- **Fișier Header:**
  - `treasure_manager.h`: :contentReference[oaicite:20]{index=20}:contentReference[oaicite:22]{index=22}

- **Alte Fișiere:**
  - `.vscode/`: :contentReference[oaicite:24]{index=24}:contentReference[oaicite:26]{index=26}
  - `README.txt`: :contentReference[oaicite:28]{index=28}:contentReference[oaicite:30]{index=30}

## Compilare și Rulare

Pentru a compila și rula proiectul principal, urmați pașii de mai jos:

1. **Compilare:**
   ```bash
   gcc -Wall -o treasure_manager treasure_hub.c treasure_manager.c treasure_managerMAIN.c -lm
