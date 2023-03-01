#include <stdio.h>
#include <string.h>

// esitell‰‰n funktiot
double Oton_maara(int tilin_saldo);
double nosta_rahat(int tilin_saldo, int nostettava_summa);
int tulosta_tapahtumat (int ottolkm, int otto_yht);
void clearBuffer(void);

//P‰‰ohjelma
int main(void)
{
    char tilinro[256];
    char Pin_koodi[256];
    char tarkistaPin[256];
    FILE *pFile;

    int valinta;

    int otot_yhteensa;
    int ottojen_lkm = 0;
    int otto_summa;

    int nostettava_rahasumma = 0;
    int saldo = 0;
    double saldodesimaali;


    printf("Tervetuloa!\n");
    printf("Anna tilinumero: "); //kysyt‰‰n tilin numeroa asiakkaalta
    fgets(tilinro, 256, stdin);
    tilinro[strcspn(tilinro, "\n")] = 0;
    strcat(tilinro, ".txt");

    if ((pFile = fopen(tilinro, "r")) == NULL){ //tiedosto avataan lukemista varten ja tarkistetaan onko se olemassa
            pFile = fopen(tilinro, "w+"); //mik‰li tiedostoa ei ollut jo olemassa, avtaan tiedosto kirjoitusmuodossa
            fputs("0000", pFile); //kirjoitetaan tiedoston PIN-koodiksi 0000
            fclose(pFile); //suljetaan tiedosto fclose-funktiolla k‰sittelyn lopuksi
    }

    do {
        pFile = fopen(tilinro, "r"); //avataan tiedosto jo olemassa ollut tai juuri ‰sken luotu tiedosto uudelleen
        fgets(tarkistaPin, 256, pFile); // tallennetaan tiedostossa oleva PIN-koodi fgets -funktiolla muuttujaan tarkistaPin

        printf("Anna nelinumeroinen PIN-koodi: "); //kysyt‰‰n k‰ytt‰j‰lt‰ PIN-koodi
        fgets(Pin_koodi, 256, stdin); // tallennetaan k‰ytt‰j‰n antama PIN-koodi fgets -funktiolla muuttujaan Pin_koodi
        Pin_koodi[strcspn(Pin_koodi, "\n")] = 0;

        if (tarkistaPin[strlen(tarkistaPin) - 1] == '\n')
            tarkistaPin[strlen(tarkistaPin) - 1] = '\0';

        if (tarkistaPin[strlen(tarkistaPin) - 1] == '\r')
            tarkistaPin[strlen(tarkistaPin) - 1] = '\0';

        tarkistaPin[strcspn(tarkistaPin, "\n")] = 0;

    if (strcmp(tarkistaPin, Pin_koodi) == 0) { //jos tekstitiedostossa oleva PIN-koodi ja k‰ytt‰j‰n antamaa PIN-koodi ovat samat
        printf("PIN oikein\n"); // jos PIN-koodi on oikein
        break; // poistutaan do-while-rakenteesta break-komennolla, mik‰li PIN-koodit ovat samat
    } else {
        printf("PIN-koodi ei ollut oikein\n"); // jos PIN-koodi on v‰‰rin
    }

    } while (1);// do-while-rakenteessa pysyt‰‰n niin kauan, kunnes PIN-koodit ovat samat


    printf("Anna tilin saldo positiivisena desimaalitasalukuna: \n"); //k‰ytt‰j‰n tulee syˆtt‰‰ tilin saldo
    scanf("%d",&saldo);
    scanf("%lf",&saldodesimaali);

    //jotta automaatin toimintoja voidaan k‰ytt‰‰, haluamme ett‰ tilill‰ on rahaa joten tarkistetaan saldo
    if (saldo < 0 || saldodesimaali > 0.00)  /* jos saldo on pienempi kuin nolla tai on positiivinen desimaaliluku*/
    {
        printf("Saldon kirjaus ei onnistunut");
    }
    else /* jos saldo on suurempi tai yht‰ suuri kuin nolla */
    {
        printf("Saldo kirjattiin onnistuneesti\n");

        while (saldo >= 0) //niin kauan kun saldo on suurempi tai yht‰ suuri kun 0 pysyt‰‰n while-toistorakenteessa
        {
            /* p‰‰valikko */
            printf("Valitse toiminto antamalla numero > (1) Otto // (2) Saldo // (3) Tapahtumat // (4) Lopeta\n"); //k‰ytt‰j‰ valitsee haluamansa toiminnon 1-4
            scanf("%d", &valinta);

            if (valinta == 1)   /* jos valinta on Otto */
            {
                nostettava_rahasumma = Oton_maara(saldo); /* Kutsutaan Oton_maara -funktiota parametrilla saldo ja tallennetaan t‰m‰ muuttujaan nostettava_rahasumma*/
                saldo = nosta_rahat(saldo, nostettava_rahasumma); //Kutsutaan nosta_rahat -funktiota parametreilla saldo ja nostettava_rahasumma + tallennetaan t‰m‰ saldo-muuttujaan
                otot_yhteensa += nostettava_rahasumma; /* plussataan muuttuja nostettava_summa arvo ottojen yhteissummaan */
                ottojen_lkm += 1; /*kasvatetaan muuttujaa ottojen_lkm m‰‰r‰‰ yhdell‰ */
            }

            if (valinta == 2)   /* jos valinta on Saldo */
            {
                printf("TILIN TILANNE:\n");
                tilinro[strlen(tilinro)-4] = 0;
                printf("Tilin %s saldo on %d euroa\n", tilinro, saldo);
            }

            if (valinta == 3) /* jos valinta on Tapahtumat */
            {
               tulosta_tapahtumat(ottojen_lkm, otot_yhteensa);
            }

            if ( valinta < 0 || valinta > 4) {
                printf("Valintaa ei ole olemassa, valitse uudestaan\n");

            }
            if (valinta == 4) /* jos valinta on Lopeta */
            {
                printf("Kiitos ja hei hei!\n");
                break;
            }
        }
    }

    return(0);

}

