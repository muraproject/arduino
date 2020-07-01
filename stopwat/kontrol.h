void kontrol() {
  if (tombolA) {
    buzzer(2, 75, 75);
    mode = 1;
    boxStart.clear();
    boxStart.print("Count");
    delay(2000);
    boxStart.clear();
  }
  if (tombolB) {
    while (tombolB) {

    }
//    digitalWrite(10, 1);
//    delay(100);
//    digitalWrite(10, 0);
//    delay(1000);
//    digitalWrite(10, 1);
//    delay(100);
//    digitalWrite(10, 0);
//    delay(1000);
//    digitalWrite(10, 1);

    mode = 2;
    boxStart.clear();
  }
  if (tombolC) {
    while (tombolC) {
      buzzer(2, 75, 75);
      mode = 5;
      boxStart.clear();
      boxStart.print("count");
      delay(2000);
      boxStart.clear();
      //dmd.selectFont(fontku);
    }
  }
  if (tombolD) {
    buzzer(2, 75, 75);
    mode = 3;
    boxStart.clear();
    boxStart.print("Sett.");
    delay(2000);
    boxStart.clear();
  }
}

void mode_stopwatch() {
  if (tombolA) {
    while (tombolA) {

    }
  }

  if (tombolB) {
    delay(75);
    digitalWrite(10, 1);
    delay(75);
    digitalWrite(10, 0);
    int detikan = hitung_stopwat % 60;
    int menitan = (hitung_stopwat - detikan) / 60;
    arr_sec[arr_count] = detikan;
    arr_min[arr_count] = menitan;
    arr_mill[arr_count] = milidetik;
    arr_count++;
    if (arr_count > 30) {
      arr_count = 30;
    }
    while (tombolB) {
      stopwatch();
      delay(8);
    }
    boxcount.clear();
    boxcount.print(arr_count);
  }

  if (tombolC) {
    buzzer(3, 75, 75);
    mode = 0;
    hit = 0;
  }

  if (tombolD) {
    delay(75);
    digitalWrite(10, 1);
    delay(75);
    digitalWrite(10, 0);
    delay(75);
    digitalWrite(10, 1);
    delay(75);
    digitalWrite(10, 0);
    while (tombolD) {

    }
    arr_limit = arr_count - 1;
    arr_count = 0;
    mode = 4;
    after();

  }
}
void mode_countdown() {
  if (tombolA) {
    while (tombolA) {

    }
  }

  if (tombolB) {
    while (tombolB) {

    }
  }

  if (tombolC) {
    buzzer(3, 75, 75);
    mode = 0;
    hit = 0;
  }

  if (tombolD) {
    while (tombolD) {

    }

  }
}


void mode_set() {
  if (tombolA) {
    buzzer(1, 75, 75);
    while (tombolA) {

    }
    if (setting) {
      setting = false;
      //      cursor1.clear();
      //      cursor1.print(".");
    } else {
      setting = true;
      //      cursor2.clear();
      //      cursor2.print(".");
    }
  }

  if (tombolB) {
    //    while(tombolB){
    //
    //    }
    buzzer(1, 75, 75);
    if (setting) {
      hitung1++;
    } else {
      hitung11++;
    }
  }

  if (tombolC) {
    while (tombolC) {

    }
    buzzer(3, 75, 75);
    byte ee_sec = hitung1 % 60;
    byte ee_min = (hitung1 - ee_sec) / 60;
    byte ee_sec1 = hitung11 % 60;
    byte ee_min1 = (hitung11 - ee_sec1) / 60;
    EEPROM.write(2, ee_sec);
    EEPROM.write(3, ee_min);
    EEPROM.write(4, ee_sec1);
    EEPROM.write(5, ee_min1);
    mode = 0;
    hit = 0;
    boxMenit.clear();
    boxStart.clear();
    boxStart.print("done!");
    delay(1500);

  }

  if (tombolD) {
    buzzer(1, 75, 75);
    if (setting) {
      hitung1--;
    } else {
      hitung11--;
    }
  }
}


void mode_after() {
  if (tombolA) {
    while (tombolA) {

    }
  }

  if (tombolD) {
    buzzer(1, 75, 75);

    arr_count++;
    if (arr_count > arr_limit) {
      arr_count = 0;
    }
    after();
  }

  if (tombolC) {
    buzzer(1, 75, 75);

    mode = 0;
    hit = 0;
  }

  if (tombolB) {
    buzzer(1, 75, 75);
    arr_count--;
    if (arr_count == 255) {
      arr_count = arr_limit;
    }
    after();

  }
}
