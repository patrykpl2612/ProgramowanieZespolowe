CREATE TABLE Zjazdy
(
    Id INT PRIMARY KEY,
    Data DATE
)

CREATE TABLE Pracownicy
(
    Id INT PRIMARY KEY,
    Imie CHAR(10),
    Nazwisko CHAR(30),
    Stopien SMALLINT NOT NULL,
    Stanowisko CHAR(20) NOT NULL
);

CREATE TABLE Dezyderaty
(
    Id_Pracownika INT PRIMARY KEY REFERENCES Pracownicy(Id),
    Dzien INT REFERENCES Zjazdy(Id),
    Adnotacja TEXT
);


CREATE TABLE Grupy
(
    Id INT PRIMARY KEY,
    Rok_Studiow SMALLINT,
    Nazwa CHAR(5)
);


CREATE TABLE Programy_Studiow
(
    Id INT PRIMARY KEY,
    Rok_Studiow SMALLINT,
    Nazwa_Przedmiotu CHAR(30),
    Liczba_Godzin INT,
    Typ_zajec CHAR(1)
);

CREATE TABLE Przydzial_Zajec
(
    Id INT PRIMARY KEY,
    Id_Pracownika INT REFERENCES Pracownicy(Id),
    Id_Przedmiotu INT REFERENCES Programy_Studiow(Id),
    Grupa INT REFERENCES Grupy(Id)
);

CREATE TABLE Plan_Zajec
(
    Id INT PRIMARY KEY,
    Dzien INT REFERENCES Zjazdy(Id),
    Id_Przydzialu INT REFERENCES Przydzial_zajec(Id),
    Od_godzina INT,
    Do_godzina INT,
    Sala CHAR(4)
);
