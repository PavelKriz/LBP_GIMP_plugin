# Pavel's LBP Gimp Plugin
## Zadání
LBP Příznaky	Vytvořte plug-in do GIMPu počítající a zobrazující příznaky (parametrické) LBP	30b

Vytvořte plugin počítající a zbrazující základní, nijak nerozšířené LBP příznaky.

Lze získat dodatečných 10b za vytvoření parametrických příznaků (poloměr).

Lze získat dodatečných 10b za použítí centralised LBP.

Lze získat dodatečných 10b za použítí dominant LBP.

Úloho lze dále rozšížit o další varianty LBP příznaků.

## Poznámka pro hodnotící
 Po konzultaci se cvičícím jsem implementoval zadání za celkem 50 bodů. Implementoval jsem základní úlohu, rozšířil o další metody LBP. Podmínka konzultujícího učitele pro 50 bodovou úlohu, byla implementace složitějšího LBP, tím je zde implementované CLBP. Také přidáno do zadání trochu vylepšit UI pluginu, nyní je trochu interaktivní. Další výhody proč si myslím, že moje implementace takového pluginu je dobrá uvádím v sekci Vlastnosti pluginu. Také bych se snažil zmínit hlavně dokumentaci, kterou jsem se snažil dělat obsáhle, protože mě takového něco chybělo při tvoření pluginu. Zvlášť žádná z ukázkových prací studentů není psaná v c++. Snažil jsem se o to, jestliže je tato práce obstojná, aby mohla pomoct budoucím studentům.

## Úvod
Podle zadání byl naiplementován plugin do editoru fotek Gimp. Cílem pluginu je vypočíst a vyzualizovat zvolené druhy LBP příznaků. Implementované LBP metody zínskávání příznaků jsou LBP, mLBP, ULBP, CLBP. Popis těchto algoritmů uvádím v příloze Popis použitých algoritmů. Další popisy použitých algoritmů se dají naléz v dokumentaci kódu. 

**Poznámka**:

Velká část pluginu se dá využít jako kostra při programování jiného plugin do programu Gimp. Stačí naimplementovat rozhraní CAlghoritm(podle dokumentace kódu), případně upravit UI část pluginu a ve třídě CProcessHandler umožnit spouštění nově naprogramovaného algortimu(podle dokumentace kódu).

## Důležité odkazy

<a href="https://github.com/PavelKriz/LBP_GIMP_plugin/tree/master/code">Složka obsahující kód pluginu a instalační skript compile.sh.</a> 

<a href="https://github.com/PavelKriz/LBP_GIMP_plugin/tree/master/docs/html">Složka s Doxygen html dokumentací kódu.</a>

<a href="https://github.com/PavelKriz/LBP_GIMP_plugin/blob/master/docs/html/index.html">Úvodní vstup do dokumentace.</a>

<a href="https://github.com/PavelKriz/LBP_GIMP_plugin/blob/master/Algoritmy/LBP.pdf">PDF dokumentace algoritmů.</a> 

## Vlastnosti pluginu

* Kód pluginu je na rozsah práce rozsáhle dokumentován.
* Plugin se dá velice rychle předělat na jinou funkčnost. Stačí naimplementovat rozhraní CAlghoritm(podle dokumentace), případně upravit UI část pluginu a ve třídě CProcessHandler umožnit spouštění nově naprogramovaného algortimu(podle dokumentace). Použitím tohoto postupu programátor ušetří velké množství času při stejné funkčnosti.
* Plugin je naimplementován v C++ a díky tomu je rychlý.
* Zpracovávání obrázku se vykonává po dlaždicích, je tedy velice nenáročný na paměť.
* Zpracovávání po částech není v LBP primitivní úloha jelikož k výpočtu hodnoty pixelu je zapotřebí jeho okolí. Okolí dlaždice je buď získáno z obrázku nebo zrcadlením dlaždice přes hranu obrázku.
* Souřadnice okolních bodů pro výpočet lokálního LBP příznaku jsou interpolovány z dvou pixelů. Získané hodnoty jsou tak přesnější.
* Plugin obsahuje interaktivní UI, které se mění na základě volby metody získávání LBP.

## Další dokumentace

Vytvořil jsem další dvě dokumentace.

PDF dokumntace popisující použité LBP metody se dá najít ve složce ./Algoritmy 

Dokumentace kódu je umístěná ve složce ./Docs/html a do úvodní stránky se spustí otevřením souboru index.html. Jde o vygenerovoanu html dokumentaci kódu. Dokumentace byla vygenerována z kódu, hlavně hlavičkových souborů. Tato dokumentace se zde nachází i v nezkompilované latex podobě ve složce ./Docs/latex.

