Proiect PG
==========

Lista fisierelor sursa
----------------------

* header.hpp
    - contine constante, variabile globale si functii utilitare
* hndl.hpp
    - contine rutinele de preluare a inputului utilizatorului
    - alte callbackuri gl/glut
* includes.hpp
    - contine toate includeurile externe
* init.hpp
    - contine rutinele de initialize a bibliotecii opengl, a modelul, a iluminarii etc
* main.cpp
    - fisier wrapper care combina toate fisierele intr-un singur modul care e compilat
* main.hpp
    - contine entry-pointul aplicatiei
* render.hpp
    - contine rutinele de afisare a modelului
* update.hpp
    - contine rutinele de logica / update a diverselor entitati reprezentate in scena
    - coliziuni
    - contine callbackul principal de update pentru glut
    
Build & Install
===============

Daca nu a fost compilat proiectul, acesta poate fi builduit cu visual studio 2008. In functie de targetul de compilare, va fi creat un folder Release sau unul Debug care va contine executabilele.

De aici, buildul poate fi copiat oriunde daca se tine cont de faptul ca school-PG.exe si freeglut.dll trebuie sa fie in acelasi folder, la acelasi nivel.

User Guide
==========

Acesta proiect este un joc de tipul first person shooter (FPS). Comenziile sunt cele standard, bine intiparite in domeniu. Anume:

| buton        | functie            |
|--------------|--------------------|
| miscare maus | miscare camera     |
| `Mouse 1`    | foc                |
| `W`          | miscare in fata    |
| `S`          | miscare in spate   |
| `A`          | miscare in stanga  |
| `D`          | miscare in dreapta |
| `Space`      | pauza a logicii    |
| `Esc`        | iesire             |
