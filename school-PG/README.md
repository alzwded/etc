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
---------------

Daca nu a fost compilat proiectul, acesta poate fi builduit cu visual studio 2008. In functie de targetul de compilare, va fi creat un folder Release sau unul Debug care va contine executabilele.

De aici, buildul poate fi copiat oriunde daca se tine cont de faptul ca school-PG.exe si freeglut.dll trebuie sa fie in acelasi folder, la acelasi nivel.

User Guide
----------

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

Tehnici implementate
--------------------

1. Signal passing (Sort of) -- utilizatorul comunica cu elementele sau acestea intre ele setand flaguri aferente. Rutinele de update vor interpreta ce s-a intamplat mai tarziu, dupa ce tot inputul a fost calculat.
2. Collision detection -- spatiul jucabil este limitat reducand puterea de procesare necesara, detectarea apartenentei la spatiul de joc fiind triviala. Coliziunea intre glonte si cuburi functioneaza pe principiul bounding-sphere-intersection.
3. supra-impozarea de grafica 2d peste grafica 3d. Se modifica matricea de proiectie opengl pentru o proiectie ortogonala si aceasta e supraimpozata peste scena 3d
4. tehnica inmultirii vectorilor -- pentru calculul proiectiei vectorilor de miscare relativ la directia in care ne uitam

Referinte
---------

* http://freeglut.sourceforge.net/docs/api.php
* http://www.opengl.org/sdk/docs/man2/

Cum am obinut modelul scenei
----------------------------

L-am construit cu primitive opengl (c.f. render.hpp) sau glut bazat pe o lista de inamici si variabile globale (c.f. header.hpp).

Modelul e modificat in rutinele din update.hpp .
