#include <DHT.h>
#include <WiFi.h>
#include "PubSubClient.h"

#define DHTPIN 33
#define DHTTYPE DHT22
#define MAX_CARACTERES 100
#define BUZZER_PIN 32
#define PIN_TRIG 22
#define PIN_ECHO 21

DHT dht (DHTPIN, DHTTYPE);


//Conexão com o WiFI
const char *SSID = "Wokwi-GUEST";
const char *PWD = "";

//Definindo os tópicos
const char *myTopicTemperatura = "/Temperatura";
const char *myTopicUmidade = "/Umidade";
const char *myTopicDistancia = "/Distancia";
const char *myTopicAction = "/Action/alerta";

bool buzzerActive = false;
bool ledActive = false;

int LED0=(13);
long duration;
int distance;

//configurações de conexão MQTT
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
char *mqttServer = "mqtt-dashboard.com";
int mqttPort = 1883;

//função para conectar ao WiFi
void ConectaNoWiFi() {
  Serial.print("Conectando ao WiFi");
  WiFi.begin(SSID, PWD);
  //Caso tenha dificuldades em conectar, imprime um “.”
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  //Se conectado, imprime “Conectado”
  Serial.print("Conectado.");
}


//Realiza as configurações do MQTT
void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
}

//A função callback é responsável por receber as msgs. Toda vez que uma novoa msg chega, a função será ativada.  
void callback(char* topic, byte* payload, unsigned int length) {
  
  //Como todos os tópicos vão entrar nessa função, é interessante desenvolver um “if” para definir qual tópico você pretende ler.  
  if (strcmp(topic, myTopicAction)==0){
    Serial.println("Mensagem recebida do tópico “/Action/Alerta");
    //A informação chega em um vetor de bytes. Você pode imprimir direto, ou salvar em outra variável para realizar as conversões desejadas.  
    //Neste caso, está sendo realizado a impressão direta do vetor de bytes (payload) 
    char dadosChar[length];
    for (int i = 0; i < length; i++) {
        dadosChar[i] = ((char)payload[i]);
    }
    //a função atof() converte char em float. Para converter em int, pode ser utilizado a função atoi() 
    Serial.println(dadosChar[0]);

    if(dadosChar[0] == '1' || dadosChar[0] == '2'){ // Ligar alarme
    // Vefica se o buzzer está ativo (al arme ligado)
      if(!buzzerActive){
        // Ativa o alerta sonoro (caso de chuve forte)
        tone(BUZZER_PIN, 262); // Toca um tom de 262Hz por 0,250 segundos*/
        buzzerActive == true;
        digitalWrite(LED0, HIGH);
        if (!ledActive) {
          // Liga o LED
          digitalWrite(LED0, HIGH);
          // Atualiza a variável para indicar que o LED está ativo
          ledActive = true;
        }
              
      }
    }  
    // Verifica se o valor recebido é 0
    else if (dadosChar[0] == '0' || dadosChar[0] == '3') {
      // Desliga o buzzer
      noTone(BUZZER_PIN);
      if (ledActive) {
        // Desliga o LED
        digitalWrite(LED0, LOW);
        // Atualiza a variável para indicar que o LED está inativo
        ledActive = false;
      buzzerActive = false;
      }
    }
 
  }
}
//Realiza a conceção com o Broker MQTT
void conectaBrokerMQTT() {
  Serial.println("Conectando ao broker");
  //A função mqttClient.connected() verifica se existe uma conexão ativa.  Depende do Broker, a conexão pode se manter ativa, ou desativar a cada envio de msg.
  while (!mqttClient.connected()) {
    //Se entrou aqui, é porque não está conectado. Então será feito uma tentativa de conexão infinita, até ser conectado.
    Serial.println("Conectando ao Broker MQTT");
    //define o nome da ESP na conexão. Está sendo gerado um nome aleatório, para evitar ter duas ESPs com o mesmo nome. Neste caso, uma derrubaria a outra.
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    //Realiza a conexão com a função “mqttClient.connect”. Caso seja um sucesso, entra no if e imprime “Conectado ao Broker MQTT.”
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("Conectado ao Broker MQTT.");
    }
  }
}


void setup() {
  Serial.begin(9600);
  ConectaNoWiFi();
  setupMQTT();
  Serial.println("Sistem Monitoring Temperatura e Umidade");
  dht.begin();

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  //LED
  pinMode(LED0, OUTPUT);
  //ultrassonico
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
}

int distancia(){
  
  // Limpa o Trigger antes de enviar um pulso
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  
  // Envia um pulso de 10 microssegundos para iniciar a medição
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  // Lê a duração do Echo
  duration = pulseIn(PIN_ECHO, HIGH);
  
  // Calcula a distância em centímetros
  distance = duration * 0.034 / 2;
  Serial.print("Distância: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  delay(100); // Aguarda um pouco entre as medições
  return distance;
}

void loop() {

  //Verifica se a conexão está ativa, caso não esteja, tenta conectar novamente.
  if (!mqttClient.connected()) {
    conectaBrokerMQTT();
    //Define quais tópicos vão ser assinados, ou seja, toda vez que alguma informação chegar destes tópicos, receberemos a informação.
    //Está adicionado dentro deste if, pois se a cair a conexão com o broker, será possível reativa-la.
    mqttClient.subscribe(myTopicAction);
    //Define o nome da função que será o callback, ou seja, toda vez que uma novo msg chegar de um dos tópicos, ela será enviada para a função ‘callback’
    mqttClient.setCallback(callback);
  }

  char data_temp[MAX_CARACTERES];
  char data_umid[MAX_CARACTERES];
  char data_dist[MAX_CARACTERES];

  Serial.print("começarei a leitura dos sensores \n \n");
  delay(2000);
  // put your main code here, to run repeatedly:
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float distance = distancia();

  // Faz parte da conversão de String para inteiro, lê os caracteres de uma string para armazenar como inteiro
  sprintf(data_temp, "%.0f", t);
  sprintf(data_umid, "%.0f", h);
  sprintf(data_dist, "%.0f", distance);


  // Este comando realiza a publicação da variável "data", que nada mais é do que o valor dos sensores no tópico descrito
  mqttClient.publish(myTopicTemperatura, data_temp);
  mqttClient.publish(myTopicUmidade, data_umid);
  mqttClient.publish(myTopicDistancia, data_dist);


  //Impressão das informações do sensor DHT22
  Serial.print("Temperatura = ");
  Serial.print(t);
  Serial.println(" C°");
  Serial.print("Umidade = ");
  Serial.print(h);
  Serial.println("% \n");

  //realiza o sincronismo com o Broker, por exemplo, verifica se existem msgs para ler se estiver inscrito em algum tópico
  mqttClient.loop();

  delay(1000);
}
