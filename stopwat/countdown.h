void mundur() {
  int detikan = hitung1 % 60;
  int menitan = (hitung1 - detikan) / 60;
  if (menitan != pembanding) {
    pembanding = menitan;
    boxMenit.clear();
    if (menitan < 10) {
      boxMenit.print("0");
    }
    boxMenit.print(menitan);
    boxTitik.clear();
    boxTitik.print(":");
  }
  boxDetik.clear();
  if (detikan < 10) {
    boxDetik.print("0");
  }
  boxDetik.print(detikan);

  hitung1--;
  if (hitung1 < 0) {
    hitung1 = input_hitung_1;
  }
}

void mundur1() {
  int detikan1 = hitung11 % 60;
  int menitan1 = (hitung11 - detikan1) / 60;

  boxMenit1.clear();
  if (menitan1 < 10) {
    boxMenit1.print("0");
  }
  boxMenit1.print(menitan1);
  boxTitik1.clear();
  boxTitik1.print(":");
  boxDetik1.clear();
  if (detikan1 < 10) {
    boxDetik1.print("0");
  }
  boxDetik1.print(detikan1);

  hitung11--;
  if (hitung11 < 0) {
    hitung11 = input_hitung_11;
  }
}

void set() {


  int detikan = hitung1 % 60;
  int menitan = (hitung1 - detikan) / 60;
  int detikan1 = hitung11 % 60;
  int menitan1 = (hitung11 - detikan1) / 60;

  if (pembanding != detikan  || pembanding1 != detikan1) {
    pembanding = detikan;
    pembanding1 = detikan1;
    boxMenit.clear();
    if (menitan < 10) {
      boxMenit.print("0");
    }
    boxMenit.print(menitan);
    boxTitik.clear();
    boxTitik.print(":");

    boxDetik.clear();
    if (detikan < 10) {
      boxDetik.print("0");
    }
    boxDetik.print(detikan);


    boxMenit1.clear();
    if (menitan1 < 10) {
      boxMenit1.print("0");
    }
    boxMenit1.print(menitan1);
    boxTitik1.clear();
    boxTitik1.print(":");
    boxDetik1.clear();
    if (detikan1 < 10) {
      boxDetik1.print("0");
    }
    boxDetik1.print(detikan1);
  }



}


void mundur2() {
  int detikan = hitung1 % 60;
  int menitan = (hitung1 - detikan) / 60;
  if (menitan != pembanding) {
    pembanding = menitan;
    boxMenits.clear();
    if (menitan < 10) {
      boxMenits.print("0");
    }
    boxMenits.print(menitan);
    boxTitiks.clear();
    boxTitiks.print(":");
  }
  boxDetiks.clear();
  if (detikan < 10) {
    boxDetiks.print("0");
  }
  boxDetiks.print(detikan);

  hitung1--;
  if (hitung1 < 0) {
    hitung1 = input_hitung_1;
  }
}



void buzzer(int jumlah, int lamaOn, int lamaOff) {
  for (int i = 0; i < jumlah; i++) {
    digitalWrite(10, 1);
    delay(lamaOn);
    digitalWrite(10, 0);
    delay(lamaOff);
  }
}
