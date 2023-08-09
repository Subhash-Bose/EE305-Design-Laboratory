#define voltage A0
#define current A1
#define Relay 13

#define cal_time_start 3000
#define cal_time_end 8000
#define avg_len 100
#define cal_c 0.1
#define cal_v 0.8

int cal_vmax=0;
int cal_vmin=1023;
int cal_cmax=0;
int cal_cmin=1023;

int cal_vdiff;
int cal_cdiff;

int counter=0;

int vmax=0;
int vmin=1023;
int cmax=0;
int cmin=1023;

int vdiff;
int cdiff;

int varr[avg_len];
int carr[avg_len];

int OFF=1;
int senseCurrent;
int senseVoltage;


void setup(){
  Serial.begin(9600);
  pinMode(voltage,INPUT);
  pinMode(current,INPUT);
  pinMode(Relay,OUTPUT);
}

void loop(){

  senseVoltage=analogRead(voltage);
  senseCurrent=analogRead(current);

//  delay(1);
  if(millis()==0) showSim();
  if(millis()==cal_time_start)  showCalStart();
  if(millis()==cal_time_end)  showCalEnd();

//  Serial.print("Voltage : ");
//  Serial.print(senseVoltage);
//  Serial.print("\tCurrent: ");
//  Serial.println(senseCurrent);





  if(millis()>=cal_time_start && millis()<cal_time_end){
    calVal();
  }
  else if(millis()>=cal_time_end) {
    genVal();
  }

  if(millis()%3000==0 && millis()>=cal_time_end){
    showParam();
  }

  if(vdiff<cal_vdiff*cal_v && millis()>=cal_time_end ){
    if(millis()%1000==0)
    Serial.println("Circuit is OFF ");
    OFF=0;
    digitalWrite(Relay,HIGH);
  }
  else{
    if(millis()%3000==0 && millis()>=cal_time_end)
    Serial.println("Circuit is ON ");
    digitalWrite(Relay,LOW);
    OFF=1;
  } 

}
void showSim(){
    Serial.println("Simulation Started");
    delay(2);
}

void showCalStart(){
    Serial.println("Calibration Started");
    delay(2);
}
void showCalEnd(){
    Serial.println("Calibration Done");
    Serial.print("Calibrated Maximum & Minimum Voltage: ");
    Serial.print(cal_vmax);
    Serial.print(", ");
    Serial.println(cal_vmin);
    
    Serial.print("Calibrated Maximum & Minimum Current: ");
    Serial.print(cal_cmax);
    Serial.print(", ");
    Serial.println(cal_cmin);

    Serial.print("Voltage Difference: ");
    Serial.println(cal_vdiff);
    Serial.print("Current Difference: ");
    Serial.println(cal_cdiff);
}
void calVal(){
    cal_vmax=max(cal_vmax,senseVoltage);
    cal_cmax=max(cal_cmax,senseCurrent);
    
    cal_vmin=min(cal_vmin,senseVoltage);
    cal_cmin=min(cal_cmin,senseCurrent);
    
    cal_vdiff=cal_vmax-cal_vmin;
    cal_cdiff=cal_cmax-cal_cmin;
}
void genVal(){
    varr[counter]=senseVoltage;
    carr[counter]=senseCurrent;
    
    counter=(counter+1)%avg_len;

    vmax=maximum(varr);
    vmin=minimum(varr);
    
    cmax=maximum(carr);
    cmin=minimum(carr);

    vdiff=vmax-vmin;
    cdiff=cmax-cmin;
}
void showParam(){
    Serial.print("Vdiff: ");
    Serial.print(vdiff);
    Serial.print(" Cdiff: ");
    Serial.println(cdiff);
}
int maximum(int a[]){
  int i=0;
  int mm=0;
  for(;i<avg_len;i++){
    mm=max(mm,a[i]);
  }
  return mm;
}
int minimum(int a[]){
  int i=0;
  int mm=1023;
  for(;i<avg_len;i++){
    mm=min(mm,a[i]);
  }
  return mm;
}
