


# Realização de controle/animação de LEDs utilizando Display, Joystick e PWM para controle de intensidade luminosa

Este repositório contém o projeto que visa explorar o funcionamento do Conversor Analógico Digital controlando valores que serão utilizados para produzir animações, 
como um ponto que navega por coordenadas x,y produzidas pelo joystick e controle de intensidade luminosa de LEDs variando o duty cycle de um sinal PWM.

## Componentes
- Microcontrolador Raspberry Pi Pico W.
- LED RGB
- Joystick
- Display OLED
- Botão

## Funcionamento

- O projeto captura as posições x,y lendo a leitura analógica produzida pelos potenciômetros do joystick, convertendo em uma informação digital de resolução de 12 bits.
- A leitura medida fará duas coisas:
  - Desenhará um quadrado 8x8 pixels no display OLED nas posições x,y correspondendo as dimensões do display.
  - Ligará o LED vermelho quando alterado o valor do eixo X, acendendo o LED quando movido o joystick tanto para a esquerda, quanto para a direita.
  - Ligará o LED azul quando alterado o valor do eixo Y, acendendo o LED quando movido o joystick tanto para cima, quanto para baixo.
  - O acionamento de ambos os LEDs (vermelho e azul) se darão com intensidade proporcional a distância x,y do centro, essa intensidade será produzida com o auxílio da variação de um sinal PWM
  - O acionamento do botão seleciona se a animação será reproduzida sozinha no display ou se em conjunto com o efeito luminoso
  - O acionamento do botão do joystick liga/desliga LED verde e adiciona um efeito de borda mais grossa/fina no display em conjunto com animação
  
## Instruções de uso

### Requisitos
- Ambiente de desenvolvimento **VS Code**.
- **Simulador Wokwi** para simulação do hardware.
- Kit de desenvolvimento **Pico SDK**.

### Passos para Execução
1. Clone este repositório:
   ```bash
   git clone https://github.com/andrelimatecnologias/tarefa_adc.git
   ```
2. Instale as dependências necessárias para o Pico SDK.
3. Importe o projeto no **VS Code** e configure o simulador Wokwi.
4. Compile o código
5. Inicialize a execução na placa BitDogLab
6. Manipule o joystick para visualizar o efeito luminoso dos LEDs e a animação produzida no display
7. Acione o botão A para que apenas a animação no display OLED seja notada com a alteração do joystick
8. Acione o botão do Joystick para ligar/desligar LED verde, bem como produzir o efeito de borda mais grossa/fina no display

### Observação
A prática pode ser realizada apenas na placa BitDogLab

## Vídeo de teste
Um vídeo mostrando o teste desta atividade pode ser acessado no link: (https://youtu.be/xGdZOfwsans)

## Autor do projeto:
André Souza Lima
