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
 
 
 
 A comunicação com o microcontrolador usa a interface I2C, por meio dos pinos SCL e SDA do sensor. Nos pinos XDA e XCL você pode ligar outros dispositivos I2C, como um magnetômetro por exemplo, e criar um sistema de orientação completo. A alimentação do módulo pode variar entre 3 e 5v, mas para melhores resultados e precisão recomenda-se utilizar 5v.
 
 # Resultado
 ![whatsapp image 2017-09-27 at 19 59 51](https://user-images.githubusercontent.com/32276018/30942055-f1853cc4-a3be-11e7-889b-7f6803a3b4a6.jpeg)
