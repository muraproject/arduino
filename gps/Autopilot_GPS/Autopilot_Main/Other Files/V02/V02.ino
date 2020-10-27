#include <Average.h>
#include <SoftwareSerial.h>
#include <PWMServo.h>
#include <Wire.h>
#include <HMC5883L.h>
#include "TinyGPS++.h"

int compassavg = 50;
PWMServo rudder;

int maxspeed = 40;
int waypointdist = 8;
//INITIALLY 4 DOWN

int prevangle = 90;
int accuracy = 4;

int rudderspeed = 10;

int lock = 12;
int distance = 7;
int data = 5;

int gpsavg = 10;

int ruddertrim = -2;
int compasstrim = 180;

int ang = 0;

static int r = 6371;
const float Pi = 3.14159;

HMC5883L compass;
int error = 0;

TinyGPSPlus gps;

unsigned long startTime;
unsigned long otherTime;

SoftwareSerial ss(5, 4);

void setup() {
  Serial.begin(4800);
  rudder.attach(9);
  Wire.begin();
  compass = HMC5883L();
  error = compass.SetScale(0.88);
  //if(error != 0)
  //Serial.println(compass.GetErrorText(error));
  error = compass.SetMeasurementMode(Measurement_Continuous);
  //if(error != 0)
  //Serial.println(compass.GetErrorText(error));
  ss.begin(9600);      
  //Serial.println("Latitude       Longitude        IconHeading     Description     LineColor       Icon");
}

void loop() {

  startTime = millis();
  float bearing = RealBearing(49,-123,GetBearing());
  int heading = 0;
  RudderTurn(RudderAngle(bearing,heading));
  Serial.println(millis() - startTime);
  delay(200);
  ////Serial.print("   \tHeading:\t");
  ////Serial.print(bearing);
  ////Serial.println();

  //Triangle
  //GoToWaypoint(49.223473, -122.679370); //112.34m, 122.85y, 74.63d
  //GoToWaypoint(49.223736, -122.677883); // 84.83m,  92.78y, 209.2d
  //GoToWaypoint(49.223072, -122.678449); // 80.15m,  87.65y, 303.6d

  //Triangle Total: 303.28 yards

  //Arrow Tail
  //GoToWaypoint(49.223579, -122.678757); //33.35m, 36.47y, 343.65d
  //GoToWaypoint(49.223865, -122.678884); //16.69m, 18.25y,  75.59d
  //GoToWaypoint(49.223904, -122.678662); //33.09m, 36.19y, 167.13d
  //GoToWaypoint(49.223616, -122.678559); //14.82m, 16.21y, 255.45d


  //RudderTurn(90);

  //Triangle
  //GoByPath(49.223782, -122.679067);
  //GoByPath(49.223497, -122.679675);
  //GoByPath(49.223579, -122.678757);

  //GoByPath(49.224057, -122.679166);
  //GoByPath(49.223497, -122.679675);

  //GoByPath(49.139930, -122.242968);
  //GoByPath(49.146120, -122.259322);
  //GoByPath(49.127374, -122.297720);

  //GoToWaypoint(49.2294022,-122.6927801);
  //GoToWaypoint(49.22826214,-122.692901);

  //Pond Path 2 2


  //GoToWaypoint(49.231650, -122.695583);
  //GoToWaypoint(49.231660, -122.695334);


  //RudderTurn(90);
  //delay(2000);

  //Serial.println(GetBearing());
  //Serial.println("All waypoints traveled to");
  //delay(10000);
  //RoutineDone();
  //Arrow Tail Total: 90.4 yards
  //KeepHeading(0);
}

static void KeepHeading(int heading) {
  float bearing = RealBearing(49,-123,GetBearing());
  RudderTurn(RudderAngle(bearing,heading));
  delay(200);
}

