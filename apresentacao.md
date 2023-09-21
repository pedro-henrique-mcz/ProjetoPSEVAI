# GPS Utilizando Blynk e ESP8266
## Projetos de Sistemas Embarcados - VA 1 


Pedro Henrique de Souza Oliveira 

Engenharia da Computação

## Sobre projeto
Este projeto consiste na criação de um GPS simples que recebe informações continuas de elementos externos como satélites e informa ao usuario blynk da atual posição em latitude e longitude do arduino ao fazer uma requisição. Podemos resumir o fluxo do sistema em
1. Realiza a chamada ao arduino
2. Arduino responde a chamada com a sua atual localização
3. Informações da localização atual do arduino são enviadas ao usuario Blynk.

## O código
Abaixo está o código utilizado neste projeto.  
```
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Credenciais WiFi e Blynk
char ssid[] = "SEU_SSID";
char pass[] = "SUA_SENHA";
char auth[] = "SEU_TOKEN_BLYNK";

SoftwareSerial gpsSerial(3, 4);  // RX, TX
TinyGPSPlus gps;

BLYNK_WRITE(V1) {  //V1 é o pin virtual do botão
    if (param.asInt()) {  // se o botão for pressionado
        if (gps.location.isValid()) {
            Blynk.virtualWrite(V2, gps.location.lat());
            Blynk.virtualWrite(V3, gps.location.lng());
        } else {
            Blynk.virtualWrite(V2, "Erro");
            Blynk.virtualWrite(V3, "Erro");
        }
    }
}

void setup() {
    Serial.begin(9600);
    gpsSerial.begin(9600);
    Blynk.begin(auth, ssid, pass);
}

void loop() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }
    Blynk.run();
}

```

## Destrinchando o código
```
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
```
Aqui nos adicionamos as bibliotecas que vamos precisar, que seriam.
* **SoftwareSerial.h** - é utilizada para transformarmos portas digitais em portas de comunicação serial.
* **TinyGPS++.h** - vai nos permitir ter a informção de nosso módulo GPS de maneira mais consisa. 
* **ESP8266WiFi.h** - vai configurar nosso modulo Wi-Fi.
* **BlynkSimpleEsp8266.h** - irá configurar o Blynk para funcionar perfeitamente com nossa placa Wi-Fi ESP8266.

Apenas uma sequência de variáveis que usamos para ter acesso a interface Blynk.
```
char ssid[] = "SEU_SSID";
char pass[] = "SUA_SENHA";
char auth[] = "SEU_TOKEN_BLYNK";
```
SoftwareSerial é a biblioteca do arduino que dá a capacidade que qualquer porta digital seja usada para comunicação serial, ou seja  a comunicacao e feita bit a bit de forma serializada (Half-duplex). Isso e bastante útil quando temos poras RX e TX limitadas. 

Nesse trecho em questão, a porta 3(RX) recebe os dados do GPS, e a TX envia dados para o GPS. Assim e feito o fluxo de comunicação entre o Arduino e o GPS. Ambas as portas estão ligadas por um jumper para que o processo ocorra.

```
SoftwareSerial gpsSerial(3, 4);  // RX, TX
```
Aqui criamos um objeto do tipo TinyGPSPlus. O objetivo dessa objeto em questão é guardar e simplificar as informações trazidas do modulo GPS acoplado ao arduino, que vem em padrao NMEA, um padrao marítimo de certa complexidade e com uma quantida elevada de informações. O objeto e seus métodos tornam a informação acessível e mais facil de se trabalhar na nossa interface do Blynk.
```
TinyGPSPlus gps;
```

* *BLYNK_WRITE(V1)* e chamada ao apertar o botão em nossa interface blynk, ela é ligada ao widget de mesmo nome que pode ter varias formas, neste caso, é um botão. Ao ser acionada, ela informa que o botão trocou de estado, que pode estar ligado, ou desligado.
* *param* E um objeto automãtico da biblioteca blynk, se o *BLYNK_WRITE(V1)* informa que o widget em questão (o botão) trocou de estado, o objeto *param* armazena o atual estado do widget, e como se trata de um botão, ele so pode ter dois estados - ativo e desativo, por isso a função *asInt*.
* *gps.location.isValid()* é um método do objeto gps que retorna true ou false, sendo a sua condição a validade da posição informada, para evitar erros. Em caso de erro, os widgets em wquestão V2 e V3 retornam "erro" na tela. 
* *Blynk.virtualWrite(V2, gps.location.lat());* e *Blynk.virtualWrite(V3, gps.location.lng());* utilizam funções do Blynk e do objeto GPS para retornarem as atuais latitudes e longitudes para os widgets V2 e V3.

```
BLYNK_WRITE(V1) {  //V1 é o pin virtual do botão
    if (param.asInt()) {  // se o botão for pressionado
        if (gps.location.isValid()) {
            Blynk.virtualWrite(V2, gps.location.lat());
            Blynk.virtualWrite(V3, gps.location.lng());
        } else {
            Blynk.virtualWrite(V2, "Erro");
            Blynk.virtualWrite(V3, "Erro");
        }
    }
}
```

Aqui,  se iniciam as variãevis e as configurações gerais do sistema envolvendo as bibliotecas. a maioria configurada para 9600 bits/s.

```
void setup() {
    Serial.begin(9600);
    gpsSerial.begin(9600);
    Blynk.begin(auth, ssid, pass);
}
```
O loop da aplicação basicamente verifica se há dados nas portas seriais TX e RX do fluxo GPS/ARDUINO para serem lidos, caso haja, o objeto GPS os armazena e interpreta de maneira legivel, para que quando acionamros o botao na interface Blynk, ele nos responda a com as respectivas latitude e longitude do Arduino.




