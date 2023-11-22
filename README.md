# Global-Solution-EDGE

Link: https://wokwi.com/projects/381488236442520577

Descrição do problema abordado

A garantia da guarda de medicamentos termolábeis refere-se ao desafio de manter a integridade e eficácia de medicamentos sensíveis à temperatura durante todo o processo de armazenamento e transporte. Medicamentos termolábeis são aqueles que podem ser danificados ou perder sua eficácia se expostos a variações extremas de temperatura.

Esse problema é especialmente crítico porque muitos medicamentos, como vacinas, insulina e alguns antibióticos, exigem condições específicas de temperatura para manter sua estabilidade e eficácia. Variações fora dessas faixas podem resultar em perda de potência ou até mesmo em danos à saúde do paciente.

Para garantir a guarda adequada de medicamentos termolábeis, é necessário implementar cadeias de frio eficientes, desde a fabricação até o ponto de uso. Isso envolve o uso de equipamentos de refrigeração e monitoramento constante da temperatura, além de treinamento adequado para profissionais envolvidos no manuseio e transporte desses medicamentos.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Visçao geral da solução proposta:

Nossa proposta foi construir um protótipo que irá medir a temperatura e umidade do ambiente, auxiliando assim na preservação de medicamentos termolábeis em suas condições ideais. Além disso, com nosso protótipo, tornaríamos não apenas acessível para empresas, mas também para a população em geral devido ao seu baixo custo de fabricação. Nosso protótipo não seria apenas essencial para manter esses medicamentos em casa, mas também seria perfeito para viagens longas de carro, avião ou moto (entrega).

Controle de Temperatura: O dispositivo permitirá ao usuário definir a temperatura desejada para o armazenamento do medicamento, garantindo condições ideais de conservação. 

Monitoramento em Tempo Real: Durante o transporte do medicamento, o dispositivo fornecerá ao usuário informações em tempo real sobre a temperatura do local de armazenamento, assegurando que o medicamento esteja sempre em estado adequado. 

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Primeiramente irei explicar a arquitetura Básica para projetos IOT, em seguida apresentar do que o projeto é composto e por último irei explicar o código passo a passo.

ARQUITETURA BÁSICA PARA PROJETOS DE IOT Aplicação atua como a face visível da aplicação, responsável pelo front-end e pelas ferramentas que interagem com dispositivos de IoT e os consumidores/provedores de contexto. Além disso, ela é a casa de inovações tecnológicas como algoritmos de machine learning, inteligência artificial, análises avançadas, dashboards e aplicativos móveis.

Back-end é o núcleo operacional da plataforma, onde residem elementos vitais como o Orion Context Broker, STH-Comet, IoT Agent MQTT, e o banco de dados NoSQL MongoDB. Esses componentes desempenham um papel fundamental no armazenamento e gerenciamento de entidades, registros, subscrições e dados históricos (time series). Para facilitar a comunicação, também encontramos o Eclipse-Mosquitto, um popular broker MQTT. Além desses componentes-chave, a Camada de Back-end é um hub para a integração de outros GEs oferecidos pela FIWARE Foundation e ferramentas de terceiros. Esta camada não só proporciona a infraestrutura robusta para suportar a troca de informações, mas também serve como a espinha dorsal para a construção de soluções complexas e escaláveis.

IoT (Internet of Things) é responsável por acomodar os dispositivos de IoT que estabelecem comunicação com o back-end através dos protocolos de comunicação MQTT ou HTTP/NGSIv2.

![image](https://github.com/GDPMg/Global-Solution-EDGE/assets/103905620/32dcec7b-7de6-4ee0-89d7-2a5c68eb6bba)

O Projeto é composto por: 1 ESP32, 1 Buzzer, 2 Leds, 1 DHT22, 1 LCD 16x2

![image](https://github.com/GDPMg/Global-Solution-EDGE/assets/103905620/035656d7-0c82-4716-b850-c31a1f87ca2f)

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Vou explicar as partes principais do código:

Includes de Bibliotecas:

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <Wire.h>
#include <LiquidCrystal.h>

Importa as bibliotecas necessárias para lidar com Wi-Fi, MQTT, o sensor DHT22, comunicação I2C (Wire), e controle de um display LCD.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Definições de Pinos:

#define DHT_PIN 15
#define RED_LED 17
#define GREEN_LED 16
#define BUZZER 18
#define LCD_RS 12
#define LCD_EN 27
#define LCD_D4 26
#define LCD_D5 25
#define LCD_D6 33
#define LCD_D7 32

Define os pinos a serem usados para o sensor DHT22, LEDs, buzzer e controle do LCD.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Definições de Tópicos MQTT e Identificação:

#define TOPICO_SUBSCRIBE    "/TEF/lamp104/cmd"
#define TOPICO_PUBLISH      "/TEF/lamp104/attrs"
#define TOPICO_PUBLISH_T    "/TEF/lamp104/attrs/temp"
#define TOPICO_PUBLISH_H    "/TEF/lamp104/attrs/humi"
#define ID_MQTT  "fiware_104"

Define os tópicos MQTT para subscrição e publicação, bem como a identificação única do cliente MQTT.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Configurações de Rede e MQTT:

const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";
const char* BROKER_MQTT = "46.17.108.113";
int BROKER_PORT = 1883;

Configurações para a rede Wi-Fi e informações sobre o broker MQTT.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Configuração e Inicialização:

void setup() {
    // Inicializações dos componentes e conexões.
}

Inicializações iniciais, como a configuração do sensor DHT22, LEDs, buzzer, LCD, conexão Wi-Fi e MQTT.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Declaração de Variáveis:

const int potPin = 34;
char msgBuffer[4];

Define uma constante potPin com o valor 34 e cria um array de caracteres msgBuffer com tamanho 4.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Verificação e Reconexão Wi-Fi/MQTT:

VerificaConexoesWiFIEMQTT();

Chama a função VerificaConexoesWiFIEMQTT() para garantir que as conexões Wi-Fi e MQTT estejam funcionando. Esta função é definida em algum lugar do código (não fornecido aqui).

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Leitura da Temperatura e Umidade:

float temperature = dht.getTemperature();
float humidity = dht.getHumidity();

Lê os valores de temperatura e umidade do sensor DHT22 e os armazena nas variáveis temperature e humidity.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Verificação do Sucesso da Leitura do Sensor:

if (!isnan(temperature) && !isnan(humidity)) {
    // Código dentro desta condição
}

Verifica se a leitura do sensor foi bem-sucedida usando a função isnan() (is not a number). Se for bem-sucedida, executa o código dentro desta condição.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Lógica dos LEDs e Buzzer:

if ((temperature >= 2 && temperature <= 8) && (humidity >= 60 && humidity <= 80)) {
    // Condições para o LED verde
    // ...
} else {
    // Condições para o LED vermelho
    // ...
}

Dependendo das condições de temperatura e umidade, acende os LEDs verde ou vermelho, ativa o buzzer e exibe mensagens no LCD.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Envio de Dados para o Broker MQTT:

char tempBuffer[6];
char humiBuffer[6];
dtostrf(temperature, 4, 2, tempBuffer);
dtostrf(humidity, 4, 2, humiBuffer);
MQTT.publish(TOPICO_PUBLISH_T, tempBuffer);
MQTT.publish(TOPICO_PUBLISH_H, humiBuffer);