## Instalace

Návod je napsaný pro operační systém Linux. Jelikože se plugin sestává z více složek a využívá knihovny navíc nedá se zkompilovat pomocí základního instalátoru pluginů pro Gimp gimptool-2.0. Instalace je však jednoduchá. Všechny soubory ze složky ./code mít ve stejném adresáři. Z tohoto adresáře spustit příkaz [gimptool-2.0 --install PavelsLBPGimpPlugin.cpp]. Tento příkaz skončí neúspěchem. Je potřeba ignorovat chybová hlášení a je potřeba zkopírovat část kterou gimptool vypíše na začátku výpisu od gcc až po poslední přidanou knihovnu (měla by to být -lfreetype). Následně spustit ze stejného adresáře příkaz [./compile.sh "[zkopírovaná část]"]. Teď by již měl být plugin úspěšně zkompilován do správné složky která slouží programu Gimp pro pluginy. 

Plugin je následovně přístupný přes filters/misc/PavelsLBPGimpPlugin. Aby mohl být plugin zapnut tak musí být otevřený nějaký obrázek.

Poznámka: Hranaté závorky do příkazů nepatří, jen označují oblast ve které je příkaz nebo nějaká proměná.


## Implementované LBP metody

Zpracované výsledky pluginu budou předvedeny na následujicím obrázku. Oblast hadru na obrázku je zajímavá svojí texturou, ta je na vizualizacích LBP dobře vidět. Také ja zajímavé si všimnout absenci vlivu odstínů barev na LBP. DObře je to vidět například na hadru nebo i na paletě. Zajímavé mohou být například i hrany a to hrana palaty nebo štětce prezentující neostré hrany. Obrázek tak umožňuje pozorovat rozdíly v LBP v zaostřené části obrazku a v nezaostřené části obrazku.

<figure>
<img src=example_images/testImage.JPG width="400">
<figcaption>Obrázek použitý pro ukázku práce pluginu.</figcaption>
</figure>

Autorem obrázku je autor pluginu.

Ve všech případech implementací je rozsah radia maximálně 190 a minimálně 1. Počet sousedních bodů se liší podle implementované metody, minimum je však 4 a maximum ze všech metod je 24.

### LBP 

LBP je metoda získávání příznaků ze které další LBP metody vychází. Plugin zobrazuje hodnoty LBP podle nákresu 1. Každá barva má tak svůj význam.

Násoledující diagram vysvětluje význam jednotlivých kanálů RGB při zobrazení LBP. Barvy vysvětlují jaká část okolí měla vliv na danou hodnotu LBP. Každý barevný kanál tak zobrazuje hodnotu jako kdyby se LBP suma počítala pouze pro body z daného okolí. Okolím jsou myšleny sousední body. Vše záleží na počtu sousedních bodů. Podle počtu sousedních bodů se zobrazují nebo nezobrazují další barevné kanály. Pro 24 sousedních bodů je tak 8 sousedních bodů pro každý barevný kanál. Kdyby se však počet sousedních bodů byl jen 19 tak bude červený kanál pro prvních osm bodů ve směru šipek, zelený kanál pro dalších  osm následujících bodů a nakonec pro zbývající 3 body modrý kanál. Takto analogicky pro jiné počty sousedních bodů.
<figure>
<figcaption>Diagram 1 - vizalizace hodnot, p = počet sousedních bodů pixelu</figcaption>
<img src=GimpPluginLBPSousedniBodydrawio.JPG width="1000">
</figure>

<figure>
<img src=example_images/LBP10r8p.JPG width="400" alt="LBP s parametry radius = 10 a počet sousedních bodů = 8">
<figcaption>LBP s parametry radius = 10 a počet sousedních bodů = 8</figcaption>

<img src=example_images/LBP1r16p.JPG width="400" alt="LBP s parametry radius = 1 a počet sousedních bodů = 16">
<figcaption>LBP s parametry radius = 1 a počet sousedních bodů = 16, Zajímavé si povšimnout že červená barva je pro hodnoty LBP sumy pouze s dolními sousedními body a zelená naopak</figcaption>

<img src=example_images/LBP10r24p.JPG width="400" alt="LBP s parametry radius = 10 a počet sousedních bodů = 24">
<figcaption>LBP s parametry radius = 10 a počet sousedních bodů = 24</figcaption>
</figure>

### mLBP

