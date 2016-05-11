function  [sygnal] = generator_sygnalu_10kHz(ile_powt, kody,nazwa_pliku)
%% funkcja s�u�y generowaniu sygna�u do testowania czytnika kod�w b�yskowych
% generuje ona plik d�wi�kowy (.wav), w kt�rym kod jest modulowany amplitudowo
% jako pierwszy argument przyjmuje ilo�� powt�rze� serii kod�w,
% drugim jest wektor wierszowy generowanych kod�w, 
% a trzecim nazwa pod jak� zostanie zapisany plik

% domysla nazwa pliku
if nargin <3, nazwa_pliku ='codesigsin'; end
% parametry sygna�u

ilosc_kodow = numel( kody);
czestotliwosc_probek = 10000;
czestotliwosc_nosna = 10000;
czestotliwosc_sin = 1000;
czas_zwloki = 4;
czas_startu = 1.6;
czas_dziesiatki = 1.2;
czas_jedynki = 0.4;
przerwa_miedzy_wartosciami = 0.8;
przerwa_miedzy_powtorzeniami = 4;
przerwa_miedzy_kodami = 2;
amplituda_sygnalu = 0;
amplituda_przerwy = 1.5;

%generowanie nazwy plikunargin
nazwa_pliku = [nazwa_pliku '_x' num2str([ile_powt])];
for i = 1:ilosc_kodow,
    nazwa_pliku = [nazwa_pliku '_' num2str([kody(i)])]; 
end

nazwa_pliku = [nazwa_pliku '.wav'];


% bit startu
sygnal =  [ rectpulse(amplituda_przerwy,czestotliwosc_probek * czas_zwloki)' rectpulse(amplituda_sygnalu,czestotliwosc_probek*czas_startu)' rectpulse(amplituda_przerwy,czestotliwosc_probek*przerwa_miedzy_kodami)'];

% p�tla ilo�ci serii
for j = 1:ile_powt,

    % p�tla generowania kolejnych kod�w    
    for i = 1:ilosc_kodow,
        kody_cpy = kody;
        while kody_cpy(i)>9,
            sygnal =  [sygnal rectpulse(amplituda_sygnalu,czestotliwosc_probek*czas_dziesiatki)' rectpulse(amplituda_przerwy,czestotliwosc_probek*przerwa_miedzy_wartosciami)'];
            kody_cpy(i)= kody_cpy(i)-10;
        end
        while kody_cpy(i)>0,
            sygnal =  [sygnal rectpulse(amplituda_sygnalu,czestotliwosc_probek*czas_jedynki)' rectpulse(amplituda_przerwy,czestotliwosc_probek*przerwa_miedzy_wartosciami)'];
            kody_cpy(i)= kody_cpy(i)-1;
        end

        sygnal =  [sygnal rectpulse(amplituda_przerwy,czestotliwosc_probek*(przerwa_miedzy_kodami-przerwa_miedzy_wartosciami))'];
    end
    sygnal =  [sygnal rectpulse(amplituda_przerwy,czestotliwosc_probek*(przerwa_miedzy_powtorzeniami-przerwa_miedzy_kodami))'];
end

sygnal =  [sygnal rectpulse(amplituda_przerwy,czestotliwosc_probek*(przerwa_miedzy_powtorzeniami*2))'];


% zapis do pliku, mo�liwo�� wygenerowania sygna�u niemodulowanego, 
% jednak�e mog� wyst�pi� problemy z kart� d�wi�kow� (impulsy zamiast
% sygna�u ci�g�ego.

%wavwrite(sygnal,czestotliwosc_nosna,nazwa_pliku);
plot(sygnal);
wavwrite(ammod(sygnal,czestotliwosc_sin,czestotliwosc_nosna),czestotliwosc_nosna,nazwa_pliku);
  


end

