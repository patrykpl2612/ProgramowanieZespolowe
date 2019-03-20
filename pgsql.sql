CREATE TABLE Zjazdy
(
    ZjazdyId INT PRIMARY KEY,
    Data DATE
)

CREATE TABLE Pracownicy
(
    Id INT PRIMARY KEY,
    Imie CHAR(10),
    Nazwisko CHAR(10),
    Stopien TINYINT(10) NOT NULL,
    Stanowisko TINYINT(5) NOT NULL,
    Zatrudnienie TINYINT(2) NOT NULL
);

CREATE TABLE Dezyderaty
(
    Id_Pracownika INT PRIMARY KEY,
    Dzien DATE,
    Adnotacja TEXT,
    FOREIGN KEY (Dzien) REFERENCES Zjazdy(Id),
    FOREIGN KEY (Id_Pracownika) REFERENCES Pracownicy(Id)
);

CREATE TABLE Przydzial_Zajec
(
    Id INT PRIMARY KEY,
    Id_Pracownika INT PRIMARY KEY,
    Id_Przedmiotu INT PRIMARY KEY,
    Grupa INT,
    FOREIGN KEY (Id_Pracownika) REFERENCES Pracownicy(Id),
    FOREIGN Key (Id_Przedmiotu) REFERENCES Programy_Studiow(Id)
);

CREATE TABLE Plan_Zajec
(
    Dzien INT PRIMARY KEY,
    Id_Przydzialu INT PRIMARY KEY,
    Od DATETIME,
    Do DATETIME,
    Sala INT,
    FOREIGN KEY (Dzien) REFERENCES Zjazdy(Id),
    FOREIGN KEY (Id_Przydzialu) REFERENCES Przydzial_Zajec(Id)
);

CREATE TABLE Programy_Studiow
(
    Id INT PRIMARY KEY,
    Rok_Studiow YEAR,
    Nazwa_Przedmiotu CHAR(10),
    Liczba_Godzin INT,
    Typ CHAR
);


CREATE TABLE Liczby_Grup
(
    Rok_Studiow INT(3),
    Liczba_Grup INT
);

CREATE TABLE Stopien_Naukowy
(
    Id INT PRIMARY KEY,
    Stopien CHAR(20)
);