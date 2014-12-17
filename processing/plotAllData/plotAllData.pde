/**
 * Show GY521 Data.
 * 
 * Reads the serial port to get x- and y- axis rotational data from an accelerometer,
 * a gyroscope, and comeplementary-filtered combination of the two, and displays the
 * orientation data as it applies to three different colored rectangles.
 * It gives the z-orientation data as given by the gyroscope, but since the accelerometer
 * can't provide z-orientation, we don't use this data.
 * 
 */
 
import processing.serial.*;
Serial  myPort;
short   portIndex = 1;
int     lf = 10;       //ASCII linefeed
String  inString;      
int     calibrating;
int     dt;
float   x_gyr;  //Gyroscope data
float   y_gyr;
float   z_gyr;
float   x_acc;  //Accelerometer data
float   y_acc;
float   z_acc;
float   x_fil;  //Filtered data
float   y_fil;
float   z_fil;

int     count = 0;
int     numdata = 400;
float[] xAcc = new float[numdata];
float[] yAcc = new float[numdata];
float[] zAcc = new float[numdata];
float[] xGyr = new float[numdata];
float[] yGyr = new float[numdata];
float[] zGyr = new float[numdata];
float[] xAng = new float[numdata];
float[] yAng = new float[numdata];
float[] zAng = new float[numdata];

Table logging;
void setup()  { 
  size(1000, 650);
  stroke(1,1,1);
  colorMode(RGB, 256); 
  String portName = "COM14";
  myPort = new Serial(this, portName, 19200);
  myPort.clear();
  myPort.bufferUntil(lf);
  clearArray();
  
  logging = new Table();
  
  logging.addColumn("id");
  
  logging.addColumn("AccX");
  logging.addColumn("AccY");
  logging.addColumn("AccZ");
  
  logging.addColumn("GryX");
  logging.addColumn("GryY");
  logging.addColumn("GryZ");
  
  logging.addColumn("FilX");
  logging.addColumn("FilY");
  logging.addColumn("FilZ");
} 

void draw()  { 
  
  background(255);
  textSize(26);
  fill(#FF0000);
  text("X-Axis", 0, 30);
  fill(#00FF00);
  text("Y-Axis", 0, 60);
  fill(#0000FF);
  text("Z-Axis", 0, 90);
  fill(#000000);
  textSize(15);
  stroke(222);
  
  //plot gyr data
  pushMatrix();
  translate(150, height/2);
  stroke(0);
  text("Gyr",-25, 0);
  line(0, -height/2, 0, height/2);
  line(0, 0, 1200-150, 0);
  for(int i=1; i<numdata; i++){
    if(i<count){
      stroke(255, 0, 0);
      line(i*2, xGyr[i-1], (i+1)*2, xGyr[i]);
      stroke(0, 255, 0);
      line(i*2, yGyr[i-1], (i+1)*2, yGyr[i]);
      stroke(0, 0, 255);
      line(i*2, zGyr[i-1], (i+1)*2, zGyr[i]);
    }else{
      break;
    }
  }
  popMatrix();
  
  //plot acc data
  pushMatrix();
  translate(150, height/4); 
  stroke(#000000);
  text("Acc", -25, 0);
  line(0, 0, 1200-150, 0);
  for(int i=1; i<numdata; i++){
    if(i<count){
      stroke(255, 0, 0);
      line(i*2, xAcc[i-1], (i+1)*2, xAcc[i]);
      stroke(0, 255, 0);
      line(i*2, yAcc[i-1], (i+1)*2, yAcc[i]);
      stroke(0, 0, 255);
      line(i*2, zAcc[i-1], (i+1)*2, zAcc[i]);
    }else{
      break;
    }
  }
  popMatrix();
  
  //plot angle fill data
  pushMatrix();
  translate(150, 3*height/4); 
  stroke(#000000);
  text("Pro", -25, 0);
  line(0, 0, 1200-150, 0);
  for(int i=1; i<numdata; i++){
    if(i<count){
      stroke(255, 0, 0);
      line(i*2, xAng[i-1], (i+1)*2, xAng[i]);
      stroke(0, 255, 0);
      line(i*2, yAng[i-1], (i+1)*2, yAng[i]);
      stroke(0, 0, 255);
      line(i*2, zAng[i-1], (i+1)*2, zAng[i]);
    }else{
      break;
    }
  }
  popMatrix();
} 

void serialEvent(Serial p) {
  inString = (myPort.readString());
  println(inString);
  try {
    // Parse the data
    String[] dataStrings = split(inString, '#');
    for (int i = 0; i < dataStrings.length; i++) {
      String type = dataStrings[i].substring(0, 4);
      String dataval = dataStrings[i].substring(4);
//      if (type.equals("DEL:")) {
//        dt = int(float(dataval)*1000);
//      } else 
      if (type.equals("ACC:")) {
        String data[] = split(dataval, ',');
        x_acc = float(data[0]);
        y_acc = float(data[1]);
        z_acc = float(data[2]);
      } else if (type.equals("GYR:")) {
        String data[] = split(dataval, ',');
        x_gyr = float(data[0]);
        y_gyr = float(data[1]);
        z_gyr = float(data[2]);
      } else if (type.equals("FIL:")) {
        String data[] = split(dataval, ',');
        x_fil = float(data[0]);
        y_fil = float(data[1]);
        z_fil = float(data[2]);
      }
    }
    
    if(count == numdata){
      clearArray();
    }
    xAcc[count] = -2*98*x_acc/32768;
    yAcc[count] = -2*98*y_acc/32768;
    zAcc[count] = -2*98*z_acc/32768;
    
    xGyr[count] = -x_gyr/(131.0*5);
    yGyr[count] = -y_gyr/(131.0*5);
    zGyr[count] = -z_gyr/(131.0*5);
    
    xAng[count] = -x_fil;
    yAng[count] = -y_fil;
    zAng[count] = -z_fil%90;
    count++;
  } catch (Exception e) {
      println("Caught Exception");
  }
}

void clearArray(){
  count = 0;
  for(int i=0; i<numdata; i++){
    xAcc[i] = 0.0;
    yAcc[i] = 0.0;
    zAcc[i] = 0.0;
    
    xGyr[i] = 0.0;
    yGyr[i] = 0.0;
    zGyr[i] = 0.0;
    
    xAng[i] = 0.0;
    yAng[i] = 0.0;
    zAng[i] = 0.0;
  }
}