//Funktiot

/*
Oton_maara - kysyy k‰ytt‰j‰lt‰ nostettavan summan kokonaislukuna
Printtaa k‰ytt‰j‰lle kysymyksen ja lukee sen j‰lkeen n‰pp‰imistˆlt‰ syˆtteen (int) ja tallettaa t‰m‰n muuttujaan nostettava_summa.
Tarkistaa, ett‰ nostettava summa ei ole negatiivinen tai suurempi kuin tilin saldo.
Katsoo, ett‰ nostettava summa on joko 20e, 40e tai enemm‰n 10 euron v‰lein.

Parametrit:
 - tilin_saldo (int): saldo, paljonko tilill‰ on rahaa j‰ljell‰

Paluuarvo (int): nostettava_summa eli kokonaisluku, jonka k‰ytt‰j‰ syˆtt‰‰ vastauksena
 */

double Oton_maara(int tilin_saldo)
{
    int nostettava_summa;

    while(1){
        printf("Nostettava summa tulee olla 20e, 40e tai enemman 10 euron valein. Kertanoston raja on 1000.00 euroa.\n");
        printf("Anna summa jonka haluat nostaa: \n");
        scanf("%d", &nostettava_summa);

        if (nostettava_summa > 0.00 && nostettava_summa <= tilin_saldo) {
            for (int i = 0; i <= 960; i += 10) {
                if (nostettava_summa == 20 || nostettava_summa == 40 + i){
                    return nostettava_summa;
                }
            }
        }
        printf("Tuollaista summaa ei voida nostaa\n");
        continue;
    }

}

/*Nosta_rahat -funktio p‰ivitt‰‰ tilin saldoa v‰hent‰m‰ll‰ nostettavan summa saldosta.
Tarkistetaan, ett‰ nostettava suma ei ylit‰ tilin saldoa ja tehd‰‰n erotus.
T‰m‰n j‰lkeen katsotaan, miten asiakas saa rahat mahdollisimman isoina 20- ja 50-euron setelein‰.
Printataan automaatin antamat setelit, nostettu summa ja paljonko tilille j‰‰v‰ summa.

Parametrit:
 - tilin_saldo (int): saldo, paljonko tilill‰ on rahaa j‰ljell‰
 - nostettava_summa (int): summa, jonka asiakas haluaa tililt‰ nostaa

Paluuarvo (int): jaljella, tilin saldo noston j‰lkeen
 */

double nosta_rahat(int tilin_saldo, int nostettava_summa) {
    int jaljella;
    int viisikymppiset = 0;
    int kaksikymppiset = 0;
    int summa;


    if (nostettava_summa <= tilin_saldo) {
        jaljella = tilin_saldo - nostettava_summa;

        summa = nostettava_summa;
        do {
            if (nostettava_summa %50 == 0) {
                viisikymppiset += 1;
                nostettava_summa = nostettava_summa - 50;
            }
            else {
                kaksikymppiset += 1;
                nostettava_summa = nostettava_summa - 20;
            }
        } while (nostettava_summa != 0);

        printf("Ota rahat: %d kpl x 20 euron seteli, %d kpl x 50 euron seteli\n", kaksikymppiset, viisikymppiset);
        printf("Nostit juuri %d euroa ja tilin saldo on nyt %d euroa\n",summa, jaljella);
    }

    return jaljella;
}

/* tulosta_tapahtumat -funktion avulla k‰ytt‰j‰ voi tarkastella suorittamiansa toimintoja.
K‰ytt‰j‰lt‰ kysyt‰‰n aluksi, haluaako h‰n tiedot n‰ytˆlle vai kuitille.
T‰m‰n j‰lkeen k‰ytt‰j‰lle tulostetaan joko kuitti tai tiedot ruudulle, jossa n‰kyy onnistuneesti suoritettujen
ottojen lukum‰‰r‰ sek‰ niiden yhteissumma.

Parametrit:
 - ottolkm (int): jo suoritettujen, onnistuneiden ottojen lukum‰‰r‰
 - otto_yht (int): onnistuneiden ottojen yhteissumma, jonka asiakas on jo nostanut tililt‰

Paluuarvo (int): jaljella, tilin saldo noston j‰lkeen
 */


int tulosta_tapahtumat (int ottolkm, int otto_yht) {

    int tapahtumat;

    printf("Haluatko tiedot (1) Ruudulle vai (2) Kuitille ?\n"); /* kysyt‰‰n miten asiakas haluaa tiedot */
    scanf("%d", &tapahtumat);
        if (tapahtumat == 1)   /* jos valinta on 1 niin asiakas haluaa tiedot ruudulle */
        {
            printf("Ottoja on %d kappaletta yhteissummalla %d euroa\n", ottolkm, otto_yht);
        }
        if (tapahtumat == 2)     /* jos valinta on 2 niin asiakas haluaa tiedot kuitille */
        {
            printf("Ota kuitti\n");
        } if (tapahtumat != 1 && tapahtumat != 2) {
            printf("Valitettavasti jokin meni vikaan\n");

        }

        //return tapahtumat;

}

/*Funktio clearBuffer tyhjent‰‰ n‰pp‰imitˆpuskurin.

Parametrit:
 - funktio ei saa parametrej‰

Paluuarvo (void): funktio ei palauta mit‰‰n
*/

void clearBuffer(void){

   while (fgetc(stdin) != '\n');
}
