# Global-Solution-EDGE

Descrição do problemaabordado

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

https://user-images.githubusercontent.com/103905620/282064942-ea31ee72-ae7e-41e7-8931-a95928bb2371.png

