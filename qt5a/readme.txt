1. Uporządkowałem kod, tzn podzieliłem go na mniejsze funkcje.
2. Zastapiłem tablicę przycisków obiektem QButtonGroup. Obsługa jest znacznie łatwiejsza, klasa ta posiada sygnał, 
który przekazuje jako parametr id kliknietego przycisku oraz informację czy został włączony czy wyłaczony.
3. Dodałem wektor, który przechowuje id przydziałów zajęć związanych z poszczególnymi przyciskami.
