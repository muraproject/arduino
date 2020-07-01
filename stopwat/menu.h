void run() {
  while (mode == 0) {
    if (hit == 0) {
      boxMenits.clear();
      boxMenit.clear();
      boxStart.clear();
      boxStart.print("Ready");
      input_hitung_1 = EEPROM.read(3) * 60 + EEPROM.read(2);
      input_hitung_11 = EEPROM.read(5) * 60 + EEPROM.read(4);
      hitung1 = input_hitung_1;
      hitung11 = input_hitung_11;
      hitung_stopwat = 0;
      milidetik = 0;
      pembanding = 120;
      pembanding1 = 120;
      arr_count = 0;
    }
    hit = 1;
    kontrol();
  }
  while (mode == 1) {
    mundur();
    mundur1();
    for (int i = 0; i < 982; i++) {
      mode_countdown();
      delay(1);
    }



  }
  while (mode == 2) {
    stopwatch();
    mode_stopwatch();
    delay(8);
  }

  while (mode == 3) {
    set();
    mode_set();
    //delay(8);
  }

  while (mode == 4) {

    mode_after();
    //delay(8);
  }

  while (mode == 5) {
    mundur2();
    //mundur1();
    for (int i = 0; i < 982; i++) {
      mode_countdown();
      delay(1);
    }



  }

}
