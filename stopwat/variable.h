#define tombolA digitalRead(4)
#define tombolB digitalRead(2)
#define tombolC digitalRead(5)
#define tombolD digitalRead(3)

SoftDMD dmd(1, 1); // DMD controls the entire display
SoftDMD dmd1(1, 1);
DMD_TextBox box(dmd, 0, 0);
//DMD_TextBox box1(dmd1, 0, 0);

DMD_TextBox boxDetik(dmd, 18, 0);
DMD_TextBox boxMenit(dmd, 3, 0);
DMD_TextBox boxTitik(dmd, 14, 0);
DMD_TextBox boxDetik1(dmd, 18, 9);
DMD_TextBox boxMenit1(dmd, 3, 9);
DMD_TextBox boxTitik1(dmd, 14, 9);
DMD_TextBox cursor1(dmd, 0, 0);
DMD_TextBox cursor2(dmd, 0, 11);
DMD_TextBox boxStart(dmd, 1, 4);
DMD_TextBox boxcount(dmd, 1, 9);
DMD_TextBox boxMill(dmd, 19, 9);

DMD_TextBox boxDetiks(dmd1, 17, 1);
DMD_TextBox boxMenits(dmd1, 0, 1);
DMD_TextBox boxTitiks(dmd1, 14, -1);

int input_hitung_1 = 300;
int input_hitung_11 = 45;
int hitung1 = input_hitung_1;
int hitung11 = input_hitung_11;
int hitung_stopwat;

short milidetik = 0;

byte pembanding = 120;
byte pembanding1 = 120;
byte hit = 0;

byte mode = 0;

byte arr_count = 0;
byte arr_limit = 0;
byte arr_min[32];
byte arr_sec[32];
byte arr_mill[32];

boolean setting = true;
