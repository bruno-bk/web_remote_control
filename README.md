# Controle Remoto Via Interface Web

Este é um projeto simples que demonstra o controle de motores usando uma 
interface web interativa. O projeto é baseado em C++, HTML, CSS e JavaScript,
permitindo controlar a velocidade e a direção dos motores por meio de uma interface web.

## Pinout usado

| ESP32 | Ponte H |
| -- | -- |
| D25 | ENA |
| D26 | IN1 |
| D27 | IN2 |
| D14 | IN3 |
| D12 | IN4 |
| D13 | ENB |

## Como Usar

1. Clone ou faça o download deste repositório.

2. Abra o VS Code e instale a extensão PlatformIO, se ainda não a tiver instalada.

3. No VS Code, abra a pasta do projeto.

4. Abra o arquivo src/main.cpp para editar o código, altere o usuário e senha do wifi.

5. Conecte o seu ESP32 ao computador usando um cabo USB.

6. Clique no ícone de "Upload" na barra inferior do VS Code para compilar e enviar o
código para o ESP32.

7. O PlatformIO irá compilar o código e transferi-lo para o seu ESP32. Você verá
mensagens de status na barra inferior.

Após a conclusão do upload, acesse o endereço IP atribuído ao ESP32 na sua rede
local (você pode verificar o IP no monitor serial), utilizando um navegador. Isso
abrirá a interface web de controle.

## Bugs conhecido

- O código não foi integrado para receber comandos simultâneos de ambos os círculos.

## Contribuição

Este projeto é apenas um exemplo básico e pode ser expandido de várias maneiras.
Sinta-se à vontade para contribuir com novos recursos, melhorias de interface,
ou qualquer outra funcionalidade que você desejar adicionar.
