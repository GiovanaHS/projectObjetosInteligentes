# projectObjetosInteligentes
i) Breve descrição do funcionamento e do uso:
Este projeto implementa um sistema de iluminação inteligente em ambientes industriais para otimizar o uso de energia com controle automático de lâmpadas. Usando o microcontrolador ESP32, o sistema lê a intensidade de luz ambiente com um sensor de luminosidade (LM393) e ajusta a intensidade das lâmpadas, reduzindo o consumo elétrico. O ESP32 controla as lâmpadas através de um dimmer (MC-8A) e envia dados de economia de energia para um servidor via protocolo MQTT para monitoramento remoto, com possibilidade de controle pela plataforma MQTT (Cedalo). Para reproduzir este projeto, é necessário configurar a IDE do Arduino para o ESP32 e um broker MQTT para comunicação e monitoramento dos dados.

ii) Software e Documentação do Código:
O código para o ESP32 utiliza C++ e a IDE Arduino, integrando as bibliotecas PubSubClient e WiFi para comunicação MQTT e conectividade com a rede. O ESP32 realiza leituras do sensor de luz LM393, ajustando automaticamente o brilho da lâmpada através de PWM conforme a luminosidade ambiente. Cada nível de luz é classificado e publicado em um tópico MQTT para monitoramento remoto, usando o servidor MQTT Cedalo. A reconexão ao broker MQTT e ao Wi-Fi é gerida automaticamente no loop principal do código.

iii) Descrição do Hardware:
ESP32: Microcontrolador com suporte Wi-Fi e Bluetooth, usado para controlar o sistema de iluminação com leitura de sensores e comunicação remota.
Sensor de Luz LM393: Responsável pela medição da intensidade da luz ambiente.
Dimmer MC-8A: Atuador utilizado para ligar e desligar as lâmpadas de acordo com a leitura do sensor de luminosidade.
Cedalo como Broker MQTT: Plataforma usada para a comunicação remota e monitoramento dos dados de economia de energia.

iv) Interfaces, Protocolos e Módulos de Comunicação:
O sistema integra o sensor LM393 e o dimmer MC-8A ao ESP32, que controla a intensidade da lâmpada com PWM. As medições do sensor de luz são publicadas em tópicos MQTT, permitindo ao servidor Cedalo receber dados em tempo real. A comunicação segue o protocolo MQTT, com tópicos que contêm o valor do sensor e a classificação da luz, como "Muito Brilhante", "Brilhante", e "Escuro". Este sistema permite controle remoto e monitoramento contínuo, facilitando intervenções de ajuste se necessário.

v) Comunicação e Controle via Internet:
A comunicação utiliza TCP/IP para a conexão com o broker MQTT no servidor Cedalo, transmitindo informações em tempo real sobre o consumo de energia e classificação da luz ambiente. O sistema é capaz de ajustar a intensidade da lâmpada automaticamente e, com a estrutura MQTT, permite tanto o monitoramento remoto quanto a possibilidade de comando à distância para controle de iluminação.
