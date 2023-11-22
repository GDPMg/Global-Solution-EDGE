#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h> // Adiciona a biblioteca do sensor DHT22
#include <Wire.h>
#include <LiquidCrystal.h>

 
// Define o pino ao qual o sensor DHT22 está conectado
#define DHT_PIN 15
#define RED_LED 17    // Pino do LED vermelho
#define GREEN_LED 16  // Pino do LED verde
#define BUZZER 18     // Pino do buzzer

// Define os pinos de controle do LCD
#define LCD_RS 12
#define LCD_EN 27
#define LCD_D4 26
#define LCD_D5 25
#define LCD_D6 33
#define LCD_D7 32

 
//defines:
//defines de id mqtt e tópicos para publicação e subscribe denominado TEF(Telemetria e Monitoramento de Equipamentos)
#define TOPICO_SUBSCRIBE    "/TEF/lamp104/cmd"        //tópico MQTT de escuta
#define TOPICO_PUBLISH      "/TEF/lamp104/attrs"      //tópico MQTT de envio de informações para Broker
#define TOPICO_PUBLISH_T    "/TEF/lamp104/attrs/temp"    //tópico MQTT de envio de informações para Broker
#define TOPICO_PUBLISH_H    "/TEF/lamp104/attrs/humi"    //tópico MQTT de envio de informações para Broker
                                                      //IMPORTANTE: recomendamos fortemente alterar os nomes
                                                      //            desses tópicos. Caso contrário, há grandes
                                                      //            chances de você controlar e monitorar o ESP32
                                                      //            de outra pessoa.
#define ID_MQTT  "fiware_104"   //id mqtt (para identificação de sessão)
                                 //IMPORTANTE: este deve ser único no broker (ou seja,
                                 //            se um client MQTT tentar entrar com o mesmo
                                 //            id de outro já conectado ao broker, o broker
                                 //            irá fechar a conexão de um deles).
                                 // o valor "n" precisa ser único!
                               
 
// WIFI
const char* SSID = "Wokwi-GUEST"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = ""; // Senha da rede WI-FI que deseja se conectar
 
// MQTT
const char* BROKER_MQTT = "46.17.108.113"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT
 
int D4 = 2;
 
//Variáveis e objetos globais
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
char EstadoSaida = '0';  //variável que armazena o estado atual da saída
 
//Prototypes
void initSerial();
void initWiFi();
void initMQTT();
void reconectWiFi();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);

 
DHTesp dht; // Cria um objeto DHT
int redLedState = LOW;   // Estado inicial do LED vermelho
int greenLedState = HIGH;  // Estado inicial do LED verde
 
unsigned long previousBuzzTime = 0;  // Variável para rastrear o tempo desde o último som do buzzer
const long buzzerInterval = 1000;  // Intervalo desejado para o som do buzzer (em milissegundos)
 
// Objeto LCD
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Variáveis globais para armazenar os valores da temperatura e umidade
float temperature = 0.0;
float humidity = 0.0;

// Variáveis globais para armazenar os buffers de mensagem
char tempBuffer[6];
char humiBuffer[6];
 
void setup()
{
    ledcSetup(0, 1000, 8);  // Configura o canal 0 com frequência de 1000 Hz e resolução de 8 bits
    ledcAttachPin(BUZZER, 0);  // Associa o pino BUZZER ao canal 0
    //inicializações:
    initSerial();
    initWiFi();
    initMQTT();
    delay(5000);
    MQTT.publish(TOPICO_PUBLISH, "s|off");
 
    // Inicializa os LEDs
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
 
    // Inicializa o buzzer
    pinMode(BUZZER, OUTPUT);
    noTone(BUZZER);  // Garante que o buzzer esteja inicialmente em silêncio
 
    // Inicializa o sensor DHT22
    dht.setup(DHT_PIN, DHTesp::DHT22);

    // Inicializa o LCD
    lcd.begin(16, 2);  // 16 colunas, 2 linhas
    lcd.clear();
 
}
 
 
//Função: inicializa comunicação serial com baudrate 115200 (para fins de monitorar no terminal serial
//        o que está acontecendo.
//Parâmetros: nenhum
//Retorno: nenhum
void initSerial()
{
    Serial.begin(115200);
}
 
//Função: inicializa e conecta-se na rede WI-FI desejada
//Parâmetros: nenhum
//Retorno: nenhum
void initWiFi()
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
     
    reconectWiFi();
}
 
//Função: inicializa parâmetros de conexão MQTT(endereço do
//        broker, porta e seta função de callback)
//Parâmetros: nenhum
//Retorno: nenhum
void initMQTT()
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}
 
//Função: função de callback
//        esta função é chamada toda vez que uma informação de
//        um dos tópicos subescritos chega)
//Parâmetros: nenhum
//Retorno: nenhum
void mqtt_callback(char* topic, byte* payload, unsigned int length)
{
    String msg;
     
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++)
    {
       char c = (char)payload[i];
       msg += c;
    }
   
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);
   
}
 
//Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
//        em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
//Parâmetros: nenhum
//Retorno: nenhum
void reconnectMQTT()
{
    while (!MQTT.connected())
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT))
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE);
        }
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}
 
//Função: reconecta-se ao WiFi
//Parâmetros: nenhum
//Retorno: nenhum
void reconectWiFi()
{
    //se já está conectado a rede WI-FI, nada é feito.
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
         
    WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
     
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    }
   
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}
 
//Função: verifica o estado das conexões WiFI e ao broker MQTT.
//        Em caso de desconexão (qualquer uma das duas), a conexão
//        é refeita.
//Parâmetros: nenhum
//Retorno: nenhum
void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected())
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
     
     reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}

//programa principal
void loop()
{  
    const int potPin = 34;
   
    char msgBuffer[4];
    //garante funcionamento das conexões WiFi e ao broker MQTT
    VerificaConexoesWiFIEMQTT();
 
    // Leitura da temperatura e umidade
    float temperature = dht.getTemperature();
    float humidity = dht.getHumidity();
 
    // Verifica se a leitura do sensor foi bem-sucedida
    if (!isnan(temperature) && !isnan(humidity)) {
        Serial.print("Temperatura: ");
        Serial.print(temperature);
        Serial.print(" °C | Umidade: ");
        Serial.print(humidity);
        Serial.println(" %");
 
 
        // Lógica dos LEDs
        if ((temperature >= 2 && temperature <= 8) && (humidity >= 60 && humidity <= 80)) {
            // Condições para o LED verde
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(RED_LED, LOW);
            greenLedState = HIGH;
            redLedState = LOW;

            lcd.clear();
            lcd.print("Temp OK");
            lcd.setCursor(0, 1);
            lcd.print("Temp: ");
            lcd.print(temperature);
            delay(3000);
            lcd.clear();
            lcd.print("Umidade OK");
            lcd.setCursor(0, 1);
            lcd.print("Hum: ");
            lcd.print(humidity);
            delay(3000);

            // Desliga o buzzer
            noTone(BUZZER);
 
        } else {
          // Condições para o LED vermelho
          digitalWrite(GREEN_LED, LOW);
          digitalWrite(RED_LED, HIGH);
          greenLedState = LOW;
          redLedState = HIGH;

      
          // Aciona o buzzer a cada 2 segundos
          unsigned long currentMillis = millis();
          if (currentMillis - previousBuzzTime >= buzzerInterval) {
              previousBuzzTime = currentMillis;
              tone(BUZZER, 1000);  // Frequência do som do buzzer
              delay(500);  // Tempo de duração do som (ajuste conforme necessário)
              noTone(BUZZER);  // Desliga o buzzer
          }
    
          lcd.clear();
          lcd.print("Temp: ");
          lcd.print(temperature);
          lcd.setCursor(0, 1);
          lcd.print("Hum: ");
          lcd.print(humidity);

          if (temperature >= 2 && temperature <= 8) {
            lcd.clear();
            lcd.print("Temp OK");
            lcd.setCursor(0, 1);
            lcd.print("Temp: ");
            lcd.print(temperature);
            delay(3000);
          }

          if (humidity >= 60 && humidity <= 80) {
            lcd.clear();
            lcd.print("Umidade OK");
            lcd.setCursor(0, 1);
            lcd.print("Hum: ");
            lcd.print(humidity);
            delay(3000);
          }   
    
          if (temperature > 8) {
            lcd.clear();
            lcd.print("Temp Alta");
            lcd.setCursor(0, 1);
            lcd.print("Temp: ");
            lcd.print(temperature);
            delay(3000);
          }
    
          if (temperature < 2) {
            lcd.clear();
            lcd.print("Temp Baixa");
            lcd.setCursor(0, 1);
            lcd.print("Temp: ");
            lcd.print(temperature);
            delay(3000);
          }
    
          if (humidity > 80) {
            lcd.clear();
            lcd.print("Umidade Alta");
            lcd.setCursor(0, 1);
            lcd.print("Hum: ");
            lcd.print(humidity);
            delay(3000);
          }
    
          if (humidity < 60) {
            lcd.clear();
            lcd.print("Umidade Baixa");
            lcd.setCursor(0, 1);
            lcd.print("Hum: ");
            lcd.print(humidity);
            delay(3000);
          }       
        }
 
        // Envie os valores de temperatura e umidade para o Broker MQTT
        char tempBuffer[6];
        char humiBuffer[6];
        dtostrf(temperature, 4, 2, tempBuffer);
        dtostrf(humidity, 4, 2, humiBuffer);
 
       
        MQTT.publish(TOPICO_PUBLISH_T, tempBuffer);
        MQTT.publish(TOPICO_PUBLISH_H, humiBuffer);
    }

    //keep-alive da comunicação com broker MQTT
    MQTT.loop();
}