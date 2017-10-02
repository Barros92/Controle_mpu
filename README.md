# Controle_mpu

# Introdução

O seu principal objetivo é integrar um conjunto de tecnologias de sensores, de sistemas de controle e atuadores para sensoriar o ambiente, determinar as melhores opções de ação e executar estas ações de forma mais segura e confiável do que poderia ser obtida por um condutor humano comum a partir de uma programação bem utilizada.

 # Componentes
 
 - Arduino uno
 - Sensor bluetooth
 - Módulo MPU6050
 - Kit carro 2WD
 - Ponte H(L298)

 # MPU
 O CI MPU6050, tem embutido um recurso chamado DMP (Digital Motion Processor), responsável por fazer cálculos complexos com os sensores e cujos dados podem ser usados para sistemas de reconhecimento de gestos, navegação (GPS), jogos e diversas outras aplicações.
 
 # Pinagem e endereçamento do MPU6050
 
 ![image](https://user-images.githubusercontent.com/32276018/31092850-21f7f246-a786-11e7-8489-2cb34bddbe24.png)
 
 A comunicação com o microcontrolador usa a interface I2C, por meio dos pinos SCL e SDA do sensor. Nos pinos XDA e XCL você pode ligar outros dispositivos I2C, como um magnetômetro por exemplo, e criar um sistema de orientação completo. A alimentação do módulo pode variar entre 3 e 5v, mas para melhores resultados e precisão recomenda-se utilizar 5v.
 O pino AD0 desconectado define que o endereço I2C do sensor é 0x68. Conecte o pino AD0 ao pino 3.3V do Arduino para que o endereço seja alterado para 0x69. Essa mudança permite que você tenha dois módulos MPU-6050 em um mesmo circuito, mas neste projeto utilizei somente 1.
 
  # Desenvolvimento Programação

1° linha Carrega a biblioteca Wire, depois carrega a biblioteca sinalizador de interrupção e endereco I2C do MPU6050 e variaveis para armazenar valores dos sensores.

![image](https://user-images.githubusercontent.com/32276018/31094336-33aebf24-a78b-11e7-9ab5-811000468268.png)

Início de transimição com MPU
![image](https://user-images.githubusercontent.com/32276018/31094604-2cdaa4aa-a78c-11e7-81b9-85679689f763.png)

 Esta matriz 3D contém o binário de banco de memória DMP padrão. No firmware Invensense UC3-A3, isso é carregado em transmissões de 128 bytes, mas o Arduino Wire a biblioteca suporta apenas transmissões de 32 bytes, incluindo o endereço de registro para o qual você está escrevendo,então eu grava-se isso em 16 cargas úteis de transmissão de bytes que são enviadas na função dmp_init ().Este foi reconstruído a partir do tráfego I2C  

 ![image](https://user-images.githubusercontent.com/32276018/31094030-097999aa-a78a-11e7-8272-25b2b96a8355.png)
 
 Transmissões de atualização DMP (Banco, Endereço de Início, Comprimento de Atualização, Dados de Atualização ...)

 ![image](https://user-images.githubusercontent.com/32276018/31093927-b02259a0-a789-11e7-8c23-647e8abc1458.png)

 
 # Resultado
 ![whatsapp image 2017-09-27 at 19 59 51](https://user-images.githubusercontent.com/32276018/30942055-f1853cc4-a3be-11e7-889b-7f6803a3b4a6.jpeg)
