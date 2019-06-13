# ProgramowanieZespolowe, wersja aktualna - Qt v7.01

ProgramowanieZespolowe



https://serwer1866993.home.pl/psql/

28568274_postgres

COMMIT SUICIDE
BEGIN
    IF NEW.od_godzina >= NEW.do_godzina THEN
    RAISE EXCEPTION '% >= %', NEW.od_godzina, NEW.do_godzina;
    END IF;
    RETURN NEW;
END;
