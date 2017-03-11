unsigned long delta;
uint32_t text[2];
uint32_t key[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delta = 0x9e3779b9;
  Serial.println("Enter text(max 8 letters): ");
  getString(text);
  Serial.println("Left string: "+String(text[0]) + ", Right string: "+ String(text[1]));
  Serial.println("Enter 128bit secret key: ");
  getKey(key);
  
  encrypt(text, key);
  
  Serial.println("********Encrypted string: "+returnString(text));
  Serial.println("Encrypted string(bit values): "+String(text[0]) +", "+String(text[1]));
  
  decrypt(text, key);
  Serial.println("********Decrypted string: "+returnString(text));
  Serial.println("Decrypted string(bit values): "+String(text[0]) +", "+String(text[1]));
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void encrypt(uint32_t text[], uint32_t key[]){
  uint32_t subKey1 = key[0];
  uint32_t subKey2 = key[1];
  uint32_t subKey3 = key[2];
  uint32_t subKey4 = key[3];
  
  uint32_t l_text = text[0];
  uint32_t r_text = text[1];

  uint32_t sum=0; 

  for(int i=0; i<32;i++){
    sum+=delta;
    r_text += ((l_text << 4) + subKey1) ^ ((l_text >> 5) + subKey2) ^ (l_text + sum);
    
    l_text += ((r_text << 4) + subKey3) ^ ((r_text >> 5) + subKey4) ^ (r_text + sum);
    
  }
  text[0] = r_text; text[1] = l_text;
}

void decrypt(uint32_t text[], uint32_t key[]){
  uint32_t subKey1 = key[0];
  uint32_t subKey2 = key[1];
  uint32_t subKey3 = key[2];
  uint32_t subKey4 = key[3];
  
  uint32_t l_text = text[0];
  uint32_t r_text = text[1];

  uint32_t sum = delta << 5;

  for(int i=0;i<32;i++){
    r_text -= ((l_text << 4) + subKey3) ^ ((l_text >> 5) + subKey4) ^ (l_text+sum);

    l_text -= ((r_text << 4) + subKey1) ^ ((r_text >> 5) + subKey2) ^ (r_text+sum);
    sum-=delta;
  }

  text[0] = r_text;
  text[1] = l_text;
}

void getString(uint32_t text[]){      //To read a text from serial input
  
  uint32_t input=0;
  while(true){
  
    if(Serial.available()){
      String test;
      test = Serial.readString();
      int n = test.length();
      if(n<=5){
        for(int i=0;i<n-1;i++){
            uint32_t charac = byte(test[i]);
            input += (charac << (8*i));
        }
        text[0]=input;
        text[1]=0;
      }
      else if(n<=9) {
        for(int i=0;i<4;i++){
            uint32_t charac = byte(test[i]);
            input += (charac << (8*i));
        }
        text[0]=input;
        input=0;
        for(int i=0;i<n-5;i++){
            uint32_t charac = byte(test[i+4]);
            input += (charac << (8*i));
        }
        text[1]=input;
      }
      return;
    }
  }
}

String returnString(uint32_t text[]){   //To convert back a string from
  String plainText = "";                //an integer
  for(int i=0;i<4;i++){
    int charac = byte(text[0] >> (8*i));
    plainText = plainText + char(charac); 
  }
  if(text[1]!=0){
    for(int i=0;i<4;i++){
      int charac = byte(text[1] >> (8*i));
      plainText = plainText + char(charac); 
    }
  }
  return plainText;
}

void getKey(uint32_t key[]){              //To recieve Key from serial input
  
  uint32_t input=0;
  while(true){
  
    if(Serial.available()){
      String test;
      test = Serial.readString();
      int n = test.length();
      if(n<=17){
        int count1=0;
        int count2=0;
        for(int i=0;i<n-1;i++){
            uint32_t charac = byte(test[i]);
            input += (charac << (8*i));
            count1+=1; 
            if(count1==4){
              key[count2]=input;
              count2+=1;
              input=0;count1=0;
            }
        }
        input=0;
        for(count2;count2<4;count2++){
          key[count2]=input;
        }
      }
      return;
    }
  }
}

