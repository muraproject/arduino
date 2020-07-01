void stopwatch() {

  int detikan = hitung_stopwat % 60;
  int menitan = (hitung_stopwat - detikan) / 60;
  if (detikan >= 2) {
    digitalWrite(10, 0);
  }
  if (detikan == 0 && milidetik == 0) {
    pembanding = menitan;
    boxMenit.clear();
    boxMenit.print("00");
    boxTitik.clear();
    boxTitik.print(":");
    boxDetik.clear();
    boxDetik.print("00");
  }



  if (milidetik > 99) {
    milidetik = 0;
    hitung_stopwat++;
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
    boxcount.clear();
    boxcount.print(arr_count);
  }


  boxMill.clear();
  if (milidetik < 10) {
    boxMill.print("0");
  }
  boxMill.print(milidetik);
  milidetik++;
}




void after() {
  boxMenit.clear();
  if (arr_min[arr_count] < 10) {
    boxMenit.print("0");
  }
  boxMenit.print(arr_min[arr_count]);
  boxTitik.clear();
  boxTitik.print(":");

  boxDetik.clear();
  if (arr_sec[arr_count] < 10) {
    boxDetik.print("0");
  }
  boxDetik.print(arr_sec[arr_count]);


  boxcount.clear();
  //      if(menitan1<10){
  //        boxMenit1.print("0");
  //      }
  boxcount.print(arr_count + 1);
  //      boxTitik1.clear();
  //      boxTitik1.print(":");
  boxMill.clear();
  if (arr_mill[arr_count] < 10) {
    boxMill.print("0");
  }
  boxMill.print(arr_mill[arr_count]);
}
