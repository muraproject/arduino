static int r = 6371;
const float Pi = 3.14159;
int accuracy = 5;
unsigned long lasttime = 0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 Serial.println(360.0-GetHeading(-6.211032, 106.759500,-6.210291, 106.760098));
 delay(1000);
}

static float GetHeading(float lat1, float long1, float lat2, float long2)
{
  //Current Latitude, Current Longitude
  //Projected Latitude, Projected Longitude
  //To get reverse heading, add 180
  lat1 = ConvertToRadians(lat1);
  lat2 = ConvertToRadians(lat2);
  long1 = ConvertToRadians(long1);
  long2 = ConvertToRadians(long2);
  float dLon = long2 - long1;
  float y = sin(dLon) * cos(lat2);
  float x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);
  return ConvertToStaticDegrees(atan2(y, x));
}

static float ConvertToDegrees(float rad)
{
  float degrees = rad / (Pi / 180);
  if (degrees <= -180) {
    return degrees + 360;
  }
  else {
    return degrees;
  }
}
static float ConvertToStaticDegrees(float rad)
{
  float degrees = rad / (Pi / 180);
  if (degrees <= 0) {
    return degrees + 360;
  }
  else {
    return degrees;
  }
}
static float ConvertToRadians(float angle)
{
  return (Pi / 180) * angle;
}
static float haversin(float input)
{
  return (sin(input / 2)) * (sin(input / 2));
}