static void GoByPath(float lat2, float long2){
  gps.encode(ss.read());
  while (1 > 0){
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      digitalWrite(data, HIGH);
    }
    else {
      digitalWrite(data, LOW);
    }
    while (ss.available() > 0){
      gps.encode(ss.read());
      if (gps.location.isUpdated())
      {
pathagain:
        if (gps.speed.mps() > maxspeed){ 
          delay(100);
          goto pathagain;
        }

        int latslice = gpsavg;
        int lati;
        float lataverage[latslice];
        for (lati = 0; lati < latslice; lati = lati + 1) {
          lataverage[lati] = gps.location.lat();
        }
        float lat1 = mean(lataverage, latslice);

        int longslice = gpsavg;
        int longi;
        float longaverage[longslice];
        for (longi = 0; longi < longslice; longi = longi + 1) {
          longaverage[longi] = gps.location.lng();
        }
        float long1 = mean(longaverage, longslice);

        ////Serial.println("Got Coordnates");
        float heading = GetHeading(lat1, long1, lat2, long2);
        while (1 > 0){
          if (GetDistanceInM(lat1, long1, lat2, long2) > accuracy){
            float heading = GetHeading(lat1, long1, lat2, long2);
            long1=GetPosition(lat1, long1, waypointdist, heading, 2);
            lat1=GetPosition(lat1, long1, waypointdist, heading, 1);
            //Serial.print(lat1,8);
            //Serial.print("    ");
            //Serial.print(long1,8);
            //Serial.print("    ");
            //Serial.print(0);
            //Serial.print("               ");
            //Serial.print(gps.speed.mps());
            //Serial.print("            ");
            //Serial.print("none");
            //Serial.print("            ");
            //Serial.print("111");
            //Serial.println();
            GoToWaypoint(lat1, long1);
            //delay(200);
          }
          else {
            //Serial.println("Waypoint Reached");
            return; 
          }
        }
      }
    }
  }
}

static void GoToWaypoint(float lat2, float long2){
  gps.encode(ss.read());
  while (1 > 0){
    while (ss.available() > 0){
      gps.encode(ss.read());
      if (gps.location.isUpdated())
      {
        digitalWrite(lock, HIGH);
        float lat1, long1;
        lat1 = gps.location.lat();
        long1 = gps.location.lng();
        if (GetDistanceInM(lat1, long1, lat2, long2) < (accuracy/1.9)){
          ////Serial.println("Path Point Reached");
          digitalWrite(distance, HIGH);
          delay(500);
          digitalWrite(distance, LOW);
          return;
        }
        else {

gpsagain:

          int latslice = gpsavg;
          int lati;
          float lataverage[latslice];
          for (lati = 0; lati < latslice; lati = lati + 1) {
            lataverage[lati] = gps.location.lat();
          }
          float lat1 = mean(lataverage, latslice);

          int longslice = gpsavg;
          int longi;
          float longaverage[longslice];
          for (longi = 0; longi < longslice; longi = longi + 1) {
            longaverage[longi] = gps.location.lng();
          }
          float long1 = mean(longaverage, longslice);

          if (gps.speed.knots() > maxspeed){ 
            delay(100);
            goto gpsagain;
          }
          delay(20); 

          int slice = 20;
          int i;
          int average[slice];
          for (i = 0; i < slice; i = i + 1) {
            average[i] = (int)GetBearing();
          }
          float bearing = mode(average, slice);
          float heading = GetHeading(lat1, long1, lat2, long2);
          bearing = RealBearing(lat1, long1, bearing);
          RudderTurn(RudderAngle(bearing, heading));

          //Serial.print(lat1,8);
          //Serial.print("    ");
          //Serial.print(long1,8);
          //Serial.print("    ");
          //Serial.print(heading);
          //Serial.print("          ");
          //Serial.print(gps.speed.mps());
          //Serial.print("            ");
          //Serial.print("cyan");
          //Serial.print("            ");
          //Serial.print("none");
          //Serial.println();
        }
        //delay(500);
      }
      digitalWrite(lock, LOW);
    }
  }
  return;
}


