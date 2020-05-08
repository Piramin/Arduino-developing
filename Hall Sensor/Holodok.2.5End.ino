
  //Описание необходимых переменных:
int Hall=7;                                   //Пин датчика Холла.
/*!!!*/ float Time[2][4]; int Num=4;          //Кол-во снимаемых значений для дальнейшей обработки(кол-во магнитов).Time[2][Num].
  int count[2]={0,0};                         //Счетчики значений.
  int HotNum[2]={0,0};                        //Отсчет кругов.
  bool KeyTrue=true;                          //Смена строк.
  bool key[2]={true,true};                    //Снятие одного значения в момент пролета магнита.
  //unsigned long TimeNow;                    //Поверочное значение
  float TimeHex;                              //Более приближенное значение времени.
  
  //Далее все переменные призваны ради графического интерфейса и поверок:
  bool Graph=true;
 /*!!!*/ int Status=5; float Circle[5][4]; float Delta[5][4];     //Circle[кол-во проверок][кол-во магнитов]=Circle[Status][Num]-время кругов каждого магнита// Delta[кол-во проверок][кол-во магнитов]=Delta[Status][Num]-время между магнитами соседними.
  int Line=0;float Sum=0;
  bool Dopusk[2]={false,false};
  //float 
  
void setup() 
{ pinMode(Hall,INPUT);
  Serial.begin(9600); }

void loop() {
//  Next:
  if(digitalRead(Hall)==LOW){
    if(key[0]==true)
      { 
        if (Graph==true) 
        { Serial.print("Колличество магнитов: ");
          for(int i=1;i<=Num;i++) {Serial.print("      ");Serial.print(i);}
          Serial.println(" ");Serial.println(" ");
          Serial.print("                         ");Serial.println("        Ожидание...");Serial.println(" ");
          Graph=false;} 
          
// TimeNow=millis();
        TimeHex=(millis())/pow(10,3);
//Serial.print("TimeNow(float): ") ;/*Serial.print(TimeNow) ;Serial.print("   ") */; Serial.println(TimeHex) ;
           
           if (count[1]==Num-1)
               {Time[count[0]][count[1]]=TimeHex; count[1]=0 ; }
           else 
               {Time[count[0]][count[1]]=TimeHex; count[1]++ ; } 
         HotNum[0]++;
         HotNum[1]++;
         
/*Serial.print(HotNum[0]);Serial.print("  ");Serial.println(HotNum[1]);
    if (count[0]==0){ Serial.print("               ");Serial.print(Time[count[0]][count[1]]);Serial.print("  ");Serial.println("0");}
     else {Serial.print("               ");Serial.print(Time[count[0]][count[1]]);Serial.print("  ");Serial.println("1");}
 Serial.println(" ");*/ 
      
       if (HotNum[0]==Num)
        { 
          if (KeyTrue==true) {count[0]=1 ;KeyTrue=false ;}
          else {count[0]=0 ;KeyTrue=true ;}
          HotNum[0]=0 ;
        }
        
       if (HotNum[1]==2*Num)
        {  for(int i=0;i<Num;i++) {Circle[Line][i]=Time[1][i]-Time[0][i];}
           for(int i=0;i<Num-1;i++) {Delta[Line][i]=Time[0][i+1]-Time[0][i];Delta[Line][i]=abs(Delta[Line][i]);}
           Delta[Line][Num-1]=Time[0][0]-Time[0][Num-1];Delta[Line][Num-1]=abs(Delta[Line][Num-1]);
          HotNum[1]=0 ;Line++;}
          
       if(Line==Status)
       { Line=0;
        for(int j=0;j<Num;j++){
         Sum=0;
         for(int i=0;i<Status;i++) {Sum=Circle[i][j]+Sum;}
         Circle[Status][j]=Sum/Status; //среднее значение кругов для каждого магнита(после 5 измерений,т.е. Status),эл-т матрицы Status
         }
         
       for(int j=0;j<Num;j++){
        Sum=0;
        for(int i=0;i<Status;i++){Sum=abs(Circle[Status][j]-Circle[i][j])+Sum;}
        if (Sum<=(0.01*Status)) {Line++;}
        }
        
         TimeHex=float(Line)/float(Num);
       if (TimeHex>=(float(Num-1)/float(Num))) {Dopusk[0]=true;}
       else{Dopusk[0]=false;}
Serial.print("Совпадение кругов: ");Serial.print(TimeHex*100);Serial.println("%"); 
         Line=0;
        for(int j=0;j<Num-1;j++) {
          for (int i=0;i<Status-1;i++){TimeHex=abs(Delta[i][j]-Delta[i+1][j]);
            if(0.01*float(Status)>=TimeHex) {Line++;}
          }
        }
        TimeHex=float(Line)/float((Status-1)*(Num-1));
        if (TimeHex>=float(Status-1)/float(Status)){Dopusk[1]=true;}
        else{Dopusk[1]=false;}
Serial.print("Совпадение времени датчика: ");Serial.print(TimeHex*100);Serial.println("%");Serial.println();   
        if (Dopusk[0]&&Dopusk[1])
        {
        Serial.print("Период[с.]:            ");
         for(int j=0;j<Num;j++){Serial.print("   ");Serial.print(Circle[Status][j]);}
        Serial.println(" ");
        Serial.print("Частота[мин.^(-1)]:    ");
         for(int j=0;j<Num;j++)
         { Circle[Status][j]=60/Circle[Status][j];
           Serial.print("   ");Serial.print(Circle[Status][j]);}
          Dopusk[0]=false;Dopusk[1]=false; 
         }
       else {Serial.print("Система нестабильна!Проверьте целостность стенда!");}
      Serial.println(" ");Serial.println(" ");
      Serial.print("                         ");Serial.println("        Ожидание...");Serial.println(" ");  
      Line=0;HotNum[0]=0;HotNum[1]=0;count[0]=0;count[1]=0;KeyTrue=true;
      
//      goto Next; 
       }
       key[0]=false;
       key[1]=true;}
   }
  else { if(key[1]==true){key[0]=true;key[1]=false;} }

}
