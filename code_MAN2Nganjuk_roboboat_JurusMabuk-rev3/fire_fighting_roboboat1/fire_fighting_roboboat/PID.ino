void follow_wall_kiri() {

  int jarakKanan   = get_data(trigKanan, echoKanan);
  int jarakKiri   = get_data(trigKiri, echoKiri);
  int error = jarakKiri - setPoint;

  int rateError = error - lastError;
  lastError = error;
  int moveVal = (int) (error * kp) + (rateError * kd);
  int moveLeft = speed - moveVal;
  int moveRight = speed + moveVal;
  int minSpeed = -125;
  int maxSpeed = 255;
  moveLeft = constrain(moveLeft, minSpeed, maxSpeed);
  moveRight = constrain(moveRight, minSpeed, maxSpeed);

  Serial.print(moveLeft);
  Serial.print("     ");
  Serial.println(moveRight);

  if (jarakKanan < 30 && jarakKiri < 30) {
    digitalWrite(fanPin, HIGH);
    setMotor(0, 0);
    if (fanState == 0) {
      servoFanState--;
      if (servoFanState < 0) fanState = 1;
    }
    else {
      servoFanState++;
      if (servoFanState > 180) fanState = 0;
    }
  }
  else {
    digitalWrite(fanPin, LOW);
    setMotor(moveLeft, moveRight);
    servoFanState = 90;
  }
  fan.write(servoFanState);
}


void follow_wall_kanan() {

  int jarakKanan   = get_data(trigKanan, echoKanan);
  int jarakKiri   = get_data(trigKiri, echoKiri);
  int error = jarakKanan - setPoint;

  int rateError = error - lastError;
  lastError = error;
  int moveVal = (int) (error * kp) + (rateError * kd);
  int moveRight = speed - moveVal;
  int moveLeft = speed + moveVal;
  int minSpeed = -255;
  int maxSpeed = 255;
  moveLeft = constrain(moveLeft, minSpeed, maxSpeed);
  moveRight = constrain(moveRight, minSpeed, maxSpeed);

  Serial.print(moveLeft);
  Serial.print("     ");
  Serial.println(moveRight);

  if (jarakKanan < 30 && jarakKiri < 30) {
    digitalWrite(fanPin, HIGH);
    setMotor(0, 0);
    if (fanState == 0) {
      servoFanState--;
      if (servoFanState < 0) fanState = 1;
    }
    else {
      servoFanState++;
      if (servoFanState > 180) fanState = 0;
    }
  }
  else {
    digitalWrite(fanPin, LOW);
    setMotor(moveLeft, moveRight);
    servoFanState = 90;
  }
  fan.write(servoFanState);
}


void setMotor(int L, int R) {
  if (L > 0) {
    digitalWrite(pin_MOTOR_DIRL1, 0);
    digitalWrite(pin_MOTOR_DIRL2, 1);
  } else {
    digitalWrite(pin_MOTOR_DIRL1, 1);
    digitalWrite(pin_MOTOR_DIRL2, 0);
    L = 255 + L;
  }
  analogWrite(pin_MOTOR_PWML, L);
  if (R > 0) {
    digitalWrite(pin_MOTOR_DIRR1, 0);
    digitalWrite(pin_MOTOR_DIRR2, 1);
  } else {
    digitalWrite(pin_MOTOR_DIRR1, 1);
    digitalWrite(pin_MOTOR_DIRR2, 0);
    R = 255 + R;
  }
  analogWrite(pin_MOTOR_PWMR, R);
}