static void RudderTurn(int angle){
  otherTime = millis();
  while (angle > prevangle) {
    prevangle++;
    rudder.write(prevangle); 
    delay(rudderspeed);  
  }
  while (angle < prevangle) {
    prevangle--;
    rudder.write(prevangle); 
    delay(rudderspeed);  
  }
  otherTime = millis() - otherTime;
  if (otherTime < 800) {
    delay(800 - otherTime);
  }
  prevangle = angle;
}
static float GetPosition(float lat1, float long1, float distance, float heading, int option)
{
  //Current Latitude, Current Longitude, Distance in meters, Current bearing in degrees
  //An option, 1 equals Latitude, 2 (or anything else) equals longitude
  distance = distance / 1000;
  lat1 = ConvertToRadians(lat1);
  long1 = ConvertToRadians(long1);
  heading = ConvertToRadians(heading);
  float lat2 = asin(sin(lat1) * cos(distance / r) +
    cos(lat1) * sin(distance / r) * cos(heading));
  float long2 = long1 + atan2(sin(heading) * sin(distance / r) * cos(lat1),
  cos(distance / r) - sin(lat1) * sin(lat2));
  if (option == 1)
  {
    return ConvertToDegrees(lat2);
  }
  else
  {
    return ConvertToDegrees(long2);
  }
}

static int RudderAngle(float bearing, float heading){
  heading = (int)heading % 360;
  bearing = (int)bearing % 360;
  if (heading == bearing)
  {
    return 90;
  }
  int o1 = (int)heading - (int)bearing;
  int o2 = (int)bearing - (int)heading;
  int dir = ((360-((int)heading - (int)bearing)) % 360);
  if (dir < 180) {
    if (dir > 30)
    {
      return 60;
    }
    else
    {
      return 90 - dir / 2;
    }
  }
  else if (dir >= 180)
  {
    if ((360 - dir) > 30)
    {
      return 120;
    }
    else
    {
      return 90 + (360 - dir) / 2;
    }
  }
  return 90;
}

static float GetBearing(){
  int i;
  int average[compassavg];
  for (i = 0; i < compassavg; i = i + 1) {
    MagnetometerRaw raw = compass.ReadRawAxis();
    MagnetometerScaled scaled = compass.ReadScaledAxis();
    int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)
    float heading = atan2(scaled.YAxis, scaled.XAxis);
    if(heading < 0)
      heading += 2*Pi;
    if(heading > 2*Pi)
      heading -= 2*Pi;
    float headingDegrees = (heading * 180/M_PI); 
    headingDegrees = fmod((headingDegrees + compasstrim),360);
    average[i] = (int)headingDegrees;
  }
  return mode(average, compassavg);
}


static float RealBearing(float lat1, float long1, float bearing)
{
  //Current Latitude, Current Longitude, Current bearing in degrees
  //magnetic north declanation function
  //75.7667Ã‚Â° N, 99.7833Ã‚Â° W North Magnetic Pole, Coordinates
  //+ or - ?
  float magdif = GetHeading(lat1, long1, 75.7667, -99.7833);
  return (fmod((bearing + magdif),360));
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
static float GetDistanceInM(float lat1, float long1, float lat2, float long2)
{
  //Current Latitude, Current Longitude
  //Projected Latitude, Projected Longitude
  lat1 = ConvertToRadians(lat1);
  lat2 = ConvertToRadians(lat2);
  long1 = ConvertToRadians(long1);
  long2 = ConvertToRadians(long2);
  float dLat = lat2 - lat1;
  float dLon = long2 - long1;
  float a = haversin(dLat) + cos(lat1) * cos(lat2) * haversin(dLon);
  float c = 2 * atan2(sqrt(a), sqrt(1 - a));
  return r * c * 1000; // Distance in M
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

static void RoutineDone(){
  RudderTurn(0);
  delay(100);
  RudderTurn(180);
  delay(100);
}

























