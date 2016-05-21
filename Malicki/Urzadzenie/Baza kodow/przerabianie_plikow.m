function  [err] = przerabianie_plikow(nazwa_pliku_do_edycji,nazwa_pliku_zedytowanego)
%SKRYPT DO PRZERABIANIA PLIKOW


IDNED = fopen(nazwa_pliku_do_edycji);  %otwarcie pliku niesformatowanego
IDED = fopen(nazwa_pliku_zedytowanego,'w');  %otwarcie pliku sformatowanego
znak = 10;
nazwa_pliku_zedytowanego_uint8 = uint8(nazwa_pliku_zedytowanego);
nazwa = [13];
j=1;
while znak ~=46
    nazwa = [nazwa znak];
    znak = nazwa_pliku_zedytowanego_uint8(j);
    j=j+1;
end
nazwa = [nazwa ':'];
m = numel(nazwa);     %odczyt d³ugoœci stringu
for i=m:61
    nazwa = [nazwa 0];  % uzupe³nienie stringo NULL'ami
end
nazwa = [nazwa 13 10];  % dodanie znaków koñca lini
fwrite(IDED,nazwa); % zapis linijki do pliku
j=1;
err = 0;
while j<100
    str = fgetl(IDNED); %wczytanie jednej linijki
    if str==-1
        str_1 = sprintf('%u: Nierozpoznany kod.', j);
        m = numel(str_1);     %odczyt d³ugoœci stringu
        for i=m:61
            str_1 = [str_1 0];  % uzupe³nienie stringo NULL'ami
        end
        str_1 = [str_1 13 10];  % dodanie znaków koñca lini
        fwrite(IDED,str_1); % zapis linijki do pliku
        j=j+1;
    else
        kod = sscanf(str,'%u');
        while j<kod
            str_1 = sprintf('%u: Nierozpoznany kod.', j);
            m = numel(str_1);     %odczyt d³ugoœci stringu
            for i=m:61
                str_1 = [str_1 0];  % uzupe³nienie stringo NULL'ami
            end
            str_1 = [str_1 13 10];  % dodanie znaków koñca lini
            fwrite(IDED,str_1); % zapis linijki do pliku
            j=j+1;
        end
        m = numel(str);     %odczyt d³ugoœci stringu
        if m>60
            err = [err kod];
        end
        for i=m:61
            str = [str 0];  % uzupe³nienie stringo NULL'ami
        end
        str = [str 13 10];  % dodanie znaków koñca lini
        fwrite(IDED,str); % zapis linijki do pliku
        j=j+1;
    end
end
fclose(IDNED);  %zamkniecie pliku
fclose(IDED);   %zamkniecie pliku
%type('MAZDA 626 ABS.txt');  %wyswietlenie zawartosci pliku