Metoda mLBP je odvozená z LBP a její výsledky jsou podobné LBB. Vizualizace mLBP má stejná pravidla jako LBP viz diagram 1.

<figure>
<img src=example_images/mLBP10r8p.JPG width="400" alt="mLBP s parametry radius = 10 a počet sousedních bodů = 8">
<figcaption>mLBP s parametry radius = 10 a počet sousedních bodů = 8</figcaption>
</figure>

### ULBP

Metoda ULBP je výpočtem stejná základnímu LBP ale vynechává neuniformní hodnoty. V pluginu jsou ULBP vizualizovány  stejně jako LBP o max 16 bodech s tím že modrá barva simbolizuje vynechaný pixel s neuniformní hodnotou LBP. Na těchto obrázcích, které byli vytvořeny se stejnými parametry jen jednou s LBP a podruhé s ULBP, lze vidět oblasti s neuniformními hodnotami.

Metoda LBP se zobrazuje podobně jako LBP jen s tím rozdílem, že maximální počet sousedních bodů je jen 16 a neuniformní hodnoty se zobrazují jako RGB(0,0,255).

<figure>
<img src=example_images/LBP190r16p.JPG width="400" alt="LBP s parametry radius = 190 a počet sousedních bodů = 16">
<figcaption>LBP s parametry radius = 190 a počet sousedních bodů = 16</figcaption>

<img src=example_images/ULBP190r16p.JPG width="400" alt="ULBP s parametry radius = 190 a počet sousedních bodů = 16">
<figcaption>ULBP s parametry radius = 190 a počet sousedních bodů = 16</figcaption>
</figure>

### CLBP

Výpočet CLBP se skládá ze třech částí CLBP_S, CLBP_M, CLBP_C. Plugin je vizualizuje všechny tři najednou CLBP_S( červený kanál RGB), CLBP_M( zelený kanál RGB) a CLBP_C( modrý kanál LBP). Jednotlivé kanály pak vizulizují hodnoty CLBP podčástí v rozsahu 0 až 255. Maximální počet sousedních bodů je 8.

<figure>
<img src=example_images/CLBP10r8p.JPG width="400" alt="CLBP s parametry radius = 10 a počet sousedních bodů = 8">
<figcaption>CLBP s parametry radius = 10 a počet sousedních bodů = 8</figcaption>
</figure>

#### CLBP_S

Plugin umí zobrazit i jednotlivé komponenty CLBP. Hodnoty se mapují od nejmenších( nejtmavší) po největší(nejsvětlejší). Nejvyšší hodnota není bílá ale je to zabarvená bílá. Zde je zabarvená červenou, jelikož v CLBP je CLBP_S reprezentováno červeným kanálem RGB.

<figure>
<img src=example_images/CLBP_S10r8p.JPG width="400" alt="CLBP_S s parametry radius = 10 a počet sousedních bodů = 8">
<figcaption>CLBP_S s parametry radius = 10 a počet sousedních bodů = 8</figcaption>
</figure>

#### CLBP_M

Plugin umí zobrazit i jednotlivé komponenty CLBP. Hodnoty se mapují od nejmenších( nejtmavší) po největší(nejsvětlejší). Nejvyšší hodnota není bílá ale je to zabarvená bílá. Zde je zabarvená zelenou, jelikož v CLBP je CLBP_M reprezentováno zeleným kanálem RGB.

<figure>
<img src=example_images/CLBP_M10r8p.JPG width="400" alt="CLBP_M s parametry radius = 10 a počet sousedních bodů = 8">
<figcaption>CLBP_M s parametry radius = 10 a počet sousedních bodů = 8</figcaption>
</figure>

#### CLBP_C

Plugin umí zobrazit i jednotlivé komponenty CLBP. Hodnoty se mapují od nejmenších( nejtmavší) po největší(nejsvětlejší). Nejvyšší hodnota není bílá ale je to zabarvená bílá. Zde je zabarvená modrou, jelikož v CLBP je CLBP_C reprezentováno modrým kanálem RGB.

<figure>
<img src=example_images/CLBP_C10r8p.JPG width="400" alt="CLBP_C s parametry radius = 10 a počet sousedních bodů = 8">
<figcaption>CLBP_C s parametry radius = 10 a počet sousedních bodů = 8</figcaption>
</figure>

## Krátké shrnutí funkcí

Na všech funkcích je vidět, že jsou nějakým způsobem podobné. Není tedy divu, že mají všechny v názvu LBP. CLBP obsahuje nejvíce informací a hodnota LBP je zobrazována už jiným způsobem. Je však ze všech funkcí nejnáročnější na výpočet.
