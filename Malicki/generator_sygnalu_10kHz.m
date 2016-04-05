function  [sygnal] = generator_sygnalu_10kHz(ile_powt, kody,nazwa_pliku)
% funkcja s³u¿y generowaniu sygna³u do testowania czytnika kodów b³yskowych
% generuje ona plik dŸwiêkowy (.wav), w którym kod jest modulowany amplitudowo
% jako pierwszy argument przyjmuje iloœæ powtórzeñ serii kodów,
% drugim jest wektor wierszowy generowanych kodów, 
% a trzecim nazwa pod jak¹ zostanie zapisany plik

% parametry sygna³u
ilosc_kodow = numel( kody);
czestotliwosc_probek = 10000;
czestotliwosc_nosna = 10000;
czestotliwosc_sin = 1000;
czas_startu = 1.6;
czas_dziesiatki = 1.2;
czas_jedynki = 0.4;
przerwa_miedzy_wartosciami = 0.8;
przerwa_miedzy_powtorzeniami = 4;
przerwa_miedzy_kodami = 2;
amplituda = 1;


% bit startu
sygnal =  [rectpulse(amplituda,czestotliwosc_probek*czas_startu)' rectpulse(0,czestotliwosc_probek*przerwa_miedzy_kodami)'];

% pêtla iloœci serii
for j = 1:ile_powt,

% pêtla generowania kolejnych kodów    
for i = 1:ilosc_kodow,
    kody_cpy = kody;
    while kody_cpy(i)>9,
        sygnal =  [sygnal rectpulse(amplituda,czestotliwosc_probek*czas_dziesiatki)' rectpulse(0,czestotliwosc_probek*przerwa_miedzy_wartosciami)'];
        kody_cpy(i)= kody_cpy(i)-10;
    end
    while kody_cpy(i)>0,
        sygnal =  [sygnal rectpulse(amplituda,czestotliwosc_probek*czas_jedynki)' rectpulse(0,czestotliwosc_probek*przerwa_miedzy_wartosciami)'];
        kody_cpy(i)= kody_cpy(i)-1;
    end
    
    sygnal =  [sygnal rectpulse(0,czestotliwosc_probek*(przerwa_miedzy_kodami-przerwa_miedzy_wartosciami))'];
end
sygnal =  [sygnal rectpulse(0,czestotliwosc_probek*(przerwa_miedzy_powtorzeniami-przerwa_miedzy_kodami))'];
end

sygnal =  [sygnal rectpulse(0,czestotliwosc_probek*(przerwa_miedzy_powtorzeniami*2))'];


% zapis do pliku, mo¿liwoœæ wygenerowania sygna³u niemodulowanego, 
% jednak¿e mog¹ wyst¹piæ problemy z kart¹ dŸwiêkow¹ (impulsy zamiast
% sygna³u ci¹g³ego.

%wavwrite(sygnal,czestotliwosc_nosna,nazwa_pliku);
wavwrite(ammod(sygnal,czestotliwosc_sin,czestotliwosc_nosna),czestotliwosc_nosna,nazwa_pliku);
end

